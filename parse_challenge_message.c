#include "lcs35.h"
#include "parse_challenge_message.h"

bool
parse_challenge_message (mpz_t seed, const mpz_t message)
{
  size_t offset, space, src, dest;
  const uint8_t *raw_bytes;
  char *ascii_str;

  offset = mpz_size (message) * sizeof (mp_limb_t);
  raw_bytes = (const uint8_t *) mpz_limbs_read (message);

  if (0 == offset)
    return false;
  if (')' != raw_bytes[0])
    return false;

  for (space = 1; (space < offset) && (raw_bytes[space] >= '0') && (raw_bytes[space] <= '9'); space++)
    ;

  if (space == offset)
    return false;
  if (' ' != raw_bytes[space])
    return false;
  if (('0' == raw_bytes[space - 1]) && (space > 2))
    return false;

  ascii_str = malloc (space);
  ASSERT_FATAL (NULL != ascii_str, "failed to allocate ascii_str");

  for (src = space - 1, dest = 0; src > 0; src--, dest++)
    {
      ascii_str[dest] = raw_bytes[src];
    }
  ascii_str[dest] = '\0';

  mpz_set_str (seed, ascii_str, 10);

  free (ascii_str);

  return true;
}
