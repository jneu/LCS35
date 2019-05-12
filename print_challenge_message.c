#include "lcs35.h"
#include "print_challenge_message.h"

void
print_challenge_message (const mpz_t z, const mpz_t w)
{
	mpz_t message;
  size_t offset;
  const uint8_t *raw_bytes;

	mpz_init (message);

  mpz_xor (message, z, w);

  printf ("message:\n");
  mpz_out_str (stdout, 10, message);
  printf ("\n");

  offset = mpz_size (message) * sizeof (mp_limb_t);
  raw_bytes = (const uint8_t *) mpz_limbs_read (message);

  while ((offset > 0) && (0 == raw_bytes[offset - 1]))
    {
      offset--;
    }
  while (offset > 0)
    {
      putchar ((int) raw_bytes[--offset]);
    }
  printf ("\n");

	mpz_clear (message);
}
