#include "lcs35.h"
#include "print_challenge_message.h"

/*
 * A 64 bit prime to use in error checking
 */

#define C ((uint64_t) 13901611780890809387UL)

/*
 * Options
 */

static uint64_t s_error_check = 0;

static void
parse_options (int *argc, char ***argv)
{
  static struct option options[2] = {
    {"error-check", required_argument, NULL, 'e'},
    {NULL, 0, NULL, 0}
  };
  int c;

  while (true)
    {
      c = getopt_long (*argc, *argv, "e:", options, NULL);
      if (-1 == c)
        break;

      switch (c)
        {
        case 'e':
          {
            bool rv;
            mpz_t ec_arg;

            rv = (0 == mpz_init_set_str (ec_arg, optarg, 10));
            if (!rv)
              {
                fputs ("bad argument\n", stderr);
                exit (EXIT_FAILURE);
              }

            if (mpz_cmp_ui (ec_arg, UINT64_MAX) > 0)
              {
                fputs ("argument too large\n", stderr);
                exit (EXIT_FAILURE);
              }

            s_error_check = mpz_get_ui (ec_arg);

            mpz_clear (ec_arg);
          }
          break;
        default:
          exit (EXIT_FAILURE);
        }
    }
}

static void
error_check (uint64_t s, const mpz_t w)
{
  static const mp_limb_t two_limb[1] = { 2 };
  static const mp_limb_t c_limb[1] = { C };
  static const mp_limb_t totient_c_limb[1] = { C - 1 };
  static const mpz_t two = MPZ_ROINIT_N ((mp_limb_t *) two_limb, 1);
  static const mpz_t c = MPZ_ROINIT_N ((mp_limb_t *) c_limb, 1);
  static const mpz_t totient_c = MPZ_ROINIT_N ((mp_limb_t *) totient_c_limb, 1);

  mpz_t check;

  if (0 == s_error_check)
    return;

  mpz_init (check);

  mpz_powm_ui (check, two, s, totient_c);
  mpz_powm (check, two, check, c);

  if (0 != mpz_cmp_ui (check, mpz_fdiv_ui (w, C)))
    {
      puts ("error check failed");
      exit (EXIT_FAILURE);
    }

  mpz_clear (check);
}

int
main (int argc, char *argv[])
{
  bool rv;
  mpz_t n, z, cn, w, w2, message;
  uint64_t s, t;

  parse_options (&argc, &argv);

  /* Initializations */
  mpz_inits (n, z, cn, w, w2, message, NULL);

  rv = (0 == mpz_set_str (n, N, 10));
  ASSERT_FATAL (rv, "failed to set n");
  rv = (0 == mpz_set_str (z, Z, 10));
  ASSERT_FATAL (rv, "failed to set z");

  mpz_mul_ui (cn, n, C);

  s = 0;
  t = T;
  mpz_set_ui (w, 2);
  error_check (s, w);

  /* Get going */
  if (0 == s_error_check)
    {
      while (t-- > 0)
        {
          s++;

          mpz_mul (w2, w, w);
          mpz_mod (w, w2, n);
        }
    }
  else
    {
      while (t-- > 0)
        {
          s++;

          mpz_mul (w2, w, w);
          mpz_mod (w, w2, cn);

          if (0 == (s % s_error_check))
            error_check (s, w);
        }
    }

  /* This mod is a nop unless error checking */
  error_check (s, w);
  mpz_mod (w, w, n);

  /* Create and show the message */
  mpz_xor (message, z, w);
  print_challenge_message (message);

  /* Clean up */
  mpz_clears (n, z, cn, w, w2, message, NULL);

  return EXIT_SUCCESS;
}
