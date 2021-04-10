#include "lcs35.h"
#include "challenge.h"
#include "parse_challenge_message.h"
#include "print_challenge_message.h"

/*
 * How sure should we be that a number is prime?
 */

#define NUM_PRIME_REPS 40

/*
 * Options
 */

static void
parse_options (int argc, char *argv[], const challenge ** puzzle)
{
  static struct option options[2] = {
    {"puzzle", required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0}
  };

  while (true)
    {
      int c;

      c = getopt_long (argc, argv, "p:", options, NULL);
      if (-1 == c)
        break;

      switch (c)
        {
        case 'p':
          for (*puzzle = &challenges[0]; NULL != (*puzzle)->name; (*puzzle)++)
            {
              if (0 == strcmp (optarg, (*puzzle)->name))
                break;
            }

          if (NULL == (*puzzle)->name)
            {
              fputs ("argument --puzzle is unknown\n", stderr);
              exit (EXIT_FAILURE);
            }

          break;
        default:
          fputs ("unknown option\n", stderr);
          exit (EXIT_FAILURE);
        }
    }
}

static void
validate_challenge (const mpz_t n, const mpz_t p, const mpz_t q)
{
  int rv;
  mpz_t gcd, pq;

  mpz_inits (gcd, pq, NULL);

  if (0 != mpz_odd_p (p))
    {
      puts ("p is odd");
    }
  else
    {
      puts ("p is even");
      exit (EXIT_FAILURE);
    }

  if (0 != mpz_odd_p (q))
    {
      puts ("q is odd");
    }
  else
    {
      puts ("q is even");
      exit (EXIT_FAILURE);
    }

  mpz_gcd (gcd, p, q);
  if (0 == mpz_cmp_ui (gcd, 1))
    {
      puts ("p and q are relatively prime");
    }
  else
    {
      puts ("p and q are not relatively prime; their gcd is:");
      mpz_out_str (stdout, 10, gcd);
      putchar ('\n');
      exit (EXIT_FAILURE);
    }

  rv = mpz_probab_prime_p (p, NUM_PRIME_REPS);
  if (2 == rv)
    {
      puts ("p is definitely prime");
    }
  else if (1 == rv)
    {
      printf ("p is probably prime (%d reps)\n", NUM_PRIME_REPS);
    }
  else
    {
      puts ("p is definitely not prime");
      exit (EXIT_FAILURE);
    }

  rv = mpz_probab_prime_p (q, NUM_PRIME_REPS);
  if (2 == rv)
    {
      puts ("q is definitely prime");
    }
  else if (1 == rv)
    {
      printf ("q is probably prime (%d reps)\n", NUM_PRIME_REPS);
    }
  else
    {
      puts ("q is definitely not prime");
      exit (EXIT_FAILURE);
    }

  mpz_mul (pq, p, q);
  if (0 == mpz_cmp (pq, n))
    {
      puts ("p * q is equal to n");
    }
  else
    {
      puts ("p * q is not equal to n");
      exit (EXIT_FAILURE);
    }

  mpz_clears (gcd, pq, NULL);
}

static void
run_challenge (mpz_t w, uint64_t t, const mpz_t p, const mpz_t q, const mpz_t n)
{
  mpz_t p_minus_1, q_minus_1, phi;
  mpz_t two, tmp;

  mpz_inits (p_minus_1, q_minus_1, phi, two, tmp, NULL);

  /* Compute the Euler totient of p * q, assuming both are prime */
  mpz_sub_ui (p_minus_1, p, 1);
  mpz_sub_ui (q_minus_1, q, 1);
  mpz_mul (phi, p_minus_1, q_minus_1);

  /* Use Euler's Theorem to compute the challenge value */
  mpz_set_ui (two, 2);
  mpz_powm_ui (tmp, two, t, phi);
  mpz_powm (w, two, tmp, n);

  mpz_clears (two, p_minus_1, q_minus_1, phi, tmp, NULL);
}

static void
recover_p_from_seed (const mpz_t message, const mpz_t p, const mpz_t q, int prime_length)
{
  mpz_t seed, big_2, w;

  mpz_inits (seed, big_2, w, NULL);

  if (!parse_challenge_message (seed, message))
    {
      puts ("failed to parse challenge message");
      exit (EXIT_FAILURE);
    }

  mpz_set_ui (w, 5);
  mpz_ui_pow_ui (big_2, 2, prime_length);
  mpz_powm (w, w, seed, big_2);
  mpz_nextprime (w, w);

  if (0 == mpz_cmp (w, p))
    {
      puts ("message seed gives p");
    }
  else if (0 == mpz_cmp (w, q))
    {
      puts ("message seed gives q");
    }
  else
    {
      puts ("failed to find p or q from message seed");
      exit (EXIT_FAILURE);
    }

  mpz_clears (seed, big_2, w, NULL);
}

int
main (int argc, char *argv[])
{
  bool rv;
  mpz_t n, z, p, q, w, message;
  const challenge *puzzle = &challenges[0];

  /* We need to check this somewhere... */
  assert ((64 / 8) == sizeof (mp_limb_t));

  parse_options (argc, argv, &puzzle);

  printf ("puzzle: %s\n", puzzle->name);

  /* Initialize the challenge values */
  mpz_inits (n, z, p, q, w, message, NULL);

  rv = (0 == mpz_set_str (n, puzzle->N, 10));
  ASSERT_FATAL (rv, "failed to set n");
  rv = (0 == mpz_set_str (z, puzzle->Z, 10));
  ASSERT_FATAL (rv, "failed to set z");
  rv = (0 == mpz_set_str (p, puzzle->P, 10));
  ASSERT_FATAL (rv, "failed to set p");
  rv = (0 == mpz_set_str (q, puzzle->Q, 10));
  ASSERT_FATAL (rv, "failed to set q");

  validate_challenge (n, p, q);

  /* Now do the hard work of running the challenge */
  run_challenge (w, puzzle->T, p, q, n);

  mpz_xor (message, z, w);
  print_challenge_message (message);

  /* Try to recover p or q */
  recover_p_from_seed (message, p, q, puzzle->prime_length);

  /* Clean up */
  mpz_clears (n, z, p, q, w, message, NULL);

  return EXIT_SUCCESS;
}
