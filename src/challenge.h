#ifndef CHALLENGE_H
#define CHALLENGE_H

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

extern challenge LCS35_example;
extern challenge LCS35_easy;
extern challenge LCS35;
extern challenge CSAIL2019_example;
extern challenge CSAIL2019_easy;
extern challenge CSAIL2019;

#endif
