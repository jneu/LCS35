#include "lcs35.h"
#include "print_challenge_message.h"

int
main (void)
{
  int rv;
  mpz_t n, z, w, w2, message;
  uint64_t t = T;

  /* Initializations */
  mpz_inits (n, z, w, w2, message, NULL);

  rv = mpz_set_str (n, N, 10);
  CHECK_RV (rv, "failed to set n");
  rv = mpz_set_str (z, Z, 10);
  CHECK_RV (rv, "failed to set z");

  mpz_set_ui (w, 2);

  /* Get going */
  while (t-- > 0)
    {
      mpz_mul (w2, w, w);
      mpz_mod (w, w2, n);
    }

  /* Create and show the message */
  mpz_xor (message, z, w);
  print_challenge_message (message);

  /* Clean up */
  mpz_clears (n, z, w, w2, message, NULL);

  return EXIT_SUCCESS;
}
