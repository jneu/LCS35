#include <assert.h>
#include <error.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>

/* Basic error reporting */
#define CHECK_RV(rv) do { if (0 != (rv)) { error_at_line (EXIT_FAILURE, 0, __FILE__, __LINE__, "CHECK_RV failed"); } } while (0)
#define CHECK_RV0(rv) do { if (0 == (rv)) { error_at_line (EXIT_FAILURE, 0, __FILE__, __LINE__, "CHECK_RV0 failed"); } } while (0)

/* Define the challenge parameters */
#if _RUN_EXAMPLE
# define N "253"
# define Z "19"
# define T 10
#else
# define N \
  "631446608307288889379935712613129233236329881833084137558899" \
  "077270195712892488554730844605575320651361834662884894808866" \
  "350036848039658817136198766052189726781016228055747539383830" \
  "826175971321892666861177695452639157012069093997368008972127" \
  "446466642331918780683055206795125307008202024124623398241073" \
  "775370512734449416950118097524189066796385875485631980550727" \
  "370990439711973361466670154390536015254337398252457931357531" \
  "765364633198906465140213398526580034199190398219284471021246" \
  "488745938885358207031808428902320971090703239693491996277899" \
  "532332018406452247646396635593736700936921275809208629319872" \
  "7008292431243681"

# define Z \
  "427338526681239414707099486152541907807623930474842759553127" \
  "699575212802021361367225451651600353733949495680760238284875" \
  "258690199022379638588291839885522498545851997481849074579523" \
  "880422628363751913235562086585480775061024927773968205036369" \
  "669785002263076319003533000450157772067087172252728016627835" \
  "400463807389033342175518988780339070669313124967596962087173" \
  "533318107116757443584187074039849389081123568362582652760250" \
  "029401090870231288509578454981440888629750522601069337564316" \
  "940360631375375394366442662022050529545706707758321979377282" \
  "989361374561414204719371297211725179287931039547753581030226" \
  "7611143659071382"

# if 1
   /* Just do a million for now... */
#  define T ((uint64_t) 1000000)
# else
#  define T ((uint64_t) 79685186856218)
# endif
#endif

/* Pick the number of bits to use in the calculations */
#define R_NUM_BITS 2048
#define R_NUM_BYTES (R_NUM_BITS / 8)
#define R_NUM_LIMBS (R_NUM_BYTES / sizeof (mp_limb_t))

typedef struct def_context
{
  mpz_t r_inv;
  mpz_t n;
  mpz_t n_prime;
  mpz_t z;
  mpz_t w;
} context;

static void
context_init (context * ctx)
{
  mpz_t r;
  int rv;

  mpz_init (r);
  mpz_init2 (ctx->r_inv, R_NUM_BITS);
  mpz_init2 (ctx->n, R_NUM_BITS);
  mpz_init2 (ctx->n_prime, R_NUM_BITS);
  mpz_init2 (ctx->z, R_NUM_BITS);
  mpz_init (ctx->w);

  memset (mpz_limbs_write (ctx->r_inv, R_NUM_BITS), 0, R_NUM_BYTES);
  memset (mpz_limbs_write (ctx->n, R_NUM_BITS), 0, R_NUM_BYTES);
  memset (mpz_limbs_write (ctx->n_prime, R_NUM_BITS), 0, R_NUM_BYTES);
  memset (mpz_limbs_write (ctx->z, R_NUM_BITS), 0, R_NUM_BYTES);

  mpz_ui_pow_ui (r, 2, R_NUM_BITS);

  rv = mpz_set_str (ctx->n, N, 10);
  CHECK_RV (rv);
  assert (mpz_cmp (ctx->n, r) < 0);
  assert (0 == (R_NUM_BITS % 8));
  assert (0 == (R_NUM_BYTES % sizeof (mp_limb_t)));
  assert (R_NUM_LIMBS > 0);

  rv = mpz_invert (ctx->r_inv, r, ctx->n);
  CHECK_RV0 (rv);

  mpz_invert (ctx->n_prime, ctx->n, r);
  mpz_mul_si (ctx->n_prime, ctx->n_prime, -1);
  mpz_mod (ctx->n_prime, ctx->n_prime, r);

  rv = mpz_set_str (ctx->z, Z, 10);
  CHECK_RV (rv);

  mpz_mul_ui (ctx->w, r, 2);
  mpz_mod (ctx->w, ctx->w, ctx->n);

  mpz_clear (r);
}

static void
context_fini (context * ctx)
{
  mpz_clears (ctx->r_inv, NULL);
  mpz_clears (ctx->n, ctx->n_prime, NULL);
  mpz_clears (ctx->z, ctx->w, NULL);
}

int
main (void)
{
  context ctx;
  mpz_t final;
  const mp_limb_t *p_n, *p_n_prime, *p_tmp1;
  mp_limb_t *p_tmp2, *p_tmp3, *p_tmp4;
  uint64_t t = T;

  /* Initializations */
  context_init (&ctx);

  /* Get going */
  p_n = mpz_limbs_read (ctx.n);
  p_n_prime = mpz_limbs_read (ctx.n_prime);
  p_tmp1 = mpz_limbs_read (ctx.w);
  p_tmp2 = aligned_alloc (256, 2 * R_NUM_BYTES);
  p_tmp3 = aligned_alloc (256, 2 * R_NUM_BYTES);
  p_tmp4 = aligned_alloc (256, 2 * R_NUM_BYTES);

  memset (p_tmp2, 0, 2 * R_NUM_BYTES);
  memset (p_tmp3, 0, 2 * R_NUM_BYTES);
  memset (p_tmp4, 0, 2 * R_NUM_BYTES);

  while (t-- > 0)
    {
      mpn_sqr (p_tmp2, p_tmp1, R_NUM_LIMBS);

      mpn_mul_n (p_tmp3, p_tmp2, p_n_prime, R_NUM_LIMBS);
      mpn_mul_n (p_tmp4, p_tmp3, p_n, R_NUM_LIMBS);
      mpn_add_n (p_tmp3, p_tmp4, p_tmp2, 2 * R_NUM_LIMBS);

      if (0 != mpn_sub_n (p_tmp4, p_tmp3 + R_NUM_LIMBS, p_n, R_NUM_LIMBS)) {
	      p_tmp1 = p_tmp3 + R_NUM_LIMBS;
      } else {
	      p_tmp1 = p_tmp4;
      }
    }

  {
	  int num = R_NUM_LIMBS;
	  while (0 == p_tmp1[num - 1]) {
		  num--;
	  }
  mpz_mul (ctx.w, mpz_roinit_n (final, p_tmp1, num), ctx.r_inv);
  }
  mpz_mod (ctx.w, ctx.w, ctx.n);

  /* Create and show the message */
  mpz_xor (ctx.w, ctx.w, ctx.z);

  printf ("message:\n");
  mpz_out_str (stdout, 10, ctx.w);
  printf ("\n");

  /* Clean up */
  context_fini (&ctx);

  return EXIT_SUCCESS;
}
