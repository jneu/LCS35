#ifndef PARSE_CHALLENGE_MESSAGE_H
#define PARSE_CHALLENGE_MESSAGE_H

/*
 * Parse the message to find the challenge seed. Return
 * true if successful and false otherwise.
 */

bool parse_challenge_message (mpz_t seed, const mpz_t message);

#endif
