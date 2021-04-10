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
parse_options (int argc, char *argv[], uint64_t * s, mpz_t w, const challenge ** puzzle)
{
  static struct option options[3] = {
    {"error-check", required_argument, NULL, 'e'},
    {"puzzle", required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0}
  };
  bool rv;
  mpz_t arg;

  mpz_init (arg);

  while (true)
    {
      int c;

      c = getopt_long (argc, argv, "e:p:", options, NULL);
      if (-1 == c)
        break;

      switch (c)
        {
        case 'e':
          {
            rv = (0 == mpz_set_str (arg, optarg, 10));
            if (!rv)
              {
                fputs ("bad --error-check argument\n", stderr);
                exit (EXIT_FAILURE);
              }

            if (mpz_cmp_ui (arg, UINT64_MAX) > 0)
              {
                fputs ("argument --error-check too large\n", stderr);
                exit (EXIT_FAILURE);
              }

            if (mpz_cmp_ui (arg, 0) < 0)
              {
                fputs ("argument --error-check is negative\n", stderr);
                exit (EXIT_FAILURE);
              }

            s_error_check = mpz_get_ui (arg);
          }
          break;
        case 'p':
          if (0 == strcmp (optarg, "LCS35-example"))
            {
              *puzzle = &LCS35_example;
            }
          else if (0 == strcmp (optarg, "LCS35-easy"))
            {
              *puzzle = &LCS35_easy;
            }
          else if (0 == strcmp (optarg, "LCS35"))
            {
              *puzzle = &LCS35;
            }
          else if (0 == strcmp (optarg, "CSAIL2019-example"))
            {
              *puzzle = &CSAIL2019_example;
            }
          else if (0 == strcmp (optarg, "CSAIL2019-easy"))
            {
              *puzzle = &CSAIL2019_easy;
            }
          else if (0 == strcmp (optarg, "CSAIL2019"))
            {
              *puzzle = &CSAIL2019;
            }
          else
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

  if (optind < argc)
    {
      if (optind != (argc - 2))
        {
          fputs ("invalid number of arguments\n", stderr);
          exit (EXIT_FAILURE);
        }

      if (0 == s_error_check)
        {
          fputs ("--error-check must be set if s and w are given\n", stderr);
          exit (EXIT_FAILURE);
        }

      /* Parse s */
      rv = (0 == mpz_set_str (arg, argv[optind], 10));
      if (!rv)
        {
          fputs ("bad s argument\n", stderr);
          exit (EXIT_FAILURE);
        }

      if (mpz_cmp_ui (arg, UINT64_MAX) > 0)
        {
          fputs ("argument s too large\n", stderr);
          exit (EXIT_FAILURE);
        }

      if (mpz_cmp_ui (arg, 0) < 0)
        {
          fputs ("argument s is negative\n", stderr);
          exit (EXIT_FAILURE);
        }

      *s = mpz_get_ui (arg);

      /* Parse w */
      rv = (0 == mpz_set_str (arg, argv[optind + 1], 10));
      if (!rv)
        {
          fputs ("bad w argument\n", stderr);
          exit (EXIT_FAILURE);
        }

      if (mpz_cmp_ui (arg, 0) < 0)
        {
          fputs ("argument w is negative\n", stderr);
          exit (EXIT_FAILURE);
        }

      mpz_set (w, arg);
    }

  mpz_clear (arg);
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

  mpz_init (check);

  mpz_powm_ui (check, two, s, totient_c);
  mpz_powm (check, two, check, c);

  if (0 != mpz_cmp_ui (check, mpz_fdiv_ui (w, C)))
    {
      puts ("error check failed");
      exit (EXIT_FAILURE);
    }

  mpz_clear (check);

  printf ("%" PRIu64 " ", s);
  mpz_out_str (stdout, 10, w);
  putchar ('\n');
  putchar ('\n');
}

int
main (int argc, char *argv[])
{
  bool rv;
  mpz_t n, z, w, w2, message;
  uint64_t s, t;
  const challenge *puzzle = &LCS35_easy;

  /* Initializations */
  mpz_inits (n, z, w, w2, message, NULL);

  mpz_set_ui (w, 2);
  s = 0;
  parse_options (argc, argv, &s, w, &puzzle);
  t = puzzle->T - s;

  rv = (0 == mpz_set_str (n, puzzle->N, 10));
  ASSERT_FATAL (rv, "failed to set n");
  rv = (0 == mpz_set_str (z, puzzle->Z, 10));
  ASSERT_FATAL (rv, "failed to set z");

  /* Get going */
  if (0 == s_error_check)
    {
      while (t-- > 0)
        {
          mpz_mul (w2, w, w);
          mpz_mod (w, w2, n);
        }
    }
  else
    {
      mpz_t cn;

      error_check (s, w);

      mpz_init (cn);
      mpz_mul_ui (cn, n, C);

      while (t-- > 0)
        {
          s++;

          mpz_mul (w2, w, w);
          mpz_mod (w, w2, cn);

          if (0 == (s % s_error_check))
            error_check (s, w);
        }

      mpz_clear (cn);

      error_check (s, w);
      mpz_mod (w, w, n);
    }

  /* Create and show the message */
  mpz_xor (message, z, w);
  print_challenge_message (message);

  /* Clean up */
  mpz_clears (n, z, w, w2, message, NULL);

  return EXIT_SUCCESS;
}
