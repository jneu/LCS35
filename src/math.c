#include "lcs35.h"
#include "math.h"

void
carmichael_pq_unchecked (mpz_t lambda_n, const mpz_t p, const mpz_t q)
{
  mpz_t p_minus_1, q_minus_1;

  mpz_inits (p_minus_1, q_minus_1, NULL);

  /*
   * The Carmichael function of the product of two distinct odd primes
   * is the least common multiple of (p-1) and (q-1).
   */
  mpz_sub_ui (p_minus_1, p, 1);
  mpz_sub_ui (q_minus_1, q, 1);
  mpz_lcm (lambda_n, p_minus_1, q_minus_1);

  mpz_clears (p_minus_1, q_minus_1, NULL);
}
