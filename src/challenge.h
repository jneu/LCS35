#ifndef CHALLENGE_H
#define CHALLENGE_H

/*
 * How sure should we be that a number is prime?
 */

#define DEFAULT_NUM_PRIME_REPS 40

/*
 * Define the challenge parameters
 */

typedef struct def_challenge
{
  uint64_t T;

  int prime_length;
  const char *P;
  const char *Q;
  const char *N;

  const char *Z;
} challenge;

#endif
