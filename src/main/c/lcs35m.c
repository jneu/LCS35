#include "lcs35.h"
#include "print_challenge_message.h"

/*
 * The number of bits to use in the calculations
 */

#define R_NUM_BITS 2048
#define R_NUM_BYTES (R_NUM_BITS / 8)
#define R_NUM_LIMBS (R_NUM_BYTES / sizeof (mp_limb_t))

#if (0 != (R_NUM_BITS % 8))
#error something is wrong with the bits
#endif


int
main (void)
{
  /*
   * r is a power of 2 strictly larger than n
   * r * r_inv = 1 mod n
   * n * n_prime = -1 mod r
   */

  bool rv;
  mpz_t n, z, w, message;
  mpz_t r, r_inv, n_prime;
  mp_limb_t *p_tmp2, *p_tmp3, *p_tmp4;
  const mp_limb_t *p_tmp1, *p_n, *p_n_prime;
  mpz_t final;
  uint64_t t;

  /* Initializations */
  assert (0 == (R_NUM_BYTES % sizeof (mp_limb_t)));
  assert (R_NUM_LIMBS > 0);

  mpz_inits (n, z, w, message, NULL);
  mpz_inits (r, r_inv, n_prime, NULL);

  memset (mpz_limbs_modify (n, R_NUM_LIMBS), 0, R_NUM_BYTES);

  rv = (0 == mpz_set_str (n, N, 10));
  ASSERT_FATAL (rv, "failed to set n");
  rv = (0 == mpz_set_str (z, Z, 10));
  ASSERT_FATAL (rv, "failed to set z");

  t = T;
  mpz_set_ui (w, 2);

  mpz_ui_pow_ui (r, 2, R_NUM_BITS);
  if (mpz_cmp (n, r) >= 0)
    {
      fputs ("not enough bits\n", stderr);
      exit (EXIT_FAILURE);
    }

  rv = (0 != mpz_invert (r_inv, r, n));
  if (!rv)
    {
      puts ("r is not invertiable mod n");
      exit (EXIT_FAILURE);
    }

  mpz_invert (n_prime, n, r);
  mpz_mul_si (n_prime, n_prime, -1);
  mpz_mod (n_prime, n_prime, r);

  mpz_mul (w, w, r);
  mpz_mod (w, w, n);

  rv = (0 == posix_memalign ((void **) &p_tmp2, 64, 2 * R_NUM_BYTES));
  ASSERT_FATAL (rv, "failed to allocate p_tmp2");
  rv = (0 == posix_memalign ((void **) &p_tmp3, 64, 2 * R_NUM_BYTES));
  ASSERT_FATAL (rv, "failed to allocate p_tmp3");
  rv = (0 == posix_memalign ((void **) &p_tmp4, 64, 2 * R_NUM_BYTES));
  ASSERT_FATAL (rv, "failed to allocate p_tmp4");

  memset (p_tmp2, 0, 2 * R_NUM_BYTES);
  memset (p_tmp3, 0, 2 * R_NUM_BYTES);
  memset (p_tmp4, 0, 2 * R_NUM_BYTES);

  p_tmp1 = mpz_limbs_read (w);
  p_n = mpz_limbs_read (n);
  p_n_prime = mpz_limbs_read (n_prime);

  /* Get going */
  while (t-- > 0)
    {
      mpn_sqr (p_tmp2, p_tmp1, R_NUM_LIMBS);

      mpn_mul_n (p_tmp3, p_tmp2, p_n_prime, R_NUM_LIMBS);
      mpn_mul_n (p_tmp4, p_tmp3, p_n, R_NUM_LIMBS);
      mpn_add_n (p_tmp3, p_tmp4, p_tmp2, 2 * R_NUM_LIMBS);

      if (0 != mpn_sub_n (p_tmp4, p_tmp3 + R_NUM_LIMBS, p_n, R_NUM_LIMBS))
        {
          p_tmp1 = p_tmp3 + R_NUM_LIMBS;
        }
      else
        {
          p_tmp1 = p_tmp4;
        }

    }

  mpz_mul (w, mpz_roinit_n (final, p_tmp1, R_NUM_LIMBS), r_inv);
  mpz_mod (w, w, n);

  /* Create and show the message */
  mpz_xor (message, z, w);
  print_challenge_message (message);

  /* Clean up */
  free (p_tmp4);
  free (p_tmp3);
  free (p_tmp2);
  mpz_clears (r, r_inv, n_prime, NULL);
  mpz_clears (n, z, w, message, NULL);

  return EXIT_SUCCESS;
}
