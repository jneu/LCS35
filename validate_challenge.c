#include "lcs35.h"
#include "print_challenge_message.h"

static void
validate_challenge (mpz_t * n, mpz_t * p, mpz_t * q)
{
  int rv;
  mpz_t gcd, pq;

  mpz_inits (gcd, pq, NULL);

  if (0 != mpz_odd_p (*p))
    {
      printf ("p is odd\n");
    }
  else
    {
      printf ("p is even\n");
      exit (EXIT_FAILURE);
    }

  if (0 != mpz_odd_p (*q))
    {
      printf ("q is odd\n");
    }
  else
    {
      printf ("q is even\n");
      exit (EXIT_FAILURE);
    }

  mpz_gcd (gcd, *p, *q);
  if (0 == mpz_cmp_ui (gcd, 1))
    {
      printf ("p and q are relatively prime\n");
    }
  else
    {
      printf ("p and q are not relatively prime; their gcd is:\n");
      mpz_out_str (stdout, 10, gcd);
      printf ("\n");
      exit (EXIT_FAILURE);
    }

  rv = mpz_probab_prime_p (*p, NUM_PRIME_REPS);
  if (2 == rv)
    {
      printf ("p is definitely prime\n");
    }
  else if (1 == rv)
    {
      printf ("p is probably prime (%d reps)\n", NUM_PRIME_REPS);
    }
  else
    {
      printf ("p is definitely not prime\n");
      exit (EXIT_FAILURE);
    }

  rv = mpz_probab_prime_p (*q, NUM_PRIME_REPS);
  if (2 == rv)
    {
      printf ("q is definitely prime\n");
    }
  else if (1 == rv)
    {
      printf ("q is probably prime (%d reps)\n", NUM_PRIME_REPS);
    }
  else
    {
      printf ("q is definitely not prime\n");
      exit (EXIT_FAILURE);
    }

  mpz_mul (pq, *p, *q);
  if (0 == mpz_cmp (pq, *n))
    {
      printf ("p * q is equal to n\n");
    }
  else
    {
      printf ("p * q is not equal to n\n");
      exit (EXIT_FAILURE);
    }

  mpz_clears (gcd, pq, NULL);
}

static void
run_challenge (uint64_t t, mpz_t * p, mpz_t * q, mpz_t * n, mpz_t * w)
{
  mpz_t p_minus_1, q_minus_1, phi;
  mpz_t two, tmp;

  mpz_inits (p_minus_1, q_minus_1, phi, two, tmp, NULL);

  /* Compute the Euler totient of p * q, assuming both are prime */
  mpz_sub_ui (p_minus_1, *p, 1);
  mpz_sub_ui (q_minus_1, *q, 1);
  mpz_mul (phi, p_minus_1, q_minus_1);

  /* Use Euler's Theorem to compute the challenge value */
  mpz_set_ui (two, 2);
  mpz_powm_ui (tmp, two, t, phi);
  mpz_powm (*w, two, tmp, *n);

  mpz_clears (two, p_minus_1, q_minus_1, phi, tmp, NULL);
}

int
main (void)
{
  int rv;
  mpz_t n, z, p, q, w, message;
  uint64_t t = T;

  /* Initialize the challenge values */
  mpz_inits (n, z, p, q, w, message, NULL);

  rv = mpz_set_str (n, N, 10);
  CHECK_RV (rv, "failed to set n");
  rv = mpz_set_str (z, Z, 10);
  CHECK_RV (rv, "failed to set z");
  rv = mpz_set_str (p, P, 10);
  CHECK_RV (rv, "failed to set p");
  rv = mpz_set_str (q, Q, 10);
  CHECK_RV (rv, "failed to set q");

  validate_challenge (&n, &p, &q);

  /* Now do the hard work of running the challenge */
  run_challenge (t, &p, &q, &n, &w);
  mpz_xor (message, z, w);
  print_challenge_message (message);

  /* Clean up */
  mpz_clears (n, z, p, q, w, message, NULL);

  return EXIT_SUCCESS;
}
