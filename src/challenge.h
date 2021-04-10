#ifndef CHALLENGE_H
#define CHALLENGE_H

/*
 * Define the challenge parameters
 */

typedef struct def_challenge
{
  const char *name;

  uint64_t T;

  int prime_length;
  const char *P;
  const char *Q;
  const char *N;

  const char *Z;
} challenge;

extern challenge challenges[7];

#endif
