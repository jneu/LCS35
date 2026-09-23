#ifndef CHALLENGE_H
#define CHALLENGE_H

/*
 * List the parameters of a challenge.
 *
 * Large numbers are represented as base 10 strings.
 */

typedef struct def_challenge
{
  /* Name the challenge for reference */
  const char *name;

  /* The challenge is to compute 2^(2^T) mod N */
  uint64_t T;
  const char *N;

  /* If the challenge has a message, it is found by xor-ing
   * the computation with Z; can be NULL */
  const char *Z;

  /* If N is the product of two known distinct odd primes,
   * they are listed as P and Q; can be NULL */
  const char *P;
  const char *Q;

  /* The challenge may have a seed B such that one of P or Q is
   * just larger than 5^B mod (2^prime_length); can be 0 otherwise */
  int prime_length;
} challenge;

/*
 * Predefined challenges
 */
extern challenge challenges[7];

#endif
