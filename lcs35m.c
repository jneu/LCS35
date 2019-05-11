#include "lcs35.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>

/*
 * Pick the number of bits to use in the calculations
 */

#define R_NUM_BITS 2048
#define R_NUM_BYTES (R_NUM_BITS / 8)
#define R_NUM_LIMBS (R_NUM_BYTES / sizeof (mp_limb_t))

int
main (void)
{
  /*
   * r is a power of 2 strictly larger than n
   * r * r_inv = 1 mod n
   * n * n_prime = -1 mod r
   */

  int rv;
  mpz_t n, z, w;
  mpz_t r, r_inv, n_prime;
  mp_limb_t *p_tmp2, *p_tmp3, *p_tmp4;
  const mp_limb_t *p_tmp1, *p_n, *p_n_prime;
  mpz_t final;
  uint64_t t = T;

  /* Initializations */
  assert (0 == (R_NUM_BITS % 8));
  assert (0 == (R_NUM_BYTES % sizeof (mp_limb_t)));
  assert (R_NUM_LIMBS > 0);

  mpz_inits (n, z, w, NULL);
  mpz_inits (r, r_inv, n_prime, NULL);

  memset (mpz_limbs_modify (n, R_NUM_LIMBS), 0, R_NUM_BYTES);

  rv = mpz_set_str (n, N, 10);
  CHECK_RV (rv, "failed to set n\n");
  rv = mpz_set_str (z, Z, 10);
  CHECK_RV (rv, "failed to set z\n");

  mpz_set_ui (w, 2);

  mpz_ui_pow_ui (r, 2, R_NUM_BITS);
  assert (mpz_cmp (n, r) < 0);

  rv = mpz_invert (r_inv, r, n);
  CHECK_RV0 (rv, "r in not invertible mod n\n");

  mpz_invert (n_prime, n, r);
  mpz_mul_si (n_prime, n_prime, -1);
  mpz_mod (n_prime, n_prime, r);

  mpz_mul (w, w, r);
  mpz_mod (w, w, n);

  p_tmp2 = aligned_alloc (64, 2 * R_NUM_BYTES);
  p_tmp3 = aligned_alloc (64, 2 * R_NUM_BYTES);
  p_tmp4 = aligned_alloc (64, 2 * R_NUM_BYTES);
  memset (p_tmp2, 0, 2 * R_NUM_BYTES);
  memset (p_tmp3, 0, 2 * R_NUM_BYTES);
  memset (p_tmp4, 0, 2 * R_NUM_BYTES);

  p_tmp1 = mpz_limbs_read (w);
  p_n = mpz_limbs_read (n);
  p_n_prime = mpz_limbs_read (n_prime);

  /* Get going */
  while (t-- > 0)
    {
      mpz_mod (w, w, n);

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
  mpz_xor (w, w, z);

  printf ("message:\n");
  mpz_out_str (stdout, 10, w);
  printf ("\n");

  /* Clean up */
  free (p_tmp4);
  free (p_tmp3);
  free (p_tmp2);
  mpz_clears (r, r_inv, n_prime, NULL);
  mpz_clears (n, z, w, NULL);

  return EXIT_SUCCESS;
}
