#ifndef MATH_H
#define MATH_H

/*
 * Calculate the Carmichael function lambda_n of the product of p and q.
 *
 * p and q must be distinct odd prime numbers. This assumption is not checked.
 */
void carmichael_pq_unchecked (mpz_t lambda_n, const mpz_t p, const mpz_t q);

#endif
