#define CRYPT 0x0117

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint64_t mp_digit;
typedef unsigned long mp_word __attribute__((mode(TI)));

#define DIGIT_BIT 60

/* otherwise the bits per digit is calculated automatically from the size of a
 * mp_digit */

typedef mp_digit mp_min_u32;

#define MP_DIGIT_BIT DIGIT_BIT
#define MP_MASK ((((mp_digit)1) << ((mp_digit)DIGIT_BIT)) - ((mp_digit)1))

/* equalities */
#define MP_LT -1 /* less than */
#define MP_EQ 0  /* equal to */
#define MP_GT 1  /* greater than */

#define MP_ZPOS 0 /* positive integer */
#define MP_NEG 1  /* negative */

#define MP_OKAY 0 /* ok result */
#define MP_MEM -2 /* out of mem */
#define MP_VAL -3 /* invalid input */

#define MP_YES 1 /* yes response */
#define MP_NO 0  /* no response */
typedef int mp_err;

/* you'll have to tune these... */
extern int KARATSUBA_MUL_CUTOFF, KARATSUBA_SQR_CUTOFF, TOOM_MUL_CUTOFF,
    TOOM_SQR_CUTOFF;

/* default precision */
#define MP_PREC 32 /* default digits of precision */

/* size of comba arrays, should be at least 2 * 2**(BITS_PER_WORD -
 * BITS_PER_DIGIT*2) */
#define MP_WARRAY (1 << (((sizeof(mp_word) * CHAR_BIT) - (2 * DIGIT_BIT)) + 1))

/* the infamous mp_int structure */
typedef struct {
  int used, alloc, sign;
  mp_digit *dp;
} mp_int;

#define DIGIT(m, k) ((m)->dp[(k)])

/* ---> init and deinit bignum functions <--- */
/* init a bignum */
int mp_init(mp_int *a);

/* free a bignum */
void mp_clear(mp_int *a);

/* init a null terminated series of arguments */
int mp_init_multi(mp_int *mp, ...);

/* clear a null terminated series of arguments */
void mp_clear_multi(mp_int *mp, ...);

/* exchange two ints */
void mp_exch(mp_int *a, mp_int *b);

/* grow an int to a given size */
int mp_grow(mp_int *a, int size);

/* init to a given number of digits */
int mp_init_size(mp_int *a, int size);

/* ---> Basic Manipulations <--- */
#define mp_iszero(a) (((a)->used == 0) ? MP_YES : MP_NO)
#define mp_iseven(a) \
  ((((a)->used > 0) && (((a)->dp[0] & 1u) == 0u)) ? MP_YES : MP_NO)
#define mp_isodd(a) \
  ((((a)->used > 0) && (((a)->dp[0] & 1u) == 1u)) ? MP_YES : MP_NO)

/* set to zero */
void mp_zero(mp_int *a);

/* set to a digit */
void mp_set(mp_int *a, mp_digit b);

/* set a 32-bit const */
int mp_set_int(mp_int *a, unsigned long b);

/* set a platform dependent unsigned long value */
int mp_set_long(mp_int *a, unsigned long b);

/* set a platform dependent unsigned long long value */
int mp_set_long_long(mp_int *a, unsigned long long b);

/* get a 32-bit value */
unsigned long mp_get_int(mp_int *a);

/* copy, b = a */
int mp_copy(mp_int *a, mp_int *b);

/* inits and copies, a = b */
int mp_init_copy(mp_int *a, mp_int *b);

/* trim unused digits */
void mp_clamp(mp_int *a);

/* ---> digit manipulation <--- */

/* right shift by "b" digits */
void mp_rshd(mp_int *a, int b);

/* left shift by "b" digits */
int mp_lshd(mp_int *a, int b);

/* c = a / 2**b, implemented as c = a >> b */
int mp_div_2d(mp_int *a, int b, mp_int *c, mp_int *d);

/* b = a/2 */
int mp_div_2(mp_int *a, mp_int *b);

/* c = a * 2**b, implemented as c = a << b */
int mp_mul_2d(mp_int *a, int b, mp_int *c);

/* b = a*2 */
int mp_mul_2(mp_int *a, mp_int *b);

/* c = a mod 2**b */
int mp_mod_2d(mp_int *a, int b, mp_int *c);

/* computes a = 2**b */
int mp_2expt(mp_int *a, int b);

/* Counts the number of lsbs which are zero before the first zero bit */
int mp_cnt_lsb(mp_int *a);

/* I Love Earth! */

/* ---> binary operations <--- */

/* ---> Basic arithmetic <--- */

/* b = -a */
int mp_neg(mp_int *a, mp_int *b);

/* b = |a| */
int mp_abs(mp_int *a, mp_int *b);

/* compare a to b */
int mp_cmp(mp_int *a, mp_int *b);

/* compare |a| to |b| */
int mp_cmp_mag(mp_int *a, mp_int *b);

/* c = a + b */
int mp_add(mp_int *a, mp_int *b, mp_int *c);

/* c = a - b */
int mp_sub(mp_int *a, mp_int *b, mp_int *c);

/* c = a * b */
int mp_mul(mp_int *a, mp_int *b, mp_int *c);

/* b = a*a  */
int mp_sqr(mp_int *a, mp_int *b);

/* a/b => cb + d == a */
int mp_div(mp_int *a, mp_int *b, mp_int *c, mp_int *d);

/* c = a mod b, 0 <= c < b  */
int mp_mod(mp_int *a, mp_int *b, mp_int *c);

/* ---> single digit functions <--- */

/* compare against a single digit */
int mp_cmp_d(mp_int *a, mp_digit b);

/* c = a + b */
int mp_add_d(mp_int *a, mp_digit b, mp_int *c);

/* c = a - b */
int mp_sub_d(mp_int *a, mp_digit b, mp_int *c);

/* c = a * b */
int mp_mul_d(mp_int *a, mp_digit b, mp_int *c);

/* a/b => cb + d == a */
int mp_div_d(mp_int *a, mp_digit b, mp_int *c, mp_digit *d);

/* a/3 => 3c + d == a */
int mp_div_3(mp_int *a, mp_int *c, mp_digit *d);

/* c = a mod b, 0 <= c < b  */
int mp_mod_d(mp_int *a, mp_digit b, mp_digit *c);

/* ---> number theory <--- */

/* d = a * b (mod c) */
int mp_mulmod(mp_int *a, mp_int *b, mp_int *c, mp_int *d);

/* c = a * a (mod b) */
int mp_sqrmod(mp_int *a, mp_int *b, mp_int *c);

/* c = 1/a (mod b) */
int mp_invmod(mp_int *a, mp_int *b, mp_int *c);

/* c = (a, b) */
int mp_gcd(mp_int *a, mp_int *b, mp_int *c);

/* c = [a, b] or (a*b)/(a, b) */
int mp_lcm(mp_int *a, mp_int *b, mp_int *c);

/* computes the jacobi c = (a | n) (or Legendre if b is prime)  */
int mp_jacobi(mp_int *a, mp_int *n, int *c);

/* used to setup the Barrett reduction for a given modulus b */
int mp_reduce_setup(mp_int *a, mp_int *b);

/* Barrett Reduction, computes a (mod b) with a precomputed value c
 *
 * Assumes that 0 < a <= b*b, note if 0 > a > -(b*b) then you can merely
 * compute the reduction as -1 * mp_reduce(mp_abs(a)) [pseudo code].
 */
int mp_reduce(mp_int *a, mp_int *b, mp_int *c);

/* setups the montgomery reduction */
int mp_montgomery_setup(mp_int *a, mp_digit *mp);

/* computes a = B**n mod b without division or multiplication useful for
 * normalizing numbers in a Montgomery system.
 */
int mp_montgomery_calc_normalization(mp_int *a, mp_int *b);

/* computes x/R == x (mod N) via Montgomery Reduction */
int mp_montgomery_reduce(mp_int *a, mp_int *m, mp_digit mp);

/* returns 1 if a is a valid DR modulus */
int mp_dr_is_modulus(mp_int *a);

/* sets the value of "d" required for mp_dr_reduce */
void mp_dr_setup(mp_int *a, mp_digit *d);

/* reduces a modulo b using the Diminished Radix method */
int mp_dr_reduce(mp_int *a, mp_int *b, mp_digit mp);

/* returns true if a can be reduced with mp_reduce_2k */
int mp_reduce_is_2k(mp_int *a);

/* determines k value for 2k reduction */
int mp_reduce_2k_setup(mp_int *a, mp_digit *d);

/* reduces a modulo b where b is of the form 2**p - k [0 <= a] */
int mp_reduce_2k(mp_int *a, mp_int *n, mp_digit d);

/* returns true if a can be reduced with mp_reduce_2k_l */
int mp_reduce_is_2k_l(mp_int *a);

/* determines k value for 2k reduction */
int mp_reduce_2k_setup_l(mp_int *a, mp_int *d);

/* reduces a modulo b where b is of the form 2**p - k [0 <= a] */
int mp_reduce_2k_l(mp_int *a, mp_int *n, mp_int *d);

/* d = a**b (mod c) */
int mp_exptmod(mp_int *a, mp_int *b, mp_int *c, mp_int *d);

/* ---> Primes <--- */

/* number of primes */
#define PRIME_SIZE 256

/* table of first PRIME_SIZE primes */
extern const mp_digit ltm_prime_tab[PRIME_SIZE];

/* result=1 if a is divisible by one of the first PRIME_SIZE primes */
int mp_prime_is_divisible(mp_int *a, int *result);

/* performs one Miller-Rabin test of "a" using base "b".
 * Sets result to 0 if composite or 1 if probable prime
 */
int mp_prime_miller_rabin(mp_int *a, mp_int *b, int *result);

/* performs t rounds of Miller-Rabin on "a" using the first
 * t prime bases.  Also performs an initial sieve of trial
 * division.  Determines if "a" is prime with probability
 * of error no more than (1/4)**t.
 *
 * Sets result to 1 if probably prime, 0 otherwise
 */
int mp_prime_is_prime(mp_int *a, int t, int *result);

/* makes a truly random prime of a given size (bytes),
 * call with bbs = 1 if you want it to be congruent to 3 mod 4
 *
 * You have to supply a callback which fills in a buffer with random bytes.
 * "dat" is a parameter you can have passed to the callback (e.g. a state or
 * something).  This function doesn't use "dat" itself so it can be NULL
 *
 * The prime generated will be larger than 2^(8*size).
 */

/* ---> radix conversion <--- */
int mp_count_bits(mp_int *a);

int mp_unsigned_bin_size(mp_int *a);

int mp_read_unsigned_bin(mp_int *a, const unsigned char *b, int c);

int mp_to_unsigned_bin(mp_int *a, unsigned char *b);

int mp_read_radix(mp_int *a, const char *str, int radix);

int mp_toradix(mp_int *a, char *str, int radix);

#define mp_tohex(M, S) mp_toradix((M), (S), 16)

#include <ctype.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/* C on the other hand doesn't care */
#define OPT_CAST(x)

#define XMALLOC malloc
#define XFREE free
#define XREALLOC realloc
#define XCALLOC calloc

/* lowlevel functions, do not call! */
int s_mp_add(mp_int *a, mp_int *b, mp_int *c);

int s_mp_sub(mp_int *a, mp_int *b, mp_int *c);

#define s_mp_mul(a, b, c) s_mp_mul_digs(a, b, c, (a)->used + (b)->used + 1)

int fast_s_mp_mul_digs(mp_int *a, mp_int *b, mp_int *c, int digs);

int s_mp_mul_digs(mp_int *a, mp_int *b, mp_int *c, int digs);

int fast_s_mp_mul_high_digs(mp_int *a, mp_int *b, mp_int *c, int digs);

int s_mp_mul_high_digs(mp_int *a, mp_int *b, mp_int *c, int digs);

int fast_s_mp_sqr(mp_int *a, mp_int *b);

int s_mp_sqr(mp_int *a, mp_int *b);

int mp_karatsuba_mul(mp_int *a, mp_int *b, mp_int *c);

int mp_toom_mul(mp_int *a, mp_int *b, mp_int *c);

int mp_karatsuba_sqr(mp_int *a, mp_int *b);

int mp_toom_sqr(mp_int *a, mp_int *b);

int fast_mp_invmod(mp_int *a, mp_int *b, mp_int *c);

int mp_invmod_slow(mp_int *a, mp_int *b, mp_int *c);

int fast_mp_montgomery_reduce(mp_int *x, mp_int *n, mp_digit rho);

int mp_exptmod_fast(mp_int *G, mp_int *X, mp_int *P, mp_int *Y, int redmode);

int s_mp_exptmod(mp_int *G, mp_int *X, mp_int *P, mp_int *Y, int redmode);

void bn_reverse(unsigned char *s, int len);

extern const char *mp_s_rmap;

/* Fancy macro to set an MPI from another type.
 * There are several things assumed:
 *  x is the counter and unsigned
 *  a is the pointer to the MPI
 *  b is the original value that should be set in the MPI.
 */
#define MP_SET_XLONG(func_name, type)                      \
  int func_name(mp_int *a, type b) {                       \
    unsigned int x;                                        \
    int res;                                               \
                                                           \
    mp_zero(a);                                            \
                                                           \
    /* set four bits at a time */                          \
    for (x = 0; x < (sizeof(type) * 2u); x++) {            \
      /* shift the number up four bits */                  \
      if ((res = mp_mul_2d(a, 4, a)) != MP_OKAY) {         \
        return res;                                        \
      }                                                    \
                                                           \
      /* OR in the top four bits of the source */          \
      a->dp[0] |= (b >> ((sizeof(type) * 8u) - 4u)) & 15u; \
                                                           \
      /* shift the source up to the next four bits */      \
      b <<= 4;                                             \
                                                           \
      /* ensure that digits are not clamped off */         \
      a->used += 1;                                        \
    }                                                      \
    mp_clamp(a);                                           \
    return MP_OKAY;                                        \
  }

/* computes the modular inverse via binary extended euclidean algorithm,
 * that is c = 1/a mod b
 *
 * Based on slow invmod except this is optimized for the case where b is
 * odd as per HAC Note 14.64 on pp. 610
 */
int fast_mp_invmod(mp_int *a, mp_int *b, mp_int *c) {
  mp_int x, y, u, v, B, D;
  int res, neg;

  /* 2. [modified] b must be odd   */
  if (mp_iseven(b) == MP_YES) {
    return MP_VAL;
  }

  /* init all our temps */
  if ((res = mp_init_multi(&x, &y, &u, &v, &B, &D, NULL)) != MP_OKAY) {
    return res;
  }

  /* x == modulus, y == value to invert */
  if ((res = mp_copy(b, &x)) != MP_OKAY) {
    goto LBL_ERR;
  }

  /* we need y = |a| */
  if ((res = mp_mod(a, b, &y)) != MP_OKAY) {
    goto LBL_ERR;
  }

  /* 3. u=x, v=y, A=1, B=0, C=0,D=1 */
  if ((res = mp_copy(&x, &u)) != MP_OKAY) {
    goto LBL_ERR;
  }
  if ((res = mp_copy(&y, &v)) != MP_OKAY) {
    goto LBL_ERR;
  }
  mp_set(&D, 1);

top:
  /* 4.  while u is even do */
  while (mp_iseven(&u) == MP_YES) {
    /* 4.1 u = u/2 */
    if ((res = mp_div_2(&u, &u)) != MP_OKAY) {
      goto LBL_ERR;
    }
    /* 4.2 if B is odd then */
    if (mp_isodd(&B) == MP_YES) {
      if ((res = mp_sub(&B, &x, &B)) != MP_OKAY) {
        goto LBL_ERR;
      }
    }
    /* B = B/2 */
    if ((res = mp_div_2(&B, &B)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* 5.  while v is even do */
  while (mp_iseven(&v) == MP_YES) {
    /* 5.1 v = v/2 */
    if ((res = mp_div_2(&v, &v)) != MP_OKAY) {
      goto LBL_ERR;
    }
    /* 5.2 if D is odd then */
    if (mp_isodd(&D) == MP_YES) {
      /* D = (D-x)/2 */
      if ((res = mp_sub(&D, &x, &D)) != MP_OKAY) {
        goto LBL_ERR;
      }
    }
    /* D = D/2 */
    if ((res = mp_div_2(&D, &D)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* 6.  if u >= v then */
  if (mp_cmp(&u, &v) != MP_LT) {
    /* u = u - v, B = B - D */
    if ((res = mp_sub(&u, &v, &u)) != MP_OKAY) {
      goto LBL_ERR;
    }

    if ((res = mp_sub(&B, &D, &B)) != MP_OKAY) {
      goto LBL_ERR;
    }
  } else {
    /* v - v - u, D = D - B */
    if ((res = mp_sub(&v, &u, &v)) != MP_OKAY) {
      goto LBL_ERR;
    }

    if ((res = mp_sub(&D, &B, &D)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* if not zero goto step 4 */
  if (mp_iszero(&u) == MP_NO) {
    goto top;
  }

  /* now a = C, b = D, gcd == g*v */

  /* if v != 1 then there is no inverse */
  if (mp_cmp_d(&v, 1) != MP_EQ) {
    res = MP_VAL;
    goto LBL_ERR;
  }

  /* b is now the inverse */
  neg = a->sign;
  while (D.sign == MP_NEG) {
    if ((res = mp_add(&D, b, &D)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }
  mp_exch(&D, c);
  c->sign = neg;
  res = MP_OKAY;

LBL_ERR:
  mp_clear_multi(&x, &y, &u, &v, &B, &D, NULL);
  return res;
}

/* computes xR**-1 == x (mod N) via Montgomery Reduction
 *
 * This is an optimized implementation of montgomery_reduce
 * which uses the comba method to quickly calculate the columns of the
 * reduction.
 *
 * Based on Algorithm 14.32 on pp.601 of HAC.
 */
int fast_mp_montgomery_reduce(mp_int *x, mp_int *n, mp_digit rho) {
  int ix, res, olduse;
  mp_word W[MP_WARRAY];

  /* get old used count */
  olduse = x->used;

  /* grow a as required */
  if (x->alloc < (n->used + 1)) {
    if ((res = mp_grow(x, n->used + 1)) != MP_OKAY) {
      return res;
    }
  }

  /* first we have to get the digits of the input into
   * an array of double precision words W[...]
   */
  {
    mp_word *_W;
    mp_digit *tmpx;

    /* alias for the W[] array */
    _W = W;

    /* alias for the digits of  x*/
    tmpx = x->dp;

    /* copy the digits of a into W[0..a->used-1] */
    for (ix = 0; ix < x->used; ix++) {
      *_W++ = *tmpx++;
    }

    /* zero the high words of W[a->used..m->used*2] */
    for (; ix < ((n->used * 2) + 1); ix++) {
      *_W++ = 0;
    }
  }

  /* now we proceed to zero successive digits
   * from the least significant upwards
   */
  for (ix = 0; ix < n->used; ix++) {
    /* mu = ai * m' mod b
     *
     * We avoid a double precision multiplication (which isn't required)
     * by casting the value down to a mp_digit.  Note this requires
     * that W[ix-1] have  the carry cleared (see after the inner loop)
     */
    mp_digit mu;
    mu = (mp_digit)(((W[ix] & MP_MASK) * rho) & MP_MASK);

    /* a = a + mu * m * b**i
     *
     * This is computed in place and on the fly.  The multiplication
     * by b**i is handled by offseting which columns the results
     * are added to.
     *
     * Note the comba method normally doesn't handle carries in the
     * inner loop In this case we fix the carry from the previous
     * column since the Montgomery reduction requires digits of the
     * result (so far) [see above] to work.  This is
     * handled by fixing up one carry after the inner loop.  The
     * carry fixups are done in order so after these loops the
     * first m->used words of W[] have the carries fixed
     */
    {
      int iy;
      mp_digit *tmpn;
      mp_word *_W;

      /* alias for the digits of the modulus */
      tmpn = n->dp;

      /* Alias for the columns set by an offset of ix */
      _W = W + ix;

      /* inner loop */
      for (iy = 0; iy < n->used; iy++) {
        *_W++ += ((mp_word)mu) * ((mp_word)*tmpn++);
      }
    }

    /* now fix carry for next digit, W[ix+1] */
    W[ix + 1] += W[ix] >> ((mp_word)DIGIT_BIT);
  }

  /* now we have to propagate the carries and
   * shift the words downward [all those least
   * significant digits we zeroed].
   */
  {
    mp_digit *tmpx;
    mp_word *_W, *_W1;

    /* nox fix rest of carries */

    /* alias for current word */
    _W1 = W + ix;

    /* alias for next word, where the carry goes */
    _W = W + ++ix;

    for (; ix <= ((n->used * 2) + 1); ix++) {
      *_W++ += *_W1++ >> ((mp_word)DIGIT_BIT);
    }

    /* copy out, A = A/b**n
     *
     * The result is A/b**n but instead of converting from an
     * array of mp_word to mp_digit than calling mp_rshd
     * we just copy them in the right order
     */

    /* alias for destination word */
    tmpx = x->dp;

    /* alias for shifted double precision result */
    _W = W + n->used;

    for (ix = 0; ix < (n->used + 1); ix++) {
      *tmpx++ = (mp_digit)(*_W++ & ((mp_word)MP_MASK));
    }

    /* zero oldused digits, if the input a was larger than
     * m->used+1 we'll have to clear the digits
     */
    for (; ix < olduse; ix++) {
      *tmpx++ = 0;
    }
  }

  /* set the max used and clamp */
  x->used = n->used + 1;
  mp_clamp(x);

  /* if A >= m then A = A - m */
  if (mp_cmp_mag(x, n) != MP_LT) {
    return s_mp_sub(x, n, x);
  }
  return MP_OKAY;
}

/* Fast (comba) multiplier
 *
 * This is the fast column-array [comba] multiplier.  It is
 * designed to compute the columns of the product first
 * then handle the carries afterwards.  This has the effect
 * of making the nested loops that compute the columns very
 * simple and schedulable on super-scalar processors.
 *
 * This has been modified to produce a variable number of
 * digits of output so if say only a half-product is required
 * you don't have to compute the upper half (a feature
 * required for fast Barrett reduction).
 *
 * Based on Algorithm 14.12 on pp.595 of HAC.
 *
 */
int fast_s_mp_mul_digs(mp_int *a, mp_int *b, mp_int *c, int digs) {
  int olduse, res, pa, ix, iz;
  mp_digit W[MP_WARRAY];
  mp_word _W;

  /* grow the destination as required */
  if (c->alloc < digs) {
    if ((res = mp_grow(c, digs)) != MP_OKAY) {
      return res;
    }
  }

  /* number of output digits to produce */
  pa = MIN(digs, a->used + b->used);

  /* clear the carry */
  _W = 0;
  for (ix = 0; ix < pa; ix++) {
    int tx, ty;
    int iy;
    mp_digit *tmpx, *tmpy;

    /* get offsets into the two bignums */
    ty = MIN(b->used - 1, ix);
    tx = ix - ty;

    /* setup temp aliases */
    tmpx = a->dp + tx;
    tmpy = b->dp + ty;

    /* this is the number of times the loop will iterrate, essentially
   while (tx++ < a->used && ty-- >= 0) { ... }
 */
    iy = MIN(a->used - tx, ty + 1);

    /* execute loop */
    for (iz = 0; iz < iy; ++iz) {
      _W += ((mp_word)*tmpx++) * ((mp_word)*tmpy--);
    }

    /* store term */
    W[ix] = ((mp_digit)_W) & MP_MASK;

    /* make next carry */
    _W = _W >> ((mp_word)DIGIT_BIT);
  }

  /* setup dest */
  olduse = c->used;
  c->used = pa;

  {
    mp_digit *tmpc;
    tmpc = c->dp;
    for (ix = 0; ix < (pa + 1); ix++) {
      /* now extract the previous digit [below the carry] */
      *tmpc++ = W[ix];
    }

    /* clear unused digits [that existed in the old copy of c] */
    for (; ix < olduse; ix++) {
      *tmpc++ = 0;
    }
  }
  mp_clamp(c);
  return MP_OKAY;
}

/* this is a modified version of fast_s_mul_digs that only produces
 * output digits *above* digs.  See the comments for fast_s_mul_digs
 * to see how it works.
 *
 * This is used in the Barrett reduction since for one of the multiplications
 * only the higher digits were needed.  This essentially halves the work.
 *
 * Based on Algorithm 14.12 on pp.595 of HAC.
 */
int fast_s_mp_mul_high_digs(mp_int *a, mp_int *b, mp_int *c, int digs) {
  int olduse, res, pa, ix, iz;
  mp_digit W[MP_WARRAY];
  mp_word _W;

  /* grow the destination as required */
  pa = a->used + b->used;
  if (c->alloc < pa) {
    if ((res = mp_grow(c, pa)) != MP_OKAY) {
      return res;
    }
  }

  /* number of output digits to produce */
  pa = a->used + b->used;
  _W = 0;
  for (ix = digs; ix < pa; ix++) {
    int tx, ty, iy;
    mp_digit *tmpx, *tmpy;

    /* get offsets into the two bignums */
    ty = MIN(b->used - 1, ix);
    tx = ix - ty;

    /* setup temp aliases */
    tmpx = a->dp + tx;
    tmpy = b->dp + ty;

    /* this is the number of times the loop will iterrate, essentially its
   while (tx++ < a->used && ty-- >= 0) { ... }
 */
    iy = MIN(a->used - tx, ty + 1);

    /* execute loop */
    for (iz = 0; iz < iy; iz++) {
      _W += ((mp_word)*tmpx++) * ((mp_word)*tmpy--);
    }

    /* store term */
    W[ix] = ((mp_digit)_W) & MP_MASK;

    /* make next carry */
    _W = _W >> ((mp_word)DIGIT_BIT);
  }

  /* setup dest */
  olduse = c->used;
  c->used = pa;

  {
    mp_digit *tmpc;

    tmpc = c->dp + digs;
    for (ix = digs; ix < pa; ix++) {
      /* now extract the previous digit [below the carry] */
      *tmpc++ = W[ix];
    }

    /* clear unused digits [that existed in the old copy of c] */
    for (; ix < olduse; ix++) {
      *tmpc++ = 0;
    }
  }
  mp_clamp(c);
  return MP_OKAY;
}

/* the jist of squaring...
 * you do like mult except the offset of the tmpx [one that
 * starts closer to zero] can't equal the offset of tmpy.
 * So basically you set up iy like before then you min it with
 * (ty-tx) so that it never happens.  You double all those
 * you add in the inner loop

   After that loop you do the squares and add them in.
 */

int fast_s_mp_sqr(mp_int *a, mp_int *b) {
  int olduse, res, pa, ix, iz;
  mp_digit W[MP_WARRAY], *tmpx;
  mp_word W1;

  /* grow the destination as required */
  pa = a->used + a->used;
  if (b->alloc < pa) {
    if ((res = mp_grow(b, pa)) != MP_OKAY) {
      return res;
    }
  }

  /* number of output digits to produce */
  W1 = 0;
  for (ix = 0; ix < pa; ix++) {
    int tx, ty, iy;
    mp_word _W;
    mp_digit *tmpy;

    /* clear counter */
    _W = 0;

    /* get offsets into the two bignums */
    ty = MIN(a->used - 1, ix);
    tx = ix - ty;

    /* setup temp aliases */
    tmpx = a->dp + tx;
    tmpy = a->dp + ty;

    /* this is the number of times the loop will iterrate, essentially
   while (tx++ < a->used && ty-- >= 0) { ... }
 */
    iy = MIN(a->used - tx, ty + 1);

    /* now for squaring tx can never equal ty
     * we halve the distance since they approach at a rate of 2x
     * and we have to round because odd cases need to be executed
     */
    iy = MIN(iy, ((ty - tx) + 1) >> 1);

    /* execute loop */
    for (iz = 0; iz < iy; iz++) {
      _W += ((mp_word)*tmpx++) * ((mp_word)*tmpy--);
    }

    /* double the inner product and add carry */
    _W = _W + _W + W1;

    /* even columns have the square term in them */
    if ((ix & 1) == 0) {
      _W += ((mp_word)a->dp[ix >> 1]) * ((mp_word)a->dp[ix >> 1]);
    }

    /* store it */
    W[ix] = (mp_digit)(_W & MP_MASK);

    /* make next carry */
    W1 = _W >> ((mp_word)DIGIT_BIT);
  }

  /* setup dest */
  olduse = b->used;
  b->used = a->used + a->used;

  {
    mp_digit *tmpb;
    tmpb = b->dp;
    for (ix = 0; ix < pa; ix++) {
      *tmpb++ = W[ix] & MP_MASK;
    }

    /* clear unused digits [that existed in the old copy of c] */
    for (; ix < olduse; ix++) {
      *tmpb++ = 0;
    }
  }
  mp_clamp(b);
  return MP_OKAY;
}

/* computes a = 2**b
 *
 * Simple algorithm which zeroes the int, grows it then just sets one bit
 * as required.
 */
int mp_2expt(mp_int *a, int b) {
  int res;

  /* zero a as per default */
  mp_zero(a);

  /* grow a to accomodate the single bit */
  if ((res = mp_grow(a, (b / DIGIT_BIT) + 1)) != MP_OKAY) {
    return res;
  }

  /* set the used count of where the bit will go */
  a->used = (b / DIGIT_BIT) + 1;

  /* put the single bit in its place */
  a->dp[b / DIGIT_BIT] = ((mp_digit)1) << (b % DIGIT_BIT);

  return MP_OKAY;
}

/* b = |a|
 *
 * Simple function copies the input and fixes the sign to positive
 */
int mp_abs(mp_int *a, mp_int *b) {
  int res;

  /* copy a to b */
  if (a != b) {
    if ((res = mp_copy(a, b)) != MP_OKAY) {
      return res;
    }
  }

  /* force the sign of b to positive */
  b->sign = MP_ZPOS;

  return MP_OKAY;
}

/* high level addition (handles signs) */
int mp_add(mp_int *a, mp_int *b, mp_int *c) {
  int sa, sb, res;

  /* get sign of both inputs */
  sa = a->sign;
  sb = b->sign;

  /* handle two cases, not four */
  if (sa == sb) {
    /* both positive or both negative */
    /* add their magnitudes, copy the sign */
    c->sign = sa;
    res = s_mp_add(a, b, c);
  } else {
    /* one positive, the other negative */
    /* subtract the one with the greater magnitude from */
    /* the one of the lesser magnitude.  The result gets */
    /* the sign of the one with the greater magnitude. */
    if (mp_cmp_mag(a, b) == MP_LT) {
      c->sign = sb;
      res = s_mp_sub(b, a, c);
    } else {
      c->sign = sa;
      res = s_mp_sub(a, b, c);
    }
  }
  return res;
}

/* single digit addition */
int mp_add_d(mp_int *a, mp_digit b, mp_int *c) {
  int res, ix, oldused;
  mp_digit *tmpa, *tmpc, mu;

  /* grow c as required */
  if (c->alloc < (a->used + 1)) {
    if ((res = mp_grow(c, a->used + 1)) != MP_OKAY) {
      return res;
    }
  }

  /* if a is negative and |a| >= b, call c = |a| - b */
  if ((a->sign == MP_NEG) && ((a->used > 1) || (a->dp[0] >= b))) {
    /* temporarily fix sign of a */
    a->sign = MP_ZPOS;

    /* c = |a| - b */
    res = mp_sub_d(a, b, c);

    /* fix sign  */
    a->sign = c->sign = MP_NEG;

    /* clamp */
    mp_clamp(c);

    return res;
  }

  /* old number of used digits in c */
  oldused = c->used;

  /* sign always positive */
  c->sign = MP_ZPOS;

  /* source alias */
  tmpa = a->dp;

  /* destination alias */
  tmpc = c->dp;

  /* if a is positive */
  if (a->sign == MP_ZPOS) {
    /* add digit, after this we're propagating
     * the carry.
     */
    *tmpc = *tmpa++ + b;
    mu = *tmpc >> DIGIT_BIT;
    *tmpc++ &= MP_MASK;

    /* now handle rest of the digits */
    for (ix = 1; ix < a->used; ix++) {
      *tmpc = *tmpa++ + mu;
      mu = *tmpc >> DIGIT_BIT;
      *tmpc++ &= MP_MASK;
    }
    /* set final carry */
    ix++;
    *tmpc++ = mu;

    /* setup size */
    c->used = a->used + 1;
  } else {
    /* a was negative and |a| < b */
    c->used = 1;

    /* the result is a single digit */
    if (a->used == 1) {
      *tmpc++ = b - a->dp[0];
    } else {
      *tmpc++ = b;
    }

    /* setup count so the clearing of oldused
     * can fall through correctly
     */
    ix = 1;
  }

  /* now zero to oldused */
  while (ix++ < oldused) {
    *tmpc++ = 0;
  }
  mp_clamp(c);

  return MP_OKAY;
}

/* trim unused digits
 *
 * This is used to ensure that leading zero digits are
 * trimed and the leading "used" digit will be non-zero
 * Typically very fast.  Also fixes the sign if there
 * are no more leading digits
 */
void mp_clamp(mp_int *a) {
  /* decrease used while the most significant digit is
   * zero.
   */
  while ((a->used > 0) && (a->dp[a->used - 1] == 0)) {
    --(a->used);
  }

  /* reset the sign flag if used == 0 */
  if (a->used == 0) {
    a->sign = MP_ZPOS;
  }
}

/* clear one (frees)  */
void mp_clear(mp_int *a) {
  int i;

  /* only do anything if a hasn't been freed previously */
  if (a->dp != NULL) {
    /* first zero the digits */
    for (i = 0; i < a->used; i++) {
      a->dp[i] = 0;
    }

    /* free ram */
    XFREE(a->dp);

    /* reset members to make debugging easier */
    a->dp = NULL;
    a->alloc = a->used = 0;
    a->sign = MP_ZPOS;
  }
}

#include <stdarg.h>

void mp_clear_multi(mp_int *mp, ...) {
  mp_int *next_mp = mp;
  va_list args;

  va_start(args, mp);
  while (next_mp != NULL) {
    mp_clear(next_mp);
    next_mp = va_arg(args, mp_int *);
  }
  va_end(args);
}

/* compare two ints (signed)*/
int mp_cmp(mp_int *a, mp_int *b) {
  /* compare based on sign */
  if (a->sign != b->sign) {
    if (a->sign == MP_NEG) {
      return MP_LT;
    } else {
      return MP_GT;
    }
  }

  /* compare digits */
  if (a->sign == MP_NEG) {
    /* if negative compare opposite direction */
    return mp_cmp_mag(b, a);
  } else {
    return mp_cmp_mag(a, b);
  }
}

/* compare a digit */
int mp_cmp_d(mp_int *a, mp_digit b) {
  /* compare based on sign */
  if (a->sign == MP_NEG) {
    return MP_LT;
  }

  /* compare based on magnitude */
  if (a->used > 1) {
    return MP_GT;
  }

  /* compare the only digit of a to b */
  if (a->dp[0] > b) {
    return MP_GT;
  } else if (a->dp[0] < b) {
    return MP_LT;
  } else {
    return MP_EQ;
  }
}

/* compare maginitude of two ints (unsigned) */
int mp_cmp_mag(mp_int *a, mp_int *b) {
  int n;
  mp_digit *tmpa, *tmpb;

  /* compare based on # of non-zero digits */
  if (a->used > b->used) {
    return MP_GT;
  }

  if (a->used < b->used) {
    return MP_LT;
  }

  /* alias for a */
  tmpa = a->dp + (a->used - 1);

  /* alias for b */
  tmpb = b->dp + (a->used - 1);

  /* compare based on digits  */
  for (n = 0; n < a->used; ++n, --tmpa, --tmpb) {
    if (*tmpa > *tmpb) {
      return MP_GT;
    }

    if (*tmpa < *tmpb) {
      return MP_LT;
    }
  }
  return MP_EQ;
}

static const int lnz[16] = {4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

/* Counts the number of lsbs which are zero before the first zero bit */
int mp_cnt_lsb(mp_int *a) {
  int x;
  mp_digit q, qq;

  /* easy out */
  if (mp_iszero(a) == MP_YES) {
    return 0;
  }

  /* scan lower digits until non-zero */
  for (x = 0; (x < a->used) && (a->dp[x] == 0); x++) {
  }
  q = a->dp[x];
  x *= DIGIT_BIT;

  /* now scan this digit until a 1 is found */
  if ((q & 1) == 0) {
    do {
      qq = q & 15;
      x += lnz[qq];
      q >>= 4;
    } while (qq == 0);
  }
  return x;
}

/* copy, b = a */
int mp_copy(mp_int *a, mp_int *b) {
  int res, n;

  /* if dst == src do nothing */
  if (a == b) {
    return MP_OKAY;
  }

  /* grow dest */
  if (b->alloc < a->used) {
    if ((res = mp_grow(b, a->used)) != MP_OKAY) {
      return res;
    }
  }

  /* zero b and copy the parameters over */
  {
    mp_digit *tmpa, *tmpb;

    /* pointer aliases */

    /* source */
    tmpa = a->dp;

    /* destination */
    tmpb = b->dp;

    /* copy all the digits */
    for (n = 0; n < a->used; n++) {
      *tmpb++ = *tmpa++;
    }

    /* clear high digits */
    for (; n < b->used; n++) {
      *tmpb++ = 0;
    }
  }

  /* copy used count and sign */
  b->used = a->used;
  b->sign = a->sign;
  return MP_OKAY;
}

/* returns the number of bits in an int */
int mp_count_bits(mp_int *a) {
  int r;
  mp_digit q;

  /* shortcut */
  if (a->used == 0) {
    return 0;
  }

  /* get number of digits and add that */
  r = (a->used - 1) * DIGIT_BIT;

  /* take the last digit and count the bits in it */
  q = a->dp[a->used - 1];
  while (q > ((mp_digit)0)) {
    ++r;
    q >>= ((mp_digit)1);
  }
  return r;
}

/* integer signed division.
 * c*b + d == a [e.g. a/b, c=quotient, d=remainder]
 * HAC pp.598 Algorithm 14.20
 *
 * Note that the description in HAC is horribly
 * incomplete.  For example, it doesn't consider
 * the case where digits are removed from 'x' in
 * the inner loop.  It also doesn't consider the
 * case that y has fewer than three digits, etc..
 *
 * The overall algorithm is as described as
 * 14.20 from HAC but fixed to treat these cases.
 */
int mp_div(mp_int *a, mp_int *b, mp_int *c, mp_int *d) {
  mp_int q, x, y, t1, t2;
  int res, n, t, i, norm, neg;

  /* is divisor zero ? */
  if (mp_iszero(b) == MP_YES) {
    return MP_VAL;
  }

  /* if a < b then q=0, r = a */
  if (mp_cmp_mag(a, b) == MP_LT) {
    if (d != NULL) {
      res = mp_copy(a, d);
    } else {
      res = MP_OKAY;
    }
    if (c != NULL) {
      mp_zero(c);
    }
    return res;
  }

  if ((res = mp_init_size(&q, a->used + 2)) != MP_OKAY) {
    return res;
  }
  q.used = a->used + 2;

  if ((res = mp_init(&t1)) != MP_OKAY) {
    goto LBL_Q;
  }

  if ((res = mp_init(&t2)) != MP_OKAY) {
    goto LBL_T1;
  }

  if ((res = mp_init_copy(&x, a)) != MP_OKAY) {
    goto LBL_T2;
  }

  if ((res = mp_init_copy(&y, b)) != MP_OKAY) {
    goto LBL_X;
  }

  /* fix the sign */
  neg = (a->sign == b->sign) ? MP_ZPOS : MP_NEG;
  x.sign = y.sign = MP_ZPOS;

  /* normalize both x and y, ensure that y >= b/2, [b == 2**DIGIT_BIT] */
  norm = mp_count_bits(&y) % DIGIT_BIT;
  if (norm < (int)(DIGIT_BIT - 1)) {
    norm = (DIGIT_BIT - 1) - norm;
    if ((res = mp_mul_2d(&x, norm, &x)) != MP_OKAY) {
      goto LBL_Y;
    }
    if ((res = mp_mul_2d(&y, norm, &y)) != MP_OKAY) {
      goto LBL_Y;
    }
  } else {
    norm = 0;
  }

  /* note hac does 0 based, so if used==5 then its 0,1,2,3,4, e.g. use 4 */
  n = x.used - 1;
  t = y.used - 1;

  /* while (x >= y*b**n-t) do { q[n-t] += 1; x -= y*b**{n-t} } */
  if ((res = mp_lshd(&y, n - t)) != MP_OKAY) { /* y = y*b**{n-t} */
    goto LBL_Y;
  }

  while (mp_cmp(&x, &y) != MP_LT) {
    ++(q.dp[n - t]);
    if ((res = mp_sub(&x, &y, &x)) != MP_OKAY) {
      goto LBL_Y;
    }
  }

  /* reset y by shifting it back down */
  mp_rshd(&y, n - t);

  /* step 3. for i from n down to (t + 1) */
  for (i = n; i >= (t + 1); i--) {
    if (i > x.used) {
      continue;
    }

    /* step 3.1 if xi == yt then set q{i-t-1} to b-1,
     * otherwise set q{i-t-1} to (xi*b + x{i-1})/yt */
    if (x.dp[i] == y.dp[t]) {
      q.dp[(i - t) - 1] = ((((mp_digit)1) << DIGIT_BIT) - 1);
    } else {
      mp_word tmp;
      tmp = ((mp_word)x.dp[i]) << ((mp_word)DIGIT_BIT);
      tmp |= ((mp_word)x.dp[i - 1]);
      tmp /= ((mp_word)y.dp[t]);
      if (tmp > (mp_word)MP_MASK) {
        tmp = MP_MASK;
      }
      q.dp[(i - t) - 1] = (mp_digit)(tmp & (mp_word)(MP_MASK));
    }

    /* while (q{i-t-1} * (yt * b + y{t-1})) >
         xi * b**2 + xi-1 * b + xi-2

   do q{i-t-1} -= 1;
 */
    q.dp[(i - t) - 1] = (q.dp[(i - t) - 1] + 1) & MP_MASK;
    do {
      q.dp[(i - t) - 1] = (q.dp[(i - t) - 1] - 1) & MP_MASK;

      /* find left hand */
      mp_zero(&t1);
      t1.dp[0] = ((t - 1) < 0) ? 0 : y.dp[t - 1];
      t1.dp[1] = y.dp[t];
      t1.used = 2;
      if ((res = mp_mul_d(&t1, q.dp[(i - t) - 1], &t1)) != MP_OKAY) {
        goto LBL_Y;
      }

      /* find right hand */
      t2.dp[0] = ((i - 2) < 0) ? 0 : x.dp[i - 2];
      t2.dp[1] = ((i - 1) < 0) ? 0 : x.dp[i - 1];
      t2.dp[2] = x.dp[i];
      t2.used = 3;
    } while (mp_cmp_mag(&t1, &t2) == MP_GT);

    /* step 3.3 x = x - q{i-t-1} * y * b**{i-t-1} */
    if ((res = mp_mul_d(&y, q.dp[(i - t) - 1], &t1)) != MP_OKAY) {
      goto LBL_Y;
    }

    if ((res = mp_lshd(&t1, (i - t) - 1)) != MP_OKAY) {
      goto LBL_Y;
    }

    if ((res = mp_sub(&x, &t1, &x)) != MP_OKAY) {
      goto LBL_Y;
    }

    /* if x < 0 then { x = x + y*b**{i-t-1}; q{i-t-1} -= 1; } */
    if (x.sign == MP_NEG) {
      if ((res = mp_copy(&y, &t1)) != MP_OKAY) {
        goto LBL_Y;
      }
      if ((res = mp_lshd(&t1, (i - t) - 1)) != MP_OKAY) {
        goto LBL_Y;
      }
      if ((res = mp_add(&x, &t1, &x)) != MP_OKAY) {
        goto LBL_Y;
      }

      q.dp[(i - t) - 1] = (q.dp[(i - t) - 1] - 1UL) & MP_MASK;
    }
  }

  /* now q is the quotient and x is the remainder
   * [which we have to normalize]
   */

  /* get sign before writing to c */
  x.sign = (x.used == 0) ? MP_ZPOS : a->sign;

  if (c != NULL) {
    mp_clamp(&q);
    mp_exch(&q, c);
    c->sign = neg;
  }

  if (d != NULL) {
    if ((res = mp_div_2d(&x, norm, &x, NULL)) != MP_OKAY) {
      goto LBL_Y;
    }
    mp_exch(&x, d);
  }

  res = MP_OKAY;

LBL_Y:
  mp_clear(&y);
LBL_X:
  mp_clear(&x);
LBL_T2:
  mp_clear(&t2);
LBL_T1:
  mp_clear(&t1);
LBL_Q:
  mp_clear(&q);
  return res;
}

/* b = a/2 */
int mp_div_2(mp_int *a, mp_int *b) {
  int x, res, oldused;

  /* copy */
  if (b->alloc < a->used) {
    if ((res = mp_grow(b, a->used)) != MP_OKAY) {
      return res;
    }
  }

  oldused = b->used;
  b->used = a->used;
  {
    mp_digit r, rr, *tmpa, *tmpb;

    /* source alias */
    tmpa = a->dp + b->used - 1;

    /* dest alias */
    tmpb = b->dp + b->used - 1;

    /* carry */
    r = 0;
    for (x = b->used - 1; x >= 0; x--) {
      /* get the carry for the next iteration */
      rr = *tmpa & 1;

      /* shift the current digit, add in carry and store */
      *tmpb-- = (*tmpa-- >> 1) | (r << (DIGIT_BIT - 1));

      /* forward carry to next iteration */
      r = rr;
    }

    /* zero excess digits */
    tmpb = b->dp + b->used;
    for (x = b->used; x < oldused; x++) {
      *tmpb++ = 0;
    }
  }
  b->sign = a->sign;
  mp_clamp(b);
  return MP_OKAY;
}

/* shift right by a certain bit count (store quotient in c, optional remainder
 * in d) */
int mp_div_2d(mp_int *a, int b, mp_int *c, mp_int *d) {
  mp_digit D, r, rr;
  int x, res;
  mp_int t;

  /* if the shift count is <= 0 then we do no work */
  if (b <= 0) {
    res = mp_copy(a, c);
    if (d != NULL) {
      mp_zero(d);
    }
    return res;
  }

  if ((res = mp_init(&t)) != MP_OKAY) {
    return res;
  }

  /* get the remainder */
  if (d != NULL) {
    if ((res = mp_mod_2d(a, b, &t)) != MP_OKAY) {
      mp_clear(&t);
      return res;
    }
  }

  /* copy */
  if ((res = mp_copy(a, c)) != MP_OKAY) {
    mp_clear(&t);
    return res;
  }

  /* shift by as many digits in the bit count */
  if (b >= (int)DIGIT_BIT) {
    mp_rshd(c, b / DIGIT_BIT);
  }

  /* shift any bit count < DIGIT_BIT */
  D = (mp_digit)(b % DIGIT_BIT);
  if (D != 0) {
    mp_digit *tmpc, mask, shift;

    /* mask */
    mask = (((mp_digit)1) << D) - 1;

    /* shift for lsb */
    shift = DIGIT_BIT - D;

    /* alias */
    tmpc = c->dp + (c->used - 1);

    /* carry */
    r = 0;
    for (x = c->used - 1; x >= 0; x--) {
      /* get the lower  bits of this word in a temp */
      rr = *tmpc & mask;

      /* shift the current word and mix in the carry bits from the previous word
       */
      *tmpc = (*tmpc >> D) | (r << shift);
      --tmpc;

      /* set the carry to the carry bits of the current word found above */
      r = rr;
    }
  }
  mp_clamp(c);
  if (d != NULL) {
    mp_exch(&t, d);
  }
  mp_clear(&t);
  return MP_OKAY;
}

/* divide by three (based on routine from MPI and the GMP manual) */
int mp_div_3(mp_int *a, mp_int *c, mp_digit *d) {
  mp_int q;
  mp_word w, t;
  mp_digit b;
  int res, ix;

  /* b = 2**DIGIT_BIT / 3 */
  b = (((mp_word)1) << ((mp_word)DIGIT_BIT)) / ((mp_word)3);

  if ((res = mp_init_size(&q, a->used)) != MP_OKAY) {
    return res;
  }

  q.used = a->used;
  q.sign = a->sign;
  w = 0;
  for (ix = a->used - 1; ix >= 0; ix--) {
    w = (w << ((mp_word)DIGIT_BIT)) | ((mp_word)a->dp[ix]);

    if (w >= 3) {
      /* multiply w by [1/3] */
      t = (w * ((mp_word)b)) >> ((mp_word)DIGIT_BIT);

      /* now subtract 3 * [w/3] from w, to get the remainder */
      w -= t + t + t;

      /* fixup the remainder as required since
       * the optimization is not exact.
       */
      while (w >= 3) {
        t += 1;
        w -= 3;
      }
    } else {
      t = 0;
    }
    q.dp[ix] = (mp_digit)t;
  }

  /* [optional] store the remainder */
  if (d != NULL) {
    *d = (mp_digit)w;
  }

  /* [optional] store the quotient */
  if (c != NULL) {
    mp_clamp(&q);
    mp_exch(&q, c);
  }
  mp_clear(&q);

  return res;
}

static int s_is_power_of_two(mp_digit b, int *p) {
  int x;

  /* fast return if no power of two */
  if ((b == 0) || ((b & (b - 1)) != 0)) {
    return 0;
  }

  for (x = 0; x < DIGIT_BIT; x++) {
    if (b == (((mp_digit)1) << x)) {
      *p = x;
      return 1;
    }
  }
  return 0;
}

/* single digit division (based on routine from MPI) */
int mp_div_d(mp_int *a, mp_digit b, mp_int *c, mp_digit *d) {
  mp_int q;
  mp_word w;
  mp_digit t;
  int res, ix;

  /* cannot divide by zero */
  if (b == 0) {
    return MP_VAL;
  }

  /* quick outs */
  if ((b == 1) || (mp_iszero(a) == MP_YES)) {
    if (d != NULL) {
      *d = 0;
    }
    if (c != NULL) {
      return mp_copy(a, c);
    }
    return MP_OKAY;
  }

  /* power of two ? */
  if (s_is_power_of_two(b, &ix) == 1) {
    if (d != NULL) {
      *d = a->dp[0] & ((((mp_digit)1) << ix) - 1);
    }
    if (c != NULL) {
      return mp_div_2d(a, ix, c, NULL);
    }
    return MP_OKAY;
  }

  /* three? */
  if (b == 3) {
    return mp_div_3(a, c, d);
  }

  /* no easy answer [c'est la vie].  Just division */
  if ((res = mp_init_size(&q, a->used)) != MP_OKAY) {
    return res;
  }

  q.used = a->used;
  q.sign = a->sign;
  w = 0;
  for (ix = a->used - 1; ix >= 0; ix--) {
    w = (w << ((mp_word)DIGIT_BIT)) | ((mp_word)a->dp[ix]);

    if (w >= b) {
      t = (mp_digit)(w / b);
      w -= ((mp_word)t) * ((mp_word)b);
    } else {
      t = 0;
    }
    q.dp[ix] = (mp_digit)t;
  }

  if (d != NULL) {
    *d = (mp_digit)w;
  }

  if (c != NULL) {
    mp_clamp(&q);
    mp_exch(&q, c);
  }
  mp_clear(&q);

  return res;
}

/* determines if a number is a valid DR modulus */
int mp_dr_is_modulus(mp_int *a) {
  int ix;

  /* must be at least two digits */
  if (a->used < 2) {
    return 0;
  }

  /* must be of the form b**k - a [a <= b] so all
   * but the first digit must be equal to -1 (mod b).
   */
  for (ix = 1; ix < a->used; ix++) {
    if (a->dp[ix] != MP_MASK) {
      return 0;
    }
  }
  return 1;
}

/* reduce "x" in place modulo "n" using the Diminished Radix algorithm.
 *
 * Based on algorithm from the paper
 *
 * "Generating Efficient Primes for Discrete Log Cryptosystems"
 *                 Chae Hoon Lim, Pil Joong Lee,
 *          POSTECH Information Research Laboratories
 *
 * The modulus must be of a special format [see manual]
 *
 * Has been modified to use algorithm 7.10 from the LTM book instead
 *
 * Input x must be in the range 0 <= x <= (n-1)**2
 */
int mp_dr_reduce(mp_int *x, mp_int *n, mp_digit k) {
  int err, i, m;
  mp_word r;
  mp_digit mu, *tmpx1, *tmpx2;

  /* m = digits in modulus */
  m = n->used;

  /* ensure that "x" has at least 2m digits */
  if (x->alloc < (m + m)) {
    if ((err = mp_grow(x, m + m)) != MP_OKAY) {
      return err;
    }
  }

  /* top of loop, this is where the code resumes if
   * another reduction pass is required.
   */
top:
  /* aliases for digits */
  /* alias for lower half of x */
  tmpx1 = x->dp;

  /* alias for upper half of x, or x/B**m */
  tmpx2 = x->dp + m;

  /* set carry to zero */
  mu = 0;

  /* compute (x mod B**m) + k * [x/B**m] inline and inplace */
  for (i = 0; i < m; i++) {
    r = (((mp_word)*tmpx2++) * (mp_word)k) + *tmpx1 + mu;
    *tmpx1++ = (mp_digit)(r & MP_MASK);
    mu = (mp_digit)(r >> ((mp_word)DIGIT_BIT));
  }

  /* set final carry */
  *tmpx1++ = mu;

  /* zero words above m */
  for (i = m + 1; i < x->used; i++) {
    *tmpx1++ = 0;
  }

  /* clamp, sub and return */
  mp_clamp(x);

  /* if x >= n then subtract and reduce again
   * Each successive "recursion" makes the input smaller and smaller.
   */
  if (mp_cmp_mag(x, n) != MP_LT) {
    if ((err = s_mp_sub(x, n, x)) != MP_OKAY) {
      return err;
    }
    goto top;
  }
  return MP_OKAY;
}

/* determines the setup value */
void mp_dr_setup(mp_int *a, mp_digit *d) {
  /* the casts are required if DIGIT_BIT is one less than
   * the number of bits in a mp_digit [e.g. DIGIT_BIT==31]
   */
  *d = (mp_digit)((((mp_word)1) << ((mp_word)DIGIT_BIT)) - ((mp_word)a->dp[0]));
}

/* swap the elements of two integers, for cases where you can't simply swap the
 * mp_int pointers around
 */
void mp_exch(mp_int *a, mp_int *b) {
  mp_int t;

  t = *a;
  *a = *b;
  *b = t;
}

/* this is a shell function that calls either the normal or Montgomery
 * exptmod functions.  Originally the call to the montgomery code was
 * embedded in the normal function but that wasted alot of stack space
 * for nothing (since 99% of the time the Montgomery code would be called)
 */
int mp_exptmod(mp_int *G, mp_int *X, mp_int *P, mp_int *Y) {
  int dr;

  /* modulus P must be positive */
  if (P->sign == MP_NEG) {
    return MP_VAL;
  }

  /* if exponent X is negative we have to recurse */
  if (X->sign == MP_NEG) {
    mp_int tmpG, tmpX;
    int err;

    /* first compute 1/G mod P */
    if ((err = mp_init(&tmpG)) != MP_OKAY) {
      return err;
    }
    if ((err = mp_invmod(G, P, &tmpG)) != MP_OKAY) {
      mp_clear(&tmpG);
      return err;
    }

    /* now get |X| */
    if ((err = mp_init(&tmpX)) != MP_OKAY) {
      mp_clear(&tmpG);
      return err;
    }
    if ((err = mp_abs(X, &tmpX)) != MP_OKAY) {
      mp_clear_multi(&tmpG, &tmpX, NULL);
      return err;
    }

    /* and now compute (1/G)**|X| instead of G**X [X < 0] */
    err = mp_exptmod(&tmpG, &tmpX, P, Y);
    mp_clear_multi(&tmpG, &tmpX, NULL);
    return err;
  }

  /* modified diminished radix reduction */
  if (mp_reduce_is_2k_l(P) == MP_YES) {
    return s_mp_exptmod(G, X, P, Y, 1);
  }

  /* is it a DR modulus? */
  dr = mp_dr_is_modulus(P);

  /* if not, is it a unrestricted DR modulus? */
  if (dr == 0) {
    dr = mp_reduce_is_2k(P) << 1;
  }

  /* if the modulus is odd or dr != 0 use the montgomery method */
  if ((mp_isodd(P) == MP_YES) || (dr != 0)) {
    return mp_exptmod_fast(G, X, P, Y, dr);
  } else {
    /* otherwise use the generic Barrett reduction technique */
    return s_mp_exptmod(G, X, P, Y, 0);
  }
}

/* computes Y == G**X mod P, HAC pp.616, Algorithm 14.85
 *
 * Uses a left-to-right k-ary sliding window to compute the modular
 * exponentiation. The value of k changes based on the size of the exponent.
 *
 * Uses Montgomery or Diminished Radix reduction [whichever appropriate]
 */

#define TAB_SIZE 256

int mp_exptmod_fast(mp_int *G, mp_int *X, mp_int *P, mp_int *Y, int redmode) {
  mp_int M[TAB_SIZE], res;
  mp_digit buf, mp;
  int err, bitbuf, bitcpy, bitcnt, mode, digidx, x, y, winsize;

  /* use a pointer to the reduction algorithm.  This allows us to use
   * one of many reduction algorithms without modding the guts of
   * the code with if statements everywhere.
   */
  int (*redux)(mp_int *, mp_int *, mp_digit);

  /* find window size */
  x = mp_count_bits(X);
  if (x <= 7) {
    winsize = 2;
  } else if (x <= 36) {
    winsize = 3;
  } else if (x <= 140) {
    winsize = 4;
  } else if (x <= 450) {
    winsize = 5;
  } else if (x <= 1303) {
    winsize = 6;
  } else if (x <= 3529) {
    winsize = 7;
  } else {
    winsize = 8;
  }

  /* init M array */
  /* init first cell */
  if ((err = mp_init(&M[1])) != MP_OKAY) {
    return err;
  }

  /* now init the second half of the array */
  for (x = 1 << (winsize - 1); x < (1 << winsize); x++) {
    if ((err = mp_init(&M[x])) != MP_OKAY) {
      for (y = 1 << (winsize - 1); y < x; y++) {
        mp_clear(&M[y]);
      }
      mp_clear(&M[1]);
      return err;
    }
  }

  /* determine and setup reduction code */
  if (redmode == 0) {
    /* now setup montgomery  */
    if ((err = mp_montgomery_setup(P, &mp)) != MP_OKAY) {
      goto LBL_M;
    }

    /* automatically pick the comba one if available (saves quite a few
     * calls/ifs) */
    if ((((P->used * 2) + 1) < MP_WARRAY) &&
        (P->used < (1 << ((CHAR_BIT * sizeof(mp_word)) - (2 * DIGIT_BIT))))) {
      redux = fast_mp_montgomery_reduce;
    } else {
      /* use slower baseline Montgomery method */
      redux = mp_montgomery_reduce;
    }
  } else if (redmode == 1) {
    /* setup DR reduction for moduli of the form B**k - b */
    mp_dr_setup(P, &mp);
    redux = mp_dr_reduce;
  } else {
    /* setup DR reduction for moduli of the form 2**k - b */
    if ((err = mp_reduce_2k_setup(P, &mp)) != MP_OKAY) {
      goto LBL_M;
    }
    redux = mp_reduce_2k;
  }

  /* setup result */
  if ((err = mp_init(&res)) != MP_OKAY) {
    goto LBL_M;
  }

  /* create M table
 *

 *
 * The first half of the table is not computed though accept for M[0] and M[1]
 */

  if (redmode == 0) {
    /* now we need R mod m */
    if ((err = mp_montgomery_calc_normalization(&res, P)) != MP_OKAY) {
      goto LBL_RES;
    }

    /* now set M[1] to G * R mod m */
    if ((err = mp_mulmod(G, &res, P, &M[1])) != MP_OKAY) {
      goto LBL_RES;
    }
  } else {
    mp_set(&res, 1);
    if ((err = mp_mod(G, P, &M[1])) != MP_OKAY) {
      goto LBL_RES;
    }
  }

  /* compute the value at M[1<<(winsize-1)] by squaring M[1] (winsize-1) times
   */
  if ((err = mp_copy(&M[1], &M[1 << (winsize - 1)])) != MP_OKAY) {
    goto LBL_RES;
  }

  for (x = 0; x < (winsize - 1); x++) {
    if ((err = mp_sqr(&M[1 << (winsize - 1)], &M[1 << (winsize - 1)])) !=
        MP_OKAY) {
      goto LBL_RES;
    }
    if ((err = redux(&M[1 << (winsize - 1)], P, mp)) != MP_OKAY) {
      goto LBL_RES;
    }
  }

  /* create upper table */
  for (x = (1 << (winsize - 1)) + 1; x < (1 << winsize); x++) {
    if ((err = mp_mul(&M[x - 1], &M[1], &M[x])) != MP_OKAY) {
      goto LBL_RES;
    }
    if ((err = redux(&M[x], P, mp)) != MP_OKAY) {
      goto LBL_RES;
    }
  }

  /* set initial mode and bit cnt */
  mode = 0;
  bitcnt = 1;
  buf = 0;
  digidx = X->used - 1;
  bitcpy = 0;
  bitbuf = 0;

  for (;;) {
    /* grab next digit as required */
    if (--bitcnt == 0) {
      /* if digidx == -1 we are out of digits so break */
      if (digidx == -1) {
        break;
      }
      /* read next digit and reset bitcnt */
      buf = X->dp[digidx--];
      bitcnt = (int)DIGIT_BIT;
    }

    /* grab the next msb from the exponent */
    y = (mp_digit)(buf >> (DIGIT_BIT - 1)) & 1;
    buf <<= (mp_digit)1;

    /* if the bit is zero and mode == 0 then we ignore it
     * These represent the leading zero bits before the first 1 bit
     * in the exponent.  Technically this opt is not required but it
     * does lower the # of trivial squaring/reductions used
     */
    if ((mode == 0) && (y == 0)) {
      continue;
    }

    /* if the bit is zero and mode == 1 then we square */
    if ((mode == 1) && (y == 0)) {
      if ((err = mp_sqr(&res, &res)) != MP_OKAY) {
        goto LBL_RES;
      }
      if ((err = redux(&res, P, mp)) != MP_OKAY) {
        goto LBL_RES;
      }
      continue;
    }

    /* else we add it to the window */
    bitbuf |= (y << (winsize - ++bitcpy));
    mode = 2;

    if (bitcpy == winsize) {
      /* ok window is filled so square as required and multiply  */
      /* square first */
      for (x = 0; x < winsize; x++) {
        if ((err = mp_sqr(&res, &res)) != MP_OKAY) {
          goto LBL_RES;
        }
        if ((err = redux(&res, P, mp)) != MP_OKAY) {
          goto LBL_RES;
        }
      }

      /* then multiply */
      if ((err = mp_mul(&res, &M[bitbuf], &res)) != MP_OKAY) {
        goto LBL_RES;
      }
      if ((err = redux(&res, P, mp)) != MP_OKAY) {
        goto LBL_RES;
      }

      /* empty window and reset */
      bitcpy = 0;
      bitbuf = 0;
      mode = 1;
    }
  }

  /* if bits remain then square/multiply */
  if ((mode == 2) && (bitcpy > 0)) {
    /* square then multiply if the bit is set */
    for (x = 0; x < bitcpy; x++) {
      if ((err = mp_sqr(&res, &res)) != MP_OKAY) {
        goto LBL_RES;
      }
      if ((err = redux(&res, P, mp)) != MP_OKAY) {
        goto LBL_RES;
      }

      /* get next bit of the window */
      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0) {
        /* then multiply */
        if ((err = mp_mul(&res, &M[1], &res)) != MP_OKAY) {
          goto LBL_RES;
        }
        if ((err = redux(&res, P, mp)) != MP_OKAY) {
          goto LBL_RES;
        }
      }
    }
  }

  if (redmode == 0) {
    /* fixup result if Montgomery reduction is used
     * recall that any value in a Montgomery system is
     * actually multiplied by R mod n.  So we have
     * to reduce one more time to cancel out the factor
     * of R.
     */
    if ((err = redux(&res, P, mp)) != MP_OKAY) {
      goto LBL_RES;
    }
  }

  /* swap res with Y */
  mp_exch(&res, Y);
  err = MP_OKAY;
LBL_RES:
  mp_clear(&res);
LBL_M:
  mp_clear(&M[1]);
  for (x = 1 << (winsize - 1); x < (1 << winsize); x++) {
    mp_clear(&M[x]);
  }
  return err;
}

/* Greatest Common Divisor using the binary method */
int mp_gcd(mp_int *a, mp_int *b, mp_int *c) {
  mp_int u, v;
  int k, u_lsb, v_lsb, res;

  /* either zero than gcd is the largest */
  if (mp_iszero(a) == MP_YES) {
    return mp_abs(b, c);
  }
  if (mp_iszero(b) == MP_YES) {
    return mp_abs(a, c);
  }

  /* get copies of a and b we can modify */
  if ((res = mp_init_copy(&u, a)) != MP_OKAY) {
    return res;
  }

  if ((res = mp_init_copy(&v, b)) != MP_OKAY) {
    goto LBL_U;
  }

  /* must be positive for the remainder of the algorithm */
  u.sign = v.sign = MP_ZPOS;

  /* B1.  Find the common power of two for u and v */
  u_lsb = mp_cnt_lsb(&u);
  v_lsb = mp_cnt_lsb(&v);
  k = MIN(u_lsb, v_lsb);

  if (k > 0) {
    /* divide the power of two out */
    if ((res = mp_div_2d(&u, k, &u, NULL)) != MP_OKAY) {
      goto LBL_V;
    }

    if ((res = mp_div_2d(&v, k, &v, NULL)) != MP_OKAY) {
      goto LBL_V;
    }
  }

  /* divide any remaining factors of two out */
  if (u_lsb != k) {
    if ((res = mp_div_2d(&u, u_lsb - k, &u, NULL)) != MP_OKAY) {
      goto LBL_V;
    }
  }

  if (v_lsb != k) {
    if ((res = mp_div_2d(&v, v_lsb - k, &v, NULL)) != MP_OKAY) {
      goto LBL_V;
    }
  }

  while (mp_iszero(&v) == MP_NO) {
    /* make sure v is the largest */
    if (mp_cmp_mag(&u, &v) == MP_GT) {
      /* swap u and v to make sure v is >= u */
      mp_exch(&u, &v);
    }

    /* subtract smallest from largest */
    if ((res = s_mp_sub(&v, &u, &v)) != MP_OKAY) {
      goto LBL_V;
    }

    /* Divide out all factors of two */
    if ((res = mp_div_2d(&v, mp_cnt_lsb(&v), &v, NULL)) != MP_OKAY) {
      goto LBL_V;
    }
  }

  /* multiply by 2**k which we divided out at the beginning */
  if ((res = mp_mul_2d(&u, k, c)) != MP_OKAY) {
    goto LBL_V;
  }
  c->sign = MP_ZPOS;
  res = MP_OKAY;
LBL_V:
  mp_clear(&u);
LBL_U:
  mp_clear(&v);
  return res;
}

/* get the lower 32-bits of an mp_int */
unsigned long mp_get_int(mp_int *a) {
  int i;
  mp_min_u32 res;

  if (a->used == 0) {
    return 0;
  }

  /* get number of digits of the lsb we have to read */
  i = MIN(a->used, (int)(((sizeof(unsigned long) * CHAR_BIT) + DIGIT_BIT - 1) /
                         DIGIT_BIT)) -
      1;

  /* get most significant digit of result */
  res = DIGIT(a, i);

  while (--i >= 0) {
    res = (res << DIGIT_BIT) | DIGIT(a, i);
  }

  /* force result to 32-bits always so it is consistent on non 32-bit platforms
   */
  return res & 0xFFFFFFFFUL;
}

/* grow as required */
int mp_grow(mp_int *a, int size) {
  int i;
  mp_digit *tmp;

  /* if the alloc size is smaller alloc more ram */
  if (a->alloc < size) {
    /* ensure there are always at least MP_PREC digits extra on top */
    size += (MP_PREC * 2) - (size % MP_PREC);

    /* reallocate the array a->dp
     *
     * We store the return in a temporary variable
     * in case the operation failed we don't want
     * to overwrite the dp member of a.
     */
    tmp = OPT_CAST(mp_digit) XREALLOC(a->dp, sizeof(mp_digit) * size);
    if (tmp == NULL) {
      /* reallocation failed but "a" is still valid [can be freed] */
      return MP_MEM;
    }

    /* reallocation succeeded so set a->dp */
    a->dp = tmp;

    /* zero excess digits */
    i = a->alloc;
    a->alloc = size;
    for (; i < a->alloc; i++) {
      a->dp[i] = 0;
    }
  }
  return MP_OKAY;
}

/* init a new mp_int */
int mp_init(mp_int *a) {
  int i;

  /* allocate memory required and clear it */
  a->dp = OPT_CAST(mp_digit) XMALLOC(sizeof(mp_digit) * MP_PREC);
  if (a->dp == NULL) {
    return MP_MEM;
  }

  /* set the digits to zero */
  for (i = 0; i < MP_PREC; i++) {
    a->dp[i] = 0;
  }

  /* set the used to zero, allocated digits to the default precision
   * and sign to positive */
  a->used = 0;
  a->alloc = MP_PREC;
  a->sign = MP_ZPOS;

  return MP_OKAY;
}

/* creates "a" then copies b into it */
int mp_init_copy(mp_int *a, mp_int *b) {
  int res;

  if ((res = mp_init_size(a, b->used)) != MP_OKAY) {
    return res;
  }
  return mp_copy(b, a);
}

int mp_init_multi(mp_int *mp, ...) {
  mp_err res = MP_OKAY; /* Assume ok until proven otherwise */
  int n = 0;            /* Number of ok inits */
  mp_int *cur_arg = mp;
  va_list args;

  va_start(args, mp); /* init args to next argument from caller */
  while (cur_arg != NULL) {
    if (mp_init(cur_arg) != MP_OKAY) {
      /* Oops - error! Back-track and mp_clear what we already
   succeeded in init-ing, then return error.
 */
      va_list clean_args;

      /* end the current list */
      va_end(args);

      /* now start cleaning up */
      cur_arg = mp;
      va_start(clean_args, mp);
      while (n-- != 0) {
        mp_clear(cur_arg);
        cur_arg = va_arg(clean_args, mp_int *);
      }
      va_end(clean_args);
      res = MP_MEM;
      break;
    }
    n++;
    cur_arg = va_arg(args, mp_int *);
  }
  va_end(args);
  return res; /* Assumed ok, if error flagged above. */
}

/* init an mp_init for a given size */
int mp_init_size(mp_int *a, int size) {
  int x;

  /* pad size so there are always extra digits */
  size += (MP_PREC * 2) - (size % MP_PREC);

  /* alloc mem */
  a->dp = OPT_CAST(mp_digit) XMALLOC(sizeof(mp_digit) * size);
  if (a->dp == NULL) {
    return MP_MEM;
  }

  /* set the members */
  a->used = 0;
  a->alloc = size;
  a->sign = MP_ZPOS;

  /* zero the digits */
  for (x = 0; x < size; x++) {
    a->dp[x] = 0;
  }

  return MP_OKAY;
}

/* hac 14.61, pp608 */
int mp_invmod(mp_int *a, mp_int *b, mp_int *c) {
  /* b cannot be negative */
  if ((b->sign == MP_NEG) || (mp_iszero(b) == MP_YES)) {
    return MP_VAL;
  }

  /* if the modulus is odd we can use a faster routine instead */
  if (mp_isodd(b) == MP_YES) {
    return fast_mp_invmod(a, b, c);
  }

  return mp_invmod_slow(a, b, c);
}

/* hac 14.61, pp608 */
int mp_invmod_slow(mp_int *a, mp_int *b, mp_int *c) {
  mp_int x, y, u, v, A, B, C, D;
  int res;

  /* b cannot be negative */
  if ((b->sign == MP_NEG) || (mp_iszero(b) == MP_YES)) {
    return MP_VAL;
  }

  /* init temps */
  if ((res = mp_init_multi(&x, &y, &u, &v, &A, &B, &C, &D, NULL)) != MP_OKAY) {
    return res;
  }

  /* x = a, y = b */
  if ((res = mp_mod(a, b, &x)) != MP_OKAY) {
    goto LBL_ERR;
  }
  if ((res = mp_copy(b, &y)) != MP_OKAY) {
    goto LBL_ERR;
  }

  /* 2. [modified] if x,y are both even then return an error! */
  if ((mp_iseven(&x) == MP_YES) && (mp_iseven(&y) == MP_YES)) {
    res = MP_VAL;
    goto LBL_ERR;
  }

  /* 3. u=x, v=y, A=1, B=0, C=0,D=1 */
  if ((res = mp_copy(&x, &u)) != MP_OKAY) {
    goto LBL_ERR;
  }
  if ((res = mp_copy(&y, &v)) != MP_OKAY) {
    goto LBL_ERR;
  }
  mp_set(&A, 1);
  mp_set(&D, 1);

top:
  /* 4.  while u is even do */
  while (mp_iseven(&u) == MP_YES) {
    /* 4.1 u = u/2 */
    if ((res = mp_div_2(&u, &u)) != MP_OKAY) {
      goto LBL_ERR;
    }
    /* 4.2 if A or B is odd then */
    if ((mp_isodd(&A) == MP_YES) || (mp_isodd(&B) == MP_YES)) {
      /* A = (A+y)/2, B = (B-x)/2 */
      if ((res = mp_add(&A, &y, &A)) != MP_OKAY) {
        goto LBL_ERR;
      }
      if ((res = mp_sub(&B, &x, &B)) != MP_OKAY) {
        goto LBL_ERR;
      }
    }
    /* A = A/2, B = B/2 */
    if ((res = mp_div_2(&A, &A)) != MP_OKAY) {
      goto LBL_ERR;
    }
    if ((res = mp_div_2(&B, &B)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* 5.  while v is even do */
  while (mp_iseven(&v) == MP_YES) {
    /* 5.1 v = v/2 */
    if ((res = mp_div_2(&v, &v)) != MP_OKAY) {
      goto LBL_ERR;
    }
    /* 5.2 if C or D is odd then */
    if ((mp_isodd(&C) == MP_YES) || (mp_isodd(&D) == MP_YES)) {
      /* C = (C+y)/2, D = (D-x)/2 */
      if ((res = mp_add(&C, &y, &C)) != MP_OKAY) {
        goto LBL_ERR;
      }
      if ((res = mp_sub(&D, &x, &D)) != MP_OKAY) {
        goto LBL_ERR;
      }
    }
    /* C = C/2, D = D/2 */
    if ((res = mp_div_2(&C, &C)) != MP_OKAY) {
      goto LBL_ERR;
    }
    if ((res = mp_div_2(&D, &D)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* 6.  if u >= v then */
  if (mp_cmp(&u, &v) != MP_LT) {
    /* u = u - v, A = A - C, B = B - D */
    if ((res = mp_sub(&u, &v, &u)) != MP_OKAY) {
      goto LBL_ERR;
    }

    if ((res = mp_sub(&A, &C, &A)) != MP_OKAY) {
      goto LBL_ERR;
    }

    if ((res = mp_sub(&B, &D, &B)) != MP_OKAY) {
      goto LBL_ERR;
    }
  } else {
    /* v - v - u, C = C - A, D = D - B */
    if ((res = mp_sub(&v, &u, &v)) != MP_OKAY) {
      goto LBL_ERR;
    }

    if ((res = mp_sub(&C, &A, &C)) != MP_OKAY) {
      goto LBL_ERR;
    }

    if ((res = mp_sub(&D, &B, &D)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* if not zero goto step 4 */
  if (mp_iszero(&u) == MP_NO) goto top;

  /* now a = C, b = D, gcd == g*v */

  /* if v != 1 then there is no inverse */
  if (mp_cmp_d(&v, 1) != MP_EQ) {
    res = MP_VAL;
    goto LBL_ERR;
  }

  /* if its too low */
  while (mp_cmp_d(&C, 0) == MP_LT) {
    if ((res = mp_add(&C, b, &C)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* too big */
  while (mp_cmp_mag(&C, b) != MP_LT) {
    if ((res = mp_sub(&C, b, &C)) != MP_OKAY) {
      goto LBL_ERR;
    }
  }

  /* C is now the inverse */
  mp_exch(&C, c);
  res = MP_OKAY;
LBL_ERR:
  mp_clear_multi(&x, &y, &u, &v, &A, &B, &C, &D, NULL);
  return res;
}

/* computes the jacobi c = (a | n) (or Legendre if n is prime)
 * HAC pp. 73 Algorithm 2.149
 * HAC is wrong here, as the special case of (0 | 1) is not
 * handled correctly.
 */
int mp_jacobi(mp_int *a, mp_int *n, int *c) {
  mp_int a1, p1;
  int k, s, r, res;
  mp_digit residue;

  /* if n <= 0 return MP_VAL */
  if (mp_cmp_d(n, 0) != MP_GT) {
    return MP_VAL;
  }

  /* step 1. handle case of a == 0 */
  if (mp_iszero(a) == MP_YES) {
    /* special case of a == 0 and n == 1 */
    if (mp_cmp_d(n, 1) == MP_EQ) {
      *c = 1;
    } else {
      *c = 0;
    }
    return MP_OKAY;
  }

  /* step 2.  if a == 1, return 1 */
  if (mp_cmp_d(a, 1) == MP_EQ) {
    *c = 1;
    return MP_OKAY;
  }

  /* default */
  s = 0;

  /* step 3.  write a = a1 * 2**k  */
  if ((res = mp_init_copy(&a1, a)) != MP_OKAY) {
    return res;
  }

  if ((res = mp_init(&p1)) != MP_OKAY) {
    goto LBL_A1;
  }

  /* divide out larger power of two */
  k = mp_cnt_lsb(&a1);
  if ((res = mp_div_2d(&a1, k, &a1, NULL)) != MP_OKAY) {
    goto LBL_P1;
  }

  /* step 4.  if e is even set s=1 */
  if ((k & 1) == 0) {
    s = 1;
  } else {
    /* else set s=1 if p = 1/7 (mod 8) or s=-1 if p = 3/5 (mod 8) */
    residue = n->dp[0] & 7;

    if ((residue == 1) || (residue == 7)) {
      s = 1;
    } else if ((residue == 3) || (residue == 5)) {
      s = -1;
    }
  }

  /* step 5.  if p == 3 (mod 4) *and* a1 == 3 (mod 4) then s = -s */
  if (((n->dp[0] & 3) == 3) && ((a1.dp[0] & 3) == 3)) {
    s = -s;
  }

  /* if a1 == 1 we're done */
  if (mp_cmp_d(&a1, 1) == MP_EQ) {
    *c = s;
  } else {
    /* n1 = n mod a1 */
    if ((res = mp_mod(n, &a1, &p1)) != MP_OKAY) {
      goto LBL_P1;
    }
    if ((res = mp_jacobi(&p1, &a1, &r)) != MP_OKAY) {
      goto LBL_P1;
    }
    *c = s * r;
  }

  /* done */
  res = MP_OKAY;
LBL_P1:
  mp_clear(&p1);
LBL_A1:
  mp_clear(&a1);
  return res;
}

/* c = |a| * |b| using Karatsuba Multiplication using
 * three half size multiplications
 *
 * Let B represent the radix [e.g. 2**DIGIT_BIT] and
 * let n represent half of the number of digits in
 * the min(a,b)
 *
 * a = a1 * B**n + a0
 * b = b1 * B**n + b0
 *
 * Then, a * b =>
   a1b1 * B**2n + ((a1 + a0)(b1 + b0) - (a0b0 + a1b1)) * B + a0b0
 *
 * Note that a1b1 and a0b0 are used twice and only need to be
 * computed once.  So in total three half size (half # of
 * digit) multiplications are performed, a0b0, a1b1 and
 * (a1+b1)(a0+b0)
 *
 * Note that a multiplication of half the digits requires
 * 1/4th the number of single precision multiplications so in
 * total after one call 25% of the single precision multiplications
 * are saved.  Note also that the call to mp_mul can end up back
 * in this function if the a0, a1, b0, or b1 are above the threshold.
 * This is known as divide-and-conquer and leads to the famous
 * O(N**lg(3)) or O(N**1.584) work which is asymptopically lower than
 * the standard O(N**2) that the baseline/comba methods use.
 * Generally though the overhead of this method doesn't pay off
 * until a certain size (N ~ 80) is reached.
 */
int mp_karatsuba_mul(mp_int *a, mp_int *b, mp_int *c) {
  mp_int x0, x1, y0, y1, t1, x0y0, x1y1;
  int B, err;

  /* default the return code to an error */
  err = MP_MEM;

  /* min # of digits */
  B = MIN(a->used, b->used);

  /* now divide in two */
  B = B >> 1;

  /* init copy all the temps */
  if (mp_init_size(&x0, B) != MP_OKAY) goto ERR;
  if (mp_init_size(&x1, a->used - B) != MP_OKAY) goto X0;
  if (mp_init_size(&y0, B) != MP_OKAY) goto X1;
  if (mp_init_size(&y1, b->used - B) != MP_OKAY) goto Y0;

  /* init temps */
  if (mp_init_size(&t1, B * 2) != MP_OKAY) goto Y1;
  if (mp_init_size(&x0y0, B * 2) != MP_OKAY) goto T1;
  if (mp_init_size(&x1y1, B * 2) != MP_OKAY) goto X0Y0;

  /* now shift the digits */
  x0.used = y0.used = B;
  x1.used = a->used - B;
  y1.used = b->used - B;

  {
    int x;
    mp_digit *tmpa, *tmpb, *tmpx, *tmpy;

    /* we copy the digits directly instead of using higher level functions
     * since we also need to shift the digits
     */
    tmpa = a->dp;
    tmpb = b->dp;

    tmpx = x0.dp;
    tmpy = y0.dp;
    for (x = 0; x < B; x++) {
      *tmpx++ = *tmpa++;
      *tmpy++ = *tmpb++;
    }

    tmpx = x1.dp;
    for (x = B; x < a->used; x++) {
      *tmpx++ = *tmpa++;
    }

    tmpy = y1.dp;
    for (x = B; x < b->used; x++) {
      *tmpy++ = *tmpb++;
    }
  }

  /* only need to clamp the lower words since by definition the
   * upper words x1/y1 must have a known number of digits
   */
  mp_clamp(&x0);
  mp_clamp(&y0);

  /* now calc the products x0y0 and x1y1 */
  /* after this x0 is no longer required, free temp [x0==t2]! */
  if (mp_mul(&x0, &y0, &x0y0) != MP_OKAY) goto X1Y1; /* x0y0 = x0*y0 */
  if (mp_mul(&x1, &y1, &x1y1) != MP_OKAY) goto X1Y1; /* x1y1 = x1*y1 */

  /* now calc x1+x0 and y1+y0 */
  if (s_mp_add(&x1, &x0, &t1) != MP_OKAY) goto X1Y1; /* t1 = x1 - x0 */
  if (s_mp_add(&y1, &y0, &x0) != MP_OKAY) goto X1Y1; /* t2 = y1 - y0 */
  if (mp_mul(&t1, &x0, &t1) != MP_OKAY)
    goto X1Y1; /* t1 = (x1 + x0) * (y1 + y0) */

  /* add x0y0 */
  if (mp_add(&x0y0, &x1y1, &x0) != MP_OKAY) goto X1Y1; /* t2 = x0y0 + x1y1 */
  if (s_mp_sub(&t1, &x0, &t1) != MP_OKAY)
    goto X1Y1; /* t1 = (x1+x0)*(y1+y0) - (x1y1 + x0y0) */

  /* shift by B */
  if (mp_lshd(&t1, B) != MP_OKAY)
    goto X1Y1; /* t1 = (x0y0 + x1y1 - (x1-x0)*(y1-y0))<<B */
  if (mp_lshd(&x1y1, B * 2) != MP_OKAY) goto X1Y1; /* x1y1 = x1y1 << 2*B */

  if (mp_add(&x0y0, &t1, &t1) != MP_OKAY) goto X1Y1; /* t1 = x0y0 + t1 */
  if (mp_add(&t1, &x1y1, c) != MP_OKAY) goto X1Y1;   /* t1 = x0y0 + t1 + x1y1 */

  /* Algorithm succeeded set the return code to MP_OKAY */
  err = MP_OKAY;

X1Y1:
  mp_clear(&x1y1);
X0Y0:
  mp_clear(&x0y0);
T1:
  mp_clear(&t1);
Y1:
  mp_clear(&y1);
Y0:
  mp_clear(&y0);
X1:
  mp_clear(&x1);
X0:
  mp_clear(&x0);
ERR:
  return err;
}

/* Karatsuba squaring, computes b = a*a using three
 * half size squarings
 *
 * See comments of karatsuba_mul for details.  It
 * is essentially the same algorithm but merely
 * tuned to perform recursive squarings.
 */
int mp_karatsuba_sqr(mp_int *a, mp_int *b) {
  mp_int x0, x1, t1, t2, x0x0, x1x1;
  int B, err;

  err = MP_MEM;

  /* min # of digits */
  B = a->used;

  /* now divide in two */
  B = B >> 1;

  /* init copy all the temps */
  if (mp_init_size(&x0, B) != MP_OKAY) goto ERR;
  if (mp_init_size(&x1, a->used - B) != MP_OKAY) goto X0;

  /* init temps */
  if (mp_init_size(&t1, a->used * 2) != MP_OKAY) goto X1;
  if (mp_init_size(&t2, a->used * 2) != MP_OKAY) goto T1;
  if (mp_init_size(&x0x0, B * 2) != MP_OKAY) goto T2;
  if (mp_init_size(&x1x1, (a->used - B) * 2) != MP_OKAY) goto X0X0;

  {
    int x;
    mp_digit *dst, *src;

    src = a->dp;

    /* now shift the digits */
    dst = x0.dp;
    for (x = 0; x < B; x++) {
      *dst++ = *src++;
    }

    dst = x1.dp;
    for (x = B; x < a->used; x++) {
      *dst++ = *src++;
    }
  }

  x0.used = B;
  x1.used = a->used - B;

  mp_clamp(&x0);

  /* now calc the products x0*x0 and x1*x1 */
  if (mp_sqr(&x0, &x0x0) != MP_OKAY) goto X1X1; /* x0x0 = x0*x0 */
  if (mp_sqr(&x1, &x1x1) != MP_OKAY) goto X1X1; /* x1x1 = x1*x1 */

  /* now calc (x1+x0)**2 */
  if (s_mp_add(&x1, &x0, &t1) != MP_OKAY) goto X1X1; /* t1 = x1 - x0 */
  if (mp_sqr(&t1, &t1) != MP_OKAY) goto X1X1; /* t1 = (x1 - x0) * (x1 - x0) */

  /* add x0y0 */
  if (s_mp_add(&x0x0, &x1x1, &t2) != MP_OKAY) goto X1X1; /* t2 = x0x0 + x1x1 */
  if (s_mp_sub(&t1, &t2, &t1) != MP_OKAY)
    goto X1X1; /* t1 = (x1+x0)**2 - (x0x0 + x1x1) */

  /* shift by B */
  if (mp_lshd(&t1, B) != MP_OKAY)
    goto X1X1; /* t1 = (x0x0 + x1x1 - (x1-x0)*(x1-x0))<<B */
  if (mp_lshd(&x1x1, B * 2) != MP_OKAY) goto X1X1; /* x1x1 = x1x1 << 2*B */

  if (mp_add(&x0x0, &t1, &t1) != MP_OKAY) goto X1X1; /* t1 = x0x0 + t1 */
  if (mp_add(&t1, &x1x1, b) != MP_OKAY) goto X1X1;   /* t1 = x0x0 + t1 + x1x1 */

  err = MP_OKAY;

X1X1:
  mp_clear(&x1x1);
X0X0:
  mp_clear(&x0x0);
T2:
  mp_clear(&t2);
T1:
  mp_clear(&t1);
X1:
  mp_clear(&x1);
X0:
  mp_clear(&x0);
ERR:
  return err;
}

/* computes least common multiple as |a*b|/(a, b) */
int mp_lcm(mp_int *a, mp_int *b, mp_int *c) {
  int res;
  mp_int t1, t2;

  if ((res = mp_init_multi(&t1, &t2, NULL)) != MP_OKAY) {
    return res;
  }

  /* t1 = get the GCD of the two inputs */
  if ((res = mp_gcd(a, b, &t1)) != MP_OKAY) {
    goto LBL_T;
  }

  /* divide the smallest by the GCD */
  if (mp_cmp_mag(a, b) == MP_LT) {
    /* store quotient in t2 such that t2 * b is the LCM */
    if ((res = mp_div(a, &t1, &t2, NULL)) != MP_OKAY) {
      goto LBL_T;
    }
    res = mp_mul(b, &t2, c);
  } else {
    /* store quotient in t2 such that t2 * a is the LCM */
    if ((res = mp_div(b, &t1, &t2, NULL)) != MP_OKAY) {
      goto LBL_T;
    }
    res = mp_mul(a, &t2, c);
  }

  /* fix the sign to positive */
  c->sign = MP_ZPOS;

LBL_T:
  mp_clear_multi(&t1, &t2, NULL);
  return res;
}

/* shift left a certain amount of digits */
int mp_lshd(mp_int *a, int b) {
  int x, res;

  /* if its less than zero return */
  if (b <= 0) {
    return MP_OKAY;
  }

  /* grow to fit the new digits */
  if (a->alloc < (a->used + b)) {
    if ((res = mp_grow(a, a->used + b)) != MP_OKAY) {
      return res;
    }
  }

  {
    mp_digit *top, *bottom;

    /* increment the used by the shift amount then copy upwards */
    a->used += b;

    /* top */
    top = a->dp + a->used - 1;

    /* base */
    bottom = (a->dp + a->used - 1) - b;

    /* much like mp_rshd this is implemented using a sliding window
     * except the window goes the otherway around.  Copying from
     * the bottom to the top.  see bn_mp_rshd.c for more info.
     */
    for (x = a->used - 1; x >= b; x--) {
      *top-- = *bottom--;
    }

    /* zero the lower digits */
    top = a->dp;
    for (x = 0; x < b; x++) {
      *top++ = 0;
    }
  }
  return MP_OKAY;
}

/* c = a mod b, 0 <= c < b if b > 0, b < c <= 0 if b < 0 */
int mp_mod(mp_int *a, mp_int *b, mp_int *c) {
  mp_int t;
  int res;

  if ((res = mp_init(&t)) != MP_OKAY) {
    return res;
  }

  if ((res = mp_div(a, b, NULL, &t)) != MP_OKAY) {
    mp_clear(&t);
    return res;
  }

  if ((mp_iszero(&t) != MP_NO) || (t.sign == b->sign)) {
    res = MP_OKAY;
    mp_exch(&t, c);
  } else {
    res = mp_add(b, &t, c);
  }

  mp_clear(&t);
  return res;
}

/* calc a value mod 2**b */
int mp_mod_2d(mp_int *a, int b, mp_int *c) {
  int x, res;

  /* if b is <= 0 then zero the int */
  if (b <= 0) {
    mp_zero(c);
    return MP_OKAY;
  }

  /* if the modulus is larger than the value than return */
  if (b >= (int)(a->used * DIGIT_BIT)) {
    res = mp_copy(a, c);
    return res;
  }

  /* copy */
  if ((res = mp_copy(a, c)) != MP_OKAY) {
    return res;
  }

  /* zero digits above the last digit of the modulus */
  for (x = (b / DIGIT_BIT) + (((b % DIGIT_BIT) == 0) ? 0 : 1); x < c->used;
       x++) {
    c->dp[x] = 0;
  }
  /* clear the digit that is not completely outside/inside the modulus */
  c->dp[b / DIGIT_BIT] &= (mp_digit)(
      (((mp_digit)1) << (((mp_digit)b) % DIGIT_BIT)) - ((mp_digit)1));
  mp_clamp(c);
  return MP_OKAY;
}

int mp_mod_d(mp_int *a, mp_digit b, mp_digit *c) {
  return mp_div_d(a, b, NULL, c);
}

/*
 * shifts with subtractions when the result is greater than b.
 *
 * The method is slightly modified to shift B unconditionally upto just under
 * the leading bit of b.  This saves alot of multiple precision shifting.
 */
int mp_montgomery_calc_normalization(mp_int *a, mp_int *b) {
  int x, bits, res;

  /* how many bits of last digit does b use */
  bits = mp_count_bits(b) % DIGIT_BIT;

  if (b->used > 1) {
    if ((res = mp_2expt(a, ((b->used - 1) * DIGIT_BIT) + bits - 1)) !=
        MP_OKAY) {
      return res;
    }
  } else {
    mp_set(a, 1);
    bits = 1;
  }

  /* now compute C = A * B mod b */
  for (x = bits - 1; x < (int)DIGIT_BIT; x++) {
    if ((res = mp_mul_2(a, a)) != MP_OKAY) {
      return res;
    }
    if (mp_cmp_mag(a, b) != MP_LT) {
      if ((res = s_mp_sub(a, b, a)) != MP_OKAY) {
        return res;
      }
    }
  }

  return MP_OKAY;
}

/* computes xR**-1 == x (mod N) via Montgomery Reduction */
int mp_montgomery_reduce(mp_int *x, mp_int *n, mp_digit rho) {
  int ix, res, digs;
  mp_digit mu;

  /* can the fast reduction [comba] method be used?
   *
   * Note that unlike in mul you're safely allowed *less*
   * than the available columns [255 per default] since carries
   * are fixed up in the inner loop.
   */
  digs = (n->used * 2) + 1;
  if ((digs < MP_WARRAY) &&
      (n->used < (1 << ((CHAR_BIT * sizeof(mp_word)) - (2 * DIGIT_BIT))))) {
    return fast_mp_montgomery_reduce(x, n, rho);
  }

  /* grow the input as required */
  if (x->alloc < digs) {
    if ((res = mp_grow(x, digs)) != MP_OKAY) {
      return res;
    }
  }
  x->used = digs;

  for (ix = 0; ix < n->used; ix++) {
    /* mu = ai * rho mod b
     *
     * The value of rho must be precalculated via
     * montgomery_setup() such that
     * it equals -1/n0 mod b this allows the
     * following inner loop to reduce the
     * input one digit at a time
     */
    mu = (mp_digit)(((mp_word)x->dp[ix] * (mp_word)rho) & MP_MASK);

    /* a = a + mu * m * b**i */
    {
      int iy;
      mp_digit *tmpn, *tmpx, u;
      mp_word r;

      /* alias for digits of the modulus */
      tmpn = n->dp;

      /* alias for the digits of x [the input] */
      tmpx = x->dp + ix;

      /* set the carry to zero */
      u = 0;

      /* Multiply and add in place */
      for (iy = 0; iy < n->used; iy++) {
        /* compute product and sum */
        r = ((mp_word)mu * (mp_word)*tmpn++) + (mp_word)u + (mp_word)*tmpx;

        /* get carry */
        u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));

        /* fix digit */
        *tmpx++ = (mp_digit)(r & ((mp_word)MP_MASK));
      }
      /* At this point the ix'th digit of x should be zero */

      /* propagate carries upwards as required*/
      while (u != 0) {
        *tmpx += u;
        u = *tmpx >> DIGIT_BIT;
        *tmpx++ &= MP_MASK;
      }
    }
  }

  /* at this point the n.used'th least
   * significant digits of x are all zero
   * which means we can shift x to the
   * right by n.used digits and the
   * residue is unchanged.
   */

  /* x = x/b**n.used */
  mp_clamp(x);
  mp_rshd(x, n->used);

  /* if x >= n then x = x - n */
  if (mp_cmp_mag(x, n) != MP_LT) {
    return s_mp_sub(x, n, x);
  }

  return MP_OKAY;
}

/* setups the montgomery reduction stuff */
int mp_montgomery_setup(mp_int *n, mp_digit *rho) {
  mp_digit x, b;

  /* fast inversion mod 2**k
   *
   * Based on the fact that
   *
   * XA = 1 (mod 2**n)  =>  (X(2-XA)) A = 1 (mod 2**2n)
   *                    =>  2*X*A - X*X*A*A = 1
   *                    =>  2*(1) - (1)     = 1
   */
  b = n->dp[0];

  if ((b & 1) == 0) {
    return MP_VAL;
  }

  x = (((b + 2) & 4) << 1) + b; /* here x*a==1 mod 2**4 */
  x *= 2 - (b * x);             /* here x*a==1 mod 2**8 */
  x *= 2 - (b * x);             /* here x*a==1 mod 2**16 */
  x *= 2 - (b * x);             /* here x*a==1 mod 2**32 */
  x *= 2 - (b * x);             /* here x*a==1 mod 2**64 */

  /* rho = -1/m mod b */
  *rho = (mp_digit)(((mp_word)1 << ((mp_word)DIGIT_BIT)) - x) & MP_MASK;

  return MP_OKAY;
}

/* high level multiplication (handles sign) */
int mp_mul(mp_int *a, mp_int *b, mp_int *c) {
  int res, neg;

  neg = (a->sign == b->sign) ? MP_ZPOS : MP_NEG;

  /* use Toom-Cook? */
  if (MIN(a->used, b->used) >= TOOM_MUL_CUTOFF) {
    res = mp_toom_mul(a, b, c);
  } else
      /* use Karatsuba? */
      if (MIN(a->used, b->used) >= KARATSUBA_MUL_CUTOFF) {
    res = mp_karatsuba_mul(a, b, c);
  } else {
    /* can we use the fast multiplier?
     *
     * The fast multiplier can be used if the output will
     * have less than MP_WARRAY digits and the number of
     * digits won't affect carry propagation
     */
    int digs = a->used + b->used + 1;

    if ((digs < MP_WARRAY) &&
        (MIN(a->used, b->used) <=
         (1 << ((CHAR_BIT * sizeof(mp_word)) - (2 * DIGIT_BIT))))) {
      res = fast_s_mp_mul_digs(a, b, c, digs);
    } else {
      res = s_mp_mul(a, b, c); /* uses s_mp_mul_digs */
    }
  }
  c->sign = (c->used > 0) ? neg : MP_ZPOS;
  return res;
}

/* b = a*2 */
int mp_mul_2(mp_int *a, mp_int *b) {
  int x, res, oldused;

  /* grow to accomodate result */
  if (b->alloc < (a->used + 1)) {
    if ((res = mp_grow(b, a->used + 1)) != MP_OKAY) {
      return res;
    }
  }

  oldused = b->used;
  b->used = a->used;

  {
    mp_digit r, rr, *tmpa, *tmpb;

    /* alias for source */
    tmpa = a->dp;

    /* alias for dest */
    tmpb = b->dp;

    /* carry */
    r = 0;
    for (x = 0; x < a->used; x++) {
      /* get what will be the *next* carry bit from the
       * MSB of the current digit
       */
      rr = *tmpa >> ((mp_digit)(DIGIT_BIT - 1));

      /* now shift up this digit, add in the carry [from the previous] */
      *tmpb++ = ((*tmpa++ << ((mp_digit)1)) | r) & MP_MASK;

      /* copy the carry that would be from the source
       * digit into the next iteration
       */
      r = rr;
    }

    /* new leading digit? */
    if (r != 0) {
      /* add a MSB which is always 1 at this point */
      *tmpb = 1;
      ++(b->used);
    }

    /* now zero any excess digits on the destination
     * that we didn't write to
     */
    tmpb = b->dp + b->used;
    for (x = b->used; x < oldused; x++) {
      *tmpb++ = 0;
    }
  }
  b->sign = a->sign;
  return MP_OKAY;
}

/* shift left by a certain bit count */
int mp_mul_2d(mp_int *a, int b, mp_int *c) {
  mp_digit d;
  int res;

  /* copy */
  if (a != c) {
    if ((res = mp_copy(a, c)) != MP_OKAY) {
      return res;
    }
  }

  if (c->alloc < (int)(c->used + (b / DIGIT_BIT) + 1)) {
    if ((res = mp_grow(c, c->used + (b / DIGIT_BIT) + 1)) != MP_OKAY) {
      return res;
    }
  }

  /* shift by as many digits in the bit count */
  if (b >= (int)DIGIT_BIT) {
    if ((res = mp_lshd(c, b / DIGIT_BIT)) != MP_OKAY) {
      return res;
    }
  }

  /* shift any bit count < DIGIT_BIT */
  d = (mp_digit)(b % DIGIT_BIT);
  if (d != 0) {
    mp_digit *tmpc, shift, mask, r, rr;
    int x;

    /* bitmask for carries */
    mask = (((mp_digit)1) << d) - 1;

    /* shift for msbs */
    shift = DIGIT_BIT - d;

    /* alias */
    tmpc = c->dp;

    /* carry */
    r = 0;
    for (x = 0; x < c->used; x++) {
      /* get the higher bits of the current word */
      rr = (*tmpc >> shift) & mask;

      /* shift the current word and OR in the carry */
      *tmpc = ((*tmpc << d) | r) & MP_MASK;
      ++tmpc;

      /* set the carry to the carry bits of the current word */
      r = rr;
    }

    /* set final carry */
    if (r != 0) {
      c->dp[(c->used)++] = r;
    }
  }
  mp_clamp(c);
  return MP_OKAY;
}

/* multiply by a digit */
int mp_mul_d(mp_int *a, mp_digit b, mp_int *c) {
  mp_digit u, *tmpa, *tmpc;
  mp_word r;
  int ix, res, olduse;

  /* make sure c is big enough to hold a*b */
  if (c->alloc < (a->used + 1)) {
    if ((res = mp_grow(c, a->used + 1)) != MP_OKAY) {
      return res;
    }
  }

  /* get the original destinations used count */
  olduse = c->used;

  /* set the sign */
  c->sign = a->sign;

  /* alias for a->dp [source] */
  tmpa = a->dp;

  /* alias for c->dp [dest] */
  tmpc = c->dp;

  /* zero carry */
  u = 0;

  /* compute columns */
  for (ix = 0; ix < a->used; ix++) {
    /* compute product and carry sum for this term */
    r = (mp_word)u + ((mp_word)*tmpa++ * (mp_word)b);

    /* mask off higher bits to get a single digit */
    *tmpc++ = (mp_digit)(r & ((mp_word)MP_MASK));

    /* send carry into next iteration */
    u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));
  }

  /* store final carry [if any] and increment ix offset  */
  *tmpc++ = u;
  ++ix;

  /* now zero digits above the top */
  while (ix++ < olduse) {
    *tmpc++ = 0;
  }

  /* set used count */
  c->used = a->used + 1;
  mp_clamp(c);

  return MP_OKAY;
}

/* d = a * b (mod c) */
int mp_mulmod(mp_int *a, mp_int *b, mp_int *c, mp_int *d) {
  int res;
  mp_int t;

  if ((res = mp_init(&t)) != MP_OKAY) {
    return res;
  }

  if ((res = mp_mul(a, b, &t)) != MP_OKAY) {
    mp_clear(&t);
    return res;
  }
  res = mp_mod(&t, c, d);
  mp_clear(&t);
  return res;
}

/* b = -a */
int mp_neg(mp_int *a, mp_int *b) {
  int res;

  if (a != b) {
    if ((res = mp_copy(a, b)) != MP_OKAY) {
      return res;
    }
  }

  if (mp_iszero(b) != MP_YES) {
    b->sign = (a->sign == MP_ZPOS) ? MP_NEG : MP_ZPOS;
  } else {
    b->sign = MP_ZPOS;
  }

  return MP_OKAY;
}

/* determines if an integers is divisible by one
 * of the first PRIME_SIZE primes or not
 *
 * sets result to 0 if not, 1 if yes
 */
int mp_prime_is_divisible(mp_int *a, int *result) {
  int err, ix;
  mp_digit res;

  /* default to not */
  *result = MP_NO;

  for (ix = 0; ix < PRIME_SIZE; ix++) {
    /* what is a mod LBL_prime_tab[ix] */
    if ((err = mp_mod_d(a, ltm_prime_tab[ix], &res)) != MP_OKAY) {
      return err;
    }

    /* is the residue zero? */
    if (res == 0) {
      *result = MP_YES;
      return MP_OKAY;
    }
  }

  return MP_OKAY;
}

/* performs a variable number of rounds of Miller-Rabin
 *
 * Probability of error after t rounds is no more than

 *
 * Sets result to 1 if probably prime, 0 otherwise
 */
int mp_prime_is_prime(mp_int *a, int t, int *result) {
  mp_int b;
  int ix, err, res;

  /* default to no */
  *result = MP_NO;

  /* valid value of t? */
  if ((t <= 0) || (t > PRIME_SIZE)) {
    return MP_VAL;
  }

  /* is the input equal to one of the primes in the table? */
  for (ix = 0; ix < PRIME_SIZE; ix++) {
    if (mp_cmp_d(a, ltm_prime_tab[ix]) == MP_EQ) {
      *result = 1;
      return MP_OKAY;
    }
  }

  /* first perform trial division */
  if ((err = mp_prime_is_divisible(a, &res)) != MP_OKAY) {
    return err;
  }

  /* return if it was trivially divisible */
  if (res == MP_YES) {
    return MP_OKAY;
  }

  /* now perform the miller-rabin rounds */
  if ((err = mp_init(&b)) != MP_OKAY) {
    return err;
  }

  for (ix = 0; ix < t; ix++) {
    /* set the prime */
    mp_set(&b, ltm_prime_tab[ix]);

    if ((err = mp_prime_miller_rabin(a, &b, &res)) != MP_OKAY) {
      goto LBL_B;
    }

    if (res == MP_NO) {
      goto LBL_B;
    }
  }

  /* passed the test */
  *result = MP_YES;
LBL_B:
  mp_clear(&b);
  return err;
}

/* Miller-Rabin test of "a" to the base of "b" as described in
 * HAC pp. 139 Algorithm 4.24
 *
 * Sets result to 0 if definitely composite or 1 if probably prime.
 * Randomly the chance of error is no more than 1/4 and often
 * very much lower.
 */
int mp_prime_miller_rabin(mp_int *a, mp_int *b, int *result) {
  mp_int n1, y, r;
  int s, j, err;

  /* default */
  *result = MP_NO;

  /* ensure b > 1 */
  if (mp_cmp_d(b, 1) != MP_GT) {
    return MP_VAL;
  }

  /* get n1 = a - 1 */
  if ((err = mp_init_copy(&n1, a)) != MP_OKAY) {
    return err;
  }
  if ((err = mp_sub_d(&n1, 1, &n1)) != MP_OKAY) {
    goto LBL_N1;
  }

  /* set 2**s * r = n1 */
  if ((err = mp_init_copy(&r, &n1)) != MP_OKAY) {
    goto LBL_N1;
  }

  /* count the number of least significant bits
   * which are zero
   */
  s = mp_cnt_lsb(&r);

  /* now divide n - 1 by 2**s */
  if ((err = mp_div_2d(&r, s, &r, NULL)) != MP_OKAY) {
    goto LBL_R;
  }

  /* compute y = b**r mod a */
  if ((err = mp_init(&y)) != MP_OKAY) {
    goto LBL_R;
  }
  if ((err = mp_exptmod(b, &r, a, &y)) != MP_OKAY) {
    goto LBL_Y;
  }

  /* if y != 1 and y != n1 do */
  if ((mp_cmp_d(&y, 1) != MP_EQ) && (mp_cmp(&y, &n1) != MP_EQ)) {
    j = 1;
    /* while j <= s-1 and y != n1 */
    while ((j <= (s - 1)) && (mp_cmp(&y, &n1) != MP_EQ)) {
      if ((err = mp_sqrmod(&y, a, &y)) != MP_OKAY) {
        goto LBL_Y;
      }

      /* if y == 1 then composite */
      if (mp_cmp_d(&y, 1) == MP_EQ) {
        goto LBL_Y;
      }

      ++j;
    }

    /* if y != n1 then composite */
    if (mp_cmp(&y, &n1) != MP_EQ) {
      goto LBL_Y;
    }
  }

  /* probably prime now */
  *result = MP_YES;
LBL_Y:
  mp_clear(&y);
LBL_R:
  mp_clear(&r);
LBL_N1:
  mp_clear(&n1);
  return err;
}

/* chars used in radix conversions */
const char *mp_s_rmap =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

/* read a string [ASCII] in a given radix */
int mp_read_radix(mp_int *a, const char *str, int radix) {
  int y, res, neg;
  char ch;

  /* zero the digit bignum */
  mp_zero(a);

  /* make sure the radix is ok */
  if ((radix < 2) || (radix > 64)) {
    return MP_VAL;
  }

  /* if the leading digit is a
   * minus set the sign to negative.
   */
  if (*str == '-') {
    ++str;
    neg = MP_NEG;
  } else {
    neg = MP_ZPOS;
  }

  /* set the integer to the default of zero */
  mp_zero(a);

  /* process each digit of the string */
  while (*str != '\0') {
    /* if the radix <= 36 the conversion is case insensitive
     * this allows numbers like 1AB and 1ab to represent the same  value
     * [e.g. in hex]
     */
    ch = (radix <= 36) ? (char)toupper((int)*str) : *str;
    for (y = 0; y < 64; y++) {
      if (ch == mp_s_rmap[y]) {
        break;
      }
    }

    /* if the char was found in the map
     * and is less than the given radix add it
     * to the number, otherwise exit the loop.
     */
    if (y < radix) {
      if ((res = mp_mul_d(a, (mp_digit)radix, a)) != MP_OKAY) {
        return res;
      }
      if ((res = mp_add_d(a, (mp_digit)y, a)) != MP_OKAY) {
        return res;
      }
    } else {
      break;
    }
    ++str;
  }

  /* set the sign only if a != 0 */
  if (mp_iszero(a) != MP_YES) {
    a->sign = neg;
  }
  return MP_OKAY;
}

/* reads a unsigned char array, assumes the msb is stored first [big endian] */
int mp_read_unsigned_bin(mp_int *a, const unsigned char *b, int c) {
  int res;

  /* make sure there are at least two digits */
  if (a->alloc < 2) {
    if ((res = mp_grow(a, 2)) != MP_OKAY) {
      return res;
    }
  }

  /* zero the int */
  mp_zero(a);

  /* read the bytes in */
  while (c-- > 0) {
    if ((res = mp_mul_2d(a, 8, a)) != MP_OKAY) {
      return res;
    }

    a->dp[0] |= *b++;
    a->used += 1;
  }
  mp_clamp(a);
  return MP_OKAY;
}

/* reduces x mod m, assumes 0 < x < m**2, mu is
 * precomputed via mp_reduce_setup.
 * From HAC pp.604 Algorithm 14.42
 */
int mp_reduce(mp_int *x, mp_int *m, mp_int *mu) {
  mp_int q;
  int res, um = m->used;

  /* q = x */
  if ((res = mp_init_copy(&q, x)) != MP_OKAY) {
    return res;
  }

  /* q1 = x / b**(k-1)  */
  mp_rshd(&q, um - 1);

  /* according to HAC this optimization is ok */
  if (((mp_digit)um) > (((mp_digit)1) << (DIGIT_BIT - 1))) {
    if ((res = mp_mul(&q, mu, &q)) != MP_OKAY) {
      goto CLEANUP;
    }
  } else {
    if ((res = s_mp_mul_high_digs(&q, mu, &q, um)) != MP_OKAY) {
      goto CLEANUP;
    }
  }

  /* q3 = q2 / b**(k+1) */
  mp_rshd(&q, um + 1);

  /* x = x mod b**(k+1), quick (no division) */
  if ((res = mp_mod_2d(x, DIGIT_BIT * (um + 1), x)) != MP_OKAY) {
    goto CLEANUP;
  }

  /* q = q * m mod b**(k+1), quick (no division) */
  if ((res = s_mp_mul_digs(&q, m, &q, um + 1)) != MP_OKAY) {
    goto CLEANUP;
  }

  /* x = x - q */
  if ((res = mp_sub(x, &q, x)) != MP_OKAY) {
    goto CLEANUP;
  }

  /* If x < 0, add b**(k+1) to it */
  if (mp_cmp_d(x, 0) == MP_LT) {
    mp_set(&q, 1);
    if ((res = mp_lshd(&q, um + 1)) != MP_OKAY) goto CLEANUP;
    if ((res = mp_add(x, &q, x)) != MP_OKAY) goto CLEANUP;
  }

  /* Back off if it's too big */
  while (mp_cmp(x, m) != MP_LT) {
    if ((res = s_mp_sub(x, m, x)) != MP_OKAY) {
      goto CLEANUP;
    }
  }

CLEANUP:
  mp_clear(&q);

  return res;
}

/* reduces a modulo n where n is of the form 2**p - d */
int mp_reduce_2k(mp_int *a, mp_int *n, mp_digit d) {
  mp_int q;
  int p, res;

  if ((res = mp_init(&q)) != MP_OKAY) {
    return res;
  }

  p = mp_count_bits(n);
top:
  /* q = a/2**p, a = a mod 2**p */
  if ((res = mp_div_2d(a, p, &q, a)) != MP_OKAY) {
    goto ERR;
  }

  if (d != 1) {
    /* q = q * d */
    if ((res = mp_mul_d(&q, d, &q)) != MP_OKAY) {
      goto ERR;
    }
  }

  /* a = a + q */
  if ((res = s_mp_add(a, &q, a)) != MP_OKAY) {
    goto ERR;
  }

  if (mp_cmp_mag(a, n) != MP_LT) {
    if ((res = s_mp_sub(a, n, a)) != MP_OKAY) {
      goto ERR;
    }
    goto top;
  }

ERR:
  mp_clear(&q);
  return res;
}

/* reduces a modulo n where n is of the form 2**p - d
   This differs from reduce_2k since "d" can be larger
   than a single digit.
 */
int mp_reduce_2k_l(mp_int *a, mp_int *n, mp_int *d) {
  mp_int q;
  int p, res;

  if ((res = mp_init(&q)) != MP_OKAY) {
    return res;
  }

  p = mp_count_bits(n);
top:
  /* q = a/2**p, a = a mod 2**p */
  if ((res = mp_div_2d(a, p, &q, a)) != MP_OKAY) {
    goto ERR;
  }

  /* q = q * d */
  if ((res = mp_mul(&q, d, &q)) != MP_OKAY) {
    goto ERR;
  }

  /* a = a + q */
  if ((res = s_mp_add(a, &q, a)) != MP_OKAY) {
    goto ERR;
  }

  if (mp_cmp_mag(a, n) != MP_LT) {
    if ((res = s_mp_sub(a, n, a)) != MP_OKAY) {
      goto ERR;
    }
    goto top;
  }

ERR:
  mp_clear(&q);
  return res;
}

/* determines the setup value */
int mp_reduce_2k_setup(mp_int *a, mp_digit *d) {
  int res, p;
  mp_int tmp;

  if ((res = mp_init(&tmp)) != MP_OKAY) {
    return res;
  }

  p = mp_count_bits(a);
  if ((res = mp_2expt(&tmp, p)) != MP_OKAY) {
    mp_clear(&tmp);
    return res;
  }

  if ((res = s_mp_sub(&tmp, a, &tmp)) != MP_OKAY) {
    mp_clear(&tmp);
    return res;
  }

  *d = tmp.dp[0];
  mp_clear(&tmp);
  return MP_OKAY;
}

/* determines the setup value */
int mp_reduce_2k_setup_l(mp_int *a, mp_int *d) {
  int res;
  mp_int tmp;

  if ((res = mp_init(&tmp)) != MP_OKAY) {
    return res;
  }

  if ((res = mp_2expt(&tmp, mp_count_bits(a))) != MP_OKAY) {
    goto ERR;
  }

  if ((res = s_mp_sub(&tmp, a, d)) != MP_OKAY) {
    goto ERR;
  }

ERR:
  mp_clear(&tmp);
  return res;
}

/* determines if mp_reduce_2k can be used */
int mp_reduce_is_2k(mp_int *a) {
  int ix, iy, iw;
  mp_digit iz;

  if (a->used == 0) {
    return MP_NO;
  } else if (a->used == 1) {
    return MP_YES;
  } else if (a->used > 1) {
    iy = mp_count_bits(a);
    iz = 1;
    iw = 1;

    /* Test every bit from the second digit up, must be 1 */
    for (ix = DIGIT_BIT; ix < iy; ix++) {
      if ((a->dp[iw] & iz) == 0) {
        return MP_NO;
      }
      iz <<= 1;
      if (iz > (mp_digit)MP_MASK) {
        ++iw;
        iz = 1;
      }
    }
  }
  return MP_YES;
}

/* determines if reduce_2k_l can be used */
int mp_reduce_is_2k_l(mp_int *a) {
  int ix, iy;

  if (a->used == 0) {
    return MP_NO;
  } else if (a->used == 1) {
    return MP_YES;
  } else if (a->used > 1) {
    /* if more than half of the digits are -1 we're sold */
    for (iy = ix = 0; ix < a->used; ix++) {
      if (a->dp[ix] == MP_MASK) {
        ++iy;
      }
    }
    return (iy >= (a->used / 2)) ? MP_YES : MP_NO;
  }
  return MP_NO;
}

/* pre-calculate the value required for Barrett reduction
 * For a given modulus "b" it calulates the value required in "a"
 */
int mp_reduce_setup(mp_int *a, mp_int *b) {
  int res;

  if ((res = mp_2expt(a, b->used * 2 * DIGIT_BIT)) != MP_OKAY) {
    return res;
  }
  return mp_div(a, b, a, NULL);
}

/* shift right a certain amount of digits */
void mp_rshd(mp_int *a, int b) {
  int x;

  /* if b <= 0 then ignore it */
  if (b <= 0) {
    return;
  }

  /* if b > used then simply zero it and return */
  if (a->used <= b) {
    mp_zero(a);
    return;
  }

  {
    mp_digit *bottom, *top;

    /* shift the digits down */

    /* bottom */
    bottom = a->dp;

    /* top [offset into digits] */
    top = a->dp + b;

    /* this is implemented as a sliding window where
 * the window is b-digits long and digits from
 * the top of the window are copied to the bottom
 *
 * e.g.

   b-2 | b-1 | b0 | b1 | b2 | ... | bb |   ---->
             /\                   |      ---->
 **\-------------------/      ---->
 */
    for (x = 0; x < (a->used - b); x++) {
      *bottom++ = *top++;
    }

    /* zero the top digits */
    for (; x < a->used; x++) {
      *bottom++ = 0;
    }
  }

  /* remove excess digits */
  a->used -= b;
}

/* set to a digit */
void mp_set(mp_int *a, mp_digit b) {
  mp_zero(a);
  a->dp[0] = b & MP_MASK;
  a->used = (a->dp[0] != 0) ? 1 : 0;
}

/* set a 32-bit const */
int mp_set_int(mp_int *a, unsigned long b) {
  int x, res;

  mp_zero(a);

  /* set four bits at a time */
  for (x = 0; x < 8; x++) {
    /* shift the number up four bits */
    if ((res = mp_mul_2d(a, 4, a)) != MP_OKAY) {
      return res;
    }

    /* OR in the top four bits of the source */
    a->dp[0] |= (b >> 28) & 15;

    /* shift the source up to the next four bits */
    b <<= 4;

    /* ensure that digits are not clamped off */
    a->used += 1;
  }
  mp_clamp(a);
  return MP_OKAY;
}

/* set a platform dependent unsigned long int */
MP_SET_XLONG(mp_set_long, unsigned long)

/* set a platform dependent unsigned long long int */
MP_SET_XLONG(mp_set_long_long, unsigned long long)

/* computes b = a*a */
int mp_sqr(mp_int *a, mp_int *b) {
  int res;

  /* use Toom-Cook? */
  if (a->used >= TOOM_SQR_CUTOFF) {
    res = mp_toom_sqr(a, b);
    /* Karatsuba? */
  } else if (a->used >= KARATSUBA_SQR_CUTOFF) {
    res = mp_karatsuba_sqr(a, b);
  } else {
    /* can we use the fast comba multiplier? */
    if ((((a->used * 2) + 1) < MP_WARRAY) &&
        (a->used <
         (1 << (((sizeof(mp_word) * CHAR_BIT) - (2 * DIGIT_BIT)) - 1)))) {
      res = fast_s_mp_sqr(a, b);
    } else {
      res = s_mp_sqr(a, b);
    }
  }
  b->sign = MP_ZPOS;
  return res;
}

/* c = a * a (mod b) */
int mp_sqrmod(mp_int *a, mp_int *b, mp_int *c) {
  int res;
  mp_int t;

  if ((res = mp_init(&t)) != MP_OKAY) {
    return res;
  }

  if ((res = mp_sqr(a, &t)) != MP_OKAY) {
    mp_clear(&t);
    return res;
  }
  res = mp_mod(&t, b, c);
  mp_clear(&t);
  return res;
}

/* LibTomMath, multiple-precision integer library -- Tom St Denis
 *
 * LibTomMath is a library that provides multiple-precision
 * integer arithmetic as well as number theoretic functionality.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

/* high level subtraction (handles signs) */
int mp_sub(mp_int *a, mp_int *b, mp_int *c) {
  int sa, sb, res;

  sa = a->sign;
  sb = b->sign;

  if (sa != sb) {
    /* subtract a negative from a positive, OR */
    /* subtract a positive from a negative. */
    /* In either case, ADD their magnitudes, */
    /* and use the sign of the first number. */
    c->sign = sa;
    res = s_mp_add(a, b, c);
  } else {
    /* subtract a positive from a positive, OR */
    /* subtract a negative from a negative. */
    /* First, take the difference between their */
    /* magnitudes, then... */
    if (mp_cmp_mag(a, b) != MP_LT) {
      /* Copy the sign from the first */
      c->sign = sa;
      /* The first has a larger or equal magnitude */
      res = s_mp_sub(a, b, c);
    } else {
      /* The result has the *opposite* sign from */
      /* the first number. */
      c->sign = (sa == MP_ZPOS) ? MP_NEG : MP_ZPOS;
      /* The second has a larger magnitude */
      res = s_mp_sub(b, a, c);
    }
  }
  return res;
}

/* single digit subtraction */
int mp_sub_d(mp_int *a, mp_digit b, mp_int *c) {
  mp_digit *tmpa, *tmpc, mu;
  int res, ix, oldused;

  /* grow c as required */
  if (c->alloc < (a->used + 1)) {
    if ((res = mp_grow(c, a->used + 1)) != MP_OKAY) {
      return res;
    }
  }

  /* if a is negative just do an unsigned
   * addition [with fudged signs]
   */
  if (a->sign == MP_NEG) {
    a->sign = MP_ZPOS;
    res = mp_add_d(a, b, c);
    a->sign = c->sign = MP_NEG;

    /* clamp */
    mp_clamp(c);

    return res;
  }

  /* setup regs */
  oldused = c->used;
  tmpa = a->dp;
  tmpc = c->dp;

  /* if a <= b simply fix the single digit */
  if (((a->used == 1) && (a->dp[0] <= b)) || (a->used == 0)) {
    if (a->used == 1) {
      *tmpc++ = b - *tmpa;
    } else {
      *tmpc++ = b;
    }
    ix = 1;

    /* negative/1digit */
    c->sign = MP_NEG;
    c->used = 1;
  } else {
    /* positive/size */
    c->sign = MP_ZPOS;
    c->used = a->used;

    /* subtract first digit */
    *tmpc = *tmpa++ - b;
    mu = *tmpc >> ((sizeof(mp_digit) * CHAR_BIT) - 1);
    *tmpc++ &= MP_MASK;

    /* handle rest of the digits */
    for (ix = 1; ix < a->used; ix++) {
      *tmpc = *tmpa++ - mu;
      mu = *tmpc >> ((sizeof(mp_digit) * CHAR_BIT) - 1);
      *tmpc++ &= MP_MASK;
    }
  }

  /* zero excess digits */
  while (ix++ < oldused) {
    *tmpc++ = 0;
  }
  mp_clamp(c);
  return MP_OKAY;
}

/* store in unsigned [big endian] format */
int mp_to_unsigned_bin(mp_int *a, unsigned char *b) {
  int x, res;
  mp_int t;

  if ((res = mp_init_copy(&t, a)) != MP_OKAY) {
    return res;
  }

  x = 0;
  while (mp_iszero(&t) == MP_NO) {
    b[x++] = (unsigned char)(t.dp[0] & 255);
    if ((res = mp_div_2d(&t, 8, &t, NULL)) != MP_OKAY) {
      mp_clear(&t);
      return res;
    }
  }
  bn_reverse(b, x);
  mp_clear(&t);
  return MP_OKAY;
}

/* multiplication using the Toom-Cook 3-way algorithm
 *
 * Much more complicated than Karatsuba but has a lower
 * asymptotic running time of O(N**1.464).  This algorithm is
 * only particularly useful on VERY large inputs
 * (we're talking 1000s of digits here...).
 */
int mp_toom_mul(mp_int *a, mp_int *b, mp_int *c) {
  mp_int w0, w1, w2, w3, w4, tmp1, tmp2, a0, a1, a2, b0, b1, b2;
  int res, B;

  /* init temps */
  if ((res = mp_init_multi(&w0, &w1, &w2, &w3, &w4, &a0, &a1, &a2, &b0, &b1,
                           &b2, &tmp1, &tmp2, NULL)) != MP_OKAY) {
    return res;
  }

  /* B */
  B = MIN(a->used, b->used) / 3;

  /* a = a2 * B**2 + a1 * B + a0 */
  if ((res = mp_mod_2d(a, DIGIT_BIT * B, &a0)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_copy(a, &a1)) != MP_OKAY) {
    goto ERR;
  }
  mp_rshd(&a1, B);
  if ((res = mp_mod_2d(&a1, DIGIT_BIT * B, &a1)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_copy(a, &a2)) != MP_OKAY) {
    goto ERR;
  }
  mp_rshd(&a2, B * 2);

  /* b = b2 * B**2 + b1 * B + b0 */
  if ((res = mp_mod_2d(b, DIGIT_BIT * B, &b0)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_copy(b, &b1)) != MP_OKAY) {
    goto ERR;
  }
  mp_rshd(&b1, B);
  (void)mp_mod_2d(&b1, DIGIT_BIT * B, &b1);

  if ((res = mp_copy(b, &b2)) != MP_OKAY) {
    goto ERR;
  }
  mp_rshd(&b2, B * 2);

  /* w0 = a0*b0 */
  if ((res = mp_mul(&a0, &b0, &w0)) != MP_OKAY) {
    goto ERR;
  }

  /* w4 = a2 * b2 */
  if ((res = mp_mul(&a2, &b2, &w4)) != MP_OKAY) {
    goto ERR;
  }

  /* w1 = (a2 + 2(a1 + 2a0))(b2 + 2(b1 + 2b0)) */
  if ((res = mp_mul_2(&a0, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul_2(&tmp1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a2, &tmp1)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_mul_2(&b0, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp2, &b1, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul_2(&tmp2, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp2, &b2, &tmp2)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_mul(&tmp1, &tmp2, &w1)) != MP_OKAY) {
    goto ERR;
  }

  /* w3 = (a0 + 2(a1 + 2a2))(b0 + 2(b1 + 2b2)) */
  if ((res = mp_mul_2(&a2, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul_2(&tmp1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a0, &tmp1)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_mul_2(&b2, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp2, &b1, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul_2(&tmp2, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp2, &b0, &tmp2)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_mul(&tmp1, &tmp2, &w3)) != MP_OKAY) {
    goto ERR;
  }

  /* w2 = (a2 + a1 + a0)(b2 + b1 + b0) */
  if ((res = mp_add(&a2, &a1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a0, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&b2, &b1, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp2, &b0, &tmp2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul(&tmp1, &tmp2, &w2)) != MP_OKAY) {
    goto ERR;
  }

  /* now solve the matrix

   0  0  0  0  1
   1  2  4  8  16
   1  1  1  1  1
   16 8  4  2  1
   1  0  0  0  0

   using 12 subtractions, 4 shifts,
          2 small divisions and 1 small multiplication
 */

  /* r1 - r4 */
  if ((res = mp_sub(&w1, &w4, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - r0 */
  if ((res = mp_sub(&w3, &w0, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r1/2 */
  if ((res = mp_div_2(&w1, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3/2 */
  if ((res = mp_div_2(&w3, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r2 - r0 - r4 */
  if ((res = mp_sub(&w2, &w0, &w2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w2, &w4, &w2)) != MP_OKAY) {
    goto ERR;
  }
  /* r1 - r2 */
  if ((res = mp_sub(&w1, &w2, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - r2 */
  if ((res = mp_sub(&w3, &w2, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r1 - 8r0 */
  if ((res = mp_mul_2d(&w0, 3, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w1, &tmp1, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - 8r4 */
  if ((res = mp_mul_2d(&w4, 3, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w3, &tmp1, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* 3r2 - r1 - r3 */
  if ((res = mp_mul_d(&w2, 3, &w2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w2, &w1, &w2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w2, &w3, &w2)) != MP_OKAY) {
    goto ERR;
  }
  /* r1 - r2 */
  if ((res = mp_sub(&w1, &w2, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - r2 */
  if ((res = mp_sub(&w3, &w2, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r1/3 */
  if ((res = mp_div_3(&w1, &w1, NULL)) != MP_OKAY) {
    goto ERR;
  }
  /* r3/3 */
  if ((res = mp_div_3(&w3, &w3, NULL)) != MP_OKAY) {
    goto ERR;
  }

  /* at this point shift W[n] by B*n */
  if ((res = mp_lshd(&w1, 1 * B)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_lshd(&w2, 2 * B)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_lshd(&w3, 3 * B)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_lshd(&w4, 4 * B)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_add(&w0, &w1, c)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&w2, &w3, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&w4, &tmp1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, c, c)) != MP_OKAY) {
    goto ERR;
  }

ERR:
  mp_clear_multi(&w0, &w1, &w2, &w3, &w4, &a0, &a1, &a2, &b0, &b1, &b2, &tmp1,
                 &tmp2, NULL);
  return res;
}

/* squaring using Toom-Cook 3-way algorithm */
int mp_toom_sqr(mp_int *a, mp_int *b) {
  mp_int w0, w1, w2, w3, w4, tmp1, a0, a1, a2;
  int res, B;

  /* init temps */
  if ((res = mp_init_multi(&w0, &w1, &w2, &w3, &w4, &a0, &a1, &a2, &tmp1,
                           NULL)) != MP_OKAY) {
    return res;
  }

  /* B */
  B = a->used / 3;

  /* a = a2 * B**2 + a1 * B + a0 */
  if ((res = mp_mod_2d(a, DIGIT_BIT * B, &a0)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_copy(a, &a1)) != MP_OKAY) {
    goto ERR;
  }
  mp_rshd(&a1, B);
  if ((res = mp_mod_2d(&a1, DIGIT_BIT * B, &a1)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_copy(a, &a2)) != MP_OKAY) {
    goto ERR;
  }
  mp_rshd(&a2, B * 2);

  /* w0 = a0*a0 */
  if ((res = mp_sqr(&a0, &w0)) != MP_OKAY) {
    goto ERR;
  }

  /* w4 = a2 * a2 */
  if ((res = mp_sqr(&a2, &w4)) != MP_OKAY) {
    goto ERR;
  }

  /* w1 = (a2 + 2(a1 + 2a0))**2 */
  if ((res = mp_mul_2(&a0, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul_2(&tmp1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a2, &tmp1)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_sqr(&tmp1, &w1)) != MP_OKAY) {
    goto ERR;
  }

  /* w3 = (a0 + 2(a1 + 2a2))**2 */
  if ((res = mp_mul_2(&a2, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_mul_2(&tmp1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a0, &tmp1)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_sqr(&tmp1, &w3)) != MP_OKAY) {
    goto ERR;
  }

  /* w2 = (a2 + a1 + a0)**2 */
  if ((res = mp_add(&a2, &a1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, &a0, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sqr(&tmp1, &w2)) != MP_OKAY) {
    goto ERR;
  }

  /* now solve the matrix

   0  0  0  0  1
   1  2  4  8  16
   1  1  1  1  1
   16 8  4  2  1
   1  0  0  0  0

   using 12 subtractions, 4 shifts, 2 small divisions and 1 small
   multiplication.
 */

  /* r1 - r4 */
  if ((res = mp_sub(&w1, &w4, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - r0 */
  if ((res = mp_sub(&w3, &w0, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r1/2 */
  if ((res = mp_div_2(&w1, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3/2 */
  if ((res = mp_div_2(&w3, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r2 - r0 - r4 */
  if ((res = mp_sub(&w2, &w0, &w2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w2, &w4, &w2)) != MP_OKAY) {
    goto ERR;
  }
  /* r1 - r2 */
  if ((res = mp_sub(&w1, &w2, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - r2 */
  if ((res = mp_sub(&w3, &w2, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r1 - 8r0 */
  if ((res = mp_mul_2d(&w0, 3, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w1, &tmp1, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - 8r4 */
  if ((res = mp_mul_2d(&w4, 3, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w3, &tmp1, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* 3r2 - r1 - r3 */
  if ((res = mp_mul_d(&w2, 3, &w2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w2, &w1, &w2)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_sub(&w2, &w3, &w2)) != MP_OKAY) {
    goto ERR;
  }
  /* r1 - r2 */
  if ((res = mp_sub(&w1, &w2, &w1)) != MP_OKAY) {
    goto ERR;
  }
  /* r3 - r2 */
  if ((res = mp_sub(&w3, &w2, &w3)) != MP_OKAY) {
    goto ERR;
  }
  /* r1/3 */
  if ((res = mp_div_3(&w1, &w1, NULL)) != MP_OKAY) {
    goto ERR;
  }
  /* r3/3 */
  if ((res = mp_div_3(&w3, &w3, NULL)) != MP_OKAY) {
    goto ERR;
  }

  /* at this point shift W[n] by B*n */
  if ((res = mp_lshd(&w1, 1 * B)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_lshd(&w2, 2 * B)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_lshd(&w3, 3 * B)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_lshd(&w4, 4 * B)) != MP_OKAY) {
    goto ERR;
  }

  if ((res = mp_add(&w0, &w1, b)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&w2, &w3, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&w4, &tmp1, &tmp1)) != MP_OKAY) {
    goto ERR;
  }
  if ((res = mp_add(&tmp1, b, b)) != MP_OKAY) {
    goto ERR;
  }

ERR:
  mp_clear_multi(&w0, &w1, &w2, &w3, &w4, &a0, &a1, &a2, &tmp1, NULL);
  return res;
}

/* stores a bignum as a ASCII string in a given radix (2..64) */
int mp_toradix(mp_int *a, char *str, int radix) {
  int res, digs;
  mp_int t;
  mp_digit d;
  char *_s = str;

  /* check range of the radix */
  if ((radix < 2) || (radix > 64)) {
    return MP_VAL;
  }

  /* quick out if its zero */
  if (mp_iszero(a) == MP_YES) {
    *str++ = '0';
    *str = '\0';
    return MP_OKAY;
  }

  if ((res = mp_init_copy(&t, a)) != MP_OKAY) {
    return res;
  }

  /* if it is negative output a - */
  if (t.sign == MP_NEG) {
    ++_s;
    *str++ = '-';
    t.sign = MP_ZPOS;
  }

  digs = 0;
  while (mp_iszero(&t) == MP_NO) {
    if ((res = mp_div_d(&t, (mp_digit)radix, &t, &d)) != MP_OKAY) {
      mp_clear(&t);
      return res;
    }
    *str++ = mp_s_rmap[d];
    ++digs;
  }

  /* reverse the digits of the string.  In this case _s points
   * to the first digit [exluding the sign] of the number]
   */
  bn_reverse((unsigned char *)_s, digs);

  /* append a NULL so the string is properly terminated */
  *str = '\0';

  mp_clear(&t);
  return MP_OKAY;
}

/* get the size for an unsigned equivalent */
int mp_unsigned_bin_size(mp_int *a) {
  int size = mp_count_bits(a);

  return (size / 8) + (((size & 7) != 0) ? 1 : 0);
}

/* set to zero */
void mp_zero(mp_int *a) {
  int n;
  mp_digit *tmp;

  a->sign = MP_ZPOS;
  a->used = 0;

  tmp = a->dp;
  for (n = 0; n < a->alloc; n++) {
    *tmp++ = 0;
  }
}

const mp_digit ltm_prime_tab[] = {
    0x0002, 0x0003, 0x0005, 0x0007, 0x000B, 0x000D, 0x0011, 0x0013,
    0x0017, 0x001D, 0x001F, 0x0025, 0x0029, 0x002B, 0x002F, 0x0035,
    0x003B, 0x003D, 0x0043, 0x0047, 0x0049, 0x004F, 0x0053, 0x0059,
    0x0061, 0x0065, 0x0067, 0x006B, 0x006D, 0x0071, 0x007F, 0x0083,
    0x0089, 0x008B, 0x0095, 0x0097, 0x009D, 0x00A3, 0x00A7, 0x00AD,
    0x00B3, 0x00B5, 0x00BF, 0x00C1, 0x00C5, 0x00C7, 0x00D3, 0x00DF,
    0x00E3, 0x00E5, 0x00E9, 0x00EF, 0x00F1, 0x00FB, 0x0101, 0x0107,
    0x010D, 0x010F, 0x0115, 0x0119, 0x011B, 0x0125, 0x0133, 0x0137,

    0x0139, 0x013D, 0x014B, 0x0151, 0x015B, 0x015D, 0x0161, 0x0167,
    0x016F, 0x0175, 0x017B, 0x017F, 0x0185, 0x018D, 0x0191, 0x0199,
    0x01A3, 0x01A5, 0x01AF, 0x01B1, 0x01B7, 0x01BB, 0x01C1, 0x01C9,
    0x01CD, 0x01CF, 0x01D3, 0x01DF, 0x01E7, 0x01EB, 0x01F3, 0x01F7,
    0x01FD, 0x0209, 0x020B, 0x021D, 0x0223, 0x022D, 0x0233, 0x0239,
    0x023B, 0x0241, 0x024B, 0x0251, 0x0257, 0x0259, 0x025F, 0x0265,
    0x0269, 0x026B, 0x0277, 0x0281, 0x0283, 0x0287, 0x028D, 0x0293,
    0x0295, 0x02A1, 0x02A5, 0x02AB, 0x02B3, 0x02BD, 0x02C5, 0x02CF,

    0x02D7, 0x02DD, 0x02E3, 0x02E7, 0x02EF, 0x02F5, 0x02F9, 0x0301,
    0x0305, 0x0313, 0x031D, 0x0329, 0x032B, 0x0335, 0x0337, 0x033B,
    0x033D, 0x0347, 0x0355, 0x0359, 0x035B, 0x035F, 0x036D, 0x0371,
    0x0373, 0x0377, 0x038B, 0x038F, 0x0397, 0x03A1, 0x03A9, 0x03AD,
    0x03B3, 0x03B9, 0x03C7, 0x03CB, 0x03D1, 0x03D7, 0x03DF, 0x03E5,
    0x03F1, 0x03F5, 0x03FB, 0x03FD, 0x0407, 0x0409, 0x040F, 0x0419,
    0x041B, 0x0425, 0x0427, 0x042D, 0x043F, 0x0443, 0x0445, 0x0449,
    0x044F, 0x0455, 0x045D, 0x0463, 0x0469, 0x047F, 0x0481, 0x048B,

    0x0493, 0x049D, 0x04A3, 0x04A9, 0x04B1, 0x04BD, 0x04C1, 0x04C7,
    0x04CD, 0x04CF, 0x04D5, 0x04E1, 0x04EB, 0x04FD, 0x04FF, 0x0503,
    0x0509, 0x050B, 0x0511, 0x0515, 0x0517, 0x051B, 0x0527, 0x0529,
    0x052F, 0x0551, 0x0557, 0x055D, 0x0565, 0x0577, 0x0581, 0x058F,
    0x0593, 0x0595, 0x0599, 0x059F, 0x05A7, 0x05AB, 0x05AD, 0x05B3,
    0x05BF, 0x05C9, 0x05CB, 0x05CF, 0x05D1, 0x05D5, 0x05DB, 0x05E7,
    0x05F3, 0x05FB, 0x0607, 0x060D, 0x0611, 0x0617, 0x061F, 0x0623,
    0x062B, 0x062F, 0x063D, 0x0641, 0x0647, 0x0649, 0x064D, 0x0653};

/* reverse an array, used for radix code */
void bn_reverse(unsigned char *s, int len) {
  int ix, iy;
  unsigned char t;

  ix = 0;
  iy = len - 1;
  while (ix < iy) {
    t = s[ix];
    s[ix] = s[iy];
    s[iy] = t;
    ++ix;
    --iy;
  }
}

/* low level addition, based on HAC pp.594, Algorithm 14.7 */
int s_mp_add(mp_int *a, mp_int *b, mp_int *c) {
  mp_int *x;
  int olduse, res, min, max;

  /* find sizes, we let |a| <= |b| which means we have to sort
   * them.  "x" will point to the input with the most digits
   */
  if (a->used > b->used) {
    min = b->used;
    max = a->used;
    x = a;
  } else {
    min = a->used;
    max = b->used;
    x = b;
  }

  /* init result */
  if (c->alloc < (max + 1)) {
    if ((res = mp_grow(c, max + 1)) != MP_OKAY) {
      return res;
    }
  }

  /* get old used digit count and set new one */
  olduse = c->used;
  c->used = max + 1;

  {
    mp_digit u, *tmpa, *tmpb, *tmpc;
    int i;

    /* alias for digit pointers */

    /* first input */
    tmpa = a->dp;

    /* second input */
    tmpb = b->dp;

    /* destination */
    tmpc = c->dp;

    /* zero the carry */
    u = 0;
    for (i = 0; i < min; i++) {
      /* Compute the sum at one digit, T[i] = A[i] + B[i] + U */
      *tmpc = *tmpa++ + *tmpb++ + u;

      /* U = carry bit of T[i] */
      u = *tmpc >> ((mp_digit)DIGIT_BIT);

      /* take away carry bit from T[i] */
      *tmpc++ &= MP_MASK;
    }

    /* now copy higher words if any, that is in A+B
     * if A or B has more digits add those in
     */
    if (min != max) {
      for (; i < max; i++) {
        /* T[i] = X[i] + U */
        *tmpc = x->dp[i] + u;

        /* U = carry bit of T[i] */
        u = *tmpc >> ((mp_digit)DIGIT_BIT);

        /* take away carry bit from T[i] */
        *tmpc++ &= MP_MASK;
      }
    }

    /* add carry */
    *tmpc++ = u;

    /* clear digits above oldused */
    for (i = c->used; i < olduse; i++) {
      *tmpc++ = 0;
    }
  }

  mp_clamp(c);
  return MP_OKAY;
}

int s_mp_exptmod(mp_int *G, mp_int *X, mp_int *P, mp_int *Y, int redmode) {
  mp_int M[TAB_SIZE], res, mu;
  mp_digit buf;
  int err, bitbuf, bitcpy, bitcnt, mode, digidx, x, y, winsize;

  int (*redux)(mp_int *, mp_int *, mp_int *);

  /* find window size */
  x = mp_count_bits(X);
  if (x <= 7) {
    winsize = 2;
  } else if (x <= 36) {
    winsize = 3;
  } else if (x <= 140) {
    winsize = 4;
  } else if (x <= 450) {
    winsize = 5;
  } else if (x <= 1303) {
    winsize = 6;
  } else if (x <= 3529) {
    winsize = 7;
  } else {
    winsize = 8;
  }

  /* init M array */
  /* init first cell */
  if ((err = mp_init(&M[1])) != MP_OKAY) {
    return err;
  }

  /* now init the second half of the array */
  for (x = 1 << (winsize - 1); x < (1 << winsize); x++) {
    if ((err = mp_init(&M[x])) != MP_OKAY) {
      for (y = 1 << (winsize - 1); y < x; y++) {
        mp_clear(&M[y]);
      }
      mp_clear(&M[1]);
      return err;
    }
  }

  /* create mu, used for Barrett reduction */
  if ((err = mp_init(&mu)) != MP_OKAY) {
    goto LBL_M;
  }

  if (redmode == 0) {
    if ((err = mp_reduce_setup(&mu, P)) != MP_OKAY) {
      goto LBL_MU;
    }
    redux = mp_reduce;
  } else {
    if ((err = mp_reduce_2k_setup_l(P, &mu)) != MP_OKAY) {
      goto LBL_MU;
    }
    redux = mp_reduce_2k_l;
  }

  /* create M table
   *
   * The M table contains powers of the base,
   * e.g. M[x] = G**x mod P
   *
   * The first half of the table is not
   * computed though accept for M[0] and M[1]
   */
  if ((err = mp_mod(G, P, &M[1])) != MP_OKAY) {
    goto LBL_MU;
  }

  /* compute the value at M[1<<(winsize-1)] by squaring
   * M[1] (winsize-1) times
   */
  if ((err = mp_copy(&M[1], &M[1 << (winsize - 1)])) != MP_OKAY) {
    goto LBL_MU;
  }

  for (x = 0; x < (winsize - 1); x++) {
    /* square it */
    if ((err = mp_sqr(&M[1 << (winsize - 1)], &M[1 << (winsize - 1)])) !=
        MP_OKAY) {
      goto LBL_MU;
    }

    /* reduce modulo P */
    if ((err = redux(&M[1 << (winsize - 1)], P, &mu)) != MP_OKAY) {
      goto LBL_MU;
    }
  }

  /* create upper table, that is M[x] = M[x-1] * M[1] (mod P)
   * for x = (2**(winsize - 1) + 1) to (2**winsize - 1)
   */
  for (x = (1 << (winsize - 1)) + 1; x < (1 << winsize); x++) {
    if ((err = mp_mul(&M[x - 1], &M[1], &M[x])) != MP_OKAY) {
      goto LBL_MU;
    }
    if ((err = redux(&M[x], P, &mu)) != MP_OKAY) {
      goto LBL_MU;
    }
  }

  /* setup result */
  if ((err = mp_init(&res)) != MP_OKAY) {
    goto LBL_MU;
  }
  mp_set(&res, 1);

  /* set initial mode and bit cnt */
  mode = 0;
  bitcnt = 1;
  buf = 0;
  digidx = X->used - 1;
  bitcpy = 0;
  bitbuf = 0;

  for (;;) {
    /* grab next digit as required */
    if (--bitcnt == 0) {
      /* if digidx == -1 we are out of digits */
      if (digidx == -1) {
        break;
      }
      /* read next digit and reset the bitcnt */
      buf = X->dp[digidx--];
      bitcnt = (int)DIGIT_BIT;
    }

    /* grab the next msb from the exponent */
    y = (buf >> (mp_digit)(DIGIT_BIT - 1)) & 1;
    buf <<= (mp_digit)1;

    /* if the bit is zero and mode == 0 then we ignore it
     * These represent the leading zero bits before the first 1 bit
     * in the exponent.  Technically this opt is not required but it
     * does lower the # of trivial squaring/reductions used
     */
    if ((mode == 0) && (y == 0)) {
      continue;
    }

    /* if the bit is zero and mode == 1 then we square */
    if ((mode == 1) && (y == 0)) {
      if ((err = mp_sqr(&res, &res)) != MP_OKAY) {
        goto LBL_RES;
      }
      if ((err = redux(&res, P, &mu)) != MP_OKAY) {
        goto LBL_RES;
      }
      continue;
    }

    /* else we add it to the window */
    bitbuf |= (y << (winsize - ++bitcpy));
    mode = 2;

    if (bitcpy == winsize) {
      /* ok window is filled so square as required and multiply  */
      /* square first */
      for (x = 0; x < winsize; x++) {
        if ((err = mp_sqr(&res, &res)) != MP_OKAY) {
          goto LBL_RES;
        }
        if ((err = redux(&res, P, &mu)) != MP_OKAY) {
          goto LBL_RES;
        }
      }

      /* then multiply */
      if ((err = mp_mul(&res, &M[bitbuf], &res)) != MP_OKAY) {
        goto LBL_RES;
      }
      if ((err = redux(&res, P, &mu)) != MP_OKAY) {
        goto LBL_RES;
      }

      /* empty window and reset */
      bitcpy = 0;
      bitbuf = 0;
      mode = 1;
    }
  }

  /* if bits remain then square/multiply */
  if ((mode == 2) && (bitcpy > 0)) {
    /* square then multiply if the bit is set */
    for (x = 0; x < bitcpy; x++) {
      if ((err = mp_sqr(&res, &res)) != MP_OKAY) {
        goto LBL_RES;
      }
      if ((err = redux(&res, P, &mu)) != MP_OKAY) {
        goto LBL_RES;
      }

      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0) {
        /* then multiply */
        if ((err = mp_mul(&res, &M[1], &res)) != MP_OKAY) {
          goto LBL_RES;
        }
        if ((err = redux(&res, P, &mu)) != MP_OKAY) {
          goto LBL_RES;
        }
      }
    }
  }

  mp_exch(&res, Y);
  err = MP_OKAY;
LBL_RES:
  mp_clear(&res);
LBL_MU:
  mp_clear(&mu);
LBL_M:
  mp_clear(&M[1]);
  for (x = 1 << (winsize - 1); x < (1 << winsize); x++) {
    mp_clear(&M[x]);
  }
  return err;
}

/* multiplies |a| * |b| and only computes upto digs digits of result
 * HAC pp. 595, Algorithm 14.12  Modified so you can control how
 * many digits of output are created.
 */
int s_mp_mul_digs(mp_int *a, mp_int *b, mp_int *c, int digs) {
  mp_int t;
  int res, pa, pb, ix, iy;
  mp_digit u;
  mp_word r;
  mp_digit tmpx, *tmpt, *tmpy;

  /* can we use the fast multiplier? */
  if (((digs) < MP_WARRAY) &&
      (MIN(a->used, b->used) <
       (1 << ((CHAR_BIT * sizeof(mp_word)) - (2 * DIGIT_BIT))))) {
    return fast_s_mp_mul_digs(a, b, c, digs);
  }

  if ((res = mp_init_size(&t, digs)) != MP_OKAY) {
    return res;
  }
  t.used = digs;

  /* compute the digits of the product directly */
  pa = a->used;
  for (ix = 0; ix < pa; ix++) {
    /* set the carry to zero */
    u = 0;

    /* limit ourselves to making digs digits of output */
    pb = MIN(b->used, digs - ix);

    /* setup some aliases */
    /* copy of the digit from a used within the nested loop */
    tmpx = a->dp[ix];

    /* an alias for the destination shifted ix places */
    tmpt = t.dp + ix;

    /* an alias for the digits of b */
    tmpy = b->dp;

    /* compute the columns of the output and propagate the carry */
    for (iy = 0; iy < pb; iy++) {
      /* compute the column as a mp_word */
      r = (mp_word)*tmpt + ((mp_word)tmpx * (mp_word)*tmpy++) + (mp_word)u;

      /* the new column is the lower part of the result */
      *tmpt++ = (mp_digit)(r & ((mp_word)MP_MASK));

      /* get the carry word from the result */
      u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));
    }
    /* set carry if it is placed below digs */
    if ((ix + iy) < digs) {
      *tmpt = u;
    }
  }

  mp_clamp(&t);
  mp_exch(&t, c);

  mp_clear(&t);
  return MP_OKAY;
}

/* multiplies |a| * |b| and does not compute the lower digs digits
 * [meant to get the higher part of the product]
 */
int s_mp_mul_high_digs(mp_int *a, mp_int *b, mp_int *c, int digs) {
  mp_int t;
  int res, pa, pb, ix, iy;
  mp_digit u;
  mp_word r;
  mp_digit tmpx, *tmpt, *tmpy;

  /* can we use the fast multiplier? */
  if (((a->used + b->used + 1) < MP_WARRAY) &&
      (MIN(a->used, b->used) <
       (1 << ((CHAR_BIT * sizeof(mp_word)) - (2 * DIGIT_BIT))))) {
    return fast_s_mp_mul_high_digs(a, b, c, digs);
  }

  if ((res = mp_init_size(&t, a->used + b->used + 1)) != MP_OKAY) {
    return res;
  }
  t.used = a->used + b->used + 1;

  pa = a->used;
  pb = b->used;
  for (ix = 0; ix < pa; ix++) {
    /* clear the carry */
    u = 0;

    /* left hand side of A[ix] * B[iy] */
    tmpx = a->dp[ix];

    /* alias to the address of where the digits will be stored */
    tmpt = &(t.dp[digs]);

    /* alias for where to read the right hand side from */
    tmpy = b->dp + (digs - ix);

    for (iy = digs - ix; iy < pb; iy++) {
      /* calculate the double precision result */
      r = (mp_word)*tmpt + ((mp_word)tmpx * (mp_word)*tmpy++) + (mp_word)u;

      /* get the lower part */
      *tmpt++ = (mp_digit)(r & ((mp_word)MP_MASK));

      /* carry the carry */
      u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));
    }
    *tmpt = u;
  }
  mp_clamp(&t);
  mp_exch(&t, c);
  mp_clear(&t);
  return MP_OKAY;
}

/* low level squaring, b = a*a, HAC pp.596-597, Algorithm 14.16 */
int s_mp_sqr(mp_int *a, mp_int *b) {
  mp_int t;
  int res, ix, iy, pa;
  mp_word r;
  mp_digit u, tmpx, *tmpt;

  pa = a->used;
  if ((res = mp_init_size(&t, (2 * pa) + 1)) != MP_OKAY) {
    return res;
  }

  /* default used is maximum possible size */
  t.used = (2 * pa) + 1;

  for (ix = 0; ix < pa; ix++) {
    /* first calculate the digit at 2*ix */
    /* calculate double precision result */
    r = (mp_word)t.dp[2 * ix] + ((mp_word)a->dp[ix] * (mp_word)a->dp[ix]);

    /* store lower part in result */
    t.dp[ix + ix] = (mp_digit)(r & ((mp_word)MP_MASK));

    /* get the carry */
    u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));

    /* left hand side of A[ix] * A[iy] */
    tmpx = a->dp[ix];

    /* alias for where to store the results */
    tmpt = t.dp + ((2 * ix) + 1);

    for (iy = ix + 1; iy < pa; iy++) {
      /* first calculate the product */
      r = ((mp_word)tmpx) * ((mp_word)a->dp[iy]);

      /* now calculate the double precision result, note we use
       * addition instead of *2 since it's easier to optimize
       */
      r = ((mp_word)*tmpt) + r + r + ((mp_word)u);

      /* store lower part */
      *tmpt++ = (mp_digit)(r & ((mp_word)MP_MASK));

      /* get carry */
      u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));
    }
    /* propagate upwards */
    while (u != ((mp_digit)0)) {
      r = ((mp_word)*tmpt) + ((mp_word)u);
      *tmpt++ = (mp_digit)(r & ((mp_word)MP_MASK));
      u = (mp_digit)(r >> ((mp_word)DIGIT_BIT));
    }
  }

  mp_clamp(&t);
  mp_exch(&t, b);
  mp_clear(&t);
  return MP_OKAY;
}

/* low level subtraction (assumes |a| > |b|), HAC pp.595 Algorithm 14.9 */
int s_mp_sub(mp_int *a, mp_int *b, mp_int *c) {
  int olduse, res, min, max;

  /* find sizes */
  min = b->used;
  max = a->used;

  /* init result */
  if (c->alloc < max) {
    if ((res = mp_grow(c, max)) != MP_OKAY) {
      return res;
    }
  }
  olduse = c->used;
  c->used = max;

  {
    mp_digit u, *tmpa, *tmpb, *tmpc;
    int i;

    /* alias for digit pointers */
    tmpa = a->dp;
    tmpb = b->dp;
    tmpc = c->dp;

    /* set carry to zero */
    u = 0;
    for (i = 0; i < min; i++) {
      /* T[i] = A[i] - B[i] - U */
      *tmpc = (*tmpa++ - *tmpb++) - u;

      /* U = carry bit of T[i]
       * Note this saves performing an AND operation since
       * if a carry does occur it will propagate all the way to the
       * MSB.  As a result a single shift is enough to get the carry
       */
      u = *tmpc >> ((mp_digit)((CHAR_BIT * sizeof(mp_digit)) - 1));

      /* Clear carry from T[i] */
      *tmpc++ &= MP_MASK;
    }

    /* now copy higher words if any, e.g. if A has more digits than B  */
    for (; i < max; i++) {
      /* T[i] = A[i] - U */
      *tmpc = *tmpa++ - u;

      /* U = carry bit of T[i] */
      u = *tmpc >> ((mp_digit)((CHAR_BIT * sizeof(mp_digit)) - 1));

      /* Clear carry from T[i] */
      *tmpc++ &= MP_MASK;
    }

    /* clear digits above used (since we may not have grown result above) */
    for (i = c->used; i < olduse; i++) {
      *tmpc++ = 0;
    }
  }

  mp_clamp(c);
  return MP_OKAY;
}

/* Known optimal configurations

   CPU                    /Compiler     /MUL CUTOFF/SQR CUTOFF
   -------------------------------------------------------------
   Intel P4 Northwood     /GCC v3.4.1   /        88/       128/LTM 0.32 ;-)
   AMD Athlon64           /GCC v3.4.4   /        80/       120/LTM 0.35

 */

int KARATSUBA_MUL_CUTOFF =
        80, /* Min. number of digits before Karatsuba multiplication is used. */
    KARATSUBA_SQR_CUTOFF =
        120, /* Min. number of digits before Karatsuba squaring is used. */

    TOOM_MUL_CUTOFF =
        350, /* no optimal values of these are known yet so set em high */
    TOOM_SQR_CUTOFF = 400;

#include <signal.h>

void crypt_argchk(char *v, char *s, int d) {
  fprintf(stderr, "LTC_ARGCHK '%s' failure on line %d of file %s\n", v, d, s);
  (void)raise(SIGABRT);
}

#include <string.h>
#include <time.h>

#define XMEMSET memset
#define XMEMCPY memcpy
#define XMEMCMP memcmp
#define XSTRCMP strcmp
#define XCLOCK clock

#define XQSORT qsort

#define LRW_TABLES

#define LTC_GCM_MODE

/* Use 64KiB tables */
#define LTC_GCM_TABLES

/* Supported ECC Key Sizes */
#define ECC112
#define ECC128
#define ECC160
#define ECC192
#define ECC224
#define ECC256
#define ECC384
#define ECC521

/* THREAD management */

/* default no functions */
#define LTC_MUTEX_GLOBAL(x)
#define LTC_MUTEX_PROTO(x)
#define LTC_MUTEX_LOCK(x)
#define LTC_MUTEX_UNLOCK(x)

/* version */

/* max size of either a cipher/hash block or symmetric key [largest of the two]
 */

/* descriptor table size */

/* error codes [will be expanded in future releases] */
enum {
  CRYPT_OK = 0, /* Result OK */
  CRYPT_ERROR,  /* Generic Error */
  CRYPT_NOP,    /* Not a failure but no operation was performed */

  CRYPT_INVALID_KEYSIZE, /* Invalid key size given */
  CRYPT_INVALID_ROUNDS   /* Invalid number of rounds */
  ,                      /* Algorithm failed test vectors */

  CRYPT_BUFFER_OVERFLOW, /* Not enough space for output */
  CRYPT_INVALID_PACKET,  /* Invalid input packet given */

  CRYPT_INVALID_PRNGSIZE, /* Invalid number of bits for a PRNG */
  CRYPT_ERROR_READPRNG,   /* Could not read enough from PRNG */

  CRYPT_INVALID_CIPHER, /* Invalid cipher specified */
  CRYPT_INVALID_HASH,   /* Invalid hash specified */
  CRYPT_INVALID_PRNG,   /* Invalid PRNG specified */

  CRYPT_MEM, /* Out of memory */

  CRYPT_PK_TYPE_MISMATCH, /* Not equivalent types of PK keys */
  CRYPT_PK_NOT_PRIVATE,   /* Requires a private PK key */

  CRYPT_INVALID_ARG /* Generic invalid argument */
  ,                 /* File Not Found */

  CRYPT_PK_INVALID_TYPE, /* Invalid type of PK key */

  CRYPT_PK_INVALID_SIZE, /* Invalid size input for PK parameters */

  CRYPT_INVALID_PRIME_SIZE, /* Invalid size of prime requested */
  CRYPT_PK_INVALID_PADDING  /* Invalid padding on input */
};

/* certain platforms use macros for these, making the prototypes broken */

/* you can change how memory allocation works ... */
void *XMALLOC(size_t n);

void *XREALLOC(void *p, size_t n);

void *XCALLOC(size_t n, size_t s);

void XFREE(void *p);

void XQSORT(void *base, size_t nmemb, size_t size,
            int (*compar)(const void *, const void *));

/* change the clock function too */
clock_t XCLOCK(void);

/* various other functions */
void *XMEMCPY(void *dest, const void *src, size_t n);

int XMEMCMP(const void *s1, const void *s2, size_t n);

void *XMEMSET(void *s, int c, size_t n);

int XSTRCMP(const char *s1, const char *s2);

#define LTC_FAST
#define LTC_FAST_TYPE unsigned long

#define CONST64(n) n##ULL
typedef unsigned long long ulong64;

/* this is the "32-bit at least" data type
 * Re-define it to suit your platform but it must be at least 32-bits
 */
typedef unsigned ulong32;

/* ---- HELPER MACROS ---- */

#define STORE32H(x, y)               \
  asm __volatile__(                  \
      "bswapl %0     \n\t"           \
      "movl   %0,(%1)\n\t"           \
      "bswapl %0     \n\t" ::"r"(x), \
      "r"(y));

#define LOAD32H(x, y)    \
  asm __volatile__(      \
      "movl (%1),%0\n\t" \
      "bswapl %0\n\t"    \
      : "=r"(x)          \
      : "r"(y));

#define STORE64H(x, y)               \
  asm __volatile__(                  \
      "bswapq %0     \n\t"           \
      "movq   %0,(%1)\n\t"           \
      "bswapq %0     \n\t" ::"r"(x), \
      "r"(y));

#define LOAD64H(x, y)    \
  asm __volatile__(      \
      "movq (%1),%0\n\t" \
      "bswapq %0\n\t"    \
      : "=r"(x)          \
      : "r"(y));

#define STORE32L(x, y)   \
  {                      \
    ulong32 __t = (x);   \
    XMEMCPY(y, &__t, 4); \
  }

#define LOAD32L(x, y)    \
  {                      \
    XMEMCPY(&(x), y, 4); \
    x &= 0xFFFFFFFF;     \
  }

#define STORE64L(x, y)   \
  {                      \
    ulong64 __t = (x);   \
    XMEMCPY(y, &__t, 8); \
  }

#define LOAD64L(x, y) \
  { XMEMCPY(&(x), y, 8); }

static inline unsigned ROL(unsigned word, int i) {
  asm("roll %%cl,%0" : "=r"(word) : "0"(word), "c"(i));
  return word;
}

static inline unsigned ROR(unsigned word, int i) {
  asm("rorl %%cl,%0" : "=r"(word) : "0"(word), "c"(i));
  return word;
}

#define ROLc ROL
#define RORc ROR

static inline unsigned long ROR64(unsigned long word, int i) {
  asm("rorq %%cl,%0" : "=r"(word) : "0"(word), "c"(i));
  return word;
}

#define ROR64c ROR64

#define byte(x, n) (((x) >> (8 * (n))) & 255)

/* ---- SYMMETRIC KEY STUFF -----
 *
 * We put each of the ciphers scheduled keys in their own structs then we put
 * all of the key formats in one union.  This makes the function prototypes
 * easier to use.
 */

struct rijndael_key {
  ulong32 eK[60], dK[60];
  int Nr;
};

typedef union Symmetric_key {
  struct rijndael_key rijndael;
  void *data;
} symmetric_key;

/** cipher descriptor table, last entry has "name == NULL" to mark the end of
 * table */
extern struct ltc_cipher_descriptor {
  /** name of cipher */
  char *name;
  /** internal ID */
  unsigned char ID;
  /** min keysize (octets) */
  int
      /** max keysize (octets) */
      /** block size (octets) */
      block_length;

  /** Setup the cipher
   @param key         The input symmetric key
   @param keylen      The length of the input key (octets)
   @param num_rounds  The requested number of rounds (0==default)
   @param skey        [out] The destination of the scheduled key
   @return CRYPT_OK if successful
 */
  int (*setup)(const unsigned char *key, int keylen, int num_rounds,
               symmetric_key *skey);

  /** Encrypt a block
   @param pt      The plaintext
   @param ct      [out] The ciphertext
   @param skey    The scheduled key
   @return CRYPT_OK if successful
 */
  int (*ecb_encrypt)(const unsigned char *pt, unsigned char *ct,
                     symmetric_key *skey);

  /** Decrypt a block
   @param ct      The ciphertext
   @param pt      [out] The plaintext
   @param skey    The scheduled key
   @return CRYPT_OK if successful
 */

  /** Terminate the context
   @param skey    The scheduled key
 */
  void (*done)(symmetric_key *skey);

} cipher_descriptor[];

/* make aes an alias */

int rijndael_setup(const unsigned char *key, int keylen, int num_rounds,
                   symmetric_key *skey);

int rijndael_ecb_encrypt(const unsigned char *pt, unsigned char *ct,
                         symmetric_key *skey);

int rijndael_ecb_decrypt(const unsigned char *ct, unsigned char *pt,
                         symmetric_key *skey);

int rijndael_test(void);

void rijndael_done(symmetric_key *skey);

int rijndael_keysize(int *keysize);

extern const struct ltc_cipher_descriptor aes_desc;

int find_cipher(const char *name);

int register_cipher(const struct ltc_cipher_descriptor *cipher);

int cipher_is_valid(int idx);

LTC_MUTEX_PROTO(ltc_cipher_mutex)

/* ---- HASH FUNCTIONS ---- */
struct sha512_state {
  ulong64 length, state[8];
  unsigned long curlen;
  unsigned char buf[128];
};

struct sha256_state {
  ulong64 length;
  ulong32 state[8], curlen;
  unsigned char buf[64];
};

struct sha1_state {
  ulong64 length;
  ulong32 state[5], curlen;
  unsigned char buf[64];
};

struct md5_state {
  ulong64 length;
  ulong32 state[4], curlen;
  unsigned char buf[64];
};

typedef union Hash_state {
  struct sha512_state sha512;
  struct sha256_state sha256;
  struct sha1_state sha1;
  struct md5_state md5;
  void *data;
} hash_state;

/** hash descriptor */
extern struct ltc_hash_descriptor {
  /** name of hash */
  char *name;
  /** Size of digest in octets */
  unsigned long hashsize;
  /** Input block size in octets */
  unsigned long blocksize;
  /** ASN.1 OID */
  unsigned long OID[16];
  /** Length of DER encoding */
  unsigned long OIDlen;

  /** Init a hash state
   @param hash   The hash to initialize
   @return CRYPT_OK if successful
 */
  int (*init)(hash_state *hash);

  /** Process a block of data
   @param hash   The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return CRYPT_OK if successful
 */
  int (*process)(hash_state *hash, const unsigned char *in,
                 unsigned long inlen);

  /** Produce the digest and store it
   @param hash   The hash state
   @param out    [out] The destination of the digest
   @return CRYPT_OK if successful
 */
  int (*done)(hash_state *hash, unsigned char *out);

} hash_descriptor[];

int sha512_init(hash_state *md);

int sha512_process(hash_state *md, const unsigned char *in,
                   unsigned long inlen);

int sha512_done(hash_state *md, unsigned char *hash);

extern const struct ltc_hash_descriptor sha512_desc;

int sha384_init(hash_state *md);

#define sha384_process sha512_process

int sha384_done(hash_state *md, unsigned char *hash);

extern const struct ltc_hash_descriptor sha384_desc;

int sha256_init(hash_state *md);

int sha256_process(hash_state *md, const unsigned char *in,
                   unsigned long inlen);

int sha256_done(hash_state *md, unsigned char *hash);

extern const struct ltc_hash_descriptor sha256_desc;

int sha1_init(hash_state *md);

int sha1_process(hash_state *md, const unsigned char *in, unsigned long inlen);

int sha1_done(hash_state *md, unsigned char *hash);

extern const struct ltc_hash_descriptor sha1_desc;

int md5_init(hash_state *md);

int md5_process(hash_state *md, const unsigned char *in, unsigned long inlen);

int md5_done(hash_state *md, unsigned char *hash);

extern const struct ltc_hash_descriptor md5_desc;

int find_hash(const char *name);

int register_hash(const struct ltc_hash_descriptor *hash);

int hash_is_valid(int idx);

LTC_MUTEX_PROTO(ltc_hash_mutex)

int hash_memory(int hash, const unsigned char *in, unsigned long inlen,
                unsigned char *out, unsigned long *outlen);

/* a simple macro for making hash "process" functions */
#define HASH_PROCESS(func_name, compress_name, state_var, block_size)     \
  int func_name(hash_state *md, const unsigned char *in,                  \
                unsigned long inlen) {                                    \
    unsigned long n;                                                      \
    int err;                                                              \
    LTC_ARGCHK(md != NULL);                                               \
    LTC_ARGCHK(in != NULL);                                               \
    if (md->state_var.curlen > sizeof(md->state_var.buf)) {               \
      return CRYPT_INVALID_ARG;                                           \
    }                                                                     \
    while (inlen > 0) {                                                   \
      if (md->state_var.curlen == 0 && inlen >= block_size) {             \
        if ((err = compress_name(md, (unsigned char *)in)) != CRYPT_OK) { \
          return err;                                                     \
        }                                                                 \
        md->state_var.length += block_size * 8;                           \
        in += block_size;                                                 \
        inlen -= block_size;                                              \
      } else {                                                            \
        n = MIN(inlen, (block_size - md->state_var.curlen));              \
        memcpy(md->state_var.buf + md->state_var.curlen, in, (size_t)n);  \
        md->state_var.curlen += n;                                        \
        in += n;                                                          \
        inlen -= n;                                                       \
        if (md->state_var.curlen == block_size) {                         \
          if ((err = compress_name(md, md->state_var.buf)) != CRYPT_OK) { \
            return err;                                                   \
          }                                                               \
          md->state_var.length += 8 * block_size;                         \
          md->state_var.curlen = 0;                                       \
        }                                                                 \
      }                                                                   \
    }                                                                     \
    return CRYPT_OK;                                                      \
  }

typedef struct Hmac_state {
  hash_state md;
  int hash;
  unsigned char *key;
} hmac_state;

int hmac_init(hmac_state *hmac, int hash, const unsigned char *key,
              unsigned long keylen);

int hmac_process(hmac_state *hmac, const unsigned char *in,
                 unsigned long inlen);

int hmac_done(hmac_state *hmac, unsigned char *out, unsigned long *outlen);

void gcm_gf_mult(const unsigned char *a, const unsigned char *b,
                 unsigned char *c);

extern const unsigned char gcm_shift_table[];

#define GCM_ENCRYPT 0
#define GCM_DECRYPT 1

#define LTC_GCM_MODE_IV 0
#define LTC_GCM_MODE_AAD 1
#define LTC_GCM_MODE_TEXT 2

typedef struct {
  symmetric_key K;
  unsigned char H[16], /* multiplier */
      X[16],           /* accumulator */
      Y[16],           /* counter */
      Y_0[16],         /* initial counter */
      buf[16];         /* buffer for stuff */

  int cipher, /* which cipher */
      ivmode, /* Which mode is the IV in? */
      mode,   /* mode the GCM code is in */
      buflen; /* length of data in buf */

  ulong64 totlen, /* 64-bit counter used for IV and AAD */
      pttotlen;   /* 64-bit counter for the PT */

  unsigned char PC[16][256][16] /* 16 tables of 8x128 */
      ;
} gcm_state;

void gcm_mult_h(gcm_state *gcm, unsigned char *I);

int gcm_init(gcm_state *gcm, int cipher, const unsigned char *key, int keylen);

int gcm_reset(gcm_state *gcm);

int gcm_add_iv(gcm_state *gcm, const unsigned char *IV, unsigned long IVlen);

int gcm_add_aad(gcm_state *gcm, const unsigned char *adata,
                unsigned long adatalen);

int gcm_process(gcm_state *gcm, unsigned char *pt, unsigned long ptlen,
                unsigned char *ct, int direction);

int gcm_done(gcm_state *gcm, unsigned char *tag, unsigned long *taglen);

/* ---- PRNG Stuff ---- */

typedef void *prng_state;

/** PRNG descriptor */
extern struct ltc_prng_descriptor {
  /** Name of the PRNG */
  char *name;

  /** Start a PRNG state
      @param prng   [out] The state to initialize
      @return CRYPT_OK if successful
   */
  int (*start)(prng_state *prng);

  /** Add entropy to the PRNG
    @param in         The entropy
    @param inlen      Length of the entropy (octets)\
    @param prng       The PRNG state
    @return CRYPT_OK if successful
 */
  int (*add_entropy)(const unsigned char *in, unsigned long inlen,
                     prng_state *prng);

  /** Ready a PRNG state to read from
    @param prng       The PRNG state to ready
    @return CRYPT_OK if successful
 */
  int (*ready)(prng_state *prng);

  /** Read from the PRNG
    @param out     [out] Where to store the data
    @param outlen  Length of data desired (octets)
    @param prng    The PRNG state to read from
    @return Number of octets read
 */
  unsigned long (*read)(unsigned char *out, unsigned long outlen,
                        prng_state *prng);

} prng_descriptor[];

int sprng_start(prng_state *prng);

int sprng_add_entropy(const unsigned char *in, unsigned long inlen,
                      prng_state *prng);

int sprng_ready(prng_state *prng);

unsigned long sprng_read(unsigned char *out, unsigned long outlen,
                         prng_state *prng);

extern const struct ltc_prng_descriptor sprng_desc;

int find_prng(const char *name);

int register_prng(const struct ltc_prng_descriptor *prng);

int prng_is_valid(int idx);

LTC_MUTEX_PROTO(ltc_prng_mutex)

/* Slow RNG you **might** be able to use to seed a PRNG with.  Be careful as
 * this might not work on all platforms as planned
 */
unsigned long rng_get_bytes(unsigned char *out, unsigned long outlen,
                            void (*callback)(void));

int rng_make_prng(int bits, int wprng, prng_state *prng,
                  void (*callback)(void));

/* ---- NUMBER THEORY ---- */

enum { PK_PUBLIC = 0, PK_PRIVATE = 1 };

int rand_prime(void *N, long len, prng_state *prng, int wprng);

/* ---- RSA ---- */

/* Min and Max RSA key sizes (in bits) */
#define MIN_RSA_SIZE 1024
#define MAX_RSA_SIZE 4096

/** RSA LTC_PKCS style key */
typedef struct Rsa_key {
  /** Type of key, PK_PRIVATE or PK_PUBLIC */
  int type;
  /** The public exponent */
  void *e;
  /** The private exponent */
  void *d;
  /** The modulus */
  void *N;
  /** The p factor of N */
  void *p;
  /** The q factor of N */
  void *q;
  /** The 1/q mod p CRT param */
  void *qP;
  /** The d mod (p - 1) CRT param */
  void *dP;
  /** The d mod (q - 1) CRT param */
  void *dQ;
} rsa_key;

int rsa_make_key(prng_state *prng, int wprng, int size, long e, rsa_key *key);

int rsa_exptmod(const unsigned char *in, unsigned long inlen,
                unsigned char *out, unsigned long *outlen, int which,
                rsa_key *key);

void rsa_free(rsa_key *key);

/* These use LTC_PKCS #1 v2.0 padding */

/* These can be switched between LTC_PKCS #1 v2.x and LTC_PKCS #1 v1.5 paddings
 */
int rsa_encrypt_key_ex(const unsigned char *in, unsigned long inlen,
                       unsigned char *out, unsigned long *outlen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       prng_state *prng, int prng_idx, int hash_idx,
                       int padding, rsa_key *key);

int rsa_decrypt_key_ex(const unsigned char *in, unsigned long inlen,
                       unsigned char *out, unsigned long *outlen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       int hash_idx, int padding, int *stat, rsa_key *key);

int rsa_sign_hash_ex(const unsigned char *in, unsigned long inlen,
                     unsigned char *out, unsigned long *outlen, int padding,
                     prng_state *prng, int prng_idx, int hash_idx,
                     unsigned long saltlen, rsa_key *key);

int rsa_verify_hash_ex(const unsigned char *sig, unsigned long siglen,
                       const unsigned char *hash, unsigned long hashlen,
                       int padding, int hash_idx, unsigned long saltlen,
                       int *stat, rsa_key *key);

int rsa_import(const unsigned char *in, unsigned long inlen, rsa_key *key);

/* ---- Katja ---- */

/* ---- ECC Routines ---- */

/* size of our temp buffers for exported keys */
#define ECC_BUF_SIZE 256

/* max private key size */
#define ECC_MAXSIZE 66

/** Structure defines a NIST GF(p) curve */
typedef struct {
  /** The size of the curve in octets */
  int size;

  /** name of curve */
  char *name;

  /** The prime that defines the field the curve is in (encoded in hex) */
  char *prime;

  /** The fields B param (hex) */
  char *B;

  /** The order of the curve (hex) */
  char *order;

  /** The x co-ordinate of the base point on the curve (hex) */
  char *Gx;

  /** The y co-ordinate of the base point on the curve (hex) */
  char *Gy;
} ltc_ecc_set_type;

/** A point on a ECC curve, stored in Jacbobian format such that (x,y,z) =>
 * (x/z^2, y/z^3, 1) when interpretted as affine */
typedef struct {
  /** The x co-ordinate */
  void *x;

  /** The y co-ordinate */
  void *y;

  /** The z co-ordinate */
  void *z;
} ecc_point;

/** An ECC key */
typedef struct {
  /** Type of key, PK_PRIVATE or PK_PUBLIC */
  int type;

  /** Index into the ltc_ecc_sets[] for the parameters of this curve; if -1,
   * then this key is using user supplied curve in dp */
  int idx;

  /** pointer to domain parameters; either points to NIST curves (identified by
   * idx >= 0) or user supplied curve */
  const ltc_ecc_set_type *dp;

  /** The public key */
  ecc_point pubkey;

  /** The private key */
  void *k;
} ecc_key;

/** the ECC params provided */
extern const ltc_ecc_set_type ltc_ecc_sets[];

int ecc_make_key_ex(prng_state *prng, int wprng, ecc_key *key,
                    const ltc_ecc_set_type *dp);

void ecc_free(ecc_key *key);

int ecc_ansi_x963_export(ecc_key *key, unsigned char *out,
                         unsigned long *outlen);

int ecc_ansi_x963_import_ex(const unsigned char *in, unsigned long inlen,
                            ecc_key *key, const ltc_ecc_set_type *dp);

int ecc_shared_secret(ecc_key *private_key, ecc_key *public_key,
                      unsigned char *out, unsigned long *outlen);

int ecc_sign_hash(const unsigned char *in, unsigned long inlen,
                  unsigned char *out, unsigned long *outlen, prng_state *prng,
                  int wprng, ecc_key *key);

int ecc_verify_hash(const unsigned char *sig, unsigned long siglen,
                    const unsigned char *hash, unsigned long hashlen, int *stat,
                    ecc_key *key);

/* low level functions */
ecc_point *ltc_ecc_new_point(void);

void ltc_ecc_del_point(ecc_point *p);

int ltc_ecc_is_valid_idx(int n);

/* point ops (mp == montgomery digit) */

/* R = 2P */
int ltc_ecc_projective_dbl_point(ecc_point *P, ecc_point *R, void *modulus,
                                 void *mp);

/* R = P + Q */
int ltc_ecc_projective_add_point(ecc_point *P, ecc_point *Q, ecc_point *R,
                                 void *modulus, void *mp);

/* R = kG */
int ltc_ecc_mulmod(void *k, ecc_point *G, ecc_point *R, void *modulus, int map);

/* kA*A + kB*B = C */
int ltc_ecc_mul2add(ecc_point *A, void *kA, ecc_point *B, void *kB,
                    ecc_point *C, void *modulus);

/* map P to affine from projective */
int ltc_ecc_map(ecc_point *P, void *modulus, void *mp);

/* DER handling */

enum {
  LTC_ASN1_EOL,
  LTC_ASN1_BOOLEAN,
  LTC_ASN1_INTEGER,
  LTC_ASN1_SHORT_INTEGER,
  LTC_ASN1_BIT_STRING,
  LTC_ASN1_OCTET_STRING,
  LTC_ASN1_NULL,
  LTC_ASN1_OBJECT_IDENTIFIER,
  LTC_ASN1_IA5_STRING,
  LTC_ASN1_PRINTABLE_STRING,
  LTC_ASN1_UTF8_STRING,
  LTC_ASN1_UTCTIME,
  LTC_ASN1_CHOICE,
  LTC_ASN1_SEQUENCE,
  LTC_ASN1_SET,
  LTC_ASN1_SETOF
};

/** A LTC ASN.1 list type */
typedef struct ltc_asn1_list_ {
  /** The LTC ASN.1 enumerated type identifier */
  int type;
  /** The data to encode or place for decoding */
  void *data;
  /** The size of the input or resulting output */
  unsigned long size;
  /** The used flag, this is used by the CHOICE ASN.1 type to indicate which
   * choice was made */
  int used;
  /** prev/next entry in the list */
  struct ltc_asn1_list_ *prev, *next, *child, *parent;
} ltc_asn1_list;

#define LTC_SET_ASN1(list, index, Type, Data, Size)       \
  do {                                                    \
    int LTC_MACRO_temp = (index);                         \
    ltc_asn1_list *LTC_MACRO_list = (list);               \
    LTC_MACRO_list[LTC_MACRO_temp].type = (Type);         \
    LTC_MACRO_list[LTC_MACRO_temp].data = (void *)(Data); \
    LTC_MACRO_list[LTC_MACRO_temp].size = (Size);         \
    LTC_MACRO_list[LTC_MACRO_temp].used = 0;              \
  } while (0);

/* SEQUENCE */
int der_encode_sequence_ex(ltc_asn1_list *list, unsigned long inlen,
                           unsigned char *out, unsigned long *outlen,
                           int type_of);

#define der_encode_sequence(list, inlen, out, outlen) \
  der_encode_sequence_ex(list, inlen, out, outlen, LTC_ASN1_SEQUENCE)

int der_decode_sequence_ex(const unsigned char *in, unsigned long inlen,
                           ltc_asn1_list *list, unsigned long outlen,
                           int ordered);

#define der_decode_sequence(in, inlen, list, outlen) \
  der_decode_sequence_ex(in, inlen, list, outlen, 1)

int der_length_sequence(ltc_asn1_list *list, unsigned long inlen,
                        unsigned long *outlen);

/* SET */
#define der_decode_set(in, inlen, list, outlen) \
  der_decode_sequence_ex(in, inlen, list, outlen, 0)

int der_encode_set(ltc_asn1_list *list, unsigned long inlen, unsigned char *out,
                   unsigned long *outlen);

int der_encode_setof(ltc_asn1_list *list, unsigned long inlen,
                     unsigned char *out, unsigned long *outlen);

/* VA list handy helpers with triplets of <type, size, data> */
int der_encode_sequence_multi(unsigned char *out, unsigned long *outlen, ...);

int der_decode_sequence_multi(const unsigned char *in, unsigned long inlen,
                              ...);

/* FLEXI DECODER handle unknown list decoder */
int der_decode_sequence_flexi(const unsigned char *in, unsigned long *inlen,
                              ltc_asn1_list **out);

void der_sequence_free(ltc_asn1_list *in);

/* BOOLEAN */
int der_length_boolean(unsigned long *outlen);

int der_encode_boolean(int in, unsigned char *out, unsigned long *outlen);

int der_decode_boolean(const unsigned char *in, unsigned long inlen, int *out);

/* INTEGER */
int der_encode_integer(void *num, unsigned char *out, unsigned long *outlen);

int der_decode_integer(const unsigned char *in, unsigned long inlen, void *num);

int der_length_integer(void *num, unsigned long *len);

/* INTEGER -- handy for 0..2^32-1 values */
int der_decode_short_integer(const unsigned char *in, unsigned long inlen,
                             unsigned long *num);

int der_encode_short_integer(unsigned long num, unsigned char *out,
                             unsigned long *outlen);

int der_length_short_integer(unsigned long num, unsigned long *outlen);

/* BIT STRING */
int der_encode_bit_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen);

int der_decode_bit_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen);

int der_length_bit_string(unsigned long nbits, unsigned long *outlen);

/* OCTET STRING */
int der_encode_octet_string(const unsigned char *in, unsigned long inlen,
                            unsigned char *out, unsigned long *outlen);

int der_decode_octet_string(const unsigned char *in, unsigned long inlen,
                            unsigned char *out, unsigned long *outlen);

int der_length_octet_string(unsigned long noctets, unsigned long *outlen);

/* OBJECT IDENTIFIER */
int der_encode_object_identifier(unsigned long *words, unsigned long nwords,
                                 unsigned char *out, unsigned long *outlen);

int der_decode_object_identifier(const unsigned char *in, unsigned long inlen,
                                 unsigned long *words, unsigned long *outlen);

int der_length_object_identifier(unsigned long *words, unsigned long nwords,
                                 unsigned long *outlen);

unsigned long der_object_identifier_bits(unsigned long x);

/* IA5 STRING */
int der_encode_ia5_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen);

int der_decode_ia5_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen);

int der_length_ia5_string(const unsigned char *octets, unsigned long noctets,
                          unsigned long *outlen);

int der_ia5_char_encode(int c);

int der_ia5_value_decode(int v);

/* Printable STRING */
int der_encode_printable_string(const unsigned char *in, unsigned long inlen,
                                unsigned char *out, unsigned long *outlen);

int der_decode_printable_string(const unsigned char *in, unsigned long inlen,
                                unsigned char *out, unsigned long *outlen);

int der_length_printable_string(const unsigned char *octets,
                                unsigned long noctets, unsigned long *outlen);

int der_printable_char_encode(int c);

int der_printable_value_decode(int v);

int der_encode_utf8_string(const wchar_t *in, unsigned long inlen,
                           unsigned char *out, unsigned long *outlen);

int der_decode_utf8_string(const unsigned char *in, unsigned long inlen,
                           wchar_t *out, unsigned long *outlen);

unsigned long der_utf8_charsize(const wchar_t c);

int der_length_utf8_string(const wchar_t *in, unsigned long noctets,
                           unsigned long *outlen);

/* CHOICE */
int der_decode_choice(const unsigned char *in, unsigned long *inlen,
                      ltc_asn1_list *list, unsigned long outlen);

/* UTCTime */
typedef struct {
  unsigned YY, /* year */
      MM,      /* month */
      DD,      /* day */
      hh,      /* hour */
      mm,      /* minute */
      ss,      /* second */
      off_dir, /* timezone offset direction 0 == +, 1 == - */
      off_hh,  /* timezone offset hours */
      off_mm;  /* timezone offset minutes */
} ltc_utctime;

int der_encode_utctime(ltc_utctime *utctime, unsigned char *out,
                       unsigned long *outlen);

int der_decode_utctime(const unsigned char *in, unsigned long *inlen,
                       ltc_utctime *out);

int der_length_utctime(ltc_utctime *utctime, unsigned long *outlen);

/** math functions **/
#define LTC_MP_LT -1
#define LTC_MP_EQ 0
#define LTC_MP_GT 1

#define LTC_MP_NO 0
#define LTC_MP_YES 1

/** math descriptor */
typedef struct {
  /** Name of the math provider */
  char *name;

  /** Bits per digit, amount of bits must fit in an unsigned long */
  int bits_per_digit;

  /* ---- init/deinit functions ---- */

  /** initialize a bignum
   @param   a     The number to initialize
   @return  CRYPT_OK on success
 */
  int (*init)(void **a);

  /** init copy
   @param  dst    The number to initialize and write to
   @param  src    The number to copy from
   @return CRYPT_OK on success
 */
  int (*init_copy)(void **dst, void *src);

  /** deinit
   @param   a    The number to free
   @return CRYPT_OK on success
 */
  void (*deinit)(void *a);

  /* ---- data movement ---- */

  /** negate
   @param   src   The number to negate
   @param   dst   The destination
   @return CRYPT_OK on success
 */
  int (*neg)(void *src, void *dst);

  /** copy
   @param   src   The number to copy from
   @param   dst   The number to write to
   @return CRYPT_OK on success
 */
  int (*copy)(void *src, void *dst);

  /* ---- trivial low level functions ---- */

  /** set small constant
   @param a    Number to write to
   @param n    Source upto bits_per_digit (actually meant for very small
   constants)
   @return CRYPT_OK on succcess
 */
  int (*set_int)(void *a, unsigned long n);

  /** get small constant
   @param a    Number to read, only fetches upto bits_per_digit from the
   number
   @return  The lower bits_per_digit of the integer (unsigned)
 */
  unsigned long (*get_int)(void *a);

  /** get digit n
   @param a  The number to read from
   @param n  The number of the digit to fetch
   @return  The bits_per_digit  sized n'th digit of a
 */
  unsigned long (*get_digit)(void *a, int n);

  /** Get the number of digits that represent the number
   @param a   The number to count
   @return The number of digits used to represent the number
 */
  int (*get_digit_count)(void *a);

  /** compare two integers
   @param a   The left side integer
   @param b   The right side integer
   @return LTC_MP_LT if a < b, LTC_MP_GT if a > b and LTC_MP_EQ otherwise.
   (signed comparison)
 */
  int (*compare)(void *a, void *b);

  /** compare against int
   @param a   The left side integer
   @param b   The right side integer (upto bits_per_digit)
   @return LTC_MP_LT if a < b, LTC_MP_GT if a > b and LTC_MP_EQ otherwise.
   (signed comparison)
 */
  int (*compare_d)(void *a, unsigned long n);

  /** Count the number of bits used to represent the integer
   @param a   The integer to count
   @return The number of bits required to represent the integer
 */
  int (*count_bits)(void *a);

  /** Count the number of LSB bits which are zero
   @param a   The integer to count
   @return The number of contiguous zero LSB bits
 */
  int (*count_lsb_bits)(void *a);

  /** Compute a power of two
   @param a  The integer to store the power in
   @param n  The power of two you want to store (a = 2^n)
   @return CRYPT_OK on success
 */
  int (*twoexpt)(void *a, int n);

  /* ---- radix conversions ---- */

  /** read ascii string
   @param a     The integer to store into
   @param str   The string to read
   @param radix The radix the integer has been represented in (2-64)
   @return CRYPT_OK on success
 */
  int (*read_radix)(void *a, const char *str, int radix);

  /** write number to string
   @param a     The integer to store
   @param str   The destination for the string
   @param radix The radix the integer is to be represented in (2-64)
   @return CRYPT_OK on success
 */
  int (*write_radix)(void *a, char *str, int radix);

  /** get size as unsigned char string
   @param a     The integer to get the size (when stored in array of octets)
   @return The length of the integer
 */
  unsigned long (*unsigned_size)(void *a);

  /** store an integer as an array of octets
   @param src   The integer to store
   @param dst   The buffer to store the integer in
   @return CRYPT_OK on success
 */
  int (*unsigned_write)(void *src, unsigned char *dst);

  /** read an array of octets and store as integer
   @param dst   The integer to load
   @param src   The array of octets
   @param len   The number of octets
   @return CRYPT_OK on success
 */
  int (*unsigned_read)(void *dst, unsigned char *src, unsigned long len);

  /* ---- basic math ---- */

  /** add two integers
   @param a   The first source integer
   @param b   The second source integer
   @param c   The destination of "a + b"
   @return CRYPT_OK on success
 */
  int (*add)(void *a, void *b, void *c);

  /** add two integers
   @param a   The first source integer
   @param b   The second source integer (single digit of upto bits_per_digit
   in length)
   @param c   The destination of "a + b"
   @return CRYPT_OK on success
 */
  int (*addi)(void *a, unsigned long b, void *c);

  /** subtract two integers
   @param a   The first source integer
   @param b   The second source integer
   @param c   The destination of "a - b"
   @return CRYPT_OK on success
 */
  int (*sub)(void *a, void *b, void *c);

  /** subtract two integers
   @param a   The first source integer
   @param b   The second source integer (single digit of upto bits_per_digit
   in length)
   @param c   The destination of "a - b"
   @return CRYPT_OK on success
 */
  int (*subi)(void *a, unsigned long b, void *c);

  /** multiply two integers
   @param a   The first source integer
   @param b   The second source integer (single digit of upto bits_per_digit
   in length)
   @param c   The destination of "a * b"
   @return CRYPT_OK on success
 */
  int (*mul)(void *a, void *b, void *c);

  /** multiply two integers
   @param a   The first source integer
   @param b   The second source integer (single digit of upto bits_per_digit
   in length)
   @param c   The destination of "a * b"
   @return CRYPT_OK on success
 */
  int (*muli)(void *a, unsigned long b, void *c);

  /** Square an integer
   @param a    The integer to square
   @param b    The destination
   @return CRYPT_OK on success
 */
  int (*sqr)(void *a, void *b);

  /** Divide an integer
   @param a    The dividend
   @param b    The divisor
   @param c    The quotient (can be NULL to signify don't care)
   @param d    The remainder (can be NULL to signify don't care)
   @return CRYPT_OK on success
 */
  int (*mpdiv)(void *a, void *b, void *c, void *d);

  /** divide by two
   @param  a   The integer to divide (shift right)
   @param  b   The destination
   @return CRYPT_OK on success
 */
  int (*div_2)(void *a, void *b);

  /** Get remainder (small value)
   @param  a    The integer to reduce
   @param  b    The modulus (upto bits_per_digit in length)
   @param  c    The destination for the residue
   @return CRYPT_OK on success
 */
  int (*modi)(void *a, unsigned long b, unsigned long *c);

  /** gcd
   @param  a     The first integer
   @param  b     The second integer
   @param  c     The destination for (a, b)
   @return CRYPT_OK on success
 */
  int (*gcd)(void *a, void *b, void *c);

  /** lcm
   @param  a     The first integer
   @param  b     The second integer
   @param  c     The destination for [a, b]
   @return CRYPT_OK on success
 */
  int (*lcm)(void *a, void *b, void *c);

  /** Modular multiplication
   @param  a     The first source
   @param  b     The second source
   @param  c     The modulus
   @param  d     The destination (a*b mod c)
   @return CRYPT_OK on success
 */
  int (*mulmod)(void *a, void *b, void *c, void *d);

  /** Modular squaring
   @param  a     The first source
   @param  b     The modulus
   @param  c     The destination (a*a mod b)
   @return CRYPT_OK on success
 */
  int (*sqrmod)(void *a, void *b, void *c);

  /** Modular inversion
   @param  a     The value to invert
   @param  b     The modulus
   @param  c     The destination (1/a mod b)
   @return CRYPT_OK on success
 */
  int (*invmod)(void *, void *, void *);

  /* ---- reduction ---- */

  /** setup montgomery
    @param a  The modulus
    @param b  The destination for the reduction digit
    @return CRYPT_OK on success
 */
  int (*montgomery_setup)(void *a, void **b);

  /** get normalization value
    @param a   The destination for the normalization value
    @param b   The modulus
    @return  CRYPT_OK on success
 */
  int (*montgomery_normalization)(void *a, void *b);

  /** reduce a number
    @param a   The number [and dest] to reduce
    @param b   The modulus
    @param c   The value "b" from montgomery_setup()
    @return CRYPT_OK on success
 */
  int (*montgomery_reduce)(void *a, void *b, void *c);

  /** clean up  (frees memory)
    @param a   The value "b" from montgomery_setup()
    @return CRYPT_OK on success
 */
  void (*montgomery_deinit)(void *a);

  /* ---- exponentiation ---- */

  /** Modular exponentiation
    @param a    The base integer
    @param b    The power (can be negative) integer
    @param c    The modulus integer
    @param d    The destination
    @return CRYPT_OK on success
 */
  int (*exptmod)(void *a, void *b, void *c, void *d);

  /** Primality testing
    @param a     The integer to test
    @param b     The destination of the result (FP_YES if prime)
    @return CRYPT_OK on success
 */
  int (*isprime)(void *a, int *b);

  /* ----  (optional) ecc point math ---- */

  /** ECC GF(p) point multiplication (from the NIST curves)
    @param k   The integer to multiply the point by
    @param G   The point to multiply
    @param R   The destination for kG
    @param modulus  The modulus for the field
    @param map Boolean indicated whether to map back to affine or not (can be
   ignored if you work in affine only)
    @return CRYPT_OK on success
 */
  int (*ecc_ptmul)(void *k, ecc_point *G, ecc_point *R, void *modulus, int map);

  /** ECC GF(p) point addition
    @param P    The first point
    @param Q    The second point
    @param R    The destination of P + Q
    @param modulus  The modulus
    @param mp   The "b" value from montgomery_setup()
    @return CRYPT_OK on success
 */
  int (*ecc_ptadd)(ecc_point *P, ecc_point *Q, ecc_point *R, void *modulus,
                   void *mp);

  /** ECC GF(p) point double
    @param P    The first point
    @param R    The destination of 2P
    @param modulus  The modulus
    @param mp   The "b" value from montgomery_setup()
    @return CRYPT_OK on success
 */
  int (*ecc_ptdbl)(ecc_point *P, ecc_point *R, void *modulus, void *mp);

  /** ECC mapping from projective to affine, currently uses (x,y,z) => (x/z^2,
   y/z^3, 1)
    @param P     The point to map
    @param modulus The modulus
    @param mp    The "b" value from montgomery_setup()
    @return CRYPT_OK on success
    @remark  The mapping can be different but keep in mind a ecc_point only
   has three integers (x,y,z) so if you use a different mapping you have to
   make it fit.
 */
  int (*ecc_map)(ecc_point *P, void *modulus, void *mp);

  /** Computes kA*A + kB*B = C using Shamir's Trick
    @param A        First point to multiply
    @param kA       What to multiple A by
    @param B        Second point to multiply
    @param kB       What to multiple B by
    @param C        [out] Destination point (can overlap with A or B
    @param modulus  Modulus for curve
    @return CRYPT_OK on success
 */
  int (*ecc_mul2add)(ecc_point *A, void *kA, ecc_point *B, void *kB,
                     ecc_point *C, void *modulus);

  /* ---- (optional) rsa optimized math (for internal CRT) ---- */

  /** RSA Key Generation
    @param prng     An active PRNG state
    @param wprng    The index of the PRNG desired
    @param size     The size of the modulus (key size) desired (octets)
    @param e        The "e" value (public key).  e==65537 is a good choice
    @param key      [out] Destination of a newly created private key pair
    @return CRYPT_OK if successful, upon error all allocated ram is freed
 */
  int (*rsa_keygen)(prng_state *prng, int wprng, int size, long e,
                    rsa_key *key);

  /** RSA exponentiation
   @param in       The octet array representing the base
   @param inlen    The length of the input
   @param out      The destination (to be stored in an octet array format)
   @param outlen   The length of the output buffer and the resulting size
   (zero padded to the size of the modulus)
   @param which    PK_PUBLIC for public RSA and PK_PRIVATE for private RSA
   @param key      The RSA key to use
   @return CRYPT_OK on success
 */
  int (*rsa_me)(const unsigned char *in, unsigned long inlen,
                unsigned char *out, unsigned long *outlen, int which,
                rsa_key *key);
} ltc_math_descriptor;

extern ltc_math_descriptor ltc_mp;

int ltc_init_multi(void **a, ...);

void ltc_deinit_multi(void *a, ...);

extern const ltc_math_descriptor ltm_desc;

#undef MP_DIGIT_BIT
#undef mp_iszero
#undef mp_isodd
#undef mp_tohex

#define MP_DIGIT_BIT ltc_mp.bits_per_digit

/* some handy macros */
#define mp_init(a) ltc_mp.init(a)
#define mp_init_multi ltc_init_multi
#define mp_clear(a) ltc_mp.deinit(a)
#define mp_clear_multi ltc_deinit_multi
#define mp_init_copy(a, b) ltc_mp.init_copy(a, b)

#define mp_neg(a, b) ltc_mp.neg(a, b)
#define mp_copy(a, b) ltc_mp.copy(a, b)

#define mp_set(a, b) ltc_mp.set_int(a, b)
#define mp_set_int(a, b) ltc_mp.set_int(a, b)
#define mp_get_int(a) ltc_mp.get_int(a)
#define mp_get_digit(a, n) ltc_mp.get_digit(a, n)
#define mp_get_digit_count(a) ltc_mp.get_digit_count(a)
#define mp_cmp(a, b) ltc_mp.compare(a, b)
#define mp_cmp_d(a, b) ltc_mp.compare_d(a, b)
#define mp_count_bits(a) ltc_mp.count_bits(a)
#define mp_cnt_lsb(a) ltc_mp.count_lsb_bits(a)
#define mp_2expt(a, b) ltc_mp.twoexpt(a, b)

#define mp_read_radix(a, b, c) ltc_mp.read_radix(a, b, c)
#define mp_toradix(a, b, c) ltc_mp.write_radix(a, b, c)
#define mp_unsigned_bin_size(a) ltc_mp.unsigned_size(a)
#define mp_to_unsigned_bin(a, b) ltc_mp.unsigned_write(a, b)
#define mp_read_unsigned_bin(a, b, c) ltc_mp.unsigned_read(a, b, c)

#define mp_add(a, b, c) ltc_mp.add(a, b, c)
#define mp_add_d(a, b, c) ltc_mp.addi(a, b, c)
#define mp_sub(a, b, c) ltc_mp.sub(a, b, c)
#define mp_sub_d(a, b, c) ltc_mp.subi(a, b, c)
#define mp_mul(a, b, c) ltc_mp.mul(a, b, c)
#define mp_mul_d(a, b, c) ltc_mp.muli(a, b, c)
#define mp_sqr(a, b) ltc_mp.sqr(a, b)
#define mp_div(a, b, c, d) ltc_mp.mpdiv(a, b, c, d)
#define mp_div_2(a, b) ltc_mp.div_2(a, b)
#define mp_mod(a, b, c) ltc_mp.mpdiv(a, b, NULL, c)
#define mp_mod_d(a, b, c) ltc_mp.modi(a, b, c)
#define mp_gcd(a, b, c) ltc_mp.gcd(a, b, c)
#define mp_lcm(a, b, c) ltc_mp.lcm(a, b, c)

#define mp_mulmod(a, b, c, d) ltc_mp.mulmod(a, b, c, d)
#define mp_sqrmod(a, b, c) ltc_mp.sqrmod(a, b, c)
#define mp_invmod(a, b, c) ltc_mp.invmod(a, b, c)

#define mp_montgomery_setup(a, b) ltc_mp.montgomery_setup(a, b)
#define mp_montgomery_normalization(a, b) ltc_mp.montgomery_normalization(a, b)
#define mp_montgomery_reduce(a, b, c) ltc_mp.montgomery_reduce(a, b, c)
#define mp_montgomery_free(a) ltc_mp.montgomery_deinit(a)

#define mp_exptmod(a, b, c, d) ltc_mp.exptmod(a, b, c, d)
#define mp_prime_is_prime(a, b, c) ltc_mp.isprime(a, c)

#define mp_iszero(a) (mp_cmp_d(a, 0) == LTC_MP_EQ ? LTC_MP_YES : LTC_MP_NO)
#define mp_isodd(a)                                        \
  (mp_get_digit_count(a) > 0                               \
       ? (mp_get_digit(a, 0) & 1 ? LTC_MP_YES : LTC_MP_NO) \
       : LTC_MP_NO)
#define mp_exch(a, b)   \
  do {                  \
    void *ABC__tmp = a; \
    a = b;              \
    b = ABC__tmp;       \
  } while (0);

#define mp_tohex(a, b) mp_toradix(a, b, 16)

/* ---- LTC_BASE64 Routines ---- */

/* ---- MEM routines ---- */
void zeromem(void *dst, size_t len);

/* Defines the LTC_ARGCHK macro used within the library */
/* ARGTYPE is defined in mycrypt_cfg.h */

/* this is the default LibTomCrypt macro  */
void crypt_argchk(char *v, char *s, int d);

#define LTC_ARGCHK(x)                     \
  if (!(x)) {                             \
    crypt_argchk(#x, __FILE__, __LINE__); \
  }
#define LTC_ARGCHKVD(x) LTC_ARGCHK(x)

/* LTC_PKCS Header Info */

/* ===> LTC_PKCS #1 -- RSA Cryptography <=== */

enum ltc_pkcs_1_v1_5_blocks {
  LTC_LTC_PKCS_1_EMSA =
      1, /* Block type 1 (LTC_PKCS #1 v1.5 signature padding) */
  LTC_LTC_PKCS_1_EME =
      2 /* Block type 2 (LTC_PKCS #1 v1.5 encryption padding) */
};

enum ltc_pkcs_1_paddings {
  LTC_LTC_PKCS_1_V1_5 =
      1, /* LTC_PKCS #1 v1.5 padding (\sa ltc_pkcs_1_v1_5_blocks) */
  LTC_LTC_PKCS_1_OAEP = 2, /* LTC_PKCS #1 v2.0 encryption padding */
  LTC_LTC_PKCS_1_PSS = 3   /* LTC_PKCS #1 v2.1 signature padding */
};

int pkcs_1_mgf1(int hash_idx, const unsigned char *seed, unsigned long seedlen,
                unsigned char *mask, unsigned long masklen);

/* *** v1.5 padding */
int pkcs_1_v1_5_encode(const unsigned char *msg, unsigned long msglen,
                       int block_type, unsigned long modulus_bitlen,
                       prng_state *prng, int prng_idx, unsigned char *out,
                       unsigned long *outlen);

int pkcs_1_v1_5_decode(const unsigned char *msg, unsigned long msglen,
                       int block_type, unsigned long modulus_bitlen,
                       unsigned char *out, unsigned long *outlen,
                       int *is_valid);

/* *** v2.1 padding */
int pkcs_1_oaep_encode(const unsigned char *msg, unsigned long msglen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       unsigned long modulus_bitlen, prng_state *prng,
                       int prng_idx, int hash_idx, unsigned char *out,
                       unsigned long *outlen);

int pkcs_1_oaep_decode(const unsigned char *msg, unsigned long msglen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       unsigned long modulus_bitlen, int hash_idx,
                       unsigned char *out, unsigned long *outlen, int *res);

int pkcs_1_pss_encode(const unsigned char *msghash, unsigned long msghashlen,
                      unsigned long saltlen, prng_state *prng, int prng_idx,
                      int hash_idx, unsigned long modulus_bitlen,
                      unsigned char *out, unsigned long *outlen);

int pkcs_1_pss_decode(const unsigned char *msghash, unsigned long msghashlen,
                      const unsigned char *sig, unsigned long siglen,
                      unsigned long saltlen, int hash_idx,
                      unsigned long modulus_bitlen, int *res);

/**
   @file crypt_cipher_descriptor.c
   Stores the cipher descriptor table, Tom St Denis
 */

struct ltc_cipher_descriptor cipher_descriptor[TAB_SIZE] = {{
    NULL,
    0,
    0,
    NULL,
    NULL,
    NULL,
}};

LTC_MUTEX_GLOBAL(ltc_cipher_mutex)

/*
   Test if a cipher index is valid
   @param idx   The index of the cipher to search for
   @return CRYPT_OK if valid
 */
int cipher_is_valid(int idx) {
  LTC_MUTEX_LOCK(&ltc_cipher_mutex);
  if ((idx < 0) || (idx >= TAB_SIZE) || (cipher_descriptor[idx].name == NULL)) {
    LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
    return CRYPT_INVALID_CIPHER;
  }
  LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
  return CRYPT_OK;
}

/**
   Find a registered cipher by name
   @param name   The name of the cipher to look for
   @return >= 0 if found, -1 if not present
 */
int find_cipher(const char *name) {
  int x;

  LTC_ARGCHK(name != NULL);
  LTC_MUTEX_LOCK(&ltc_cipher_mutex);
  for (x = 0; x < TAB_SIZE; x++) {
    if ((cipher_descriptor[x].name != NULL) &&
        !XSTRCMP(cipher_descriptor[x].name, name)) {
      LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
      return x;
    }
  }
  LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
  return -1;
}

/**
   Find a registered hash by name
   @param name   The name of the hash to look for
   @return >= 0 if found, -1 if not present
 */
int find_hash(const char *name) {
  int x;

  LTC_ARGCHK(name != NULL);
  LTC_MUTEX_LOCK(&ltc_hash_mutex);
  for (x = 0; x < TAB_SIZE; x++) {
    if ((hash_descriptor[x].name != NULL) &&
        (XSTRCMP(hash_descriptor[x].name, name) == 0)) {
      LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
      return x;
    }
  }
  LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
  return -1;
}

/**
   Find a registered PRNG by name
   @param name   The name of the PRNG to look for
   @return >= 0 if found, -1 if not present
 */
int find_prng(const char *name) {
  int x;

  LTC_ARGCHK(name != NULL);
  LTC_MUTEX_LOCK(&ltc_prng_mutex);
  for (x = 0; x < TAB_SIZE; x++) {
    if ((prng_descriptor[x].name != NULL) &&
        (XSTRCMP(prng_descriptor[x].name, name) == 0)) {
      LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
      return x;
    }
  }
  LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
  return -1;
}

/**
   @file crypt_hash_descriptor.c
   Stores the hash descriptor table, Tom St Denis
 */

struct ltc_hash_descriptor hash_descriptor[TAB_SIZE] = {{
    NULL,
    0,
    0,
    {0},
    0,
    NULL,
    NULL,
    NULL,
}};

LTC_MUTEX_GLOBAL(ltc_hash_mutex)

/*
   Test if a hash index is valid
   @param idx   The index of the hash to search for
   @return CRYPT_OK if valid
 */
int hash_is_valid(int idx) {
  LTC_MUTEX_LOCK(&ltc_hash_mutex);
  if ((idx < 0) || (idx >= TAB_SIZE) || (hash_descriptor[idx].name == NULL)) {
    LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
    return CRYPT_INVALID_HASH;
  }
  LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
  return CRYPT_OK;
}

ltc_math_descriptor ltc_mp;

/**
   @file crypt_prng_descriptor.c
   Stores the PRNG descriptors, Tom St Denis
 */
struct ltc_prng_descriptor prng_descriptor[TAB_SIZE] = {{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

}};

LTC_MUTEX_GLOBAL(ltc_prng_mutex)

/*
   Test if a PRNG index is valid
   @param idx   The index of the PRNG to search for
   @return CRYPT_OK if valid
 */
int prng_is_valid(int idx) {
  LTC_MUTEX_LOCK(&ltc_prng_mutex);
  if ((idx < 0) || (idx >= TAB_SIZE) || (prng_descriptor[idx].name == NULL)) {
    LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
    return CRYPT_INVALID_PRNG;
  }
  LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
  return CRYPT_OK;
}

/**
   Register a cipher with the descriptor table
   @param cipher   The cipher you wish to register
   @return value >= 0 if successfully added (or already present), -1 if
   unsuccessful
 */
int register_cipher(const struct ltc_cipher_descriptor *cipher) {
  int x;

  LTC_ARGCHK(cipher != NULL);

  /* is it already registered? */
  LTC_MUTEX_LOCK(&ltc_cipher_mutex);
  for (x = 0; x < TAB_SIZE; x++) {
    if ((cipher_descriptor[x].name != NULL) &&
        (cipher_descriptor[x].ID == cipher->ID)) {
      LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
      return x;
    }
  }

  /* find a blank spot */
  for (x = 0; x < TAB_SIZE; x++) {
    if (cipher_descriptor[x].name == NULL) {
      XMEMCPY(&cipher_descriptor[x], cipher,
              sizeof(struct ltc_cipher_descriptor));
      LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
      return x;
    }
  }

  /* no spot */
  LTC_MUTEX_UNLOCK(&ltc_cipher_mutex);
  return -1;
}

/**
   Register a hash with the descriptor table
   @param hash   The hash you wish to register
   @return value >= 0 if successfully added (or already present), -1 if
   unsuccessful
 */
int register_hash(const struct ltc_hash_descriptor *hash) {
  int x;

  LTC_ARGCHK(hash != NULL);

  /* is it already registered? */
  LTC_MUTEX_LOCK(&ltc_hash_mutex);
  for (x = 0; x < TAB_SIZE; x++) {
    if (XMEMCMP(&hash_descriptor[x], hash,
                sizeof(struct ltc_hash_descriptor)) == 0) {
      LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
      return x;
    }
  }

  /* find a blank spot */
  for (x = 0; x < TAB_SIZE; x++) {
    if (hash_descriptor[x].name == NULL) {
      XMEMCPY(&hash_descriptor[x], hash, sizeof(struct ltc_hash_descriptor));
      LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
      return x;
    }
  }

  /* no spot */
  LTC_MUTEX_UNLOCK(&ltc_hash_mutex);
  return -1;
}

/**
   Register a PRNG with the descriptor table
   @param prng   The PRNG you wish to register
   @return value >= 0 if successfully added (or already present), -1 if
   unsuccessful
 */
int register_prng(const struct ltc_prng_descriptor *prng) {
  int x;

  LTC_ARGCHK(prng != NULL);

  /* is it already registered? */
  LTC_MUTEX_LOCK(&ltc_prng_mutex);
  for (x = 0; x < TAB_SIZE; x++) {
    if (XMEMCMP(&prng_descriptor[x], prng,
                sizeof(struct ltc_prng_descriptor)) == 0) {
      LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
      return x;
    }
  }

  /* find a blank spot */
  for (x = 0; x < TAB_SIZE; x++) {
    if (prng_descriptor[x].name == NULL) {
      XMEMCPY(&prng_descriptor[x], prng, sizeof(struct ltc_prng_descriptor));
      LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
      return x;
    }
  }

  /* no spot */
  LTC_MUTEX_UNLOCK(&ltc_prng_mutex);
  return -1;
}

/**
   Store a BIT STRING
   @param in      The DER encoded BIT STRING
   @param inlen   The size of the DER BIT STRING
   @param out     [out] The array of bits stored (one per char)
   @param outlen  [in/out] The number of bits stored
   @return CRYPT_OK if successful
 */
int der_decode_bit_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen) {
  unsigned long dlen, blen, x, y;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* packet must be at least 4 bytes */
  if (inlen < 4) {
    return CRYPT_INVALID_ARG;
  }

  /* check for 0x03 */
  if ((in[0] & 0x1F) != 0x03) {
    return CRYPT_INVALID_PACKET;
  }

  /* offset in the data */
  x = 1;

  /* get the length of the data */
  if (in[x] & 0x80) {
    /* long format get number of length bytes */
    y = in[x++] & 0x7F;

    /* invalid if 0 or > 2 */
    if ((y == 0) || (y > 2)) {
      return CRYPT_INVALID_PACKET;
    }

    /* read the data len */
    dlen = 0;
    while (y--) {
      dlen = (dlen << 8) | (unsigned long)in[x++];
    }
  } else {
    /* short format */
    dlen = in[x++] & 0x7F;
  }

  /* is the data len too long or too short? */
  if ((dlen == 0) || (dlen + x > inlen)) {
    return CRYPT_INVALID_PACKET;
  }

  /* get padding count */
  blen = ((dlen - 1) << 3) - (in[x++] & 7);

  /* too many bits? */
  if (blen > *outlen) {
    *outlen = blen;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* decode/store the bits */
  for (y = 0; y < blen; y++) {
    out[y] = (in[x] & (1 << (7 - (y & 7)))) ? 1 : 0;
    if ((y & 7) == 7) {
      ++x;
    }
  }

  /* we done */
  *outlen = blen;
  return CRYPT_OK;
}

/**
   Read a BOOLEAN
   @param in      The destination for the DER encoded BOOLEAN
   @param inlen   The size of the DER BOOLEAN
   @param out     [out]  The boolean to decode
   @return CRYPT_OK if successful
 */
int der_decode_boolean(const unsigned char *in, unsigned long inlen, int *out) {
  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);

  if ((inlen != 3) || (in[0] != 0x01) || (in[1] != 0x01) ||
      ((in[2] != 0x00) && (in[2] != 0xFF))) {
    return CRYPT_INVALID_ARG;
  }

  *out = (in[2] == 0xFF) ? 1 : 0;

  return CRYPT_OK;
}

/**
   Decode a CHOICE
   @param in       The DER encoded input
   @param inlen    [in/out] The size of the input and resulting size of read
   type
   @param list     The list of items to decode
   @param outlen   The number of items in the list
   @return CRYPT_OK on success
 */
int der_decode_choice(const unsigned char *in, unsigned long *inlen,
                      ltc_asn1_list *list, unsigned long outlen) {
  unsigned long size, x, z;
  void *data;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(inlen != NULL);
  LTC_ARGCHK(list != NULL);

  /* get blk size */
  if (*inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* set all of the "used" flags to zero */
  for (x = 0; x < outlen; x++) {
    list[x].used = 0;
  }

  /* now scan until we have a winner */
  for (x = 0; x < outlen; x++) {
    size = list[x].size;
    data = list[x].data;

    switch (list[x].type) {
      case LTC_ASN1_INTEGER:
        if (der_decode_integer(in, *inlen, data) == CRYPT_OK) {
          if (der_length_integer(data, &z) == CRYPT_OK) {
            list[x].used = 1;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_SHORT_INTEGER:
        if (der_decode_short_integer(in, *inlen, data) == CRYPT_OK) {
          if (der_length_short_integer(size, &z) == CRYPT_OK) {
            list[x].used = 1;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_BIT_STRING:
        if (der_decode_bit_string(in, *inlen, data, &size) == CRYPT_OK) {
          if (der_length_bit_string(size, &z) == CRYPT_OK) {
            list[x].used = 1;
            list[x].size = size;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_OCTET_STRING:
        if (der_decode_octet_string(in, *inlen, data, &size) == CRYPT_OK) {
          if (der_length_octet_string(size, &z) == CRYPT_OK) {
            list[x].used = 1;
            list[x].size = size;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_NULL:
        if ((*inlen == 2) && (in[x] == 0x05) && (in[x + 1] == 0x00)) {
          *inlen = 2;
          list[x].used = 1;
          return CRYPT_OK;
        }
        break;

      case LTC_ASN1_OBJECT_IDENTIFIER:
        if (der_decode_object_identifier(in, *inlen, data, &size) == CRYPT_OK) {
          if (der_length_object_identifier(data, size, &z) == CRYPT_OK) {
            list[x].used = 1;
            list[x].size = size;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_IA5_STRING:
        if (der_decode_ia5_string(in, *inlen, data, &size) == CRYPT_OK) {
          if (der_length_ia5_string(data, size, &z) == CRYPT_OK) {
            list[x].used = 1;
            list[x].size = size;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_PRINTABLE_STRING:
        if (der_decode_printable_string(in, *inlen, data, &size) == CRYPT_OK) {
          if (der_length_printable_string(data, size, &z) == CRYPT_OK) {
            list[x].used = 1;
            list[x].size = size;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_UTF8_STRING:
        if (der_decode_utf8_string(in, *inlen, data, &size) == CRYPT_OK) {
          if (der_length_utf8_string(data, size, &z) == CRYPT_OK) {
            list[x].used = 1;
            list[x].size = size;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      case LTC_ASN1_UTCTIME:
        z = *inlen;
        if (der_decode_utctime(in, &z, data) == CRYPT_OK) {
          list[x].used = 1;
          *inlen = z;
          return CRYPT_OK;
        }
        break;

      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
      case LTC_ASN1_SEQUENCE:
        if (der_decode_sequence(in, *inlen, data, size) == CRYPT_OK) {
          if (der_length_sequence(data, size, &z) == CRYPT_OK) {
            list[x].used = 1;
            *inlen = z;
            return CRYPT_OK;
          }
        }
        break;

      default:
        return CRYPT_INVALID_ARG;
    }
  }

  return CRYPT_INVALID_PACKET;
}

/**
   Store a IA5 STRING
   @param in      The DER encoded IA5 STRING
   @param inlen   The size of the DER IA5 STRING
   @param out     [out] The array of octets stored (one per char)
   @param outlen  [in/out] The number of octets stored
   @return CRYPT_OK if successful
 */
int der_decode_ia5_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;
  int t;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* must have header at least */
  if (inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* check for 0x16 */
  if ((in[0] & 0x1F) != 0x16) {
    return CRYPT_INVALID_PACKET;
  }
  x = 1;

  /* decode the length */
  if (in[x] & 0x80) {
    /* valid # of bytes in length are 1,2,3 */
    y = in[x] & 0x7F;
    if ((y == 0) || (y > 3) || ((x + y) > inlen)) {
      return CRYPT_INVALID_PACKET;
    }

    /* read the length in */
    len = 0;
    ++x;
    while (y--) {
      len = (len << 8) | in[x++];
    }
  } else {
    len = in[x++] & 0x7F;
  }

  /* is it too long? */
  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  if (len + x > inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* read the data */
  for (y = 0; y < len; y++) {
    t = der_ia5_value_decode(in[x++]);
    if (t == -1) {
      return CRYPT_INVALID_ARG;
    }
    out[y] = t;
  }

  *outlen = y;

  return CRYPT_OK;
}

/**
   Read a mp_int integer
   @param in       The DER encoded data
   @param inlen    Size of DER encoded data
   @param num      The first mp_int to decode
   @return CRYPT_OK if successful
 */
int der_decode_integer(const unsigned char *in, unsigned long inlen,
                       void *num) {
  unsigned long x, y, z;
  int err;

  LTC_ARGCHK(num != NULL);
  LTC_ARGCHK(in != NULL);

  /* min DER INTEGER is 0x02 01 00 == 0 */
  if (inlen < (1 + 1 + 1)) {
    return CRYPT_INVALID_PACKET;
  }

  /* ok expect 0x02 when we AND with 0001 1111 [1F] */
  x = 0;
  if ((in[x++] & 0x1F) != 0x02) {
    return CRYPT_INVALID_PACKET;
  }

  /* now decode the len stuff */
  z = in[x++];

  if ((z & 0x80) == 0x00) {
    /* short form */

    /* will it overflow? */
    if (x + z > inlen) {
      return CRYPT_INVALID_PACKET;
    }

    /* no so read it */
    if ((err = mp_read_unsigned_bin(num, (unsigned char *)in + x, z)) !=
        CRYPT_OK) {
      return err;
    }
  } else {
    /* long form */
    z &= 0x7F;

    /* will number of length bytes overflow? (or > 4) */
    if (((x + z) > inlen) || (z > 4) || (z == 0)) {
      return CRYPT_INVALID_PACKET;
    }

    /* now read it in */
    y = 0;
    while (z--) {
      y = ((unsigned long)(in[x++])) | (y << 8);
    }

    /* now will reading y bytes overrun? */
    if ((x + y) > inlen) {
      return CRYPT_INVALID_PACKET;
    }

    /* no so read it */
    if ((err = mp_read_unsigned_bin(num, (unsigned char *)in + x, y)) !=
        CRYPT_OK) {
      return err;
    }
  }

  /* see if it's negative */
  if (in[x] & 0x80) {
    void *tmp;
    if (mp_init(&tmp) != CRYPT_OK) {
      return CRYPT_MEM;
    }

    if ((mp_2expt(tmp, mp_count_bits(num)) != CRYPT_OK) ||
        (mp_sub(num, tmp, num) != CRYPT_OK)) {
      mp_clear(tmp);
      return CRYPT_MEM;
    }
    mp_clear(tmp);
  }

  return CRYPT_OK;
}

/**
   Decode OID data and store the array of integers in words
   @param in      The OID DER encoded data
   @param inlen   The length of the OID data
   @param words   [out] The destination of the OID words
   @param outlen  [in/out] The number of OID words
   @return CRYPT_OK if successful
 */
int der_decode_object_identifier(const unsigned char *in, unsigned long inlen,
                                 unsigned long *words, unsigned long *outlen) {
  unsigned long x, y, t, len;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(words != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* header is at least 3 bytes */
  if (inlen < 3) {
    return CRYPT_INVALID_PACKET;
  }

  /* must be room for at least two words */
  if (*outlen < 2) {
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* decode the packet header */
  x = 0;
  if ((in[x++] & 0x1F) != 0x06) {
    return CRYPT_INVALID_PACKET;
  }

  /* get the length */
  if (in[x] < 128) {
    len = in[x++];
  } else {
    if ((in[x] < 0x81) || (in[x] > 0x82)) {
      return CRYPT_INVALID_PACKET;
    }
    y = in[x++] & 0x7F;
    len = 0;
    while (y--) {
      len = (len << 8) | (unsigned long)in[x++];
    }
  }

  if ((len < 1) || ((len + x) > inlen)) {
    return CRYPT_INVALID_PACKET;
  }

  /* decode words */
  y = 0;
  t = 0;
  while (len--) {
    t = (t << 7) | (in[x] & 0x7F);
    if (!(in[x++] & 0x80)) {
      /* store t */
      if (y >= *outlen) {
        return CRYPT_BUFFER_OVERFLOW;
      }
      if (y == 0) {
        words[0] = t / 40;
        words[1] = t % 40;
        y = 2;
      } else {
        words[y++] = t;
      }
      t = 0;
    }
  }

  *outlen = y;
  return CRYPT_OK;
}

/**
   Store a OCTET STRING
   @param in      The DER encoded OCTET STRING
   @param inlen   The size of the DER OCTET STRING
   @param out     [out] The array of octets stored (one per char)
   @param outlen  [in/out] The number of octets stored
   @return CRYPT_OK if successful
 */
int der_decode_octet_string(const unsigned char *in, unsigned long inlen,
                            unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* must have header at least */
  if (inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* check for 0x04 */
  if ((in[0] & 0x1F) != 0x04) {
    return CRYPT_INVALID_PACKET;
  }
  x = 1;

  /* decode the length */
  if (in[x] & 0x80) {
    /* valid # of bytes in length are 1,2,3 */
    y = in[x] & 0x7F;
    if ((y == 0) || (y > 3) || ((x + y) > inlen)) {
      return CRYPT_INVALID_PACKET;
    }

    /* read the length in */
    len = 0;
    ++x;
    while (y--) {
      len = (len << 8) | in[x++];
    }
  } else {
    len = in[x++] & 0x7F;
  }

  /* is it too long? */
  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  if (len + x > inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* read the data */
  for (y = 0; y < len; y++) {
    out[y] = in[x++];
  }

  *outlen = y;

  return CRYPT_OK;
}

/**
   Store a printable STRING
   @param in      The DER encoded printable STRING
   @param inlen   The size of the DER printable STRING
   @param out     [out] The array of octets stored (one per char)
   @param outlen  [in/out] The number of octets stored
   @return CRYPT_OK if successful
 */
int der_decode_printable_string(const unsigned char *in, unsigned long inlen,
                                unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;
  int t;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* must have header at least */
  if (inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* check for 0x13 */
  if ((in[0] & 0x1F) != 0x13) {
    return CRYPT_INVALID_PACKET;
  }
  x = 1;

  /* decode the length */
  if (in[x] & 0x80) {
    /* valid # of bytes in length are 1,2,3 */
    y = in[x] & 0x7F;
    if ((y == 0) || (y > 3) || ((x + y) > inlen)) {
      return CRYPT_INVALID_PACKET;
    }

    /* read the length in */
    len = 0;
    ++x;
    while (y--) {
      len = (len << 8) | in[x++];
    }
  } else {
    len = in[x++] & 0x7F;
  }

  /* is it too long? */
  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  if (len + x > inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* read the data */
  for (y = 0; y < len; y++) {
    t = der_printable_value_decode(in[x++]);
    if (t == -1) {
      return CRYPT_INVALID_ARG;
    }
    out[y] = t;
  }

  *outlen = y;

  return CRYPT_OK;
}

/**
   Decode a SEQUENCE
   @param in       The DER encoded input
   @param inlen    The size of the input
   @param list     The list of items to decode
   @param outlen   The number of items in the list
   @param ordered  Search an unordeded or ordered list
   @return CRYPT_OK on success
 */
int der_decode_sequence_ex(const unsigned char *in, unsigned long inlen,
                           ltc_asn1_list *list, unsigned long outlen,
                           int ordered) {
  int err, type;
  unsigned long size, x, y, z, i, blksize;
  blksize = 0;
  void *data;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(list != NULL);

  /* get blk size */
  if (inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* sequence type? We allow 0x30 SEQUENCE and 0x31 SET since fundamentally
   * they're the same structure */
  x = 0;
  if ((in[x] != 0x30) && (in[x] != 0x31)) {
    return CRYPT_INVALID_PACKET;
  }
  ++x;

  if (in[x] < 128) {
    blksize = in[x++];
  } else if (in[x] & 0x80) {
    if ((in[x] < 0x81) || (in[x] > 0x83)) {
      return CRYPT_INVALID_PACKET;
    }
    y = in[x++] & 0x7F;

    /* would reading the len bytes overrun? */
    if (x + y > inlen) {
      return CRYPT_INVALID_PACKET;
    }

    /* read len */
    blksize = 0;
    while (y--) {
      blksize = (blksize << 8) | (unsigned long)in[x++];
    }
  }

  /* would this blksize overflow? */
  if (x + blksize > inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* mark all as unused */
  for (i = 0; i < outlen; i++) {
    list[i].used = 0;
  }

  /* ok read data */
  inlen = blksize;
  for (i = 0; i < outlen; i++) {
    z = 0;
    type = list[i].type;
    size = list[i].size;
    data = list[i].data;
    if (!ordered && (list[i].used == 1)) {
      continue;
    }

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
        z = inlen;
        if ((err = der_decode_boolean(in + x, z, ((int *)data))) != CRYPT_OK) {
          goto LBL_ERR;
        }
        if ((err = der_length_boolean(&z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_INTEGER:
        z = inlen;
        if ((err = der_decode_integer(in + x, z, data)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        if ((err = der_length_integer(data, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_SHORT_INTEGER:
        z = inlen;
        if ((err = der_decode_short_integer(in + x, z, data)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        if ((err = der_length_short_integer(((unsigned long *)data)[0], &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }

        break;

      case LTC_ASN1_BIT_STRING:
        z = inlen;
        if ((err = der_decode_bit_string(in + x, z, data, &size)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        list[i].size = size;
        if ((err = der_length_bit_string(size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_OCTET_STRING:
        z = inlen;
        if ((err = der_decode_octet_string(in + x, z, data, &size)) !=
            CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        list[i].size = size;
        if ((err = der_length_octet_string(size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_NULL:
        if ((inlen < 2) || (in[x] != 0x05) || (in[x + 1] != 0x00)) {
          if (!ordered) {
            continue;
          }
          err = CRYPT_INVALID_PACKET;
          goto LBL_ERR;
        }
        z = 2;
        break;

      case LTC_ASN1_OBJECT_IDENTIFIER:
        z = inlen;
        if ((err = der_decode_object_identifier(in + x, z, data, &size)) !=
            CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        list[i].size = size;
        if ((err = der_length_object_identifier(data, size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_IA5_STRING:
        z = inlen;
        if ((err = der_decode_ia5_string(in + x, z, data, &size)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        list[i].size = size;
        if ((err = der_length_ia5_string(data, size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_PRINTABLE_STRING:
        z = inlen;
        if ((err = der_decode_printable_string(in + x, z, data, &size)) !=
            CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        list[i].size = size;
        if ((err = der_length_printable_string(data, size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_UTF8_STRING:
        z = inlen;
        if ((err = der_decode_utf8_string(in + x, z, data, &size)) !=
            CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        list[i].size = size;
        if ((err = der_length_utf8_string(data, size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_UTCTIME:
        z = inlen;
        if ((err = der_decode_utctime(in + x, &z, data)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_SET:
        z = inlen;
        if ((err = der_decode_set(in + x, z, data, size)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        if ((err = der_length_sequence(data, size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_SETOF:
      case LTC_ASN1_SEQUENCE:
        /* detect if we have the right type */
        if (((type == LTC_ASN1_SETOF) && ((in[x] & 0x3F) != 0x31)) ||
            ((type == LTC_ASN1_SEQUENCE) && ((in[x] & 0x3F) != 0x30))) {
          err = CRYPT_INVALID_PACKET;
          goto LBL_ERR;
        }

        z = inlen;
        if ((err = der_decode_sequence(in + x, z, data, size)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        if ((err = der_length_sequence(data, size, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        break;

      case LTC_ASN1_CHOICE:
        z = inlen;
        if ((err = der_decode_choice(in + x, &z, data, size)) != CRYPT_OK) {
          if (!ordered) {
            continue;
          }
          goto LBL_ERR;
        }
        break;

      default:
        err = CRYPT_INVALID_ARG;
        goto LBL_ERR;
    }
    x += z;
    inlen -= z;
    list[i].used = 1;
    if (!ordered) {
      /* restart the decoder */
      i = -1;
    }
  }

  for (i = 0; i < outlen; i++) {
    if (list[i].used == 0) {
      err = CRYPT_INVALID_PACKET;
      goto LBL_ERR;
    }
  }
  err = CRYPT_OK;

LBL_ERR:
  return err;
}

static unsigned long fetch_length(const unsigned char *in,
                                  unsigned long inlen) {
  unsigned long x, y, z;

  y = 0;

  /* skip type and read len */
  if (inlen < 2) {
    return 0xFFFFFFFF;
  }
  ++in;
  ++y;

  /* read len */
  x = *in++;
  ++y;

  /* <128 means literal */
  if (x < 128) {
    return x + y;
  }
  x &= 0x7F; /* the lower 7 bits are the length of the length */
  inlen -= 2;

  /* len means len of len! */
  if ((x == 0) || (x > 4) || (x > inlen)) {
    return 0xFFFFFFFF;
  }

  y += x;
  z = 0;
  while (x--) {
    z = (z << 8) | ((unsigned long)*in);
    ++in;
  }
  return z + y;
}

/**
   ASN.1 DER Flexi(ble) decoder will decode arbitrary DER packets and create a
   linked list of the decoded elements.
   @param in      The input buffer
   @param inlen   [in/out] The length of the input buffer and on output the
   amount of decoded data
   @param out     [out] A pointer to the linked list
   @return CRYPT_OK on success.
 */
int der_decode_sequence_flexi(const unsigned char *in, unsigned long *inlen,
                              ltc_asn1_list **out) {
  ltc_asn1_list *l;
  unsigned long err, type, len, totlen, x, y;
  void *realloc_tmp;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(inlen != NULL);
  LTC_ARGCHK(out != NULL);

  l = NULL;
  totlen = 0;

  /* scan the input and and get lengths and what not */
  while (*inlen) {
    /* read the type byte */
    type = *in;

    /* fetch length */
    len = fetch_length(in, *inlen);
    if (len > *inlen) {
      err = CRYPT_INVALID_PACKET;
      goto error;
    }

    /* alloc new link */
    if (l == NULL) {
      l = XCALLOC(1, sizeof(*l));
      if (l == NULL) {
        err = CRYPT_MEM;
        goto error;
      }
    } else {
      l->next = XCALLOC(1, sizeof(*l));
      if (l->next == NULL) {
        err = CRYPT_MEM;
        goto error;
      }
      l->next->prev = l;
      l = l->next;
    }

    /* now switch on type */
    switch (type) {
      case 0x01: /* BOOLEAN */
        l->type = LTC_ASN1_BOOLEAN;
        l->size = 1;
        l->data = XCALLOC(1, sizeof(int));

        if ((err = der_decode_boolean(in, *inlen, l->data)) != CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_boolean(&len)) != CRYPT_OK) {
          goto error;
        }
        break;

      case 0x02: /* INTEGER */
        /* init field */
        l->type = LTC_ASN1_INTEGER;
        l->size = 1;
        if ((err = mp_init(&l->data)) != CRYPT_OK) {
          goto error;
        }

        /* decode field */
        if ((err = der_decode_integer(in, *inlen, l->data)) != CRYPT_OK) {
          goto error;
        }

        /* calc length of object */
        if ((err = der_length_integer(l->data, &len)) != CRYPT_OK) {
          goto error;
        }
        break;

      case 0x03: /* BIT */
        /* init field */
        l->type = LTC_ASN1_BIT_STRING;
        l->size = len * 8; /* *8 because we store decoded bits one per char and
                      they are encoded 8 per char.  */

        if ((l->data = XCALLOC(1, l->size)) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        if ((err = der_decode_bit_string(in, *inlen, l->data, &l->size)) !=
            CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_bit_string(l->size, &len)) != CRYPT_OK) {
          goto error;
        }
        break;

      case 0x04: /* OCTET */

        /* init field */
        l->type = LTC_ASN1_OCTET_STRING;
        l->size = len;

        if ((l->data = XCALLOC(1, l->size)) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        if ((err = der_decode_octet_string(in, *inlen, l->data, &l->size)) !=
            CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_octet_string(l->size, &len)) != CRYPT_OK) {
          goto error;
        }
        break;

      case 0x05: /* NULL */

        /* valid NULL is 0x05 0x00 */
        if ((in[0] != 0x05) || (in[1] != 0x00)) {
          err = CRYPT_INVALID_PACKET;
          goto error;
        }

        /* simple to store ;-) */
        l->type = LTC_ASN1_NULL;
        l->data = NULL;
        l->size = 0;
        len = 2;

        break;

      case 0x06: /* OID */

        /* init field */
        l->type = LTC_ASN1_OBJECT_IDENTIFIER;
        l->size = len;

        if ((l->data = XCALLOC(len, sizeof(unsigned long))) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        if ((err = der_decode_object_identifier(in, *inlen, l->data,
                                                &l->size)) != CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_object_identifier(l->data, l->size, &len)) !=
            CRYPT_OK) {
          goto error;
        }

        /* resize it to save a bunch of mem */
        if ((realloc_tmp =
                 XREALLOC(l->data, l->size * sizeof(unsigned long))) == NULL) {
          /* out of heap but this is not an error */
          break;
        }
        l->data = realloc_tmp;
        break;

      case 0x0C: /* UTF8 */

        /* init field */
        l->type = LTC_ASN1_UTF8_STRING;
        l->size = len;

        if ((l->data = XCALLOC(sizeof(wchar_t), l->size)) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        if ((err = der_decode_utf8_string(in, *inlen, l->data, &l->size)) !=
            CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_utf8_string(l->data, l->size, &len)) !=
            CRYPT_OK) {
          goto error;
        }
        break;

      case 0x13: /* PRINTABLE */

        /* init field */
        l->type = LTC_ASN1_PRINTABLE_STRING;
        l->size = len;

        if ((l->data = XCALLOC(1, l->size)) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        if ((err = der_decode_printable_string(in, *inlen, l->data,
                                               &l->size)) != CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_printable_string(l->data, l->size, &len)) !=
            CRYPT_OK) {
          goto error;
        }
        break;

      case 0x16: /* IA5 */

        /* init field */
        l->type = LTC_ASN1_IA5_STRING;
        l->size = len;

        if ((l->data = XCALLOC(1, l->size)) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        if ((err = der_decode_ia5_string(in, *inlen, l->data, &l->size)) !=
            CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_ia5_string(l->data, l->size, &len)) != CRYPT_OK) {
          goto error;
        }
        break;

      case 0x17: /* UTC TIME */

        /* init field */
        l->type = LTC_ASN1_UTCTIME;
        l->size = 1;

        if ((l->data = XCALLOC(1, sizeof(ltc_utctime))) == NULL) {
          err = CRYPT_MEM;
          goto error;
        }

        len = *inlen;
        if ((err = der_decode_utctime(in, &len, l->data)) != CRYPT_OK) {
          goto error;
        }

        if ((err = der_length_utctime(l->data, &len)) != CRYPT_OK) {
          goto error;
        }
        break;

      case 0x30: /* SEQUENCE */
      case 0x31: /* SET */

        /* init field */
        l->type = (type == 0x30) ? LTC_ASN1_SEQUENCE : LTC_ASN1_SET;

        /* we have to decode the SEQUENCE header and get it's length */

        /* move past type */
        ++in;
        --(*inlen);

        /* read length byte */
        x = *in++;
        --(*inlen);

        /* smallest SEQUENCE/SET header */
        y = 2;

        /* now if it's > 127 the next bytes are the length of the length */
        if (x > 128) {
          x &= 0x7F;
          in += x;
          *inlen -= x;

          /* update sequence header len */
          y += x;
        }

        /* Sequence elements go as child */
        len = len - y;
        if ((err = der_decode_sequence_flexi(in, &len, &(l->child))) !=
            CRYPT_OK) {
          goto error;
        }

        /* len update */
        totlen += y;

        /* link them up y0 */
        l->child->parent = l;

        break;

      default:
        /* invalid byte ... this is a soft error */
        /* remove link */
        l = l->prev;
        XFREE(l->next);
        l->next = NULL;
        goto outside;
    }

    /* advance pointers */
    totlen += len;
    in += len;
    *inlen -= len;
  }

outside:

  /* rewind l please */
  while (l->prev != NULL || l->parent != NULL) {
    if (l->parent != NULL) {
      l = l->parent;
    } else {
      l = l->prev;
    }
  }

  /* return */
  *out = l;
  *inlen = totlen;
  return CRYPT_OK;

error:
  /* free list */
  der_sequence_free(l);

  return err;
}

/**
   Decode a SEQUENCE type using a VA list
   @param in    Input buffer
   @param inlen Length of input in octets
   @remark <...> is of the form <type, size, data> (int, unsigned long, void*)
   @return CRYPT_OK on success
 */
int der_decode_sequence_multi(const unsigned char *in, unsigned long inlen,
                              ...) {
  int err, type;
  unsigned long size, x;
  void *data;
  va_list args;
  ltc_asn1_list *list;

  LTC_ARGCHK(in != NULL);

  /* get size of output that will be required */
  va_start(args, inlen);
  x = 0;
  for (;;) {
    type = va_arg(args, int);
    size = va_arg(args, unsigned long);
    data = va_arg(args, void *);

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
      case LTC_ASN1_INTEGER:
      case LTC_ASN1_SHORT_INTEGER:
      case LTC_ASN1_BIT_STRING:
      case LTC_ASN1_OCTET_STRING:
      case LTC_ASN1_NULL:
      case LTC_ASN1_OBJECT_IDENTIFIER:
      case LTC_ASN1_IA5_STRING:
      case LTC_ASN1_PRINTABLE_STRING:
      case LTC_ASN1_UTF8_STRING:
      case LTC_ASN1_UTCTIME:
      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
      case LTC_ASN1_SEQUENCE:
      case LTC_ASN1_CHOICE:
        ++x;
        break;

      default:
        va_end(args);
        return CRYPT_INVALID_ARG;
    }
  }
  va_end(args);

  /* allocate structure for x elements */
  if (x == 0) {
    return CRYPT_NOP;
  }

  list = XCALLOC(sizeof(*list), x);
  if (list == NULL) {
    return CRYPT_MEM;
  }

  /* fill in the structure */
  va_start(args, inlen);
  x = 0;
  for (;;) {
    type = va_arg(args, int);
    size = va_arg(args, unsigned long);
    data = va_arg(args, void *);

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
      case LTC_ASN1_INTEGER:
      case LTC_ASN1_SHORT_INTEGER:
      case LTC_ASN1_BIT_STRING:
      case LTC_ASN1_OCTET_STRING:
      case LTC_ASN1_NULL:
      case LTC_ASN1_OBJECT_IDENTIFIER:
      case LTC_ASN1_IA5_STRING:
      case LTC_ASN1_PRINTABLE_STRING:
      case LTC_ASN1_UTF8_STRING:
      case LTC_ASN1_UTCTIME:
      case LTC_ASN1_SEQUENCE:
      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
      case LTC_ASN1_CHOICE:
        list[x].type = type;
        list[x].size = size;
        list[x++].data = data;
        break;

      default:
        va_end(args);
        err = CRYPT_INVALID_ARG;
        goto LBL_ERR;
    }
  }
  va_end(args);

  err = der_decode_sequence(in, inlen, list, x);
LBL_ERR:
  XFREE(list);
  return err;
}

/**
   Read a short integer
   @param in       The DER encoded data
   @param inlen    Size of data
   @param num      [out] The integer to decode
   @return CRYPT_OK if successful
 */
int der_decode_short_integer(const unsigned char *in, unsigned long inlen,
                             unsigned long *num) {
  unsigned long len, x, y;

  LTC_ARGCHK(num != NULL);
  LTC_ARGCHK(in != NULL);

  /* check length */
  if (inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* check header */
  x = 0;
  if ((in[x++] & 0x1F) != 0x02) {
    return CRYPT_INVALID_PACKET;
  }

  /* get the packet len */
  len = in[x++];

  if (x + len > inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* read number */
  y = 0;
  while (len--) {
    y = (y << 8) | (unsigned long)in[x++];
  }
  *num = y;

  return CRYPT_OK;
}

static int char_to_int(unsigned char x) {
  switch (x) {
    case '0':
      return 0;

    case '1':
      return 1;

    case '2':
      return 2;

    case '3':
      return 3;

    case '4':
      return 4;

    case '5':
      return 5;

    case '6':
      return 6;

    case '7':
      return 7;

    case '8':
      return 8;

    case '9':
      return 9;
  }
  return 100;
}

#define DECODE_V(y, max)                                  \
  y = char_to_int(buf[x]) * 10 + char_to_int(buf[x + 1]); \
  if (y >= max) return CRYPT_INVALID_PACKET;              \
  x += 2;

/**
   Decodes a UTC time structure in DER format (reads all 6 valid encoding
   formats)
   @param in     Input buffer
   @param inlen  Length of input buffer in octets
   @param out    [out] Destination of UTC time structure
   @return CRYPT_OK   if successful
 */
int der_decode_utctime(const unsigned char *in, unsigned long *inlen,
                       ltc_utctime *out) {
  unsigned char buf[32];
  unsigned long x;
  int y;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(inlen != NULL);
  LTC_ARGCHK(out != NULL);

  /* check header */
  if ((*inlen < 2UL) || (in[1] >= sizeof(buf)) || ((in[1] + 2UL) > *inlen)) {
    return CRYPT_INVALID_PACKET;
  }

  /* decode the string */
  for (x = 0; x < in[1]; x++) {
    y = der_ia5_value_decode(in[x + 2]);
    if (y == -1) {
      return CRYPT_INVALID_PACKET;
    }
    buf[x] = y;
  }
  *inlen = 2 + x;

  /* possible encodings are
   YYMMDDhhmmZ
   YYMMDDhhmm+hh'mm'
   YYMMDDhhmm-hh'mm'
   YYMMDDhhmmssZ
   YYMMDDhhmmss+hh'mm'
   YYMMDDhhmmss-hh'mm'

   So let's do a trivial decode upto [including] mm
 */

  x = 0;
  DECODE_V(out->YY, 100);
  DECODE_V(out->MM, 13);
  DECODE_V(out->DD, 32);
  DECODE_V(out->hh, 24);
  DECODE_V(out->mm, 60);

  /* clear timezone and seconds info */
  out->off_dir = out->off_hh = out->off_mm = out->ss = 0;

  /* now is it Z, +, - or 0-9 */
  if (buf[x] == 'Z') {
    return CRYPT_OK;
  } else if ((buf[x] == '+') || (buf[x] == '-')) {
    out->off_dir = (buf[x++] == '+') ? 0 : 1;
    DECODE_V(out->off_hh, 24);
    DECODE_V(out->off_mm, 60);
    return CRYPT_OK;
  }

  /* decode seconds */
  DECODE_V(out->ss, 60);

  /* now is it Z, +, - */
  if (buf[x] == 'Z') {
    return CRYPT_OK;
  } else if ((buf[x] == '+') || (buf[x] == '-')) {
    out->off_dir = (buf[x++] == '+') ? 0 : 1;
    DECODE_V(out->off_hh, 24);
    DECODE_V(out->off_mm, 60);
    return CRYPT_OK;
  } else {
    return CRYPT_INVALID_PACKET;
  }
}

/**
   Store a UTF8 STRING
   @param in      The DER encoded UTF8 STRING
   @param inlen   The size of the DER UTF8 STRING
   @param out     [out] The array of utf8s stored (one per char)
   @param outlen  [in/out] The number of utf8s stored
   @return CRYPT_OK if successful
 */
int der_decode_utf8_string(const unsigned char *in, unsigned long inlen,
                           wchar_t *out, unsigned long *outlen) {
  wchar_t tmp;
  unsigned long x, y, z, len;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* must have header at least */
  if (inlen < 2) {
    return CRYPT_INVALID_PACKET;
  }

  /* check for 0x0C */
  if ((in[0] & 0x1F) != 0x0C) {
    return CRYPT_INVALID_PACKET;
  }
  x = 1;

  /* decode the length */
  if (in[x] & 0x80) {
    /* valid # of bytes in length are 1,2,3 */
    y = in[x] & 0x7F;
    if ((y == 0) || (y > 3) || ((x + y) > inlen)) {
      return CRYPT_INVALID_PACKET;
    }

    /* read the length in */
    len = 0;
    ++x;
    while (y--) {
      len = (len << 8) | in[x++];
    }
  } else {
    len = in[x++] & 0x7F;
  }

  if (len + x > inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* proceed to decode */
  for (y = 0; x < inlen;) {
    /* get first byte */
    tmp = in[x++];

    /* count number of bytes */
    for (z = 0; (tmp & 0x80) && (z <= 4); z++, tmp = (tmp << 1) & 0xFF)
      ;

    if ((z > 4) || (x + (z - 1) > inlen)) {
      return CRYPT_INVALID_PACKET;
    }

    /* decode, grab upper bits */
    tmp >>= z;

    /* grab remaining bytes */
    if (z > 1) {
      --z;
    }
    while (z-- != 0) {
      if ((in[x] & 0xC0) != 0x80) {
        return CRYPT_INVALID_PACKET;
      }
      tmp = (tmp << 6) | ((wchar_t)in[x++] & 0x3F);
    }

    if (y > *outlen) {
      *outlen = y;
      return CRYPT_BUFFER_OVERFLOW;
    }
    out[y++] = tmp;
  }
  *outlen = y;

  return CRYPT_OK;
}

/**
   Store a BIT STRING
   @param in       The array of bits to store (one per char)
   @param inlen    The number of bits tostore
   @param out      [out] The destination for the DER encoded BIT STRING
   @param outlen   [in/out] The max size and resulting size of the DER BIT
   STRING
   @return CRYPT_OK if successful
 */
int der_encode_bit_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen) {
  unsigned long len, x, y;
  unsigned char buf;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* avoid overflows */
  if ((err = der_length_bit_string(inlen, &len)) != CRYPT_OK) {
    return err;
  }

  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* store header (include bit padding count in length) */
  x = 0;
  y = (inlen >> 3) + ((inlen & 7) ? 1 : 0) + 1;

  out[x++] = 0x03;
  if (y < 128) {
    out[x++] = (unsigned char)y;
  } else if (y < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)y;
  } else if (y < 65536) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((y >> 8) & 255);
    out[x++] = (unsigned char)(y & 255);
  }

  /* store number of zero padding bits */
  out[x++] = (unsigned char)((8 - inlen) & 7);

  /* store the bits in big endian format */
  for (y = buf = 0; y < inlen; y++) {
    buf |= (in[y] ? 1 : 0) << (7 - (y & 7));
    if ((y & 7) == 7) {
      out[x++] = buf;
      buf = 0;
    }
  }
  /* store last byte */
  if (inlen & 7) {
    out[x++] = buf;
  }
  *outlen = x;
  return CRYPT_OK;
}

/* $Source:
 * /cvs/libtom/libtomcrypt/src/pk/asn1/der/bit/der_encode_bit_string.c,v $ */

/**
   @file der_encode_boolean.c
   ASN.1 DER, encode a BOOLEAN, Tom St Denis
 */

/**
   Store a BOOLEAN
   @param in       The boolean to encode
   @param out      [out] The destination for the DER encoded BOOLEAN
   @param outlen   [in/out] The max size and resulting size of the DER BOOLEAN
   @return CRYPT_OK if successful
 */
int der_encode_boolean(int in, unsigned char *out, unsigned long *outlen) {
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(out != NULL);

  if (*outlen < 3) {
    *outlen = 3;
    return CRYPT_BUFFER_OVERFLOW;
  }

  *outlen = 3;
  out[0] = 0x01;
  out[1] = 0x01;
  out[2] = in ? 0xFF : 0x00;

  return CRYPT_OK;
}

/**
   Store an IA5 STRING
   @param in       The array of IA5 to store (one per char)
   @param inlen    The number of IA5 to store
   @param out      [out] The destination for the DER encoded IA5 STRING
   @param outlen   [in/out] The max size and resulting size of the DER IA5
   STRING
   @return CRYPT_OK if successful
 */
int der_encode_ia5_string(const unsigned char *in, unsigned long inlen,
                          unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get the size */
  if ((err = der_length_ia5_string(in, inlen, &len)) != CRYPT_OK) {
    return err;
  }

  /* too big? */
  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* encode the header+len */
  x = 0;
  out[x++] = 0x16;
  if (inlen < 128) {
    out[x++] = (unsigned char)inlen;
  } else if (inlen < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)inlen;
  } else if (inlen < 65536UL) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((inlen >> 8) & 255);
    out[x++] = (unsigned char)(inlen & 255);
  } else if (inlen < 16777216UL) {
    out[x++] = 0x83;
    out[x++] = (unsigned char)((inlen >> 16) & 255);
    out[x++] = (unsigned char)((inlen >> 8) & 255);
    out[x++] = (unsigned char)(inlen & 255);
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* store octets */
  for (y = 0; y < inlen; y++) {
    out[x++] = der_ia5_char_encode(in[y]);
  }

  /* retun length */
  *outlen = x;

  return CRYPT_OK;
}

/* Exports a positive bignum as DER format (upto 2^32 bytes in size) */

/**
   Store a mp_int integer
   @param num      The first mp_int to encode
   @param out      [out] The destination for the DER encoded integers
   @param outlen   [in/out] The max size and resulting size of the DER encoded
   integers
   @return CRYPT_OK if successful
 */
int der_encode_integer(void *num, unsigned char *out, unsigned long *outlen) {
  unsigned long tmplen, y;
  int err, leading_zero;

  LTC_ARGCHK(num != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* find out how big this will be */
  if ((err = der_length_integer(num, &tmplen)) != CRYPT_OK) {
    return err;
  }

  if (*outlen < tmplen) {
    *outlen = tmplen;
    return CRYPT_BUFFER_OVERFLOW;
  }

  if (mp_cmp_d(num, 0) != LTC_MP_LT) {
    /* we only need a leading zero if the msb of the first byte is one */
    if (((mp_count_bits(num) & 7) == 0) || (mp_iszero(num) == LTC_MP_YES)) {
      leading_zero = 1;
    } else {
      leading_zero = 0;
    }

    /* get length of num in bytes (plus 1 since we force the msbyte to zero) */
    y = mp_unsigned_bin_size(num) + leading_zero;
  } else {
    leading_zero = 0;
    y = mp_count_bits(num);
    y = y + (8 - (y & 7));
    y = y >> 3;
    if (((mp_cnt_lsb(num) + 1) == mp_count_bits(num)) &&
        ((mp_count_bits(num) & 7) == 0))
      --y;
  }

  /* now store initial data */
  *out++ = 0x02;
  if (y < 128) {
    /* short form */
    *out++ = (unsigned char)y;
  } else if (y < 256) {
    *out++ = 0x81;
    *out++ = (unsigned char)y;
  } else if (y < 65536UL) {
    *out++ = 0x82;
    *out++ = (unsigned char)((y >> 8) & 255);
    *out++ = (unsigned char)y;
  } else if (y < 16777216UL) {
    *out++ = 0x83;
    *out++ = (unsigned char)((y >> 16) & 255);
    *out++ = (unsigned char)((y >> 8) & 255);
    *out++ = (unsigned char)y;
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* now store msbyte of zero if num is non-zero */
  if (leading_zero) {
    *out++ = 0x00;
  }

  /* if it's not zero store it as big endian */
  if (mp_cmp_d(num, 0) == LTC_MP_GT) {
    /* now store the mpint */
    if ((err = mp_to_unsigned_bin(num, out)) != CRYPT_OK) {
      return err;
    }
  } else if (mp_iszero(num) != LTC_MP_YES) {
    void *tmp;

    /* negative */
    if (mp_init(&tmp) != CRYPT_OK) {
      return CRYPT_MEM;
    }

    /* 2^roundup and subtract */
    y = mp_count_bits(num);
    y = y + (8 - (y & 7));
    if (((mp_cnt_lsb(num) + 1) == mp_count_bits(num)) &&
        ((mp_count_bits(num) & 7) == 0))
      y -= 8;
    if ((mp_2expt(tmp, y) != CRYPT_OK) || (mp_add(tmp, num, tmp) != CRYPT_OK)) {
      mp_clear(tmp);
      return CRYPT_MEM;
    }
    if ((err = mp_to_unsigned_bin(tmp, out)) != CRYPT_OK) {
      mp_clear(tmp);
      return err;
    }
    mp_clear(tmp);
  }

  /* we good */
  *outlen = tmplen;
  return CRYPT_OK;
}

/**
   Encode an OID
   @param words   The words to encode  (upto 32-bits each)
   @param nwords  The number of words in the OID
   @param out     [out] Destination of OID data
   @param outlen  [in/out] The max and resulting size of the OID
   @return CRYPT_OK if successful
 */
int der_encode_object_identifier(unsigned long *words, unsigned long nwords,
                                 unsigned char *out, unsigned long *outlen) {
  unsigned long i, x, y, z, t, mask, wordbuf;
  int err;

  LTC_ARGCHK(words != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* check length */
  if ((err = der_length_object_identifier(words, nwords, &x)) != CRYPT_OK) {
    return err;
  }
  if (x > *outlen) {
    *outlen = x;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* compute length to store OID data */
  z = 0;
  wordbuf = words[0] * 40 + words[1];
  for (y = 1; y < nwords; y++) {
    t = der_object_identifier_bits(wordbuf);
    z += t / 7 + ((t % 7) ? 1 : 0) + (wordbuf == 0 ? 1 : 0);
    if (y < nwords - 1) {
      wordbuf = words[y + 1];
    }
  }

  /* store header + length */
  x = 0;
  out[x++] = 0x06;
  if (z < 128) {
    out[x++] = (unsigned char)z;
  } else if (z < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)z;
  } else if (z < 65536UL) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((z >> 8) & 255);
    out[x++] = (unsigned char)(z & 255);
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* store first byte */
  wordbuf = words[0] * 40 + words[1];
  for (i = 1; i < nwords; i++) {
    /* store 7 bit words in little endian */
    t = wordbuf & 0xFFFFFFFF;
    if (t) {
      y = x;
      mask = 0;
      while (t) {
        out[x++] = (unsigned char)((t & 0x7F) | mask);
        t >>= 7;
        mask |= 0x80; /* upper bit is set on all but the last byte */
      }
      /* now swap bytes y...x-1 */
      z = x - 1;
      while (y < z) {
        t = out[y];
        out[y] = out[z];
        out[z] = (unsigned char)t;
        ++y;
        --z;
      }
    } else {
      /* zero word */
      out[x++] = 0x00;
    }

    if (i < nwords - 1) {
      wordbuf = words[i + 1];
    }
  }

  *outlen = x;
  return CRYPT_OK;
}

/**
   Store an OCTET STRING
   @param in       The array of OCTETS to store (one per char)
   @param inlen    The number of OCTETS to store
   @param out      [out] The destination for the DER encoded OCTET STRING
   @param outlen   [in/out] The max size and resulting size of the DER OCTET
   STRING
   @return CRYPT_OK if successful
 */
int der_encode_octet_string(const unsigned char *in, unsigned long inlen,
                            unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get the size */
  if ((err = der_length_octet_string(inlen, &len)) != CRYPT_OK) {
    return err;
  }

  /* too big? */
  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* encode the header+len */
  x = 0;
  out[x++] = 0x04;
  if (inlen < 128) {
    out[x++] = (unsigned char)inlen;
  } else if (inlen < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)inlen;
  } else if (inlen < 65536UL) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((inlen >> 8) & 255);
    out[x++] = (unsigned char)(inlen & 255);
  } else if (inlen < 16777216UL) {
    out[x++] = 0x83;
    out[x++] = (unsigned char)((inlen >> 16) & 255);
    out[x++] = (unsigned char)((inlen >> 8) & 255);
    out[x++] = (unsigned char)(inlen & 255);
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* store octets */
  for (y = 0; y < inlen; y++) {
    out[x++] = in[y];
  }

  /* retun length */
  *outlen = x;

  return CRYPT_OK;
}

/**
   Store an printable STRING
   @param in       The array of printable to store (one per char)
   @param inlen    The number of printable to store
   @param out      [out] The destination for the DER encoded printable STRING
   @param outlen   [in/out] The max size and resulting size of the DER printable
   STRING
   @return CRYPT_OK if successful
 */
int der_encode_printable_string(const unsigned char *in, unsigned long inlen,
                                unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get the size */
  if ((err = der_length_printable_string(in, inlen, &len)) != CRYPT_OK) {
    return err;
  }

  /* too big? */
  if (len > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* encode the header+len */
  x = 0;
  out[x++] = 0x13;
  if (inlen < 128) {
    out[x++] = (unsigned char)inlen;
  } else if (inlen < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)inlen;
  } else if (inlen < 65536UL) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((inlen >> 8) & 255);
    out[x++] = (unsigned char)(inlen & 255);
  } else if (inlen < 16777216UL) {
    out[x++] = 0x83;
    out[x++] = (unsigned char)((inlen >> 16) & 255);
    out[x++] = (unsigned char)((inlen >> 8) & 255);
    out[x++] = (unsigned char)(inlen & 255);
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* store octets */
  for (y = 0; y < inlen; y++) {
    out[x++] = der_printable_char_encode(in[y]);
  }

  /* retun length */
  *outlen = x;

  return CRYPT_OK;
}

/**
   Encode a SEQUENCE
   @param list      The list of items to encode
   @param inlen     The number of items in the list
   @param out       [out] The destination
   @param outlen    [in/out] The size of the output
   @param type_of   LTC_ASN1_SEQUENCE or LTC_ASN1_SET/LTC_ASN1_SETOF
   @return CRYPT_OK on success
 */
int der_encode_sequence_ex(ltc_asn1_list *list, unsigned long inlen,
                           unsigned char *out, unsigned long *outlen,
                           int type_of) {
  int err, type;
  unsigned long size, x, y, z, i;
  void *data;

  LTC_ARGCHK(list != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get size of output that will be required */
  y = 0;
  for (i = 0; i < inlen; i++) {
    type = list[i].type;
    size = list[i].size;
    data = list[i].data;

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
        if ((err = der_length_boolean(&x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_INTEGER:
        if ((err = der_length_integer(data, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_SHORT_INTEGER:
        if ((err = der_length_short_integer(*((unsigned long *)data), &x)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_BIT_STRING:
        if ((err = der_length_bit_string(size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_OCTET_STRING:
        if ((err = der_length_octet_string(size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_NULL:
        y += 2;
        break;

      case LTC_ASN1_OBJECT_IDENTIFIER:
        if ((err = der_length_object_identifier(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_IA5_STRING:
        if ((err = der_length_ia5_string(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_PRINTABLE_STRING:
        if ((err = der_length_printable_string(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_UTF8_STRING:
        if ((err = der_length_utf8_string(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_UTCTIME:
        if ((err = der_length_utctime(data, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
      case LTC_ASN1_SEQUENCE:
        if ((err = der_length_sequence(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      default:
        err = CRYPT_INVALID_ARG;
        goto LBL_ERR;
    }
  }

  /* calc header size */
  z = y;
  if (y < 128) {
    y += 2;
  } else if (y < 256) {
    /* 0x30 0x81 LL */
    y += 3;
  } else if (y < 65536UL) {
    /* 0x30 0x82 LL LL */
    y += 4;
  } else if (y < 16777216UL) {
    /* 0x30 0x83 LL LL LL */
    y += 5;
  } else {
    err = CRYPT_INVALID_ARG;
    goto LBL_ERR;
  }

  /* too big ? */
  if (*outlen < y) {
    *outlen = y;
    err = CRYPT_BUFFER_OVERFLOW;
    goto LBL_ERR;
  }

  /* store header */
  x = 0;
  out[x++] = (type_of == LTC_ASN1_SEQUENCE) ? 0x30 : 0x31;

  if (z < 128) {
    out[x++] = (unsigned char)z;
  } else if (z < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)z;
  } else if (z < 65536UL) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((z >> 8UL) & 255);
    out[x++] = (unsigned char)(z & 255);
  } else if (z < 16777216UL) {
    out[x++] = 0x83;
    out[x++] = (unsigned char)((z >> 16UL) & 255);
    out[x++] = (unsigned char)((z >> 8UL) & 255);
    out[x++] = (unsigned char)(z & 255);
  }

  /* store data */
  *outlen -= x;
  for (i = 0; i < inlen; i++) {
    type = list[i].type;
    size = list[i].size;
    data = list[i].data;

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
        z = *outlen;
        if ((err = der_encode_boolean(*((int *)data), out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_INTEGER:
        z = *outlen;
        if ((err = der_encode_integer(data, out + x, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_SHORT_INTEGER:
        z = *outlen;
        if ((err = der_encode_short_integer(*((unsigned long *)data), out + x,
                                            &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_BIT_STRING:
        z = *outlen;
        if ((err = der_encode_bit_string(data, size, out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_OCTET_STRING:
        z = *outlen;
        if ((err = der_encode_octet_string(data, size, out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_NULL:
        out[x++] = 0x05;
        out[x++] = 0x00;
        *outlen -= 2;
        break;

      case LTC_ASN1_OBJECT_IDENTIFIER:
        z = *outlen;
        if ((err = der_encode_object_identifier(data, size, out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_IA5_STRING:
        z = *outlen;
        if ((err = der_encode_ia5_string(data, size, out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_PRINTABLE_STRING:
        z = *outlen;
        if ((err = der_encode_printable_string(data, size, out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_UTF8_STRING:
        z = *outlen;
        if ((err = der_encode_utf8_string(data, size, out + x, &z)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_UTCTIME:
        z = *outlen;
        if ((err = der_encode_utctime(data, out + x, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_SET:
        z = *outlen;
        if ((err = der_encode_set(data, size, out + x, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_SETOF:
        z = *outlen;
        if ((err = der_encode_setof(data, size, out + x, &z)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      case LTC_ASN1_SEQUENCE:
        z = *outlen;
        if ((err = der_encode_sequence_ex(data, size, out + x, &z, type)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        x += z;
        *outlen -= z;
        break;

      default:
        err = CRYPT_INVALID_ARG;
        goto LBL_ERR;
    }
  }
  *outlen = x;
  err = CRYPT_OK;

LBL_ERR:
  return err;
}

/**
   Encode a SEQUENCE type using a VA list
   @param out    [out] Destination for data
   @param outlen [in/out] Length of buffer and resulting length of output
   @remark <...> is of the form <type, size, data> (int, unsigned long, void*)
   @return CRYPT_OK on success
 */
int der_encode_sequence_multi(unsigned char *out, unsigned long *outlen, ...) {
  int err, type;
  unsigned long size, x;
  void *data;
  va_list args;
  ltc_asn1_list *list;

  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get size of output that will be required */
  va_start(args, outlen);
  x = 0;
  for (;;) {
    type = va_arg(args, int);
    size = va_arg(args, unsigned long);
    data = va_arg(args, void *);

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
      case LTC_ASN1_INTEGER:
      case LTC_ASN1_SHORT_INTEGER:
      case LTC_ASN1_BIT_STRING:
      case LTC_ASN1_OCTET_STRING:
      case LTC_ASN1_NULL:
      case LTC_ASN1_OBJECT_IDENTIFIER:
      case LTC_ASN1_IA5_STRING:
      case LTC_ASN1_PRINTABLE_STRING:
      case LTC_ASN1_UTF8_STRING:
      case LTC_ASN1_UTCTIME:
      case LTC_ASN1_SEQUENCE:
      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
        ++x;
        break;

      default:
        va_end(args);
        return CRYPT_INVALID_ARG;
    }
  }
  va_end(args);

  /* allocate structure for x elements */
  if (x == 0) {
    return CRYPT_NOP;
  }

  list = XCALLOC(sizeof(*list), x);
  if (list == NULL) {
    return CRYPT_MEM;
  }

  /* fill in the structure */
  va_start(args, outlen);
  x = 0;
  for (;;) {
    type = va_arg(args, int);
    size = va_arg(args, unsigned long);
    data = va_arg(args, void *);

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
      case LTC_ASN1_INTEGER:
      case LTC_ASN1_SHORT_INTEGER:
      case LTC_ASN1_BIT_STRING:
      case LTC_ASN1_OCTET_STRING:
      case LTC_ASN1_NULL:
      case LTC_ASN1_OBJECT_IDENTIFIER:
      case LTC_ASN1_IA5_STRING:
      case LTC_ASN1_PRINTABLE_STRING:
      case LTC_ASN1_UTF8_STRING:
      case LTC_ASN1_UTCTIME:
      case LTC_ASN1_SEQUENCE:
      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
        list[x].type = type;
        list[x].size = size;
        list[x++].data = data;
        break;

      default:
        va_end(args);
        err = CRYPT_INVALID_ARG;
        goto LBL_ERR;
    }
  }
  va_end(args);

  err = der_encode_sequence(list, x, out, outlen);
LBL_ERR:
  XFREE(list);
  return err;
}

/**
   @file der_encode_set.c
   ASN.1 DER, Encode a SET, Tom St Denis
 */

/* LTC define to ASN.1 TAG */
static int ltc_to_asn1(int v) {
  switch (v) {
    case LTC_ASN1_BOOLEAN:
      return 0x01;

    case LTC_ASN1_INTEGER:
    case LTC_ASN1_SHORT_INTEGER:
      return 0x02;

    case LTC_ASN1_BIT_STRING:
      return 0x03;

    case LTC_ASN1_OCTET_STRING:
      return 0x04;

    case LTC_ASN1_NULL:
      return 0x05;

    case LTC_ASN1_OBJECT_IDENTIFIER:
      return 0x06;

    case LTC_ASN1_UTF8_STRING:
      return 0x0C;

    case LTC_ASN1_PRINTABLE_STRING:
      return 0x13;

    case LTC_ASN1_IA5_STRING:
      return 0x16;

    case LTC_ASN1_UTCTIME:
      return 0x17;

    case LTC_ASN1_SEQUENCE:
      return 0x30;

    case LTC_ASN1_SET:
    case LTC_ASN1_SETOF:
      return 0x31;

    default:
      return -1;
  }
}

static int qsort_helper_set(const void *a, const void *b) {
  ltc_asn1_list *A = (ltc_asn1_list *)a, *B = (ltc_asn1_list *)b;
  int r;

  r = ltc_to_asn1(A->type) - ltc_to_asn1(B->type);

  /* for QSORT the order is UNDEFINED if they are "equal" which means it is NOT
   * DETERMINISTIC.  So we force it to be :-) */
  if (r == 0) {
    /* their order in the original list now determines the position */
    return A->used - B->used;
  } else {
    return r;
  }
}

/*
   Encode a SET type
   @param list      The list of items to encode
   @param inlen     The number of items in the list
   @param out       [out] The destination
   @param outlen    [in/out] The size of the output
   @return CRYPT_OK on success
 */
int der_encode_set(ltc_asn1_list *list, unsigned long inlen, unsigned char *out,
                   unsigned long *outlen) {
  ltc_asn1_list *copy;
  unsigned long x;
  int err;

  /* make copy of list */
  copy = XCALLOC(inlen, sizeof(*copy));
  if (copy == NULL) {
    return CRYPT_MEM;
  }

  /* fill in used member with index so we can fully sort it */
  for (x = 0; x < inlen; x++) {
    copy[x] = list[x];
    copy[x].used = x;
  }

  /* sort it by the "type" field */
  XQSORT(copy, inlen, sizeof(*copy), &qsort_helper_set);

  /* call der_encode_sequence_ex() */
  err = der_encode_sequence_ex(copy, inlen, out, outlen, LTC_ASN1_SET);

  /* free list */
  XFREE(copy);

  return err;
}

/**
   @file der_encode_setof.c
   ASN.1 DER, Encode SET OF, Tom St Denis
 */

struct edge {
  unsigned char *start;
  unsigned long size;
};

static int qsort_helper(const void *a, const void *b) {
  struct edge *A = (struct edge *)a, *B = (struct edge *)b;
  int r;
  unsigned long x;

  /* compare min length */
  r = XMEMCMP(A->start, B->start, MIN(A->size, B->size));

  if ((r == 0) && (A->size != B->size)) {
    if (A->size > B->size) {
      for (x = B->size; x < A->size; x++) {
        if (A->start[x]) {
          return 1;
        }
      }
    } else {
      for (x = A->size; x < B->size; x++) {
        if (B->start[x]) {
          return -1;
        }
      }
    }
  }

  return r;
}

/**
   Encode a SETOF stucture
   @param list      The list of items to encode
   @param inlen     The number of items in the list
   @param out       [out] The destination
   @param outlen    [in/out] The size of the output
   @return CRYPT_OK on success
 */
int der_encode_setof(ltc_asn1_list *list, unsigned long inlen,
                     unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, z, hdrlen;
  int err;
  struct edge *edges;
  unsigned char *ptr, *buf;

  /* check that they're all the same type */
  for (x = 1; x < inlen; x++) {
    if (list[x].type != list[x - 1].type) {
      return CRYPT_INVALID_ARG;
    }
  }

  /* alloc buffer to store copy of output */
  buf = XCALLOC(1, *outlen);
  if (buf == NULL) {
    return CRYPT_MEM;
  }

  /* encode list */
  if ((err = der_encode_sequence_ex(list, inlen, buf, outlen,
                                    LTC_ASN1_SETOF)) != CRYPT_OK) {
    XFREE(buf);
    return err;
  }

  /* allocate edges */
  edges = XCALLOC(inlen, sizeof(*edges));
  if (edges == NULL) {
    XFREE(buf);
    return CRYPT_MEM;
  }

  /* skip header */
  ptr = buf + 1;

  /* now skip length data */
  x = *ptr++;
  if (x >= 0x80) {
    ptr += (x & 0x7F);
  }

  /* get the size of the static header */
  hdrlen = ((unsigned long)ptr) - ((unsigned long)buf);

  /* scan for edges */
  x = 0;
  while (ptr < (buf + *outlen)) {
    /* store start */
    edges[x].start = ptr;

    /* skip type */
    z = 1;

    /* parse length */
    y = ptr[z++];
    if (y < 128) {
      edges[x].size = y;
    } else {
      y &= 0x7F;
      edges[x].size = 0;
      while (y--) {
        edges[x].size = (edges[x].size << 8) | ((unsigned long)ptr[z++]);
      }
    }

    /* skip content */
    edges[x].size += z;
    ptr += edges[x].size;
    ++x;
  }

  /* sort based on contents (using edges) */
  XQSORT(edges, inlen, sizeof(*edges), &qsort_helper);

  /* copy static header */
  XMEMCPY(out, buf, hdrlen);

  /* copy+sort using edges+indecies to output from buffer */
  for (y = hdrlen, x = 0; x < inlen; x++) {
    XMEMCPY(out + y, edges[x].start, edges[x].size);
    y += edges[x].size;
  }

  /* free buffers */
  XFREE(edges);
  XFREE(buf);

  return CRYPT_OK;
}

/**
   Store a short integer in the range (0,2^32-1)
   @param num      The integer to encode
   @param out      [out] The destination for the DER encoded integers
   @param outlen   [in/out] The max size and resulting size of the DER encoded
   integers
   @return CRYPT_OK if successful
 */
int der_encode_short_integer(unsigned long num, unsigned char *out,
                             unsigned long *outlen) {
  unsigned long len, x, y, z;
  int err;

  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* force to 32 bits */
  num &= 0xFFFFFFFFUL;

  /* find out how big this will be */
  if ((err = der_length_short_integer(num, &len)) != CRYPT_OK) {
    return err;
  }

  if (*outlen < len) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* get len of output */
  z = 0;
  y = num;
  while (y) {
    ++z;
    y >>= 8;
  }

  /* handle zero */
  if (z == 0) {
    z = 1;
  }

  /* see if msb is set */
  z += (num & (1UL << ((z << 3) - 1))) ? 1 : 0;

  /* adjust the number so the msB is non-zero */
  for (x = 0; (z <= 4) && (x < (4 - z)); x++) {
    num <<= 8;
  }

  /* store header */
  x = 0;
  out[x++] = 0x02;
  out[x++] = (unsigned char)z;

  /* if 31st bit is set output a leading zero and decrement count */
  if (z == 5) {
    out[x++] = 0;
    --z;
  }

  /* store values */
  for (y = 0; y < z; y++) {
    out[x++] = (unsigned char)((num >> 24) & 0xFF);
    num <<= 8;
  }

  /* we good */
  *outlen = x;

  return CRYPT_OK;
}

/**
   @file der_encode_utctime.c
   ASN.1 DER, encode a  UTCTIME, Tom St Denis
 */

static const char baseten[] = "0123456789";

#define STORE_V(y)                                        \
  out[x++] = der_ia5_char_encode(baseten[(y / 10) % 10]); \
  out[x++] = der_ia5_char_encode(baseten[y % 10]);

/**
   Encodes a UTC time structure in DER format
   @param utctime      The UTC time structure to encode
   @param out          The destination of the DER encoding of the UTC time
   structure
   @param outlen       [in/out] The length of the DER encoding
   @return CRYPT_OK if successful
 */
int der_encode_utctime(ltc_utctime *utctime, unsigned char *out,
                       unsigned long *outlen) {
  unsigned long x, tmplen;
  int err;

  LTC_ARGCHK(utctime != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  if ((err = der_length_utctime(utctime, &tmplen)) != CRYPT_OK) {
    return err;
  }
  if (tmplen > *outlen) {
    *outlen = tmplen;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* store header */
  out[0] = 0x17;

  /* store values */
  x = 2;
  STORE_V(utctime->YY);
  STORE_V(utctime->MM);
  STORE_V(utctime->DD);
  STORE_V(utctime->hh);
  STORE_V(utctime->mm);
  STORE_V(utctime->ss);

  if (utctime->off_mm || utctime->off_hh) {
    out[x++] = der_ia5_char_encode(utctime->off_dir ? '-' : '+');
    STORE_V(utctime->off_hh);
    STORE_V(utctime->off_mm);
  } else {
    out[x++] = der_ia5_char_encode('Z');
  }

  /* store length */
  out[1] = (unsigned char)(x - 2);

  /* all good let's return */
  *outlen = x;
  return CRYPT_OK;
}

/**
   Store an UTF8 STRING
   @param in       The array of UTF8 to store (one per wchar_t)
   @param inlen    The number of UTF8 to store
   @param out      [out] The destination for the DER encoded UTF8 STRING
   @param outlen   [in/out] The max size and resulting size of the DER UTF8
   STRING
   @return CRYPT_OK if successful
 */
int der_encode_utf8_string(const wchar_t *in, unsigned long inlen,
                           unsigned char *out, unsigned long *outlen) {
  unsigned long x, y, len;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get the size */
  for (x = len = 0; x < inlen; x++) {
    if ((in[x] < 0) || (in[x] > 0x1FFFF)) {
      return CRYPT_INVALID_ARG;
    }
    len += der_utf8_charsize(in[x]);
  }

  if (len < 128) {
    y = 2 + len;
  } else if (len < 256) {
    y = 3 + len;
  } else if (len < 65536UL) {
    y = 4 + len;
  } else if (len < 16777216UL) {
    y = 5 + len;
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* too big? */
  if (y > *outlen) {
    *outlen = len;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* encode the header+len */
  x = 0;
  out[x++] = 0x0C;
  if (len < 128) {
    out[x++] = (unsigned char)len;
  } else if (len < 256) {
    out[x++] = 0x81;
    out[x++] = (unsigned char)len;
  } else if (len < 65536UL) {
    out[x++] = 0x82;
    out[x++] = (unsigned char)((len >> 8) & 255);
    out[x++] = (unsigned char)(len & 255);
  } else if (len < 16777216UL) {
    out[x++] = 0x83;
    out[x++] = (unsigned char)((len >> 16) & 255);
    out[x++] = (unsigned char)((len >> 8) & 255);
    out[x++] = (unsigned char)(len & 255);
  } else {
    return CRYPT_INVALID_ARG;
  }

  /* store UTF8 */
  for (y = 0; y < inlen; y++) {
    switch (der_utf8_charsize(in[y])) {
      case 1:
        out[x++] = (unsigned char)in[y];
        break;

      case 2:
        out[x++] = 0xC0 | ((in[y] >> 6) & 0x1F);
        out[x++] = 0x80 | (in[y] & 0x3F);
        break;

      case 3:
        out[x++] = 0xE0 | ((in[y] >> 12) & 0x0F);
        out[x++] = 0x80 | ((in[y] >> 6) & 0x3F);
        out[x++] = 0x80 | (in[y] & 0x3F);
        break;

      case 4:
        out[x++] = 0xF0 | ((in[y] >> 18) & 0x07);
        out[x++] = 0x80 | ((in[y] >> 12) & 0x3F);
        out[x++] = 0x80 | ((in[y] >> 6) & 0x3F);
        out[x++] = 0x80 | (in[y] & 0x3F);
        break;
    }
  }

  /* retun length */
  *outlen = x;

  return CRYPT_OK;
}

/**
   Gets length of DER encoding of BIT STRING
   @param nbits  The number of bits in the string to encode
   @param outlen [out] The length of the DER encoding for the given string
   @return CRYPT_OK if successful
 */
int der_length_bit_string(unsigned long nbits, unsigned long *outlen) {
  unsigned long nbytes;

  LTC_ARGCHK(outlen != NULL);

  /* get the number of the bytes */
  nbytes = (nbits >> 3) + ((nbits & 7) ? 1 : 0) + 1;

  if (nbytes < 128) {
    /* 03 LL PP DD DD DD ... */
    *outlen = 2 + nbytes;
  } else if (nbytes < 256) {
    /* 03 81 LL PP DD DD DD ... */
    *outlen = 3 + nbytes;
  } else if (nbytes < 65536) {
    /* 03 82 LL LL PP DD DD DD ... */
    *outlen = 4 + nbytes;
  } else {
    return CRYPT_INVALID_ARG;
  }

  return CRYPT_OK;
}

/**
   Gets length of DER encoding of a BOOLEAN
   @param outlen [out] The length of the DER encoding
   @return CRYPT_OK if successful
 */
int der_length_boolean(unsigned long *outlen) {
  LTC_ARGCHK(outlen != NULL);
  *outlen = 3;
  return CRYPT_OK;
}

static const struct {
  int code, value;
} ia5_table[] = {
    {'\0', 0},  {'\a', 7},  {'\b', 8},  {'\t', 9},  {'\n', 10}, {'\f', 12},
    {'\r', 13}, {' ', 32},  {'!', 33},  {'"', 34},  {'#', 35},  {'$', 36},
    {'%', 37},  {'&', 38},  {'\'', 39}, {'(', 40},  {')', 41},  {'*', 42},
    {'+', 43},  {',', 44},  {'-', 45},  {'.', 46},  {'/', 47},  {'0', 48},
    {'1', 49},  {'2', 50},  {'3', 51},  {'4', 52},  {'5', 53},  {'6', 54},
    {'7', 55},  {'8', 56},  {'9', 57},  {':', 58},  {';', 59},  {'<', 60},
    {'=', 61},  {'>', 62},  {'?', 63},  {'@', 64},  {'A', 65},  {'B', 66},
    {'C', 67},  {'D', 68},  {'E', 69},  {'F', 70},  {'G', 71},  {'H', 72},
    {'I', 73},  {'J', 74},  {'K', 75},  {'L', 76},  {'M', 77},  {'N', 78},
    {'O', 79},  {'P', 80},  {'Q', 81},  {'R', 82},  {'S', 83},  {'T', 84},
    {'U', 85},  {'V', 86},  {'W', 87},  {'X', 88},  {'Y', 89},  {'Z', 90},
    {'[', 91},  {'\\', 92}, {']', 93},  {'^', 94},  {'_', 95},  {'`', 96},
    {'a', 97},  {'b', 98},  {'c', 99},  {'d', 100}, {'e', 101}, {'f', 102},
    {'g', 103}, {'h', 104}, {'i', 105}, {'j', 106}, {'k', 107}, {'l', 108},
    {'m', 109}, {'n', 110}, {'o', 111}, {'p', 112}, {'q', 113}, {'r', 114},
    {'s', 115}, {'t', 116}, {'u', 117}, {'v', 118}, {'w', 119}, {'x', 120},
    {'y', 121}, {'z', 122}, {'{', 123}, {'|', 124}, {'}', 125}, {'~', 126}};

int der_ia5_char_encode(int c) {
  int x;

  for (x = 0; x < (int)(sizeof(ia5_table) / sizeof(ia5_table[0])); x++) {
    if (ia5_table[x].code == c) {
      return ia5_table[x].value;
    }
  }
  return -1;
}

int der_ia5_value_decode(int v) {
  int x;

  for (x = 0; x < (int)(sizeof(ia5_table) / sizeof(ia5_table[0])); x++) {
    if (ia5_table[x].value == v) {
      return ia5_table[x].code;
    }
  }
  return -1;
}

/**
   Gets length of DER encoding of IA5 STRING
   @param octets   The values you want to encode
   @param noctets  The number of octets in the string to encode
   @param outlen   [out] The length of the DER encoding for the given string
   @return CRYPT_OK if successful
 */
int der_length_ia5_string(const unsigned char *octets, unsigned long noctets,
                          unsigned long *outlen) {
  unsigned long x;

  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(octets != NULL);

  /* scan string for validity */
  for (x = 0; x < noctets; x++) {
    if (der_ia5_char_encode(octets[x]) == -1) {
      return CRYPT_INVALID_ARG;
    }
  }

  if (noctets < 128) {
    /* 16 LL DD DD DD ... */
    *outlen = 2 + noctets;
  } else if (noctets < 256) {
    /* 16 81 LL DD DD DD ... */
    *outlen = 3 + noctets;
  } else if (noctets < 65536UL) {
    /* 16 82 LL LL DD DD DD ... */
    *outlen = 4 + noctets;
  } else if (noctets < 16777216UL) {
    /* 16 83 LL LL LL DD DD DD ... */
    *outlen = 5 + noctets;
  } else {
    return CRYPT_INVALID_ARG;
  }

  return CRYPT_OK;
}

/**
   Gets length of DER encoding of num
   @param num    The int to get the size of
   @param outlen [out] The length of the DER encoding for the given integer
   @return CRYPT_OK if successful
 */
int der_length_integer(void *num, unsigned long *outlen) {
  unsigned long z, len;
  int leading_zero;

  LTC_ARGCHK(num != NULL);
  LTC_ARGCHK(outlen != NULL);

  if (mp_cmp_d(num, 0) != LTC_MP_LT) {
    /* positive */

    /* we only need a leading zero if the msb of the first byte is one */
    if (((mp_count_bits(num) & 7) == 0) || (mp_iszero(num) == LTC_MP_YES)) {
      leading_zero = 1;
    } else {
      leading_zero = 0;
    }

    /* size for bignum */
    z = len = leading_zero + mp_unsigned_bin_size(num);
  } else {
    /* it's negative */
    /* find power of 2 that is a multiple of eight and greater than count bits
     */
    leading_zero = 0;
    z = mp_count_bits(num);
    z = z + (8 - (z & 7));
    if (((mp_cnt_lsb(num) + 1) == mp_count_bits(num)) &&
        ((mp_count_bits(num) & 7) == 0))
      --z;
    len = z = z >> 3;
  }

  /* now we need a length */
  if (z < 128) {
    /* short form */
    ++len;
  } else {
    /* long form (relies on z != 0), assumes length bytes < 128 */
    ++len;

    while (z) {
      ++len;
      z >>= 8;
    }
  }

  /* we need a 0x02 to indicate it's INTEGER */
  ++len;

  /* return length */
  *outlen = len;
  return CRYPT_OK;
}

unsigned long der_object_identifier_bits(unsigned long x) {
  unsigned long c;

  x &= 0xFFFFFFFF;
  c = 0;
  while (x) {
    ++c;
    x >>= 1;
  }
  return c;
}

/**
   Gets length of DER encoding of Object Identifier
   @param nwords   The number of OID words
   @param words    The actual OID words to get the size of
   @param outlen   [out] The length of the DER encoding for the given string
   @return CRYPT_OK if successful
 */
int der_length_object_identifier(unsigned long *words, unsigned long nwords,
                                 unsigned long *outlen) {
  unsigned long y, z, t, wordbuf;

  LTC_ARGCHK(words != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* must be >= 2 words */
  if (nwords < 2) {
    return CRYPT_INVALID_ARG;
  }

  /* word1 = 0,1,2,3 and word2 0..39 */
  if ((words[0] > 3) || ((words[0] < 2) && (words[1] > 39))) {
    return CRYPT_INVALID_ARG;
  }

  /* leading word is the first two */
  z = 0;
  wordbuf = words[0] * 40 + words[1];
  for (y = 1; y < nwords; y++) {
    t = der_object_identifier_bits(wordbuf);
    z += t / 7 + ((t % 7) ? 1 : 0) + (wordbuf == 0 ? 1 : 0);
    if (y < nwords - 1) {
      /* grab next word */
      wordbuf = words[y + 1];
    }
  }

  /* now depending on the length our length encoding changes */
  if (z < 128) {
    z += 2;
  } else if (z < 256) {
    z += 3;
  } else if (z < 65536UL) {
    z += 4;
  } else {
    return CRYPT_INVALID_ARG;
  }

  *outlen = z;
  return CRYPT_OK;
}

/**
   Gets length of DER encoding of OCTET STRING
   @param noctets  The number of octets in the string to encode
   @param outlen   [out] The length of the DER encoding for the given string
   @return CRYPT_OK if successful
 */
int der_length_octet_string(unsigned long noctets, unsigned long *outlen) {
  LTC_ARGCHK(outlen != NULL);

  if (noctets < 128) {
    /* 04 LL DD DD DD ... */
    *outlen = 2 + noctets;
  } else if (noctets < 256) {
    /* 04 81 LL DD DD DD ... */
    *outlen = 3 + noctets;
  } else if (noctets < 65536UL) {
    /* 04 82 LL LL DD DD DD ... */
    *outlen = 4 + noctets;
  } else if (noctets < 16777216UL) {
    /* 04 83 LL LL LL DD DD DD ... */
    *outlen = 5 + noctets;
  } else {
    return CRYPT_INVALID_ARG;
  }

  return CRYPT_OK;
}

static const struct {
  int code, value;
} printable_table[] = {
    {' ', 32},  {'\'', 39}, {'(', 40},  {')', 41},  {'+', 43},  {',', 44},
    {'-', 45},  {'.', 46},  {'/', 47},  {'0', 48},  {'1', 49},  {'2', 50},
    {'3', 51},  {'4', 52},  {'5', 53},  {'6', 54},  {'7', 55},  {'8', 56},
    {'9', 57},  {':', 58},  {'=', 61},  {'?', 63},  {'A', 65},  {'B', 66},
    {'C', 67},  {'D', 68},  {'E', 69},  {'F', 70},  {'G', 71},  {'H', 72},
    {'I', 73},  {'J', 74},  {'K', 75},  {'L', 76},  {'M', 77},  {'N', 78},
    {'O', 79},  {'P', 80},  {'Q', 81},  {'R', 82},  {'S', 83},  {'T', 84},
    {'U', 85},  {'V', 86},  {'W', 87},  {'X', 88},  {'Y', 89},  {'Z', 90},
    {'a', 97},  {'b', 98},  {'c', 99},  {'d', 100}, {'e', 101}, {'f', 102},
    {'g', 103}, {'h', 104}, {'i', 105}, {'j', 106}, {'k', 107}, {'l', 108},
    {'m', 109}, {'n', 110}, {'o', 111}, {'p', 112}, {'q', 113}, {'r', 114},
    {'s', 115}, {'t', 116}, {'u', 117}, {'v', 118}, {'w', 119}, {'x', 120},
    {'y', 121}, {'z', 122},
};

int der_printable_char_encode(int c) {
  int x;

  for (x = 0; x < (int)(sizeof(printable_table) / sizeof(printable_table[0]));
       x++) {
    if (printable_table[x].code == c) {
      return printable_table[x].value;
    }
  }
  return -1;
}

int der_printable_value_decode(int v) {
  int x;

  for (x = 0; x < (int)(sizeof(printable_table) / sizeof(printable_table[0]));
       x++) {
    if (printable_table[x].value == v) {
      return printable_table[x].code;
    }
  }
  return -1;
}

/**
   Gets length of DER encoding of Printable STRING
   @param octets   The values you want to encode
   @param noctets  The number of octets in the string to encode
   @param outlen   [out] The length of the DER encoding for the given string
   @return CRYPT_OK if successful
 */
int der_length_printable_string(const unsigned char *octets,
                                unsigned long noctets, unsigned long *outlen) {
  unsigned long x;

  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(octets != NULL);

  /* scan string for validity */
  for (x = 0; x < noctets; x++) {
    if (der_printable_char_encode(octets[x]) == -1) {
      return CRYPT_INVALID_ARG;
    }
  }

  if (noctets < 128) {
    /* 16 LL DD DD DD ... */
    *outlen = 2 + noctets;
  } else if (noctets < 256) {
    /* 16 81 LL DD DD DD ... */
    *outlen = 3 + noctets;
  } else if (noctets < 65536UL) {
    /* 16 82 LL LL DD DD DD ... */
    *outlen = 4 + noctets;
  } else if (noctets < 16777216UL) {
    /* 16 83 LL LL LL DD DD DD ... */
    *outlen = 5 + noctets;
  } else {
    return CRYPT_INVALID_ARG;
  }

  return CRYPT_OK;
}

/**
   Get the length of a DER sequence
   @param list   The sequences of items in the SEQUENCE
   @param inlen  The number of items
   @param outlen [out] The length required in octets to store it
   @return CRYPT_OK on success
 */
int der_length_sequence(ltc_asn1_list *list, unsigned long inlen,
                        unsigned long *outlen) {
  int err, type;
  unsigned long size, x, y, i;
  void *data;

  LTC_ARGCHK(list != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* get size of output that will be required */
  y = 0;
  for (i = 0; i < inlen; i++) {
    type = list[i].type;
    size = list[i].size;
    data = list[i].data;

    if (type == LTC_ASN1_EOL) {
      break;
    }

    switch (type) {
      case LTC_ASN1_BOOLEAN:
        if ((err = der_length_boolean(&x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_INTEGER:
        if ((err = der_length_integer(data, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_SHORT_INTEGER:
        if ((err = der_length_short_integer(*((unsigned long *)data), &x)) !=
            CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_BIT_STRING:
        if ((err = der_length_bit_string(size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_OCTET_STRING:
        if ((err = der_length_octet_string(size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_NULL:
        y += 2;
        break;

      case LTC_ASN1_OBJECT_IDENTIFIER:
        if ((err = der_length_object_identifier(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_IA5_STRING:
        if ((err = der_length_ia5_string(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_PRINTABLE_STRING:
        if ((err = der_length_printable_string(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_UTCTIME:
        if ((err = der_length_utctime(data, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_UTF8_STRING:
        if ((err = der_length_utf8_string(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
      case LTC_ASN1_SEQUENCE:
        if ((err = der_length_sequence(data, size, &x)) != CRYPT_OK) {
          goto LBL_ERR;
        }
        y += x;
        break;

      default:
        err = CRYPT_INVALID_ARG;
        goto LBL_ERR;
    }
  }

  /* calc header size */

  if (y < 128) {
    y += 2;
  } else if (y < 256) {
    /* 0x30 0x81 LL */
    y += 3;
  } else if (y < 65536UL) {
    /* 0x30 0x82 LL LL */
    y += 4;
  } else if (y < 16777216UL) {
    /* 0x30 0x83 LL LL LL */
    y += 5;
  } else {
    err = CRYPT_INVALID_ARG;
    goto LBL_ERR;
  }

  /* store size */
  *outlen = y;
  err = CRYPT_OK;

LBL_ERR:
  return err;
}

/**
   Gets length of DER encoding of num
   @param num    The integer to get the size of
   @param outlen [out] The length of the DER encoding for the given integer
   @return CRYPT_OK if successful
 */
int der_length_short_integer(unsigned long num, unsigned long *outlen) {
  unsigned long z, y, len;

  LTC_ARGCHK(outlen != NULL);

  /* force to 32 bits */
  num &= 0xFFFFFFFFUL;

  /* get the number of bytes */
  z = 0;
  y = num;
  while (y) {
    ++z;
    y >>= 8;
  }

  /* handle zero */
  if (z == 0) {
    z = 1;
  }

  /* we need a 0x02 to indicate it's INTEGER */
  len = 1;

  /* length byte */
  ++len;

  /* bytes in value */
  len += z;

  /* see if msb is set */
  len += (num & (1UL << ((z << 3) - 1))) ? 1 : 0;

  /* return length */
  *outlen = len;

  return CRYPT_OK;
}

/**
   Gets length of DER encoding of UTCTIME
   @param utctime      The UTC time structure to get the size of
   @param outlen [out] The length of the DER encoding
   @return CRYPT_OK if successful
 */
int der_length_utctime(ltc_utctime *utctime, unsigned long *outlen) {
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(utctime != NULL);

  if ((utctime->off_hh == 0) && (utctime->off_mm == 0)) {
    /* we encode as YYMMDDhhmmssZ */
    *outlen = 2 + 13;
  } else {
    /* we encode as YYMMDDhhmmss{+|-}hh'mm' */
    *outlen = 2 + 17;
  }

  return CRYPT_OK;
}

/** Return the size in bytes of a UTF-8 character
   @param c   The UTF-8 character to measure
   @return    The size in bytes
 */
unsigned long der_utf8_charsize(const wchar_t c) {
  if (c <= 0x7F) {
    return 1;
  } else if (c <= 0x7FF) {
    return 2;
  } else if (c <= 0xFFFF) {
    return 3;
  } else {
    return 4;
  }
}

/**
   Gets length of DER encoding of UTF8 STRING
   @param in       The characters to measure the length of
   @param noctets  The number of octets in the string to encode
   @param outlen   [out] The length of the DER encoding for the given string
   @return CRYPT_OK if successful
 */
int der_length_utf8_string(const wchar_t *in, unsigned long noctets,
                           unsigned long *outlen) {
  unsigned long x, len;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(outlen != NULL);

  len = 0;
  for (x = 0; x < noctets; x++) {
    if ((in[x] < 0) || (in[x] > 0x10FFFF)) {
      return CRYPT_INVALID_ARG;
    }
    len += der_utf8_charsize(in[x]);
  }

  if (len < 128) {
    /* 0C LL DD DD DD ... */
    *outlen = 2 + len;
  } else if (len < 256) {
    /* 0C 81 LL DD DD DD ... */
    *outlen = 3 + len;
  } else if (len < 65536UL) {
    /* 0C 82 LL LL DD DD DD ... */
    *outlen = 4 + len;
  } else if (len < 16777216UL) {
    /* 0C 83 LL LL LL DD DD DD ... */
    *outlen = 5 + len;
  } else {
    return CRYPT_INVALID_ARG;
  }

  return CRYPT_OK;
}

/**
   Free memory allocated by der_decode_sequence_flexi()
   @param in     The list to free
 */
void der_sequence_free(ltc_asn1_list *in) {
  ltc_asn1_list *l;

  /* walk to the start of the chain */
  while (in->prev != NULL || in->parent != NULL) {
    if (in->parent != NULL) {
      in = in->parent;
    } else {
      in = in->prev;
    }
  }

  /* now walk the list and free stuff */
  while (in != NULL) {
    /* is there a child? */
    if (in->child) {
      /* disconnect */
      in->child->parent = NULL;
      der_sequence_free(in->child);
    }

    switch (in->type) {
      case LTC_ASN1_SET:
      case LTC_ASN1_SETOF:
      case LTC_ASN1_SEQUENCE:
        break;

      case LTC_ASN1_INTEGER:
        if (in->data != NULL) {
          mp_clear(in->data);
        }
        break;

      default:
        if (in->data != NULL) {
          XFREE(in->data);
        }
    }

    /* move to next and free current */
    l = in->next;
    XFREE(in);
    in = l;
  }
}

/* This holds the key settings.  ***MUST*** be organized by size from smallest
 * to largest. */
const ltc_ecc_set_type ltc_ecc_sets[] = {
#ifdef ECC112
    {14, "SECP112R1", "DB7C2ABF62E35E668076BEAD208B",
     "659EF8BA043916EEDE8911702B22", "DB7C2ABF62E35E7628DFAC6561C5",
     "09487239995A5EE76B55F9C2F098", "A89CE5AF8724C0A23E0E0FF77500"},
#endif
#ifdef ECC128
    {
        16,
        "SECP128R1",
        "FFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFF",
        "E87579C11079F43DD824993C2CEE5ED3",
        "FFFFFFFE0000000075A30D1B9038A115",
        "161FF7528B899B2D0C28607CA52C5B86",
        "CF5AC8395BAFEB13C02DA292DDED7A83",
    },
#endif
#ifdef ECC160
    {
        20,
        "SECP160R1",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF",
        "1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45",
        "0100000000000000000001F4C8F927AED3CA752257",
        "4A96B5688EF573284664698968C38BB913CBFC82",
        "23A628553168947D59DCC912042351377AC5FB32",
    },
#endif
#ifdef ECC192
    {
        24,
        "ECC-192",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF",
        "64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1",
        "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831",
        "188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012",
        "7192B95FFC8DA78631011ED6B24CDD573F977A11E794811",
    },
#endif
#ifdef ECC224
    {
        28,
        "ECC-224",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",
        "B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D",
        "B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21",
        "BD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34",
    },
#endif
#ifdef ECC256
    {
        32,
        "ECC-256",
        "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF",
        "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B",
        "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551",
        "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296",
        "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5",
    },
#endif
#ifdef ECC384
    {
        48,
        "ECC-384",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFF"
        "FF0000000000000000FFFFFFFF",
        "B3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC65639"
        "8D8A2ED19D2A85C8EDD3EC2AEF",
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0D"
        "B248B0A77AECEC196ACCC52973",
        "AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F2"
        "5DBF55296C3A545E3872760AB7",
        "3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1"
        "CE1D7E819D7A431D7C90EA0E5F",
    },
#endif
#ifdef ECC521
    {
        66,
        "ECC-521",
        "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
        "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
        "51953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E15619"
        "3951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00",
        "1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA518"
        "68783BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386409",
        "C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B"
        "5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66",
        "11839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97E"
        "E72995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650",
    },
#endif
    {0, NULL, NULL, NULL, NULL, NULL, NULL}};

/* Implements ECC over Z/pZ for curve y^2 = x^3 - 3x + b
 *
 * All curves taken from NIST recommendation paper of July 1999
 * Available at http://csrc.nist.gov/cryptval/dss.htm
 */

/**
   @file ecc_ansi_x963_export.c
   ECC Crypto, Tom St Denis
 */

/** ECC X9.63 (Sec. 4.3.6) uncompressed export
   @param key     Key to export
   @param out     [out] destination of export
   @param outlen  [in/out]  Length of destination and final output size
   Return CRYPT_OK on success
 */
int ecc_ansi_x963_export(ecc_key *key, unsigned char *out,
                         unsigned long *outlen) {
  unsigned char buf[ECC_BUF_SIZE];
  unsigned long numlen;

  LTC_ARGCHK(key != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  if (ltc_ecc_is_valid_idx(key->idx) == 0) {
    return CRYPT_INVALID_ARG;
  }
  numlen = key->dp->size;

  if (*outlen < (1 + 2 * numlen)) {
    *outlen = 1 + 2 * numlen;
    return CRYPT_BUFFER_OVERFLOW;
  }

  /* store byte 0x04 */
  out[0] = 0x04;

  /* pad and store x */
  zeromem(buf, sizeof(buf));
  mp_to_unsigned_bin(key->pubkey.x,
                     buf + (numlen - mp_unsigned_bin_size(key->pubkey.x)));
  XMEMCPY(out + 1, buf, numlen);

  /* pad and store y */
  zeromem(buf, sizeof(buf));
  mp_to_unsigned_bin(key->pubkey.y,
                     buf + (numlen - mp_unsigned_bin_size(key->pubkey.y)));
  XMEMCPY(out + 1 + numlen, buf, numlen);

  *outlen = 1 + 2 * numlen;
  return CRYPT_OK;
}

/* Implements ECC over Z/pZ for curve y^2 = x^3 - 3x + b
 *
 * All curves taken from NIST recommendation paper of July 1999
 * Available at http://csrc.nist.gov/cryptval/dss.htm
 */

/**
   @file ecc_ansi_x963_import.c
   ECC Crypto, Tom St Denis
 */

int ecc_ansi_x963_import_ex(const unsigned char *in, unsigned long inlen,
                            ecc_key *key, const ltc_ecc_set_type *dp) {
  int x, err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(key != NULL);

  /* must be odd */
  if ((inlen & 1) == 0) {
    return CRYPT_INVALID_ARG;
  }

  /* init key */
  if (mp_init_multi(&key->pubkey.x, &key->pubkey.y, &key->pubkey.z, &key->k,
                    NULL) != CRYPT_OK) {
    return CRYPT_MEM;
  }

  /* check for 4, 6 or 7 */
  if ((in[0] != 4) && (in[0] != 6) && (in[0] != 7)) {
    err = CRYPT_INVALID_PACKET;
    goto error;
  }

  /* read data */
  if ((err = mp_read_unsigned_bin(key->pubkey.x, (unsigned char *)in + 1,
                                  (inlen - 1) >> 1)) != CRYPT_OK) {
    goto error;
  }

  if ((err = mp_read_unsigned_bin(key->pubkey.y,
                                  (unsigned char *)in + 1 + ((inlen - 1) >> 1),
                                  (inlen - 1) >> 1)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_set(key->pubkey.z, 1)) != CRYPT_OK) {
    goto error;
  }

  if (dp == NULL) {
    /* determine the idx */
    for (x = 0; ltc_ecc_sets[x].size != 0; x++) {
      if ((unsigned)ltc_ecc_sets[x].size >= ((inlen - 1) >> 1)) {
        break;
      }
    }
    if (ltc_ecc_sets[x].size == 0) {
      err = CRYPT_INVALID_PACKET;
      goto error;
    }
    /* set the idx */
    key->idx = x;
    key->dp = &ltc_ecc_sets[x];
  } else {
    if (((inlen - 1) >> 1) != (unsigned long)dp->size) {
      err = CRYPT_INVALID_PACKET;
      goto error;
    }
    key->idx = -1;
    key->dp = dp;
  }
  key->type = PK_PUBLIC;

  /* we're done */
  return CRYPT_OK;
error:
  mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
  return err;
}

/**
   Free an ECC key from memory
   @param key   The key you wish to free
 */
void ecc_free(ecc_key *key) {
  LTC_ARGCHKVD(key != NULL);
  mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
}

int ecc_make_key_ex(prng_state *prng, int wprng, ecc_key *key,
                    const ltc_ecc_set_type *dp) {
  int err;
  ecc_point *base;
  void *prime, *order;
  unsigned char *buf;
  int keysize;

  LTC_ARGCHK(key != NULL);
  LTC_ARGCHK(ltc_mp.name != NULL);
  LTC_ARGCHK(dp != NULL);

  /* good prng? */
  if ((err = prng_is_valid(wprng)) != CRYPT_OK) {
    return err;
  }

  key->idx = -1;
  key->dp = dp;
  keysize = dp->size;

  /* allocate ram */
  base = NULL;
  buf = XMALLOC(ECC_MAXSIZE);
  if (buf == NULL) {
    return CRYPT_MEM;
  }

  /* make up random string */
  if (prng_descriptor[wprng].read(buf, (unsigned long)keysize, prng) !=
      (unsigned long)keysize) {
    err = CRYPT_ERROR_READPRNG;
    goto ERR_BUF;
  }

  /* setup the key variables */
  if ((err = mp_init_multi(&key->pubkey.x, &key->pubkey.y, &key->pubkey.z,
                           &key->k, &prime, &order, NULL)) != CRYPT_OK) {
    goto ERR_BUF;
  }
  base = ltc_ecc_new_point();
  if (base == NULL) {
    err = CRYPT_MEM;
    goto errkey;
  }

  /* read in the specs for this key */
  if ((err = mp_read_radix(prime, (char *)key->dp->prime, 16)) != CRYPT_OK) {
    goto errkey;
  }
  if ((err = mp_read_radix(order, (char *)key->dp->order, 16)) != CRYPT_OK) {
    goto errkey;
  }
  if ((err = mp_read_radix(base->x, (char *)key->dp->Gx, 16)) != CRYPT_OK) {
    goto errkey;
  }
  if ((err = mp_read_radix(base->y, (char *)key->dp->Gy, 16)) != CRYPT_OK) {
    goto errkey;
  }
  if ((err = mp_set(base->z, 1)) != CRYPT_OK) {
    goto errkey;
  }
  if ((err = mp_read_unsigned_bin(key->k, (unsigned char *)buf, keysize)) !=
      CRYPT_OK) {
    goto errkey;
  }

  /* the key should be smaller than the order of base point */
  if (mp_cmp(key->k, order) != LTC_MP_LT) {
    if ((err = mp_mod(key->k, order, key->k)) != CRYPT_OK) {
      goto errkey;
    }
  }
  /* make the public key */
  if ((err = ltc_mp.ecc_ptmul(key->k, base, &key->pubkey, prime, 1)) !=
      CRYPT_OK) {
    goto errkey;
  }
  key->type = PK_PRIVATE;

  /* free up ram */
  err = CRYPT_OK;
  goto cleanup;
errkey:
  mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
cleanup:
  ltc_ecc_del_point(base);
  mp_clear_multi(prime, order, NULL);
ERR_BUF:
  XFREE(buf);
  return err;
}

/**
   Create an ECC shared secret between two keys
   @param private_key      The private ECC key
   @param public_key       The public key
   @param out              [out] Destination of the shared secret (Conforms to
   EC-DH from ANSI X9.63)
   @param outlen           [in/out] The max size and resulting size of the
   shared secret
   @return CRYPT_OK if successful
 */
int ecc_shared_secret(ecc_key *private_key, ecc_key *public_key,
                      unsigned char *out, unsigned long *outlen) {
  unsigned long x;
  ecc_point *result;
  void *prime;
  int err;

  LTC_ARGCHK(private_key != NULL);
  LTC_ARGCHK(public_key != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* type valid? */
  if (private_key->type != PK_PRIVATE) {
    return CRYPT_PK_NOT_PRIVATE;
  }

  if ((ltc_ecc_is_valid_idx(private_key->idx) == 0) ||
      (ltc_ecc_is_valid_idx(public_key->idx) == 0)) {
    return CRYPT_INVALID_ARG;
  }

  if (XSTRCMP(private_key->dp->name, public_key->dp->name) != 0) {
    return CRYPT_PK_TYPE_MISMATCH;
  }

  /* make new point */
  result = ltc_ecc_new_point();
  if (result == NULL) {
    return CRYPT_MEM;
  }

  if ((err = mp_init(&prime)) != CRYPT_OK) {
    ltc_ecc_del_point(result);
    return err;
  }

  if ((err = mp_read_radix(prime, (char *)private_key->dp->prime, 16)) !=
      CRYPT_OK) {
    goto done;
  }
  if ((err = ltc_mp.ecc_ptmul(private_key->k, &public_key->pubkey, result,
                              prime, 1)) != CRYPT_OK) {
    goto done;
  }

  x = (unsigned long)mp_unsigned_bin_size(prime);
  if (*outlen < x) {
    *outlen = x;
    err = CRYPT_BUFFER_OVERFLOW;
    goto done;
  }
  zeromem(out, x);
  if ((err = mp_to_unsigned_bin(result->x,
                                out + (x - mp_unsigned_bin_size(result->x)))) !=
      CRYPT_OK) {
    goto done;
  }

  err = CRYPT_OK;
  *outlen = x;
done:
  mp_clear(prime);
  ltc_ecc_del_point(result);
  return err;
}

/**
   Sign a message digest
   @param in        The message digest to sign
   @param inlen     The length of the digest
   @param out       [out] The destination for the signature
   @param outlen    [in/out] The max size and resulting size of the signature
   @param prng      An active PRNG state
   @param wprng     The index of the PRNG you wish to use
   @param key       A private ECC key
   @return CRYPT_OK if successful
 */
int ecc_sign_hash(const unsigned char *in, unsigned long inlen,
                  unsigned char *out, unsigned long *outlen, prng_state *prng,
                  int wprng, ecc_key *key) {
  ecc_key pubkey;
  void *r, *s, *e, *p;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(key != NULL);

  /* is this a private key? */
  if (key->type != PK_PRIVATE) {
    return CRYPT_PK_NOT_PRIVATE;
  }

  /* is the IDX valid ?  */
  if (ltc_ecc_is_valid_idx(key->idx) != 1) {
    return CRYPT_PK_INVALID_TYPE;
  }

  if ((err = prng_is_valid(wprng)) != CRYPT_OK) {
    return err;
  }

  /* get the hash and load it as a bignum into 'e' */
  /* init the bignums */
  if ((err = mp_init_multi(&r, &s, &p, &e, NULL)) != CRYPT_OK) {
    return err;
  }
  if ((err = mp_read_radix(p, (char *)key->dp->order, 16)) != CRYPT_OK) {
    goto errnokey;
  }
  if ((err = mp_read_unsigned_bin(e, (unsigned char *)in, (int)inlen)) !=
      CRYPT_OK) {
    goto errnokey;
  }

  /* make up a key and export the public copy */
  for (;;) {
    if ((err = ecc_make_key_ex(prng, wprng, &pubkey, key->dp)) != CRYPT_OK) {
      goto errnokey;
    }

    /* find r = x1 mod n */
    if ((err = mp_mod(pubkey.pubkey.x, p, r)) != CRYPT_OK) {
      goto error;
    }

    if (mp_iszero(r) == LTC_MP_YES) {
      ecc_free(&pubkey);
    } else {
      /* find s = (e + xr)/k */
      if ((err = mp_invmod(pubkey.k, p, pubkey.k)) != CRYPT_OK) {
        goto error;
      } /* k = 1/k */
      if ((err = mp_mulmod(key->k, r, p, s)) != CRYPT_OK) {
        goto error;
      } /* s = xr */
      if ((err = mp_add(e, s, s)) != CRYPT_OK) {
        goto error;
      } /* s = e +  xr */
      if ((err = mp_mod(s, p, s)) != CRYPT_OK) {
        goto error;
      } /* s = e +  xr */
      if ((err = mp_mulmod(s, pubkey.k, p, s)) != CRYPT_OK) {
        goto error;
      } /* s = (e + xr)/k */
      ecc_free(&pubkey);
      if (mp_iszero(s) == LTC_MP_NO) {
        break;
      }
    }
  }

  /* store as SEQUENCE { r, s -- integer } */
  err = der_encode_sequence_multi(out, outlen, LTC_ASN1_INTEGER, 1UL, r,
                                  LTC_ASN1_INTEGER, 1UL, s, LTC_ASN1_EOL, 0UL,
                                  NULL);
  goto errnokey;
error:
  ecc_free(&pubkey);
errnokey:
  mp_clear_multi(r, s, p, e, NULL);
  return err;
}

/**
   Verify an ECC signature
   @param sig         The signature to verify
   @param siglen      The length of the signature (octets)
   @param hash        The hash (message digest) that was signed
   @param hashlen     The length of the hash (octets)
   @param stat        Result of signature, 1==valid, 0==invalid
   @param key         The corresponding public ECC key
   @return CRYPT_OK if successful (even if the signature is not valid)
 */
int ecc_verify_hash(const unsigned char *sig, unsigned long siglen,
                    const unsigned char *hash, unsigned long hashlen, int *stat,
                    ecc_key *key) {
  ecc_point *mG, *mQ;
  void *r, *s, *v, *w, *u1, *u2, *e, *p, *m;
  void *mp;
  int err;

  LTC_ARGCHK(sig != NULL);
  LTC_ARGCHK(hash != NULL);
  LTC_ARGCHK(stat != NULL);
  LTC_ARGCHK(key != NULL);

  /* default to invalid signature */
  *stat = 0;
  mp = NULL;

  /* is the IDX valid ?  */
  if (ltc_ecc_is_valid_idx(key->idx) != 1) {
    return CRYPT_PK_INVALID_TYPE;
  }

  /* allocate ints */
  if ((err = mp_init_multi(&r, &s, &v, &w, &u1, &u2, &p, &e, &m, NULL)) !=
      CRYPT_OK) {
    return CRYPT_MEM;
  }

  /* allocate points */
  mG = ltc_ecc_new_point();
  mQ = ltc_ecc_new_point();
  if ((mQ == NULL) || (mG == NULL)) {
    err = CRYPT_MEM;
    goto error;
  }

  /* parse header */
  if ((err = der_decode_sequence_multi(sig, siglen, LTC_ASN1_INTEGER, 1UL, r,
                                       LTC_ASN1_INTEGER, 1UL, s, LTC_ASN1_EOL,
                                       0UL, NULL)) != CRYPT_OK) {
    goto error;
  }

  /* get the order */
  if ((err = mp_read_radix(p, (char *)key->dp->order, 16)) != CRYPT_OK) {
    goto error;
  }

  /* get the modulus */
  if ((err = mp_read_radix(m, (char *)key->dp->prime, 16)) != CRYPT_OK) {
    goto error;
  }

  /* check for zero */
  if (mp_iszero(r) || mp_iszero(s) || (mp_cmp(r, p) != LTC_MP_LT) ||
      (mp_cmp(s, p) != LTC_MP_LT)) {
    err = CRYPT_INVALID_PACKET;
    goto error;
  }

  /* read hash */
  if ((err = mp_read_unsigned_bin(e, (unsigned char *)hash, (int)hashlen)) !=
      CRYPT_OK) {
    goto error;
  }

  /*  w  = s^-1 mod n */
  if ((err = mp_invmod(s, p, w)) != CRYPT_OK) {
    goto error;
  }

  /* u1 = ew */
  if ((err = mp_mulmod(e, w, p, u1)) != CRYPT_OK) {
    goto error;
  }

  /* u2 = rw */
  if ((err = mp_mulmod(r, w, p, u2)) != CRYPT_OK) {
    goto error;
  }

  /* find mG and mQ */
  if ((err = mp_read_radix(mG->x, (char *)key->dp->Gx, 16)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_read_radix(mG->y, (char *)key->dp->Gy, 16)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_set(mG->z, 1)) != CRYPT_OK) {
    goto error;
  }

  if ((err = mp_copy(key->pubkey.x, mQ->x)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_copy(key->pubkey.y, mQ->y)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_copy(key->pubkey.z, mQ->z)) != CRYPT_OK) {
    goto error;
  }

  /* compute u1*mG + u2*mQ = mG */
  if (ltc_mp.ecc_mul2add == NULL) {
    if ((err = ltc_mp.ecc_ptmul(u1, mG, mG, m, 0)) != CRYPT_OK) {
      goto error;
    }
    if ((err = ltc_mp.ecc_ptmul(u2, mQ, mQ, m, 0)) != CRYPT_OK) {
      goto error;
    }

    /* find the montgomery mp */
    if ((err = mp_montgomery_setup(m, &mp)) != CRYPT_OK) {
      goto error;
    }

    /* add them */
    if ((err = ltc_mp.ecc_ptadd(mQ, mG, mG, m, mp)) != CRYPT_OK) {
      goto error;
    }

    /* reduce */
    if ((err = ltc_mp.ecc_map(mG, m, mp)) != CRYPT_OK) {
      goto error;
    }
  } else {
    /* use Shamir's trick to compute u1*mG + u2*mQ using half of the doubles */
    if ((err = ltc_mp.ecc_mul2add(mG, u1, mQ, u2, mG, m)) != CRYPT_OK) {
      goto error;
    }
  }

  /* v = X_x1 mod n */
  if ((err = mp_mod(mG->x, p, v)) != CRYPT_OK) {
    goto error;
  }

  /* does v == r */
  if (mp_cmp(v, r) == LTC_MP_EQ) {
    *stat = 1;
  }

  /* clear up and return */
  err = CRYPT_OK;
error:
  ltc_ecc_del_point(mG);
  ltc_ecc_del_point(mQ);
  mp_clear_multi(r, s, v, w, u1, u2, p, e, m, NULL);
  if (mp != NULL) {
    mp_montgomery_free(mp);
  }
  return err;
}

/**
   Hash a block of memory and store the digest.
   @param hash   The index of the hash you wish to use
   @param in     The data you wish to hash
   @param inlen  The length of the data to hash (octets)
   @param out    [out] Where to store the digest
   @param outlen [in/out] Max size and resulting size of the digest
   @return CRYPT_OK if successful
 */
int hash_memory(int hash, const unsigned char *in, unsigned long inlen,
                unsigned char *out, unsigned long *outlen) {
  hash_state *md;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  if ((err = hash_is_valid(hash)) != CRYPT_OK) {
    return err;
  }

  if (*outlen < hash_descriptor[hash].hashsize) {
    *outlen = hash_descriptor[hash].hashsize;
    return CRYPT_BUFFER_OVERFLOW;
  }

  md = XMALLOC(sizeof(hash_state));
  if (md == NULL) {
    return CRYPT_MEM;
  }

  if ((err = hash_descriptor[hash].init(md)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash].process(md, in, inlen)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  err = hash_descriptor[hash].done(md, out);
  *outlen = hash_descriptor[hash].hashsize;
LBL_ERR:
  XFREE(md);

  return err;
}

/** Returns whether an ECC idx is valid or not
   @param n   The idx number to check
   @return 1 if valid, 0 if not
 */
int ltc_ecc_is_valid_idx(int n) {
  int x;

  for (x = 0; ltc_ecc_sets[x].size != 0; x++)
    ;
  /* -1 is a valid index --- indicating that the domain params were supplied by
   * the user */
  if ((n >= -1) && (n < x)) {
    return 1;
  }
  return 0;
}

/**
   Map a projective jacbobian point back to affine space
   @param P        [in/out] The point to map
   @param modulus  The modulus of the field the ECC curve is in
   @param mp       The "b" value from montgomery_setup()
   @return CRYPT_OK on success
 */
int ltc_ecc_map(ecc_point *P, void *modulus, void *mp) {
  void *t1, *t2;
  int err;

  LTC_ARGCHK(P != NULL);
  LTC_ARGCHK(modulus != NULL);
  LTC_ARGCHK(mp != NULL);

  if ((err = mp_init_multi(&t1, &t2, NULL)) != CRYPT_OK) {
    return CRYPT_MEM;
  }

  /* first map z back to normal */
  if ((err = mp_montgomery_reduce(P->z, modulus, mp)) != CRYPT_OK) {
    goto done;
  }

  /* get 1/z */
  if ((err = mp_invmod(P->z, modulus, t1)) != CRYPT_OK) {
    goto done;
  }

  /* get 1/z^2 and 1/z^3 */
  if ((err = mp_sqr(t1, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_mod(t2, modulus, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_mul(t1, t2, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_mod(t1, modulus, t1)) != CRYPT_OK) {
    goto done;
  }

  /* multiply against x/y */
  if ((err = mp_mul(P->x, t2, P->x)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(P->x, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_mul(P->y, t1, P->y)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(P->y, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_set(P->z, 1)) != CRYPT_OK) {
    goto done;
  }

  err = CRYPT_OK;
done:
  mp_clear_multi(t1, t2, NULL);
  return err;
}

/** Computes kA*A + kB*B = C using Shamir's Trick
   @param A        First point to multiply
   @param kA       What to multiple A by
   @param B        Second point to multiply
   @param kB       What to multiple B by
   @param C        [out] Destination point (can overlap with A or B
   @param modulus  Modulus for curve
   @return CRYPT_OK on success
 */
int ltc_ecc_mul2add(ecc_point *A, void *kA, ecc_point *B, void *kB,
                    ecc_point *C, void *modulus) {
  ecc_point *precomp[16];
  unsigned bitbufA, bitbufB, lenA, lenB, len, x, y, nA, nB, nibble;
  unsigned char *tA, *tB;
  int err, first;
  void *mp, *mu;

  /* argchks */
  LTC_ARGCHK(A != NULL);
  LTC_ARGCHK(B != NULL);
  LTC_ARGCHK(C != NULL);
  LTC_ARGCHK(kA != NULL);
  LTC_ARGCHK(kB != NULL);
  LTC_ARGCHK(modulus != NULL);

  /* allocate memory */
  tA = XCALLOC(1, ECC_BUF_SIZE);
  if (tA == NULL) {
    return CRYPT_MEM;
  }
  tB = XCALLOC(1, ECC_BUF_SIZE);
  if (tB == NULL) {
    XFREE(tA);
    return CRYPT_MEM;
  }

  /* get sizes */
  lenA = mp_unsigned_bin_size(kA);
  lenB = mp_unsigned_bin_size(kB);
  len = MAX(lenA, lenB);

  /* sanity check */
  if ((lenA > ECC_BUF_SIZE) || (lenB > ECC_BUF_SIZE)) {
    err = CRYPT_INVALID_ARG;
    goto ERR_T;
  }

  /* extract and justify kA */
  mp_to_unsigned_bin(kA, (len - lenA) + tA);

  /* extract and justify kB */
  mp_to_unsigned_bin(kB, (len - lenB) + tB);

  /* allocate the table */
  for (x = 0; x < 16; x++) {
    precomp[x] = ltc_ecc_new_point();
    if (precomp[x] == NULL) {
      for (y = 0; y < x; ++y) {
        ltc_ecc_del_point(precomp[y]);
      }
      err = CRYPT_MEM;
      goto ERR_T;
    }
  }

  /* init montgomery reduction */
  if ((err = mp_montgomery_setup(modulus, &mp)) != CRYPT_OK) {
    goto ERR_P;
  }
  if ((err = mp_init(&mu)) != CRYPT_OK) {
    goto ERR_MP;
  }
  if ((err = mp_montgomery_normalization(mu, modulus)) != CRYPT_OK) {
    goto ERR_MU;
  }

  /* copy ones ... */
  if ((err = mp_mulmod(A->x, mu, modulus, precomp[1]->x)) != CRYPT_OK) {
    goto ERR_MU;
  }
  if ((err = mp_mulmod(A->y, mu, modulus, precomp[1]->y)) != CRYPT_OK) {
    goto ERR_MU;
  }
  if ((err = mp_mulmod(A->z, mu, modulus, precomp[1]->z)) != CRYPT_OK) {
    goto ERR_MU;
  }

  if ((err = mp_mulmod(B->x, mu, modulus, precomp[1 << 2]->x)) != CRYPT_OK) {
    goto ERR_MU;
  }
  if ((err = mp_mulmod(B->y, mu, modulus, precomp[1 << 2]->y)) != CRYPT_OK) {
    goto ERR_MU;
  }
  if ((err = mp_mulmod(B->z, mu, modulus, precomp[1 << 2]->z)) != CRYPT_OK) {
    goto ERR_MU;
  }

  /* precomp [i,0](A + B) table */
  if ((err = ltc_mp.ecc_ptdbl(precomp[1], precomp[2], modulus, mp)) !=
      CRYPT_OK) {
    goto ERR_MU;
  }
  if ((err = ltc_mp.ecc_ptadd(precomp[1], precomp[2], precomp[3], modulus,
                              mp)) != CRYPT_OK) {
    goto ERR_MU;
  }

  /* precomp [0,i](A + B) table */
  if ((err = ltc_mp.ecc_ptdbl(precomp[1 << 2], precomp[2 << 2], modulus, mp)) !=
      CRYPT_OK) {
    goto ERR_MU;
  }
  if ((err = ltc_mp.ecc_ptadd(precomp[1 << 2], precomp[2 << 2], precomp[3 << 2],
                              modulus, mp)) != CRYPT_OK) {
    goto ERR_MU;
  }

  /* precomp [i,j](A + B) table (i != 0, j != 0) */
  for (x = 1; x < 4; x++) {
    for (y = 1; y < 4; y++) {
      if ((err = ltc_mp.ecc_ptadd(precomp[x], precomp[(y << 2)],
                                  precomp[x + (y << 2)], modulus, mp)) !=
          CRYPT_OK) {
        goto ERR_MU;
      }
    }
  }

  nibble = 3;
  first = 1;
  bitbufA = tA[0];
  bitbufB = tB[0];

  /* for every byte of the multiplicands */
  for (x = -1;;) {
    /* grab a nibble */
    if (++nibble == 4) {
      ++x;
      if (x == len) break;
      bitbufA = tA[x];
      bitbufB = tB[x];
      nibble = 0;
    }

    /* extract two bits from both, shift/update */
    nA = (bitbufA >> 6) & 0x03;
    nB = (bitbufB >> 6) & 0x03;
    bitbufA = (bitbufA << 2) & 0xFF;
    bitbufB = (bitbufB << 2) & 0xFF;

    /* if both zero, if first, continue */
    if ((nA == 0) && (nB == 0) && (first == 1)) {
      continue;
    }

    /* double twice, only if this isn't the first */
    if (first == 0) {
      /* double twice */
      if ((err = ltc_mp.ecc_ptdbl(C, C, modulus, mp)) != CRYPT_OK) {
        goto ERR_MU;
      }
      if ((err = ltc_mp.ecc_ptdbl(C, C, modulus, mp)) != CRYPT_OK) {
        goto ERR_MU;
      }
    }

    /* if not both zero */
    if ((nA != 0) || (nB != 0)) {
      if (first == 1) {
        /* if first, copy from table */
        first = 0;
        if ((err = mp_copy(precomp[nA + (nB << 2)]->x, C->x)) != CRYPT_OK) {
          goto ERR_MU;
        }
        if ((err = mp_copy(precomp[nA + (nB << 2)]->y, C->y)) != CRYPT_OK) {
          goto ERR_MU;
        }
        if ((err = mp_copy(precomp[nA + (nB << 2)]->z, C->z)) != CRYPT_OK) {
          goto ERR_MU;
        }
      } else {
        /* if not first, add from table */
        if ((err = ltc_mp.ecc_ptadd(C, precomp[nA + (nB << 2)], C, modulus,
                                    mp)) != CRYPT_OK) {
          goto ERR_MU;
        }
      }
    }
  }

  /* reduce to affine */
  err = ltc_ecc_map(C, modulus, mp);

/* clean up */
ERR_MU:
  mp_clear(mu);
ERR_MP:
  mp_montgomery_free(mp);
ERR_P:
  for (x = 0; x < 16; x++) {
    ltc_ecc_del_point(precomp[x]);
  }
ERR_T:
  XFREE(tA);
  XFREE(tB);

  return err;
}

/* size of sliding window, don't change this! */
#define WINSIZE 4

/**
   Perform a point multiplication
   @param k    The scalar to multiply by
   @param G    The base point
   @param R    [out] Destination for kG
   @param modulus  The modulus of the field the ECC curve is in
   @param map      Boolean whether to map back to affine or not (1==map, 0 ==
   leave in projective)
   @return CRYPT_OK on success
 */
int ltc_ecc_mulmod(void *k, ecc_point *G, ecc_point *R, void *modulus,
                   int map) {
  ecc_point *tG, *M[8];
  int i, j, err;
  void *mu, *mp;
  unsigned long buf;
  int first, bitbuf, bitcpy, bitcnt, mode, digidx;

  LTC_ARGCHK(k != NULL);
  LTC_ARGCHK(G != NULL);
  LTC_ARGCHK(R != NULL);
  LTC_ARGCHK(modulus != NULL);

  /* init montgomery reduction */
  if ((err = mp_montgomery_setup(modulus, &mp)) != CRYPT_OK) {
    return err;
  }
  if ((err = mp_init(&mu)) != CRYPT_OK) {
    mp_montgomery_free(mp);
    return err;
  }
  if ((err = mp_montgomery_normalization(mu, modulus)) != CRYPT_OK) {
    mp_montgomery_free(mp);
    mp_clear(mu);
    return err;
  }

  /* alloc ram for window temps */
  for (i = 0; i < 8; i++) {
    M[i] = ltc_ecc_new_point();
    if (M[i] == NULL) {
      for (j = 0; j < i; j++) {
        ltc_ecc_del_point(M[j]);
      }
      mp_montgomery_free(mp);
      mp_clear(mu);
      return CRYPT_MEM;
    }
  }

  /* make a copy of G incase R==G */
  tG = ltc_ecc_new_point();
  if (tG == NULL) {
    err = CRYPT_MEM;
    goto done;
  }

  /* tG = G  and convert to montgomery */
  if (mp_cmp_d(mu, 1) == LTC_MP_EQ) {
    if ((err = mp_copy(G->x, tG->x)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_copy(G->y, tG->y)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_copy(G->z, tG->z)) != CRYPT_OK) {
      goto done;
    }
  } else {
    if ((err = mp_mulmod(G->x, mu, modulus, tG->x)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_mulmod(G->y, mu, modulus, tG->y)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_mulmod(G->z, mu, modulus, tG->z)) != CRYPT_OK) {
      goto done;
    }
  }
  mp_clear(mu);
  mu = NULL;

  /* calc the M tab, which holds kG for k==8..15 */
  /* M[0] == 8G */
  if ((err = ltc_mp.ecc_ptdbl(tG, M[0], modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  if ((err = ltc_mp.ecc_ptdbl(M[0], M[0], modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  if ((err = ltc_mp.ecc_ptdbl(M[0], M[0], modulus, mp)) != CRYPT_OK) {
    goto done;
  }

  /* now find (8+k)G for k=1..7 */
  for (j = 9; j < 16; j++) {
    if ((err = ltc_mp.ecc_ptadd(M[j - 9], tG, M[j - 8], modulus, mp)) !=
        CRYPT_OK) {
      goto done;
    }
  }

  /* setup sliding window */
  mode = 0;
  bitcnt = 1;
  buf = 0;
  digidx = mp_get_digit_count(k) - 1;
  bitcpy = bitbuf = 0;
  first = 1;

  /* perform ops */
  for (;;) {
    /* grab next digit as required */
    if (--bitcnt == 0) {
      if (digidx == -1) {
        break;
      }
      buf = mp_get_digit(k, digidx);
      bitcnt = (int)ltc_mp.bits_per_digit;
      --digidx;
    }

    /* grab the next msb from the ltiplicand */
    i = (buf >> (ltc_mp.bits_per_digit - 1)) & 1;
    buf <<= 1;

    /* skip leading zero bits */
    if ((mode == 0) && (i == 0)) {
      continue;
    }

    /* if the bit is zero and mode == 1 then we double */
    if ((mode == 1) && (i == 0)) {
      if ((err = ltc_mp.ecc_ptdbl(R, R, modulus, mp)) != CRYPT_OK) {
        goto done;
      }
      continue;
    }

    /* else we add it to the window */
    bitbuf |= (i << (WINSIZE - ++bitcpy));
    mode = 2;

    if (bitcpy == WINSIZE) {
      /* if this is the first window we do a simple copy */
      if (first == 1) {
        /* R = kG [k = first window] */
        if ((err = mp_copy(M[bitbuf - 8]->x, R->x)) != CRYPT_OK) {
          goto done;
        }
        if ((err = mp_copy(M[bitbuf - 8]->y, R->y)) != CRYPT_OK) {
          goto done;
        }
        if ((err = mp_copy(M[bitbuf - 8]->z, R->z)) != CRYPT_OK) {
          goto done;
        }
        first = 0;
      } else {
        /* normal window */
        /* ok window is filled so double as required and add  */
        /* double first */
        for (j = 0; j < WINSIZE; j++) {
          if ((err = ltc_mp.ecc_ptdbl(R, R, modulus, mp)) != CRYPT_OK) {
            goto done;
          }
        }

        /* then add, bitbuf will be 8..15 [8..2^WINSIZE] guaranteed */
        if ((err = ltc_mp.ecc_ptadd(R, M[bitbuf - 8], R, modulus, mp)) !=
            CRYPT_OK) {
          goto done;
        }
      }
      /* empty window and reset */
      bitcpy = bitbuf = 0;
      mode = 1;
    }
  }

  /* if bits remain then double/add */
  if ((mode == 2) && (bitcpy > 0)) {
    /* double then add */
    for (j = 0; j < bitcpy; j++) {
      /* only double if we have had at least one add first */
      if (first == 0) {
        if ((err = ltc_mp.ecc_ptdbl(R, R, modulus, mp)) != CRYPT_OK) {
          goto done;
        }
      }

      bitbuf <<= 1;
      if ((bitbuf & (1 << WINSIZE)) != 0) {
        if (first == 1) {
          /* first add, so copy */
          if ((err = mp_copy(tG->x, R->x)) != CRYPT_OK) {
            goto done;
          }
          if ((err = mp_copy(tG->y, R->y)) != CRYPT_OK) {
            goto done;
          }
          if ((err = mp_copy(tG->z, R->z)) != CRYPT_OK) {
            goto done;
          }
          first = 0;
        } else {
          /* then add */
          if ((err = ltc_mp.ecc_ptadd(R, tG, R, modulus, mp)) != CRYPT_OK) {
            goto done;
          }
        }
      }
    }
  }

  /* map R back from projective space */
  if (map) {
    err = ltc_ecc_map(R, modulus, mp);
  } else {
    err = CRYPT_OK;
  }
done:
  if (mu != NULL) {
    mp_clear(mu);
  }
  mp_montgomery_free(mp);
  ltc_ecc_del_point(tG);
  for (i = 0; i < 8; i++) {
    ltc_ecc_del_point(M[i]);
  }
  return err;
}

#undef WINSIZE

/**
   Allocate a new ECC point
   @return A newly allocated point or NULL on error
 */
ecc_point *ltc_ecc_new_point(void) {
  ecc_point *p;

  p = XCALLOC(1, sizeof(*p));
  if (p == NULL) {
    return NULL;
  }
  if (mp_init_multi(&p->x, &p->y, &p->z, NULL) != CRYPT_OK) {
    XFREE(p);
    return NULL;
  }
  return p;
}

/** Free an ECC point from memory
   @param p   The point to free
 */
void ltc_ecc_del_point(ecc_point *p) {
  /* prevents free'ing null arguments */
  if (p != NULL) {
    mp_clear_multi(p->x, p->y, p->z, NULL); /* note: p->z may be NULL but that's
                                           ok with this function anyways */
    XFREE(p);
  }
}

/**
   Add two ECC points
   @param P        The point to add
   @param Q        The point to add
   @param R        [out] The destination of the double
   @param modulus  The modulus of the field the ECC curve is in
   @param mp       The "b" value from montgomery_setup()
   @return CRYPT_OK on success
 */
int ltc_ecc_projective_add_point(ecc_point *P, ecc_point *Q, ecc_point *R,
                                 void *modulus, void *mp) {
  void *t1, *t2, *x, *y, *z;
  int err;

  LTC_ARGCHK(P != NULL);
  LTC_ARGCHK(Q != NULL);
  LTC_ARGCHK(R != NULL);
  LTC_ARGCHK(modulus != NULL);
  LTC_ARGCHK(mp != NULL);

  if ((err = mp_init_multi(&t1, &t2, &x, &y, &z, NULL)) != CRYPT_OK) {
    return err;
  }

  /* should we dbl instead? */
  if ((err = mp_sub(modulus, Q->y, t1)) != CRYPT_OK) {
    goto done;
  }

  if ((mp_cmp(P->x, Q->x) == LTC_MP_EQ) &&
      ((Q->z != NULL) && (mp_cmp(P->z, Q->z) == LTC_MP_EQ)) &&
      ((mp_cmp(P->y, Q->y) == LTC_MP_EQ) || (mp_cmp(P->y, t1) == LTC_MP_EQ))) {
    mp_clear_multi(t1, t2, x, y, z, NULL);
    return ltc_ecc_projective_dbl_point(P, R, modulus, mp);
  }

  if ((err = mp_copy(P->x, x)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_copy(P->y, y)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_copy(P->z, z)) != CRYPT_OK) {
    goto done;
  }

  /* if Z is one then these are no-operations */
  if (Q->z != NULL) {
    /* T1 = Z' * Z' */
    if ((err = mp_sqr(Q->z, t1)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
      goto done;
    }
    /* X = X * T1 */
    if ((err = mp_mul(t1, x, x)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_montgomery_reduce(x, modulus, mp)) != CRYPT_OK) {
      goto done;
    }
    /* T1 = Z' * T1 */
    if ((err = mp_mul(Q->z, t1, t1)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
      goto done;
    }
    /* Y = Y * T1 */
    if ((err = mp_mul(t1, y, y)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_montgomery_reduce(y, modulus, mp)) != CRYPT_OK) {
      goto done;
    }
  }

  /* T1 = Z*Z */
  if ((err = mp_sqr(z, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T2 = X' * T1 */
  if ((err = mp_mul(Q->x, t1, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t2, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T1 = Z * T1 */
  if ((err = mp_mul(z, t1, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T1 = Y' * T1 */
  if ((err = mp_mul(Q->y, t1, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
    goto done;
  }

  /* Y = Y - T1 */
  if ((err = mp_sub(y, t1, y)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(y, 0) == LTC_MP_LT) {
    if ((err = mp_add(y, modulus, y)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T1 = 2T1 */
  if ((err = mp_add(t1, t1, t1)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t1, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t1, modulus, t1)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T1 = Y + T1 */
  if ((err = mp_add(t1, y, t1)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t1, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t1, modulus, t1)) != CRYPT_OK) {
      goto done;
    }
  }
  /* X = X - T2 */
  if ((err = mp_sub(x, t2, x)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(x, 0) == LTC_MP_LT) {
    if ((err = mp_add(x, modulus, x)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T2 = 2T2 */
  if ((err = mp_add(t2, t2, t2)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t2, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t2, modulus, t2)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T2 = X + T2 */
  if ((err = mp_add(t2, x, t2)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t2, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t2, modulus, t2)) != CRYPT_OK) {
      goto done;
    }
  }

  /* if Z' != 1 */
  if (Q->z != NULL) {
    /* Z = Z * Z' */
    if ((err = mp_mul(z, Q->z, z)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_montgomery_reduce(z, modulus, mp)) != CRYPT_OK) {
      goto done;
    }
  }

  /* Z = Z * X */
  if ((err = mp_mul(z, x, z)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(z, modulus, mp)) != CRYPT_OK) {
    goto done;
  }

  /* T1 = T1 * X  */
  if ((err = mp_mul(t1, x, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* X = X * X */
  if ((err = mp_sqr(x, x)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(x, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T2 = T2 * x */
  if ((err = mp_mul(t2, x, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t2, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T1 = T1 * X  */
  if ((err = mp_mul(t1, x, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
    goto done;
  }

  /* X = Y*Y */
  if ((err = mp_sqr(y, x)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(x, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* X = X - T2 */
  if ((err = mp_sub(x, t2, x)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(x, 0) == LTC_MP_LT) {
    if ((err = mp_add(x, modulus, x)) != CRYPT_OK) {
      goto done;
    }
  }

  /* T2 = T2 - X */
  if ((err = mp_sub(t2, x, t2)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(t2, 0) == LTC_MP_LT) {
    if ((err = mp_add(t2, modulus, t2)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T2 = T2 - X */
  if ((err = mp_sub(t2, x, t2)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(t2, 0) == LTC_MP_LT) {
    if ((err = mp_add(t2, modulus, t2)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T2 = T2 * Y */
  if ((err = mp_mul(t2, y, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t2, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* Y = T2 - T1 */
  if ((err = mp_sub(t2, t1, y)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(y, 0) == LTC_MP_LT) {
    if ((err = mp_add(y, modulus, y)) != CRYPT_OK) {
      goto done;
    }
  }
  /* Y = Y/2 */
  if (mp_isodd(y)) {
    if ((err = mp_add(y, modulus, y)) != CRYPT_OK) {
      goto done;
    }
  }
  if ((err = mp_div_2(y, y)) != CRYPT_OK) {
    goto done;
  }

  if ((err = mp_copy(x, R->x)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_copy(y, R->y)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_copy(z, R->z)) != CRYPT_OK) {
    goto done;
  }

  err = CRYPT_OK;
done:
  mp_clear_multi(t1, t2, x, y, z, NULL);
  return err;
}

/**
   Double an ECC point
   @param P   The point to double
   @param R   [out] The destination of the double
   @param modulus  The modulus of the field the ECC curve is in
   @param mp       The "b" value from montgomery_setup()
   @return CRYPT_OK on success
 */
int ltc_ecc_projective_dbl_point(ecc_point *P, ecc_point *R, void *modulus,
                                 void *mp) {
  void *t1, *t2;
  int err;

  LTC_ARGCHK(P != NULL);
  LTC_ARGCHK(R != NULL);
  LTC_ARGCHK(modulus != NULL);
  LTC_ARGCHK(mp != NULL);

  if ((err = mp_init_multi(&t1, &t2, NULL)) != CRYPT_OK) {
    return err;
  }

  if (P != R) {
    if ((err = mp_copy(P->x, R->x)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_copy(P->y, R->y)) != CRYPT_OK) {
      goto done;
    }
    if ((err = mp_copy(P->z, R->z)) != CRYPT_OK) {
      goto done;
    }
  }

  /* t1 = Z * Z */
  if ((err = mp_sqr(R->z, t1)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t1, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* Z = Y * Z */
  if ((err = mp_mul(R->z, R->y, R->z)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(R->z, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* Z = 2Z */
  if ((err = mp_add(R->z, R->z, R->z)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(R->z, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(R->z, modulus, R->z)) != CRYPT_OK) {
      goto done;
    }
  }

  /* T2 = X - T1 */
  if ((err = mp_sub(R->x, t1, t2)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(t2, 0) == LTC_MP_LT) {
    if ((err = mp_add(t2, modulus, t2)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T1 = X + T1 */
  if ((err = mp_add(t1, R->x, t1)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t1, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t1, modulus, t1)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T2 = T1 * T2 */
  if ((err = mp_mul(t1, t2, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t2, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T1 = 2T2 */
  if ((err = mp_add(t2, t2, t1)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t1, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t1, modulus, t1)) != CRYPT_OK) {
      goto done;
    }
  }
  /* T1 = T1 + T2 */
  if ((err = mp_add(t1, t2, t1)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(t1, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(t1, modulus, t1)) != CRYPT_OK) {
      goto done;
    }
  }

  /* Y = 2Y */
  if ((err = mp_add(R->y, R->y, R->y)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp(R->y, modulus) != LTC_MP_LT) {
    if ((err = mp_sub(R->y, modulus, R->y)) != CRYPT_OK) {
      goto done;
    }
  }
  /* Y = Y * Y */
  if ((err = mp_sqr(R->y, R->y)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(R->y, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T2 = Y * Y */
  if ((err = mp_sqr(R->y, t2)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(t2, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* T2 = T2/2 */
  if (mp_isodd(t2)) {
    if ((err = mp_add(t2, modulus, t2)) != CRYPT_OK) {
      goto done;
    }
  }
  if ((err = mp_div_2(t2, t2)) != CRYPT_OK) {
    goto done;
  }
  /* Y = Y * X */
  if ((err = mp_mul(R->y, R->x, R->y)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(R->y, modulus, mp)) != CRYPT_OK) {
    goto done;
  }

  /* X  = T1 * T1 */
  if ((err = mp_sqr(t1, R->x)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(R->x, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* X = X - Y */
  if ((err = mp_sub(R->x, R->y, R->x)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(R->x, 0) == LTC_MP_LT) {
    if ((err = mp_add(R->x, modulus, R->x)) != CRYPT_OK) {
      goto done;
    }
  }
  /* X = X - Y */
  if ((err = mp_sub(R->x, R->y, R->x)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(R->x, 0) == LTC_MP_LT) {
    if ((err = mp_add(R->x, modulus, R->x)) != CRYPT_OK) {
      goto done;
    }
  }

  /* Y = Y - X */
  if ((err = mp_sub(R->y, R->x, R->y)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(R->y, 0) == LTC_MP_LT) {
    if ((err = mp_add(R->y, modulus, R->y)) != CRYPT_OK) {
      goto done;
    }
  }
  /* Y = Y * T1 */
  if ((err = mp_mul(R->y, t1, R->y)) != CRYPT_OK) {
    goto done;
  }
  if ((err = mp_montgomery_reduce(R->y, modulus, mp)) != CRYPT_OK) {
    goto done;
  }
  /* Y = Y - T2 */
  if ((err = mp_sub(R->y, t2, R->y)) != CRYPT_OK) {
    goto done;
  }
  if (mp_cmp_d(R->y, 0) == LTC_MP_LT) {
    if ((err = mp_add(R->y, modulus, R->y)) != CRYPT_OK) {
      goto done;
    }
  }

  err = CRYPT_OK;
done:
  mp_clear_multi(t1, t2, NULL);
  return err;
}

#undef mp_init
#undef mp_init_multi
#undef mp_clear
#undef mp_clear_multi
#undef mp_init_copy
#undef mp_neg
#undef mp_copy
#undef mp_set
#undef mp_set_int
#undef mp_get_int
#undef mp_get_digit
#undef mp_get_digit_count
#undef mp_cmp
#undef mp_cmp_d
#undef mp_count_bits
#undef mp_cnt_lsb
#undef mp_2expt
#undef mp_read_radix
#undef mp_toradix
#undef mp_unsigned_bin_size
#undef mp_to_unsigned_bin
#undef mp_read_unsigned_bin
#undef mp_add
#undef mp_add_d
#undef mp_sub
#undef mp_sub_d
#undef mp_mul
#undef mp_mul_d
#undef mp_sqr
#undef mp_div
#undef mp_div_2
#undef mp_mod
#undef mp_mod_d
#undef mp_gcd
#undef mp_lcm
#undef mp_mulmod
#undef mp_sqrmod
#undef mp_invmod
#undef mp_montgomery_setup
#undef mp_montgomery_normalization
#undef mp_montgomery_reduce
#undef mp_montgomery_free
#undef mp_exptmod
#undef mp_prime_is_prime
#undef mp_iszero
#undef mp_isodd
#undef mp_exch
#undef mp_tohex

static const struct {
  int mpi_code, ltc_code;
} mpi_to_ltc_codes[] = {
    {MP_OKAY, CRYPT_OK},
    {MP_MEM, CRYPT_MEM},
    {MP_VAL, CRYPT_INVALID_ARG},
};

/**
   Convert a MPI error to a LTC error (Possibly the most powerful function ever!
   Oh wait... no)
   @param err    The error to convert
   @return The equivalent LTC error code or CRYPT_ERROR if none found
 */
static int mpi_to_ltc_error(int err) {
  int x;

  for (x = 0; x < (int)(sizeof(mpi_to_ltc_codes) / sizeof(mpi_to_ltc_codes[0]));
       x++) {
    if (err == mpi_to_ltc_codes[x].mpi_code) {
      return mpi_to_ltc_codes[x].ltc_code;
    }
  }
  return CRYPT_ERROR;
}

static int init(void **a) {
  int err;

  LTC_ARGCHK(a != NULL);

  *a = XCALLOC(1, sizeof(mp_int));
  if (*a == NULL) {
    return CRYPT_MEM;
  }
  if ((err = mpi_to_ltc_error(mp_init(*a))) != CRYPT_OK) {
    XFREE(*a);
  }
  return err;
}

static void deinit(void *a) {
  LTC_ARGCHKVD(a != NULL);
  mp_clear(a);
  XFREE(a);
}

static int neg(void *a, void *b) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_neg(a, b));
}

static int copy(void *a, void *b) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_copy(a, b));
}

static int init_copy(void **a, void *b) {
  if (init(a) != CRYPT_OK) {
    return CRYPT_MEM;
  }
  return copy(b, *a);
}

/* ---- trivial ---- */
static int set_int(void *a, unsigned long b) {
  LTC_ARGCHK(a != NULL);
  return mpi_to_ltc_error(mp_set_int(a, b));
}

static unsigned long get_int(void *a) {
  LTC_ARGCHK(a != NULL);
  return mp_get_int(a);
}

static unsigned long get_digit(void *a, int n) {
  mp_int *A;

  LTC_ARGCHK(a != NULL);
  A = a;
  return (n >= A->used || n < 0) ? 0 : A->dp[n];
}

static int get_digit_count(void *a) {
  mp_int *A;

  LTC_ARGCHK(a != NULL);
  A = a;
  return A->used;
}

static int compare(void *a, void *b) {
  int ret;

  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  ret = mp_cmp(a, b);
  switch (ret) {
    case MP_LT:
      return LTC_MP_LT;

    case MP_EQ:
      return LTC_MP_EQ;

    case MP_GT:
      return LTC_MP_GT;
  }
  return 0;
}

static int compare_d(void *a, unsigned long b) {
  int ret;

  LTC_ARGCHK(a != NULL);
  ret = mp_cmp_d(a, b);
  switch (ret) {
    case MP_LT:
      return LTC_MP_LT;

    case MP_EQ:
      return LTC_MP_EQ;

    case MP_GT:
      return LTC_MP_GT;
  }
  return 0;
}

static int count_bits(void *a) {
  LTC_ARGCHK(a != NULL);
  return mp_count_bits(a);
}

static int count_lsb_bits(void *a) {
  LTC_ARGCHK(a != NULL);
  return mp_cnt_lsb(a);
}

static int twoexpt(void *a, int n) {
  LTC_ARGCHK(a != NULL);
  return mpi_to_ltc_error(mp_2expt(a, n));
}

/* ---- conversions ---- */

/* read ascii string */
static int read_radix(void *a, const char *b, int radix) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_read_radix(a, b, radix));
}

/* write one */
static int write_radix(void *a, char *b, int radix) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_toradix(a, b, radix));
}

/* get size as unsigned char string */
static unsigned long unsigned_size(void *a) {
  LTC_ARGCHK(a != NULL);
  return mp_unsigned_bin_size(a);
}

/* store */
static int unsigned_write(void *a, unsigned char *b) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_to_unsigned_bin(a, b));
}

/* read */
static int unsigned_read(void *a, unsigned char *b, unsigned long len) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_read_unsigned_bin(a, b, len));
}

/* add */
static int add(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_add(a, b, c));
}

static int addi(void *a, unsigned long b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_add_d(a, b, c));
}

/* sub */
static int sub(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_sub(a, b, c));
}

static int subi(void *a, unsigned long b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_sub_d(a, b, c));
}

/* mul */
static int mul(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_mul(a, b, c));
}

static int muli(void *a, unsigned long b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_mul_d(a, b, c));
}

/* sqr */
static int sqr(void *a, void *b) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_sqr(a, b));
}

/* div */
static int divide(void *a, void *b, void *c, void *d) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_div(a, b, c, d));
}

static int div_2(void *a, void *b) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_div_2(a, b));
}

/* modi */
static int modi(void *a, unsigned long b, unsigned long *c) {
  mp_digit tmp;
  int err;

  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(c != NULL);

  if ((err = mpi_to_ltc_error(mp_mod_d(a, b, &tmp))) != CRYPT_OK) {
    return err;
  }
  *c = tmp;
  return CRYPT_OK;
}

/* gcd */
static int gcd(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_gcd(a, b, c));
}

/* lcm */
static int lcm(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_lcm(a, b, c));
}

static int mulmod(void *a, void *b, void *c, void *d) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  LTC_ARGCHK(d != NULL);
  return mpi_to_ltc_error(mp_mulmod(a, b, c, d));
}

static int sqrmod(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_sqrmod(a, b, c));
}

/* invmod */
static int invmod(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_invmod(a, b, c));
}

/* setup */
static int montgomery_setup(void *a, void **b) {
  int err;

  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  *b = XCALLOC(1, sizeof(mp_digit));
  if (*b == NULL) {
    return CRYPT_MEM;
  }
  if ((err = mpi_to_ltc_error(mp_montgomery_setup(a, (mp_digit *)*b))) !=
      CRYPT_OK) {
    XFREE(*b);
  }
  return err;
}

/* get normalization value */
static int montgomery_normalization(void *a, void *b) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  return mpi_to_ltc_error(mp_montgomery_calc_normalization(a, b));
}

/* reduce */
static int montgomery_reduce(void *a, void *b, void *c) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  return mpi_to_ltc_error(mp_montgomery_reduce(a, b, *((mp_digit *)c)));
}

/* clean up */
static void montgomery_deinit(void *a) { XFREE(a); }

static int exptmod(void *a, void *b, void *c, void *d) {
  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  LTC_ARGCHK(c != NULL);
  LTC_ARGCHK(d != NULL);
  return mpi_to_ltc_error(mp_exptmod(a, b, c, d));
}

static int isprime(void *a, int *b) {
  int err;

  LTC_ARGCHK(a != NULL);
  LTC_ARGCHK(b != NULL);
  err = mpi_to_ltc_error(mp_prime_is_prime(a, 8, b));
  *b = (*b == MP_YES) ? LTC_MP_YES : LTC_MP_NO;
  return err;
}

const ltc_math_descriptor ltm_desc = {
    "LibTomMath",
    (int)DIGIT_BIT,

    &init,
    &init_copy,
    &deinit,

    &neg,
    &copy,

    &set_int,
    &get_int,
    &get_digit,
    &get_digit_count,
    &compare,
    &compare_d,
    &count_bits,
    &count_lsb_bits,
    &twoexpt,

    &read_radix,
    &write_radix,
    &unsigned_size,
    &unsigned_write,
    &unsigned_read,

    &add,
    &addi,
    &sub,
    &subi,
    &mul,
    &muli,
    &sqr,
    &divide,
    &div_2,
    &modi,
    &gcd,
    &lcm,

    &mulmod,
    &sqrmod,
    &invmod,

    &montgomery_setup,
    &montgomery_normalization,
    &montgomery_reduce,
    &montgomery_deinit,

    &exptmod,
    &isprime,

    &ltc_ecc_mulmod,
    &ltc_ecc_projective_add_point,
    &ltc_ecc_projective_dbl_point,
    &ltc_ecc_map,
    &ltc_ecc_mul2add,

    &rsa_make_key,
    &rsa_exptmod,
};

#define mp_init(a) ltc_mp.init(a)
#define mp_init_multi ltc_init_multi
#define mp_clear(a) ltc_mp.deinit(a)
#define mp_clear_multi ltc_deinit_multi
#define mp_init_copy(a, b) ltc_mp.init_copy(a, b)

#define mp_neg(a, b) ltc_mp.neg(a, b)
#define mp_copy(a, b) ltc_mp.copy(a, b)

#define mp_set(a, b) ltc_mp.set_int(a, b)
#define mp_set_int(a, b) ltc_mp.set_int(a, b)
#define mp_get_int(a) ltc_mp.get_int(a)
#define mp_get_digit(a, n) ltc_mp.get_digit(a, n)
#define mp_get_digit_count(a) ltc_mp.get_digit_count(a)
#define mp_cmp(a, b) ltc_mp.compare(a, b)
#define mp_cmp_d(a, b) ltc_mp.compare_d(a, b)
#define mp_count_bits(a) ltc_mp.count_bits(a)
#define mp_cnt_lsb(a) ltc_mp.count_lsb_bits(a)
#define mp_2expt(a, b) ltc_mp.twoexpt(a, b)

#define mp_read_radix(a, b, c) ltc_mp.read_radix(a, b, c)
#define mp_toradix(a, b, c) ltc_mp.write_radix(a, b, c)
#define mp_unsigned_bin_size(a) ltc_mp.unsigned_size(a)
#define mp_to_unsigned_bin(a, b) ltc_mp.unsigned_write(a, b)
#define mp_read_unsigned_bin(a, b, c) ltc_mp.unsigned_read(a, b, c)

#define mp_add(a, b, c) ltc_mp.add(a, b, c)
#define mp_add_d(a, b, c) ltc_mp.addi(a, b, c)
#define mp_sub(a, b, c) ltc_mp.sub(a, b, c)
#define mp_sub_d(a, b, c) ltc_mp.subi(a, b, c)
#define mp_mul(a, b, c) ltc_mp.mul(a, b, c)
#define mp_mul_d(a, b, c) ltc_mp.muli(a, b, c)
#define mp_sqr(a, b) ltc_mp.sqr(a, b)
#define mp_div(a, b, c, d) ltc_mp.mpdiv(a, b, c, d)
#define mp_div_2(a, b) ltc_mp.div_2(a, b)
#define mp_mod(a, b, c) ltc_mp.mpdiv(a, b, NULL, c)
#define mp_mod_d(a, b, c) ltc_mp.modi(a, b, c)
#define mp_gcd(a, b, c) ltc_mp.gcd(a, b, c)
#define mp_lcm(a, b, c) ltc_mp.lcm(a, b, c)

#define mp_mulmod(a, b, c, d) ltc_mp.mulmod(a, b, c, d)
#define mp_sqrmod(a, b, c) ltc_mp.sqrmod(a, b, c)
#define mp_invmod(a, b, c) ltc_mp.invmod(a, b, c)

#define mp_montgomery_setup(a, b) ltc_mp.montgomery_setup(a, b)
#define mp_montgomery_normalization(a, b) ltc_mp.montgomery_normalization(a, b)
#define mp_montgomery_reduce(a, b, c) ltc_mp.montgomery_reduce(a, b, c)
#define mp_montgomery_free(a) ltc_mp.montgomery_deinit(a)

#define mp_exptmod(a, b, c, d) ltc_mp.exptmod(a, b, c, d)
#define mp_prime_is_prime(a, b, c) ltc_mp.isprime(a, c)

#define mp_iszero(a) (mp_cmp_d(a, 0) == LTC_MP_EQ ? LTC_MP_YES : LTC_MP_NO)
#define mp_isodd(a)                                        \
  (mp_get_digit_count(a) > 0                               \
       ? (mp_get_digit(a, 0) & 1 ? LTC_MP_YES : LTC_MP_NO) \
       : LTC_MP_NO)
#define mp_exch(a, b)   \
  do {                  \
    void *ABC__tmp = a; \
    a = b;              \
    b = ABC__tmp;       \
  } while (0);

#define mp_tohex(a, b) mp_toradix(a, b, 16)

int ltc_init_multi(void **a, ...) {
  void **cur = a;
  int np = 0;
  va_list args;

  va_start(args, a);
  while (cur != NULL) {
    if (mp_init(cur) != CRYPT_OK) {
      /* failed */
      va_list clean_list;

      va_start(clean_list, a);
      cur = a;
      while (np--) {
        mp_clear(*cur);
        cur = va_arg(clean_list, void **);
      }
      va_end(clean_list);
      return CRYPT_MEM;
    }
    ++np;
    cur = va_arg(args, void **);
  }
  va_end(args);
  return CRYPT_OK;
}

void ltc_deinit_multi(void *a, ...) {
  void *cur = a;
  va_list args;

  va_start(args, a);
  while (cur != NULL) {
    mp_clear(cur);
    cur = va_arg(args, void *);
  }
  va_end(args);
}

/**
   Perform LTC_PKCS #1 MGF1 (internal)
   @param seed        The seed for MGF1
   @param seedlen     The length of the seed
   @param hash_idx    The index of the hash desired
   @param mask        [out] The destination
   @param masklen     The length of the mask desired
   @return CRYPT_OK if successful
 */
int pkcs_1_mgf1(int hash_idx, const unsigned char *seed, unsigned long seedlen,
                unsigned char *mask, unsigned long masklen) {
  unsigned long hLen, x;
  ulong32 counter;
  int err;
  hash_state *md;
  unsigned char *buf;

  LTC_ARGCHK(seed != NULL);
  LTC_ARGCHK(mask != NULL);

  /* ensure valid hash */
  if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
    return err;
  }

  /* get hash output size */
  hLen = hash_descriptor[hash_idx].hashsize;

  /* allocate memory */
  md = XMALLOC(sizeof(hash_state));
  buf = XMALLOC(hLen);
  if ((md == NULL) || (buf == NULL)) {
    if (md != NULL) {
      XFREE(md);
    }
    if (buf != NULL) {
      XFREE(buf);
    }
    return CRYPT_MEM;
  }

  /* start counter */
  counter = 0;

  while (masklen > 0) {
    /* handle counter */
    STORE32H(counter, buf);
    ++counter;

    /* get hash of seed || counter */
    if ((err = hash_descriptor[hash_idx].init(md)) != CRYPT_OK) {
      goto LBL_ERR;
    }
    if ((err = hash_descriptor[hash_idx].process(md, seed, seedlen)) !=
        CRYPT_OK) {
      goto LBL_ERR;
    }
    if ((err = hash_descriptor[hash_idx].process(md, buf, 4)) != CRYPT_OK) {
      goto LBL_ERR;
    }
    if ((err = hash_descriptor[hash_idx].done(md, buf)) != CRYPT_OK) {
      goto LBL_ERR;
    }

    /* store it */
    for (x = 0; x < hLen && masklen > 0; x++, masklen--) {
      *mask++ = buf[x];
    }
  }

  err = CRYPT_OK;
LBL_ERR:

  XFREE(buf);
  XFREE(md);

  return err;
}

/**
   LTC_PKCS #1 v2.00 OAEP decode
   @param msg              The encoded data to decode
   @param msglen           The length of the encoded data (octets)
   @param lparam           The session or system data (can be NULL)
   @param lparamlen        The length of the lparam
   @param modulus_bitlen   The bit length of the RSA modulus
   @param hash_idx         The index of the hash desired
   @param out              [out] Destination of decoding
   @param outlen           [in/out] The max size and resulting size of the
   decoding
   @param res              [out] Result of decoding, 1==valid, 0==invalid
   @return CRYPT_OK if successful (even if invalid)
 */
int pkcs_1_oaep_decode(const unsigned char *msg, unsigned long msglen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       unsigned long modulus_bitlen, int hash_idx,
                       unsigned char *out, unsigned long *outlen, int *res) {
  unsigned char *DB, *seed, *mask;
  unsigned long hLen, x, y, modulus_len;
  int err;

  LTC_ARGCHK(msg != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(res != NULL);

  /* default to invalid packet */
  *res = 0;

  /* test valid hash */
  if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
    return err;
  }
  hLen = hash_descriptor[hash_idx].hashsize;
  modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

  /* test hash/message size */
  if ((2 * hLen >= (modulus_len - 2)) || (msglen != modulus_len)) {
    return CRYPT_PK_INVALID_SIZE;
  }

  /* allocate ram for DB/mask/salt of size modulus_len */
  DB = XMALLOC(modulus_len);
  mask = XMALLOC(modulus_len);
  seed = XMALLOC(hLen);
  if ((DB == NULL) || (mask == NULL) || (seed == NULL)) {
    if (DB != NULL) {
      XFREE(DB);
    }
    if (mask != NULL) {
      XFREE(mask);
    }
    if (seed != NULL) {
      XFREE(seed);
    }
    return CRYPT_MEM;
  }

  /* ok so it's now in the form

   0x00  || maskedseed || maskedDB

    1    ||   hLen     ||  modulus_len - hLen - 1

 */

  /* must have leading 0x00 byte */
  if (msg[0] != 0x00) {
    err = CRYPT_OK;
    goto LBL_ERR;
  }

  /* now read the masked seed */
  x = 1;
  XMEMCPY(seed, msg + x, hLen);
  x += hLen;

  /* now read the masked DB */
  XMEMCPY(DB, msg + x, modulus_len - hLen - 1);
  x += modulus_len - hLen - 1;

  /* compute MGF1 of maskedDB (hLen) */
  if ((err = pkcs_1_mgf1(hash_idx, DB, modulus_len - hLen - 1, mask, hLen)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }

  /* XOR against seed */
  for (y = 0; y < hLen; y++) {
    seed[y] ^= mask[y];
  }

  /* compute MGF1 of seed (k - hlen - 1) */
  if ((err = pkcs_1_mgf1(hash_idx, seed, hLen, mask, modulus_len - hLen - 1)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }

  /* xor against DB */
  for (y = 0; y < (modulus_len - hLen - 1); y++) {
    DB[y] ^= mask[y];
  }

  /* now DB == lhash || PS || 0x01 || M, PS == k - mlen - 2hlen - 2 zeroes */

  /* compute lhash and store it in seed [reuse temps!] */
  x = modulus_len;
  if (lparam != NULL) {
    if ((err = hash_memory(hash_idx, lparam, lparamlen, seed, &x)) !=
        CRYPT_OK) {
      goto LBL_ERR;
    }
  } else {
    /* can't pass hash_memory a NULL so use DB with zero length */
    if ((err = hash_memory(hash_idx, DB, 0, seed, &x)) != CRYPT_OK) {
      goto LBL_ERR;
    }
  }

  /* compare the lhash'es */
  if (XMEMCMP(seed, DB, hLen) != 0) {
    err = CRYPT_OK;
    goto LBL_ERR;
  }

  /* now zeroes before a 0x01 */
  for (x = hLen; x < (modulus_len - hLen - 1) && DB[x] == 0x00; x++) {
    /* step... */
  }

  /* error out if wasn't 0x01 */
  if ((x == (modulus_len - hLen - 1)) || (DB[x] != 0x01)) {
    err = CRYPT_INVALID_PACKET;
    goto LBL_ERR;
  }

  /* rest is the message (and skip 0x01) */
  if ((modulus_len - hLen - 1 - ++x) > *outlen) {
    *outlen = modulus_len - hLen - 1 - x;
    err = CRYPT_BUFFER_OVERFLOW;
    goto LBL_ERR;
  }

  /* copy message */
  *outlen = modulus_len - hLen - 1 - x;
  XMEMCPY(out, DB + x, modulus_len - hLen - 1 - x);
  x += modulus_len - hLen - 1;

  /* valid packet */
  *res = 1;

  err = CRYPT_OK;
LBL_ERR:

  XFREE(seed);
  XFREE(mask);
  XFREE(DB);

  return err;
}

/**
   LTC_PKCS #1 v2.00 OAEP encode
   @param msg             The data to encode
   @param msglen          The length of the data to encode (octets)
   @param lparam          A session or system parameter (can be NULL)
   @param lparamlen       The length of the lparam data
   @param modulus_bitlen  The bit length of the RSA modulus
   @param prng            An active PRNG state
   @param prng_idx        The index of the PRNG desired
   @param hash_idx        The index of the hash desired
   @param out             [out] The destination for the encoded data
   @param outlen          [in/out] The max size and resulting size of the
   encoded data
   @return CRYPT_OK if successful
 */
int pkcs_1_oaep_encode(const unsigned char *msg, unsigned long msglen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       unsigned long modulus_bitlen, prng_state *prng,
                       int prng_idx, int hash_idx, unsigned char *out,
                       unsigned long *outlen) {
  unsigned char *DB, *seed, *mask;
  unsigned long hLen, x, y, modulus_len;
  int err;

  LTC_ARGCHK(msg != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* test valid hash */
  if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
    return err;
  }

  /* valid prng */
  if ((err = prng_is_valid(prng_idx)) != CRYPT_OK) {
    return err;
  }

  hLen = hash_descriptor[hash_idx].hashsize;
  modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

  /* test message size */
  if ((2 * hLen >= (modulus_len - 2)) ||
      (msglen > (modulus_len - 2 * hLen - 2))) {
    return CRYPT_PK_INVALID_SIZE;
  }

  /* allocate ram for DB/mask/salt of size modulus_len */
  DB = XMALLOC(modulus_len);
  mask = XMALLOC(modulus_len);
  seed = XMALLOC(hLen);
  if ((DB == NULL) || (mask == NULL) || (seed == NULL)) {
    if (DB != NULL) {
      XFREE(DB);
    }
    if (mask != NULL) {
      XFREE(mask);
    }
    if (seed != NULL) {
      XFREE(seed);
    }
    return CRYPT_MEM;
  }

  /* get lhash */
  /* DB == lhash || PS || 0x01 || M, PS == k - mlen - 2hlen - 2 zeroes */
  x = modulus_len;
  if (lparam != NULL) {
    if ((err = hash_memory(hash_idx, lparam, lparamlen, DB, &x)) != CRYPT_OK) {
      goto LBL_ERR;
    }
  } else {
    /* can't pass hash_memory a NULL so use DB with zero length */
    if ((err = hash_memory(hash_idx, DB, 0, DB, &x)) != CRYPT_OK) {
      goto LBL_ERR;
    }
  }

  /* append PS then 0x01 (to lhash)  */
  x = hLen;
  y = modulus_len - msglen - 2 * hLen - 2;
  XMEMSET(DB + x, 0, y);
  x += y;

  /* 0x01 byte */
  DB[x++] = 0x01;

  /* message (length = msglen) */
  XMEMCPY(DB + x, msg, msglen);
  x += msglen;

  /* now choose a random seed */
  if (prng_descriptor[prng_idx].read(seed, hLen, prng) != hLen) {
    err = CRYPT_ERROR_READPRNG;
    goto LBL_ERR;
  }

  /* compute MGF1 of seed (k - hlen - 1) */
  if ((err = pkcs_1_mgf1(hash_idx, seed, hLen, mask, modulus_len - hLen - 1)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }

  /* xor against DB */
  for (y = 0; y < (modulus_len - hLen - 1); y++) {
    DB[y] ^= mask[y];
  }

  /* compute MGF1 of maskedDB (hLen) */
  if ((err = pkcs_1_mgf1(hash_idx, DB, modulus_len - hLen - 1, mask, hLen)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }

  /* XOR against seed */
  for (y = 0; y < hLen; y++) {
    seed[y] ^= mask[y];
  }

  /* create string of length modulus_len */
  if (*outlen < modulus_len) {
    *outlen = modulus_len;
    err = CRYPT_BUFFER_OVERFLOW;
    goto LBL_ERR;
  }

  /* start output which is 0x00 || maskedSeed || maskedDB */
  x = 0;
  out[x++] = 0x00;
  XMEMCPY(out + x, seed, hLen);
  x += hLen;
  XMEMCPY(out + x, DB, modulus_len - hLen - 1);
  x += modulus_len - hLen - 1;

  *outlen = x;

  err = CRYPT_OK;
LBL_ERR:

  XFREE(seed);
  XFREE(mask);
  XFREE(DB);

  return err;
}

/**
   LTC_PKCS #1 v2.00 PSS decode
   @param  msghash         The hash to verify
   @param  msghashlen      The length of the hash (octets)
   @param  sig             The signature data (encoded data)
   @param  siglen          The length of the signature data (octets)
   @param  saltlen         The length of the salt used (octets)
   @param  hash_idx        The index of the hash desired
   @param  modulus_bitlen  The bit length of the RSA modulus
   @param  res             [out] The result of the comparison, 1==valid,
   0==invalid
   @return CRYPT_OK if successful (even if the comparison failed)
 */
int pkcs_1_pss_decode(const unsigned char *msghash, unsigned long msghashlen,
                      const unsigned char *sig, unsigned long siglen,
                      unsigned long saltlen, int hash_idx,
                      unsigned long modulus_bitlen, int *res) {
  unsigned char *DB, *mask, *salt, *hash;
  unsigned long x, y, hLen, modulus_len;
  int err;
  hash_state md;

  LTC_ARGCHK(msghash != NULL);
  LTC_ARGCHK(res != NULL);

  /* default to invalid */
  *res = 0;

  /* ensure hash is valid */
  if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
    return err;
  }

  hLen = hash_descriptor[hash_idx].hashsize;
  modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

  /* check sizes */
  if ((saltlen > modulus_len) || (modulus_len < hLen + saltlen + 2) ||
      (siglen != modulus_len)) {
    return CRYPT_PK_INVALID_SIZE;
  }

  /* allocate ram for DB/mask/salt/hash of size modulus_len */
  DB = XMALLOC(modulus_len);
  mask = XMALLOC(modulus_len);
  salt = XMALLOC(modulus_len);
  hash = XMALLOC(modulus_len);
  if ((DB == NULL) || (mask == NULL) || (salt == NULL) || (hash == NULL)) {
    if (DB != NULL) {
      XFREE(DB);
    }
    if (mask != NULL) {
      XFREE(mask);
    }
    if (salt != NULL) {
      XFREE(salt);
    }
    if (hash != NULL) {
      XFREE(hash);
    }
    return CRYPT_MEM;
  }

  /* ensure the 0xBC byte */
  if (sig[siglen - 1] != 0xBC) {
    err = CRYPT_INVALID_PACKET;
    goto LBL_ERR;
  }

  /* copy out the DB */
  x = 0;
  XMEMCPY(DB, sig + x, modulus_len - hLen - 1);
  x += modulus_len - hLen - 1;

  /* copy out the hash */
  XMEMCPY(hash, sig + x, hLen);
  x += hLen;

  /* check the MSB */
  if ((sig[0] & ~(0xFF >> ((modulus_len << 3) - (modulus_bitlen - 1)))) != 0) {
    err = CRYPT_INVALID_PACKET;
    goto LBL_ERR;
  }

  /* generate mask of length modulus_len - hLen - 1 from hash */
  if ((err = pkcs_1_mgf1(hash_idx, hash, hLen, mask, modulus_len - hLen - 1)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }

  /* xor against DB */
  for (y = 0; y < (modulus_len - hLen - 1); y++) {
    DB[y] ^= mask[y];
  }

  /* now clear the first byte [make sure smaller than modulus] */
  DB[0] &= 0xFF >> ((modulus_len << 3) - (modulus_bitlen - 1));

  /* DB = PS || 0x01 || salt, PS == modulus_len - saltlen - hLen - 2 zero bytes
   */

  /* check for zeroes and 0x01 */
  for (x = 0; x < modulus_len - saltlen - hLen - 2; x++) {
    if (DB[x] != 0x00) {
      err = CRYPT_INVALID_PACKET;
      goto LBL_ERR;
    }
  }

  /* check for the 0x01 */
  if (DB[x++] != 0x01) {
    err = CRYPT_INVALID_PACKET;
    goto LBL_ERR;
  }

  /* M = (eight) 0x00 || msghash || salt, mask = H(M) */
  if ((err = hash_descriptor[hash_idx].init(&md)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  zeromem(mask, 8);
  if ((err = hash_descriptor[hash_idx].process(&md, mask, 8)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash_idx].process(&md, msghash, msghashlen)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash_idx].process(&md, DB + x, saltlen)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash_idx].done(&md, mask)) != CRYPT_OK) {
    goto LBL_ERR;
  }

  /* mask == hash means valid signature */
  if (XMEMCMP(mask, hash, hLen) == 0) {
    *res = 1;
  }

  err = CRYPT_OK;
LBL_ERR:

  XFREE(hash);
  XFREE(salt);
  XFREE(mask);
  XFREE(DB);

  return err;
}

/**
   LTC_PKCS #1 v2.00 Signature Encoding
   @param msghash          The hash to encode
   @param msghashlen       The length of the hash (octets)
   @param saltlen          The length of the salt desired (octets)
   @param prng             An active PRNG context
   @param prng_idx         The index of the PRNG desired
   @param hash_idx         The index of the hash desired
   @param modulus_bitlen   The bit length of the RSA modulus
   @param out              [out] The destination of the encoding
   @param outlen           [in/out] The max size and resulting size of the
   encoded data
   @return CRYPT_OK if successful
 */
int pkcs_1_pss_encode(const unsigned char *msghash, unsigned long msghashlen,
                      unsigned long saltlen, prng_state *prng, int prng_idx,
                      int hash_idx, unsigned long modulus_bitlen,
                      unsigned char *out, unsigned long *outlen) {
  unsigned char *DB, *mask, *salt, *hash;
  unsigned long x, y, hLen, modulus_len;
  int err;
  hash_state md;

  LTC_ARGCHK(msghash != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);

  /* ensure hash and PRNG are valid */
  if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
    return err;
  }
  if ((err = prng_is_valid(prng_idx)) != CRYPT_OK) {
    return err;
  }

  hLen = hash_descriptor[hash_idx].hashsize;
  modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

  /* check sizes */
  if ((saltlen > modulus_len) || (modulus_len < hLen + saltlen + 2)) {
    return CRYPT_PK_INVALID_SIZE;
  }

  /* allocate ram for DB/mask/salt/hash of size modulus_len */
  DB = XMALLOC(modulus_len);
  mask = XMALLOC(modulus_len);
  salt = XMALLOC(modulus_len);
  hash = XMALLOC(modulus_len);
  if ((DB == NULL) || (mask == NULL) || (salt == NULL) || (hash == NULL)) {
    if (DB != NULL) {
      XFREE(DB);
    }
    if (mask != NULL) {
      XFREE(mask);
    }
    if (salt != NULL) {
      XFREE(salt);
    }
    if (hash != NULL) {
      XFREE(hash);
    }
    return CRYPT_MEM;
  }

  /* generate random salt */
  if (saltlen > 0) {
    if (prng_descriptor[prng_idx].read(salt, saltlen, prng) != saltlen) {
      err = CRYPT_ERROR_READPRNG;
      goto LBL_ERR;
    }
  }

  /* M = (eight) 0x00 || msghash || salt, hash = H(M) */
  if ((err = hash_descriptor[hash_idx].init(&md)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  zeromem(DB, 8);
  if ((err = hash_descriptor[hash_idx].process(&md, DB, 8)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash_idx].process(&md, msghash, msghashlen)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash_idx].process(&md, salt, saltlen)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash_idx].done(&md, hash)) != CRYPT_OK) {
    goto LBL_ERR;
  }

  /* generate DB = PS || 0x01 || salt, PS == modulus_len - saltlen - hLen - 2
   * zero bytes */
  x = 0;
  XMEMSET(DB + x, 0, modulus_len - saltlen - hLen - 2);
  x += modulus_len - saltlen - hLen - 2;
  DB[x++] = 0x01;
  XMEMCPY(DB + x, salt, saltlen);
  x += saltlen;

  /* generate mask of length modulus_len - hLen - 1 from hash */
  if ((err = pkcs_1_mgf1(hash_idx, hash, hLen, mask, modulus_len - hLen - 1)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }

  /* xor against DB */
  for (y = 0; y < (modulus_len - hLen - 1); y++) {
    DB[y] ^= mask[y];
  }

  /* output is DB || hash || 0xBC */
  if (*outlen < modulus_len) {
    *outlen = modulus_len;
    err = CRYPT_BUFFER_OVERFLOW;
    goto LBL_ERR;
  }

  /* DB len = modulus_len - hLen - 1 */
  y = 0;
  XMEMCPY(out + y, DB, modulus_len - hLen - 1);
  y += modulus_len - hLen - 1;

  /* hash */
  XMEMCPY(out + y, hash, hLen);
  y += hLen;

  /* 0xBC */
  out[y] = 0xBC;

  /* now clear the 8*modulus_len - modulus_bitlen most significant bits */
  out[0] &= 0xFF >> ((modulus_len << 3) - (modulus_bitlen - 1));

  /* store output size */
  *outlen = modulus_len;
  err = CRYPT_OK;
LBL_ERR:

  XFREE(hash);
  XFREE(salt);
  XFREE(mask);
  XFREE(DB);

  return err;
}

/** @brief LTC_PKCS #1 v1.5 decode.
 *
 *  @param msg              The encoded data to decode
 *  @param msglen           The length of the encoded data (octets)
 *  @param block_type       Block type to use in padding (\sa
 * ltc_pkcs_1_v1_5_blocks)
 *  @param modulus_bitlen   The bit length of the RSA modulus
 *  @param out              [out] Destination of decoding
 *  @param outlen           [in/out] The max size and resulting size of the
 * decoding
 *  @param is_valid         [out] Boolean whether the padding was valid
 *
 *  @return CRYPT_OK if successful (even if invalid)
 */
int pkcs_1_v1_5_decode(const unsigned char *msg, unsigned long msglen,
                       int block_type, unsigned long modulus_bitlen,
                       unsigned char *out, unsigned long *outlen,
                       int *is_valid) {
  unsigned long modulus_len, ps_len, i;
  int result;

  /* default to invalid packet */
  *is_valid = 0;

  modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

  /* test message size */

  if ((msglen > modulus_len) || (modulus_len < 11)) {
    return CRYPT_PK_INVALID_SIZE;
  }

  /* separate encoded message */

  if ((msg[0] != 0x00) || (msg[1] != (unsigned char)block_type)) {
    result = CRYPT_INVALID_PACKET;
    goto bail;
  }

  if (block_type == LTC_LTC_PKCS_1_EME) {
    for (i = 2; i < modulus_len; i++) {
      /* separator */
      if (msg[i] == 0x00) {
        break;
      }
    }
    ps_len = i++ - 2;

    if ((i >= modulus_len) || (ps_len < 8)) {
      /* There was no octet with hexadecimal value 0x00 to separate ps from m,
       * or the length of ps is less than 8 octets.
       */
      result = CRYPT_INVALID_PACKET;
      goto bail;
    }
  } else {
    for (i = 2; i < modulus_len - 1; i++) {
      if (msg[i] != 0xFF) {
        break;
      }
    }

    /* separator check */
    if (msg[i] != 0) {
      /* There was no octet with hexadecimal value 0x00 to separate ps from m.
       */
      result = CRYPT_INVALID_PACKET;
      goto bail;
    }

    ps_len = i - 2;
  }

  if (*outlen < (msglen - (2 + ps_len + 1))) {
    *outlen = msglen - (2 + ps_len + 1);
    result = CRYPT_BUFFER_OVERFLOW;
    goto bail;
  }

  *outlen = (msglen - (2 + ps_len + 1));
  XMEMCPY(out, &msg[2 + ps_len + 1], *outlen);

  /* valid packet */
  *is_valid = 1;
  result = CRYPT_OK;
bail:
  return result;
} /* pkcs_1_v1_5_decode */

/*! \brief LTC_PKCS #1 v1.5 encode.
 *
 *  \param msg              The data to encode
 *  \param msglen           The length of the data to encode (octets)
 *  \param block_type       Block type to use in padding (\sa
 * ltc_pkcs_1_v1_5_blocks) \param modulus_bitlen   The bit length of the RSA
 * modulus \param prng             An active PRNG state (only for
 * LTC_LTC_PKCS_1_EME) \param prng_idx         The index of the PRNG desired
 * (only for LTC_LTC_PKCS_1_EME) \param out              [out] The destination
 * for the encoded data \param outlen           [in/out] The max size and
 * resulting size of the encoded data
 *
 *  \return CRYPT_OK if successful
 */
int pkcs_1_v1_5_encode(const unsigned char *msg, unsigned long msglen,
                       int block_type, unsigned long modulus_bitlen,
                       prng_state *prng, int prng_idx, unsigned char *out,
                       unsigned long *outlen) {
  unsigned long modulus_len, ps_len, i;
  unsigned char *ps;
  int result;

  /* valid block_type? */
  if ((block_type != LTC_LTC_PKCS_1_EMSA) &&
      (block_type != LTC_LTC_PKCS_1_EME)) {
    return CRYPT_PK_INVALID_PADDING;
  }

  if (block_type ==
      LTC_LTC_PKCS_1_EME) { /* encryption padding, we need a valid PRNG */
    if ((result = prng_is_valid(prng_idx)) != CRYPT_OK) {
      return result;
    }
  }

  modulus_len = (modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0);

  /* test message size */
  if ((msglen + 11) > modulus_len) {
    return CRYPT_PK_INVALID_SIZE;
  }

  if (*outlen < modulus_len) {
    *outlen = modulus_len;
    result = CRYPT_BUFFER_OVERFLOW;
    goto bail;
  }

  /* generate an octets string PS */
  ps = &out[2];
  ps_len = modulus_len - msglen - 3;

  if (block_type == LTC_LTC_PKCS_1_EME) {
    /* now choose a random ps */
    if (prng_descriptor[prng_idx].read(ps, ps_len, prng) != ps_len) {
      result = CRYPT_ERROR_READPRNG;
      goto bail;
    }

    /* transform zero bytes (if any) to non-zero random bytes */
    for (i = 0; i < ps_len; i++) {
      while (ps[i] == 0) {
        if (prng_descriptor[prng_idx].read(&ps[i], 1, prng) != 1) {
          result = CRYPT_ERROR_READPRNG;
          goto bail;
        }
      }
    }
  } else {
    XMEMSET(ps, 0xFF, ps_len);
  }

  /* create string of length modulus_len */
  out[0] = 0x00;
  out[1] = (unsigned char)block_type; /* block_type 1 or 2 */
  out[2 + ps_len] = 0x00;
  XMEMCPY(&out[2 + ps_len + 1], msg, msglen);
  *outlen = modulus_len;

  result = CRYPT_OK;
bail:
  return result;
} /* pkcs_1_v1_5_encode */

#define USE_BBS 1

int rand_prime(void *N, long len, prng_state *prng, int wprng) {
  int err, res, type;
  unsigned char *buf;

  LTC_ARGCHK(N != NULL);

  /* get type */
  if (len < 0) {
    type = USE_BBS;
    len = -len;
  } else {
    type = 0;
  }

  /* allow sizes between 2 and 512 bytes for a prime size */
  if ((len < 2) || (len > 512)) {
    return CRYPT_INVALID_PRIME_SIZE;
  }

  /* valid PRNG? Better be! */
  if ((err = prng_is_valid(wprng)) != CRYPT_OK) {
    return err;
  }

  /* allocate buffer to work with */
  buf = XCALLOC(1, len);
  if (buf == NULL) {
    return CRYPT_MEM;
  }

  do {
    /* generate value */
    if (prng_descriptor[wprng].read(buf, len, prng) != (unsigned long)len) {
      XFREE(buf);
      return CRYPT_ERROR_READPRNG;
    }

    /* munge bits */
    buf[0] |= 0x80 | 0x40;
    buf[len - 1] |= 0x01 | ((type & USE_BBS) ? 0x02 : 0x00);

    /* load value */
    if ((err = mp_read_unsigned_bin(N, buf, len)) != CRYPT_OK) {
      XFREE(buf);
      return err;
    }

    /* test */
    if ((err = mp_prime_is_prime(N, 8, &res)) != CRYPT_OK) {
      XFREE(buf);
      return err;
    }
  } while (res == LTC_MP_NO);

  XFREE(buf);
  return CRYPT_OK;
}

/* on *NIX read /dev/random */
static unsigned long rng_nix(unsigned char *buf, unsigned long len,
                             void (*callback)(void)) {
  FILE *f;
  unsigned long x;
  f = fopen("/dev/urandom", "rb");
  if (f == NULL) f = fopen("/dev/random", "rb");

  if (f == NULL) {
    return 0;
  }

  /* disable buffering */
  if (setvbuf(f, NULL, _IONBF, 0) != 0) {
    fclose(f);
    return 0;
  }

  x = (unsigned long)fread(buf, 1, (size_t)len, f);
  fclose(f);
  return x;
}

static unsigned long rng_ansic(unsigned char *buf, unsigned long len,
                               void (*callback)(void)) {
  clock_t t1;
  int l, acc, bits, a, b;

  return 0;

  l = len;
  bits = 8;
  acc = a = b = 0;
  while (len--) {
    if (callback != NULL) callback();
    while (bits--) {
      do {
        t1 = XCLOCK();
        while (t1 == XCLOCK()) a ^= 1;
        t1 = XCLOCK();
        while (t1 == XCLOCK()) b ^= 1;
      } while (a == b);
      acc = (acc << 1) | a;
    }
    *buf++ = acc;
    acc = 0;
    bits = 8;
  }
  acc = bits = a = b = 0;
  return l;
}

/**
   Read the system RNG
   @param out       Destination
   @param outlen    Length desired (octets)
   @param callback  Pointer to void function to act as "callback" when RNG is
   slow.  This can be NULL
   @return Number of octets read
 */
unsigned long rng_get_bytes(unsigned char *out, unsigned long outlen,
                            void (*callback)(void)) {
  unsigned long x;

  LTC_ARGCHK(out != NULL);

  x = rng_nix(out, outlen, callback);
  if (x != 0) {
    return x;
  }
  x = rng_ansic(out, outlen, callback);
  if (x != 0) {
    return x;
  }
  return 0;
}

/**
   @file rng_make_prng.c
   portable way to get secure random bits to feed a PRNG  (Tom St Denis)
 */

/**
   Create a PRNG from a RNG
   @param bits     Number of bits of entropy desired (64 ... 1024)
   @param wprng    Index of which PRNG to setup
   @param prng     [out] PRNG state to initialize
   @param callback A pointer to a void function for when the RNG is slow, this
   can be NULL
   @return CRYPT_OK if successful
 */
int rng_make_prng(int bits, int wprng, prng_state *prng,
                  void (*callback)(void)) {
  unsigned char buf[256];
  int err;

  LTC_ARGCHK(prng != NULL);

  /* check parameter */
  if ((err = prng_is_valid(wprng)) != CRYPT_OK) {
    return err;
  }

  if ((bits < 64) || (bits > 1024)) {
    return CRYPT_INVALID_PRNGSIZE;
  }

  if ((err = prng_descriptor[wprng].start(prng)) != CRYPT_OK) {
    return err;
  }

  bits = ((bits / 8) + ((bits & 7) != 0 ? 1 : 0)) * 2;
  if (rng_get_bytes(buf, (unsigned long)bits, callback) !=
      (unsigned long)bits) {
    return CRYPT_ERROR_READPRNG;
  }

  if ((err = prng_descriptor[wprng].add_entropy(buf, (unsigned long)bits,
                                                prng)) != CRYPT_OK) {
    return err;
  }

  if ((err = prng_descriptor[wprng].ready(prng)) != CRYPT_OK) {
    return err;
  }

  return CRYPT_OK;
}

/**
   LTC_PKCS #1 decrypt then v1.5 or OAEP depad
   @param in          The ciphertext
   @param inlen       The length of the ciphertext (octets)
   @param out         [out] The plaintext
   @param outlen      [in/out] The max size and resulting size of the plaintext
   (octets)
   @param lparam      The system "lparam" value
   @param lparamlen   The length of the lparam value (octets)
   @param hash_idx    The index of the hash desired
   @param padding     Type of padding (LTC_LTC_PKCS_1_OAEP or
   LTC_LTC_PKCS_1_V1_5)
   @param stat        [out] Result of the decryption, 1==valid, 0==invalid
   @param key         The corresponding private RSA key
   @return CRYPT_OK if succcessul (even if invalid)
 */
int rsa_decrypt_key_ex(const unsigned char *in, unsigned long inlen,
                       unsigned char *out, unsigned long *outlen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       int hash_idx, int padding, int *stat, rsa_key *key) {
  unsigned long modulus_bitlen, modulus_bytelen, x;
  int err;
  unsigned char *tmp;

  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(key != NULL);
  LTC_ARGCHK(stat != NULL);

  /* default to invalid */
  *stat = 0;

  /* valid padding? */

  if ((padding != LTC_LTC_PKCS_1_V1_5) && (padding != LTC_LTC_PKCS_1_OAEP)) {
    return CRYPT_PK_INVALID_PADDING;
  }

  if (padding == LTC_LTC_PKCS_1_OAEP) {
    /* valid hash ? */
    if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
      return err;
    }
  }

  /* get modulus len in bits */
  modulus_bitlen = mp_count_bits((key->N));

  /* outlen must be at least the size of the modulus */
  modulus_bytelen = mp_unsigned_bin_size((key->N));
  if (modulus_bytelen != inlen) {
    return CRYPT_INVALID_PACKET;
  }

  /* allocate ram */
  tmp = XMALLOC(inlen);
  if (tmp == NULL) {
    return CRYPT_MEM;
  }

  /* rsa decode the packet */
  x = inlen;
  if ((err = ltc_mp.rsa_me(in, inlen, tmp, &x, PK_PRIVATE, key)) != CRYPT_OK) {
    XFREE(tmp);
    return err;
  }

  if (padding == LTC_LTC_PKCS_1_OAEP) {
    /* now OAEP decode the packet */
    err = pkcs_1_oaep_decode(tmp, x, lparam, lparamlen, modulus_bitlen,
                             hash_idx, out, outlen, stat);
  } else {
    /* now LTC_PKCS #1 v1.5 depad the packet */
    err = pkcs_1_v1_5_decode(tmp, x, LTC_LTC_PKCS_1_EME, modulus_bitlen, out,
                             outlen, stat);
  }

  XFREE(tmp);
  return err;
}

/**
    (LTC_PKCS #1 v2.0) OAEP pad then encrypt
    @param in          The plaintext
    @param inlen       The length of the plaintext (octets)
    @param out         [out] The ciphertext
    @param outlen      [in/out] The max size and resulting size of the
   ciphertext
    @param lparam      The system "lparam" for the encryption
    @param lparamlen   The length of lparam (octets)
    @param prng        An active PRNG
    @param prng_idx    The index of the desired prng
    @param hash_idx    The index of the desired hash
    @param padding     Type of padding (LTC_LTC_PKCS_1_OAEP or
   LTC_LTC_PKCS_1_V1_5)
    @param key         The RSA key to encrypt to
    @return CRYPT_OK if successful
 */
int rsa_encrypt_key_ex(const unsigned char *in, unsigned long inlen,
                       unsigned char *out, unsigned long *outlen,
                       const unsigned char *lparam, unsigned long lparamlen,
                       prng_state *prng, int prng_idx, int hash_idx,
                       int padding, rsa_key *key) {
  unsigned long modulus_bitlen, modulus_bytelen, x;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(key != NULL);

  /* valid padding? */
  if ((padding != LTC_LTC_PKCS_1_V1_5) && (padding != LTC_LTC_PKCS_1_OAEP)) {
    return CRYPT_PK_INVALID_PADDING;
  }

  /* valid prng? */
  if ((err = prng_is_valid(prng_idx)) != CRYPT_OK) {
    return err;
  }

  if (padding == LTC_LTC_PKCS_1_OAEP) {
    /* valid hash? */
    if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
      return err;
    }
  }

  /* get modulus len in bits */
  modulus_bitlen = mp_count_bits((key->N));

  /* outlen must be at least the size of the modulus */
  modulus_bytelen = mp_unsigned_bin_size((key->N));
  if (modulus_bytelen > *outlen) {
    *outlen = modulus_bytelen;
    return CRYPT_BUFFER_OVERFLOW;
  }

  if (padding == LTC_LTC_PKCS_1_OAEP) {
    /* OAEP pad the key */
    x = *outlen;
    if ((err = pkcs_1_oaep_encode(in, inlen, lparam, lparamlen, modulus_bitlen,
                                  prng, prng_idx, hash_idx, out, &x)) !=
        CRYPT_OK) {
      return err;
    }
  } else {
    /* LTC_PKCS #1 v1.5 pad the key */
    x = *outlen;
    if ((err = pkcs_1_v1_5_encode(in, inlen, LTC_LTC_PKCS_1_EME, modulus_bitlen,
                                  prng, prng_idx, out, &x)) != CRYPT_OK) {
      return err;
    }
  }

  /* rsa exptmod the OAEP or LTC_PKCS #1 v1.5 pad */
  return ltc_mp.rsa_me(out, x, out, outlen, PK_PUBLIC, key);
}

/**
   Compute an RSA modular exponentiation
   @param in         The input data to send into RSA
   @param inlen      The length of the input (octets)
   @param out        [out] The destination
   @param outlen     [in/out] The max size and resulting size of the output
   @param which      Which exponent to use, e.g. PK_PRIVATE or PK_PUBLIC
   @param key        The RSA key to use
   @return CRYPT_OK if successful
 */
int rsa_exptmod(const unsigned char *in, unsigned long inlen,
                unsigned char *out, unsigned long *outlen, int which,
                rsa_key *key) {
  void *tmp, *tmpa, *tmpb;
  unsigned long x;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(key != NULL);

  /* is the key of the right type for the operation? */
  if ((which == PK_PRIVATE) && (key->type != PK_PRIVATE)) {
    return CRYPT_PK_NOT_PRIVATE;
  }

  /* must be a private or public operation */
  if ((which != PK_PRIVATE) && (which != PK_PUBLIC)) {
    return CRYPT_PK_INVALID_TYPE;
  }

  /* init and copy into tmp */
  if ((err = mp_init_multi(&tmp, &tmpa, &tmpb, NULL)) != CRYPT_OK) {
    return err;
  }
  if ((err = mp_read_unsigned_bin(tmp, (unsigned char *)in, (int)inlen)) !=
      CRYPT_OK) {
    goto error;
  }

  /* sanity check on the input */
  if (mp_cmp(key->N, tmp) == LTC_MP_LT) {
    err = CRYPT_PK_INVALID_SIZE;
    goto error;
  }

  /* are we using the private exponent and is the key optimized? */
  if (which == PK_PRIVATE) {
    /* tmpa = tmp^dP mod p */
    if ((err = mp_exptmod(tmp, key->dP, key->p, tmpa)) != CRYPT_OK) {
      goto error;
    }

    /* tmpb = tmp^dQ mod q */
    if ((err = mp_exptmod(tmp, key->dQ, key->q, tmpb)) != CRYPT_OK) {
      goto error;
    }

    /* tmp = (tmpa - tmpb) * qInv (mod p) */
    if ((err = mp_sub(tmpa, tmpb, tmp)) != CRYPT_OK) {
      goto error;
    }
    if ((err = mp_mulmod(tmp, key->qP, key->p, tmp)) != CRYPT_OK) {
      goto error;
    }

    /* tmp = tmpb + q * tmp */
    if ((err = mp_mul(tmp, key->q, tmp)) != CRYPT_OK) {
      goto error;
    }
    if ((err = mp_add(tmp, tmpb, tmp)) != CRYPT_OK) {
      goto error;
    }
  } else {
    /* exptmod it */
    if ((err = mp_exptmod(tmp, key->e, key->N, tmp)) != CRYPT_OK) {
      goto error;
    }
  }

  /* read it back */
  x = (unsigned long)mp_unsigned_bin_size(key->N);
  if (x > *outlen) {
    *outlen = x;
    err = CRYPT_BUFFER_OVERFLOW;
    goto error;
  }

  /* this should never happen ... */
  if (mp_unsigned_bin_size(tmp) > mp_unsigned_bin_size(key->N)) {
    err = CRYPT_ERROR;
    goto error;
  }
  *outlen = x;

  /* convert it */
  zeromem(out, x);
  if ((err = mp_to_unsigned_bin(tmp, out + (x - mp_unsigned_bin_size(tmp)))) !=
      CRYPT_OK) {
    goto error;
  }

  /* clean up and return */
  err = CRYPT_OK;
error:
  mp_clear_multi(tmp, tmpa, tmpb, NULL);
  return err;
}

/**
   Free an RSA key from memory
   @param key   The RSA key to free
 */
void rsa_free(rsa_key *key) {
  LTC_ARGCHKVD(key != NULL);
  mp_clear_multi(key->e, key->d, key->N, key->dQ, key->dP, key->qP, key->p,
                 key->q, NULL);
}

/**
   Import an RSAPublicKey or RSAPrivateKey [two-prime only, only support >=
   1024-bit keys, defined in LTC_PKCS #1 v2.1]
   @param in      The packet to import from
   @param inlen   It's length (octets)
   @param key     [out] Destination for newly imported key
   @return CRYPT_OK if successful, upon error allocated memory is freed
 */
int rsa_import(const unsigned char *in, unsigned long inlen, rsa_key *key) {
  int err;
  void *zero;
  unsigned char *tmpbuf;
  unsigned long t, x, y, z, tmpoid[16];
  ltc_asn1_list ssl_pubkey_hashoid[2];
  ltc_asn1_list ssl_pubkey[2];

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(key != NULL);
  LTC_ARGCHK(ltc_mp.name != NULL);

  /* init key */
  if ((err = mp_init_multi(&key->e, &key->d, &key->N, &key->dQ, &key->dP,
                           &key->qP, &key->p, &key->q, NULL)) != CRYPT_OK) {
    return err;
  }

  /* see if the OpenSSL DER format RSA public key will work */
  tmpbuf = XCALLOC(1, MAX_RSA_SIZE * 8);
  if (tmpbuf == NULL) {
    err = CRYPT_MEM;
    goto LBL_ERR;
  }

  /* this includes the internal hash ID and optional params (NULL in this case)
   */
  LTC_SET_ASN1(ssl_pubkey_hashoid, 0, LTC_ASN1_OBJECT_IDENTIFIER, tmpoid,
               sizeof(tmpoid) / sizeof(tmpoid[0]));
  LTC_SET_ASN1(ssl_pubkey_hashoid, 1, LTC_ASN1_NULL, NULL, 0);

  /* the actual format of the SSL DER key is odd, it stores a RSAPublicKey in a
   **BIT** string ... so we have to extract it then proceed to convert bit to
   octet
 */
  LTC_SET_ASN1(ssl_pubkey, 0, LTC_ASN1_SEQUENCE, &ssl_pubkey_hashoid, 2);
  LTC_SET_ASN1(ssl_pubkey, 1, LTC_ASN1_BIT_STRING, tmpbuf, MAX_RSA_SIZE * 8);

  if (der_decode_sequence(in, inlen, ssl_pubkey, 2UL) == CRYPT_OK) {
    /* ok now we have to reassemble the BIT STRING to an OCTET STRING.  Thanks
     * OpenSSL... */
    for (t = y = z = x = 0; x < ssl_pubkey[1].size; x++) {
      y = (y << 1) | tmpbuf[x];
      if (++z == 8) {
        tmpbuf[t++] = (unsigned char)y;
        y = 0;
        z = 0;
      }
    }

    /* now it should be SEQUENCE { INTEGER, INTEGER } */
    if ((err = der_decode_sequence_multi(
             tmpbuf, t, LTC_ASN1_INTEGER, 1UL, key->N, LTC_ASN1_INTEGER, 1UL,
             key->e, LTC_ASN1_EOL, 0UL, NULL)) != CRYPT_OK) {
      XFREE(tmpbuf);
      goto LBL_ERR;
    }
    XFREE(tmpbuf);
    key->type = PK_PUBLIC;
    return CRYPT_OK;
  }
  XFREE(tmpbuf);

  /* not SSL public key, try to match against LTC_PKCS #1 standards */
  if ((err = der_decode_sequence_multi(in, inlen, LTC_ASN1_INTEGER, 1UL, key->N,
                                       LTC_ASN1_EOL, 0UL, NULL)) != CRYPT_OK) {
    goto LBL_ERR;
  }

  if (mp_cmp_d(key->N, 0) == LTC_MP_EQ) {
    if ((err = mp_init(&zero)) != CRYPT_OK) {
      goto LBL_ERR;
    }
    /* it's a private key */
    if ((err = der_decode_sequence_multi(
             in, inlen, LTC_ASN1_INTEGER, 1UL, zero, LTC_ASN1_INTEGER, 1UL,
             key->N, LTC_ASN1_INTEGER, 1UL, key->e, LTC_ASN1_INTEGER, 1UL,
             key->d, LTC_ASN1_INTEGER, 1UL, key->p, LTC_ASN1_INTEGER, 1UL,
             key->q, LTC_ASN1_INTEGER, 1UL, key->dP, LTC_ASN1_INTEGER, 1UL,
             key->dQ, LTC_ASN1_INTEGER, 1UL, key->qP, LTC_ASN1_EOL, 0UL,
             NULL)) != CRYPT_OK) {
      mp_clear(zero);
      goto LBL_ERR;
    }
    mp_clear(zero);
    key->type = PK_PRIVATE;
  } else if (mp_cmp_d(key->N, 1) == LTC_MP_EQ) {
    /* we don't support multi-prime RSA */
    err = CRYPT_PK_INVALID_TYPE;
    goto LBL_ERR;
  } else {
    /* it's a public key and we lack e */
    if ((err = der_decode_sequence_multi(
             in, inlen, LTC_ASN1_INTEGER, 1UL, key->N, LTC_ASN1_INTEGER, 1UL,
             key->e, LTC_ASN1_EOL, 0UL, NULL)) != CRYPT_OK) {
      goto LBL_ERR;
    }
    key->type = PK_PUBLIC;
  }
  return CRYPT_OK;
LBL_ERR:
  mp_clear_multi(key->d, key->e, key->N, key->dQ, key->dP, key->qP, key->p,
                 key->q, NULL);
  return err;
}

/**
   Create an RSA key
   @param prng     An active PRNG state
   @param wprng    The index of the PRNG desired
   @param size     The size of the modulus (key size) desired (octets)
   @param e        The "e" value (public key).  e==65537 is a good choice
   @param key      [out] Destination of a newly created private key pair
   @return CRYPT_OK if successful, upon error all allocated ram is freed
 */
int rsa_make_key(prng_state *prng, int wprng, int size, long e, rsa_key *key) {
  void *p, *q, *tmp1, *tmp2, *tmp3;
  int err;

  LTC_ARGCHK(ltc_mp.name != NULL);
  LTC_ARGCHK(key != NULL);

  if ((size < (MIN_RSA_SIZE / 8)) || (size > (MAX_RSA_SIZE / 8))) {
    return CRYPT_INVALID_KEYSIZE;
  }

  if ((e < 3) || ((e & 1) == 0)) {
    return CRYPT_INVALID_ARG;
  }

  if ((err = prng_is_valid(wprng)) != CRYPT_OK) {
    return err;
  }

  if ((err = mp_init_multi(&p, &q, &tmp1, &tmp2, &tmp3, NULL)) != CRYPT_OK) {
    return err;
  }

  /* make primes p and q (optimization provided by Wayne Scott) */
  if ((err = mp_set_int(tmp3, e)) != CRYPT_OK) {
    goto errkey;
  } /* tmp3 = e */

  /* make prime "p" */
  do {
    if ((err = rand_prime(p, size / 2, prng, wprng)) != CRYPT_OK) {
      goto errkey;
    }
    if ((err = mp_sub_d(p, 1, tmp1)) != CRYPT_OK) {
      goto errkey;
    } /* tmp1 = p-1 */
    if ((err = mp_gcd(tmp1, tmp3, tmp2)) != CRYPT_OK) {
      goto errkey;
    }                               /* tmp2 = gcd(p-1, e) */
  } while (mp_cmp_d(tmp2, 1) != 0); /* while e divides p-1 */

  /* make prime "q" */
  do {
    if ((err = rand_prime(q, size / 2, prng, wprng)) != CRYPT_OK) {
      goto errkey;
    }
    if ((err = mp_sub_d(q, 1, tmp1)) != CRYPT_OK) {
      goto errkey;
    } /* tmp1 = q-1 */
    if ((err = mp_gcd(tmp1, tmp3, tmp2)) != CRYPT_OK) {
      goto errkey;
    }                               /* tmp2 = gcd(q-1, e) */
  } while (mp_cmp_d(tmp2, 1) != 0); /* while e divides q-1 */

  /* tmp1 = lcm(p-1, q-1) */
  if ((err = mp_sub_d(p, 1, tmp2)) != CRYPT_OK) {
    goto errkey;
  } /* tmp2 = p-1 */
  /* tmp1 = q-1 (previous do/while loop) */
  if ((err = mp_lcm(tmp1, tmp2, tmp1)) != CRYPT_OK) {
    goto errkey;
  } /* tmp1 = lcm(p-1, q-1) */

  /* make key */
  if ((err = mp_init_multi(&key->e, &key->d, &key->N, &key->dQ, &key->dP,
                           &key->qP, &key->p, &key->q, NULL)) != CRYPT_OK) {
    goto errkey;
  }

  if ((err = mp_set_int(key->e, e)) != CRYPT_OK) {
    goto errkey;
  } /* key->e =  e */
  if ((err = mp_invmod(key->e, tmp1, key->d)) != CRYPT_OK) {
    goto errkey;
  } /* key->d = 1/e mod lcm(p-1,q-1) */
  if ((err = mp_mul(p, q, key->N)) != CRYPT_OK) {
    goto errkey;
  } /* key->N = pq */

  /* optimize for CRT now */
  /* find d mod q-1 and d mod p-1 */
  if ((err = mp_sub_d(p, 1, tmp1)) != CRYPT_OK) {
    goto errkey;
  } /* tmp1 = q-1 */
  if ((err = mp_sub_d(q, 1, tmp2)) != CRYPT_OK) {
    goto errkey;
  } /* tmp2 = p-1 */
  if ((err = mp_mod(key->d, tmp1, key->dP)) != CRYPT_OK) {
    goto errkey;
  } /* dP = d mod p-1 */
  if ((err = mp_mod(key->d, tmp2, key->dQ)) != CRYPT_OK) {
    goto errkey;
  } /* dQ = d mod q-1 */
  if ((err = mp_invmod(q, p, key->qP)) != CRYPT_OK) {
    goto errkey;
  } /* qP = 1/q mod p */

  if ((err = mp_copy(p, key->p)) != CRYPT_OK) {
    goto errkey;
  }
  if ((err = mp_copy(q, key->q)) != CRYPT_OK) {
    goto errkey;
  }

  /* set key type (in this case it's CRT optimized) */
  key->type = PK_PRIVATE;

  /* return ok and free temps */
  err = CRYPT_OK;
  goto cleanup;
errkey:
  mp_clear_multi(key->d, key->e, key->N, key->dQ, key->dP, key->qP, key->p,
                 key->q, NULL);
cleanup:
  mp_clear_multi(tmp3, tmp2, tmp1, p, q, NULL);
  return err;
}

/**
   LTC_PKCS #1 pad then sign
   @param in        The hash to sign
   @param inlen     The length of the hash to sign (octets)
   @param out       [out] The signature
   @param outlen    [in/out] The max size and resulting size of the signature
   @param padding   Type of padding (LTC_LTC_PKCS_1_PSS or LTC_LTC_PKCS_1_V1_5)
   @param prng      An active PRNG state
   @param prng_idx  The index of the PRNG desired
   @param hash_idx  The index of the hash desired
   @param saltlen   The length of the salt desired (octets)
   @param key       The private RSA key to use
   @return CRYPT_OK if successful
 */
int rsa_sign_hash_ex(const unsigned char *in, unsigned long inlen,
                     unsigned char *out, unsigned long *outlen, int padding,
                     prng_state *prng, int prng_idx, int hash_idx,
                     unsigned long saltlen, rsa_key *key) {
  unsigned long modulus_bitlen, modulus_bytelen, x, y;
  int err;

  LTC_ARGCHK(in != NULL);
  LTC_ARGCHK(out != NULL);
  LTC_ARGCHK(outlen != NULL);
  LTC_ARGCHK(key != NULL);

  /* valid padding? */
  if ((padding != LTC_LTC_PKCS_1_V1_5) && (padding != LTC_LTC_PKCS_1_PSS)) {
    return CRYPT_PK_INVALID_PADDING;
  }

  if (padding == LTC_LTC_PKCS_1_PSS) {
    /* valid prng and hash ? */
    if ((err = prng_is_valid(prng_idx)) != CRYPT_OK) {
      return err;
    }
    if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
      return err;
    }
  }

  /* get modulus len in bits */
  modulus_bitlen = mp_count_bits((key->N));

  /* outlen must be at least the size of the modulus */
  modulus_bytelen = mp_unsigned_bin_size((key->N));
  if (modulus_bytelen > *outlen) {
    *outlen = modulus_bytelen;
    return CRYPT_BUFFER_OVERFLOW;
  }

  if (padding == LTC_LTC_PKCS_1_PSS) {
    /* PSS pad the key */
    x = *outlen;
    if ((err = pkcs_1_pss_encode(in, inlen, saltlen, prng, prng_idx, hash_idx,
                                 modulus_bitlen, out, &x)) != CRYPT_OK) {
      return err;
    }
  } else {
    /* LTC_PKCS #1 v1.5 pad the hash */
    unsigned char *tmpin;
    ltc_asn1_list digestinfo[2], siginfo[2];

    /* not all hashes have OIDs... so sad */
    if (hash_descriptor[hash_idx].OIDlen == 0) {
      return CRYPT_INVALID_ARG;
    }

    /* construct the SEQUENCE
   SEQUENCE {
     SEQUENCE {hashoid OID
               blah    NULL
     }
     hash    OCTET STRING
   }
 */
    LTC_SET_ASN1(digestinfo, 0, LTC_ASN1_OBJECT_IDENTIFIER,
                 hash_descriptor[hash_idx].OID,
                 hash_descriptor[hash_idx].OIDlen);
    LTC_SET_ASN1(digestinfo, 1, LTC_ASN1_NULL, NULL, 0);
    LTC_SET_ASN1(siginfo, 0, LTC_ASN1_SEQUENCE, digestinfo, 2);
    LTC_SET_ASN1(siginfo, 1, LTC_ASN1_OCTET_STRING, in, inlen);

    /* allocate memory for the encoding */
    y = mp_unsigned_bin_size(key->N);
    tmpin = XMALLOC(y);
    if (tmpin == NULL) {
      return CRYPT_MEM;
    }

    if ((err = der_encode_sequence(siginfo, 2, tmpin, &y)) != CRYPT_OK) {
      XFREE(tmpin);
      return err;
    }

    x = *outlen;
    if ((err = pkcs_1_v1_5_encode(tmpin, y, LTC_LTC_PKCS_1_EMSA, modulus_bitlen,
                                  NULL, 0, out, &x)) != CRYPT_OK) {
      XFREE(tmpin);
      return err;
    }
    XFREE(tmpin);
  }

  /* RSA encode it */
  return ltc_mp.rsa_me(out, x, out, outlen, PK_PRIVATE, key);
}

/**
   LTC_PKCS #1 de-sign then v1.5 or PSS depad
   @param sig              The signature data
   @param siglen           The length of the signature data (octets)
   @param hash             The hash of the message that was signed
   @param hashlen          The length of the hash of the message that was signed
   (octets)
   @param padding          Type of padding (LTC_LTC_PKCS_1_PSS or
   LTC_LTC_PKCS_1_V1_5)
   @param hash_idx         The index of the desired hash
   @param saltlen          The length of the salt used during signature
   @param stat             [out] The result of the signature comparison,
   1==valid, 0==invalid
   @param key              The public RSA key corresponding to the key that
   performed the signature
   @return CRYPT_OK on success (even if the signature is invalid)
 */
int rsa_verify_hash_ex(const unsigned char *sig, unsigned long siglen,
                       const unsigned char *hash, unsigned long hashlen,
                       int padding, int hash_idx, unsigned long saltlen,
                       int *stat, rsa_key *key) {
  unsigned long modulus_bitlen, modulus_bytelen, x;
  int err;
  unsigned char *tmpbuf;

  LTC_ARGCHK(hash != NULL);
  LTC_ARGCHK(sig != NULL);
  LTC_ARGCHK(stat != NULL);
  LTC_ARGCHK(key != NULL);

  /* default to invalid */
  *stat = 0;

  /* valid padding? */

  if ((padding != LTC_LTC_PKCS_1_V1_5) && (padding != LTC_LTC_PKCS_1_PSS)) {
    return CRYPT_PK_INVALID_PADDING;
  }

  if (padding == LTC_LTC_PKCS_1_PSS) {
    /* valid hash ? */
    if ((err = hash_is_valid(hash_idx)) != CRYPT_OK) {
      return err;
    }
  }

  /* get modulus len in bits */
  modulus_bitlen = mp_count_bits((key->N));

  /* outlen must be at least the size of the modulus */
  modulus_bytelen = mp_unsigned_bin_size((key->N));
  if (modulus_bytelen != siglen) {
    return CRYPT_INVALID_PACKET;
  }

  /* allocate temp buffer for decoded sig */
  tmpbuf = XMALLOC(siglen);
  if (tmpbuf == NULL) {
    return CRYPT_MEM;
  }

  /* RSA decode it  */
  x = siglen;
  if ((err = ltc_mp.rsa_me(sig, siglen, tmpbuf, &x, PK_PUBLIC, key)) !=
      CRYPT_OK) {
    XFREE(tmpbuf);
    return err;
  }

  /* make sure the output is the right size */
  if (x != siglen) {
    XFREE(tmpbuf);
    return CRYPT_INVALID_PACKET;
  }

  if (padding == LTC_LTC_PKCS_1_PSS) {
    /* PSS decode and verify it */
    err = pkcs_1_pss_decode(hash, hashlen, tmpbuf, x, saltlen, hash_idx,
                            modulus_bitlen, stat);
  } else {
    /* LTC_PKCS #1 v1.5 decode it */
    unsigned char *out;
    unsigned long outlen, loid[16];
    int decoded;
    ltc_asn1_list digestinfo[2], siginfo[2];

    /* not all hashes have OIDs... so sad */
    if (hash_descriptor[hash_idx].OIDlen == 0) {
      err = CRYPT_INVALID_ARG;
      goto bail_2;
    }

    /* allocate temp buffer for decoded hash */
    outlen = ((modulus_bitlen >> 3) + (modulus_bitlen & 7 ? 1 : 0)) - 3;
    out = XMALLOC(outlen);
    if (out == NULL) {
      err = CRYPT_MEM;
      goto bail_2;
    }

    if ((err = pkcs_1_v1_5_decode(tmpbuf, x, LTC_LTC_PKCS_1_EMSA,
                                  modulus_bitlen, out, &outlen, &decoded)) !=
        CRYPT_OK) {
      XFREE(out);
      goto bail_2;
    }

    /* now we must decode out[0...outlen-1] using ASN.1, test the OID and then
     * test the hash */

    /* construct the SEQUENCE
   SEQUENCE {
     SEQUENCE {hashoid OID
               blah    NULL
     }
     hash    OCTET STRING
   }
 */
    LTC_SET_ASN1(digestinfo, 0, LTC_ASN1_OBJECT_IDENTIFIER, loid,
                 sizeof(loid) / sizeof(loid[0]));
    LTC_SET_ASN1(digestinfo, 1, LTC_ASN1_NULL, NULL, 0);
    LTC_SET_ASN1(siginfo, 0, LTC_ASN1_SEQUENCE, digestinfo, 2);
    LTC_SET_ASN1(siginfo, 1, LTC_ASN1_OCTET_STRING, tmpbuf, siglen);

    if ((err = der_decode_sequence(out, outlen, siginfo, 2)) != CRYPT_OK) {
      XFREE(out);
      goto bail_2;
    }

    /* test OID */
    if ((digestinfo[0].size == hash_descriptor[hash_idx].OIDlen) &&
        (XMEMCMP(digestinfo[0].data, hash_descriptor[hash_idx].OID,
                 sizeof(unsigned long) * hash_descriptor[hash_idx].OIDlen) ==
         0) &&
        (siginfo[1].size == hashlen) &&
        (XMEMCMP(siginfo[1].data, hash, hashlen) == 0)) {
      *stat = 1;
    }

    XFREE(out);
  }

bail_2:
  XFREE(tmpbuf);
  return err;
}

/* A secure PRNG using the RNG functions.  Basically this is a
 * wrapper that allows you to use a secure RNG as a PRNG
 * in the various other functions.
 */

const struct ltc_prng_descriptor sprng_desc = {
    "sprng", &sprng_start, &sprng_add_entropy, &sprng_ready, &sprng_read,
};

/**
   Start the PRNG
   @param prng     [out] The PRNG state to initialize
   @return CRYPT_OK if successful
 */
int sprng_start(prng_state *prng) { return CRYPT_OK; }

/**
   Add entropy to the PRNG state
   @param in       The data to add
   @param inlen    Length of the data to add
   @param prng     PRNG state to update
   @return CRYPT_OK if successful
 */
int sprng_add_entropy(const unsigned char *in, unsigned long inlen,
                      prng_state *prng) {
  return CRYPT_OK;
}

/**
   Make the PRNG ready to read from
   @param prng   The PRNG to make active
   @return CRYPT_OK if successful
 */
int sprng_ready(prng_state *prng) { return CRYPT_OK; }

/**
   Read from the PRNG
   @param out      Destination
   @param outlen   Length of output
   @param prng     The active PRNG to read from
   @return Number of octets read
 */
unsigned long sprng_read(unsigned char *out, unsigned long outlen,
                         prng_state *prng) {
  LTC_ARGCHK(out != NULL);
  return rng_get_bytes(out, outlen, NULL);
}

/**
   @file zeromem.c
   Zero a block of memory, Tom St Denis
 */

/**
   Zero a block of memory
   @param out    The destination of the area to zero
   @param outlen The length of the area to zero (octets)
 */
void zeromem(void *out, size_t outlen) {
  unsigned char *mem = out;

  LTC_ARGCHKVD(out != NULL);
  while (outlen-- > 0) {
    *mem++ = 0;
  }
}

const struct ltc_hash_descriptor sha1_desc = {
    "sha1",

    20,
    64,

    /* OID */
    {
        1,
        3,
        14,
        3,
        2,
        26,
    },
    6,

    &sha1_init,
    &sha1_process,
    &sha1_done,
};

#define F0(x, y, z) (z ^ (x & (y ^ z)))
#define F1(x, y, z) (x ^ y ^ z)
#define F2(x, y, z) ((x & y) | (z & (x | y)))
#define F3(x, y, z) (x ^ y ^ z)

static int sha1_compress(hash_state *md, unsigned char *buf) {
  ulong32 a, b, c, d, e, W[80], i;

  /* copy the state into 512-bits into W[0..15] */
  for (i = 0; i < 16; i++) {
    LOAD32H(W[i], buf + (4 * i));
  }

  /* copy state */
  a = md->sha1.state[0];
  b = md->sha1.state[1];
  c = md->sha1.state[2];
  d = md->sha1.state[3];
  e = md->sha1.state[4];

  /* expand it */
  for (i = 16; i < 80; i++) {
    W[i] = ROL(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
  }

  /* compress */
  /* round one */
#define FF0(a, b, c, d, e, i)                               \
  e = (ROLc(a, 5) + F0(b, c, d) + e + W[i] + 0x5a827999UL); \
  b = ROLc(b, 30);
#define FF1(a, b, c, d, e, i)                               \
  e = (ROLc(a, 5) + F1(b, c, d) + e + W[i] + 0x6ed9eba1UL); \
  b = ROLc(b, 30);
#define FF2(a, b, c, d, e, i)                               \
  e = (ROLc(a, 5) + F2(b, c, d) + e + W[i] + 0x8f1bbcdcUL); \
  b = ROLc(b, 30);
#define FF3(a, b, c, d, e, i)                               \
  e = (ROLc(a, 5) + F3(b, c, d) + e + W[i] + 0xca62c1d6UL); \
  b = ROLc(b, 30);

  for (i = 0; i < 20;) {
    FF0(a, b, c, d, e, i++);
    FF0(e, a, b, c, d, i++);
    FF0(d, e, a, b, c, i++);
    FF0(c, d, e, a, b, i++);
    FF0(b, c, d, e, a, i++);
  }

  /* round two */
  for (; i < 40;) {
    FF1(a, b, c, d, e, i++);
    FF1(e, a, b, c, d, i++);
    FF1(d, e, a, b, c, i++);
    FF1(c, d, e, a, b, i++);
    FF1(b, c, d, e, a, i++);
  }

  /* round three */
  for (; i < 60;) {
    FF2(a, b, c, d, e, i++);
    FF2(e, a, b, c, d, i++);
    FF2(d, e, a, b, c, i++);
    FF2(c, d, e, a, b, i++);
    FF2(b, c, d, e, a, i++);
  }

  /* round four */
  for (; i < 80;) {
    FF3(a, b, c, d, e, i++);
    FF3(e, a, b, c, d, i++);
    FF3(d, e, a, b, c, i++);
    FF3(c, d, e, a, b, i++);
    FF3(b, c, d, e, a, i++);
  }

#undef FF0
#undef FF1
#undef FF2
#undef FF3

  /* store */
  md->sha1.state[0] = md->sha1.state[0] + a;
  md->sha1.state[1] = md->sha1.state[1] + b;
  md->sha1.state[2] = md->sha1.state[2] + c;
  md->sha1.state[3] = md->sha1.state[3] + d;
  md->sha1.state[4] = md->sha1.state[4] + e;

  return CRYPT_OK;
}

/**
   Initialize the hash state
   @param md   The hash state you wish to initialize
   @return CRYPT_OK if successful
 */
int sha1_init(hash_state *md) {
  LTC_ARGCHK(md != NULL);
  md->sha1.state[0] = 0x67452301UL;
  md->sha1.state[1] = 0xefcdab89UL;
  md->sha1.state[2] = 0x98badcfeUL;
  md->sha1.state[3] = 0x10325476UL;
  md->sha1.state[4] = 0xc3d2e1f0UL;
  md->sha1.curlen = 0;
  md->sha1.length = 0;
  return CRYPT_OK;
}

/**
   Process a block of memory though the hash
   @param md     The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return CRYPT_OK if successful
 */
HASH_PROCESS(sha1_process, sha1_compress, sha1, 64)

/**
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (20 bytes)
   @return CRYPT_OK if successful
 */
int sha1_done(hash_state *md, unsigned char *out) {
  int i;

  LTC_ARGCHK(md != NULL);
  LTC_ARGCHK(out != NULL);

  if (md->sha1.curlen >= sizeof(md->sha1.buf)) {
    return CRYPT_INVALID_ARG;
  }

  /* increase the length of the message */
  md->sha1.length += md->sha1.curlen * 8;

  /* append the '1' bit */
  md->sha1.buf[md->sha1.curlen++] = (unsigned char)0x80;

  /* if the length is currently above 56 bytes we append zeros
   * then compress.  Then we can fall back to padding zeros and length
   * encoding like normal.
   */
  if (md->sha1.curlen > 56) {
    while (md->sha1.curlen < 64) {
      md->sha1.buf[md->sha1.curlen++] = (unsigned char)0;
    }
    sha1_compress(md, md->sha1.buf);
    md->sha1.curlen = 0;
  }

  /* pad upto 56 bytes of zeroes */
  while (md->sha1.curlen < 56) {
    md->sha1.buf[md->sha1.curlen++] = (unsigned char)0;
  }

  /* store length */
  STORE64H(md->sha1.length, md->sha1.buf + 56);
  sha1_compress(md, md->sha1.buf);

  /* copy output */
  for (i = 0; i < 5; i++) {
    STORE32H(md->sha1.state[i], out + (4 * i));
  }
  return CRYPT_OK;
}

const struct ltc_hash_descriptor sha256_desc = {
    "sha256",
    32,
    64,

    /* OID */
    {
        2,
        16,
        840,
        1,
        101,
        3,
        4,
        2,
        1,
    },
    9,

    &sha256_init,
    &sha256_process,
    &sha256_done,
};

/* Various logical functions */
#define Ch(x, y, z) (z ^ (x & (y ^ z)))
#define Maj(x, y, z) (((x | y) & z) | (x & y))
#define S(x, n) RORc((x), (n))
#define R(x, n) (((x)&0xFFFFFFFFUL) >> (n))
#define Sigma0(x) (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x) (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x) (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x) (S(x, 17) ^ S(x, 19) ^ R(x, 10))

/* compress 512-bits */

static int sha256_compress(hash_state *md, unsigned char *buf) {
  ulong32 S[8], W[64], t0, t1;
  int i;

  /* copy state into S */
  for (i = 0; i < 8; i++) {
    S[i] = md->sha256.state[i];
  }

  /* copy the state into 512-bits into W[0..15] */
  for (i = 0; i < 16; i++) {
    LOAD32H(W[i], buf + (4 * i));
  }

  /* fill W[16..63] */
  for (i = 16; i < 64; i++) {
    W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
  }

  /* Compress */
#define RND(a, b, c, d, e, f, g, h, i, ki)      \
  t0 = h + Sigma1(e) + Ch(e, f, g) + ki + W[i]; \
  t1 = Sigma0(a) + Maj(a, b, c);                \
  d += t0;                                      \
  h = t0 + t1;

  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 0, 0x428a2f98);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 1, 0x71374491);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 2, 0xb5c0fbcf);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 3, 0xe9b5dba5);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 4, 0x3956c25b);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 5, 0x59f111f1);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 6, 0x923f82a4);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 7, 0xab1c5ed5);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 8, 0xd807aa98);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 9, 0x12835b01);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 10, 0x243185be);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 11, 0x550c7dc3);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 12, 0x72be5d74);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 13, 0x80deb1fe);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 14, 0x9bdc06a7);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 15, 0xc19bf174);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 16, 0xe49b69c1);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 17, 0xefbe4786);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 18, 0x0fc19dc6);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 19, 0x240ca1cc);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 20, 0x2de92c6f);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 21, 0x4a7484aa);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 22, 0x5cb0a9dc);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 23, 0x76f988da);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 24, 0x983e5152);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 25, 0xa831c66d);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 26, 0xb00327c8);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 27, 0xbf597fc7);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 28, 0xc6e00bf3);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 29, 0xd5a79147);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 30, 0x06ca6351);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 31, 0x14292967);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 32, 0x27b70a85);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 33, 0x2e1b2138);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 34, 0x4d2c6dfc);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 35, 0x53380d13);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 36, 0x650a7354);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 37, 0x766a0abb);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 38, 0x81c2c92e);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 39, 0x92722c85);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 40, 0xa2bfe8a1);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 41, 0xa81a664b);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 42, 0xc24b8b70);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 43, 0xc76c51a3);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 44, 0xd192e819);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 45, 0xd6990624);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 46, 0xf40e3585);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 47, 0x106aa070);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 48, 0x19a4c116);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 49, 0x1e376c08);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 50, 0x2748774c);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 51, 0x34b0bcb5);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 52, 0x391c0cb3);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 53, 0x4ed8aa4a);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 54, 0x5b9cca4f);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 55, 0x682e6ff3);
  RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], 56, 0x748f82ee);
  RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], 57, 0x78a5636f);
  RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], 58, 0x84c87814);
  RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], 59, 0x8cc70208);
  RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], 60, 0x90befffa);
  RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], 61, 0xa4506ceb);
  RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], 62, 0xbef9a3f7);
  RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], 63, 0xc67178f2);

#undef RND

  /* feedback */
  for (i = 0; i < 8; i++) {
    md->sha256.state[i] = md->sha256.state[i] + S[i];
  }
  return CRYPT_OK;
}

/**
   Initialize the hash state
   @param md   The hash state you wish to initialize
   @return CRYPT_OK if successful
*/
int sha256_init(hash_state *md) {
  LTC_ARGCHK(md != NULL);

  md->sha256.curlen = 0;
  md->sha256.length = 0;
  md->sha256.state[0] = 0x6A09E667UL;
  md->sha256.state[1] = 0xBB67AE85UL;
  md->sha256.state[2] = 0x3C6EF372UL;
  md->sha256.state[3] = 0xA54FF53AUL;
  md->sha256.state[4] = 0x510E527FUL;
  md->sha256.state[5] = 0x9B05688CUL;
  md->sha256.state[6] = 0x1F83D9ABUL;
  md->sha256.state[7] = 0x5BE0CD19UL;
  return CRYPT_OK;
}

/**
   Process a block of memory though the hash
   @param md     The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return CRYPT_OK if successful
*/
HASH_PROCESS(sha256_process, sha256_compress, sha256, 64)

/**
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (32 bytes)
   @return CRYPT_OK if successful
*/
int sha256_done(hash_state *md, unsigned char *out) {
  int i;

  LTC_ARGCHK(md != NULL);
  LTC_ARGCHK(out != NULL);

  if (md->sha256.curlen >= sizeof(md->sha256.buf)) {
    return CRYPT_INVALID_ARG;
  }

  /* increase the length of the message */
  md->sha256.length += md->sha256.curlen * 8;

  /* append the '1' bit */
  md->sha256.buf[md->sha256.curlen++] = (unsigned char)0x80;

  /* if the length is currently above 56 bytes we append zeros
   * then compress.  Then we can fall back to padding zeros and length
   * encoding like normal.
   */
  if (md->sha256.curlen > 56) {
    while (md->sha256.curlen < 64) {
      md->sha256.buf[md->sha256.curlen++] = (unsigned char)0;
    }
    sha256_compress(md, md->sha256.buf);
    md->sha256.curlen = 0;
  }

  /* pad upto 56 bytes of zeroes */
  while (md->sha256.curlen < 56) {
    md->sha256.buf[md->sha256.curlen++] = (unsigned char)0;
  }

  /* store length */
  STORE64H(md->sha256.length, md->sha256.buf + 56);
  sha256_compress(md, md->sha256.buf);

  /* copy output */
  for (i = 0; i < 8; i++) {
    STORE32H(md->sha256.state[i], out + (4 * i));
  }
  return CRYPT_OK;
}

/**
   @param sha384.c
   LTC_SHA384 hash included in sha512.c, Tom St Denis
*/

const struct ltc_hash_descriptor sha384_desc = {
    "sha384",
    48,
    128,

    /* OID */
    {
        2,
        16,
        840,
        1,
        101,
        3,
        4,
        2,
        2,
    },
    9,

    &sha384_init,
    &sha512_process,
    &sha384_done,
};

/**
   Initialize the hash state
   @param md   The hash state you wish to initialize
   @return CRYPT_OK if successful
*/
int sha384_init(hash_state *md) {
  LTC_ARGCHK(md != NULL);

  md->sha512.curlen = 0;
  md->sha512.length = 0;
  md->sha512.state[0] = CONST64(0xcbbb9d5dc1059ed8);
  md->sha512.state[1] = CONST64(0x629a292a367cd507);
  md->sha512.state[2] = CONST64(0x9159015a3070dd17);
  md->sha512.state[3] = CONST64(0x152fecd8f70e5939);
  md->sha512.state[4] = CONST64(0x67332667ffc00b31);
  md->sha512.state[5] = CONST64(0x8eb44a8768581511);
  md->sha512.state[6] = CONST64(0xdb0c2e0d64f98fa7);
  md->sha512.state[7] = CONST64(0x47b5481dbefa4fa4);
  return CRYPT_OK;
}

/**
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (48 bytes)
   @return CRYPT_OK if successful
*/
int sha384_done(hash_state *md, unsigned char *out) {
  unsigned char buf[64];

  LTC_ARGCHK(md != NULL);
  LTC_ARGCHK(out != NULL);

  if (md->sha512.curlen >= sizeof(md->sha512.buf)) {
    return CRYPT_INVALID_ARG;
  }

  sha512_done(md, buf);
  XMEMCPY(out, buf, 48);
  return CRYPT_OK;
}

/**
   @param sha512.c
   LTC_SHA512 by Tom St Denis
*/

const struct ltc_hash_descriptor sha512_desc = {
    "sha512",
    64,
    128,

    /* OID */
    {
        2,
        16,
        840,
        1,
        101,
        3,
        4,
        2,
        3,
    },
    9,

    &sha512_init,
    &sha512_process,
    &sha512_done,
};

/* the K array */
static const ulong64 K[80] = {
    CONST64(0x428a2f98d728ae22), CONST64(0x7137449123ef65cd),
    CONST64(0xb5c0fbcfec4d3b2f), CONST64(0xe9b5dba58189dbbc),
    CONST64(0x3956c25bf348b538), CONST64(0x59f111f1b605d019),
    CONST64(0x923f82a4af194f9b), CONST64(0xab1c5ed5da6d8118),
    CONST64(0xd807aa98a3030242), CONST64(0x12835b0145706fbe),
    CONST64(0x243185be4ee4b28c), CONST64(0x550c7dc3d5ffb4e2),
    CONST64(0x72be5d74f27b896f), CONST64(0x80deb1fe3b1696b1),
    CONST64(0x9bdc06a725c71235), CONST64(0xc19bf174cf692694),
    CONST64(0xe49b69c19ef14ad2), CONST64(0xefbe4786384f25e3),
    CONST64(0x0fc19dc68b8cd5b5), CONST64(0x240ca1cc77ac9c65),
    CONST64(0x2de92c6f592b0275), CONST64(0x4a7484aa6ea6e483),
    CONST64(0x5cb0a9dcbd41fbd4), CONST64(0x76f988da831153b5),
    CONST64(0x983e5152ee66dfab), CONST64(0xa831c66d2db43210),
    CONST64(0xb00327c898fb213f), CONST64(0xbf597fc7beef0ee4),
    CONST64(0xc6e00bf33da88fc2), CONST64(0xd5a79147930aa725),
    CONST64(0x06ca6351e003826f), CONST64(0x142929670a0e6e70),
    CONST64(0x27b70a8546d22ffc), CONST64(0x2e1b21385c26c926),
    CONST64(0x4d2c6dfc5ac42aed), CONST64(0x53380d139d95b3df),
    CONST64(0x650a73548baf63de), CONST64(0x766a0abb3c77b2a8),
    CONST64(0x81c2c92e47edaee6), CONST64(0x92722c851482353b),
    CONST64(0xa2bfe8a14cf10364), CONST64(0xa81a664bbc423001),
    CONST64(0xc24b8b70d0f89791), CONST64(0xc76c51a30654be30),
    CONST64(0xd192e819d6ef5218), CONST64(0xd69906245565a910),
    CONST64(0xf40e35855771202a), CONST64(0x106aa07032bbd1b8),
    CONST64(0x19a4c116b8d2d0c8), CONST64(0x1e376c085141ab53),
    CONST64(0x2748774cdf8eeb99), CONST64(0x34b0bcb5e19b48a8),
    CONST64(0x391c0cb3c5c95a63), CONST64(0x4ed8aa4ae3418acb),
    CONST64(0x5b9cca4f7763e373), CONST64(0x682e6ff3d6b2b8a3),
    CONST64(0x748f82ee5defb2fc), CONST64(0x78a5636f43172f60),
    CONST64(0x84c87814a1f0ab72), CONST64(0x8cc702081a6439ec),
    CONST64(0x90befffa23631e28), CONST64(0xa4506cebde82bde9),
    CONST64(0xbef9a3f7b2c67915), CONST64(0xc67178f2e372532b),
    CONST64(0xca273eceea26619c), CONST64(0xd186b8c721c0c207),
    CONST64(0xeada7dd6cde0eb1e), CONST64(0xf57d4f7fee6ed178),
    CONST64(0x06f067aa72176fba), CONST64(0x0a637dc5a2c898a6),
    CONST64(0x113f9804bef90dae), CONST64(0x1b710b35131c471b),
    CONST64(0x28db77f523047d84), CONST64(0x32caab7b40c72493),
    CONST64(0x3c9ebe0a15c9bebc), CONST64(0x431d67c49c100d4c),
    CONST64(0x4cc5d4becb3e42b6), CONST64(0x597f299cfc657e2a),
    CONST64(0x5fcb6fab3ad6faec), CONST64(0x6c44198c4a475817)};

/* Various logical functions */
#undef S
#undef R
#undef Sigma0
#undef Sigma1
#undef Gamma0
#undef Gamma1

#define Ch(x, y, z) (z ^ (x & (y ^ z)))
#define Maj(x, y, z) (((x | y) & z) | (x & y))
#define S(x, n) ROR64c(x, n)
#define R(x, n) (((x)&CONST64(0xFFFFFFFFFFFFFFFF)) >> ((ulong64)n))
#define Sigma0(x) (S(x, 28) ^ S(x, 34) ^ S(x, 39))
#define Sigma1(x) (S(x, 14) ^ S(x, 18) ^ S(x, 41))
#define Gamma0(x) (S(x, 1) ^ S(x, 8) ^ R(x, 7))
#define Gamma1(x) (S(x, 19) ^ S(x, 61) ^ R(x, 6))

/* compress 1024-bits */

static int sha512_compress(hash_state *md, unsigned char *buf) {
  ulong64 S[8], W[80], t0, t1;
  int i;

  /* copy state into S */
  for (i = 0; i < 8; i++) {
    S[i] = md->sha512.state[i];
  }

  /* copy the state into 1024-bits into W[0..15] */
  for (i = 0; i < 16; i++) {
    LOAD64H(W[i], buf + (8 * i));
  }

  /* fill W[16..79] */
  for (i = 16; i < 80; i++) {
    W[i] = Gamma1(W[i - 2]) + W[i - 7] + Gamma0(W[i - 15]) + W[i - 16];
  }

  /* Compress */
#define RND(a, b, c, d, e, f, g, h, i)            \
  t0 = h + Sigma1(e) + Ch(e, f, g) + K[i] + W[i]; \
  t1 = Sigma0(a) + Maj(a, b, c);                  \
  d += t0;                                        \
  h = t0 + t1;

  for (i = 0; i < 80; i += 8) {
    RND(S[0], S[1], S[2], S[3], S[4], S[5], S[6], S[7], i + 0);
    RND(S[7], S[0], S[1], S[2], S[3], S[4], S[5], S[6], i + 1);
    RND(S[6], S[7], S[0], S[1], S[2], S[3], S[4], S[5], i + 2);
    RND(S[5], S[6], S[7], S[0], S[1], S[2], S[3], S[4], i + 3);
    RND(S[4], S[5], S[6], S[7], S[0], S[1], S[2], S[3], i + 4);
    RND(S[3], S[4], S[5], S[6], S[7], S[0], S[1], S[2], i + 5);
    RND(S[2], S[3], S[4], S[5], S[6], S[7], S[0], S[1], i + 6);
    RND(S[1], S[2], S[3], S[4], S[5], S[6], S[7], S[0], i + 7);
  }

  /* feedback */
  for (i = 0; i < 8; i++) {
    md->sha512.state[i] = md->sha512.state[i] + S[i];
  }

  return CRYPT_OK;
}

/* compress 1024-bits */

/**
   Initialize the hash state
   @param md   The hash state you wish to initialize
   @return CRYPT_OK if successful
*/
int sha512_init(hash_state *md) {
  LTC_ARGCHK(md != NULL);
  md->sha512.curlen = 0;
  md->sha512.length = 0;
  md->sha512.state[0] = CONST64(0x6a09e667f3bcc908);
  md->sha512.state[1] = CONST64(0xbb67ae8584caa73b);
  md->sha512.state[2] = CONST64(0x3c6ef372fe94f82b);
  md->sha512.state[3] = CONST64(0xa54ff53a5f1d36f1);
  md->sha512.state[4] = CONST64(0x510e527fade682d1);
  md->sha512.state[5] = CONST64(0x9b05688c2b3e6c1f);
  md->sha512.state[6] = CONST64(0x1f83d9abfb41bd6b);
  md->sha512.state[7] = CONST64(0x5be0cd19137e2179);
  return CRYPT_OK;
}

/**
   Process a block of memory though the hash
   @param md     The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return CRYPT_OK if successful
*/
HASH_PROCESS(sha512_process, sha512_compress, sha512, 128)

/**
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (64 bytes)
   @return CRYPT_OK if successful
*/
int sha512_done(hash_state *md, unsigned char *out) {
  int i;

  LTC_ARGCHK(md != NULL);
  LTC_ARGCHK(out != NULL);

  if (md->sha512.curlen >= sizeof(md->sha512.buf)) {
    return CRYPT_INVALID_ARG;
  }

  /* increase the length of the message */
  md->sha512.length += md->sha512.curlen * CONST64(8);

  /* append the '1' bit */
  md->sha512.buf[md->sha512.curlen++] = (unsigned char)0x80;

  /* if the length is currently above 112 bytes we append zeros
   * then compress.  Then we can fall back to padding zeros and length
   * encoding like normal.
   */
  if (md->sha512.curlen > 112) {
    while (md->sha512.curlen < 128) {
      md->sha512.buf[md->sha512.curlen++] = (unsigned char)0;
    }
    sha512_compress(md, md->sha512.buf);
    md->sha512.curlen = 0;
  }

  /* pad upto 120 bytes of zeroes
   * note: that from 112 to 120 is the 64 MSB of the length.  We assume that you
   * won't hash > 2^64 bits of data... :-)
   */
  while (md->sha512.curlen < 120) {
    md->sha512.buf[md->sha512.curlen++] = (unsigned char)0;
  }

  /* store length */
  STORE64H(md->sha512.length, md->sha512.buf + 120);
  sha512_compress(md, md->sha512.buf);

  /* copy output */
  for (i = 0; i < 8; i++) {
    STORE64H(md->sha512.state[i], out + (8 * i));
  }
  return CRYPT_OK;
}

#define LTC_HMAC_BLOCKSIZE hash_descriptor[hash].blocksize

/**
   Initialize an HMAC context.
   @param hmac     The HMAC state
   @param hash     The index of the hash you want to use
   @param key      The secret key
   @param keylen   The length of the secret key (octets)
   @return CRYPT_OK if successful
*/
int hmac_init(hmac_state *hmac, int hash, const unsigned char *key,
              unsigned long keylen) {
  unsigned char *buf;
  unsigned long hashsize;
  unsigned long i, z;
  int err;

  LTC_ARGCHK(hmac != NULL);
  LTC_ARGCHK(key != NULL);

  /* valid hash? */
  if ((err = hash_is_valid(hash)) != CRYPT_OK) {
    return err;
  }
  hmac->hash = hash;
  hashsize = hash_descriptor[hash].hashsize;

  /* valid key length? */
  if (keylen == 0) {
    return CRYPT_INVALID_KEYSIZE;
  }

  /* allocate ram for buf */
  buf = XMALLOC(LTC_HMAC_BLOCKSIZE);
  if (buf == NULL) {
    return CRYPT_MEM;
  }

  /* allocate memory for key */
  hmac->key = XMALLOC(LTC_HMAC_BLOCKSIZE);
  if (hmac->key == NULL) {
    XFREE(buf);
    return CRYPT_MEM;
  }

  /* (1) make sure we have a large enough key */
  if (keylen > LTC_HMAC_BLOCKSIZE) {
    z = LTC_HMAC_BLOCKSIZE;
    if ((err = hash_memory(hash, key, keylen, hmac->key, &z)) != CRYPT_OK) {
      goto LBL_ERR;
    }
    keylen = hashsize;
  } else {
    XMEMCPY(hmac->key, key, (size_t)keylen);
  }

  if (keylen < LTC_HMAC_BLOCKSIZE) {
    zeromem((hmac->key) + keylen, (size_t)(LTC_HMAC_BLOCKSIZE - keylen));
  }

  /* Create the initial vector for step (3) */
  for (i = 0; i < LTC_HMAC_BLOCKSIZE; i++) {
    buf[i] = hmac->key[i] ^ 0x36;
  }

  /* Pre-pend that to the hash data */
  if ((err = hash_descriptor[hash].init(&hmac->md)) != CRYPT_OK) {
    goto LBL_ERR;
  }

  if ((err = hash_descriptor[hash].process(&hmac->md, buf,
                                           LTC_HMAC_BLOCKSIZE)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  goto done;
LBL_ERR:
  /* free the key since we failed */
  XFREE(hmac->key);
done:

  XFREE(buf);
  return err;
}

/**
  Process data through HMAC
  @param hmac    The hmac state
  @param in      The data to send through HMAC
  @param inlen   The length of the data to HMAC (octets)
  @return CRYPT_OK if successful
*/
int hmac_process(hmac_state *hmac, const unsigned char *in,
                 unsigned long inlen) {
  int err;
  LTC_ARGCHK(hmac != NULL);
  LTC_ARGCHK(in != NULL);
  if ((err = hash_is_valid(hmac->hash)) != CRYPT_OK) {
    return err;
  }
  return hash_descriptor[hmac->hash].process(&hmac->md, in, inlen);
}

#define LTC_HMAC_BLOCKSIZE hash_descriptor[hash].blocksize

/**
   Terminate an HMAC session
   @param hmac    The HMAC state
   @param out     [out] The destination of the HMAC authentication tag
   @param outlen  [in/out]  The max size and resulting size of the HMAC
   authentication tag
   @return CRYPT_OK if successful
*/
int hmac_done(hmac_state *hmac, unsigned char *out, unsigned long *outlen) {
  unsigned char *buf, *isha;
  unsigned long hashsize, i;
  int hash, err;

  LTC_ARGCHK(hmac != NULL);
  LTC_ARGCHK(out != NULL);

  /* test hash */
  hash = hmac->hash;
  if ((err = hash_is_valid(hash)) != CRYPT_OK) {
    return err;
  }

  /* get the hash message digest size */
  hashsize = hash_descriptor[hash].hashsize;

  /* allocate buffers */
  buf = XMALLOC(LTC_HMAC_BLOCKSIZE);
  isha = XMALLOC(hashsize);
  if (buf == NULL || isha == NULL) {
    if (buf != NULL) {
      XFREE(buf);
    }
    if (isha != NULL) {
      XFREE(isha);
    }
    return CRYPT_MEM;
  }

  /* Get the hash of the first HMAC vector plus the data */
  if ((err = hash_descriptor[hash].done(&hmac->md, isha)) != CRYPT_OK) {
    goto LBL_ERR;
  }

  /* Create the second HMAC vector vector for step (3) */
  for (i = 0; i < LTC_HMAC_BLOCKSIZE; i++) {
    buf[i] = hmac->key[i] ^ 0x5C;
  }

  /* Now calculate the "outer" hash for step (5), (6), and (7) */
  if ((err = hash_descriptor[hash].init(&hmac->md)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash].process(&hmac->md, buf,
                                           LTC_HMAC_BLOCKSIZE)) != CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash].process(&hmac->md, isha, hashsize)) !=
      CRYPT_OK) {
    goto LBL_ERR;
  }
  if ((err = hash_descriptor[hash].done(&hmac->md, buf)) != CRYPT_OK) {
    goto LBL_ERR;
  }

  /* copy to output  */
  for (i = 0; i < hashsize && i < *outlen; i++) {
    out[i] = buf[i];
  }
  *outlen = i;

  err = CRYPT_OK;
LBL_ERR:
  XFREE(hmac->key);

  XFREE(isha);
  XFREE(buf);

  return err;
}

/**
  @file aes_tab.c
  AES tables
*/
static const ulong32 TE0[256] = {
    0xc66363a5UL, 0xf87c7c84UL, 0xee777799UL, 0xf67b7b8dUL, 0xfff2f20dUL,
    0xd66b6bbdUL, 0xde6f6fb1UL, 0x91c5c554UL, 0x60303050UL, 0x02010103UL,
    0xce6767a9UL, 0x562b2b7dUL, 0xe7fefe19UL, 0xb5d7d762UL, 0x4dababe6UL,
    0xec76769aUL, 0x8fcaca45UL, 0x1f82829dUL, 0x89c9c940UL, 0xfa7d7d87UL,
    0xeffafa15UL, 0xb25959ebUL, 0x8e4747c9UL, 0xfbf0f00bUL, 0x41adadecUL,
    0xb3d4d467UL, 0x5fa2a2fdUL, 0x45afafeaUL, 0x239c9cbfUL, 0x53a4a4f7UL,
    0xe4727296UL, 0x9bc0c05bUL, 0x75b7b7c2UL, 0xe1fdfd1cUL, 0x3d9393aeUL,
    0x4c26266aUL, 0x6c36365aUL, 0x7e3f3f41UL, 0xf5f7f702UL, 0x83cccc4fUL,
    0x6834345cUL, 0x51a5a5f4UL, 0xd1e5e534UL, 0xf9f1f108UL, 0xe2717193UL,
    0xabd8d873UL, 0x62313153UL, 0x2a15153fUL, 0x0804040cUL, 0x95c7c752UL,
    0x46232365UL, 0x9dc3c35eUL, 0x30181828UL, 0x379696a1UL, 0x0a05050fUL,
    0x2f9a9ab5UL, 0x0e070709UL, 0x24121236UL, 0x1b80809bUL, 0xdfe2e23dUL,
    0xcdebeb26UL, 0x4e272769UL, 0x7fb2b2cdUL, 0xea75759fUL, 0x1209091bUL,
    0x1d83839eUL, 0x582c2c74UL, 0x341a1a2eUL, 0x361b1b2dUL, 0xdc6e6eb2UL,
    0xb45a5aeeUL, 0x5ba0a0fbUL, 0xa45252f6UL, 0x763b3b4dUL, 0xb7d6d661UL,
    0x7db3b3ceUL, 0x5229297bUL, 0xdde3e33eUL, 0x5e2f2f71UL, 0x13848497UL,
    0xa65353f5UL, 0xb9d1d168UL, 0x00000000UL, 0xc1eded2cUL, 0x40202060UL,
    0xe3fcfc1fUL, 0x79b1b1c8UL, 0xb65b5bedUL, 0xd46a6abeUL, 0x8dcbcb46UL,
    0x67bebed9UL, 0x7239394bUL, 0x944a4adeUL, 0x984c4cd4UL, 0xb05858e8UL,
    0x85cfcf4aUL, 0xbbd0d06bUL, 0xc5efef2aUL, 0x4faaaae5UL, 0xedfbfb16UL,
    0x864343c5UL, 0x9a4d4dd7UL, 0x66333355UL, 0x11858594UL, 0x8a4545cfUL,
    0xe9f9f910UL, 0x04020206UL, 0xfe7f7f81UL, 0xa05050f0UL, 0x783c3c44UL,
    0x259f9fbaUL, 0x4ba8a8e3UL, 0xa25151f3UL, 0x5da3a3feUL, 0x804040c0UL,
    0x058f8f8aUL, 0x3f9292adUL, 0x219d9dbcUL, 0x70383848UL, 0xf1f5f504UL,
    0x63bcbcdfUL, 0x77b6b6c1UL, 0xafdada75UL, 0x42212163UL, 0x20101030UL,
    0xe5ffff1aUL, 0xfdf3f30eUL, 0xbfd2d26dUL, 0x81cdcd4cUL, 0x180c0c14UL,
    0x26131335UL, 0xc3ecec2fUL, 0xbe5f5fe1UL, 0x359797a2UL, 0x884444ccUL,
    0x2e171739UL, 0x93c4c457UL, 0x55a7a7f2UL, 0xfc7e7e82UL, 0x7a3d3d47UL,
    0xc86464acUL, 0xba5d5de7UL, 0x3219192bUL, 0xe6737395UL, 0xc06060a0UL,
    0x19818198UL, 0x9e4f4fd1UL, 0xa3dcdc7fUL, 0x44222266UL, 0x542a2a7eUL,
    0x3b9090abUL, 0x0b888883UL, 0x8c4646caUL, 0xc7eeee29UL, 0x6bb8b8d3UL,
    0x2814143cUL, 0xa7dede79UL, 0xbc5e5ee2UL, 0x160b0b1dUL, 0xaddbdb76UL,
    0xdbe0e03bUL, 0x64323256UL, 0x743a3a4eUL, 0x140a0a1eUL, 0x924949dbUL,
    0x0c06060aUL, 0x4824246cUL, 0xb85c5ce4UL, 0x9fc2c25dUL, 0xbdd3d36eUL,
    0x43acacefUL, 0xc46262a6UL, 0x399191a8UL, 0x319595a4UL, 0xd3e4e437UL,
    0xf279798bUL, 0xd5e7e732UL, 0x8bc8c843UL, 0x6e373759UL, 0xda6d6db7UL,
    0x018d8d8cUL, 0xb1d5d564UL, 0x9c4e4ed2UL, 0x49a9a9e0UL, 0xd86c6cb4UL,
    0xac5656faUL, 0xf3f4f407UL, 0xcfeaea25UL, 0xca6565afUL, 0xf47a7a8eUL,
    0x47aeaee9UL, 0x10080818UL, 0x6fbabad5UL, 0xf0787888UL, 0x4a25256fUL,
    0x5c2e2e72UL, 0x381c1c24UL, 0x57a6a6f1UL, 0x73b4b4c7UL, 0x97c6c651UL,
    0xcbe8e823UL, 0xa1dddd7cUL, 0xe874749cUL, 0x3e1f1f21UL, 0x964b4bddUL,
    0x61bdbddcUL, 0x0d8b8b86UL, 0x0f8a8a85UL, 0xe0707090UL, 0x7c3e3e42UL,
    0x71b5b5c4UL, 0xcc6666aaUL, 0x904848d8UL, 0x06030305UL, 0xf7f6f601UL,
    0x1c0e0e12UL, 0xc26161a3UL, 0x6a35355fUL, 0xae5757f9UL, 0x69b9b9d0UL,
    0x17868691UL, 0x99c1c158UL, 0x3a1d1d27UL, 0x279e9eb9UL, 0xd9e1e138UL,
    0xebf8f813UL, 0x2b9898b3UL, 0x22111133UL, 0xd26969bbUL, 0xa9d9d970UL,
    0x078e8e89UL, 0x339494a7UL, 0x2d9b9bb6UL, 0x3c1e1e22UL, 0x15878792UL,
    0xc9e9e920UL, 0x87cece49UL, 0xaa5555ffUL, 0x50282878UL, 0xa5dfdf7aUL,
    0x038c8c8fUL, 0x59a1a1f8UL, 0x09898980UL, 0x1a0d0d17UL, 0x65bfbfdaUL,
    0xd7e6e631UL, 0x844242c6UL, 0xd06868b8UL, 0x824141c3UL, 0x299999b0UL,
    0x5a2d2d77UL, 0x1e0f0f11UL, 0x7bb0b0cbUL, 0xa85454fcUL, 0x6dbbbbd6UL,
    0x2c16163aUL,
};

#ifndef PELI_TAB
#endif

static const ulong32 TD0[256] = {
    0x51f4a750UL, 0x7e416553UL, 0x1a17a4c3UL, 0x3a275e96UL, 0x3bab6bcbUL,
    0x1f9d45f1UL, 0xacfa58abUL, 0x4be30393UL, 0x2030fa55UL, 0xad766df6UL,
    0x88cc7691UL, 0xf5024c25UL, 0x4fe5d7fcUL, 0xc52acbd7UL, 0x26354480UL,
    0xb562a38fUL, 0xdeb15a49UL, 0x25ba1b67UL, 0x45ea0e98UL, 0x5dfec0e1UL,
    0xc32f7502UL, 0x814cf012UL, 0x8d4697a3UL, 0x6bd3f9c6UL, 0x038f5fe7UL,
    0x15929c95UL, 0xbf6d7aebUL, 0x955259daUL, 0xd4be832dUL, 0x587421d3UL,
    0x49e06929UL, 0x8ec9c844UL, 0x75c2896aUL, 0xf48e7978UL, 0x99583e6bUL,
    0x27b971ddUL, 0xbee14fb6UL, 0xf088ad17UL, 0xc920ac66UL, 0x7dce3ab4UL,
    0x63df4a18UL, 0xe51a3182UL, 0x97513360UL, 0x62537f45UL, 0xb16477e0UL,
    0xbb6bae84UL, 0xfe81a01cUL, 0xf9082b94UL, 0x70486858UL, 0x8f45fd19UL,
    0x94de6c87UL, 0x527bf8b7UL, 0xab73d323UL, 0x724b02e2UL, 0xe31f8f57UL,
    0x6655ab2aUL, 0xb2eb2807UL, 0x2fb5c203UL, 0x86c57b9aUL, 0xd33708a5UL,
    0x302887f2UL, 0x23bfa5b2UL, 0x02036abaUL, 0xed16825cUL, 0x8acf1c2bUL,
    0xa779b492UL, 0xf307f2f0UL, 0x4e69e2a1UL, 0x65daf4cdUL, 0x0605bed5UL,
    0xd134621fUL, 0xc4a6fe8aUL, 0x342e539dUL, 0xa2f355a0UL, 0x058ae132UL,
    0xa4f6eb75UL, 0x0b83ec39UL, 0x4060efaaUL, 0x5e719f06UL, 0xbd6e1051UL,
    0x3e218af9UL, 0x96dd063dUL, 0xdd3e05aeUL, 0x4de6bd46UL, 0x91548db5UL,
    0x71c45d05UL, 0x0406d46fUL, 0x605015ffUL, 0x1998fb24UL, 0xd6bde997UL,
    0x894043ccUL, 0x67d99e77UL, 0xb0e842bdUL, 0x07898b88UL, 0xe7195b38UL,
    0x79c8eedbUL, 0xa17c0a47UL, 0x7c420fe9UL, 0xf8841ec9UL, 0x00000000UL,
    0x09808683UL, 0x322bed48UL, 0x1e1170acUL, 0x6c5a724eUL, 0xfd0efffbUL,
    0x0f853856UL, 0x3daed51eUL, 0x362d3927UL, 0x0a0fd964UL, 0x685ca621UL,
    0x9b5b54d1UL, 0x24362e3aUL, 0x0c0a67b1UL, 0x9357e70fUL, 0xb4ee96d2UL,
    0x1b9b919eUL, 0x80c0c54fUL, 0x61dc20a2UL, 0x5a774b69UL, 0x1c121a16UL,
    0xe293ba0aUL, 0xc0a02ae5UL, 0x3c22e043UL, 0x121b171dUL, 0x0e090d0bUL,
    0xf28bc7adUL, 0x2db6a8b9UL, 0x141ea9c8UL, 0x57f11985UL, 0xaf75074cUL,
    0xee99ddbbUL, 0xa37f60fdUL, 0xf701269fUL, 0x5c72f5bcUL, 0x44663bc5UL,
    0x5bfb7e34UL, 0x8b432976UL, 0xcb23c6dcUL, 0xb6edfc68UL, 0xb8e4f163UL,
    0xd731dccaUL, 0x42638510UL, 0x13972240UL, 0x84c61120UL, 0x854a247dUL,
    0xd2bb3df8UL, 0xaef93211UL, 0xc729a16dUL, 0x1d9e2f4bUL, 0xdcb230f3UL,
    0x0d8652ecUL, 0x77c1e3d0UL, 0x2bb3166cUL, 0xa970b999UL, 0x119448faUL,
    0x47e96422UL, 0xa8fc8cc4UL, 0xa0f03f1aUL, 0x567d2cd8UL, 0x223390efUL,
    0x87494ec7UL, 0xd938d1c1UL, 0x8ccaa2feUL, 0x98d40b36UL, 0xa6f581cfUL,
    0xa57ade28UL, 0xdab78e26UL, 0x3fadbfa4UL, 0x2c3a9de4UL, 0x5078920dUL,
    0x6a5fcc9bUL, 0x547e4662UL, 0xf68d13c2UL, 0x90d8b8e8UL, 0x2e39f75eUL,
    0x82c3aff5UL, 0x9f5d80beUL, 0x69d0937cUL, 0x6fd52da9UL, 0xcf2512b3UL,
    0xc8ac993bUL, 0x10187da7UL, 0xe89c636eUL, 0xdb3bbb7bUL, 0xcd267809UL,
    0x6e5918f4UL, 0xec9ab701UL, 0x834f9aa8UL, 0xe6956e65UL, 0xaaffe67eUL,
    0x21bccf08UL, 0xef15e8e6UL, 0xbae79bd9UL, 0x4a6f36ceUL, 0xea9f09d4UL,
    0x29b07cd6UL, 0x31a4b2afUL, 0x2a3f2331UL, 0xc6a59430UL, 0x35a266c0UL,
    0x744ebc37UL, 0xfc82caa6UL, 0xe090d0b0UL, 0x33a7d815UL, 0xf104984aUL,
    0x41ecdaf7UL, 0x7fcd500eUL, 0x1791f62fUL, 0x764dd68dUL, 0x43efb04dUL,
    0xccaa4d54UL, 0xe49604dfUL, 0x9ed1b5e3UL, 0x4c6a881bUL, 0xc12c1fb8UL,
    0x4665517fUL, 0x9d5eea04UL, 0x018c355dUL, 0xfa877473UL, 0xfb0b412eUL,
    0xb3671d5aUL, 0x92dbd252UL, 0xe9105633UL, 0x6dd64713UL, 0x9ad7618cUL,
    0x37a10c7aUL, 0x59f8148eUL, 0xeb133c89UL, 0xcea927eeUL, 0xb761c935UL,
    0xe11ce5edUL, 0x7a47b13cUL, 0x9cd2df59UL, 0x55f2733fUL, 0x1814ce79UL,
    0x73c737bfUL, 0x53f7cdeaUL, 0x5ffdaa5bUL, 0xdf3d6f14UL, 0x7844db86UL,
    0xcaaff381UL, 0xb968c43eUL, 0x3824342cUL, 0xc2a3405fUL, 0x161dc372UL,
    0xbce2250cUL, 0x283c498bUL, 0xff0d9541UL, 0x39a80171UL, 0x080cb3deUL,
    0xd8b4e49cUL, 0x6456c190UL, 0x7bcb8461UL, 0xd532b670UL, 0x486c5c74UL,
    0xd0b85742UL,
};

static const ulong32 Td4[256] = {
    0x52525252UL, 0x09090909UL, 0x6a6a6a6aUL, 0xd5d5d5d5UL, 0x30303030UL,
    0x36363636UL, 0xa5a5a5a5UL, 0x38383838UL, 0xbfbfbfbfUL, 0x40404040UL,
    0xa3a3a3a3UL, 0x9e9e9e9eUL, 0x81818181UL, 0xf3f3f3f3UL, 0xd7d7d7d7UL,
    0xfbfbfbfbUL, 0x7c7c7c7cUL, 0xe3e3e3e3UL, 0x39393939UL, 0x82828282UL,
    0x9b9b9b9bUL, 0x2f2f2f2fUL, 0xffffffffUL, 0x87878787UL, 0x34343434UL,
    0x8e8e8e8eUL, 0x43434343UL, 0x44444444UL, 0xc4c4c4c4UL, 0xdedededeUL,
    0xe9e9e9e9UL, 0xcbcbcbcbUL, 0x54545454UL, 0x7b7b7b7bUL, 0x94949494UL,
    0x32323232UL, 0xa6a6a6a6UL, 0xc2c2c2c2UL, 0x23232323UL, 0x3d3d3d3dUL,
    0xeeeeeeeeUL, 0x4c4c4c4cUL, 0x95959595UL, 0x0b0b0b0bUL, 0x42424242UL,
    0xfafafafaUL, 0xc3c3c3c3UL, 0x4e4e4e4eUL, 0x08080808UL, 0x2e2e2e2eUL,
    0xa1a1a1a1UL, 0x66666666UL, 0x28282828UL, 0xd9d9d9d9UL, 0x24242424UL,
    0xb2b2b2b2UL, 0x76767676UL, 0x5b5b5b5bUL, 0xa2a2a2a2UL, 0x49494949UL,
    0x6d6d6d6dUL, 0x8b8b8b8bUL, 0xd1d1d1d1UL, 0x25252525UL, 0x72727272UL,
    0xf8f8f8f8UL, 0xf6f6f6f6UL, 0x64646464UL, 0x86868686UL, 0x68686868UL,
    0x98989898UL, 0x16161616UL, 0xd4d4d4d4UL, 0xa4a4a4a4UL, 0x5c5c5c5cUL,
    0xccccccccUL, 0x5d5d5d5dUL, 0x65656565UL, 0xb6b6b6b6UL, 0x92929292UL,
    0x6c6c6c6cUL, 0x70707070UL, 0x48484848UL, 0x50505050UL, 0xfdfdfdfdUL,
    0xededededUL, 0xb9b9b9b9UL, 0xdadadadaUL, 0x5e5e5e5eUL, 0x15151515UL,
    0x46464646UL, 0x57575757UL, 0xa7a7a7a7UL, 0x8d8d8d8dUL, 0x9d9d9d9dUL,
    0x84848484UL, 0x90909090UL, 0xd8d8d8d8UL, 0xababababUL, 0x00000000UL,
    0x8c8c8c8cUL, 0xbcbcbcbcUL, 0xd3d3d3d3UL, 0x0a0a0a0aUL, 0xf7f7f7f7UL,
    0xe4e4e4e4UL, 0x58585858UL, 0x05050505UL, 0xb8b8b8b8UL, 0xb3b3b3b3UL,
    0x45454545UL, 0x06060606UL, 0xd0d0d0d0UL, 0x2c2c2c2cUL, 0x1e1e1e1eUL,
    0x8f8f8f8fUL, 0xcacacacaUL, 0x3f3f3f3fUL, 0x0f0f0f0fUL, 0x02020202UL,
    0xc1c1c1c1UL, 0xafafafafUL, 0xbdbdbdbdUL, 0x03030303UL, 0x01010101UL,
    0x13131313UL, 0x8a8a8a8aUL, 0x6b6b6b6bUL, 0x3a3a3a3aUL, 0x91919191UL,
    0x11111111UL, 0x41414141UL, 0x4f4f4f4fUL, 0x67676767UL, 0xdcdcdcdcUL,
    0xeaeaeaeaUL, 0x97979797UL, 0xf2f2f2f2UL, 0xcfcfcfcfUL, 0xcecececeUL,
    0xf0f0f0f0UL, 0xb4b4b4b4UL, 0xe6e6e6e6UL, 0x73737373UL, 0x96969696UL,
    0xacacacacUL, 0x74747474UL, 0x22222222UL, 0xe7e7e7e7UL, 0xadadadadUL,
    0x35353535UL, 0x85858585UL, 0xe2e2e2e2UL, 0xf9f9f9f9UL, 0x37373737UL,
    0xe8e8e8e8UL, 0x1c1c1c1cUL, 0x75757575UL, 0xdfdfdfdfUL, 0x6e6e6e6eUL,
    0x47474747UL, 0xf1f1f1f1UL, 0x1a1a1a1aUL, 0x71717171UL, 0x1d1d1d1dUL,
    0x29292929UL, 0xc5c5c5c5UL, 0x89898989UL, 0x6f6f6f6fUL, 0xb7b7b7b7UL,
    0x62626262UL, 0x0e0e0e0eUL, 0xaaaaaaaaUL, 0x18181818UL, 0xbebebebeUL,
    0x1b1b1b1bUL, 0xfcfcfcfcUL, 0x56565656UL, 0x3e3e3e3eUL, 0x4b4b4b4bUL,
    0xc6c6c6c6UL, 0xd2d2d2d2UL, 0x79797979UL, 0x20202020UL, 0x9a9a9a9aUL,
    0xdbdbdbdbUL, 0xc0c0c0c0UL, 0xfefefefeUL, 0x78787878UL, 0xcdcdcdcdUL,
    0x5a5a5a5aUL, 0xf4f4f4f4UL, 0x1f1f1f1fUL, 0xddddddddUL, 0xa8a8a8a8UL,
    0x33333333UL, 0x88888888UL, 0x07070707UL, 0xc7c7c7c7UL, 0x31313131UL,
    0xb1b1b1b1UL, 0x12121212UL, 0x10101010UL, 0x59595959UL, 0x27272727UL,
    0x80808080UL, 0xececececUL, 0x5f5f5f5fUL, 0x60606060UL, 0x51515151UL,
    0x7f7f7f7fUL, 0xa9a9a9a9UL, 0x19191919UL, 0xb5b5b5b5UL, 0x4a4a4a4aUL,
    0x0d0d0d0dUL, 0x2d2d2d2dUL, 0xe5e5e5e5UL, 0x7a7a7a7aUL, 0x9f9f9f9fUL,
    0x93939393UL, 0xc9c9c9c9UL, 0x9c9c9c9cUL, 0xefefefefUL, 0xa0a0a0a0UL,
    0xe0e0e0e0UL, 0x3b3b3b3bUL, 0x4d4d4d4dUL, 0xaeaeaeaeUL, 0x2a2a2a2aUL,
    0xf5f5f5f5UL, 0xb0b0b0b0UL, 0xc8c8c8c8UL, 0xebebebebUL, 0xbbbbbbbbUL,
    0x3c3c3c3cUL, 0x83838383UL, 0x53535353UL, 0x99999999UL, 0x61616161UL,
    0x17171717UL, 0x2b2b2b2bUL, 0x04040404UL, 0x7e7e7e7eUL, 0xbabababaUL,
    0x77777777UL, 0xd6d6d6d6UL, 0x26262626UL, 0xe1e1e1e1UL, 0x69696969UL,
    0x14141414UL, 0x63636363UL, 0x55555555UL, 0x21212121UL, 0x0c0c0c0cUL,
    0x7d7d7d7dUL,
};

#define Te0(x) TE0[x]
#define Te1(x) TE1[x]
#define Te2(x) TE2[x]
#define Te3(x) TE3[x]

#define Td0(x) TD0[x]
#define Td1(x) TD1[x]
#define Td2(x) TD2[x]
#define Td3(x) TD3[x]

static const ulong32 TE1[256] = {
    0xa5c66363UL, 0x84f87c7cUL, 0x99ee7777UL, 0x8df67b7bUL, 0x0dfff2f2UL,
    0xbdd66b6bUL, 0xb1de6f6fUL, 0x5491c5c5UL, 0x50603030UL, 0x03020101UL,
    0xa9ce6767UL, 0x7d562b2bUL, 0x19e7fefeUL, 0x62b5d7d7UL, 0xe64dababUL,
    0x9aec7676UL, 0x458fcacaUL, 0x9d1f8282UL, 0x4089c9c9UL, 0x87fa7d7dUL,
    0x15effafaUL, 0xebb25959UL, 0xc98e4747UL, 0x0bfbf0f0UL, 0xec41adadUL,
    0x67b3d4d4UL, 0xfd5fa2a2UL, 0xea45afafUL, 0xbf239c9cUL, 0xf753a4a4UL,
    0x96e47272UL, 0x5b9bc0c0UL, 0xc275b7b7UL, 0x1ce1fdfdUL, 0xae3d9393UL,
    0x6a4c2626UL, 0x5a6c3636UL, 0x417e3f3fUL, 0x02f5f7f7UL, 0x4f83ccccUL,
    0x5c683434UL, 0xf451a5a5UL, 0x34d1e5e5UL, 0x08f9f1f1UL, 0x93e27171UL,
    0x73abd8d8UL, 0x53623131UL, 0x3f2a1515UL, 0x0c080404UL, 0x5295c7c7UL,
    0x65462323UL, 0x5e9dc3c3UL, 0x28301818UL, 0xa1379696UL, 0x0f0a0505UL,
    0xb52f9a9aUL, 0x090e0707UL, 0x36241212UL, 0x9b1b8080UL, 0x3ddfe2e2UL,
    0x26cdebebUL, 0x694e2727UL, 0xcd7fb2b2UL, 0x9fea7575UL, 0x1b120909UL,
    0x9e1d8383UL, 0x74582c2cUL, 0x2e341a1aUL, 0x2d361b1bUL, 0xb2dc6e6eUL,
    0xeeb45a5aUL, 0xfb5ba0a0UL, 0xf6a45252UL, 0x4d763b3bUL, 0x61b7d6d6UL,
    0xce7db3b3UL, 0x7b522929UL, 0x3edde3e3UL, 0x715e2f2fUL, 0x97138484UL,
    0xf5a65353UL, 0x68b9d1d1UL, 0x00000000UL, 0x2cc1ededUL, 0x60402020UL,
    0x1fe3fcfcUL, 0xc879b1b1UL, 0xedb65b5bUL, 0xbed46a6aUL, 0x468dcbcbUL,
    0xd967bebeUL, 0x4b723939UL, 0xde944a4aUL, 0xd4984c4cUL, 0xe8b05858UL,
    0x4a85cfcfUL, 0x6bbbd0d0UL, 0x2ac5efefUL, 0xe54faaaaUL, 0x16edfbfbUL,
    0xc5864343UL, 0xd79a4d4dUL, 0x55663333UL, 0x94118585UL, 0xcf8a4545UL,
    0x10e9f9f9UL, 0x06040202UL, 0x81fe7f7fUL, 0xf0a05050UL, 0x44783c3cUL,
    0xba259f9fUL, 0xe34ba8a8UL, 0xf3a25151UL, 0xfe5da3a3UL, 0xc0804040UL,
    0x8a058f8fUL, 0xad3f9292UL, 0xbc219d9dUL, 0x48703838UL, 0x04f1f5f5UL,
    0xdf63bcbcUL, 0xc177b6b6UL, 0x75afdadaUL, 0x63422121UL, 0x30201010UL,
    0x1ae5ffffUL, 0x0efdf3f3UL, 0x6dbfd2d2UL, 0x4c81cdcdUL, 0x14180c0cUL,
    0x35261313UL, 0x2fc3ececUL, 0xe1be5f5fUL, 0xa2359797UL, 0xcc884444UL,
    0x392e1717UL, 0x5793c4c4UL, 0xf255a7a7UL, 0x82fc7e7eUL, 0x477a3d3dUL,
    0xacc86464UL, 0xe7ba5d5dUL, 0x2b321919UL, 0x95e67373UL, 0xa0c06060UL,
    0x98198181UL, 0xd19e4f4fUL, 0x7fa3dcdcUL, 0x66442222UL, 0x7e542a2aUL,
    0xab3b9090UL, 0x830b8888UL, 0xca8c4646UL, 0x29c7eeeeUL, 0xd36bb8b8UL,
    0x3c281414UL, 0x79a7dedeUL, 0xe2bc5e5eUL, 0x1d160b0bUL, 0x76addbdbUL,
    0x3bdbe0e0UL, 0x56643232UL, 0x4e743a3aUL, 0x1e140a0aUL, 0xdb924949UL,
    0x0a0c0606UL, 0x6c482424UL, 0xe4b85c5cUL, 0x5d9fc2c2UL, 0x6ebdd3d3UL,
    0xef43acacUL, 0xa6c46262UL, 0xa8399191UL, 0xa4319595UL, 0x37d3e4e4UL,
    0x8bf27979UL, 0x32d5e7e7UL, 0x438bc8c8UL, 0x596e3737UL, 0xb7da6d6dUL,
    0x8c018d8dUL, 0x64b1d5d5UL, 0xd29c4e4eUL, 0xe049a9a9UL, 0xb4d86c6cUL,
    0xfaac5656UL, 0x07f3f4f4UL, 0x25cfeaeaUL, 0xafca6565UL, 0x8ef47a7aUL,
    0xe947aeaeUL, 0x18100808UL, 0xd56fbabaUL, 0x88f07878UL, 0x6f4a2525UL,
    0x725c2e2eUL, 0x24381c1cUL, 0xf157a6a6UL, 0xc773b4b4UL, 0x5197c6c6UL,
    0x23cbe8e8UL, 0x7ca1ddddUL, 0x9ce87474UL, 0x213e1f1fUL, 0xdd964b4bUL,
    0xdc61bdbdUL, 0x860d8b8bUL, 0x850f8a8aUL, 0x90e07070UL, 0x427c3e3eUL,
    0xc471b5b5UL, 0xaacc6666UL, 0xd8904848UL, 0x05060303UL, 0x01f7f6f6UL,
    0x121c0e0eUL, 0xa3c26161UL, 0x5f6a3535UL, 0xf9ae5757UL, 0xd069b9b9UL,
    0x91178686UL, 0x5899c1c1UL, 0x273a1d1dUL, 0xb9279e9eUL, 0x38d9e1e1UL,
    0x13ebf8f8UL, 0xb32b9898UL, 0x33221111UL, 0xbbd26969UL, 0x70a9d9d9UL,
    0x89078e8eUL, 0xa7339494UL, 0xb62d9b9bUL, 0x223c1e1eUL, 0x92158787UL,
    0x20c9e9e9UL, 0x4987ceceUL, 0xffaa5555UL, 0x78502828UL, 0x7aa5dfdfUL,
    0x8f038c8cUL, 0xf859a1a1UL, 0x80098989UL, 0x171a0d0dUL, 0xda65bfbfUL,
    0x31d7e6e6UL, 0xc6844242UL, 0xb8d06868UL, 0xc3824141UL, 0xb0299999UL,
    0x775a2d2dUL, 0x111e0f0fUL, 0xcb7bb0b0UL, 0xfca85454UL, 0xd66dbbbbUL,
    0x3a2c1616UL,
};
static const ulong32 TE2[256] = {
    0x63a5c663UL, 0x7c84f87cUL, 0x7799ee77UL, 0x7b8df67bUL, 0xf20dfff2UL,
    0x6bbdd66bUL, 0x6fb1de6fUL, 0xc55491c5UL, 0x30506030UL, 0x01030201UL,
    0x67a9ce67UL, 0x2b7d562bUL, 0xfe19e7feUL, 0xd762b5d7UL, 0xabe64dabUL,
    0x769aec76UL, 0xca458fcaUL, 0x829d1f82UL, 0xc94089c9UL, 0x7d87fa7dUL,
    0xfa15effaUL, 0x59ebb259UL, 0x47c98e47UL, 0xf00bfbf0UL, 0xadec41adUL,
    0xd467b3d4UL, 0xa2fd5fa2UL, 0xafea45afUL, 0x9cbf239cUL, 0xa4f753a4UL,
    0x7296e472UL, 0xc05b9bc0UL, 0xb7c275b7UL, 0xfd1ce1fdUL, 0x93ae3d93UL,
    0x266a4c26UL, 0x365a6c36UL, 0x3f417e3fUL, 0xf702f5f7UL, 0xcc4f83ccUL,
    0x345c6834UL, 0xa5f451a5UL, 0xe534d1e5UL, 0xf108f9f1UL, 0x7193e271UL,
    0xd873abd8UL, 0x31536231UL, 0x153f2a15UL, 0x040c0804UL, 0xc75295c7UL,
    0x23654623UL, 0xc35e9dc3UL, 0x18283018UL, 0x96a13796UL, 0x050f0a05UL,
    0x9ab52f9aUL, 0x07090e07UL, 0x12362412UL, 0x809b1b80UL, 0xe23ddfe2UL,
    0xeb26cdebUL, 0x27694e27UL, 0xb2cd7fb2UL, 0x759fea75UL, 0x091b1209UL,
    0x839e1d83UL, 0x2c74582cUL, 0x1a2e341aUL, 0x1b2d361bUL, 0x6eb2dc6eUL,
    0x5aeeb45aUL, 0xa0fb5ba0UL, 0x52f6a452UL, 0x3b4d763bUL, 0xd661b7d6UL,
    0xb3ce7db3UL, 0x297b5229UL, 0xe33edde3UL, 0x2f715e2fUL, 0x84971384UL,
    0x53f5a653UL, 0xd168b9d1UL, 0x00000000UL, 0xed2cc1edUL, 0x20604020UL,
    0xfc1fe3fcUL, 0xb1c879b1UL, 0x5bedb65bUL, 0x6abed46aUL, 0xcb468dcbUL,
    0xbed967beUL, 0x394b7239UL, 0x4ade944aUL, 0x4cd4984cUL, 0x58e8b058UL,
    0xcf4a85cfUL, 0xd06bbbd0UL, 0xef2ac5efUL, 0xaae54faaUL, 0xfb16edfbUL,
    0x43c58643UL, 0x4dd79a4dUL, 0x33556633UL, 0x85941185UL, 0x45cf8a45UL,
    0xf910e9f9UL, 0x02060402UL, 0x7f81fe7fUL, 0x50f0a050UL, 0x3c44783cUL,
    0x9fba259fUL, 0xa8e34ba8UL, 0x51f3a251UL, 0xa3fe5da3UL, 0x40c08040UL,
    0x8f8a058fUL, 0x92ad3f92UL, 0x9dbc219dUL, 0x38487038UL, 0xf504f1f5UL,
    0xbcdf63bcUL, 0xb6c177b6UL, 0xda75afdaUL, 0x21634221UL, 0x10302010UL,
    0xff1ae5ffUL, 0xf30efdf3UL, 0xd26dbfd2UL, 0xcd4c81cdUL, 0x0c14180cUL,
    0x13352613UL, 0xec2fc3ecUL, 0x5fe1be5fUL, 0x97a23597UL, 0x44cc8844UL,
    0x17392e17UL, 0xc45793c4UL, 0xa7f255a7UL, 0x7e82fc7eUL, 0x3d477a3dUL,
    0x64acc864UL, 0x5de7ba5dUL, 0x192b3219UL, 0x7395e673UL, 0x60a0c060UL,
    0x81981981UL, 0x4fd19e4fUL, 0xdc7fa3dcUL, 0x22664422UL, 0x2a7e542aUL,
    0x90ab3b90UL, 0x88830b88UL, 0x46ca8c46UL, 0xee29c7eeUL, 0xb8d36bb8UL,
    0x143c2814UL, 0xde79a7deUL, 0x5ee2bc5eUL, 0x0b1d160bUL, 0xdb76addbUL,
    0xe03bdbe0UL, 0x32566432UL, 0x3a4e743aUL, 0x0a1e140aUL, 0x49db9249UL,
    0x060a0c06UL, 0x246c4824UL, 0x5ce4b85cUL, 0xc25d9fc2UL, 0xd36ebdd3UL,
    0xacef43acUL, 0x62a6c462UL, 0x91a83991UL, 0x95a43195UL, 0xe437d3e4UL,
    0x798bf279UL, 0xe732d5e7UL, 0xc8438bc8UL, 0x37596e37UL, 0x6db7da6dUL,
    0x8d8c018dUL, 0xd564b1d5UL, 0x4ed29c4eUL, 0xa9e049a9UL, 0x6cb4d86cUL,
    0x56faac56UL, 0xf407f3f4UL, 0xea25cfeaUL, 0x65afca65UL, 0x7a8ef47aUL,
    0xaee947aeUL, 0x08181008UL, 0xbad56fbaUL, 0x7888f078UL, 0x256f4a25UL,
    0x2e725c2eUL, 0x1c24381cUL, 0xa6f157a6UL, 0xb4c773b4UL, 0xc65197c6UL,
    0xe823cbe8UL, 0xdd7ca1ddUL, 0x749ce874UL, 0x1f213e1fUL, 0x4bdd964bUL,
    0xbddc61bdUL, 0x8b860d8bUL, 0x8a850f8aUL, 0x7090e070UL, 0x3e427c3eUL,
    0xb5c471b5UL, 0x66aacc66UL, 0x48d89048UL, 0x03050603UL, 0xf601f7f6UL,
    0x0e121c0eUL, 0x61a3c261UL, 0x355f6a35UL, 0x57f9ae57UL, 0xb9d069b9UL,
    0x86911786UL, 0xc15899c1UL, 0x1d273a1dUL, 0x9eb9279eUL, 0xe138d9e1UL,
    0xf813ebf8UL, 0x98b32b98UL, 0x11332211UL, 0x69bbd269UL, 0xd970a9d9UL,
    0x8e89078eUL, 0x94a73394UL, 0x9bb62d9bUL, 0x1e223c1eUL, 0x87921587UL,
    0xe920c9e9UL, 0xce4987ceUL, 0x55ffaa55UL, 0x28785028UL, 0xdf7aa5dfUL,
    0x8c8f038cUL, 0xa1f859a1UL, 0x89800989UL, 0x0d171a0dUL, 0xbfda65bfUL,
    0xe631d7e6UL, 0x42c68442UL, 0x68b8d068UL, 0x41c38241UL, 0x99b02999UL,
    0x2d775a2dUL, 0x0f111e0fUL, 0xb0cb7bb0UL, 0x54fca854UL, 0xbbd66dbbUL,
    0x163a2c16UL,
};
static const ulong32 TE3[256] = {

    0x6363a5c6UL, 0x7c7c84f8UL, 0x777799eeUL, 0x7b7b8df6UL, 0xf2f20dffUL,
    0x6b6bbdd6UL, 0x6f6fb1deUL, 0xc5c55491UL, 0x30305060UL, 0x01010302UL,
    0x6767a9ceUL, 0x2b2b7d56UL, 0xfefe19e7UL, 0xd7d762b5UL, 0xababe64dUL,
    0x76769aecUL, 0xcaca458fUL, 0x82829d1fUL, 0xc9c94089UL, 0x7d7d87faUL,
    0xfafa15efUL, 0x5959ebb2UL, 0x4747c98eUL, 0xf0f00bfbUL, 0xadadec41UL,
    0xd4d467b3UL, 0xa2a2fd5fUL, 0xafafea45UL, 0x9c9cbf23UL, 0xa4a4f753UL,
    0x727296e4UL, 0xc0c05b9bUL, 0xb7b7c275UL, 0xfdfd1ce1UL, 0x9393ae3dUL,
    0x26266a4cUL, 0x36365a6cUL, 0x3f3f417eUL, 0xf7f702f5UL, 0xcccc4f83UL,
    0x34345c68UL, 0xa5a5f451UL, 0xe5e534d1UL, 0xf1f108f9UL, 0x717193e2UL,
    0xd8d873abUL, 0x31315362UL, 0x15153f2aUL, 0x04040c08UL, 0xc7c75295UL,
    0x23236546UL, 0xc3c35e9dUL, 0x18182830UL, 0x9696a137UL, 0x05050f0aUL,
    0x9a9ab52fUL, 0x0707090eUL, 0x12123624UL, 0x80809b1bUL, 0xe2e23ddfUL,
    0xebeb26cdUL, 0x2727694eUL, 0xb2b2cd7fUL, 0x75759feaUL, 0x09091b12UL,
    0x83839e1dUL, 0x2c2c7458UL, 0x1a1a2e34UL, 0x1b1b2d36UL, 0x6e6eb2dcUL,
    0x5a5aeeb4UL, 0xa0a0fb5bUL, 0x5252f6a4UL, 0x3b3b4d76UL, 0xd6d661b7UL,
    0xb3b3ce7dUL, 0x29297b52UL, 0xe3e33eddUL, 0x2f2f715eUL, 0x84849713UL,
    0x5353f5a6UL, 0xd1d168b9UL, 0x00000000UL, 0xeded2cc1UL, 0x20206040UL,
    0xfcfc1fe3UL, 0xb1b1c879UL, 0x5b5bedb6UL, 0x6a6abed4UL, 0xcbcb468dUL,
    0xbebed967UL, 0x39394b72UL, 0x4a4ade94UL, 0x4c4cd498UL, 0x5858e8b0UL,
    0xcfcf4a85UL, 0xd0d06bbbUL, 0xefef2ac5UL, 0xaaaae54fUL, 0xfbfb16edUL,
    0x4343c586UL, 0x4d4dd79aUL, 0x33335566UL, 0x85859411UL, 0x4545cf8aUL,
    0xf9f910e9UL, 0x02020604UL, 0x7f7f81feUL, 0x5050f0a0UL, 0x3c3c4478UL,
    0x9f9fba25UL, 0xa8a8e34bUL, 0x5151f3a2UL, 0xa3a3fe5dUL, 0x4040c080UL,
    0x8f8f8a05UL, 0x9292ad3fUL, 0x9d9dbc21UL, 0x38384870UL, 0xf5f504f1UL,
    0xbcbcdf63UL, 0xb6b6c177UL, 0xdada75afUL, 0x21216342UL, 0x10103020UL,
    0xffff1ae5UL, 0xf3f30efdUL, 0xd2d26dbfUL, 0xcdcd4c81UL, 0x0c0c1418UL,
    0x13133526UL, 0xecec2fc3UL, 0x5f5fe1beUL, 0x9797a235UL, 0x4444cc88UL,
    0x1717392eUL, 0xc4c45793UL, 0xa7a7f255UL, 0x7e7e82fcUL, 0x3d3d477aUL,
    0x6464acc8UL, 0x5d5de7baUL, 0x19192b32UL, 0x737395e6UL, 0x6060a0c0UL,
    0x81819819UL, 0x4f4fd19eUL, 0xdcdc7fa3UL, 0x22226644UL, 0x2a2a7e54UL,
    0x9090ab3bUL, 0x8888830bUL, 0x4646ca8cUL, 0xeeee29c7UL, 0xb8b8d36bUL,
    0x14143c28UL, 0xdede79a7UL, 0x5e5ee2bcUL, 0x0b0b1d16UL, 0xdbdb76adUL,
    0xe0e03bdbUL, 0x32325664UL, 0x3a3a4e74UL, 0x0a0a1e14UL, 0x4949db92UL,
    0x06060a0cUL, 0x24246c48UL, 0x5c5ce4b8UL, 0xc2c25d9fUL, 0xd3d36ebdUL,
    0xacacef43UL, 0x6262a6c4UL, 0x9191a839UL, 0x9595a431UL, 0xe4e437d3UL,
    0x79798bf2UL, 0xe7e732d5UL, 0xc8c8438bUL, 0x3737596eUL, 0x6d6db7daUL,
    0x8d8d8c01UL, 0xd5d564b1UL, 0x4e4ed29cUL, 0xa9a9e049UL, 0x6c6cb4d8UL,
    0x5656faacUL, 0xf4f407f3UL, 0xeaea25cfUL, 0x6565afcaUL, 0x7a7a8ef4UL,
    0xaeaee947UL, 0x08081810UL, 0xbabad56fUL, 0x787888f0UL, 0x25256f4aUL,
    0x2e2e725cUL, 0x1c1c2438UL, 0xa6a6f157UL, 0xb4b4c773UL, 0xc6c65197UL,
    0xe8e823cbUL, 0xdddd7ca1UL, 0x74749ce8UL, 0x1f1f213eUL, 0x4b4bdd96UL,
    0xbdbddc61UL, 0x8b8b860dUL, 0x8a8a850fUL, 0x707090e0UL, 0x3e3e427cUL,
    0xb5b5c471UL, 0x6666aaccUL, 0x4848d890UL, 0x03030506UL, 0xf6f601f7UL,
    0x0e0e121cUL, 0x6161a3c2UL, 0x35355f6aUL, 0x5757f9aeUL, 0xb9b9d069UL,
    0x86869117UL, 0xc1c15899UL, 0x1d1d273aUL, 0x9e9eb927UL, 0xe1e138d9UL,
    0xf8f813ebUL, 0x9898b32bUL, 0x11113322UL, 0x6969bbd2UL, 0xd9d970a9UL,
    0x8e8e8907UL, 0x9494a733UL, 0x9b9bb62dUL, 0x1e1e223cUL, 0x87879215UL,
    0xe9e920c9UL, 0xcece4987UL, 0x5555ffaaUL, 0x28287850UL, 0xdfdf7aa5UL,
    0x8c8c8f03UL, 0xa1a1f859UL, 0x89898009UL, 0x0d0d171aUL, 0xbfbfda65UL,
    0xe6e631d7UL, 0x4242c684UL, 0x6868b8d0UL, 0x4141c382UL, 0x9999b029UL,
    0x2d2d775aUL, 0x0f0f111eUL, 0xb0b0cb7bUL, 0x5454fca8UL, 0xbbbbd66dUL,
    0x16163a2cUL,
};

#ifndef PELI_TAB
static const ulong32 Te4_0[] = {
    0x00000063UL, 0x0000007cUL, 0x00000077UL, 0x0000007bUL, 0x000000f2UL,
    0x0000006bUL, 0x0000006fUL, 0x000000c5UL, 0x00000030UL, 0x00000001UL,
    0x00000067UL, 0x0000002bUL, 0x000000feUL, 0x000000d7UL, 0x000000abUL,
    0x00000076UL, 0x000000caUL, 0x00000082UL, 0x000000c9UL, 0x0000007dUL,
    0x000000faUL, 0x00000059UL, 0x00000047UL, 0x000000f0UL, 0x000000adUL,
    0x000000d4UL, 0x000000a2UL, 0x000000afUL, 0x0000009cUL, 0x000000a4UL,
    0x00000072UL, 0x000000c0UL, 0x000000b7UL, 0x000000fdUL, 0x00000093UL,
    0x00000026UL, 0x00000036UL, 0x0000003fUL, 0x000000f7UL, 0x000000ccUL,
    0x00000034UL, 0x000000a5UL, 0x000000e5UL, 0x000000f1UL, 0x00000071UL,
    0x000000d8UL, 0x00000031UL, 0x00000015UL, 0x00000004UL, 0x000000c7UL,
    0x00000023UL, 0x000000c3UL, 0x00000018UL, 0x00000096UL, 0x00000005UL,
    0x0000009aUL, 0x00000007UL, 0x00000012UL, 0x00000080UL, 0x000000e2UL,
    0x000000ebUL, 0x00000027UL, 0x000000b2UL, 0x00000075UL, 0x00000009UL,
    0x00000083UL, 0x0000002cUL, 0x0000001aUL, 0x0000001bUL, 0x0000006eUL,
    0x0000005aUL, 0x000000a0UL, 0x00000052UL, 0x0000003bUL, 0x000000d6UL,
    0x000000b3UL, 0x00000029UL, 0x000000e3UL, 0x0000002fUL, 0x00000084UL,
    0x00000053UL, 0x000000d1UL, 0x00000000UL, 0x000000edUL, 0x00000020UL,
    0x000000fcUL, 0x000000b1UL, 0x0000005bUL, 0x0000006aUL, 0x000000cbUL,
    0x000000beUL, 0x00000039UL, 0x0000004aUL, 0x0000004cUL, 0x00000058UL,
    0x000000cfUL, 0x000000d0UL, 0x000000efUL, 0x000000aaUL, 0x000000fbUL,
    0x00000043UL, 0x0000004dUL, 0x00000033UL, 0x00000085UL, 0x00000045UL,
    0x000000f9UL, 0x00000002UL, 0x0000007fUL, 0x00000050UL, 0x0000003cUL,
    0x0000009fUL, 0x000000a8UL, 0x00000051UL, 0x000000a3UL, 0x00000040UL,
    0x0000008fUL, 0x00000092UL, 0x0000009dUL, 0x00000038UL, 0x000000f5UL,
    0x000000bcUL, 0x000000b6UL, 0x000000daUL, 0x00000021UL, 0x00000010UL,
    0x000000ffUL, 0x000000f3UL, 0x000000d2UL, 0x000000cdUL, 0x0000000cUL,
    0x00000013UL, 0x000000ecUL, 0x0000005fUL, 0x00000097UL, 0x00000044UL,
    0x00000017UL, 0x000000c4UL, 0x000000a7UL, 0x0000007eUL, 0x0000003dUL,
    0x00000064UL, 0x0000005dUL, 0x00000019UL, 0x00000073UL, 0x00000060UL,
    0x00000081UL, 0x0000004fUL, 0x000000dcUL, 0x00000022UL, 0x0000002aUL,
    0x00000090UL, 0x00000088UL, 0x00000046UL, 0x000000eeUL, 0x000000b8UL,
    0x00000014UL, 0x000000deUL, 0x0000005eUL, 0x0000000bUL, 0x000000dbUL,
    0x000000e0UL, 0x00000032UL, 0x0000003aUL, 0x0000000aUL, 0x00000049UL,
    0x00000006UL, 0x00000024UL, 0x0000005cUL, 0x000000c2UL, 0x000000d3UL,
    0x000000acUL, 0x00000062UL, 0x00000091UL, 0x00000095UL, 0x000000e4UL,
    0x00000079UL, 0x000000e7UL, 0x000000c8UL, 0x00000037UL, 0x0000006dUL,
    0x0000008dUL, 0x000000d5UL, 0x0000004eUL, 0x000000a9UL, 0x0000006cUL,
    0x00000056UL, 0x000000f4UL, 0x000000eaUL, 0x00000065UL, 0x0000007aUL,
    0x000000aeUL, 0x00000008UL, 0x000000baUL, 0x00000078UL, 0x00000025UL,
    0x0000002eUL, 0x0000001cUL, 0x000000a6UL, 0x000000b4UL, 0x000000c6UL,
    0x000000e8UL, 0x000000ddUL, 0x00000074UL, 0x0000001fUL, 0x0000004bUL,
    0x000000bdUL, 0x0000008bUL, 0x0000008aUL, 0x00000070UL, 0x0000003eUL,
    0x000000b5UL, 0x00000066UL, 0x00000048UL, 0x00000003UL, 0x000000f6UL,
    0x0000000eUL, 0x00000061UL, 0x00000035UL, 0x00000057UL, 0x000000b9UL,
    0x00000086UL, 0x000000c1UL, 0x0000001dUL, 0x0000009eUL, 0x000000e1UL,
    0x000000f8UL, 0x00000098UL, 0x00000011UL, 0x00000069UL, 0x000000d9UL,
    0x0000008eUL, 0x00000094UL, 0x0000009bUL, 0x0000001eUL, 0x00000087UL,
    0x000000e9UL, 0x000000ceUL, 0x00000055UL, 0x00000028UL, 0x000000dfUL,
    0x0000008cUL, 0x000000a1UL, 0x00000089UL, 0x0000000dUL, 0x000000bfUL,
    0x000000e6UL, 0x00000042UL, 0x00000068UL, 0x00000041UL, 0x00000099UL,
    0x0000002dUL, 0x0000000fUL, 0x000000b0UL, 0x00000054UL, 0x000000bbUL,
    0x00000016UL};

static const ulong32 Te4_1[] = {
    0x00006300UL, 0x00007c00UL, 0x00007700UL, 0x00007b00UL, 0x0000f200UL,
    0x00006b00UL, 0x00006f00UL, 0x0000c500UL, 0x00003000UL, 0x00000100UL,
    0x00006700UL, 0x00002b00UL, 0x0000fe00UL, 0x0000d700UL, 0x0000ab00UL,
    0x00007600UL, 0x0000ca00UL, 0x00008200UL, 0x0000c900UL, 0x00007d00UL,
    0x0000fa00UL, 0x00005900UL, 0x00004700UL, 0x0000f000UL, 0x0000ad00UL,
    0x0000d400UL, 0x0000a200UL, 0x0000af00UL, 0x00009c00UL, 0x0000a400UL,
    0x00007200UL, 0x0000c000UL, 0x0000b700UL, 0x0000fd00UL, 0x00009300UL,
    0x00002600UL, 0x00003600UL, 0x00003f00UL, 0x0000f700UL, 0x0000cc00UL,
    0x00003400UL, 0x0000a500UL, 0x0000e500UL, 0x0000f100UL, 0x00007100UL,
    0x0000d800UL, 0x00003100UL, 0x00001500UL, 0x00000400UL, 0x0000c700UL,
    0x00002300UL, 0x0000c300UL, 0x00001800UL, 0x00009600UL, 0x00000500UL,
    0x00009a00UL, 0x00000700UL, 0x00001200UL, 0x00008000UL, 0x0000e200UL,
    0x0000eb00UL, 0x00002700UL, 0x0000b200UL, 0x00007500UL, 0x00000900UL,
    0x00008300UL, 0x00002c00UL, 0x00001a00UL, 0x00001b00UL, 0x00006e00UL,
    0x00005a00UL, 0x0000a000UL, 0x00005200UL, 0x00003b00UL, 0x0000d600UL,
    0x0000b300UL, 0x00002900UL, 0x0000e300UL, 0x00002f00UL, 0x00008400UL,
    0x00005300UL, 0x0000d100UL, 0x00000000UL, 0x0000ed00UL, 0x00002000UL,
    0x0000fc00UL, 0x0000b100UL, 0x00005b00UL, 0x00006a00UL, 0x0000cb00UL,
    0x0000be00UL, 0x00003900UL, 0x00004a00UL, 0x00004c00UL, 0x00005800UL,
    0x0000cf00UL, 0x0000d000UL, 0x0000ef00UL, 0x0000aa00UL, 0x0000fb00UL,
    0x00004300UL, 0x00004d00UL, 0x00003300UL, 0x00008500UL, 0x00004500UL,
    0x0000f900UL, 0x00000200UL, 0x00007f00UL, 0x00005000UL, 0x00003c00UL,
    0x00009f00UL, 0x0000a800UL, 0x00005100UL, 0x0000a300UL, 0x00004000UL,
    0x00008f00UL, 0x00009200UL, 0x00009d00UL, 0x00003800UL, 0x0000f500UL,
    0x0000bc00UL, 0x0000b600UL, 0x0000da00UL, 0x00002100UL, 0x00001000UL,
    0x0000ff00UL, 0x0000f300UL, 0x0000d200UL, 0x0000cd00UL, 0x00000c00UL,
    0x00001300UL, 0x0000ec00UL, 0x00005f00UL, 0x00009700UL, 0x00004400UL,
    0x00001700UL, 0x0000c400UL, 0x0000a700UL, 0x00007e00UL, 0x00003d00UL,
    0x00006400UL, 0x00005d00UL, 0x00001900UL, 0x00007300UL, 0x00006000UL,
    0x00008100UL, 0x00004f00UL, 0x0000dc00UL, 0x00002200UL, 0x00002a00UL,
    0x00009000UL, 0x00008800UL, 0x00004600UL, 0x0000ee00UL, 0x0000b800UL,
    0x00001400UL, 0x0000de00UL, 0x00005e00UL, 0x00000b00UL, 0x0000db00UL,
    0x0000e000UL, 0x00003200UL, 0x00003a00UL, 0x00000a00UL, 0x00004900UL,
    0x00000600UL, 0x00002400UL, 0x00005c00UL, 0x0000c200UL, 0x0000d300UL,
    0x0000ac00UL, 0x00006200UL, 0x00009100UL, 0x00009500UL, 0x0000e400UL,
    0x00007900UL, 0x0000e700UL, 0x0000c800UL, 0x00003700UL, 0x00006d00UL,
    0x00008d00UL, 0x0000d500UL, 0x00004e00UL, 0x0000a900UL, 0x00006c00UL,
    0x00005600UL, 0x0000f400UL, 0x0000ea00UL, 0x00006500UL, 0x00007a00UL,
    0x0000ae00UL, 0x00000800UL, 0x0000ba00UL, 0x00007800UL, 0x00002500UL,
    0x00002e00UL, 0x00001c00UL, 0x0000a600UL, 0x0000b400UL, 0x0000c600UL,
    0x0000e800UL, 0x0000dd00UL, 0x00007400UL, 0x00001f00UL, 0x00004b00UL,
    0x0000bd00UL, 0x00008b00UL, 0x00008a00UL, 0x00007000UL, 0x00003e00UL,
    0x0000b500UL, 0x00006600UL, 0x00004800UL, 0x00000300UL, 0x0000f600UL,
    0x00000e00UL, 0x00006100UL, 0x00003500UL, 0x00005700UL, 0x0000b900UL,
    0x00008600UL, 0x0000c100UL, 0x00001d00UL, 0x00009e00UL, 0x0000e100UL,
    0x0000f800UL, 0x00009800UL, 0x00001100UL, 0x00006900UL, 0x0000d900UL,
    0x00008e00UL, 0x00009400UL, 0x00009b00UL, 0x00001e00UL, 0x00008700UL,
    0x0000e900UL, 0x0000ce00UL, 0x00005500UL, 0x00002800UL, 0x0000df00UL,
    0x00008c00UL, 0x0000a100UL, 0x00008900UL, 0x00000d00UL, 0x0000bf00UL,
    0x0000e600UL, 0x00004200UL, 0x00006800UL, 0x00004100UL, 0x00009900UL,
    0x00002d00UL, 0x00000f00UL, 0x0000b000UL, 0x00005400UL, 0x0000bb00UL,
    0x00001600UL};

static const ulong32 Te4_2[] = {
    0x00630000UL, 0x007c0000UL, 0x00770000UL, 0x007b0000UL, 0x00f20000UL,
    0x006b0000UL, 0x006f0000UL, 0x00c50000UL, 0x00300000UL, 0x00010000UL,
    0x00670000UL, 0x002b0000UL, 0x00fe0000UL, 0x00d70000UL, 0x00ab0000UL,
    0x00760000UL, 0x00ca0000UL, 0x00820000UL, 0x00c90000UL, 0x007d0000UL,
    0x00fa0000UL, 0x00590000UL, 0x00470000UL, 0x00f00000UL, 0x00ad0000UL,
    0x00d40000UL, 0x00a20000UL, 0x00af0000UL, 0x009c0000UL, 0x00a40000UL,
    0x00720000UL, 0x00c00000UL, 0x00b70000UL, 0x00fd0000UL, 0x00930000UL,
    0x00260000UL, 0x00360000UL, 0x003f0000UL, 0x00f70000UL, 0x00cc0000UL,
    0x00340000UL, 0x00a50000UL, 0x00e50000UL, 0x00f10000UL, 0x00710000UL,
    0x00d80000UL, 0x00310000UL, 0x00150000UL, 0x00040000UL, 0x00c70000UL,
    0x00230000UL, 0x00c30000UL, 0x00180000UL, 0x00960000UL, 0x00050000UL,
    0x009a0000UL, 0x00070000UL, 0x00120000UL, 0x00800000UL, 0x00e20000UL,
    0x00eb0000UL, 0x00270000UL, 0x00b20000UL, 0x00750000UL, 0x00090000UL,
    0x00830000UL, 0x002c0000UL, 0x001a0000UL, 0x001b0000UL, 0x006e0000UL,
    0x005a0000UL, 0x00a00000UL, 0x00520000UL, 0x003b0000UL, 0x00d60000UL,
    0x00b30000UL, 0x00290000UL, 0x00e30000UL, 0x002f0000UL, 0x00840000UL,
    0x00530000UL, 0x00d10000UL, 0x00000000UL, 0x00ed0000UL, 0x00200000UL,
    0x00fc0000UL, 0x00b10000UL, 0x005b0000UL, 0x006a0000UL, 0x00cb0000UL,
    0x00be0000UL, 0x00390000UL, 0x004a0000UL, 0x004c0000UL, 0x00580000UL,
    0x00cf0000UL, 0x00d00000UL, 0x00ef0000UL, 0x00aa0000UL, 0x00fb0000UL,
    0x00430000UL, 0x004d0000UL, 0x00330000UL, 0x00850000UL, 0x00450000UL,
    0x00f90000UL, 0x00020000UL, 0x007f0000UL, 0x00500000UL, 0x003c0000UL,
    0x009f0000UL, 0x00a80000UL, 0x00510000UL, 0x00a30000UL, 0x00400000UL,
    0x008f0000UL, 0x00920000UL, 0x009d0000UL, 0x00380000UL, 0x00f50000UL,
    0x00bc0000UL, 0x00b60000UL, 0x00da0000UL, 0x00210000UL, 0x00100000UL,
    0x00ff0000UL, 0x00f30000UL, 0x00d20000UL, 0x00cd0000UL, 0x000c0000UL,
    0x00130000UL, 0x00ec0000UL, 0x005f0000UL, 0x00970000UL, 0x00440000UL,
    0x00170000UL, 0x00c40000UL, 0x00a70000UL, 0x007e0000UL, 0x003d0000UL,
    0x00640000UL, 0x005d0000UL, 0x00190000UL, 0x00730000UL, 0x00600000UL,
    0x00810000UL, 0x004f0000UL, 0x00dc0000UL, 0x00220000UL, 0x002a0000UL,
    0x00900000UL, 0x00880000UL, 0x00460000UL, 0x00ee0000UL, 0x00b80000UL,
    0x00140000UL, 0x00de0000UL, 0x005e0000UL, 0x000b0000UL, 0x00db0000UL,
    0x00e00000UL, 0x00320000UL, 0x003a0000UL, 0x000a0000UL, 0x00490000UL,
    0x00060000UL, 0x00240000UL, 0x005c0000UL, 0x00c20000UL, 0x00d30000UL,
    0x00ac0000UL, 0x00620000UL, 0x00910000UL, 0x00950000UL, 0x00e40000UL,
    0x00790000UL, 0x00e70000UL, 0x00c80000UL, 0x00370000UL, 0x006d0000UL,
    0x008d0000UL, 0x00d50000UL, 0x004e0000UL, 0x00a90000UL, 0x006c0000UL,
    0x00560000UL, 0x00f40000UL, 0x00ea0000UL, 0x00650000UL, 0x007a0000UL,
    0x00ae0000UL, 0x00080000UL, 0x00ba0000UL, 0x00780000UL, 0x00250000UL,
    0x002e0000UL, 0x001c0000UL, 0x00a60000UL, 0x00b40000UL, 0x00c60000UL,
    0x00e80000UL, 0x00dd0000UL, 0x00740000UL, 0x001f0000UL, 0x004b0000UL,
    0x00bd0000UL, 0x008b0000UL, 0x008a0000UL, 0x00700000UL, 0x003e0000UL,
    0x00b50000UL, 0x00660000UL, 0x00480000UL, 0x00030000UL, 0x00f60000UL,
    0x000e0000UL, 0x00610000UL, 0x00350000UL, 0x00570000UL, 0x00b90000UL,
    0x00860000UL, 0x00c10000UL, 0x001d0000UL, 0x009e0000UL, 0x00e10000UL,
    0x00f80000UL, 0x00980000UL, 0x00110000UL, 0x00690000UL, 0x00d90000UL,
    0x008e0000UL, 0x00940000UL, 0x009b0000UL, 0x001e0000UL, 0x00870000UL,
    0x00e90000UL, 0x00ce0000UL, 0x00550000UL, 0x00280000UL, 0x00df0000UL,
    0x008c0000UL, 0x00a10000UL, 0x00890000UL, 0x000d0000UL, 0x00bf0000UL,
    0x00e60000UL, 0x00420000UL, 0x00680000UL, 0x00410000UL, 0x00990000UL,
    0x002d0000UL, 0x000f0000UL, 0x00b00000UL, 0x00540000UL, 0x00bb0000UL,
    0x00160000UL};

static const ulong32 Te4_3[] = {
    0x63000000UL, 0x7c000000UL, 0x77000000UL, 0x7b000000UL, 0xf2000000UL,
    0x6b000000UL, 0x6f000000UL, 0xc5000000UL, 0x30000000UL, 0x01000000UL,
    0x67000000UL, 0x2b000000UL, 0xfe000000UL, 0xd7000000UL, 0xab000000UL,
    0x76000000UL, 0xca000000UL, 0x82000000UL, 0xc9000000UL, 0x7d000000UL,
    0xfa000000UL, 0x59000000UL, 0x47000000UL, 0xf0000000UL, 0xad000000UL,
    0xd4000000UL, 0xa2000000UL, 0xaf000000UL, 0x9c000000UL, 0xa4000000UL,
    0x72000000UL, 0xc0000000UL, 0xb7000000UL, 0xfd000000UL, 0x93000000UL,
    0x26000000UL, 0x36000000UL, 0x3f000000UL, 0xf7000000UL, 0xcc000000UL,
    0x34000000UL, 0xa5000000UL, 0xe5000000UL, 0xf1000000UL, 0x71000000UL,
    0xd8000000UL, 0x31000000UL, 0x15000000UL, 0x04000000UL, 0xc7000000UL,
    0x23000000UL, 0xc3000000UL, 0x18000000UL, 0x96000000UL, 0x05000000UL,
    0x9a000000UL, 0x07000000UL, 0x12000000UL, 0x80000000UL, 0xe2000000UL,
    0xeb000000UL, 0x27000000UL, 0xb2000000UL, 0x75000000UL, 0x09000000UL,
    0x83000000UL, 0x2c000000UL, 0x1a000000UL, 0x1b000000UL, 0x6e000000UL,
    0x5a000000UL, 0xa0000000UL, 0x52000000UL, 0x3b000000UL, 0xd6000000UL,
    0xb3000000UL, 0x29000000UL, 0xe3000000UL, 0x2f000000UL, 0x84000000UL,
    0x53000000UL, 0xd1000000UL, 0x00000000UL, 0xed000000UL, 0x20000000UL,
    0xfc000000UL, 0xb1000000UL, 0x5b000000UL, 0x6a000000UL, 0xcb000000UL,
    0xbe000000UL, 0x39000000UL, 0x4a000000UL, 0x4c000000UL, 0x58000000UL,
    0xcf000000UL, 0xd0000000UL, 0xef000000UL, 0xaa000000UL, 0xfb000000UL,
    0x43000000UL, 0x4d000000UL, 0x33000000UL, 0x85000000UL, 0x45000000UL,
    0xf9000000UL, 0x02000000UL, 0x7f000000UL, 0x50000000UL, 0x3c000000UL,
    0x9f000000UL, 0xa8000000UL, 0x51000000UL, 0xa3000000UL, 0x40000000UL,
    0x8f000000UL, 0x92000000UL, 0x9d000000UL, 0x38000000UL, 0xf5000000UL,
    0xbc000000UL, 0xb6000000UL, 0xda000000UL, 0x21000000UL, 0x10000000UL,
    0xff000000UL, 0xf3000000UL, 0xd2000000UL, 0xcd000000UL, 0x0c000000UL,
    0x13000000UL, 0xec000000UL, 0x5f000000UL, 0x97000000UL, 0x44000000UL,
    0x17000000UL, 0xc4000000UL, 0xa7000000UL, 0x7e000000UL, 0x3d000000UL,
    0x64000000UL, 0x5d000000UL, 0x19000000UL, 0x73000000UL, 0x60000000UL,
    0x81000000UL, 0x4f000000UL, 0xdc000000UL, 0x22000000UL, 0x2a000000UL,
    0x90000000UL, 0x88000000UL, 0x46000000UL, 0xee000000UL, 0xb8000000UL,
    0x14000000UL, 0xde000000UL, 0x5e000000UL, 0x0b000000UL, 0xdb000000UL,
    0xe0000000UL, 0x32000000UL, 0x3a000000UL, 0x0a000000UL, 0x49000000UL,
    0x06000000UL, 0x24000000UL, 0x5c000000UL, 0xc2000000UL, 0xd3000000UL,
    0xac000000UL, 0x62000000UL, 0x91000000UL, 0x95000000UL, 0xe4000000UL,
    0x79000000UL, 0xe7000000UL, 0xc8000000UL, 0x37000000UL, 0x6d000000UL,
    0x8d000000UL, 0xd5000000UL, 0x4e000000UL, 0xa9000000UL, 0x6c000000UL,
    0x56000000UL, 0xf4000000UL, 0xea000000UL, 0x65000000UL, 0x7a000000UL,
    0xae000000UL, 0x08000000UL, 0xba000000UL, 0x78000000UL, 0x25000000UL,
    0x2e000000UL, 0x1c000000UL, 0xa6000000UL, 0xb4000000UL, 0xc6000000UL,
    0xe8000000UL, 0xdd000000UL, 0x74000000UL, 0x1f000000UL, 0x4b000000UL,
    0xbd000000UL, 0x8b000000UL, 0x8a000000UL, 0x70000000UL, 0x3e000000UL,
    0xb5000000UL, 0x66000000UL, 0x48000000UL, 0x03000000UL, 0xf6000000UL,
    0x0e000000UL, 0x61000000UL, 0x35000000UL, 0x57000000UL, 0xb9000000UL,
    0x86000000UL, 0xc1000000UL, 0x1d000000UL, 0x9e000000UL, 0xe1000000UL,
    0xf8000000UL, 0x98000000UL, 0x11000000UL, 0x69000000UL, 0xd9000000UL,
    0x8e000000UL, 0x94000000UL, 0x9b000000UL, 0x1e000000UL, 0x87000000UL,
    0xe9000000UL, 0xce000000UL, 0x55000000UL, 0x28000000UL, 0xdf000000UL,
    0x8c000000UL, 0xa1000000UL, 0x89000000UL, 0x0d000000UL, 0xbf000000UL,
    0xe6000000UL, 0x42000000UL, 0x68000000UL, 0x41000000UL, 0x99000000UL,
    0x2d000000UL, 0x0f000000UL, 0xb0000000UL, 0x54000000UL, 0xbb000000UL,
    0x16000000UL};
#endif /* pelimac */

static const ulong32 TD1[256] = {
    0x5051f4a7UL, 0x537e4165UL, 0xc31a17a4UL, 0x963a275eUL, 0xcb3bab6bUL,
    0xf11f9d45UL, 0xabacfa58UL, 0x934be303UL, 0x552030faUL, 0xf6ad766dUL,
    0x9188cc76UL, 0x25f5024cUL, 0xfc4fe5d7UL, 0xd7c52acbUL, 0x80263544UL,
    0x8fb562a3UL, 0x49deb15aUL, 0x6725ba1bUL, 0x9845ea0eUL, 0xe15dfec0UL,
    0x02c32f75UL, 0x12814cf0UL, 0xa38d4697UL, 0xc66bd3f9UL, 0xe7038f5fUL,
    0x9515929cUL, 0xebbf6d7aUL, 0xda955259UL, 0x2dd4be83UL, 0xd3587421UL,
    0x2949e069UL, 0x448ec9c8UL, 0x6a75c289UL, 0x78f48e79UL, 0x6b99583eUL,
    0xdd27b971UL, 0xb6bee14fUL, 0x17f088adUL, 0x66c920acUL, 0xb47dce3aUL,
    0x1863df4aUL, 0x82e51a31UL, 0x60975133UL, 0x4562537fUL, 0xe0b16477UL,
    0x84bb6baeUL, 0x1cfe81a0UL, 0x94f9082bUL, 0x58704868UL, 0x198f45fdUL,
    0x8794de6cUL, 0xb7527bf8UL, 0x23ab73d3UL, 0xe2724b02UL, 0x57e31f8fUL,
    0x2a6655abUL, 0x07b2eb28UL, 0x032fb5c2UL, 0x9a86c57bUL, 0xa5d33708UL,
    0xf2302887UL, 0xb223bfa5UL, 0xba02036aUL, 0x5ced1682UL, 0x2b8acf1cUL,
    0x92a779b4UL, 0xf0f307f2UL, 0xa14e69e2UL, 0xcd65daf4UL, 0xd50605beUL,
    0x1fd13462UL, 0x8ac4a6feUL, 0x9d342e53UL, 0xa0a2f355UL, 0x32058ae1UL,
    0x75a4f6ebUL, 0x390b83ecUL, 0xaa4060efUL, 0x065e719fUL, 0x51bd6e10UL,
    0xf93e218aUL, 0x3d96dd06UL, 0xaedd3e05UL, 0x464de6bdUL, 0xb591548dUL,
    0x0571c45dUL, 0x6f0406d4UL, 0xff605015UL, 0x241998fbUL, 0x97d6bde9UL,
    0xcc894043UL, 0x7767d99eUL, 0xbdb0e842UL, 0x8807898bUL, 0x38e7195bUL,
    0xdb79c8eeUL, 0x47a17c0aUL, 0xe97c420fUL, 0xc9f8841eUL, 0x00000000UL,
    0x83098086UL, 0x48322bedUL, 0xac1e1170UL, 0x4e6c5a72UL, 0xfbfd0effUL,
    0x560f8538UL, 0x1e3daed5UL, 0x27362d39UL, 0x640a0fd9UL, 0x21685ca6UL,
    0xd19b5b54UL, 0x3a24362eUL, 0xb10c0a67UL, 0x0f9357e7UL, 0xd2b4ee96UL,
    0x9e1b9b91UL, 0x4f80c0c5UL, 0xa261dc20UL, 0x695a774bUL, 0x161c121aUL,
    0x0ae293baUL, 0xe5c0a02aUL, 0x433c22e0UL, 0x1d121b17UL, 0x0b0e090dUL,
    0xadf28bc7UL, 0xb92db6a8UL, 0xc8141ea9UL, 0x8557f119UL, 0x4caf7507UL,
    0xbbee99ddUL, 0xfda37f60UL, 0x9ff70126UL, 0xbc5c72f5UL, 0xc544663bUL,
    0x345bfb7eUL, 0x768b4329UL, 0xdccb23c6UL, 0x68b6edfcUL, 0x63b8e4f1UL,
    0xcad731dcUL, 0x10426385UL, 0x40139722UL, 0x2084c611UL, 0x7d854a24UL,
    0xf8d2bb3dUL, 0x11aef932UL, 0x6dc729a1UL, 0x4b1d9e2fUL, 0xf3dcb230UL,
    0xec0d8652UL, 0xd077c1e3UL, 0x6c2bb316UL, 0x99a970b9UL, 0xfa119448UL,
    0x2247e964UL, 0xc4a8fc8cUL, 0x1aa0f03fUL, 0xd8567d2cUL, 0xef223390UL,
    0xc787494eUL, 0xc1d938d1UL, 0xfe8ccaa2UL, 0x3698d40bUL, 0xcfa6f581UL,
    0x28a57adeUL, 0x26dab78eUL, 0xa43fadbfUL, 0xe42c3a9dUL, 0x0d507892UL,
    0x9b6a5fccUL, 0x62547e46UL, 0xc2f68d13UL, 0xe890d8b8UL, 0x5e2e39f7UL,
    0xf582c3afUL, 0xbe9f5d80UL, 0x7c69d093UL, 0xa96fd52dUL, 0xb3cf2512UL,
    0x3bc8ac99UL, 0xa710187dUL, 0x6ee89c63UL, 0x7bdb3bbbUL, 0x09cd2678UL,
    0xf46e5918UL, 0x01ec9ab7UL, 0xa8834f9aUL, 0x65e6956eUL, 0x7eaaffe6UL,
    0x0821bccfUL, 0xe6ef15e8UL, 0xd9bae79bUL, 0xce4a6f36UL, 0xd4ea9f09UL,
    0xd629b07cUL, 0xaf31a4b2UL, 0x312a3f23UL, 0x30c6a594UL, 0xc035a266UL,
    0x37744ebcUL, 0xa6fc82caUL, 0xb0e090d0UL, 0x1533a7d8UL, 0x4af10498UL,
    0xf741ecdaUL, 0x0e7fcd50UL, 0x2f1791f6UL, 0x8d764dd6UL, 0x4d43efb0UL,
    0x54ccaa4dUL, 0xdfe49604UL, 0xe39ed1b5UL, 0x1b4c6a88UL, 0xb8c12c1fUL,
    0x7f466551UL, 0x049d5eeaUL, 0x5d018c35UL, 0x73fa8774UL, 0x2efb0b41UL,
    0x5ab3671dUL, 0x5292dbd2UL, 0x33e91056UL, 0x136dd647UL, 0x8c9ad761UL,
    0x7a37a10cUL, 0x8e59f814UL, 0x89eb133cUL, 0xeecea927UL, 0x35b761c9UL,
    0xede11ce5UL, 0x3c7a47b1UL, 0x599cd2dfUL, 0x3f55f273UL, 0x791814ceUL,
    0xbf73c737UL, 0xea53f7cdUL, 0x5b5ffdaaUL, 0x14df3d6fUL, 0x867844dbUL,
    0x81caaff3UL, 0x3eb968c4UL, 0x2c382434UL, 0x5fc2a340UL, 0x72161dc3UL,
    0x0cbce225UL, 0x8b283c49UL, 0x41ff0d95UL, 0x7139a801UL, 0xde080cb3UL,
    0x9cd8b4e4UL, 0x906456c1UL, 0x617bcb84UL, 0x70d532b6UL, 0x74486c5cUL,
    0x42d0b857UL,
};
static const ulong32 TD2[256] = {
    0xa75051f4UL, 0x65537e41UL, 0xa4c31a17UL, 0x5e963a27UL, 0x6bcb3babUL,
    0x45f11f9dUL, 0x58abacfaUL, 0x03934be3UL, 0xfa552030UL, 0x6df6ad76UL,
    0x769188ccUL, 0x4c25f502UL, 0xd7fc4fe5UL, 0xcbd7c52aUL, 0x44802635UL,
    0xa38fb562UL, 0x5a49deb1UL, 0x1b6725baUL, 0x0e9845eaUL, 0xc0e15dfeUL,
    0x7502c32fUL, 0xf012814cUL, 0x97a38d46UL, 0xf9c66bd3UL, 0x5fe7038fUL,
    0x9c951592UL, 0x7aebbf6dUL, 0x59da9552UL, 0x832dd4beUL, 0x21d35874UL,
    0x692949e0UL, 0xc8448ec9UL, 0x896a75c2UL, 0x7978f48eUL, 0x3e6b9958UL,
    0x71dd27b9UL, 0x4fb6bee1UL, 0xad17f088UL, 0xac66c920UL, 0x3ab47dceUL,
    0x4a1863dfUL, 0x3182e51aUL, 0x33609751UL, 0x7f456253UL, 0x77e0b164UL,
    0xae84bb6bUL, 0xa01cfe81UL, 0x2b94f908UL, 0x68587048UL, 0xfd198f45UL,
    0x6c8794deUL, 0xf8b7527bUL, 0xd323ab73UL, 0x02e2724bUL, 0x8f57e31fUL,
    0xab2a6655UL, 0x2807b2ebUL, 0xc2032fb5UL, 0x7b9a86c5UL, 0x08a5d337UL,
    0x87f23028UL, 0xa5b223bfUL, 0x6aba0203UL, 0x825ced16UL, 0x1c2b8acfUL,
    0xb492a779UL, 0xf2f0f307UL, 0xe2a14e69UL, 0xf4cd65daUL, 0xbed50605UL,
    0x621fd134UL, 0xfe8ac4a6UL, 0x539d342eUL, 0x55a0a2f3UL, 0xe132058aUL,
    0xeb75a4f6UL, 0xec390b83UL, 0xefaa4060UL, 0x9f065e71UL, 0x1051bd6eUL,
    0x8af93e21UL, 0x063d96ddUL, 0x05aedd3eUL, 0xbd464de6UL, 0x8db59154UL,
    0x5d0571c4UL, 0xd46f0406UL, 0x15ff6050UL, 0xfb241998UL, 0xe997d6bdUL,
    0x43cc8940UL, 0x9e7767d9UL, 0x42bdb0e8UL, 0x8b880789UL, 0x5b38e719UL,
    0xeedb79c8UL, 0x0a47a17cUL, 0x0fe97c42UL, 0x1ec9f884UL, 0x00000000UL,
    0x86830980UL, 0xed48322bUL, 0x70ac1e11UL, 0x724e6c5aUL, 0xfffbfd0eUL,
    0x38560f85UL, 0xd51e3daeUL, 0x3927362dUL, 0xd9640a0fUL, 0xa621685cUL,
    0x54d19b5bUL, 0x2e3a2436UL, 0x67b10c0aUL, 0xe70f9357UL, 0x96d2b4eeUL,
    0x919e1b9bUL, 0xc54f80c0UL, 0x20a261dcUL, 0x4b695a77UL, 0x1a161c12UL,
    0xba0ae293UL, 0x2ae5c0a0UL, 0xe0433c22UL, 0x171d121bUL, 0x0d0b0e09UL,
    0xc7adf28bUL, 0xa8b92db6UL, 0xa9c8141eUL, 0x198557f1UL, 0x074caf75UL,
    0xddbbee99UL, 0x60fda37fUL, 0x269ff701UL, 0xf5bc5c72UL, 0x3bc54466UL,
    0x7e345bfbUL, 0x29768b43UL, 0xc6dccb23UL, 0xfc68b6edUL, 0xf163b8e4UL,
    0xdccad731UL, 0x85104263UL, 0x22401397UL, 0x112084c6UL, 0x247d854aUL,
    0x3df8d2bbUL, 0x3211aef9UL, 0xa16dc729UL, 0x2f4b1d9eUL, 0x30f3dcb2UL,
    0x52ec0d86UL, 0xe3d077c1UL, 0x166c2bb3UL, 0xb999a970UL, 0x48fa1194UL,
    0x642247e9UL, 0x8cc4a8fcUL, 0x3f1aa0f0UL, 0x2cd8567dUL, 0x90ef2233UL,
    0x4ec78749UL, 0xd1c1d938UL, 0xa2fe8ccaUL, 0x0b3698d4UL, 0x81cfa6f5UL,
    0xde28a57aUL, 0x8e26dab7UL, 0xbfa43fadUL, 0x9de42c3aUL, 0x920d5078UL,
    0xcc9b6a5fUL, 0x4662547eUL, 0x13c2f68dUL, 0xb8e890d8UL, 0xf75e2e39UL,
    0xaff582c3UL, 0x80be9f5dUL, 0x937c69d0UL, 0x2da96fd5UL, 0x12b3cf25UL,
    0x993bc8acUL, 0x7da71018UL, 0x636ee89cUL, 0xbb7bdb3bUL, 0x7809cd26UL,
    0x18f46e59UL, 0xb701ec9aUL, 0x9aa8834fUL, 0x6e65e695UL, 0xe67eaaffUL,
    0xcf0821bcUL, 0xe8e6ef15UL, 0x9bd9bae7UL, 0x36ce4a6fUL, 0x09d4ea9fUL,
    0x7cd629b0UL, 0xb2af31a4UL, 0x23312a3fUL, 0x9430c6a5UL, 0x66c035a2UL,
    0xbc37744eUL, 0xcaa6fc82UL, 0xd0b0e090UL, 0xd81533a7UL, 0x984af104UL,
    0xdaf741ecUL, 0x500e7fcdUL, 0xf62f1791UL, 0xd68d764dUL, 0xb04d43efUL,
    0x4d54ccaaUL, 0x04dfe496UL, 0xb5e39ed1UL, 0x881b4c6aUL, 0x1fb8c12cUL,
    0x517f4665UL, 0xea049d5eUL, 0x355d018cUL, 0x7473fa87UL, 0x412efb0bUL,
    0x1d5ab367UL, 0xd25292dbUL, 0x5633e910UL, 0x47136dd6UL, 0x618c9ad7UL,
    0x0c7a37a1UL, 0x148e59f8UL, 0x3c89eb13UL, 0x27eecea9UL, 0xc935b761UL,
    0xe5ede11cUL, 0xb13c7a47UL, 0xdf599cd2UL, 0x733f55f2UL, 0xce791814UL,
    0x37bf73c7UL, 0xcdea53f7UL, 0xaa5b5ffdUL, 0x6f14df3dUL, 0xdb867844UL,
    0xf381caafUL, 0xc43eb968UL, 0x342c3824UL, 0x405fc2a3UL, 0xc372161dUL,
    0x250cbce2UL, 0x498b283cUL, 0x9541ff0dUL, 0x017139a8UL, 0xb3de080cUL,
    0xe49cd8b4UL, 0xc1906456UL, 0x84617bcbUL, 0xb670d532UL, 0x5c74486cUL,
    0x5742d0b8UL,
};
static const ulong32 TD3[256] = {
    0xf4a75051UL, 0x4165537eUL, 0x17a4c31aUL, 0x275e963aUL, 0xab6bcb3bUL,
    0x9d45f11fUL, 0xfa58abacUL, 0xe303934bUL, 0x30fa5520UL, 0x766df6adUL,
    0xcc769188UL, 0x024c25f5UL, 0xe5d7fc4fUL, 0x2acbd7c5UL, 0x35448026UL,
    0x62a38fb5UL, 0xb15a49deUL, 0xba1b6725UL, 0xea0e9845UL, 0xfec0e15dUL,
    0x2f7502c3UL, 0x4cf01281UL, 0x4697a38dUL, 0xd3f9c66bUL, 0x8f5fe703UL,
    0x929c9515UL, 0x6d7aebbfUL, 0x5259da95UL, 0xbe832dd4UL, 0x7421d358UL,
    0xe0692949UL, 0xc9c8448eUL, 0xc2896a75UL, 0x8e7978f4UL, 0x583e6b99UL,
    0xb971dd27UL, 0xe14fb6beUL, 0x88ad17f0UL, 0x20ac66c9UL, 0xce3ab47dUL,
    0xdf4a1863UL, 0x1a3182e5UL, 0x51336097UL, 0x537f4562UL, 0x6477e0b1UL,
    0x6bae84bbUL, 0x81a01cfeUL, 0x082b94f9UL, 0x48685870UL, 0x45fd198fUL,
    0xde6c8794UL, 0x7bf8b752UL, 0x73d323abUL, 0x4b02e272UL, 0x1f8f57e3UL,
    0x55ab2a66UL, 0xeb2807b2UL, 0xb5c2032fUL, 0xc57b9a86UL, 0x3708a5d3UL,
    0x2887f230UL, 0xbfa5b223UL, 0x036aba02UL, 0x16825cedUL, 0xcf1c2b8aUL,
    0x79b492a7UL, 0x07f2f0f3UL, 0x69e2a14eUL, 0xdaf4cd65UL, 0x05bed506UL,
    0x34621fd1UL, 0xa6fe8ac4UL, 0x2e539d34UL, 0xf355a0a2UL, 0x8ae13205UL,
    0xf6eb75a4UL, 0x83ec390bUL, 0x60efaa40UL, 0x719f065eUL, 0x6e1051bdUL,
    0x218af93eUL, 0xdd063d96UL, 0x3e05aeddUL, 0xe6bd464dUL, 0x548db591UL,
    0xc45d0571UL, 0x06d46f04UL, 0x5015ff60UL, 0x98fb2419UL, 0xbde997d6UL,
    0x4043cc89UL, 0xd99e7767UL, 0xe842bdb0UL, 0x898b8807UL, 0x195b38e7UL,
    0xc8eedb79UL, 0x7c0a47a1UL, 0x420fe97cUL, 0x841ec9f8UL, 0x00000000UL,
    0x80868309UL, 0x2bed4832UL, 0x1170ac1eUL, 0x5a724e6cUL, 0x0efffbfdUL,
    0x8538560fUL, 0xaed51e3dUL, 0x2d392736UL, 0x0fd9640aUL, 0x5ca62168UL,
    0x5b54d19bUL, 0x362e3a24UL, 0x0a67b10cUL, 0x57e70f93UL, 0xee96d2b4UL,
    0x9b919e1bUL, 0xc0c54f80UL, 0xdc20a261UL, 0x774b695aUL, 0x121a161cUL,
    0x93ba0ae2UL, 0xa02ae5c0UL, 0x22e0433cUL, 0x1b171d12UL, 0x090d0b0eUL,
    0x8bc7adf2UL, 0xb6a8b92dUL, 0x1ea9c814UL, 0xf1198557UL, 0x75074cafUL,
    0x99ddbbeeUL, 0x7f60fda3UL, 0x01269ff7UL, 0x72f5bc5cUL, 0x663bc544UL,
    0xfb7e345bUL, 0x4329768bUL, 0x23c6dccbUL, 0xedfc68b6UL, 0xe4f163b8UL,
    0x31dccad7UL, 0x63851042UL, 0x97224013UL, 0xc6112084UL, 0x4a247d85UL,
    0xbb3df8d2UL, 0xf93211aeUL, 0x29a16dc7UL, 0x9e2f4b1dUL, 0xb230f3dcUL,
    0x8652ec0dUL, 0xc1e3d077UL, 0xb3166c2bUL, 0x70b999a9UL, 0x9448fa11UL,
    0xe9642247UL, 0xfc8cc4a8UL, 0xf03f1aa0UL, 0x7d2cd856UL, 0x3390ef22UL,
    0x494ec787UL, 0x38d1c1d9UL, 0xcaa2fe8cUL, 0xd40b3698UL, 0xf581cfa6UL,
    0x7ade28a5UL, 0xb78e26daUL, 0xadbfa43fUL, 0x3a9de42cUL, 0x78920d50UL,
    0x5fcc9b6aUL, 0x7e466254UL, 0x8d13c2f6UL, 0xd8b8e890UL, 0x39f75e2eUL,
    0xc3aff582UL, 0x5d80be9fUL, 0xd0937c69UL, 0xd52da96fUL, 0x2512b3cfUL,
    0xac993bc8UL, 0x187da710UL, 0x9c636ee8UL, 0x3bbb7bdbUL, 0x267809cdUL,
    0x5918f46eUL, 0x9ab701ecUL, 0x4f9aa883UL, 0x956e65e6UL, 0xffe67eaaUL,
    0xbccf0821UL, 0x15e8e6efUL, 0xe79bd9baUL, 0x6f36ce4aUL, 0x9f09d4eaUL,
    0xb07cd629UL, 0xa4b2af31UL, 0x3f23312aUL, 0xa59430c6UL, 0xa266c035UL,
    0x4ebc3774UL, 0x82caa6fcUL, 0x90d0b0e0UL, 0xa7d81533UL, 0x04984af1UL,
    0xecdaf741UL, 0xcd500e7fUL, 0x91f62f17UL, 0x4dd68d76UL, 0xefb04d43UL,
    0xaa4d54ccUL, 0x9604dfe4UL, 0xd1b5e39eUL, 0x6a881b4cUL, 0x2c1fb8c1UL,
    0x65517f46UL, 0x5eea049dUL, 0x8c355d01UL, 0x877473faUL, 0x0b412efbUL,
    0x671d5ab3UL, 0xdbd25292UL, 0x105633e9UL, 0xd647136dUL, 0xd7618c9aUL,
    0xa10c7a37UL, 0xf8148e59UL, 0x133c89ebUL, 0xa927eeceUL, 0x61c935b7UL,
    0x1ce5ede1UL, 0x47b13c7aUL, 0xd2df599cUL, 0xf2733f55UL, 0x14ce7918UL,
    0xc737bf73UL, 0xf7cdea53UL, 0xfdaa5b5fUL, 0x3d6f14dfUL, 0x44db8678UL,
    0xaff381caUL, 0x68c43eb9UL, 0x24342c38UL, 0xa3405fc2UL, 0x1dc37216UL,
    0xe2250cbcUL, 0x3c498b28UL, 0x0d9541ffUL, 0xa8017139UL, 0x0cb3de08UL,
    0xb4e49cd8UL, 0x56c19064UL, 0xcb84617bUL, 0x32b670d5UL, 0x6c5c7448UL,
    0xb85742d0UL,
};

static const ulong32 Tks0[] = {
    0x00000000UL, 0x0e090d0bUL, 0x1c121a16UL, 0x121b171dUL, 0x3824342cUL,
    0x362d3927UL, 0x24362e3aUL, 0x2a3f2331UL, 0x70486858UL, 0x7e416553UL,
    0x6c5a724eUL, 0x62537f45UL, 0x486c5c74UL, 0x4665517fUL, 0x547e4662UL,
    0x5a774b69UL, 0xe090d0b0UL, 0xee99ddbbUL, 0xfc82caa6UL, 0xf28bc7adUL,
    0xd8b4e49cUL, 0xd6bde997UL, 0xc4a6fe8aUL, 0xcaaff381UL, 0x90d8b8e8UL,
    0x9ed1b5e3UL, 0x8ccaa2feUL, 0x82c3aff5UL, 0xa8fc8cc4UL, 0xa6f581cfUL,
    0xb4ee96d2UL, 0xbae79bd9UL, 0xdb3bbb7bUL, 0xd532b670UL, 0xc729a16dUL,
    0xc920ac66UL, 0xe31f8f57UL, 0xed16825cUL, 0xff0d9541UL, 0xf104984aUL,
    0xab73d323UL, 0xa57ade28UL, 0xb761c935UL, 0xb968c43eUL, 0x9357e70fUL,
    0x9d5eea04UL, 0x8f45fd19UL, 0x814cf012UL, 0x3bab6bcbUL, 0x35a266c0UL,
    0x27b971ddUL, 0x29b07cd6UL, 0x038f5fe7UL, 0x0d8652ecUL, 0x1f9d45f1UL,
    0x119448faUL, 0x4be30393UL, 0x45ea0e98UL, 0x57f11985UL, 0x59f8148eUL,
    0x73c737bfUL, 0x7dce3ab4UL, 0x6fd52da9UL, 0x61dc20a2UL, 0xad766df6UL,
    0xa37f60fdUL, 0xb16477e0UL, 0xbf6d7aebUL, 0x955259daUL, 0x9b5b54d1UL,
    0x894043ccUL, 0x87494ec7UL, 0xdd3e05aeUL, 0xd33708a5UL, 0xc12c1fb8UL,
    0xcf2512b3UL, 0xe51a3182UL, 0xeb133c89UL, 0xf9082b94UL, 0xf701269fUL,
    0x4de6bd46UL, 0x43efb04dUL, 0x51f4a750UL, 0x5ffdaa5bUL, 0x75c2896aUL,
    0x7bcb8461UL, 0x69d0937cUL, 0x67d99e77UL, 0x3daed51eUL, 0x33a7d815UL,
    0x21bccf08UL, 0x2fb5c203UL, 0x058ae132UL, 0x0b83ec39UL, 0x1998fb24UL,
    0x1791f62fUL, 0x764dd68dUL, 0x7844db86UL, 0x6a5fcc9bUL, 0x6456c190UL,
    0x4e69e2a1UL, 0x4060efaaUL, 0x527bf8b7UL, 0x5c72f5bcUL, 0x0605bed5UL,
    0x080cb3deUL, 0x1a17a4c3UL, 0x141ea9c8UL, 0x3e218af9UL, 0x302887f2UL,
    0x223390efUL, 0x2c3a9de4UL, 0x96dd063dUL, 0x98d40b36UL, 0x8acf1c2bUL,
    0x84c61120UL, 0xaef93211UL, 0xa0f03f1aUL, 0xb2eb2807UL, 0xbce2250cUL,
    0xe6956e65UL, 0xe89c636eUL, 0xfa877473UL, 0xf48e7978UL, 0xdeb15a49UL,
    0xd0b85742UL, 0xc2a3405fUL, 0xccaa4d54UL, 0x41ecdaf7UL, 0x4fe5d7fcUL,
    0x5dfec0e1UL, 0x53f7cdeaUL, 0x79c8eedbUL, 0x77c1e3d0UL, 0x65daf4cdUL,
    0x6bd3f9c6UL, 0x31a4b2afUL, 0x3fadbfa4UL, 0x2db6a8b9UL, 0x23bfa5b2UL,
    0x09808683UL, 0x07898b88UL, 0x15929c95UL, 0x1b9b919eUL, 0xa17c0a47UL,
    0xaf75074cUL, 0xbd6e1051UL, 0xb3671d5aUL, 0x99583e6bUL, 0x97513360UL,
    0x854a247dUL, 0x8b432976UL, 0xd134621fUL, 0xdf3d6f14UL, 0xcd267809UL,
    0xc32f7502UL, 0xe9105633UL, 0xe7195b38UL, 0xf5024c25UL, 0xfb0b412eUL,
    0x9ad7618cUL, 0x94de6c87UL, 0x86c57b9aUL, 0x88cc7691UL, 0xa2f355a0UL,
    0xacfa58abUL, 0xbee14fb6UL, 0xb0e842bdUL, 0xea9f09d4UL, 0xe49604dfUL,
    0xf68d13c2UL, 0xf8841ec9UL, 0xd2bb3df8UL, 0xdcb230f3UL, 0xcea927eeUL,
    0xc0a02ae5UL, 0x7a47b13cUL, 0x744ebc37UL, 0x6655ab2aUL, 0x685ca621UL,
    0x42638510UL, 0x4c6a881bUL, 0x5e719f06UL, 0x5078920dUL, 0x0a0fd964UL,
    0x0406d46fUL, 0x161dc372UL, 0x1814ce79UL, 0x322bed48UL, 0x3c22e043UL,
    0x2e39f75eUL, 0x2030fa55UL, 0xec9ab701UL, 0xe293ba0aUL, 0xf088ad17UL,
    0xfe81a01cUL, 0xd4be832dUL, 0xdab78e26UL, 0xc8ac993bUL, 0xc6a59430UL,
    0x9cd2df59UL, 0x92dbd252UL, 0x80c0c54fUL, 0x8ec9c844UL, 0xa4f6eb75UL,
    0xaaffe67eUL, 0xb8e4f163UL, 0xb6edfc68UL, 0x0c0a67b1UL, 0x02036abaUL,
    0x10187da7UL, 0x1e1170acUL, 0x342e539dUL, 0x3a275e96UL, 0x283c498bUL,
    0x26354480UL, 0x7c420fe9UL, 0x724b02e2UL, 0x605015ffUL, 0x6e5918f4UL,
    0x44663bc5UL, 0x4a6f36ceUL, 0x587421d3UL, 0x567d2cd8UL, 0x37a10c7aUL,
    0x39a80171UL, 0x2bb3166cUL, 0x25ba1b67UL, 0x0f853856UL, 0x018c355dUL,
    0x13972240UL, 0x1d9e2f4bUL, 0x47e96422UL, 0x49e06929UL, 0x5bfb7e34UL,
    0x55f2733fUL, 0x7fcd500eUL, 0x71c45d05UL, 0x63df4a18UL, 0x6dd64713UL,
    0xd731dccaUL, 0xd938d1c1UL, 0xcb23c6dcUL, 0xc52acbd7UL, 0xef15e8e6UL,
    0xe11ce5edUL, 0xf307f2f0UL, 0xfd0efffbUL, 0xa779b492UL, 0xa970b999UL,
    0xbb6bae84UL, 0xb562a38fUL, 0x9f5d80beUL, 0x91548db5UL, 0x834f9aa8UL,
    0x8d4697a3UL};

static const ulong32 Tks1[] = {
    0x00000000UL, 0x0b0e090dUL, 0x161c121aUL, 0x1d121b17UL, 0x2c382434UL,
    0x27362d39UL, 0x3a24362eUL, 0x312a3f23UL, 0x58704868UL, 0x537e4165UL,
    0x4e6c5a72UL, 0x4562537fUL, 0x74486c5cUL, 0x7f466551UL, 0x62547e46UL,
    0x695a774bUL, 0xb0e090d0UL, 0xbbee99ddUL, 0xa6fc82caUL, 0xadf28bc7UL,
    0x9cd8b4e4UL, 0x97d6bde9UL, 0x8ac4a6feUL, 0x81caaff3UL, 0xe890d8b8UL,
    0xe39ed1b5UL, 0xfe8ccaa2UL, 0xf582c3afUL, 0xc4a8fc8cUL, 0xcfa6f581UL,
    0xd2b4ee96UL, 0xd9bae79bUL, 0x7bdb3bbbUL, 0x70d532b6UL, 0x6dc729a1UL,
    0x66c920acUL, 0x57e31f8fUL, 0x5ced1682UL, 0x41ff0d95UL, 0x4af10498UL,
    0x23ab73d3UL, 0x28a57adeUL, 0x35b761c9UL, 0x3eb968c4UL, 0x0f9357e7UL,
    0x049d5eeaUL, 0x198f45fdUL, 0x12814cf0UL, 0xcb3bab6bUL, 0xc035a266UL,
    0xdd27b971UL, 0xd629b07cUL, 0xe7038f5fUL, 0xec0d8652UL, 0xf11f9d45UL,
    0xfa119448UL, 0x934be303UL, 0x9845ea0eUL, 0x8557f119UL, 0x8e59f814UL,
    0xbf73c737UL, 0xb47dce3aUL, 0xa96fd52dUL, 0xa261dc20UL, 0xf6ad766dUL,
    0xfda37f60UL, 0xe0b16477UL, 0xebbf6d7aUL, 0xda955259UL, 0xd19b5b54UL,
    0xcc894043UL, 0xc787494eUL, 0xaedd3e05UL, 0xa5d33708UL, 0xb8c12c1fUL,
    0xb3cf2512UL, 0x82e51a31UL, 0x89eb133cUL, 0x94f9082bUL, 0x9ff70126UL,
    0x464de6bdUL, 0x4d43efb0UL, 0x5051f4a7UL, 0x5b5ffdaaUL, 0x6a75c289UL,
    0x617bcb84UL, 0x7c69d093UL, 0x7767d99eUL, 0x1e3daed5UL, 0x1533a7d8UL,
    0x0821bccfUL, 0x032fb5c2UL, 0x32058ae1UL, 0x390b83ecUL, 0x241998fbUL,
    0x2f1791f6UL, 0x8d764dd6UL, 0x867844dbUL, 0x9b6a5fccUL, 0x906456c1UL,
    0xa14e69e2UL, 0xaa4060efUL, 0xb7527bf8UL, 0xbc5c72f5UL, 0xd50605beUL,
    0xde080cb3UL, 0xc31a17a4UL, 0xc8141ea9UL, 0xf93e218aUL, 0xf2302887UL,
    0xef223390UL, 0xe42c3a9dUL, 0x3d96dd06UL, 0x3698d40bUL, 0x2b8acf1cUL,
    0x2084c611UL, 0x11aef932UL, 0x1aa0f03fUL, 0x07b2eb28UL, 0x0cbce225UL,
    0x65e6956eUL, 0x6ee89c63UL, 0x73fa8774UL, 0x78f48e79UL, 0x49deb15aUL,
    0x42d0b857UL, 0x5fc2a340UL, 0x54ccaa4dUL, 0xf741ecdaUL, 0xfc4fe5d7UL,
    0xe15dfec0UL, 0xea53f7cdUL, 0xdb79c8eeUL, 0xd077c1e3UL, 0xcd65daf4UL,
    0xc66bd3f9UL, 0xaf31a4b2UL, 0xa43fadbfUL, 0xb92db6a8UL, 0xb223bfa5UL,
    0x83098086UL, 0x8807898bUL, 0x9515929cUL, 0x9e1b9b91UL, 0x47a17c0aUL,
    0x4caf7507UL, 0x51bd6e10UL, 0x5ab3671dUL, 0x6b99583eUL, 0x60975133UL,
    0x7d854a24UL, 0x768b4329UL, 0x1fd13462UL, 0x14df3d6fUL, 0x09cd2678UL,
    0x02c32f75UL, 0x33e91056UL, 0x38e7195bUL, 0x25f5024cUL, 0x2efb0b41UL,
    0x8c9ad761UL, 0x8794de6cUL, 0x9a86c57bUL, 0x9188cc76UL, 0xa0a2f355UL,
    0xabacfa58UL, 0xb6bee14fUL, 0xbdb0e842UL, 0xd4ea9f09UL, 0xdfe49604UL,
    0xc2f68d13UL, 0xc9f8841eUL, 0xf8d2bb3dUL, 0xf3dcb230UL, 0xeecea927UL,
    0xe5c0a02aUL, 0x3c7a47b1UL, 0x37744ebcUL, 0x2a6655abUL, 0x21685ca6UL,
    0x10426385UL, 0x1b4c6a88UL, 0x065e719fUL, 0x0d507892UL, 0x640a0fd9UL,
    0x6f0406d4UL, 0x72161dc3UL, 0x791814ceUL, 0x48322bedUL, 0x433c22e0UL,
    0x5e2e39f7UL, 0x552030faUL, 0x01ec9ab7UL, 0x0ae293baUL, 0x17f088adUL,
    0x1cfe81a0UL, 0x2dd4be83UL, 0x26dab78eUL, 0x3bc8ac99UL, 0x30c6a594UL,
    0x599cd2dfUL, 0x5292dbd2UL, 0x4f80c0c5UL, 0x448ec9c8UL, 0x75a4f6ebUL,
    0x7eaaffe6UL, 0x63b8e4f1UL, 0x68b6edfcUL, 0xb10c0a67UL, 0xba02036aUL,
    0xa710187dUL, 0xac1e1170UL, 0x9d342e53UL, 0x963a275eUL, 0x8b283c49UL,
    0x80263544UL, 0xe97c420fUL, 0xe2724b02UL, 0xff605015UL, 0xf46e5918UL,
    0xc544663bUL, 0xce4a6f36UL, 0xd3587421UL, 0xd8567d2cUL, 0x7a37a10cUL,
    0x7139a801UL, 0x6c2bb316UL, 0x6725ba1bUL, 0x560f8538UL, 0x5d018c35UL,
    0x40139722UL, 0x4b1d9e2fUL, 0x2247e964UL, 0x2949e069UL, 0x345bfb7eUL,
    0x3f55f273UL, 0x0e7fcd50UL, 0x0571c45dUL, 0x1863df4aUL, 0x136dd647UL,
    0xcad731dcUL, 0xc1d938d1UL, 0xdccb23c6UL, 0xd7c52acbUL, 0xe6ef15e8UL,
    0xede11ce5UL, 0xf0f307f2UL, 0xfbfd0effUL, 0x92a779b4UL, 0x99a970b9UL,
    0x84bb6baeUL, 0x8fb562a3UL, 0xbe9f5d80UL, 0xb591548dUL, 0xa8834f9aUL,
    0xa38d4697UL};

static const ulong32 Tks2[] = {
    0x00000000UL, 0x0d0b0e09UL, 0x1a161c12UL, 0x171d121bUL, 0x342c3824UL,
    0x3927362dUL, 0x2e3a2436UL, 0x23312a3fUL, 0x68587048UL, 0x65537e41UL,
    0x724e6c5aUL, 0x7f456253UL, 0x5c74486cUL, 0x517f4665UL, 0x4662547eUL,
    0x4b695a77UL, 0xd0b0e090UL, 0xddbbee99UL, 0xcaa6fc82UL, 0xc7adf28bUL,
    0xe49cd8b4UL, 0xe997d6bdUL, 0xfe8ac4a6UL, 0xf381caafUL, 0xb8e890d8UL,
    0xb5e39ed1UL, 0xa2fe8ccaUL, 0xaff582c3UL, 0x8cc4a8fcUL, 0x81cfa6f5UL,
    0x96d2b4eeUL, 0x9bd9bae7UL, 0xbb7bdb3bUL, 0xb670d532UL, 0xa16dc729UL,
    0xac66c920UL, 0x8f57e31fUL, 0x825ced16UL, 0x9541ff0dUL, 0x984af104UL,
    0xd323ab73UL, 0xde28a57aUL, 0xc935b761UL, 0xc43eb968UL, 0xe70f9357UL,
    0xea049d5eUL, 0xfd198f45UL, 0xf012814cUL, 0x6bcb3babUL, 0x66c035a2UL,
    0x71dd27b9UL, 0x7cd629b0UL, 0x5fe7038fUL, 0x52ec0d86UL, 0x45f11f9dUL,
    0x48fa1194UL, 0x03934be3UL, 0x0e9845eaUL, 0x198557f1UL, 0x148e59f8UL,
    0x37bf73c7UL, 0x3ab47dceUL, 0x2da96fd5UL, 0x20a261dcUL, 0x6df6ad76UL,
    0x60fda37fUL, 0x77e0b164UL, 0x7aebbf6dUL, 0x59da9552UL, 0x54d19b5bUL,
    0x43cc8940UL, 0x4ec78749UL, 0x05aedd3eUL, 0x08a5d337UL, 0x1fb8c12cUL,
    0x12b3cf25UL, 0x3182e51aUL, 0x3c89eb13UL, 0x2b94f908UL, 0x269ff701UL,
    0xbd464de6UL, 0xb04d43efUL, 0xa75051f4UL, 0xaa5b5ffdUL, 0x896a75c2UL,
    0x84617bcbUL, 0x937c69d0UL, 0x9e7767d9UL, 0xd51e3daeUL, 0xd81533a7UL,
    0xcf0821bcUL, 0xc2032fb5UL, 0xe132058aUL, 0xec390b83UL, 0xfb241998UL,
    0xf62f1791UL, 0xd68d764dUL, 0xdb867844UL, 0xcc9b6a5fUL, 0xc1906456UL,
    0xe2a14e69UL, 0xefaa4060UL, 0xf8b7527bUL, 0xf5bc5c72UL, 0xbed50605UL,
    0xb3de080cUL, 0xa4c31a17UL, 0xa9c8141eUL, 0x8af93e21UL, 0x87f23028UL,
    0x90ef2233UL, 0x9de42c3aUL, 0x063d96ddUL, 0x0b3698d4UL, 0x1c2b8acfUL,
    0x112084c6UL, 0x3211aef9UL, 0x3f1aa0f0UL, 0x2807b2ebUL, 0x250cbce2UL,
    0x6e65e695UL, 0x636ee89cUL, 0x7473fa87UL, 0x7978f48eUL, 0x5a49deb1UL,
    0x5742d0b8UL, 0x405fc2a3UL, 0x4d54ccaaUL, 0xdaf741ecUL, 0xd7fc4fe5UL,
    0xc0e15dfeUL, 0xcdea53f7UL, 0xeedb79c8UL, 0xe3d077c1UL, 0xf4cd65daUL,
    0xf9c66bd3UL, 0xb2af31a4UL, 0xbfa43fadUL, 0xa8b92db6UL, 0xa5b223bfUL,
    0x86830980UL, 0x8b880789UL, 0x9c951592UL, 0x919e1b9bUL, 0x0a47a17cUL,
    0x074caf75UL, 0x1051bd6eUL, 0x1d5ab367UL, 0x3e6b9958UL, 0x33609751UL,
    0x247d854aUL, 0x29768b43UL, 0x621fd134UL, 0x6f14df3dUL, 0x7809cd26UL,
    0x7502c32fUL, 0x5633e910UL, 0x5b38e719UL, 0x4c25f502UL, 0x412efb0bUL,
    0x618c9ad7UL, 0x6c8794deUL, 0x7b9a86c5UL, 0x769188ccUL, 0x55a0a2f3UL,
    0x58abacfaUL, 0x4fb6bee1UL, 0x42bdb0e8UL, 0x09d4ea9fUL, 0x04dfe496UL,
    0x13c2f68dUL, 0x1ec9f884UL, 0x3df8d2bbUL, 0x30f3dcb2UL, 0x27eecea9UL,
    0x2ae5c0a0UL, 0xb13c7a47UL, 0xbc37744eUL, 0xab2a6655UL, 0xa621685cUL,
    0x85104263UL, 0x881b4c6aUL, 0x9f065e71UL, 0x920d5078UL, 0xd9640a0fUL,
    0xd46f0406UL, 0xc372161dUL, 0xce791814UL, 0xed48322bUL, 0xe0433c22UL,
    0xf75e2e39UL, 0xfa552030UL, 0xb701ec9aUL, 0xba0ae293UL, 0xad17f088UL,
    0xa01cfe81UL, 0x832dd4beUL, 0x8e26dab7UL, 0x993bc8acUL, 0x9430c6a5UL,
    0xdf599cd2UL, 0xd25292dbUL, 0xc54f80c0UL, 0xc8448ec9UL, 0xeb75a4f6UL,
    0xe67eaaffUL, 0xf163b8e4UL, 0xfc68b6edUL, 0x67b10c0aUL, 0x6aba0203UL,
    0x7da71018UL, 0x70ac1e11UL, 0x539d342eUL, 0x5e963a27UL, 0x498b283cUL,
    0x44802635UL, 0x0fe97c42UL, 0x02e2724bUL, 0x15ff6050UL, 0x18f46e59UL,
    0x3bc54466UL, 0x36ce4a6fUL, 0x21d35874UL, 0x2cd8567dUL, 0x0c7a37a1UL,
    0x017139a8UL, 0x166c2bb3UL, 0x1b6725baUL, 0x38560f85UL, 0x355d018cUL,
    0x22401397UL, 0x2f4b1d9eUL, 0x642247e9UL, 0x692949e0UL, 0x7e345bfbUL,
    0x733f55f2UL, 0x500e7fcdUL, 0x5d0571c4UL, 0x4a1863dfUL, 0x47136dd6UL,
    0xdccad731UL, 0xd1c1d938UL, 0xc6dccb23UL, 0xcbd7c52aUL, 0xe8e6ef15UL,
    0xe5ede11cUL, 0xf2f0f307UL, 0xfffbfd0eUL, 0xb492a779UL, 0xb999a970UL,
    0xae84bb6bUL, 0xa38fb562UL, 0x80be9f5dUL, 0x8db59154UL, 0x9aa8834fUL,
    0x97a38d46UL};

static const ulong32 Tks3[] = {
    0x00000000UL, 0x090d0b0eUL, 0x121a161cUL, 0x1b171d12UL, 0x24342c38UL,
    0x2d392736UL, 0x362e3a24UL, 0x3f23312aUL, 0x48685870UL, 0x4165537eUL,
    0x5a724e6cUL, 0x537f4562UL, 0x6c5c7448UL, 0x65517f46UL, 0x7e466254UL,
    0x774b695aUL, 0x90d0b0e0UL, 0x99ddbbeeUL, 0x82caa6fcUL, 0x8bc7adf2UL,
    0xb4e49cd8UL, 0xbde997d6UL, 0xa6fe8ac4UL, 0xaff381caUL, 0xd8b8e890UL,
    0xd1b5e39eUL, 0xcaa2fe8cUL, 0xc3aff582UL, 0xfc8cc4a8UL, 0xf581cfa6UL,
    0xee96d2b4UL, 0xe79bd9baUL, 0x3bbb7bdbUL, 0x32b670d5UL, 0x29a16dc7UL,
    0x20ac66c9UL, 0x1f8f57e3UL, 0x16825cedUL, 0x0d9541ffUL, 0x04984af1UL,
    0x73d323abUL, 0x7ade28a5UL, 0x61c935b7UL, 0x68c43eb9UL, 0x57e70f93UL,
    0x5eea049dUL, 0x45fd198fUL, 0x4cf01281UL, 0xab6bcb3bUL, 0xa266c035UL,
    0xb971dd27UL, 0xb07cd629UL, 0x8f5fe703UL, 0x8652ec0dUL, 0x9d45f11fUL,
    0x9448fa11UL, 0xe303934bUL, 0xea0e9845UL, 0xf1198557UL, 0xf8148e59UL,
    0xc737bf73UL, 0xce3ab47dUL, 0xd52da96fUL, 0xdc20a261UL, 0x766df6adUL,
    0x7f60fda3UL, 0x6477e0b1UL, 0x6d7aebbfUL, 0x5259da95UL, 0x5b54d19bUL,
    0x4043cc89UL, 0x494ec787UL, 0x3e05aeddUL, 0x3708a5d3UL, 0x2c1fb8c1UL,
    0x2512b3cfUL, 0x1a3182e5UL, 0x133c89ebUL, 0x082b94f9UL, 0x01269ff7UL,
    0xe6bd464dUL, 0xefb04d43UL, 0xf4a75051UL, 0xfdaa5b5fUL, 0xc2896a75UL,
    0xcb84617bUL, 0xd0937c69UL, 0xd99e7767UL, 0xaed51e3dUL, 0xa7d81533UL,
    0xbccf0821UL, 0xb5c2032fUL, 0x8ae13205UL, 0x83ec390bUL, 0x98fb2419UL,
    0x91f62f17UL, 0x4dd68d76UL, 0x44db8678UL, 0x5fcc9b6aUL, 0x56c19064UL,
    0x69e2a14eUL, 0x60efaa40UL, 0x7bf8b752UL, 0x72f5bc5cUL, 0x05bed506UL,
    0x0cb3de08UL, 0x17a4c31aUL, 0x1ea9c814UL, 0x218af93eUL, 0x2887f230UL,
    0x3390ef22UL, 0x3a9de42cUL, 0xdd063d96UL, 0xd40b3698UL, 0xcf1c2b8aUL,
    0xc6112084UL, 0xf93211aeUL, 0xf03f1aa0UL, 0xeb2807b2UL, 0xe2250cbcUL,
    0x956e65e6UL, 0x9c636ee8UL, 0x877473faUL, 0x8e7978f4UL, 0xb15a49deUL,
    0xb85742d0UL, 0xa3405fc2UL, 0xaa4d54ccUL, 0xecdaf741UL, 0xe5d7fc4fUL,
    0xfec0e15dUL, 0xf7cdea53UL, 0xc8eedb79UL, 0xc1e3d077UL, 0xdaf4cd65UL,
    0xd3f9c66bUL, 0xa4b2af31UL, 0xadbfa43fUL, 0xb6a8b92dUL, 0xbfa5b223UL,
    0x80868309UL, 0x898b8807UL, 0x929c9515UL, 0x9b919e1bUL, 0x7c0a47a1UL,
    0x75074cafUL, 0x6e1051bdUL, 0x671d5ab3UL, 0x583e6b99UL, 0x51336097UL,
    0x4a247d85UL, 0x4329768bUL, 0x34621fd1UL, 0x3d6f14dfUL, 0x267809cdUL,
    0x2f7502c3UL, 0x105633e9UL, 0x195b38e7UL, 0x024c25f5UL, 0x0b412efbUL,
    0xd7618c9aUL, 0xde6c8794UL, 0xc57b9a86UL, 0xcc769188UL, 0xf355a0a2UL,
    0xfa58abacUL, 0xe14fb6beUL, 0xe842bdb0UL, 0x9f09d4eaUL, 0x9604dfe4UL,
    0x8d13c2f6UL, 0x841ec9f8UL, 0xbb3df8d2UL, 0xb230f3dcUL, 0xa927eeceUL,
    0xa02ae5c0UL, 0x47b13c7aUL, 0x4ebc3774UL, 0x55ab2a66UL, 0x5ca62168UL,
    0x63851042UL, 0x6a881b4cUL, 0x719f065eUL, 0x78920d50UL, 0x0fd9640aUL,
    0x06d46f04UL, 0x1dc37216UL, 0x14ce7918UL, 0x2bed4832UL, 0x22e0433cUL,
    0x39f75e2eUL, 0x30fa5520UL, 0x9ab701ecUL, 0x93ba0ae2UL, 0x88ad17f0UL,
    0x81a01cfeUL, 0xbe832dd4UL, 0xb78e26daUL, 0xac993bc8UL, 0xa59430c6UL,
    0xd2df599cUL, 0xdbd25292UL, 0xc0c54f80UL, 0xc9c8448eUL, 0xf6eb75a4UL,
    0xffe67eaaUL, 0xe4f163b8UL, 0xedfc68b6UL, 0x0a67b10cUL, 0x036aba02UL,
    0x187da710UL, 0x1170ac1eUL, 0x2e539d34UL, 0x275e963aUL, 0x3c498b28UL,
    0x35448026UL, 0x420fe97cUL, 0x4b02e272UL, 0x5015ff60UL, 0x5918f46eUL,
    0x663bc544UL, 0x6f36ce4aUL, 0x7421d358UL, 0x7d2cd856UL, 0xa10c7a37UL,
    0xa8017139UL, 0xb3166c2bUL, 0xba1b6725UL, 0x8538560fUL, 0x8c355d01UL,
    0x97224013UL, 0x9e2f4b1dUL, 0xe9642247UL, 0xe0692949UL, 0xfb7e345bUL,
    0xf2733f55UL, 0xcd500e7fUL, 0xc45d0571UL, 0xdf4a1863UL, 0xd647136dUL,
    0x31dccad7UL, 0x38d1c1d9UL, 0x23c6dccbUL, 0x2acbd7c5UL, 0x15e8e6efUL,
    0x1ce5ede1UL, 0x07f2f0f3UL, 0x0efffbfdUL, 0x79b492a7UL, 0x70b999a9UL,
    0x6bae84bbUL, 0x62a38fb5UL, 0x5d80be9fUL, 0x548db591UL, 0x4f9aa883UL,
    0x4697a38dUL};

static const ulong32 rcon[] = {
    0x01000000UL, 0x02000000UL, 0x04000000UL, 0x08000000UL,
    0x10000000UL, 0x20000000UL, 0x40000000UL, 0x80000000UL,
    0x1B000000UL, 0x36000000UL, /* for 128-bit blocks, Rijndael never uses more
                               than 10 rcon values */
};

/* AES implementation by Tom St Denis
 *
 * Derived from the Public Domain source code by

---
  * rijndael-alg-fst.c
  *
  * @version 3.0 (December 2000)
  *
  * Optimised ANSI C code for the Rijndael cipher (now AES)
  *
  * @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
  * @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
  * @author Paulo Barreto <paulo.barreto@terra.com.br>
---
 */
/**
  @file aes.c
  Implementation of AES
*/

#define SETUP rijndael_setup
#define ECB_ENC rijndael_ecb_encrypt
#define ECB_DEC rijndael_ecb_decrypt
#define ECB_DONE rijndael_done
#define ECB_TEST rijndael_test
#define ECB_KS rijndael_keysize

const struct ltc_cipher_descriptor aes_desc = {
    "aes", 6, 16, SETUP, ECB_ENC, ECB_DONE,
};

static ulong32 setup_mix(ulong32 temp) {
  return (Te4_3[byte(temp, 2)]) ^ (Te4_2[byte(temp, 1)]) ^
         (Te4_1[byte(temp, 0)]) ^ (Te4_0[byte(temp, 3)]);
}

/**
   Initialize the AES (Rijndael) block cipher
   @param key The symmetric key you wish to pass
   @param keylen The key length in bytes
   @param num_rounds The number of rounds desired (0 for default)
   @param skey The key in as scheduled by this function.
   @return CRYPT_OK if successful
*/
int SETUP(const unsigned char *key, int keylen, int num_rounds,
          symmetric_key *skey) {
  int i;
  ulong32 temp, *rk;
  ulong32 *rrk;
  LTC_ARGCHK(key != NULL);
  LTC_ARGCHK(skey != NULL);

  if (keylen != 16 && keylen != 24 && keylen != 32) {
    return CRYPT_INVALID_KEYSIZE;
  }

  if (num_rounds != 0 && num_rounds != (10 + ((keylen / 8) - 2) * 2)) {
    return CRYPT_INVALID_ROUNDS;
  }

  skey->rijndael.Nr = 10 + ((keylen / 8) - 2) * 2;

  /* setup the forward key */
  i = 0;
  rk = skey->rijndael.eK;
  LOAD32H(rk[0], key);
  LOAD32H(rk[1], key + 4);
  LOAD32H(rk[2], key + 8);
  LOAD32H(rk[3], key + 12);
  if (keylen == 16) {
    for (;;) {
      temp = rk[3];
      rk[4] = rk[0] ^ setup_mix(temp) ^ rcon[i];
      rk[5] = rk[1] ^ rk[4];
      rk[6] = rk[2] ^ rk[5];
      rk[7] = rk[3] ^ rk[6];
      if (++i == 10) {
        break;
      }
      rk += 4;
    }
  } else if (keylen == 24) {
    LOAD32H(rk[4], key + 16);
    LOAD32H(rk[5], key + 20);
    for (;;) {
      temp = rk[5];
      rk[6] = rk[0] ^ setup_mix(temp) ^ rcon[i];
      rk[7] = rk[1] ^ rk[6];
      rk[8] = rk[2] ^ rk[7];
      rk[9] = rk[3] ^ rk[8];
      if (++i == 8) {
        break;
      }
      rk[10] = rk[4] ^ rk[9];
      rk[11] = rk[5] ^ rk[10];
      rk += 6;
    }
  } else if (keylen == 32) {
    LOAD32H(rk[4], key + 16);
    LOAD32H(rk[5], key + 20);
    LOAD32H(rk[6], key + 24);
    LOAD32H(rk[7], key + 28);
    for (;;) {
      temp = rk[7];
      rk[8] = rk[0] ^ setup_mix(temp) ^ rcon[i];
      rk[9] = rk[1] ^ rk[8];
      rk[10] = rk[2] ^ rk[9];
      rk[11] = rk[3] ^ rk[10];
      if (++i == 7) {
        break;
      }
      temp = rk[11];
      rk[12] = rk[4] ^ setup_mix(RORc(temp, 8));
      rk[13] = rk[5] ^ rk[12];
      rk[14] = rk[6] ^ rk[13];
      rk[15] = rk[7] ^ rk[14];
      rk += 8;
    }
  } else {
    /* this can't happen */
    /* coverity[dead_error_line] */
    return CRYPT_ERROR;
  }

  /* setup the inverse key now */
  rk = skey->rijndael.dK;
  rrk = skey->rijndael.eK + (28 + keylen) - 4;

  /* apply the inverse MixColumn transform to all round keys but the first and
   * the last: */
  /* copy first */
  *rk++ = *rrk++;
  *rk++ = *rrk++;
  *rk++ = *rrk++;
  *rk = *rrk;
  rk -= 3;
  rrk -= 3;

  for (i = 1; i < skey->rijndael.Nr; i++) {
    rrk -= 4;
    rk += 4;
    temp = rrk[0];
    rk[0] = Tks0[byte(temp, 3)] ^ Tks1[byte(temp, 2)] ^ Tks2[byte(temp, 1)] ^
            Tks3[byte(temp, 0)];
    temp = rrk[1];
    rk[1] = Tks0[byte(temp, 3)] ^ Tks1[byte(temp, 2)] ^ Tks2[byte(temp, 1)] ^
            Tks3[byte(temp, 0)];
    temp = rrk[2];
    rk[2] = Tks0[byte(temp, 3)] ^ Tks1[byte(temp, 2)] ^ Tks2[byte(temp, 1)] ^
            Tks3[byte(temp, 0)];
    temp = rrk[3];
    rk[3] = Tks0[byte(temp, 3)] ^ Tks1[byte(temp, 2)] ^ Tks2[byte(temp, 1)] ^
            Tks3[byte(temp, 0)];
  }

  /* copy last */
  rrk -= 4;
  rk += 4;
  *rk++ = *rrk++;
  *rk++ = *rrk++;
  *rk++ = *rrk++;
  *rk = *rrk;

  return CRYPT_OK;
}

/**
  Encrypts a block of text with AES
  @param pt The input plaintext (16 bytes)
  @param ct The output ciphertext (16 bytes)
  @param skey The key as scheduled
  @return CRYPT_OK if successful
*/

int ECB_ENC(const unsigned char *pt, unsigned char *ct, symmetric_key *skey) {
  ulong32 s0, s1, s2, s3, t0, t1, t2, t3, *rk;
  int Nr, r;

  LTC_ARGCHK(pt != NULL);
  LTC_ARGCHK(ct != NULL);
  LTC_ARGCHK(skey != NULL);

  Nr = skey->rijndael.Nr;
  rk = skey->rijndael.eK;

  /*
   * map byte array block to cipher state
   * and add initial round key:
   */
  LOAD32H(s0, pt);
  s0 ^= rk[0];
  LOAD32H(s1, pt + 4);
  s1 ^= rk[1];
  LOAD32H(s2, pt + 8);
  s2 ^= rk[2];
  LOAD32H(s3, pt + 12);
  s3 ^= rk[3];

  /*
   * Nr - 1 full rounds:
   */
  r = Nr >> 1;
  for (;;) {
    t0 = Te0(byte(s0, 3)) ^ Te1(byte(s1, 2)) ^ Te2(byte(s2, 1)) ^
         Te3(byte(s3, 0)) ^ rk[4];
    t1 = Te0(byte(s1, 3)) ^ Te1(byte(s2, 2)) ^ Te2(byte(s3, 1)) ^
         Te3(byte(s0, 0)) ^ rk[5];
    t2 = Te0(byte(s2, 3)) ^ Te1(byte(s3, 2)) ^ Te2(byte(s0, 1)) ^
         Te3(byte(s1, 0)) ^ rk[6];
    t3 = Te0(byte(s3, 3)) ^ Te1(byte(s0, 2)) ^ Te2(byte(s1, 1)) ^
         Te3(byte(s2, 0)) ^ rk[7];

    rk += 8;
    if (--r == 0) {
      break;
    }

    s0 = Te0(byte(t0, 3)) ^ Te1(byte(t1, 2)) ^ Te2(byte(t2, 1)) ^
         Te3(byte(t3, 0)) ^ rk[0];
    s1 = Te0(byte(t1, 3)) ^ Te1(byte(t2, 2)) ^ Te2(byte(t3, 1)) ^
         Te3(byte(t0, 0)) ^ rk[1];
    s2 = Te0(byte(t2, 3)) ^ Te1(byte(t3, 2)) ^ Te2(byte(t0, 1)) ^
         Te3(byte(t1, 0)) ^ rk[2];
    s3 = Te0(byte(t3, 3)) ^ Te1(byte(t0, 2)) ^ Te2(byte(t1, 1)) ^
         Te3(byte(t2, 0)) ^ rk[3];
  }

  /*
   * apply last round and
   * map cipher state to byte array block:
   */
  s0 = (Te4_3[byte(t0, 3)]) ^ (Te4_2[byte(t1, 2)]) ^ (Te4_1[byte(t2, 1)]) ^
       (Te4_0[byte(t3, 0)]) ^ rk[0];
  STORE32H(s0, ct);
  s1 = (Te4_3[byte(t1, 3)]) ^ (Te4_2[byte(t2, 2)]) ^ (Te4_1[byte(t3, 1)]) ^
       (Te4_0[byte(t0, 0)]) ^ rk[1];
  STORE32H(s1, ct + 4);
  s2 = (Te4_3[byte(t2, 3)]) ^ (Te4_2[byte(t3, 2)]) ^ (Te4_1[byte(t0, 1)]) ^
       (Te4_0[byte(t1, 0)]) ^ rk[2];
  STORE32H(s2, ct + 8);
  s3 = (Te4_3[byte(t3, 3)]) ^ (Te4_2[byte(t0, 2)]) ^ (Te4_1[byte(t1, 1)]) ^
       (Te4_0[byte(t2, 0)]) ^ rk[3];
  STORE32H(s3, ct + 12);

  return CRYPT_OK;
}

/**
  Decrypts a block of text with AES
  @param ct The input ciphertext (16 bytes)
  @param pt The output plaintext (16 bytes)
  @param skey The key as scheduled
  @return CRYPT_OK if successful
*/

int ECB_DEC(const unsigned char *ct, unsigned char *pt, symmetric_key *skey) {
  ulong32 s0, s1, s2, s3, t0, t1, t2, t3, *rk;
  int Nr, r;

  LTC_ARGCHK(pt != NULL);
  LTC_ARGCHK(ct != NULL);
  LTC_ARGCHK(skey != NULL);

  Nr = skey->rijndael.Nr;
  rk = skey->rijndael.dK;

  /*
   * map byte array block to cipher state
   * and add initial round key:
   */
  LOAD32H(s0, ct);
  s0 ^= rk[0];
  LOAD32H(s1, ct + 4);
  s1 ^= rk[1];
  LOAD32H(s2, ct + 8);
  s2 ^= rk[2];
  LOAD32H(s3, ct + 12);
  s3 ^= rk[3];

  /*
   * Nr - 1 full rounds:
   */
  r = Nr >> 1;
  for (;;) {
    t0 = Td0(byte(s0, 3)) ^ Td1(byte(s3, 2)) ^ Td2(byte(s2, 1)) ^
         Td3(byte(s1, 0)) ^ rk[4];
    t1 = Td0(byte(s1, 3)) ^ Td1(byte(s0, 2)) ^ Td2(byte(s3, 1)) ^
         Td3(byte(s2, 0)) ^ rk[5];
    t2 = Td0(byte(s2, 3)) ^ Td1(byte(s1, 2)) ^ Td2(byte(s0, 1)) ^
         Td3(byte(s3, 0)) ^ rk[6];
    t3 = Td0(byte(s3, 3)) ^ Td1(byte(s2, 2)) ^ Td2(byte(s1, 1)) ^
         Td3(byte(s0, 0)) ^ rk[7];

    rk += 8;
    if (--r == 0) {
      break;
    }

    s0 = Td0(byte(t0, 3)) ^ Td1(byte(t3, 2)) ^ Td2(byte(t2, 1)) ^
         Td3(byte(t1, 0)) ^ rk[0];
    s1 = Td0(byte(t1, 3)) ^ Td1(byte(t0, 2)) ^ Td2(byte(t3, 1)) ^
         Td3(byte(t2, 0)) ^ rk[1];
    s2 = Td0(byte(t2, 3)) ^ Td1(byte(t1, 2)) ^ Td2(byte(t0, 1)) ^
         Td3(byte(t3, 0)) ^ rk[2];
    s3 = Td0(byte(t3, 3)) ^ Td1(byte(t2, 2)) ^ Td2(byte(t1, 1)) ^
         Td3(byte(t0, 0)) ^ rk[3];
  }

  /*
   * apply last round and
   * map cipher state to byte array block:
   */
  s0 = (Td4[byte(t0, 3)] & 0xff000000) ^ (Td4[byte(t3, 2)] & 0x00ff0000) ^
       (Td4[byte(t2, 1)] & 0x0000ff00) ^ (Td4[byte(t1, 0)] & 0x000000ff) ^
       rk[0];
  STORE32H(s0, pt);
  s1 = (Td4[byte(t1, 3)] & 0xff000000) ^ (Td4[byte(t0, 2)] & 0x00ff0000) ^
       (Td4[byte(t3, 1)] & 0x0000ff00) ^ (Td4[byte(t2, 0)] & 0x000000ff) ^
       rk[1];
  STORE32H(s1, pt + 4);
  s2 = (Td4[byte(t2, 3)] & 0xff000000) ^ (Td4[byte(t1, 2)] & 0x00ff0000) ^
       (Td4[byte(t0, 1)] & 0x0000ff00) ^ (Td4[byte(t3, 0)] & 0x000000ff) ^
       rk[2];
  STORE32H(s2, pt + 8);
  s3 = (Td4[byte(t3, 3)] & 0xff000000) ^ (Td4[byte(t2, 2)] & 0x00ff0000) ^
       (Td4[byte(t1, 1)] & 0x0000ff00) ^ (Td4[byte(t0, 0)] & 0x000000ff) ^
       rk[3];
  STORE32H(s3, pt + 12);

  return CRYPT_OK;
}

/**
  Performs a self-test of the AES block cipher
  @return CRYPT_OK if functional, CRYPT_NOP if self-test has been disabled
*/
int ECB_TEST(void) { return CRYPT_NOP; }

/** Terminate the context
   @param skey    The scheduled key
*/
void ECB_DONE(symmetric_key *skey) {
  // LTC_UNUSED_PARAM(skey);
}

/**
  Gets suitable key size
  @param keysize [in/out] The length of the recommended key (in bytes).  This
  function will store the suitable size back in this variable.
  @return CRYPT_OK if the input key size is acceptable.
*/
int ECB_KS(int *keysize) {
  LTC_ARGCHK(keysize != NULL);

  if (*keysize < 16) return CRYPT_INVALID_KEYSIZE;
  if (*keysize < 24) {
    *keysize = 16;
    return CRYPT_OK;
  } else if (*keysize < 32) {
    *keysize = 24;
    return CRYPT_OK;
  } else {
    *keysize = 32;
    return CRYPT_OK;
  }
}

/**
  Add IV data to the GCM state
  @param gcm    The GCM state
  @param IV     The initial value data to add
  @param IVlen  The length of the IV
  @return CRYPT_OK on success
 */
int gcm_add_iv(gcm_state *gcm, const unsigned char *IV, unsigned long IVlen) {
  unsigned long x, y;
  int err;

  LTC_ARGCHK(gcm != NULL);
  if (IVlen > 0) {
    LTC_ARGCHK(IV != NULL);
  }

  /* must be in IV mode */
  if (gcm->mode != LTC_GCM_MODE_IV) {
    return CRYPT_INVALID_ARG;
  }

  if (gcm->buflen >= 16 || gcm->buflen < 0) {
    return CRYPT_INVALID_ARG;
  }

  if ((err = cipher_is_valid(gcm->cipher)) != CRYPT_OK) {
    return err;
  }

  /* trip the ivmode flag */
  if (IVlen + gcm->buflen > 12) {
    gcm->ivmode |= 1;
  }

  x = 0;
  if (gcm->buflen == 0) {
    for (x = 0; x < (IVlen & ~15); x += 16) {
      for (y = 0; y < 16; y += sizeof(LTC_FAST_TYPE)) {
        *((LTC_FAST_TYPE *)(&gcm->X[y])) ^= *((LTC_FAST_TYPE *)(&IV[x + y]));
      }
      gcm_mult_h(gcm, gcm->X);
      gcm->totlen += 128;
    }
    IV += x;
  }

  /* start adding IV data to the state */
  for (; x < IVlen; x++) {
    gcm->buf[gcm->buflen++] = *IV++;

    if (gcm->buflen == 16) {
      /* GF mult it */
      for (y = 0; y < 16; y++) {
        gcm->X[y] ^= gcm->buf[y];
      }
      gcm_mult_h(gcm, gcm->X);
      gcm->buflen = 0;
      gcm->totlen += 128;
    }
  }

  return CRYPT_OK;
}

/**
  Terminate a GCM stream
  @param gcm     The GCM state
  @param tag     [out] The destination for the MAC tag
  @param taglen  [in/out]  The length of the MAC tag
  @return CRYPT_OK on success
 */
int gcm_done(gcm_state *gcm, unsigned char *tag, unsigned long *taglen) {
  unsigned long x;
  int err;

  LTC_ARGCHK(gcm != NULL);
  LTC_ARGCHK(tag != NULL);
  LTC_ARGCHK(taglen != NULL);

  if (gcm->buflen > 16 || gcm->buflen < 0) {
    return CRYPT_INVALID_ARG;
  }

  if ((err = cipher_is_valid(gcm->cipher)) != CRYPT_OK) {
    return err;
  }

  if (gcm->mode != LTC_GCM_MODE_TEXT) {
    return CRYPT_INVALID_ARG;
  }

  /* handle remaining ciphertext */
  if (gcm->buflen) {
    gcm->pttotlen += gcm->buflen * CONST64(8);
    gcm_mult_h(gcm, gcm->X);
  }

  /* length */
  STORE64H(gcm->totlen, gcm->buf);
  STORE64H(gcm->pttotlen, gcm->buf + 8);
  for (x = 0; x < 16; x++) {
    gcm->X[x] ^= gcm->buf[x];
  }
  gcm_mult_h(gcm, gcm->X);

  /* encrypt original counter */
  if ((err = cipher_descriptor[gcm->cipher].ecb_encrypt(gcm->Y_0, gcm->buf,
                                                        &gcm->K)) != CRYPT_OK) {
    return err;
  }
  for (x = 0; x < 16 && x < *taglen; x++) {
    tag[x] = gcm->buf[x] ^ gcm->X[x];
  }
  *taglen = x;

  cipher_descriptor[gcm->cipher].done(&gcm->K);

  return CRYPT_OK;
}

/**
  Initialize a GCM state
  @param gcm     The GCM state to initialize
  @param cipher  The index of the cipher to use
  @param key     The secret key
  @param keylen  The length of the secret key
  @return CRYPT_OK on success
 */
int gcm_init(gcm_state *gcm, int cipher, const unsigned char *key, int keylen) {
  int err;
  unsigned char B[16];
  int x, y, z, t;

  LTC_ARGCHK(gcm != NULL);
  LTC_ARGCHK(key != NULL);

  /* is cipher valid? */
  if ((err = cipher_is_valid(cipher)) != CRYPT_OK) {
    return err;
  }
  if (cipher_descriptor[cipher].block_length != 16) {
    return CRYPT_INVALID_CIPHER;
  }

  /* schedule key */
  if ((err = cipher_descriptor[cipher].setup(key, keylen, 0, &gcm->K)) !=
      CRYPT_OK) {
    return err;
  }

  /* H = E(0) */
  zeromem(B, 16);
  if ((err = cipher_descriptor[cipher].ecb_encrypt(B, gcm->H, &gcm->K)) !=
      CRYPT_OK) {
    return err;
  }

  /* setup state */
  zeromem(gcm->buf, sizeof(gcm->buf));
  zeromem(gcm->X, sizeof(gcm->X));
  gcm->cipher = cipher;
  gcm->mode = LTC_GCM_MODE_IV;
  gcm->ivmode = 0;
  gcm->buflen = 0;
  gcm->totlen = 0;
  gcm->pttotlen = 0;

  /* setup tables */

  /* generate the first table as it has no shifting (from which we make the
   * other tables) */
  zeromem(B, 16);
  for (y = 0; y < 256; y++) {
    B[0] = y;
    gcm_gf_mult(gcm->H, B, &gcm->PC[0][y][0]);
  }

  /* now generate the rest of the tables based the previous table */
  for (x = 1; x < 16; x++) {
    for (y = 0; y < 256; y++) {
      /* now shift it right by 8 bits */
      t = gcm->PC[x - 1][y][15];
      for (z = 15; z > 0; z--) {
        gcm->PC[x][y][z] = gcm->PC[x - 1][y][z - 1];
      }
      gcm->PC[x][y][0] = gcm_shift_table[t << 1];
      gcm->PC[x][y][1] ^= gcm_shift_table[(t << 1) + 1];
    }
  }

  return CRYPT_OK;
}

/**
  Process plaintext/ciphertext through GCM
  @param gcm       The GCM state
  @param pt        The plaintext
  @param ptlen     The plaintext length (ciphertext length is the same)
  @param ct        The ciphertext
  @param direction Encrypt or Decrypt mode (GCM_ENCRYPT or GCM_DECRYPT)
  @return CRYPT_OK on success
 */
int gcm_process(gcm_state *gcm, unsigned char *pt, unsigned long ptlen,
                unsigned char *ct, int direction) {
  unsigned long x;
  int y, err;
  unsigned char b;

  LTC_ARGCHK(gcm != NULL);
  if (ptlen > 0) {
    LTC_ARGCHK(pt != NULL);
    LTC_ARGCHK(ct != NULL);
  }

  if (gcm->buflen > 16 || gcm->buflen < 0) {
    return CRYPT_INVALID_ARG;
  }

  if ((err = cipher_is_valid(gcm->cipher)) != CRYPT_OK) {
    return err;
  }

  /* in AAD mode? */
  if (gcm->mode == LTC_GCM_MODE_AAD) {
    /* let's process the AAD */
    if (gcm->buflen) {
      gcm->totlen += gcm->buflen * CONST64(8);
      gcm_mult_h(gcm, gcm->X);
    }

    /* increment counter */
    for (y = 15; y >= 12; y--) {
      if (++gcm->Y[y] & 255) {
        break;
      }
    }
    /* encrypt the counter */
    if ((err = cipher_descriptor[gcm->cipher].ecb_encrypt(
             gcm->Y, gcm->buf, &gcm->K)) != CRYPT_OK) {
      return err;
    }

    gcm->buflen = 0;
    gcm->mode = LTC_GCM_MODE_TEXT;
  }

  if (gcm->mode != LTC_GCM_MODE_TEXT) {
    return CRYPT_INVALID_ARG;
  }

  x = 0;
  if (gcm->buflen == 0) {
    if (direction == GCM_ENCRYPT) {
      for (x = 0; x < (ptlen & ~15); x += 16) {
        /* ctr encrypt */
        for (y = 0; y < 16; y += sizeof(LTC_FAST_TYPE)) {
          *((LTC_FAST_TYPE *)(&ct[x + y])) = *((LTC_FAST_TYPE *)(&pt[x + y])) ^
                                             *((LTC_FAST_TYPE *)(&gcm->buf[y]));
          *((LTC_FAST_TYPE *)(&gcm->X[y])) ^= *((LTC_FAST_TYPE *)(&ct[x + y]));
        }
        /* GMAC it */
        gcm->pttotlen += 128;
        gcm_mult_h(gcm, gcm->X);
        /* increment counter */
        for (y = 15; y >= 12; y--) {
          if (++gcm->Y[y] & 255) {
            break;
          }
        }
        if ((err = cipher_descriptor[gcm->cipher].ecb_encrypt(
                 gcm->Y, gcm->buf, &gcm->K)) != CRYPT_OK) {
          return err;
        }
      }
    } else {
      for (x = 0; x < (ptlen & ~15); x += 16) {
        /* ctr encrypt */
        for (y = 0; y < 16; y += sizeof(LTC_FAST_TYPE)) {
          *((LTC_FAST_TYPE *)(&gcm->X[y])) ^= *((LTC_FAST_TYPE *)(&ct[x + y]));
          *((LTC_FAST_TYPE *)(&pt[x + y])) = *((LTC_FAST_TYPE *)(&ct[x + y])) ^
                                             *((LTC_FAST_TYPE *)(&gcm->buf[y]));
        }
        /* GMAC it */
        gcm->pttotlen += 128;
        gcm_mult_h(gcm, gcm->X);
        /* increment counter */
        for (y = 15; y >= 12; y--) {
          if (++gcm->Y[y] & 255) {
            break;
          }
        }
        if ((err = cipher_descriptor[gcm->cipher].ecb_encrypt(
                 gcm->Y, gcm->buf, &gcm->K)) != CRYPT_OK) {
          return err;
        }
      }
    }
  }

  /* process text */
  for (; x < ptlen; x++) {
    if (gcm->buflen == 16) {
      gcm->pttotlen += 128;
      gcm_mult_h(gcm, gcm->X);

      /* increment counter */
      for (y = 15; y >= 12; y--) {
        if (++gcm->Y[y] & 255) {
          break;
        }
      }
      if ((err = cipher_descriptor[gcm->cipher].ecb_encrypt(
               gcm->Y, gcm->buf, &gcm->K)) != CRYPT_OK) {
        return err;
      }
      gcm->buflen = 0;
    }

    if (direction == GCM_ENCRYPT) {
      b = ct[x] = pt[x] ^ gcm->buf[gcm->buflen];
    } else {
      b = ct[x];
      pt[x] = ct[x] ^ gcm->buf[gcm->buflen];
    }
    gcm->X[gcm->buflen++] ^= b;
  }

  return CRYPT_OK;
}

/**
  GCM multiply by H
  @param gcm   The GCM state which holds the H value
  @param I     The value to multiply H by
 */
void gcm_mult_h(gcm_state *gcm, unsigned char *I) {
  unsigned char T[16];
  int x, y;
  XMEMCPY(T, &gcm->PC[0][I[0]][0], 16);
  for (x = 1; x < 16; x++) {
    for (y = 0; y < 16; y += sizeof(LTC_FAST_TYPE)) {
      *((LTC_FAST_TYPE *)(T + y)) ^= *((LTC_FAST_TYPE *)(&gcm->PC[x][I[x]][y]));
    }
  }
  XMEMCPY(I, T, 16);
}

#if defined(LTC_GCM_TABLES) || defined(LRW_TABLES) || \
    ((defined(LTC_GCM_MODE) || defined(LTC_GCM_MODE)) && defined(LTC_FAST))

/* this is x*2^128 mod p(x) ... the results are 16 bytes each stored in a packed
 * format.  Since only the
 * lower 16 bits are not zero'ed I removed the upper 14 bytes */
const unsigned char gcm_shift_table[256 * 2] = {
    0x00, 0x00, 0x01, 0xc2, 0x03, 0x84, 0x02, 0x46, 0x07, 0x08, 0x06, 0xca,
    0x04, 0x8c, 0x05, 0x4e, 0x0e, 0x10, 0x0f, 0xd2, 0x0d, 0x94, 0x0c, 0x56,
    0x09, 0x18, 0x08, 0xda, 0x0a, 0x9c, 0x0b, 0x5e, 0x1c, 0x20, 0x1d, 0xe2,
    0x1f, 0xa4, 0x1e, 0x66, 0x1b, 0x28, 0x1a, 0xea, 0x18, 0xac, 0x19, 0x6e,
    0x12, 0x30, 0x13, 0xf2, 0x11, 0xb4, 0x10, 0x76, 0x15, 0x38, 0x14, 0xfa,
    0x16, 0xbc, 0x17, 0x7e, 0x38, 0x40, 0x39, 0x82, 0x3b, 0xc4, 0x3a, 0x06,
    0x3f, 0x48, 0x3e, 0x8a, 0x3c, 0xcc, 0x3d, 0x0e, 0x36, 0x50, 0x37, 0x92,
    0x35, 0xd4, 0x34, 0x16, 0x31, 0x58, 0x30, 0x9a, 0x32, 0xdc, 0x33, 0x1e,
    0x24, 0x60, 0x25, 0xa2, 0x27, 0xe4, 0x26, 0x26, 0x23, 0x68, 0x22, 0xaa,
    0x20, 0xec, 0x21, 0x2e, 0x2a, 0x70, 0x2b, 0xb2, 0x29, 0xf4, 0x28, 0x36,
    0x2d, 0x78, 0x2c, 0xba, 0x2e, 0xfc, 0x2f, 0x3e, 0x70, 0x80, 0x71, 0x42,
    0x73, 0x04, 0x72, 0xc6, 0x77, 0x88, 0x76, 0x4a, 0x74, 0x0c, 0x75, 0xce,
    0x7e, 0x90, 0x7f, 0x52, 0x7d, 0x14, 0x7c, 0xd6, 0x79, 0x98, 0x78, 0x5a,
    0x7a, 0x1c, 0x7b, 0xde, 0x6c, 0xa0, 0x6d, 0x62, 0x6f, 0x24, 0x6e, 0xe6,
    0x6b, 0xa8, 0x6a, 0x6a, 0x68, 0x2c, 0x69, 0xee, 0x62, 0xb0, 0x63, 0x72,
    0x61, 0x34, 0x60, 0xf6, 0x65, 0xb8, 0x64, 0x7a, 0x66, 0x3c, 0x67, 0xfe,
    0x48, 0xc0, 0x49, 0x02, 0x4b, 0x44, 0x4a, 0x86, 0x4f, 0xc8, 0x4e, 0x0a,
    0x4c, 0x4c, 0x4d, 0x8e, 0x46, 0xd0, 0x47, 0x12, 0x45, 0x54, 0x44, 0x96,
    0x41, 0xd8, 0x40, 0x1a, 0x42, 0x5c, 0x43, 0x9e, 0x54, 0xe0, 0x55, 0x22,
    0x57, 0x64, 0x56, 0xa6, 0x53, 0xe8, 0x52, 0x2a, 0x50, 0x6c, 0x51, 0xae,
    0x5a, 0xf0, 0x5b, 0x32, 0x59, 0x74, 0x58, 0xb6, 0x5d, 0xf8, 0x5c, 0x3a,
    0x5e, 0x7c, 0x5f, 0xbe, 0xe1, 0x00, 0xe0, 0xc2, 0xe2, 0x84, 0xe3, 0x46,
    0xe6, 0x08, 0xe7, 0xca, 0xe5, 0x8c, 0xe4, 0x4e, 0xef, 0x10, 0xee, 0xd2,
    0xec, 0x94, 0xed, 0x56, 0xe8, 0x18, 0xe9, 0xda, 0xeb, 0x9c, 0xea, 0x5e,
    0xfd, 0x20, 0xfc, 0xe2, 0xfe, 0xa4, 0xff, 0x66, 0xfa, 0x28, 0xfb, 0xea,
    0xf9, 0xac, 0xf8, 0x6e, 0xf3, 0x30, 0xf2, 0xf2, 0xf0, 0xb4, 0xf1, 0x76,
    0xf4, 0x38, 0xf5, 0xfa, 0xf7, 0xbc, 0xf6, 0x7e, 0xd9, 0x40, 0xd8, 0x82,
    0xda, 0xc4, 0xdb, 0x06, 0xde, 0x48, 0xdf, 0x8a, 0xdd, 0xcc, 0xdc, 0x0e,
    0xd7, 0x50, 0xd6, 0x92, 0xd4, 0xd4, 0xd5, 0x16, 0xd0, 0x58, 0xd1, 0x9a,
    0xd3, 0xdc, 0xd2, 0x1e, 0xc5, 0x60, 0xc4, 0xa2, 0xc6, 0xe4, 0xc7, 0x26,
    0xc2, 0x68, 0xc3, 0xaa, 0xc1, 0xec, 0xc0, 0x2e, 0xcb, 0x70, 0xca, 0xb2,
    0xc8, 0xf4, 0xc9, 0x36, 0xcc, 0x78, 0xcd, 0xba, 0xcf, 0xfc, 0xce, 0x3e,
    0x91, 0x80, 0x90, 0x42, 0x92, 0x04, 0x93, 0xc6, 0x96, 0x88, 0x97, 0x4a,
    0x95, 0x0c, 0x94, 0xce, 0x9f, 0x90, 0x9e, 0x52, 0x9c, 0x14, 0x9d, 0xd6,
    0x98, 0x98, 0x99, 0x5a, 0x9b, 0x1c, 0x9a, 0xde, 0x8d, 0xa0, 0x8c, 0x62,
    0x8e, 0x24, 0x8f, 0xe6, 0x8a, 0xa8, 0x8b, 0x6a, 0x89, 0x2c, 0x88, 0xee,
    0x83, 0xb0, 0x82, 0x72, 0x80, 0x34, 0x81, 0xf6, 0x84, 0xb8, 0x85, 0x7a,
    0x87, 0x3c, 0x86, 0xfe, 0xa9, 0xc0, 0xa8, 0x02, 0xaa, 0x44, 0xab, 0x86,
    0xae, 0xc8, 0xaf, 0x0a, 0xad, 0x4c, 0xac, 0x8e, 0xa7, 0xd0, 0xa6, 0x12,
    0xa4, 0x54, 0xa5, 0x96, 0xa0, 0xd8, 0xa1, 0x1a, 0xa3, 0x5c, 0xa2, 0x9e,
    0xb5, 0xe0, 0xb4, 0x22, 0xb6, 0x64, 0xb7, 0xa6, 0xb2, 0xe8, 0xb3, 0x2a,
    0xb1, 0x6c, 0xb0, 0xae, 0xbb, 0xf0, 0xba, 0x32, 0xb8, 0x74, 0xb9, 0xb6,
    0xbc, 0xf8, 0xbd, 0x3a, 0xbf, 0x7c, 0xbe, 0xbe};

#endif

/* map normal numbers to "ieee" way ... e.g. bit reversed */
#define M(x) (((x & 8) >> 3) | ((x & 4) >> 1) | ((x & 2) << 1) | ((x & 1) << 3))

#define BPD (sizeof(LTC_FAST_TYPE) * 8)
#define WPV (1 + (16 / sizeof(LTC_FAST_TYPE)))

/**
  GCM GF multiplier (internal use only)  word oriented
  @param a   First value
  @param b   Second value
  @param c   Destination for a * b
 */
void gcm_gf_mult(const unsigned char *a, const unsigned char *b,
                 unsigned char *c) {
  int i, j, k, u;
  LTC_FAST_TYPE B[16][WPV], tmp[32 / sizeof(LTC_FAST_TYPE)],
      pB[16 / sizeof(LTC_FAST_TYPE)], zz, z;
  unsigned char pTmp[32];

  /* create simple tables */
  zeromem(B[0], sizeof(B[0]));
  zeromem(B[M(1)], sizeof(B[M(1)]));

  for (i = 0; i < 2; i++) {
    LOAD64H(B[M(1)][i], a + (i << 3));
    LOAD64L(pB[i], b + (i << 3));
  }

  /* now create 2, 4 and 8 */
  B[M(2)][0] = B[M(1)][0] >> 1;
  B[M(4)][0] = B[M(1)][0] >> 2;
  B[M(8)][0] = B[M(1)][0] >> 3;
  for (i = 1; i < (int)WPV; i++) {
    B[M(2)][i] = (B[M(1)][i - 1] << (BPD - 1)) | (B[M(1)][i] >> 1);
    B[M(4)][i] = (B[M(1)][i - 1] << (BPD - 2)) | (B[M(1)][i] >> 2);
    B[M(8)][i] = (B[M(1)][i - 1] << (BPD - 3)) | (B[M(1)][i] >> 3);
  }

  /*  now all values with two bits which are 3, 5, 6, 9, 10, 12 */
  for (i = 0; i < (int)WPV; i++) {
    B[M(3)][i] = B[M(1)][i] ^ B[M(2)][i];
    B[M(5)][i] = B[M(1)][i] ^ B[M(4)][i];
    B[M(6)][i] = B[M(2)][i] ^ B[M(4)][i];
    B[M(9)][i] = B[M(1)][i] ^ B[M(8)][i];
    B[M(10)][i] = B[M(2)][i] ^ B[M(8)][i];
    B[M(12)][i] = B[M(8)][i] ^ B[M(4)][i];

    /*  now all 3 bit values and the only 4 bit value: 7, 11, 13, 14, 15 */
    B[M(7)][i] = B[M(3)][i] ^ B[M(4)][i];
    B[M(11)][i] = B[M(3)][i] ^ B[M(8)][i];
    B[M(13)][i] = B[M(1)][i] ^ B[M(12)][i];
    B[M(14)][i] = B[M(6)][i] ^ B[M(8)][i];
    B[M(15)][i] = B[M(7)][i] ^ B[M(8)][i];
  }

  zeromem(tmp, sizeof(tmp));

  /* compute product four bits of each word at a time */
  /* for each nibble */
  for (i = (BPD / 4) - 1; i >= 0; i--) {
    /* for each word */
    for (j = 0; j < (int)(WPV - 1); j++) {
      /* grab the 4 bits recall the nibbles are backwards so it's a shift by
       * (i^1)*4 */
      u = (pB[j] >> ((i ^ 1) << 2)) & 15;

      /* add offset by the word count the table looked up value to the result */
      for (k = 0; k < (int)WPV; k++) {
        tmp[k + j] ^= B[u][k];
      }
    }
    /* shift result up by 4 bits */
    if (i != 0) {
      for (z = j = 0; j < (int)(32 / sizeof(LTC_FAST_TYPE)); j++) {
        zz = tmp[j] << (BPD - 4);
        tmp[j] = (tmp[j] >> 4) | z;
        z = zz;
      }
    }
  }

  /* store product */
  for (i = 0; i < 4; i++) {
    STORE64H(tmp[i], pTmp + (i << 3));
  }

  /* reduce by taking most significant byte and adding the appropriate two byte
   * sequence 16 bytes down */
  for (i = 31; i >= 16; i--) {
    pTmp[i - 16] ^= gcm_shift_table[((unsigned)pTmp[i] << 1)];
    pTmp[i - 15] ^= gcm_shift_table[((unsigned)pTmp[i] << 1) + 1];
  }

  for (i = 0; i < 16; i++) {
    c[i] = pTmp[i];
  }
}

/**
  Add AAD to the GCM state
  @param gcm       The GCM state
  @param adata     The additional authentication data to add to the GCM state
  @param adatalen  The length of the AAD data.
  @return CRYPT_OK on success
 */
int gcm_add_aad(gcm_state *gcm, const unsigned char *adata,
                unsigned long adatalen) {
  unsigned long x;
  int err;
  unsigned long y;

  LTC_ARGCHK(gcm != NULL);
  if (adatalen > 0) {
    LTC_ARGCHK(adata != NULL);
  }

  if (gcm->buflen > 16 || gcm->buflen < 0) {
    return CRYPT_INVALID_ARG;
  }

  if ((err = cipher_is_valid(gcm->cipher)) != CRYPT_OK) {
    return err;
  }

  /* in IV mode? */
  if (gcm->mode == LTC_GCM_MODE_IV) {
    /* let's process the IV */
    if (gcm->ivmode || gcm->buflen != 12) {
      for (x = 0; x < (unsigned long)gcm->buflen; x++) {
        gcm->X[x] ^= gcm->buf[x];
      }
      if (gcm->buflen) {
        gcm->totlen += gcm->buflen * CONST64(8);
        gcm_mult_h(gcm, gcm->X);
      }

      /* mix in the length */
      zeromem(gcm->buf, 8);
      STORE64H(gcm->totlen, gcm->buf + 8);
      for (x = 0; x < 16; x++) {
        gcm->X[x] ^= gcm->buf[x];
      }
      gcm_mult_h(gcm, gcm->X);

      /* copy counter out */
      XMEMCPY(gcm->Y, gcm->X, 16);
      zeromem(gcm->X, 16);
    } else {
      XMEMCPY(gcm->Y, gcm->buf, 12);
      gcm->Y[12] = 0;
      gcm->Y[13] = 0;
      gcm->Y[14] = 0;
      gcm->Y[15] = 1;
    }
    XMEMCPY(gcm->Y_0, gcm->Y, 16);
    zeromem(gcm->buf, 16);
    gcm->buflen = 0;
    gcm->totlen = 0;
    gcm->mode = LTC_GCM_MODE_AAD;
  }

  if (gcm->mode != LTC_GCM_MODE_AAD || gcm->buflen >= 16) {
    return CRYPT_INVALID_ARG;
  }

  x = 0;
  if (gcm->buflen == 0) {
    for (x = 0; x < (adatalen & ~15); x += 16) {
      for (y = 0; y < 16; y += sizeof(LTC_FAST_TYPE)) {
        *((LTC_FAST_TYPE *)(&gcm->X[y])) ^= *((LTC_FAST_TYPE *)(&adata[x + y]));
      }
      gcm_mult_h(gcm, gcm->X);
      gcm->totlen += 128;
    }
    adata += x;
  }

  /* start adding AAD data to the state */
  for (; x < adatalen; x++) {
    gcm->X[gcm->buflen++] ^= *adata++;

    if (gcm->buflen == 16) {
      /* GF mult it */
      gcm_mult_h(gcm, gcm->X);
      gcm->buflen = 0;
      gcm->totlen += 128;
    }
  }

  return CRYPT_OK;
}

/**
  Reset a GCM state to as if you just called gcm_init().  This saves the
  initialization time.
  @param gcm   The GCM state to reset
  @return CRYPT_OK on success
*/
int gcm_reset(gcm_state *gcm) {
  LTC_ARGCHK(gcm != NULL);

  zeromem(gcm->buf, sizeof(gcm->buf));
  zeromem(gcm->X, sizeof(gcm->X));
  gcm->mode = LTC_GCM_MODE_IV;
  gcm->ivmode = 0;
  gcm->buflen = 0;
  gcm->totlen = 0;
  gcm->pttotlen = 0;

  return CRYPT_OK;
}

/**
  @file md5.c
  LTC_MD5 hash function by Tom St Denis
*/

const struct ltc_hash_descriptor md5_desc = {
    "md5",
    16,
    64,

    /* OID */
    {
        1,
        2,
        840,
        113549,
        2,
        5,
    },
    6,

    &md5_init,
    &md5_process,
    &md5_done,
};

#define F(x, y, z) (z ^ (x & (y ^ z)))
#define G(x, y, z) (y ^ (z & (y ^ x)))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | (~z)))

#define FF(a, b, c, d, M, s, t) \
  a = (a + F(b, c, d) + M + t); \
  a = ROLc(a, s) + b;

#define GG(a, b, c, d, M, s, t) \
  a = (a + G(b, c, d) + M + t); \
  a = ROLc(a, s) + b;

#define HH(a, b, c, d, M, s, t) \
  a = (a + H(b, c, d) + M + t); \
  a = ROLc(a, s) + b;

#define II(a, b, c, d, M, s, t) \
  a = (a + I(b, c, d) + M + t); \
  a = ROLc(a, s) + b;

static int md5_compress(hash_state *md, unsigned char *buf) {
  ulong32 i, W[16], a, b, c, d;

  /* copy the state into 512-bits into W[0..15] */
  for (i = 0; i < 16; i++) {
    LOAD32L(W[i], buf + (4 * i));
  }

  /* copy state */
  a = md->md5.state[0];
  b = md->md5.state[1];
  c = md->md5.state[2];
  d = md->md5.state[3];

  FF(a, b, c, d, W[0], 7, 0xd76aa478UL)
  FF(d, a, b, c, W[1], 12, 0xe8c7b756UL)
  FF(c, d, a, b, W[2], 17, 0x242070dbUL)
  FF(b, c, d, a, W[3], 22, 0xc1bdceeeUL)
  FF(a, b, c, d, W[4], 7, 0xf57c0fafUL)
  FF(d, a, b, c, W[5], 12, 0x4787c62aUL)
  FF(c, d, a, b, W[6], 17, 0xa8304613UL)
  FF(b, c, d, a, W[7], 22, 0xfd469501UL)
  FF(a, b, c, d, W[8], 7, 0x698098d8UL)
  FF(d, a, b, c, W[9], 12, 0x8b44f7afUL)
  FF(c, d, a, b, W[10], 17, 0xffff5bb1UL)
  FF(b, c, d, a, W[11], 22, 0x895cd7beUL)
  FF(a, b, c, d, W[12], 7, 0x6b901122UL)
  FF(d, a, b, c, W[13], 12, 0xfd987193UL)
  FF(c, d, a, b, W[14], 17, 0xa679438eUL)
  FF(b, c, d, a, W[15], 22, 0x49b40821UL)
  GG(a, b, c, d, W[1], 5, 0xf61e2562UL)
  GG(d, a, b, c, W[6], 9, 0xc040b340UL)
  GG(c, d, a, b, W[11], 14, 0x265e5a51UL)
  GG(b, c, d, a, W[0], 20, 0xe9b6c7aaUL)
  GG(a, b, c, d, W[5], 5, 0xd62f105dUL)
  GG(d, a, b, c, W[10], 9, 0x02441453UL)
  GG(c, d, a, b, W[15], 14, 0xd8a1e681UL)
  GG(b, c, d, a, W[4], 20, 0xe7d3fbc8UL)
  GG(a, b, c, d, W[9], 5, 0x21e1cde6UL)
  GG(d, a, b, c, W[14], 9, 0xc33707d6UL)
  GG(c, d, a, b, W[3], 14, 0xf4d50d87UL)
  GG(b, c, d, a, W[8], 20, 0x455a14edUL)
  GG(a, b, c, d, W[13], 5, 0xa9e3e905UL)
  GG(d, a, b, c, W[2], 9, 0xfcefa3f8UL)
  GG(c, d, a, b, W[7], 14, 0x676f02d9UL)
  GG(b, c, d, a, W[12], 20, 0x8d2a4c8aUL)
  HH(a, b, c, d, W[5], 4, 0xfffa3942UL)
  HH(d, a, b, c, W[8], 11, 0x8771f681UL)
  HH(c, d, a, b, W[11], 16, 0x6d9d6122UL)
  HH(b, c, d, a, W[14], 23, 0xfde5380cUL)
  HH(a, b, c, d, W[1], 4, 0xa4beea44UL)
  HH(d, a, b, c, W[4], 11, 0x4bdecfa9UL)
  HH(c, d, a, b, W[7], 16, 0xf6bb4b60UL)
  HH(b, c, d, a, W[10], 23, 0xbebfbc70UL)
  HH(a, b, c, d, W[13], 4, 0x289b7ec6UL)
  HH(d, a, b, c, W[0], 11, 0xeaa127faUL)
  HH(c, d, a, b, W[3], 16, 0xd4ef3085UL)
  HH(b, c, d, a, W[6], 23, 0x04881d05UL)
  HH(a, b, c, d, W[9], 4, 0xd9d4d039UL)
  HH(d, a, b, c, W[12], 11, 0xe6db99e5UL)
  HH(c, d, a, b, W[15], 16, 0x1fa27cf8UL)
  HH(b, c, d, a, W[2], 23, 0xc4ac5665UL)
  II(a, b, c, d, W[0], 6, 0xf4292244UL)
  II(d, a, b, c, W[7], 10, 0x432aff97UL)
  II(c, d, a, b, W[14], 15, 0xab9423a7UL)
  II(b, c, d, a, W[5], 21, 0xfc93a039UL)
  II(a, b, c, d, W[12], 6, 0x655b59c3UL)
  II(d, a, b, c, W[3], 10, 0x8f0ccc92UL)
  II(c, d, a, b, W[10], 15, 0xffeff47dUL)
  II(b, c, d, a, W[1], 21, 0x85845dd1UL)
  II(a, b, c, d, W[8], 6, 0x6fa87e4fUL)
  II(d, a, b, c, W[15], 10, 0xfe2ce6e0UL)
  II(c, d, a, b, W[6], 15, 0xa3014314UL)
  II(b, c, d, a, W[13], 21, 0x4e0811a1UL)
  II(a, b, c, d, W[4], 6, 0xf7537e82UL)
  II(d, a, b, c, W[11], 10, 0xbd3af235UL)
  II(c, d, a, b, W[2], 15, 0x2ad7d2bbUL)
  II(b, c, d, a, W[9], 21, 0xeb86d391UL)

  md->md5.state[0] = md->md5.state[0] + a;
  md->md5.state[1] = md->md5.state[1] + b;
  md->md5.state[2] = md->md5.state[2] + c;
  md->md5.state[3] = md->md5.state[3] + d;

  return CRYPT_OK;
}

/**
   Initialize the hash state
   @param md   The hash state you wish to initialize
   @return CRYPT_OK if successful
*/
int md5_init(hash_state *md) {
  LTC_ARGCHK(md != NULL);
  md->md5.state[0] = 0x67452301UL;
  md->md5.state[1] = 0xefcdab89UL;
  md->md5.state[2] = 0x98badcfeUL;
  md->md5.state[3] = 0x10325476UL;
  md->md5.curlen = 0;
  md->md5.length = 0;
  return CRYPT_OK;
}

/**
   Process a block of memory though the hash
   @param md     The hash state
   @param in     The data to hash
   @param inlen  The length of the data (octets)
   @return CRYPT_OK if successful
*/
HASH_PROCESS(md5_process, md5_compress, md5, 64)

/**
   Terminate the hash to get the digest
   @param md  The hash state
   @param out [out] The destination of the hash (16 bytes)
   @return CRYPT_OK if successful
*/
int md5_done(hash_state *md, unsigned char *out) {
  int i;

  LTC_ARGCHK(md != NULL);
  LTC_ARGCHK(out != NULL);

  if (md->md5.curlen >= sizeof(md->md5.buf)) {
    return CRYPT_INVALID_ARG;
  }

  /* increase the length of the message */
  md->md5.length += md->md5.curlen * 8;

  /* append the '1' bit */
  md->md5.buf[md->md5.curlen++] = (unsigned char)0x80;

  /* if the length is currently above 56 bytes we append zeros
   * then compress.  Then we can fall back to padding zeros and length
   * encoding like normal.
   */
  if (md->md5.curlen > 56) {
    while (md->md5.curlen < 64) {
      md->md5.buf[md->md5.curlen++] = (unsigned char)0;
    }
    md5_compress(md, md->md5.buf);
    md->md5.curlen = 0;
  }

  /* pad upto 56 bytes of zeroes */
  while (md->md5.curlen < 56) {
    md->md5.buf[md->md5.curlen++] = (unsigned char)0;
  }

  /* store length */
  STORE64L(md->md5.length, md->md5.buf + 56);
  md5_compress(md, md->md5.buf);

  /* copy output */
  for (i = 0; i < 4; i++) {
    STORE32L(md->md5.state[i], out + (4 * i));
  }
  return CRYPT_OK;
}

#include <arpa/inet.h>

#if (CRYPT <= 0x0117)
#define LTC_PKCS_1_EMSA LTC_LTC_PKCS_1_EMSA
#define LTC_PKCS_1_V1_5 LTC_LTC_PKCS_1_V1_5
#define LTC_PKCS_1_PSS LTC_LTC_PKCS_1_PSS
#endif

#define SSL_V30 0x0300
#define TLS_V10 0x0301
#define TLS_V11 0x0302
#define TLS_V12 0x0303
#define TLS_V13 0x0304
#define DTLS_V10 0xFEFF
#define DTLS_V12 0xFEFD
#define DTLS_V13 0xFEFC

#define TLS_NEED_MORE_DATA 0
#define TLS_GENERIC_ERROR -1
#define TLS_BROKEN_PACKET -2
#define TLS_NOT_UNDERSTOOD -3
#define TLS_NOT_SAFE -4
#define TLS_NO_COMMON_CIPHER -5
#define TLS_UNEXPECTED_MESSAGE -6
#define TLS_COMPRESSION_NOT_SUPPORTED -8
#define TLS_NO_MEMORY -9
#define TLS_NOT_VERIFIED -10
#define TLS_INTEGRITY_FAILED -11
#define TLS_ERROR_ALERT -12
#define TLS_BROKEN_CONNECTION -13
#define TLS_BAD_CERTIFICATE -14
#define TLS_UNSUPPORTED_CERTIFICATE -15
#define TLS_NO_RENEGOTIATION -16
#define TLS_FEATURE_NOT_SUPPORTED -17
#define TLS_DECRYPTION_FAILED -20

#define TLS_AES_128_GCM_SHA256 0x1301
#define TLS_AES_256_GCM_SHA384 0x1302
#define TLS_CHACHA20_POLY1305_SHA256 0x1303
#define TLS_AES_128_CCM_SHA256 0x1304
#define TLS_AES_128_CCM_8_SHA256 0x1305

#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA 0x0033
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA 0x0039
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA256 0x0067
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA256 0x006B

#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA 0xC013
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA 0xC014
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256 0xC027

#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA 0xC009
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA 0xC00A
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 0xC023
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384 0xC024

#define TLS_RSA_WITH_AES_128_CBC_SHA 0x002F
#define TLS_RSA_WITH_AES_256_CBC_SHA 0x0035
#define TLS_RSA_WITH_AES_128_CBC_SHA256 0x003C
#define TLS_RSA_WITH_AES_256_CBC_SHA256 0x003D

#define TLS_RSA_WITH_AES_128_GCM_SHA256 0x009C
#define TLS_RSA_WITH_AES_256_GCM_SHA384 0x009D

// forward secrecy
#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA 0x0033
#define TLS_DHE_RSA_WITH_AES_128_GCM_SHA256 0x009E
#define TLS_DHE_RSA_WITH_AES_256_GCM_SHA384 0x009F

#define TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 0xC02F
#define TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 0xC030

#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA 0xC009
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA 0xC00A
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 0xC02B
#define TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 0xC02C

#define TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256 0xCCA8
#define TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 0xCCA9
#define TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256 0xCCAA

#define TLS_FALLBACK_SCSV 0x5600

#define TLS_UNSUPPORTED_ALGORITHM 0x00
#define TLS_RSA_SIGN_RSA 0x01
#define TLS_RSA_SIGN_MD5 0x04
#define TLS_RSA_SIGN_SHA1 0x05
#define TLS_RSA_SIGN_SHA256 0x0B
#define TLS_RSA_SIGN_SHA384 0x0C
#define TLS_RSA_SIGN_SHA512 0x0D

#define TLS_EC_PUBLIC_KEY 0x11
#define TLS_EC_prime192v1 0x12
#define TLS_EC_prime192v2 0x13
#define TLS_EC_prime192v3 0x14
#define TLS_EC_prime239v1 0x15
#define TLS_EC_prime239v2 0x16
#define TLS_EC_prime239v3 0x17
#define TLS_EC_prime256v1 0x18
#define TLS_EC_secp224r1 21
#define TLS_EC_secp256r1 23
#define TLS_EC_secp384r1 24
#define TLS_EC_secp521r1 25

#define TLS_ALERT_WARNING 0x01
#define TLS_ALERT_CRITICAL 0x02

#define TLS_CIPHERS_SIZE(n, mitigated) n * 2

typedef enum {
  close_notify = 0,
  unexpected_message = 10,
  bad_record_mac = 20,
  decryption_failed_RESERVED = 21,
  decompression_failure = 30,
  bad_certificate = 42,
  unsupported_certificate = 43,
  certificate_expired = 45,
  certificate_unknown = 46,
  decode_error = 50,
  decrypt_error = 51,
  insufficient_security = 71,
  internal_error = 80,
  inappropriate_fallback = 86,
  no_renegotiation = 100,
  no_error = 255
} TLSAlertDescription;

// forward declarations
struct TLSPacket;
struct TLSCertificate;
struct TLSContext;
struct ECCCurveParameters;
typedef struct TLSContext TLS;
typedef struct TLSCertificate Certificate;

typedef int (*tls_validation_function)(
    struct TLSContext *context, struct TLSCertificate **certificate_chain,
    int len);

/*
  Global initialization. Optional, as it will be called automatically;
  however, the initialization is not thread-safe, so if you intend to use TLSe
  from multiple threads, you'll need to call tls_init() once, from a single
  thread, before using the library.
 */
void tls_init();
unsigned char *tls_pem_decode(const unsigned char *data_in,
                              unsigned int input_length, int cert_index,
                              unsigned int *output_len);
struct TLSCertificate *tls_create_certificate();
int tls_certificate_valid_subject(struct TLSCertificate *cert,
                                  const char *subject);
int tls_certificate_valid_subject_name(const unsigned char *cert_subject,
                                       const char *subject);
int tls_certificate_is_valid(struct TLSCertificate *cert);
void tls_certificate_set_copy(unsigned char **member, const unsigned char *val,
                              int len);
void tls_certificate_set_copy_date(unsigned char **member,
                                   const unsigned char *val, int len);
void tls_certificate_set_key(struct TLSCertificate *cert,
                             const unsigned char *val, int len);
void tls_certificate_set_priv(struct TLSCertificate *cert,
                              const unsigned char *val, int len);
void tls_certificate_set_sign_key(struct TLSCertificate *cert,
                                  const unsigned char *val, int len);
char *tls_certificate_to_string(struct TLSCertificate *cert, char *buffer,
                                int len);
void tls_certificate_set_exponent(struct TLSCertificate *cert,
                                  const unsigned char *val, int len);
void tls_certificate_set_serial(struct TLSCertificate *cert,
                                const unsigned char *val, int len);
void tls_certificate_set_algorithm(unsigned int *algorithm,
                                   const unsigned char *val, int len);
void tls_destroy_certificate(struct TLSCertificate *cert);
struct TLSPacket *tls_create_packet(struct TLSContext *context,
                                    unsigned char type, unsigned short version,
                                    int payload_size_hint);
void tls_destroy_packet(struct TLSPacket *packet);
void tls_packet_update(struct TLSPacket *packet);
int tls_packet_append(struct TLSPacket *packet, const unsigned char *buf,
                      unsigned int len);
int tls_packet_uint8(struct TLSPacket *packet, unsigned char i);
int tls_packet_uint16(struct TLSPacket *packet, unsigned short i);

int tls_packet_uint24(struct TLSPacket *packet, unsigned int i);
// int tls_random(unsigned char *key, int len);

/*
  Get encrypted data to write, if any. Once you've sent all of it, call
  tls_buffer_clear().
 */
const unsigned char *tls_get_write_buffer(struct TLSContext *context,
                                          unsigned int *outlen);

void tls_buffer_clear(struct TLSContext *context);

/* Returns 1 for established, 0 for not established yet, and -1 for a critical
 * error. */
int tls_established(struct TLSContext *context);

/*
  Reads any unread decrypted data (see tls_consume_stream). If you don't read
  all of it, the remainder will be left in the internal buffers for next
  tls_read(). Returns -1 for fatal error, 0 for no more data, or otherwise the
  number of bytes copied into the buffer (up to a maximum of the given size).
 */
int tls_read(struct TLSContext *context, unsigned char *buf, unsigned int size);

struct TLSContext *tls_create_context(unsigned char is_server,
                                      unsigned short version);

/* Create a context for a given client, from a server context. Returns NULL on
 * error. */
struct TLSContext *tls_accept(struct TLSContext *context);

int tls_set_default_dhe_pg(struct TLSContext *context, const char *p_hex_str,
                           const char *g_hex_str);
void tls_destroy_context(struct TLSContext *context);
int tls_cipher_supported(struct TLSContext *context, unsigned short cipher);
int tls_cipher_is_fs(struct TLSContext *context, unsigned short cipher);
int tls_choose_cipher(struct TLSContext *context, const unsigned char *buf,
                      int buf_len, int *scsv_set);
int tls_cipher_is_ephemeral(struct TLSContext *context);
const char *tls_cipher_name(struct TLSContext *context);
int tls_is_ecdsa(struct TLSContext *context);
struct TLSPacket *tls_build_client_key_exchange(struct TLSContext *context);
struct TLSPacket *tls_build_server_key_exchange(struct TLSContext *context,
                                                int method);
struct TLSPacket *tls_build_hello(struct TLSContext *context,
                                  int tls13_downgrade);
struct TLSPacket *tls_certificate_request(struct TLSContext *context);
struct TLSPacket *tls_build_verify_request(struct TLSContext *context);
int tls_parse_hello(struct TLSContext *context, const unsigned char *buf,
                    int buf_len, unsigned int *write_packets,
                    unsigned int *dtls_verified);
int tls_parse_certificate(struct TLSContext *context, const unsigned char *buf,
                          int buf_len, int is_client);
int tls_parse_server_key_exchange(struct TLSContext *context,
                                  const unsigned char *buf, int buf_len);
int tls_parse_client_key_exchange(struct TLSContext *context,
                                  const unsigned char *buf, int buf_len);
int tls_parse_server_hello_done(struct TLSContext *context,
                                const unsigned char *buf, int buf_len);
int tls_parse_finished(struct TLSContext *context, const unsigned char *buf,
                       int buf_len, unsigned int *write_packets);
int tls_parse_verify(struct TLSContext *context, const unsigned char *buf,
                     int buf_len);
int tls_parse_payload(struct TLSContext *context, const unsigned char *buf,
                      int buf_len, tls_validation_function certificate_verify);
int tls_parse_message(struct TLSContext *context, unsigned char *buf,
                      int buf_len, tls_validation_function certificate_verify);
int tls_certificate_verify_signature(struct TLSCertificate *cert,
                                     struct TLSCertificate *parent);
int tls_certificate_chain_is_valid(struct TLSCertificate **certificates,
                                   int len);
int tls_certificate_chain_is_valid_root(struct TLSContext *context,
                                        struct TLSCertificate **certificates,
                                        int len);

/*
  Add a certificate or a certificate chain to the given context, in PEM form.
  Returns a negative value (TLS_GENERIC_ERROR etc.) on error, 0 if there were no
  certificates in the buffer, or the number of loaded certificates on success.
 */
int tls_load_certificates(struct TLSContext *context,
                          const unsigned char *pem_buffer,
                          unsigned long pem_size);

/*
  Add a private key to the given context, in PEM form. Returns a negative value
  (TLS_GENERIC_ERROR etc.) on error, 0 if there was no private key in the
  buffer, or 1 on success.
 */
int tls_load_private_key(struct TLSContext *context,
                         const unsigned char *pem_buffer,
                         unsigned long pem_size);
struct TLSPacket *tls_build_certificate(struct TLSContext *context);
struct TLSPacket *tls_build_finished(struct TLSContext *context);
struct TLSPacket *tls_build_change_cipher_spec(struct TLSContext *context);
struct TLSPacket *tls_build_done(struct TLSContext *context);
struct TLSPacket *tls_build_message(struct TLSContext *context,
                                    const unsigned char *data,
                                    unsigned int len);
int tls_client_connect(struct TLSContext *context);
int tls_write(struct TLSContext *context, const unsigned char *data,
              unsigned int len);
struct TLSPacket *tls_build_alert(struct TLSContext *context, char critical,
                                  unsigned char code);

/*
  Process a given number of input bytes from a socket. If the other side just
  presented a certificate and certificate_verify is not NULL, it will be called.

  Returns 0 if there's no data ready yet, a negative value (see
  TLS_GENERIC_ERROR etc.) for an error, or a positive value (the number of bytes
  used from buf) if one or more complete TLS messages were received. The data
  is copied into an internal buffer even if not all of it was consumed,
  so you should not re-send it the next time.

  Decrypted data, if any, should be read back with tls_read(). Can change the
  status of tls_established(). If the library has anything to send back on the
  socket (e.g. as part of the handshake), tls_get_write_buffer() will return
  non-NULL.
 */
int tls_consume_stream(struct TLSContext *context, const unsigned char *buf,
                       ssize_t buf_len,
                       tls_validation_function certificate_verify);
void tls_close_notify(struct TLSContext *context);

const char *tls_sni(struct TLSContext *context);
int tls_sni_set(struct TLSContext *context, const char *sni);
int tls_load_root_certificates(struct TLSContext *context,
                               const unsigned char *pem_buffer,
                               size_t pem_size);

void tls_print_certificate(const char *fname);
// int tls_add_alpn(struct TLSContext *context, const char *alpn);
int tls_alpn_contains(struct TLSContext *context, const char *alpn,
                      unsigned char alpn_size);

// useful when renewing certificates for servers, without the need to restart
// the server
int tls_clear_certificates(struct TLSContext *context);
/*
  Creates a new DTLS random cookie secret to be used in HelloVerifyRequest
  (server-side). It is recommended to call this function from time to time, to
  protect against some DoS attacks.
*/
void dtls_reset_cookie_secret();

// from NaCl

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
 * Handy CPP defines and C inline functions.
 */

/* Evaluates to the number of items in array-type variable arr. */
#define ARRAYCOUNT(arr) (sizeof arr / sizeof arr[0])

/* Normal MIN/MAX macros.  Evaluate argument expressions only once. */
#ifndef MIN
#define MIN(x, y)          \
  ({                       \
    typeof(x) __x = (x);   \
    typeof(y) __y = (y);   \
    __x < __y ? __x : __y; \
  })
#endif
#ifndef MAX
#define MAX(x, y)          \
  ({                       \
    typeof(x) __x = (x);   \
    typeof(y) __y = (y);   \
    __x > __y ? __x : __y; \
  })
#endif

/* Swap two values.  Uses GCC type inference magic. */
#ifndef SWAP
#define SWAP(x, y)         \
  do {                     \
    typeof(x) __tmp = (x); \
    (x) = (y);             \
    (y) = __tmp;           \
  } while (0)
#endif

/** Stringify its argument. */
#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

/* Error handling macros.
 *
 * These expect a zero = success, non-zero = error convention.
 */

/** Error: return.
 *
 *  If the expression fails, return the error from this function. */
#define ER(expr)                \
  do {                          \
    typeof(expr) err_ = (expr); \
    if (err_) return err_;      \
  } while (0)

/** Error: goto.
 *
 *  If the expression fails, goto x_err.  Assumes defn of label
 *  x_err and 'error_type err'. */
#define EG(expr)         \
  do {                   \
    err = (expr);        \
    if (err) goto x_err; \
  } while (0)

/** Like memset(ptr, 0, len), but not allowed to be removed by
 *  compilers. */
static inline void mem_clean(volatile void *v, size_t len) {
  if (len) {
    memset((void *)v, 0, len);
    (void)*((volatile uint8_t *)v);
  }
}

/** Returns 1 if len bytes at va equal len bytes at vb, 0 if they do not.
 *  Does not leak length of common prefix through timing. */
static inline unsigned mem_eq(const void *va, const void *vb, size_t len) {
  const volatile uint8_t *a = va;
  const volatile uint8_t *b = vb;
  uint8_t diff = 0;

  while (len--) {
    diff |= *a++ ^ *b++;
  }

  return !diff;
}

#include <stddef.h>
#include <stdint.h>

typedef int64_t gf[16];

static const uint8_t _0[16], _9[32] = {9};
static const gf gf0,
    gf1 = {1}, _121665 = {0xDB41, 1},
    D = {0x78a3, 0x1359, 0x4dca, 0x75eb, 0xd8ab, 0x4141, 0x0a4d, 0x0070,
         0xe898, 0x7779, 0x4079, 0x8cc7, 0xfe73, 0x2b6f, 0x6cee, 0x5203},
    D2 = {0xf159, 0x26b2, 0x9b94, 0xebd6, 0xb156, 0x8283, 0x149a, 0x00e0,
          0xd130, 0xeef3, 0x80f2, 0x198e, 0xfce7, 0x56df, 0xd9dc, 0x2406},
    X = {0xd51a, 0x8f25, 0x2d60, 0xc956, 0xa7b2, 0x9525, 0xc760, 0x692c,
         0xdc5c, 0xfdd6, 0xe231, 0xc0a4, 0x53fe, 0xcd6e, 0x36d3, 0x2169},
    Y = {0x6658, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666,
         0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666, 0x6666},
    I = {0xa0b0, 0x4a0e, 0x1b27, 0xc4ee, 0xe478, 0xad2f, 0x1806, 0x2f43,
         0xd7a7, 0x3dfb, 0x0099, 0x2b4d, 0xdf0b, 0x4fc1, 0x2480, 0x2b83};

static void set25519(gf r, const gf a) {
  for (size_t i = 0; i < 16; i++) r[i] = a[i];
}

static void car25519(gf o) {
  int64_t c;

  for (size_t i = 0; i < 16; i++) {
    o[i] += (1LL << 16);
    c = o[i] >> 16;
    o[(i + 1) * (i < 15)] += c - 1 + 37 * (c - 1) * (i == 15);
    o[i] -= c << 16;
  }
}

static void sel25519(gf p, gf q, int b) {
  int64_t tmp, mask = ~(b - 1);
  for (size_t i = 0; i < 16; i++) {
    tmp = mask & (p[i] ^ q[i]);
    p[i] ^= tmp;
    q[i] ^= tmp;
  }
}

static void pack25519(uint8_t out[32], const gf n) {
  int b;
  gf m, t;
  set25519(t, n);
  car25519(t);
  car25519(t);
  car25519(t);

  for (size_t j = 0; j < 2; j++) {
    m[0] = t[0] - 0xffed;
    for (size_t i = 1; i < 15; i++) {
      m[i] = t[i] - 0xffff - ((m[i - 1] >> 16) & 1);
      m[i - 1] &= 0xffff;
    }
    m[15] = t[15] - 0x7fff - ((m[14] >> 16) & 1);
    b = (m[15] >> 16) & 1;
    m[14] &= 0xffff;
    sel25519(t, m, 1 - b);
  }

  for (size_t i = 0; i < 16; i++) {
    out[2 * i] = t[i] & 0xff;
    out[2 * i + 1] = t[i] >> 8;
  }
}

static void unpack25519(gf o, const uint8_t *n) {
  for (size_t i = 0; i < 16; i++)
    o[i] = n[2 * i] + ((int64_t)n[2 * i + 1] << 8);
  o[15] &= 0x7fff;
}

static void c5_add(gf o, const gf a, const gf b) {
  for (size_t i = 0; i < 16; i++) o[i] = a[i] + b[i];
}

static void c5_sub(gf o, const gf a, const gf b) {
  for (size_t i = 0; i < 16; i++) o[i] = a[i] - b[i];
}

static void c5_mul(gf o, const gf a, const gf b) {
  int64_t t[31];

  for (size_t i = 0; i < 31; i++) t[i] = 0;

  for (size_t i = 0; i < 16; i++)
    for (size_t j = 0; j < 16; j++) t[i + j] += a[i] * b[j];

  for (size_t i = 0; i < 15; i++) t[i] += 38 * t[i + 16];

  for (size_t i = 0; i < 16; i++) o[i] = t[i];

  car25519(o);
  car25519(o);
}

static void c5_sqr(gf o, const gf a) { c5_mul(o, a, a); }

static void inv25519(gf o, const gf i) {
  gf c;
  for (int a = 0; a < 16; a++) c[a] = i[a];

  for (int a = 253; a >= 0; a--) {
    c5_sqr(c, c);
    if (a != 2 && a != 4) c5_mul(c, c, i);
  }

  for (int a = 0; a < 16; a++) o[a] = c[a];
}

void curve25519(uint8_t *q, const uint8_t *n, const uint8_t *p) {
  uint8_t z[32];
  gf x;
  gf a, b, c, d, e, f;

  for (size_t i = 0; i < 31; i++) z[i] = n[i];
  z[31] = (n[31] & 127) | 64;
  z[0] &= 248;

  unpack25519(x, p);

  for (size_t i = 0; i < 16; i++) {
    b[i] = x[i];
    d[i] = a[i] = c[i] = 0;
  }

  a[0] = d[0] = 1;

  for (int i = 254; i >= 0; i--) {
    int64_t r = (z[i >> 3] >> (i & 7)) & 1;
    sel25519(a, b, r);
    sel25519(c, d, r);
    c5_add(e, a, c);
    c5_sub(a, a, c);
    c5_add(c, b, d);
    c5_sub(b, b, d);
    c5_sqr(d, e);
    c5_sqr(f, a);
    c5_mul(a, c, a);
    c5_mul(c, b, e);
    c5_add(e, a, c);
    c5_sub(a, a, c);
    c5_sqr(b, a);
    c5_sub(c, d, f);
    c5_mul(a, c, _121665);
    c5_add(a, a, d);
    c5_mul(c, c, a);
    c5_mul(a, d, f);
    c5_mul(d, b, x);
    c5_sqr(b, e);
    sel25519(a, b, r);
    sel25519(c, d, r);
  }

  inv25519(c, c);
  c5_mul(a, a, c);
  pack25519(q, a);
}

// using ChaCha20 implementation by D. J. Bernstein

// generared from a safe-prime with
// GENERATE:  openssl dhparam -dsaparam -out dh4k.pem 4096
// ENCODE:    openssl dhparam -inform PEM -in dh4k.pem -check -text

#define TLS_DH_DEFAULT_P                                                       \
  "00B90486BB459345F09EDC8F786D7BBA17D9FE4954C87D3E264A2CD2216ADA77272118D6F9" \
  "2F155644EAD21A5FE0DE91B7A014C0E8D70FAB9ED31B16DF2E4665BE8FE92514F6540836FF" \
  "CBDD8E85D06117F1D36E80318EDD6B7A2FD82B29E7766F3634BF6A521671A987167A7C9850" \
  "8D7CE6AFBD59D84EFAA296817E223BA83AE763900B5961C3402EC9AB7D5B94D7388036EA72" \
  "671DCF4DF67771E69A373A00BCA0EBE86B3AFA4A739BD188C3D84D36E12DEDA8D551672828" \
  "1729C0A028FA729422AD21EEA6230F3C877ACC0247DF98D7AA1A4D6B7C69B81DDF0D0479B6" \
  "9577ECFC7264EDDE92646DEF07A34920C64D5501E3DB4C2AC25D2A4C7A211B598796628043" \
  "798B7CEDCEF81CEF035E50B79332CAB810EBCE0760CCBA3012EA6F1143C4929F82C48D46DA" \
  "3BA1232A7D7DDEA37802F68FF1905E17400F10808888B6D7C3D5D5A69B2715EE9D510893AE" \
  "A0F1956AF42F3789570F69AAA54B3B72A762C961AA6D9545B0B955F41EB26165C2E6F3F292" \
  "83693B86D670C32FEE22933077B5F3EAB878E618E7DF21AD8AE660E20065A93D5E915B6B33" \
  "FA6B514D0E616432A28742EBE54427855F3F899C0CA29F96809F097F96469A7339AF350878" \
  "668F81A66E6779906BC61425F4C0B718C528AEAC1020EDCF352BE350AA296EFED7660DEC32" \
  "F984C929CAD41E62C8E3B325023D5DFD158A42565F2003F8ECD74D85A399C995"

#define TLS_DH_DEFAULT_G                                                       \
  "50b1047264232f44bae9673dda2968206a422cc5e8b25b24718bdcf747a1e1d4e8d0fc1c4c" \
  "b225ce4c00d1c2c35de34ee4aeafeef0690c0f4cb6049e6f0cc92acb225bd1315922175a19" \
  "87ccb0da7f947a0744e68b79c3c8d39bab02f3ee3d8245d4e98e218eca893a577fccebc55b" \
  "ff6d60ccf9e4fbc1db205026411e494047a9c4d061c6d797f2299d3142082f7150c54637b7" \
  "d952c13473752467dd9759ad7d3d18f5a5734f283753d82648d699ae883b2bcc9c31eb1168" \
  "4667a66c67a941e3a76b4184b022fd03d45fcfc4580623e27994c7ff3c1cda80cef69b5d9d" \
  "3c575288019d4cdab3a3c1240eb65ad8b532d717756ffbeba56638e4338218ee460a573d05" \
  "41e15463264209beae400fac206d2df5827a17dfc841a710ebb371c2fa398d9b93c50e66be" \
  "cf754fe2d227f5a6bb06e0c4dd49a4c63ffb6cd1248a7b84dcdbe383b45d72ec0af6730336" \
  "a65dbc4ab3302061bb0a5ac39abb3106eb4310f1d06d4ea4ebb2ebdbd563c6e08772d846b0" \
  "63b3964eb5070e2219f0c194a736fa6a310abc8f22614d490d47e868b58cbd6e4a523e869e" \
  "76f18062aae97186ffa2fc5ca9d872f9c751b3828d624395a8aea59e4abfb78ed25daa2bc3" \
  "7a7df74aa687ffa26f3c2edec80690f6ee55746f3b67cce981258bcd4d7cf47e23a39b9a31" \
  "beb6060412d0cea8e77827946a1354ca66af9ed9e3a8c3f87a23b2daa67033"

#define TLS_DHE_KEY_SIZE 4096

#ifndef TLS_MALLOC
#define TLS_MALLOC(size) malloc(size)
#endif
#ifndef TLS_REALLOC
#define TLS_REALLOC(ptr, size) realloc(ptr, size)
#endif
#ifndef TLS_FREE
#define TLS_FREE(ptr) \
  if (ptr) free(ptr)
#endif

#define TLS_ERROR(err, statement) \
  if (err) statement;

#ifdef DEBUG
#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#define DEBUG_DUMP_HEX(buf, len)                        \
  {                                                     \
    if (buf) {                                          \
      int _i_;                                          \
      for (_i_ = 0; _i_ < len; _i_++) {                 \
        DEBUG_PRINT("%02X ", (unsigned int)(buf)[_i_]); \
      }                                                 \
    } else {                                            \
      fprintf(stderr, "(null)");                        \
    }                                                   \
  }
#define DEBUG_INDEX(fields) print_index(fields)
#define DEBUG_DUMP(buf, length) fwrite(buf, 1, length, stderr);
#define DEBUG_DUMP_HEX_LABEL(title, buf, len)      \
  {                                                \
    fprintf(stderr, "%s (%i): ", title, (int)len); \
    DEBUG_DUMP_HEX(buf, len);                      \
    fprintf(stderr, "\n");                         \
  }
#else

#define DEBUG_PRINT(...) \
  {}
#define DEBUG_DUMP_HEX(buf, len) \
  {}
#define DEBUG_INDEX(fields) \
  {}
#define DEBUG_DUMP(buf, length) \
  {}
#define DEBUG_DUMP_HEX_LABEL(title, buf, len) \
  {}

#endif

#ifndef htonll
#define htonll(x)  \
  ((1 == htonl(1)) \
       ? (x)       \
       : ((uint64_t)htonl((x)&0xFFFFFFFF) << 32) | htonl((x) >> 32))
#endif

#ifndef ntohll
#define ntohll(x)  \
  ((1 == ntohl(1)) \
       ? (x)       \
       : ((uint64_t)ntohl((x)&0xFFFFFFFF) << 32) | ntohl((x) >> 32))
#endif

#define TLS_CHANGE_CIPHER 0x14
#define TLS_ALERT 0x15
#define TLS_HANDSHAKE 0x16
#define TLS_APPLICATION_DATA 0x17

#define TLS_CLIENT_HELLO_MINSIZE 41
#define TLS_CLIENT_RANDOM_SIZE 32
#define TLS_SERVER_RANDOM_SIZE 32
#define TLS_MAX_SESSION_ID 32
#define TLS_SHA256_MAC_SIZE 32
#define TLS_SHA1_MAC_SIZE 20
#define TLS_SHA384_MAC_SIZE 48
#define TLS_MAX_MAC_SIZE TLS_SHA384_MAC_SIZE
// 160
#define TLS_MAX_KEY_EXPANSION_SIZE 192
// 512bits (sha256) = 64 bytes
#define TLS_MAX_HASH_LEN 64
#define TLS_AES_IV_LENGTH 16
#define TLS_AES_BLOCK_SIZE 16
#define TLS_AES_GCM_IV_LENGTH 4
#define TLS_13_AES_GCM_IV_LENGTH 12
#define TLS_GCM_TAG_LEN 16
#define TLS_MAX_TAG_LEN 16
#define TLS_MIN_FINISHED_OPAQUE_LEN 12

#define TLS_BLOB_INCREMENT 0xFFF
#define TLS_ASN1_MAXLEVEL 0xFF

#define DTLS_COOKIE_SIZE 32

#define TLS_MAX_SHA_SIZE 48
// 16(md5) + 20(sha1)
#define TLS_V11_HASH_SIZE 36
#define TLS_MAX_HASH_SIZE TLS_MAX_SHA_SIZE
// 16(md5) + 20(sha1)
#define TLS_MAX_RSA_KEY 2048

#define TLS_MAXTLS_APP_SIZE 0x4000
// max 1 second sleep
#define TLS_MAX_ERROR_SLEEP_uS 1000000
// max 5 seconds context sleep
#define TLS_MAX_ERROR_IDLE_S 5

#define TLS_V13_MAX_KEY_SIZE 32
#define TLS_V13_MAX_IV_SIZE 12

#define VERSION_SUPPORTED(version, err)                                       \
  if ((version != TLS_V13) && (version != TLS_V12) && (version != TLS_V11) && \
      (version != TLS_V10) && (version != DTLS_V13) &&                        \
      (version != DTLS_V12) && (version != DTLS_V10)) {                       \
    if ((version == SSL_V30) && (context->connection_status == 0)) {          \
      version = TLS_V12;                                                      \
    } else {                                                                  \
      DEBUG_PRINT("UNSUPPORTED TLS VERSION %x\n", (int)version);              \
      return err;                                                             \
    }                                                                         \
  }
#define CHECK_SIZE(size, buf_size, err) \
  if (((int)(size) > (int)(buf_size)) || ((int)(buf_size) < 0)) return err;
#define CHECK_HANDSHAKE_STATE(context, n, limit)          \
  {                                                       \
    if (context->hs_messages[n] >= limit) {               \
      DEBUG_PRINT("* UNEXPECTED MESSAGE (%i)\n", (int)n); \
      payload_res = TLS_UNEXPECTED_MESSAGE;               \
      break;                                              \
    }                                                     \
    context->hs_messages[n]++;                            \
  }

#define TLS_CHACHA20_IV_LENGTH 12

// ChaCha20 implementation by D. J. Bernstein
// Public domain.

#define CHACHA_BLOCKLEN 64

#define POLY1305_MAX_AAD 32
#define POLY1305_KEYLEN 32
#define POLY1305_TAGLEN 16

#define u_int unsigned int
#define uint8_t unsigned char
#define u_char unsigned char
#ifndef NULL
#define NULL (void *)0
#endif

struct chacha_ctx {
  u_int input[16];
  uint8_t ks[CHACHA_BLOCKLEN];
  uint8_t unused;
};

static inline void chacha_keysetup(struct chacha_ctx *x, const u_char *k,
                                   u_int kbits);

static inline void chacha_encrypt_bytes(struct chacha_ctx *x, const u_char *m,
                                        u_char *c, u_int bytes);
static inline int poly1305_generate_key(unsigned char *key256,
                                        unsigned char *nonce,
                                        unsigned int noncelen,
                                        unsigned char *poly_key,
                                        unsigned int counter);

#define poly1305_block_size 16
#define poly1305_context poly1305_state_internal_t

//========== ChaCha20 from D. J. Bernstein ========= //
// Source available at https://cr.yp.to/chacha.html  //

typedef unsigned char u8;
typedef unsigned int u32;

typedef struct chacha_ctx chacha_ctx;

#define U8C(v) (v##U)
#define U32C(v) (v##U)

#define U8V(v) ((u8)(v)&U8C(0xFF))
#define U32V(v) ((u32)(v)&U32C(0xFFFFFFFF))

#define ROTL32(v, n) (U32V((v) << (n)) | ((v) >> (32 - (n))))

#define _private_tls_U8TO32_LITTLE(p)                               \
  (((u32)((p)[0])) | ((u32)((p)[1]) << 8) | ((u32)((p)[2]) << 16) | \
   ((u32)((p)[3]) << 24))

#define _private_tls_U32TO8_LITTLE(p, v) \
  do {                                   \
    (p)[0] = U8V((v));                   \
    (p)[1] = U8V((v) >> 8);              \
    (p)[2] = U8V((v) >> 16);             \
    (p)[3] = U8V((v) >> 24);             \
  } while (0)

#define ROTATE(v, c) (ROTL32(v, c))
#define XOR(v, w) ((v) ^ (w))
#define PLUS(v, w) (U32V((v) + (w)))
#define PLUSONE(v) (PLUS((v), 1))

#define QUARTERROUND(a, b, c, d) \
  a = PLUS(a, b);                \
  d = ROTATE(XOR(d, a), 16);     \
  c = PLUS(c, d);                \
  b = ROTATE(XOR(b, c), 12);     \
  a = PLUS(a, b);                \
  d = ROTATE(XOR(d, a), 8);      \
  c = PLUS(c, d);                \
  b = ROTATE(XOR(b, c), 7);

static const char sigma[] = "expand 32-byte k";
static const char tau[] = "expand 16-byte k";

static inline void chacha_keysetup(chacha_ctx *x, const u8 *k, u32 kbits) {
  const char *constants;

  x->input[4] = _private_tls_U8TO32_LITTLE(k + 0);
  x->input[5] = _private_tls_U8TO32_LITTLE(k + 4);
  x->input[6] = _private_tls_U8TO32_LITTLE(k + 8);
  x->input[7] = _private_tls_U8TO32_LITTLE(k + 12);
  if (kbits == 256) { /* recommended */
    k += 16;
    constants = sigma;
  } else { /* kbits == 128 */
    constants = tau;
  }
  x->input[8] = _private_tls_U8TO32_LITTLE(k + 0);
  x->input[9] = _private_tls_U8TO32_LITTLE(k + 4);
  x->input[10] = _private_tls_U8TO32_LITTLE(k + 8);
  x->input[11] = _private_tls_U8TO32_LITTLE(k + 12);
  x->input[0] = _private_tls_U8TO32_LITTLE(constants + 0);
  x->input[1] = _private_tls_U8TO32_LITTLE(constants + 4);
  x->input[2] = _private_tls_U8TO32_LITTLE(constants + 8);
  x->input[3] = _private_tls_U8TO32_LITTLE(constants + 12);
}

static inline void chacha_key(chacha_ctx *x, u8 *k) {
  _private_tls_U32TO8_LITTLE(k, x->input[4]);
  _private_tls_U32TO8_LITTLE(k + 4, x->input[5]);
  _private_tls_U32TO8_LITTLE(k + 8, x->input[6]);
  _private_tls_U32TO8_LITTLE(k + 12, x->input[7]);

  _private_tls_U32TO8_LITTLE(k + 16, x->input[8]);
  _private_tls_U32TO8_LITTLE(k + 20, x->input[9]);
  _private_tls_U32TO8_LITTLE(k + 24, x->input[10]);
  _private_tls_U32TO8_LITTLE(k + 28, x->input[11]);
}

static inline void chacha_nonce(chacha_ctx *x, u8 *nonce) {
  _private_tls_U32TO8_LITTLE(nonce + 0, x->input[13]);
  _private_tls_U32TO8_LITTLE(nonce + 4, x->input[14]);
  _private_tls_U32TO8_LITTLE(nonce + 8, x->input[15]);
}

static inline void chacha_ivsetup(chacha_ctx *x, const u8 *iv,
                                  const u8 *counter) {
  x->input[12] = counter == NULL ? 0 : _private_tls_U8TO32_LITTLE(counter + 0);
  x->input[13] = counter == NULL ? 0 : _private_tls_U8TO32_LITTLE(counter + 4);
  if (iv) {
    x->input[14] = _private_tls_U8TO32_LITTLE(iv + 0);
    x->input[15] = _private_tls_U8TO32_LITTLE(iv + 4);
  }
}

static inline void chacha_ivsetup_96bitnonce(chacha_ctx *x, const u8 *iv,
                                             const u8 *counter) {
  x->input[12] = counter == NULL ? 0 : _private_tls_U8TO32_LITTLE(counter + 0);
  if (iv) {
    x->input[13] = _private_tls_U8TO32_LITTLE(iv + 0);
    x->input[14] = _private_tls_U8TO32_LITTLE(iv + 4);
    x->input[15] = _private_tls_U8TO32_LITTLE(iv + 8);
  }
}

static inline void chacha_ivupdate(chacha_ctx *x, const u8 *iv, const u8 *aad,
                                   const u8 *counter) {
  x->input[12] = counter == NULL ? 0 : _private_tls_U8TO32_LITTLE(counter + 0);
  x->input[13] = _private_tls_U8TO32_LITTLE(iv + 0);
  x->input[14] =
      _private_tls_U8TO32_LITTLE(iv + 4) ^ _private_tls_U8TO32_LITTLE(aad);
  x->input[15] =
      _private_tls_U8TO32_LITTLE(iv + 8) ^ _private_tls_U8TO32_LITTLE(aad + 4);
}

static inline void chacha_encrypt_bytes(chacha_ctx *x, const u8 *m, u8 *c,
                                        u32 bytes) {
  u32 x0, x1, x2, x3, x4, x5, x6, x7;
  u32 x8, x9, x10, x11, x12, x13, x14, x15;
  u32 j0, j1, j2, j3, j4, j5, j6, j7;
  u32 j8, j9, j10, j11, j12, j13, j14, j15;
  u8 *ctarget = NULL;
  u8 tmp[64];
  u_int i;

  if (!bytes) return;

  j0 = x->input[0];
  j1 = x->input[1];
  j2 = x->input[2];
  j3 = x->input[3];
  j4 = x->input[4];
  j5 = x->input[5];
  j6 = x->input[6];
  j7 = x->input[7];
  j8 = x->input[8];
  j9 = x->input[9];
  j10 = x->input[10];
  j11 = x->input[11];
  j12 = x->input[12];
  j13 = x->input[13];
  j14 = x->input[14];
  j15 = x->input[15];

  for (;;) {
    if (bytes < 64) {
      for (i = 0; i < bytes; ++i) tmp[i] = m[i];
      m = tmp;
      ctarget = c;
      c = tmp;
    }
    x0 = j0;
    x1 = j1;
    x2 = j2;
    x3 = j3;
    x4 = j4;
    x5 = j5;
    x6 = j6;
    x7 = j7;
    x8 = j8;
    x9 = j9;
    x10 = j10;
    x11 = j11;
    x12 = j12;
    x13 = j13;
    x14 = j14;
    x15 = j15;
    for (i = 20; i > 0; i -= 2) {
      QUARTERROUND(x0, x4, x8, x12)
      QUARTERROUND(x1, x5, x9, x13)
      QUARTERROUND(x2, x6, x10, x14)
      QUARTERROUND(x3, x7, x11, x15)
      QUARTERROUND(x0, x5, x10, x15)
      QUARTERROUND(x1, x6, x11, x12)
      QUARTERROUND(x2, x7, x8, x13)
      QUARTERROUND(x3, x4, x9, x14)
    }
    x0 = PLUS(x0, j0);
    x1 = PLUS(x1, j1);
    x2 = PLUS(x2, j2);
    x3 = PLUS(x3, j3);
    x4 = PLUS(x4, j4);
    x5 = PLUS(x5, j5);
    x6 = PLUS(x6, j6);
    x7 = PLUS(x7, j7);
    x8 = PLUS(x8, j8);
    x9 = PLUS(x9, j9);
    x10 = PLUS(x10, j10);
    x11 = PLUS(x11, j11);
    x12 = PLUS(x12, j12);
    x13 = PLUS(x13, j13);
    x14 = PLUS(x14, j14);
    x15 = PLUS(x15, j15);

    if (bytes < 64) {
      _private_tls_U32TO8_LITTLE(x->ks + 0, x0);
      _private_tls_U32TO8_LITTLE(x->ks + 4, x1);
      _private_tls_U32TO8_LITTLE(x->ks + 8, x2);
      _private_tls_U32TO8_LITTLE(x->ks + 12, x3);
      _private_tls_U32TO8_LITTLE(x->ks + 16, x4);
      _private_tls_U32TO8_LITTLE(x->ks + 20, x5);
      _private_tls_U32TO8_LITTLE(x->ks + 24, x6);
      _private_tls_U32TO8_LITTLE(x->ks + 28, x7);
      _private_tls_U32TO8_LITTLE(x->ks + 32, x8);
      _private_tls_U32TO8_LITTLE(x->ks + 36, x9);
      _private_tls_U32TO8_LITTLE(x->ks + 40, x10);
      _private_tls_U32TO8_LITTLE(x->ks + 44, x11);
      _private_tls_U32TO8_LITTLE(x->ks + 48, x12);
      _private_tls_U32TO8_LITTLE(x->ks + 52, x13);
      _private_tls_U32TO8_LITTLE(x->ks + 56, x14);
      _private_tls_U32TO8_LITTLE(x->ks + 60, x15);
    }

    x0 = XOR(x0, _private_tls_U8TO32_LITTLE(m + 0));
    x1 = XOR(x1, _private_tls_U8TO32_LITTLE(m + 4));
    x2 = XOR(x2, _private_tls_U8TO32_LITTLE(m + 8));
    x3 = XOR(x3, _private_tls_U8TO32_LITTLE(m + 12));
    x4 = XOR(x4, _private_tls_U8TO32_LITTLE(m + 16));
    x5 = XOR(x5, _private_tls_U8TO32_LITTLE(m + 20));
    x6 = XOR(x6, _private_tls_U8TO32_LITTLE(m + 24));
    x7 = XOR(x7, _private_tls_U8TO32_LITTLE(m + 28));
    x8 = XOR(x8, _private_tls_U8TO32_LITTLE(m + 32));
    x9 = XOR(x9, _private_tls_U8TO32_LITTLE(m + 36));
    x10 = XOR(x10, _private_tls_U8TO32_LITTLE(m + 40));
    x11 = XOR(x11, _private_tls_U8TO32_LITTLE(m + 44));
    x12 = XOR(x12, _private_tls_U8TO32_LITTLE(m + 48));
    x13 = XOR(x13, _private_tls_U8TO32_LITTLE(m + 52));
    x14 = XOR(x14, _private_tls_U8TO32_LITTLE(m + 56));
    x15 = XOR(x15, _private_tls_U8TO32_LITTLE(m + 60));

    j12 = PLUSONE(j12);
    if (!j12) {
      j13 = PLUSONE(j13);
      /*
       * Stopping at 2^70 bytes per nonce is the user's
       * responsibility.
       */
    }

    _private_tls_U32TO8_LITTLE(c + 0, x0);
    _private_tls_U32TO8_LITTLE(c + 4, x1);
    _private_tls_U32TO8_LITTLE(c + 8, x2);
    _private_tls_U32TO8_LITTLE(c + 12, x3);
    _private_tls_U32TO8_LITTLE(c + 16, x4);
    _private_tls_U32TO8_LITTLE(c + 20, x5);
    _private_tls_U32TO8_LITTLE(c + 24, x6);
    _private_tls_U32TO8_LITTLE(c + 28, x7);
    _private_tls_U32TO8_LITTLE(c + 32, x8);
    _private_tls_U32TO8_LITTLE(c + 36, x9);
    _private_tls_U32TO8_LITTLE(c + 40, x10);
    _private_tls_U32TO8_LITTLE(c + 44, x11);
    _private_tls_U32TO8_LITTLE(c + 48, x12);
    _private_tls_U32TO8_LITTLE(c + 52, x13);
    _private_tls_U32TO8_LITTLE(c + 56, x14);
    _private_tls_U32TO8_LITTLE(c + 60, x15);

    if (bytes <= 64) {
      if (bytes < 64) {
        for (i = 0; i < bytes; ++i) ctarget[i] = c[i];
      }
      x->input[12] = j12;
      x->input[13] = j13;
      x->unused = 64 - bytes;
      return;
    }
    bytes -= 64;
    c += 64;
    m += 64;
  }
}

static inline void chacha20_block(chacha_ctx *x, unsigned char *c, u_int len) {
  u_int i;

  unsigned int state[16];
  for (i = 0; i < 16; i++) state[i] = x->input[i];
  for (i = 20; i > 0; i -= 2) {
    QUARTERROUND(state[0], state[4], state[8], state[12])
    QUARTERROUND(state[1], state[5], state[9], state[13])
    QUARTERROUND(state[2], state[6], state[10], state[14])
    QUARTERROUND(state[3], state[7], state[11], state[15])
    QUARTERROUND(state[0], state[5], state[10], state[15])
    QUARTERROUND(state[1], state[6], state[11], state[12])
    QUARTERROUND(state[2], state[7], state[8], state[13])
    QUARTERROUND(state[3], state[4], state[9], state[14])
  }

  for (i = 0; i < 16; i++) x->input[i] = PLUS(x->input[i], state[i]);

  for (i = 0; i < len; i += 4) {
    _private_tls_U32TO8_LITTLE(c + i, x->input[i / 4]);
  }
}

static inline int poly1305_generate_key(unsigned char *key256,
                                        unsigned char *nonce,
                                        unsigned int noncelen,
                                        unsigned char *poly_key,
                                        unsigned int counter) {
  struct chacha_ctx ctx;
  uint64_t ctr;
  memset(&ctx, 0, sizeof(ctx));
  chacha_keysetup(&ctx, key256, 256);
  switch (noncelen) {
    case 8:
      ctr = counter;
      chacha_ivsetup(&ctx, nonce, (unsigned char *)&ctr);
      break;
    case 12:
      chacha_ivsetup_96bitnonce(&ctx, nonce, (unsigned char *)&counter);
      break;
    default:
      return -1;
  }
  chacha20_block(&ctx, poly_key, POLY1305_KEYLEN);
  return 0;
}

/* 17 + sizeof(size_t) + 14*sizeof(unsigned long) */
typedef struct poly1305_state_internal_t {
  unsigned long r[5];
  unsigned long h[5];
  unsigned long pad[4];
  size_t leftover;
  unsigned char buffer[poly1305_block_size];
  unsigned char final;
} poly1305_state_internal_t;

/* interpret four 8 bit unsigned integers as a 32 bit unsigned integer in little
 * endian */
static unsigned long private_tls_U8TO32(const unsigned char *p) {
  return (((unsigned long)(p[0] & 0xff)) | ((unsigned long)(p[1] & 0xff) << 8) |
          ((unsigned long)(p[2] & 0xff) << 16) |
          ((unsigned long)(p[3] & 0xff) << 24));
}

/* store a 32 bit unsigned integer as four 8 bit unsigned integers in little
 * endian */
static void private_tls_U32TO8(unsigned char *p, unsigned long v) {
  p[0] = (v)&0xff;
  p[1] = (v >> 8) & 0xff;
  p[2] = (v >> 16) & 0xff;
  p[3] = (v >> 24) & 0xff;
}

void private_tls_poly1305_init(poly1305_context *ctx,
                               const unsigned char key[32]) {
  poly1305_state_internal_t *st = ctx;

  /* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
  st->r[0] = (private_tls_U8TO32(&key[0])) & 0x3ffffff;
  st->r[1] = (private_tls_U8TO32(&key[3]) >> 2) & 0x3ffff03;
  st->r[2] = (private_tls_U8TO32(&key[6]) >> 4) & 0x3ffc0ff;
  st->r[3] = (private_tls_U8TO32(&key[9]) >> 6) & 0x3f03fff;
  st->r[4] = (private_tls_U8TO32(&key[12]) >> 8) & 0x00fffff;

  /* h = 0 */
  st->h[0] = 0;
  st->h[1] = 0;
  st->h[2] = 0;
  st->h[3] = 0;
  st->h[4] = 0;

  /* save pad for later */
  st->pad[0] = private_tls_U8TO32(&key[16]);
  st->pad[1] = private_tls_U8TO32(&key[20]);
  st->pad[2] = private_tls_U8TO32(&key[24]);
  st->pad[3] = private_tls_U8TO32(&key[28]);

  st->leftover = 0;
  st->final = 0;
}

static void private_tls_poly1305_blocks(poly1305_state_internal_t *st,
                                        const unsigned char *m, size_t bytes) {
  const unsigned long hibit = (st->final) ? 0 : (1UL << 24); /* 1 << 128 */
  unsigned long r0, r1, r2, r3, r4;
  unsigned long s1, s2, s3, s4;
  unsigned long h0, h1, h2, h3, h4;
  unsigned long long d0, d1, d2, d3, d4;
  unsigned long c;

  r0 = st->r[0];
  r1 = st->r[1];
  r2 = st->r[2];
  r3 = st->r[3];
  r4 = st->r[4];

  s1 = r1 * 5;
  s2 = r2 * 5;
  s3 = r3 * 5;
  s4 = r4 * 5;

  h0 = st->h[0];
  h1 = st->h[1];
  h2 = st->h[2];
  h3 = st->h[3];
  h4 = st->h[4];

  while (bytes >= poly1305_block_size) {
    /* h += m[i] */
    h0 += (private_tls_U8TO32(m + 0)) & 0x3ffffff;
    h1 += (private_tls_U8TO32(m + 3) >> 2) & 0x3ffffff;
    h2 += (private_tls_U8TO32(m + 6) >> 4) & 0x3ffffff;
    h3 += (private_tls_U8TO32(m + 9) >> 6) & 0x3ffffff;
    h4 += (private_tls_U8TO32(m + 12) >> 8) | hibit;

    /* h *= r */
    d0 = ((unsigned long long)h0 * r0) + ((unsigned long long)h1 * s4) +
         ((unsigned long long)h2 * s3) + ((unsigned long long)h3 * s2) +
         ((unsigned long long)h4 * s1);
    d1 = ((unsigned long long)h0 * r1) + ((unsigned long long)h1 * r0) +
         ((unsigned long long)h2 * s4) + ((unsigned long long)h3 * s3) +
         ((unsigned long long)h4 * s2);
    d2 = ((unsigned long long)h0 * r2) + ((unsigned long long)h1 * r1) +
         ((unsigned long long)h2 * r0) + ((unsigned long long)h3 * s4) +
         ((unsigned long long)h4 * s3);
    d3 = ((unsigned long long)h0 * r3) + ((unsigned long long)h1 * r2) +
         ((unsigned long long)h2 * r1) + ((unsigned long long)h3 * r0) +
         ((unsigned long long)h4 * s4);
    d4 = ((unsigned long long)h0 * r4) + ((unsigned long long)h1 * r3) +
         ((unsigned long long)h2 * r2) + ((unsigned long long)h3 * r1) +
         ((unsigned long long)h4 * r0);

    /* (partial) h %= p */
    c = (unsigned long)(d0 >> 26);
    h0 = (unsigned long)d0 & 0x3ffffff;
    d1 += c;
    c = (unsigned long)(d1 >> 26);
    h1 = (unsigned long)d1 & 0x3ffffff;
    d2 += c;
    c = (unsigned long)(d2 >> 26);
    h2 = (unsigned long)d2 & 0x3ffffff;
    d3 += c;
    c = (unsigned long)(d3 >> 26);
    h3 = (unsigned long)d3 & 0x3ffffff;
    d4 += c;
    c = (unsigned long)(d4 >> 26);
    h4 = (unsigned long)d4 & 0x3ffffff;
    h0 += c * 5;
    c = (h0 >> 26);
    h0 = h0 & 0x3ffffff;
    h1 += c;

    m += poly1305_block_size;
    bytes -= poly1305_block_size;
  }

  st->h[0] = h0;
  st->h[1] = h1;
  st->h[2] = h2;
  st->h[3] = h3;
  st->h[4] = h4;
}

void private_tls_poly1305_finish(poly1305_context *ctx, unsigned char mac[16]) {
  poly1305_state_internal_t *st = ctx;
  unsigned long h0, h1, h2, h3, h4, c;
  unsigned long g0, g1, g2, g3, g4;
  unsigned long long f;
  unsigned long mask;

  /* process the remaining block */
  if (st->leftover) {
    size_t i = st->leftover;
    st->buffer[i++] = 1;
    for (; i < poly1305_block_size; i++) st->buffer[i] = 0;
    st->final = 1;
    private_tls_poly1305_blocks(st, st->buffer, poly1305_block_size);
  }

  /* fully carry h */
  h0 = st->h[0];
  h1 = st->h[1];
  h2 = st->h[2];
  h3 = st->h[3];
  h4 = st->h[4];

  c = h1 >> 26;
  h1 = h1 & 0x3ffffff;
  h2 += c;
  c = h2 >> 26;
  h2 = h2 & 0x3ffffff;
  h3 += c;
  c = h3 >> 26;
  h3 = h3 & 0x3ffffff;
  h4 += c;
  c = h4 >> 26;
  h4 = h4 & 0x3ffffff;
  h0 += c * 5;
  c = h0 >> 26;
  h0 = h0 & 0x3ffffff;
  h1 += c;

  /* compute h + -p */
  g0 = h0 + 5;
  c = g0 >> 26;
  g0 &= 0x3ffffff;
  g1 = h1 + c;
  c = g1 >> 26;
  g1 &= 0x3ffffff;
  g2 = h2 + c;
  c = g2 >> 26;
  g2 &= 0x3ffffff;
  g3 = h3 + c;
  c = g3 >> 26;
  g3 &= 0x3ffffff;
  g4 = h4 + c - (1UL << 26);

  /* select h if h < p, or h + -p if h >= p */
  mask = (g4 >> ((sizeof(unsigned long) * 8) - 1)) - 1;
  g0 &= mask;
  g1 &= mask;
  g2 &= mask;
  g3 &= mask;
  g4 &= mask;
  mask = ~mask;
  h0 = (h0 & mask) | g0;
  h1 = (h1 & mask) | g1;
  h2 = (h2 & mask) | g2;
  h3 = (h3 & mask) | g3;
  h4 = (h4 & mask) | g4;

  /* h = h % (2^128) */
  h0 = ((h0) | (h1 << 26)) & 0xffffffff;
  h1 = ((h1 >> 6) | (h2 << 20)) & 0xffffffff;
  h2 = ((h2 >> 12) | (h3 << 14)) & 0xffffffff;
  h3 = ((h3 >> 18) | (h4 << 8)) & 0xffffffff;

  /* mac = (h + pad) % (2^128) */
  f = (unsigned long long)h0 + st->pad[0];
  h0 = (unsigned long)f;
  f = (unsigned long long)h1 + st->pad[1] + (f >> 32);
  h1 = (unsigned long)f;
  f = (unsigned long long)h2 + st->pad[2] + (f >> 32);
  h2 = (unsigned long)f;
  f = (unsigned long long)h3 + st->pad[3] + (f >> 32);
  h3 = (unsigned long)f;

  private_tls_U32TO8(mac + 0, h0);
  private_tls_U32TO8(mac + 4, h1);
  private_tls_U32TO8(mac + 8, h2);
  private_tls_U32TO8(mac + 12, h3);

  /* zero out the state */
  st->h[0] = 0;
  st->h[1] = 0;
  st->h[2] = 0;
  st->h[3] = 0;
  st->h[4] = 0;
  st->r[0] = 0;
  st->r[1] = 0;
  st->r[2] = 0;
  st->r[3] = 0;
  st->r[4] = 0;
  st->pad[0] = 0;
  st->pad[1] = 0;
  st->pad[2] = 0;
  st->pad[3] = 0;
}

void private_tls_poly1305_update(poly1305_context *ctx, const unsigned char *m,
                                 size_t bytes) {
  poly1305_state_internal_t *st = ctx;
  size_t i;
  /* handle leftover */
  if (st->leftover) {
    size_t want = (poly1305_block_size - st->leftover);
    if (want > bytes) want = bytes;
    for (i = 0; i < want; i++) st->buffer[st->leftover + i] = m[i];
    bytes -= want;
    m += want;
    st->leftover += want;
    if (st->leftover < poly1305_block_size) return;
    private_tls_poly1305_blocks(st, st->buffer, poly1305_block_size);
    st->leftover = 0;
  }

  /* process full blocks */
  if (bytes >= poly1305_block_size) {
    size_t want = (bytes & ~(poly1305_block_size - 1));
    private_tls_poly1305_blocks(st, m, want);
    m += want;
    bytes -= want;
  }

  /* store leftover */
  if (bytes) {
    for (i = 0; i < bytes; i++) st->buffer[st->leftover + i] = m[i];
    st->leftover += bytes;
  }
}

void chacha20_poly1305_key(struct chacha_ctx *ctx,
                           unsigned char *poly1305_key) {
  unsigned char key[32];
  unsigned char nonce[12];
  chacha_key(ctx, key);
  chacha_nonce(ctx, nonce);
  poly1305_generate_key(key, nonce, sizeof(nonce), poly1305_key, 0);
}

unsigned chacha20_poly1305_aead(struct chacha_ctx *ctx, unsigned char *pt,
                                unsigned int len, unsigned char *aad,
                                unsigned int aad_len, unsigned char *poly_key,
                                unsigned char *out) {
  static unsigned char zeropad[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0};
  if (aad_len > POLY1305_MAX_AAD) return -1;

  unsigned int counter = 1;
  chacha_ivsetup_96bitnonce(ctx, NULL, (unsigned char *)&counter);
  chacha_encrypt_bytes(ctx, pt, out, len);

  poly1305_context aead_ctx;
  private_tls_poly1305_init(&aead_ctx, poly_key);
  private_tls_poly1305_update(&aead_ctx, aad, aad_len);
  unsigned rem = aad_len % 16;
  if (rem) private_tls_poly1305_update(&aead_ctx, zeropad, 16 - rem);
  private_tls_poly1305_update(&aead_ctx, out, len);
  rem = len % 16;
  if (rem) private_tls_poly1305_update(&aead_ctx, zeropad, 16 - rem);

  unsigned char trail[16];
  private_tls_U32TO8(trail, aad_len);
  *(int *)(trail + 4) = 0;
  private_tls_U32TO8(trail + 8, len);
  *(int *)(trail + 12) = 0;

  private_tls_poly1305_update(&aead_ctx, trail, 16);
  private_tls_poly1305_finish(&aead_ctx, out + len);

  return len + POLY1305_TAGLEN;
}

typedef enum { KEA_dhe_rsa, KEA_ec_diffie_hellman } KeyExchangeAlgorithm;

typedef enum {
  rsa_sign = 1,
  //  dss_sign = 2,
  //  rsa_ephemeral_dh_RESERVED = 5,
  //  dss_ephemeral_dh_RESERVED = 6,
  //  ecdsa_sign = 64
} TLSClientCertificateType;

typedef enum {
  //  none = 0,
  md5 = 1,
  sha1 = 2,
  //  sha224 = 3,
  sha256 = 4,
  sha384 = 5,
  sha512 = 6,
  _md5_sha1 = 255
} TLSHashAlgorithm;

typedef enum {
  //  anonymous = 0,
  rsa = 1,
  //  dsa = 2,
  ecdsa = 3
} TLSSignatureAlgorithm;

struct private_OID_chain {
  void *top;
  unsigned char *oid;
};

struct TLSCertificate {
  unsigned short version;
  unsigned int algorithm;
  unsigned int key_algorithm;
  unsigned int ec_algorithm;
  unsigned char *exponent;
  unsigned char *pk;
  unsigned int pk_len;
  unsigned char *priv;
  unsigned int priv_len;
  unsigned char *issuer_country;
  unsigned char *issuer_state;
  unsigned char *issuer_location;
  unsigned char *issuer_entity;
  unsigned char *issuer_subject;
  unsigned char *not_before;
  unsigned char *not_after;
  unsigned char *country;
  unsigned char *state;
  unsigned char *location;
  unsigned char *entity;
  unsigned char *subject;
  unsigned char **san;
  unsigned short san_length;
  unsigned char *ocsp;
  unsigned char *serial_number;
  unsigned int serial_len;
  unsigned char *sign_key;
  unsigned int sign_len;
  unsigned char *fingerprint;
  unsigned char *der_bytes;
  unsigned int der_len;
  unsigned char *bytes;
  unsigned int len;
};

typedef struct {
  union {
    gcm_state aes_gcm_local;
    chacha_ctx chacha_local;
  } ctx_local;
  union {
    gcm_state aes_gcm_remote;
    chacha_ctx chacha_remote;
  } ctx_remote;
  union {
    unsigned char local_mac[TLS_MAX_MAC_SIZE];
    unsigned char local_aead_iv[TLS_AES_GCM_IV_LENGTH];
    unsigned char local_iv[TLS_13_AES_GCM_IV_LENGTH];
    unsigned char local_nonce[TLS_CHACHA20_IV_LENGTH];
  } ctx_local_mac;
  union {
    unsigned char remote_aead_iv[TLS_AES_GCM_IV_LENGTH];
    unsigned char remote_mac[TLS_MAX_MAC_SIZE];
    unsigned char remote_iv[TLS_13_AES_GCM_IV_LENGTH];
    unsigned char remote_nonce[TLS_CHACHA20_IV_LENGTH];
  } ctx_remote_mac;
  unsigned char created;
} TLSCipher;

typedef struct {
  hash_state hash;
  hash_state hash2;
  unsigned char created;
} TLSHash;

#define mp_init(a) ltc_mp.init(a)
#define mp_init_multi ltc_init_multi
#define mp_clear(a) ltc_mp.deinit(a)
#define mp_clear_multi ltc_deinit_multi
#define mp_count_bits(a) ltc_mp.count_bits(a)
#define mp_read_radix(a, b, c) ltc_mp.read_radix(a, b, c)
#define mp_unsigned_bin_size(a) ltc_mp.unsigned_size(a)
#define mp_to_unsigned_bin(a, b) ltc_mp.unsigned_write(a, b)
#define mp_read_unsigned_bin(a, b, c) ltc_mp.unsigned_read(a, b, c)
#define mp_exptmod(a, b, c, d) ltc_mp.exptmod(a, b, c, d)
#define mp_add(a, b, c) ltc_mp.add(a, b, c)
#define mp_mul(a, b, c) ltc_mp.mul(a, b, c)
#define mp_cmp(a, b) ltc_mp.compare(a, b)
#define mp_cmp_d(a, b) ltc_mp.compare_d(a, b)
#define mp_sqr(a, b) ltc_mp.sqr(a, b)
#define mp_mod(a, b, c) ltc_mp.mpdiv(a, b, NULL, c)
#define mp_sub(a, b, c) ltc_mp.sub(a, b, c)
#define mp_set(a, b) ltc_mp.set_int(a, b)

typedef struct {
  int iana;
  void *x;
  void *y;
  void *p;
  void *g;
} DHKey;

static DHKey ffdhe2048 = {
        0x0100,
        NULL,
        NULL,
        (void *)"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B423861285C97FFFFFFFFFFFFFFFF",
        (void *)"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002"
};

static DHKey ffdhe3072 = {
        0x0101,
        NULL,
        NULL,
        (void *)"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B4238611FCFDCDE355B3B6519035BBC34F4DEF99C023861B46FC9D6E6C9077AD91D2691F7F7EE598CB0FAC186D91CAEFE130985139270B4130C93BC437944F4FD4452E2D74DD364F2E21E71F54BFF5CAE82AB9C9DF69EE86D2BC522363A0DABC521979B0DEADA1DBF9A42D5C4484E0ABCD06BFA53DDEF3C1B20EE3FD59D7C25E41D2B66C62E37FFFFFFFFFFFFFFFF",
        (void *)"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002"
};

static DHKey ffdhe4096 = {
        0x0102,
        NULL,
        NULL,
        (void *)"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B4238611FCFDCDE355B3B6519035BBC34F4DEF99C023861B46FC9D6E6C9077AD91D2691F7F7EE598CB0FAC186D91CAEFE130985139270B4130C93BC437944F4FD4452E2D74DD364F2E21E71F54BFF5CAE82AB9C9DF69EE86D2BC522363A0DABC521979B0DEADA1DBF9A42D5C4484E0ABCD06BFA53DDEF3C1B20EE3FD59D7C25E41D2B669E1EF16E6F52C3164DF4FB7930E9E4E58857B6AC7D5F42D69F6D187763CF1D5503400487F55BA57E31CC7A7135C886EFB4318AED6A1E012D9E6832A907600A918130C46DC778F971AD0038092999A333CB8B7A1A1DB93D7140003C2A4ECEA9F98D0ACC0A8291CDCEC97DCF8EC9B55A7F88A46B4DB5A851F44182E1C68A007E5E655F6AFFFFFFFFFFFFFFFF",
        (void *)"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002"
};

static DHKey ffdhe6144 = {
        0x0103,
        NULL,
        NULL,
        (void *)"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B4238611FCFDCDE355B3B6519035BBC34F4DEF99C023861B46FC9D6E6C9077AD91D2691F7F7EE598CB0FAC186D91CAEFE130985139270B4130C93BC437944F4FD4452E2D74DD364F2E21E71F54BFF5CAE82AB9C9DF69EE86D2BC522363A0DABC521979B0DEADA1DBF9A42D5C4484E0ABCD06BFA53DDEF3C1B20EE3FD59D7C25E41D2B669E1EF16E6F52C3164DF4FB7930E9E4E58857B6AC7D5F42D69F6D187763CF1D5503400487F55BA57E31CC7A7135C886EFB4318AED6A1E012D9E6832A907600A918130C46DC778F971AD0038092999A333CB8B7A1A1DB93D7140003C2A4ECEA9F98D0ACC0A8291CDCEC97DCF8EC9B55A7F88A46B4DB5A851F44182E1C68A007E5E0DD9020BFD64B645036C7A4E677D2C38532A3A23BA4442CAF53EA63BB454329B7624C8917BDD64B1C0FD4CB38E8C334C701C3ACDAD0657FCCFEC719B1F5C3E4E46041F388147FB4CFDB477A52471F7A9A96910B855322EDB6340D8A00EF092350511E30ABEC1FFF9E3A26E7FB29F8C183023C3587E38DA0077D9B4763E4E4B94B2BBC194C6651E77CAF992EEAAC0232A281BF6B3A739C1226116820AE8DB5847A67CBEF9C9091B462D538CD72B03746AE77F5E62292C311562A846505DC82DB854338AE49F5235C95B91178CCF2DD5CACEF403EC9D1810C6272B045B3B71F9DC6B80D63FDD4A8E9ADB1E6962A69526D43161C1A41D570D7938DAD4A40E329CD0E40E65FFFFFFFFFFFFFFFF",
        (void *)"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002"
};

static DHKey ffdhe8192 = {
        0x0104,
        NULL,
        NULL,
        (void *)"FFFFFFFFFFFFFFFFADF85458A2BB4A9AAFDC5620273D3CF1D8B9C583CE2D3695A9E13641146433FBCC939DCE249B3EF97D2FE363630C75D8F681B202AEC4617AD3DF1ED5D5FD65612433F51F5F066ED0856365553DED1AF3B557135E7F57C935984F0C70E0E68B77E2A689DAF3EFE8721DF158A136ADE73530ACCA4F483A797ABC0AB182B324FB61D108A94BB2C8E3FBB96ADAB760D7F4681D4F42A3DE394DF4AE56EDE76372BB190B07A7C8EE0A6D709E02FCE1CDF7E2ECC03404CD28342F619172FE9CE98583FF8E4F1232EEF28183C3FE3B1B4C6FAD733BB5FCBC2EC22005C58EF1837D1683B2C6F34A26C1B2EFFA886B4238611FCFDCDE355B3B6519035BBC34F4DEF99C023861B46FC9D6E6C9077AD91D2691F7F7EE598CB0FAC186D91CAEFE130985139270B4130C93BC437944F4FD4452E2D74DD364F2E21E71F54BFF5CAE82AB9C9DF69EE86D2BC522363A0DABC521979B0DEADA1DBF9A42D5C4484E0ABCD06BFA53DDEF3C1B20EE3FD59D7C25E41D2B669E1EF16E6F52C3164DF4FB7930E9E4E58857B6AC7D5F42D69F6D187763CF1D5503400487F55BA57E31CC7A7135C886EFB4318AED6A1E012D9E6832A907600A918130C46DC778F971AD0038092999A333CB8B7A1A1DB93D7140003C2A4ECEA9F98D0ACC0A8291CDCEC97DCF8EC9B55A7F88A46B4DB5A851F44182E1C68A007E5E0DD9020BFD64B645036C7A4E677D2C38532A3A23BA4442CAF53EA63BB454329B7624C8917BDD64B1C0FD4CB38E8C334C701C3ACDAD0657FCCFEC719B1F5C3E4E46041F388147FB4CFDB477A52471F7A9A96910B855322EDB6340D8A00EF092350511E30ABEC1FFF9E3A26E7FB29F8C183023C3587E38DA0077D9B4763E4E4B94B2BBC194C6651E77CAF992EEAAC0232A281BF6B3A739C1226116820AE8DB5847A67CBEF9C9091B462D538CD72B03746AE77F5E62292C311562A846505DC82DB854338AE49F5235C95B91178CCF2DD5CACEF403EC9D1810C6272B045B3B71F9DC6B80D63FDD4A8E9ADB1E6962A69526D43161C1A41D570D7938DAD4A40E329CCFF46AAA36AD004CF600C8381E425A31D951AE64FDB23FCEC9509D43687FEB69EDD1CC5E0B8CC3BDF64B10EF86B63142A3AB8829555B2F747C932665CB2C0F1CC01BD70229388839D2AF05E454504AC78B7582822846C0BA35C35F5C59160CC046FD8251541FC68C9C86B022BB7099876A460E7451A8A93109703FEE1C217E6C3826E52C51AA691E0E423CFC99E9E31650C1217B624816CDAD9A95F9D5B8019488D9C0A0A1FE3075A577E23183F81D4A3F2FA4571EFC8CE0BA8A4FE8B6855DFE72B0A66EDED2FBABFBE58A30FAFABE1C5D71A87E2F741EF8C1FE86FEA6BBFDE530677F0D97D11D49F7A8443D0822E506A9F4614E011E2A94838FF88CD68C8BB7C5C6424CFFFFFFFFFFFFFFFF",
        (void *)"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002"
};

struct ECCCurveParameters {
  int size;
  int iana;
  const char *name;
  const char *P;
  const char *B;
  const char *Gx;
  const char *Gy;
  const char *order;
  ltc_ecc_set_type dp;
};

static struct ECCCurveParameters secp192r1 = {
    24,
    19,
    "secp192r1",
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF",  // P
    "64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1",  // B
    "188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012",  // Gx
    "07192B95FFC8DA78631011ED6B24CDD573F977A11E794811",  // Gy
    "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831"   // order (n)
};

static struct ECCCurveParameters secp224r1 = {
    28,
    21,
    "secp224r1",
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001",  // P
    "B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4",  // B
    "B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21",  // Gx
    "BD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34",  // Gy
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D"   // order (n)
};

static struct ECCCurveParameters secp224k1 = {
    28,
    20,
    "secp224k1",
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFE56D",  // P
    "00000000000000000000000000000000000000000000000000000005",  // B
    "A1455B334DF099DF30FC28A169A467E9E47075A90F7E650EB6B7A45C",  // Gx
    "7E089FED7FBA344282CAFBD6F7E319F7C0B0BD59E2CA4BDB556D61A5",  // Gy
    "0000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7"   // order (n)
};

static struct ECCCurveParameters secp256r1 = {
    32,
    23,
    "secp256r1",
    "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF",  // P
    "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B",  // B
    "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296",  // Gx
    "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5",  // Gy
    "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551"  // order
                                                                        // (n)
};

static struct ECCCurveParameters secp256k1 = {
    32,
    22,
    "secp256k1",
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F",  // P
    "0000000000000000000000000000000000000000000000000000000000000007",  // B
    "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798",  // Gx
    "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8",  // Gy
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"  // order
                                                                        // (n)
};

static struct ECCCurveParameters secp384r1 = {
    48,
    24,
    "secp384r1",
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFF00"
    "00000000000000FFFFFFFF",  // P
    "B3312FA7E23EE7E4988E056BE3F82D19181D9C6EFE8141120314088F5013875AC656398D8A"
    "2ED19D2A85C8EDD3EC2AEF",  // B
    "AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B9859F741E082542A385502F25DBF"
    "55296C3A545E3872760AB7",  // Gx
    "3617DE4A96262C6F5D9E98BF9292DC29F8F41DBD289A147CE9DA3113B5F0B8C00A60B1CE1D"
    "7E819D7A431D7C90EA0E5F",  // Gy
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC7634D81F4372DDF581A0DB248"
    "B0A77AECEC196ACCC52973"  // order (n)
};

static struct ECCCurveParameters secp521r1 = {
    66,
    25,
    "secp521r1",
    "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",  // P

    "0051953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E1561939"
    "51EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00",  // B
    "00C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B5E"
    "77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66",  // Gx
    "011839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97EE72"
    "995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650",  // Gy
    "01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA518687"
    "83BF2F966B7FCC0148F709A5D03BB5C9B8899C47AEBB6FB71E91386409"  // order (n)
};

// dummy
static struct ECCCurveParameters x25519 = {
    32,
    29,
    "x25519",
    "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFED",  // P
    "0000000000000000000000000000000000000000000000000000000000000000",  // B
    "0000000000000000000000000000000000000000000000000000000000000009",  // Gx
    "20AE19A1B8A086B4E01EDD2C7748D14C923D4D7E6D7C61B229E9C5A27ECED3D9",  // Gy
    "1000000000000000000000000000000014DEF9DEA2F79CD65812631A5CF5D3ED"  // order
                                                                        // (n)
};

static struct ECCCurveParameters *const default_curve = &secp384r1;

void init_curve(struct ECCCurveParameters *curve) {
  curve->dp.size = curve->size;
  curve->dp.name = (char *)curve->name;
  curve->dp.B = (char *)curve->B;
  curve->dp.prime = (char *)curve->P;
  curve->dp.Gx = (char *)curve->Gx;
  curve->dp.Gy = (char *)curve->Gy;
  curve->dp.order = (char *)curve->order;
}

void init_curves() {
  init_curve(&secp192r1);
  init_curve(&secp224r1);
  init_curve(&secp224k1);
  init_curve(&secp256r1);
  init_curve(&secp256k1);
  init_curve(&secp384r1);
  init_curve(&secp521r1);
}

struct TLSContext {
  unsigned char remote_random[TLS_CLIENT_RANDOM_SIZE];
  unsigned char local_random[TLS_SERVER_RANDOM_SIZE];
  unsigned char session[TLS_MAX_SESSION_ID];
  unsigned char session_size;
  unsigned short cipher;
  unsigned short version;
  unsigned char is_server;
  struct TLSCertificate **certificates;
  struct TLSCertificate *private_key;
  struct TLSCertificate *ec_private_key;
  DHKey *dhe;
  ecc_key *ecc_dhe;
  char *default_dhe_p;
  char *default_dhe_g;
  const struct ECCCurveParameters *curve;
  unsigned int certificates_count;
  unsigned char *master_key;
  unsigned int master_key_len;
  unsigned char *premaster_key;
  unsigned int premaster_key_len;
  unsigned char cipher_spec_set;
  TLSCipher crypto;
  TLSHash *handshake_hash;

  unsigned char *message_buffer;
  unsigned int message_buffer_len;
  uint64_t remote_sequence_number;
  uint64_t local_sequence_number;

  unsigned char connection_status;
  unsigned char critical_error;
  unsigned char error_code;

  unsigned char *tls_buffer;
  unsigned int tls_buffer_len;

  unsigned char *application_buffer;
  unsigned int application_buffer_len;
  unsigned char is_child;
  unsigned char exportable;
  unsigned char *exportable_keys;
  unsigned char exportable_size;
  char *sni;
  unsigned char dtls;
  unsigned short dtls_epoch_local;
  unsigned short dtls_epoch_remote;
  unsigned char *dtls_cookie;
  unsigned char dtls_cookie_len;
  unsigned char dtls_seq;
  unsigned char *cached_handshake;
  unsigned int cached_handshake_len;
  unsigned char client_verified;
  // handshake messages flags
  unsigned char hs_messages[11];
  struct TLSCertificate **root_certificates;
  unsigned int root_count;
  unsigned char *finished_key;
  unsigned char *remote_finished_key;
  unsigned char *server_finished_hash;
  unsigned char *client_secret;
  char **alpn;
  unsigned char alpn_count;
  char *negotiated_alpn;
  unsigned int sleep_until;
  unsigned short tls13_version;
  unsigned char false_start;
};

struct TLSPacket {
  unsigned char *buf;
  unsigned int len;
  unsigned int size;
  unsigned char broken;
  struct TLSContext *context;
};

static const unsigned int version_id[] = {1, 1, 1, 0};
static const unsigned int pk_id[] = {1, 1, 7, 0};
static const unsigned int serial_id[] = {1, 1, 2, 1, 0};
static const unsigned int issurer_id[] = {1, 1, 4, 0};
static const unsigned int owner_id[] = {1, 1, 6, 0};
static const unsigned int validity_id[] = {1, 1, 5, 0};
static const unsigned int algorithm_id[] = {1, 1, 3, 0};
static const unsigned int sign_id[] = {1, 3, 2, 1, 0};
static const unsigned int priv_id[] = {1, 4, 0};
static const unsigned int priv_der_id[] = {1, 3, 1, 0};
static const unsigned int ecc_priv_id[] = {1, 2, 0};

static const unsigned char country_oid[] = {0x55, 0x04, 0x06, 0x00};
static const unsigned char state_oid[] = {0x55, 0x04, 0x08, 0x00};
static const unsigned char location_oid[] = {0x55, 0x04, 0x07, 0x00};
static const unsigned char entity_oid[] = {0x55, 0x04, 0x0A, 0x00};
static const unsigned char subject_oid[] = {0x55, 0x04, 0x03, 0x00};
static const unsigned char san_oid[] = {0x55, 0x1D, 0x11, 0x00};
static const unsigned char ocsp_oid[] = {0x2B, 0x06, 0x01, 0x05, 0x05,
                                         0x07, 0x30, 0x01, 0x00};

static const unsigned char TLS_RSA_SIGN_RSA_OID[] = {
    0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x00};
static const unsigned char TLS_RSA_SIGN_MD5_OID[] = {
    0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x04, 0x00};
static const unsigned char TLS_RSA_SIGN_SHA1_OID[] = {
    0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05, 0x00};
static const unsigned char TLS_RSA_SIGN_SHA256_OID[] = {
    0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x00};
static const unsigned char TLS_RSA_SIGN_SHA384_OID[] = {
    0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0C, 0x00};
static const unsigned char TLS_RSA_SIGN_SHA512_OID[] = {
    0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0D, 0x00};
//
// static const unsigned char TLS_ECDSA_SIGN_SHA1_OID[] = {
//    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x01, 0x05, 0x00, 0x00};
// static const unsigned char TLS_ECDSA_SIGN_SHA224_OID[] = {
//    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x01, 0x05, 0x00, 0x00};
//
// 1.2.840.10045.4.3.2
// static const unsigned char TLS_ECDSA_SIGN_SHA256_OID[] = {
//    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x05, 0x00, 0x00};
//
// static const unsigned char TLS_ECDSA_SIGN_SHA384_OID[] = {
//    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x03, 0x05, 0x00, 0x00};
// static const unsigned char TLS_ECDSA_SIGN_SHA512_OID[] = {
//    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x04, 0x05, 0x00, 0x00};

static const unsigned char TLS_EC_PUBLIC_KEY_OID[] = {0x2A, 0x86, 0x48, 0xCE,
                                                      0x3D, 0x02, 0x01, 0x00};

static const unsigned char TLS_EC_prime192v1_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x01, 0x00};
static const unsigned char TLS_EC_prime192v2_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x02, 0x00};
static const unsigned char TLS_EC_prime192v3_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x03, 0x00};
static const unsigned char TLS_EC_prime239v1_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x04, 0x00};
static const unsigned char TLS_EC_prime239v2_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x05, 0x00};
static const unsigned char TLS_EC_prime239v3_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x06, 0x00};
static const unsigned char TLS_EC_prime256v1_OID[] = {
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x00};

static const unsigned char TLS_EC_secp224r1_OID[] = {0x2B, 0x81, 0x04,
                                                     0x00, 0x21, 0x00};
static const unsigned char TLS_EC_secp384r1_OID[] = {0x2B, 0x81, 0x04,
                                                     0x00, 0x22, 0x00};
static const unsigned char TLS_EC_secp521r1_OID[] = {0x2B, 0x81, 0x04,
                                                     0x00, 0x23, 0x00};

struct TLSCertificate *asn1_parse(struct TLSContext *context,
                                  const unsigned char *buffer,
                                  unsigned int size, int client_cert);
int _private_tls_update_hash(struct TLSContext *context,
                             const unsigned char *in, unsigned int len);
struct TLSPacket *tls_build_finished(struct TLSContext *context);
unsigned int _private_tls_hmac_message(unsigned char local,
                                       struct TLSContext *context,
                                       const unsigned char *buf, int buf_len,
                                       const unsigned char *buf2, int buf_len2,
                                       unsigned char *out, unsigned int outlen,
                                       uint64_t remote_sequence_number);
int tls_random(unsigned char *key, unsigned long len);
void tls_destroy_packet(struct TLSPacket *packet);
struct TLSPacket *tls_build_hello(struct TLSContext *context,
                                  int tls13_downgrade);
struct TLSPacket *tls_build_certificate(struct TLSContext *context);
struct TLSPacket *tls_build_done(struct TLSContext *context);
struct TLSPacket *tls_build_alert(struct TLSContext *context, char critical,
                                  unsigned char code);
struct TLSPacket *tls_build_change_cipher_spec(struct TLSContext *context);
struct TLSPacket *tls_build_verify_request(struct TLSContext *context);
int _private_tls_crypto_create(struct TLSContext *context, int key_length,
                               unsigned char *localkey, unsigned char *localiv,
                               unsigned char *remotekey,
                               unsigned char *remoteiv);
int _private_tls_get_hash(struct TLSContext *context, unsigned char *hout);
int _private_tls_done_hash(struct TLSContext *context, unsigned char *hout);
int _private_tls_get_hash_idx(struct TLSContext *context);
int _private_tls_build_random(struct TLSPacket *packet);
unsigned int _private_tls_mac_length(struct TLSContext *context);
void _private_dtls_handshake_data(struct TLSContext *context,
                                  struct TLSPacket *packet,
                                  unsigned int dataframe);
void _private_tls_dhe_free(struct TLSContext *context);
void _private_tls_ecc_dhe_free(struct TLSContext *context);
void _private_tls_dh_clear_key(DHKey *key);

struct TLSPacket *tls_build_encrypted_extensions(struct TLSContext *context);
struct TLSPacket *tls_build_certificate_verify(struct TLSContext *context);

// dtls base secret
static unsigned char dtls_secret[32];

static unsigned char dependecies_loaded = 0;
// not supported
// static unsigned char TLS_DSA_SIGN_SHA1_OID[] = {0x2A, 0x86, 0x52, 0xCE, 0x38,
// 0x04, 0x03, 0x00};

// base64 stuff
static const char cd64[] =
    "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`"
    "abcdefghijklmnopq";

void _private_b64_decodeblock(unsigned char in[4], unsigned char out[3]) {
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = (unsigned char)(((in[2] << 6) & 0xc0) | in[3]);
}

int _private_b64_decode(const char *in_buffer, int in_buffer_size,
                        unsigned char *out_buffer) {
  unsigned char in[4], out[3], v;
  int i, len;

  const char *ptr = in_buffer;
  char *out_ptr = (char *)out_buffer;

  while (ptr <= in_buffer + in_buffer_size) {
    for (len = 0, i = 0; i < 4 && (ptr <= in_buffer + in_buffer_size); i++) {
      v = 0;
      while ((ptr <= in_buffer + in_buffer_size) && v == 0) {
        v = (unsigned char)ptr[0];
        ptr++;
        v = (unsigned char)((v < 43 || v > 122) ? 0 : cd64[v - 43]);
        if (v) v = (unsigned char)((v == '$') ? 0 : v - 61);
      }
      if (ptr <= in_buffer + in_buffer_size) {
        len++;
        if (v) in[i] = (unsigned char)(v - 1);
      } else {
        in[i] = 0;
      }
    }
    if (len) {
      _private_b64_decodeblock(in, out);
      for (i = 0; i < len - 1; i++) {
        out_ptr[0] = out[i];
        out_ptr++;
      }
    }
  }
  return (int)((intptr_t)out_ptr - (intptr_t)out_buffer);
}

void dtls_reset_cookie_secret() {
  tls_random(dtls_secret, sizeof(dtls_secret));
}

void tls_init() {
  if (dependecies_loaded) return;
  DEBUG_PRINT("Initializing dependencies\n");
  dependecies_loaded = 1;
  ltc_mp = ltm_desc;
  register_prng(&sprng_desc);
  register_hash(&sha256_desc);
  register_hash(&sha1_desc);
  register_hash(&sha384_desc);
  register_hash(&sha512_desc);
  register_hash(&md5_desc);
  register_cipher(&aes_desc);
  init_curves();
  dtls_reset_cookie_secret();
}

int _private_tls_dh_shared_secret(DHKey *private_key, DHKey *public_key,
                                  unsigned char *out, unsigned long *outlen) {
  void *tmp;
  unsigned long x;
  int err;

  if ((!private_key) || (!public_key) || (!out) || (!outlen))
    return TLS_GENERIC_ERROR;

  /* compute y^x mod p */
  if ((err = mp_init(&tmp)) != CRYPT_OK) return err;

  if ((err = mp_exptmod(public_key->y, private_key->x, private_key->p, tmp)) !=
      CRYPT_OK) {
    mp_clear(tmp);
    return err;
  }

  x = mp_unsigned_bin_size(tmp);
  if (*outlen < x) {
    err = CRYPT_BUFFER_OVERFLOW;
    mp_clear(tmp);
    return err;
  }

  if ((err = mp_to_unsigned_bin(tmp, out)) != CRYPT_OK) {
    mp_clear(tmp);
    return err;
  }
  *outlen = x;
  mp_clear(tmp);
  return 0;
}

unsigned char *_private_tls_decrypt_dhe(struct TLSContext *context,
                                        const unsigned char *buffer,
                                        unsigned int len, unsigned int *size,
                                        int clear_key) {
  *size = 0;
  if ((!len) || (!context) || (!context->dhe)) {
    DEBUG_PRINT("DECRYPT DHE: No private DHE key set\n");
    return NULL;
  }

  if (!buffer) {
    DEBUG_PRINT("DECRYPT DHE: FATAL: Buffer is NULL\n");
    return NULL;
  }

  unsigned long out_size = len;
  void *Yc = NULL;

  if (mp_init(&Yc)) {
    DEBUG_PRINT("DECRYPT DHE: ERROR CREATING Yc\n");
    return NULL;
  }

  DEBUG_PRINT("DECRYPT DHE: MPI INITED\n");

  if (mp_read_unsigned_bin(Yc, (unsigned char *)buffer, len)) {
    DEBUG_PRINT("DECRYPT DHE: ERROR LOADING DHE Yc\n");
    mp_clear(Yc);
    return NULL;
  }

  DEBUG_PRINT("DECRYPT DHE: DHE LOAD SUCCESS\n");

  unsigned char *out = (unsigned char *)TLS_MALLOC(len);
  DHKey client_key;
  memset(&client_key, 0, sizeof(DHKey));

  client_key.p = context->dhe->p;
  client_key.g = context->dhe->g;
  client_key.y = Yc;
  int err =
      _private_tls_dh_shared_secret(context->dhe, &client_key, out, &out_size);
  // don't delete p and g
  client_key.p = NULL;
  client_key.g = NULL;
  _private_tls_dh_clear_key(&client_key);
  // not needing the dhe key anymore
  if (clear_key) _private_tls_dhe_free(context);
  if (err) {
    DEBUG_PRINT("DECRYPT DHE: DHE DECRYPT ERROR %i\n", err);
    TLS_FREE(out);
    return NULL;
  }
  DEBUG_PRINT("DECRYPT DHE: OUT_SIZE: %lu\n", out_size);
  DEBUG_DUMP_HEX_LABEL("DHE", out, out_size);
  *size = (unsigned int)out_size;
  return out;
}

unsigned char *_private_tls_decrypt_ecc_dhe(struct TLSContext *context,
                                            const unsigned char *buffer,
                                            unsigned int len,
                                            unsigned int *size, int clear_key) {
  *size = 0;
  if ((!len) || (!context) || (!context->ecc_dhe)) {
    DEBUG_PRINT("DECRYPT DHE: No private ECC DHE key set\n");
    return NULL;
  }

  const struct ECCCurveParameters *curve;
  if (context->curve)
    curve = context->curve;
  else
    curve = default_curve;

  const ltc_ecc_set_type *dp = &curve->dp;

  ecc_key client_key;
  memset(&client_key, 0, sizeof(client_key));
  if (ecc_ansi_x963_import_ex(buffer, len, &client_key, dp)) {
    DEBUG_PRINT("DECRYPT DHE: Error importing ECC DHE key\n");
    return NULL;
  }
  unsigned char *out = (unsigned char *)TLS_MALLOC(len);
  unsigned long out_size = len;

  int err = ecc_shared_secret(context->ecc_dhe, &client_key, out, &out_size);
  ecc_free(&client_key);
  if (clear_key) _private_tls_ecc_dhe_free(context);
  if (err) {
    DEBUG_PRINT("DECRYPT DHE: ECC DHE DECRYPT ERROR %i\n", err);
    TLS_FREE(out);
    return NULL;
  }
  DEBUG_PRINT("DECRYPT DHE: OUT_SIZE: %lu\n", out_size);
  DEBUG_DUMP_HEX_LABEL("ECC DHE", out, out_size);
  *size = (unsigned int)out_size;
  return out;
}

unsigned char *_private_tls_decrypt_rsa(struct TLSContext *context,
                                        const unsigned char *buffer,
                                        unsigned int len, unsigned int *size) {
  *size = 0;
  if ((!len) || (!context) || (!context->private_key) ||
      (!context->private_key->der_bytes) || (!context->private_key->der_len)) {
    DEBUG_PRINT("No private key set\n");
    return NULL;
  }
  tls_init();
  rsa_key key;
  int err;
  err = rsa_import(context->private_key->der_bytes,
                   context->private_key->der_len, &key);

  if (err) {
    DEBUG_PRINT("Error importing RSA key (code: %i)\n", err);
    return NULL;
  }
  unsigned char *out = (unsigned char *)TLS_MALLOC(len);
  unsigned long out_size = len;
  int res = 0;

  err = rsa_decrypt_key_ex(buffer, len, out, &out_size, NULL, 0, -1,
                           LTC_PKCS_1_V1_5, &res, &key);
  rsa_free(&key);

  if ((err) || (out_size != 48) ||
      (ntohs(*(unsigned short *)out) != context->version)) {
    // generate a random secret and continue (ROBOT fix)
    // silently ignore and generate a random secret
    out_size = 48;
    tls_random(out, out_size);
    *(unsigned short *)out = htons(context->version);
  }
  *size = (unsigned int)out_size;
  return out;
}

unsigned char *_private_tls_encrypt_rsa(struct TLSContext *context,
                                        const unsigned char *buffer,
                                        unsigned int len, unsigned int *size) {
  *size = 0;
  if ((!len) || (!context) || (!context->certificates) ||
      (!context->certificates_count) || (!context->certificates[0]) ||
      (!context->certificates[0]->der_bytes) ||
      (!context->certificates[0]->der_len)) {
    DEBUG_PRINT("No certificate set\n");
    return NULL;
  }
  tls_init();
  rsa_key key;
  int err;
  err = rsa_import(context->certificates[0]->der_bytes,
                   context->certificates[0]->der_len, &key);

  if (err) {
    DEBUG_PRINT("Error importing RSA certificate (code: %i)\n", err);
    return NULL;
  }
  unsigned long out_size = TLS_MAX_RSA_KEY;
  unsigned char *out = (unsigned char *)TLS_MALLOC(out_size);
  int hash_idx = find_hash("sha256");
  int prng_idx = find_prng("sprng");
  err = rsa_encrypt_key_ex(buffer, len, out, &out_size,
                           (unsigned char *)"Concept", 7, NULL, prng_idx,
                           hash_idx, LTC_PKCS_1_V1_5, &key);
  rsa_free(&key);
  if ((err) || (!out_size)) {
    TLS_FREE(out);
    return NULL;
  }
  *size = (unsigned int)out_size;
  return out;
}

int _private_rsa_verify_hash_md5sha1(const unsigned char *sig,
                                     unsigned long siglen, unsigned char *hash,
                                     unsigned long hashlen, int *stat,
                                     rsa_key *key) {
  unsigned long modulus_bitlen, modulus_bytelen, x;
  int err;
  unsigned char *tmpbuf = NULL;

  if ((hash == NULL) || (sig == NULL) || (stat == NULL) || (key == NULL) ||
      (!siglen) || (!hashlen))
    return TLS_GENERIC_ERROR;

  *stat = 0;

  modulus_bitlen = mp_count_bits((key->N));

  modulus_bytelen = mp_unsigned_bin_size((key->N));
  if (modulus_bytelen != siglen) return TLS_GENERIC_ERROR;

  tmpbuf = (unsigned char *)TLS_MALLOC(siglen);
  if (!tmpbuf) return TLS_GENERIC_ERROR;

  x = siglen;
  if ((err = ltc_mp.rsa_me(sig, siglen, tmpbuf, &x, PK_PUBLIC, key)) !=
      CRYPT_OK) {
    TLS_FREE(tmpbuf);
    return err;
  }

  if (x != siglen) {
    TLS_FREE(tmpbuf);
    return CRYPT_INVALID_PACKET;
  }
  unsigned long out_len = siglen;
  unsigned char *out = (unsigned char *)TLS_MALLOC(siglen);
  if (!out) {
    TLS_FREE(tmpbuf);
    return TLS_GENERIC_ERROR;
  }

  int decoded = 0;
  err = pkcs_1_v1_5_decode(tmpbuf, x, LTC_PKCS_1_EMSA, modulus_bitlen, out,
                           &out_len, &decoded);
  if (decoded) {
    if (out_len == hashlen) {
      if (!memcmp(out, hash, hashlen)) *stat = 1;
    }
  }

  TLS_FREE(tmpbuf);
  TLS_FREE(out);
  return err;
}

int _private_tls_verify_rsa(struct TLSContext *context, unsigned int hash_type,
                            const unsigned char *buffer, unsigned int len,
                            const unsigned char *message,
                            unsigned int message_len) {
  tls_init();
  rsa_key key;
  int err;

  if (context->is_server) {
    DEBUG_PRINT("No client certificate support\n");
    return TLS_GENERIC_ERROR;
  } else {
    if ((!len) || (!context->certificates) || (!context->certificates_count) ||
        (!context->certificates[0]) || (!context->certificates[0]->der_bytes) ||
        (!context->certificates[0]->der_len)) {
      DEBUG_PRINT("No server certificate set\n");
      return TLS_GENERIC_ERROR;
    }
    err = rsa_import(context->certificates[0]->der_bytes,
                     context->certificates[0]->der_len, &key);
  }
  if (err) {
    DEBUG_PRINT("Error importing RSA certificate (code: %i)\n", err);
    return TLS_GENERIC_ERROR;
  }
  int hash_idx = -1;
  unsigned char hash[TLS_MAX_HASH_LEN];
  unsigned int hash_len = 0;
  hash_state state;
  switch (hash_type) {
    case md5:
      hash_idx = find_hash("md5");
      err = md5_init(&state);
      TLS_ERROR(err, break);
      err = md5_process(&state, message, message_len);
      TLS_ERROR(err, break);
      err = md5_done(&state, hash);
      TLS_ERROR(err, break);
      hash_len = 16;
      break;
    case sha1:
      hash_idx = find_hash("sha1");
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 20;
      break;
    case sha256:
      hash_idx = find_hash("sha256");
      err = sha256_init(&state);
      TLS_ERROR(err, break)
      err = sha256_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha256_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 32;
      break;
    case sha384:
      hash_idx = find_hash("sha384");
      err = sha384_init(&state);
      TLS_ERROR(err, break)
      err = sha384_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha384_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 48;
      break;
    case sha512:
      hash_idx = find_hash("sha512");
      err = sha512_init(&state);
      TLS_ERROR(err, break)
      err = sha512_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha512_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 64;
      break;
    case _md5_sha1:
      hash_idx = find_hash("md5");
      err = md5_init(&state);
      TLS_ERROR(err, break)
      err = md5_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = md5_done(&state, hash);
      TLS_ERROR(err, break)
      hash_idx = find_hash("sha1");
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash + 16);
      TLS_ERROR(err, break)
      hash_len = 36;
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash + 16);
      TLS_ERROR(err, break)
      hash_len = 36;
      break;
  }
  if ((hash_idx < 0) || (err)) {
    DEBUG_PRINT("Unsupported hash type: %i\n", hash_type);
    return TLS_GENERIC_ERROR;
  }
  int rsa_stat = 0;
  if (hash_type == _md5_sha1)
    err = _private_rsa_verify_hash_md5sha1(buffer, len, hash, hash_len,
                                           &rsa_stat, &key);
  else if ((context->version == TLS_V13) || (context->version == DTLS_V13))
    err = rsa_verify_hash_ex(buffer, len, hash, hash_len, LTC_PKCS_1_PSS,
                             hash_idx, 0, &rsa_stat, &key);
  else
    err = rsa_verify_hash_ex(buffer, len, hash, hash_len, LTC_PKCS_1_V1_5,
                             hash_idx, 0, &rsa_stat, &key);
  rsa_free(&key);
  if (err) return 0;
  return rsa_stat;
}

int _private_rsa_sign_hash_md5sha1(const unsigned char *in, unsigned long inlen,
                                   unsigned char *out, unsigned long *outlen,
                                   rsa_key *key) {
  unsigned long modulus_bitlen, modulus_bytelen, x;
  int err;

  if ((in == NULL) || (out == NULL) || (outlen == NULL) || (key == NULL))
    return TLS_GENERIC_ERROR;

  modulus_bitlen = mp_count_bits((key->N));

  modulus_bytelen = mp_unsigned_bin_size((key->N));
  if (modulus_bytelen > *outlen) {
    *outlen = modulus_bytelen;
    return CRYPT_BUFFER_OVERFLOW;
  }
  x = modulus_bytelen;
  err = pkcs_1_v1_5_encode(in, inlen, LTC_PKCS_1_EMSA, modulus_bitlen, NULL, 0,
                           out, &x);
  if (err != CRYPT_OK) return err;

  return ltc_mp.rsa_me(out, x, out, outlen, PK_PRIVATE, key);
}

int _private_tls_sign_rsa(struct TLSContext *context, unsigned int hash_type,
                          const unsigned char *message,
                          unsigned int message_len, unsigned char *out,
                          unsigned long *outlen) {
  if ((!outlen) || (!context) || (!out) || (!outlen) ||
      (!context->private_key) || (!context->private_key->der_bytes) ||
      (!context->private_key->der_len)) {
    DEBUG_PRINT("No private key set\n");
    return TLS_GENERIC_ERROR;
  }
  tls_init();
  rsa_key key;
  int err;
  err = rsa_import(context->private_key->der_bytes,
                   context->private_key->der_len, &key);

  if (err) {
    DEBUG_PRINT("Error importing RSA certificate (code: %i)\n", err);
    return TLS_GENERIC_ERROR;
  }
  int hash_idx = -1;
  unsigned char hash[TLS_MAX_HASH_LEN];
  unsigned int hash_len = 0;
  hash_state state;
  switch (hash_type) {
    case md5:
      hash_idx = find_hash("md5");
      err = md5_init(&state);
      TLS_ERROR(err, break)
      err = md5_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = md5_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 16;
      break;
    case sha1:
      hash_idx = find_hash("sha1");
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 20;
      break;
    case sha256:
      hash_idx = find_hash("sha256");
      err = sha256_init(&state);
      TLS_ERROR(err, break)
      err = sha256_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha256_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 32;
      break;
    case sha384:
      hash_idx = find_hash("sha384");
      err = sha384_init(&state);
      TLS_ERROR(err, break)
      err = sha384_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha384_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 48;
      break;
    case sha512:
      hash_idx = find_hash("sha512");
      err = sha512_init(&state);
      TLS_ERROR(err, break)
      err = sha512_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha512_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 64;
      break;
    case _md5_sha1:
      hash_idx = find_hash("md5");
      err = md5_init(&state);
      TLS_ERROR(err, break)
      err = md5_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = md5_done(&state, hash);
      TLS_ERROR(err, break)
      hash_idx = find_hash("sha1");
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash + 16);
      TLS_ERROR(err, break)
      hash_len = 36;
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash + 16);
      TLS_ERROR(err, break)
      hash_len = 36;
      break;
  }
  if (hash_type == _md5_sha1) {
    if (err) {
      DEBUG_PRINT("Unsupported hash type: %i\n", hash_type);
      return TLS_GENERIC_ERROR;
    }
    err = _private_rsa_sign_hash_md5sha1(hash, hash_len, out, outlen, &key);
  } else {
    if ((hash_idx < 0) || (err)) {
      DEBUG_PRINT("Unsupported hash type: %i\n", hash_type);
      return TLS_GENERIC_ERROR;
    }
    if ((context->version == TLS_V13) || (context->version == DTLS_V13))
      err = rsa_sign_hash_ex(hash, hash_len, out, outlen, LTC_PKCS_1_PSS, NULL,
                             find_prng("sprng"), hash_idx,
                             hash_type == sha256 ? 32 : 48, &key);
    else
      err = rsa_sign_hash_ex(hash, hash_len, out, outlen, LTC_PKCS_1_V1_5, NULL,
                             find_prng("sprng"), hash_idx, 0, &key);
  }
  rsa_free(&key);
  if (err) return 0;

  return 1;
}

static int _private_tls_is_point(ecc_key *key) {
  void *prime, *b, *t1, *t2;
  int err;

  if ((err = mp_init_multi(&prime, &b, &t1, &t2, NULL)) != CRYPT_OK) {
    return err;
  }

  /* load prime and b */
  if ((err = mp_read_radix(prime, key->dp->prime, 16)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_read_radix(b, key->dp->B, 16)) != CRYPT_OK) {
    goto error;
  }

  /* compute y^2 */
  if ((err = mp_sqr(key->pubkey.y, t1)) != CRYPT_OK) {
    goto error;
  }

  /* compute x^3 */
  if ((err = mp_sqr(key->pubkey.x, t2)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_mod(t2, prime, t2)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_mul(key->pubkey.x, t2, t2)) != CRYPT_OK) {
    goto error;
  }

  /* compute y^2 - x^3 */
  if ((err = mp_sub(t1, t2, t1)) != CRYPT_OK) {
    goto error;
  }

  /* compute y^2 - x^3 + 3x */
  if ((err = mp_add(t1, key->pubkey.x, t1)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_add(t1, key->pubkey.x, t1)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_add(t1, key->pubkey.x, t1)) != CRYPT_OK) {
    goto error;
  }
  if ((err = mp_mod(t1, prime, t1)) != CRYPT_OK) {
    goto error;
  }
  while (mp_cmp_d(t1, 0) == LTC_MP_LT) {
    if ((err = mp_add(t1, prime, t1)) != CRYPT_OK) {
      goto error;
    }
  }
  while (mp_cmp(t1, prime) != LTC_MP_LT) {
    if ((err = mp_sub(t1, prime, t1)) != CRYPT_OK) {
      goto error;
    }
  }

  /* compare to b */
  if (mp_cmp(t1, b) != LTC_MP_EQ) {
    err = CRYPT_INVALID_PACKET;
  } else {
    err = CRYPT_OK;
  }

error:
  mp_clear_multi(prime, b, t1, t2, NULL);
  return err;
}

int _private_tls_ecc_import_key(const unsigned char *private_key,
                                int private_len,
                                const unsigned char *public_key, int public_len,
                                ecc_key *key, const ltc_ecc_set_type *dp) {
  int err;

  if ((!key) || (!ltc_mp.name)) return CRYPT_MEM;

  key->type = PK_PRIVATE;

  if (mp_init_multi(&key->pubkey.x, &key->pubkey.y, &key->pubkey.z, &key->k,
                    NULL) != CRYPT_OK)
    return CRYPT_MEM;

  if ((public_len) && (!public_key[0])) {
    public_key++;
    public_len--;
  }
  if ((err =
           mp_read_unsigned_bin(key->pubkey.x, (unsigned char *)public_key + 1,
                                (public_len - 1) >> 1)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  if ((err = mp_read_unsigned_bin(
           key->pubkey.y,
           (unsigned char *)public_key + 1 + ((public_len - 1) >> 1),
           (public_len - 1) >> 1)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  if ((err = mp_read_unsigned_bin(key->k, (unsigned char *)private_key,
                                  private_len)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  key->idx = -1;
  key->dp = dp;

  /* set z */
  if ((err = mp_set(key->pubkey.z, 1)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  /* is it a point on the curve?  */
  if ((err = _private_tls_is_point(key)) != CRYPT_OK) {
    DEBUG_PRINT("KEY IS NOT ON CURVE\n");
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  /* we're good */
  return CRYPT_OK;
}

int _private_tls_sign_ecdsa(struct TLSContext *context, unsigned int hash_type,
                            const unsigned char *message,
                            unsigned int message_len, unsigned char *out,
                            unsigned long *outlen) {
  if ((!outlen) || (!context) || (!out) || (!outlen) ||
      (!context->ec_private_key) || (!context->ec_private_key->priv) ||
      (!context->ec_private_key->priv_len) || (!context->ec_private_key->pk) ||
      (!context->ec_private_key->pk_len)) {
    DEBUG_PRINT("No private ECDSA key set\n");
    return TLS_GENERIC_ERROR;
  }

  const struct ECCCurveParameters *curve = NULL;

  switch (context->ec_private_key->ec_algorithm) {
    case 19:
      curve = &secp192r1;
      break;
    case 20:
      curve = &secp224k1;
      break;
    case 21:
      curve = &secp224r1;
      break;
    case 22:
      curve = &secp256k1;
      break;
    case 23:
      curve = &secp256r1;
      break;
    case 24:
      curve = &secp384r1;
      break;
    case 25:
      curve = &secp521r1;
      break;
    default:
      DEBUG_PRINT("UNSUPPORTED CURVE\n");
  }

  if (!curve) return TLS_GENERIC_ERROR;

  tls_init();
  ecc_key key;
  int err;

  const ltc_ecc_set_type *dp = &curve->dp;

  // broken ... fix this
  err = _private_tls_ecc_import_key(
      context->ec_private_key->priv, context->ec_private_key->priv_len,
      context->ec_private_key->pk, context->ec_private_key->pk_len, &key, dp);
  if (err) {
    DEBUG_PRINT("Error importing ECC certificate (code: %i)\n", (int)err);
    return TLS_GENERIC_ERROR;
  }
  unsigned char hash[TLS_MAX_HASH_LEN];
  unsigned int hash_len = 0;
  hash_state state;
  switch (hash_type) {
    case md5:
      err = md5_init(&state);
      TLS_ERROR(err, break)
      err = md5_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = md5_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 16;
      break;
    case sha1:
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 20;
      break;
    case sha256:
      err = sha256_init(&state);
      TLS_ERROR(err, break)
      err = sha256_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha256_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 32;
      break;
    case sha384:
      err = sha384_init(&state);
      TLS_ERROR(err, break)
      err = sha384_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha384_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 48;
      break;
    case sha512:
      err = sha512_init(&state);
      TLS_ERROR(err, break)
      err = sha512_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha512_done(&state, hash);
      TLS_ERROR(err, break)
      hash_len = 64;
      break;
    case _md5_sha1:
      err = md5_init(&state);
      TLS_ERROR(err, break)
      err = md5_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = md5_done(&state, hash);
      TLS_ERROR(err, break)
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash + 16);
      TLS_ERROR(err, break)
      hash_len = 36;
      err = sha1_init(&state);
      TLS_ERROR(err, break)
      err = sha1_process(&state, message, message_len);
      TLS_ERROR(err, break)
      err = sha1_done(&state, hash + 16);
      TLS_ERROR(err, break)
      hash_len = 36;
      break;
  }

  if (err) {
    DEBUG_PRINT("Unsupported hash type: %i\n", hash_type);
    return TLS_GENERIC_ERROR;
  }
  // "Let z be the Ln leftmost bits of e, where Ln is the bit length of the
  // group order n."
  if (hash_len > (unsigned int)curve->size)
    hash_len = (unsigned int)curve->size;
  err = ecc_sign_hash(hash, hash_len, out, outlen, NULL, find_prng("sprng"),
                      &key);
  DEBUG_DUMP_HEX_LABEL("ECC SIGNATURE", out, *outlen);
  ecc_free(&key);
  if (err) return 0;

  return 1;
}

int _private_tls_ecc_import_pk(const unsigned char *public_key, int public_len,
                               ecc_key *key, const ltc_ecc_set_type *dp) {
  int err;

  if ((!key) || (!ltc_mp.name)) return CRYPT_MEM;

  key->type = PK_PUBLIC;

  if (mp_init_multi(&key->pubkey.x, &key->pubkey.y, &key->pubkey.z, &key->k,
                    NULL) != CRYPT_OK)
    return CRYPT_MEM;

  if ((public_len) && (!public_key[0])) {
    public_key++;
    public_len--;
  }
  if ((err =
           mp_read_unsigned_bin(key->pubkey.x, (unsigned char *)public_key + 1,
                                (public_len - 1) >> 1)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  if ((err = mp_read_unsigned_bin(
           key->pubkey.y,
           (unsigned char *)public_key + 1 + ((public_len - 1) >> 1),
           (public_len - 1) >> 1)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  key->idx = -1;
  key->dp = dp;

  /* set z */
  if ((err = mp_set(key->pubkey.z, 1)) != CRYPT_OK) {
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  /* is it a point on the curve?  */
  if ((err = _private_tls_is_point(key)) != CRYPT_OK) {
    DEBUG_PRINT("KEY IS NOT ON CURVE\n");
    mp_clear_multi(key->pubkey.x, key->pubkey.y, key->pubkey.z, key->k, NULL);
    return err;
  }

  /* we're good */
  return CRYPT_OK;
}

int _private_tls_verify_ecdsa(struct TLSContext *context,
                              unsigned int hash_type,
                              const unsigned char *buffer, unsigned int len,
                              const unsigned char *message,
                              unsigned int message_len,
                              const struct ECCCurveParameters *curve_hint) {
  tls_init();
  ecc_key key;
  int err;

  DEBUG_PRINT("ECDSA VERIFY");

  if (!curve_hint) curve_hint = context->curve;

  if (context->is_server) {
    DEBUG_PRINT("No client certificate support\n");
    return TLS_GENERIC_ERROR;
  } else {
    if ((!len) || (!context->certificates) || (!context->certificates_count) ||
        (!context->certificates[0]) || (!context->certificates[0]->pk) ||
        (!context->certificates[0]->pk_len) || (!curve_hint)) {
      DEBUG_PRINT("No server certificate set\n");
      return TLS_GENERIC_ERROR;
    }
    err = _private_tls_ecc_import_pk(context->certificates[0]->pk,
                                     context->certificates[0]->pk_len, &key,
                                     &curve_hint->dp);
  }
  if (err) {
    DEBUG_PRINT("Error importing ECC certificate (code: %i)", err);
    return TLS_GENERIC_ERROR;
  }
  int hash_idx = -1;
  unsigned char hash[TLS_MAX_HASH_LEN];
  unsigned int hash_len = 0;
  hash_state state;
  switch (hash_type) {
    case md5:
      hash_idx = find_hash("md5");
      err = md5_init(&state);
      if (!err) {
        err = md5_process(&state, message, message_len);
        if (!err) err = md5_done(&state, hash);
      }
      hash_len = 16;
      break;
    case sha1:
      hash_idx = find_hash("sha1");
      err = sha1_init(&state);
      if (!err) {
        err = sha1_process(&state, message, message_len);
        if (!err) err = sha1_done(&state, hash);
      }
      hash_len = 20;
      break;
    case sha256:
      hash_idx = find_hash("sha256");
      err = sha256_init(&state);
      if (!err) {
        err = sha256_process(&state, message, message_len);
        if (!err) err = sha256_done(&state, hash);
      }
      hash_len = 32;
      break;
    case sha384:
      hash_idx = find_hash("sha384");
      err = sha384_init(&state);
      if (!err) {
        err = sha384_process(&state, message, message_len);
        if (!err) err = sha384_done(&state, hash);
      }
      hash_len = 48;
      break;
    case sha512:
      hash_idx = find_hash("sha512");
      err = sha512_init(&state);
      if (!err) {
        err = sha512_process(&state, message, message_len);
        if (!err) err = sha512_done(&state, hash);
      }
      hash_len = 64;
      break;
    case _md5_sha1:
      hash_idx = find_hash("md5");
      err = md5_init(&state);
      if (!err) {
        err = md5_process(&state, message, message_len);
        if (!err) err = md5_done(&state, hash);
      }
      hash_idx = find_hash("sha1");
      err = sha1_init(&state);
      if (!err) {
        err = sha1_process(&state, message, message_len);
        if (!err) err = sha1_done(&state, hash + 16);
      }
      hash_len = 36;
      err = sha1_init(&state);
      if (!err) {
        err = sha1_process(&state, message, message_len);
        if (!err) err = sha1_done(&state, hash + 16);
      }
      hash_len = 36;
      break;
  }
  if ((hash_idx < 0) || (err)) {
    DEBUG_PRINT("Unsupported hash type: %i\n", hash_type);
    return TLS_GENERIC_ERROR;
  }
  int ecc_stat = 0;
  err = ecc_verify_hash(buffer, len, hash, hash_len, &ecc_stat, &key);
  ecc_free(&key);
  if (err) return 0;
  return ecc_stat;
}

unsigned int _private_tls_random_int(int limit) {
  unsigned int res = 0;
  tls_random((unsigned char *)&res, sizeof(int));
  if (limit) res %= limit;
  return res;
}

void _private_tls_sleep(unsigned int microseconds) {
  struct timespec ts;

  ts.tv_sec = microseconds / 1000000;
  ts.tv_nsec = microseconds % 1000000 * 1000ul;

  nanosleep(&ts, NULL);
}

void _private_random_sleep(struct TLSContext *context, int max_microseconds) {
  if (context)
    context->sleep_until = (unsigned int)time(NULL) +
                           _private_tls_random_int(max_microseconds / 1000000 *
                                                   TLS_MAX_ERROR_IDLE_S);
  else
    _private_tls_sleep(_private_tls_random_int(max_microseconds));
}

void _private_tls_prf_helper(int hash_idx, unsigned long dlen,
                             unsigned char *output, unsigned int outlen,
                             const unsigned char *secret,
                             const unsigned int secret_len,
                             const unsigned char *label, unsigned int label_len,
                             unsigned char *seed, unsigned int seed_len,
                             unsigned char *seed_b, unsigned int seed_b_len) {
  unsigned char digest_out0[TLS_MAX_HASH_LEN];
  unsigned char digest_out1[TLS_MAX_HASH_LEN];
  unsigned int i;
  hmac_state hmac;

  hmac_init(&hmac, hash_idx, secret, secret_len);
  hmac_process(&hmac, label, label_len);

  hmac_process(&hmac, seed, seed_len);
  if ((seed_b) && (seed_b_len)) hmac_process(&hmac, seed_b, seed_b_len);
  hmac_done(&hmac, digest_out0, &dlen);
  int idx = 0;
  while (outlen) {
    hmac_init(&hmac, hash_idx, secret, secret_len);
    hmac_process(&hmac, digest_out0, dlen);
    hmac_process(&hmac, label, label_len);
    hmac_process(&hmac, seed, seed_len);
    if ((seed_b) && (seed_b_len)) hmac_process(&hmac, seed_b, seed_b_len);
    hmac_done(&hmac, digest_out1, &dlen);

    unsigned long copylen = outlen;
    if (copylen > dlen) copylen = dlen;

    for (i = 0; i < copylen; i++) {
      output[idx++] ^= digest_out1[i];
      outlen--;
    }

    if (!outlen) break;

    hmac_init(&hmac, hash_idx, secret, secret_len);
    hmac_process(&hmac, digest_out0, dlen);
    hmac_done(&hmac, digest_out0, &dlen);
  }
}

int _private_tls_hkdf_label(const char *label, unsigned char label_len,
                            const unsigned char *data, unsigned char data_len,
                            unsigned char *hkdflabel, unsigned short length,
                            const char *prefix) {
  *(unsigned short *)hkdflabel = htons(length);
  int prefix_len;
  if (prefix) {
    prefix_len = (int)strlen(prefix);
    memcpy(&hkdflabel[3], prefix, prefix_len);
  } else {
    memcpy(&hkdflabel[3], "tls13 ", 6);
    prefix_len = 6;
  }
  hkdflabel[2] = (unsigned char)prefix_len + label_len;
  memcpy(&hkdflabel[3 + prefix_len], label, label_len);
  hkdflabel[3 + prefix_len + label_len] = data_len;
  if (data_len) memcpy(&hkdflabel[4 + prefix_len + label_len], data, data_len);
  return 4 + prefix_len + label_len + data_len;
}

int _private_tls_hkdf_extract(unsigned int mac_length, unsigned char *output,
                              unsigned int outlen, const unsigned char *salt,
                              unsigned int salt_len, const unsigned char *ikm,
                              unsigned char ikm_len) {
  unsigned long dlen = outlen;
  static unsigned char dummy_label[1] = {0};
  if ((!salt) || (salt_len == 0)) {
    salt_len = 1;
    salt = dummy_label;
  }
  int hash_idx;
  if (mac_length == TLS_SHA384_MAC_SIZE) {
    hash_idx = find_hash("sha384");
    dlen = mac_length;
  } else
    hash_idx = find_hash("sha256");

  hmac_state hmac;
  hmac_init(&hmac, hash_idx, salt, salt_len);
  hmac_process(&hmac, ikm, ikm_len);
  hmac_done(&hmac, output, &dlen);
  DEBUG_DUMP_HEX_LABEL("EXTRACT", output, dlen);
  return dlen;
}

void _private_tls_hkdf_expand(unsigned int mac_length, unsigned char *output,
                              unsigned int outlen, const unsigned char *secret,
                              unsigned int secret_len,
                              const unsigned char *info,
                              unsigned char info_len) {
  unsigned char digest_out[TLS_MAX_HASH_LEN];
  unsigned long dlen = 32;
  int hash_idx;
  if (mac_length == TLS_SHA384_MAC_SIZE) {
    hash_idx = find_hash("sha384");
    dlen = mac_length;
  } else
    hash_idx = find_hash("sha256");
  unsigned int i;
  unsigned int idx = 0;
  hmac_state hmac;
  unsigned char i2 = 0;
  while (outlen) {
    hmac_init(&hmac, hash_idx, secret, secret_len);
    if (i2) hmac_process(&hmac, digest_out, dlen);
    if ((info) && (info_len)) hmac_process(&hmac, info, info_len);
    i2++;
    hmac_process(&hmac, &i2, 1);
    hmac_done(&hmac, digest_out, &dlen);

    unsigned int copylen = outlen;
    if (copylen > dlen) copylen = (unsigned int)dlen;

    for (i = 0; i < copylen; i++) {
      output[idx++] = digest_out[i];
      outlen--;
    }

    if (!outlen) break;
  }
}

void _private_tls_hkdf_expand_label(unsigned int mac_length,
                                    unsigned char *output, unsigned int outlen,
                                    const unsigned char *secret,
                                    unsigned int secret_len, const char *label,
                                    unsigned char label_len,
                                    const unsigned char *data,
                                    unsigned char data_len) {
  unsigned char hkdf_label[512];
  int len = _private_tls_hkdf_label(label, label_len, data, data_len,
                                    hkdf_label, outlen, NULL);
  DEBUG_DUMP_HEX_LABEL("INFO", hkdf_label, len);
  _private_tls_hkdf_expand(mac_length, output, outlen, secret, secret_len,
                           hkdf_label, len);
}

void _private_tls_prf(struct TLSContext *context, unsigned char *output,
                      unsigned int outlen, const unsigned char *secret,
                      const unsigned int secret_len, const unsigned char *label,
                      unsigned int label_len, unsigned char *seed,
                      unsigned int seed_len, unsigned char *seed_b,
                      unsigned int seed_b_len) {
  if ((!secret) || (!secret_len)) {
    DEBUG_PRINT("NULL SECRET\n");
    return;
  }
  if ((context->version != TLS_V12) && (context->version != DTLS_V12)) {
    int md5_hash_idx = find_hash("md5");
    int sha1_hash_idx = find_hash("sha1");
    int half_secret = (secret_len + 1) / 2;

    memset(output, 0, outlen);
    _private_tls_prf_helper(md5_hash_idx, 16, output, outlen, secret,
                            half_secret, label, label_len, seed, seed_len,
                            seed_b, seed_b_len);
    _private_tls_prf_helper(sha1_hash_idx, 20, output, outlen,
                            secret + (secret_len - half_secret),
                            secret_len - half_secret, label, label_len, seed,
                            seed_len, seed_b, seed_b_len);
  } else {
    // sha256_hmac
    unsigned char digest_out0[TLS_MAX_HASH_LEN];
    unsigned char digest_out1[TLS_MAX_HASH_LEN];
    unsigned long dlen = 32;
    int hash_idx;
    unsigned int mac_length = _private_tls_mac_length(context);
    if (mac_length == TLS_SHA384_MAC_SIZE) {
      hash_idx = find_hash("sha384");
      dlen = mac_length;
    } else
      hash_idx = find_hash("sha256");
    unsigned int i;
    hmac_state hmac;

    hmac_init(&hmac, hash_idx, secret, secret_len);
    hmac_process(&hmac, label, label_len);

    hmac_process(&hmac, seed, seed_len);
    if ((seed_b) && (seed_b_len)) hmac_process(&hmac, seed_b, seed_b_len);
    hmac_done(&hmac, digest_out0, &dlen);
    int idx = 0;
    while (outlen) {
      hmac_init(&hmac, hash_idx, secret, secret_len);
      hmac_process(&hmac, digest_out0, dlen);
      hmac_process(&hmac, label, label_len);
      hmac_process(&hmac, seed, seed_len);
      if ((seed_b) && (seed_b_len)) hmac_process(&hmac, seed_b, seed_b_len);
      hmac_done(&hmac, digest_out1, &dlen);

      unsigned int copylen = outlen;
      if (copylen > dlen) copylen = (unsigned int)dlen;

      for (i = 0; i < copylen; i++) {
        output[idx++] = digest_out1[i];
        outlen--;
      }

      if (!outlen) break;

      hmac_init(&hmac, hash_idx, secret, secret_len);
      hmac_process(&hmac, digest_out0, dlen);
      hmac_done(&hmac, digest_out0, &dlen);
    }
  }
}

int _private_tls_key_length(struct TLSContext *context) {
  switch (context->cipher) {
    // case TLS_RSA_WITH_AES_128_CBC_SHA:
    // case TLS_RSA_WITH_AES_128_CBC_SHA256:
    case TLS_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA:
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA256:
    case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:
      // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
    case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:
      // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
    case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
    case TLS_AES_128_GCM_SHA256:
      return 16;
      // case TLS_RSA_WITH_AES_256_CBC_SHA:
      // case TLS_RSA_WITH_AES_256_CBC_SHA256:
    case TLS_RSA_WITH_AES_256_GCM_SHA384:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA256:
    case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
      // case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:
    case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
      // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:
      // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
    case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_AES_256_GCM_SHA384:
    case TLS_CHACHA20_POLY1305_SHA256:
      return 32;
  }
  return 0;
}

int _private_tls_is_aead(struct TLSContext *context) {
  switch (context->cipher) {
    case TLS_RSA_WITH_AES_128_GCM_SHA256:
    case TLS_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
    case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
    case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
    case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
    case TLS_AES_128_GCM_SHA256:
    case TLS_AES_256_GCM_SHA384:
      return 1;
    case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_CHACHA20_POLY1305_SHA256:
      return 2;
  }
  return 0;
}

unsigned int _private_tls_mac_length(struct TLSContext *context) {
  switch (context->cipher) {
    // case TLS_RSA_WITH_AES_128_CBC_SHA:
    // case TLS_RSA_WITH_AES_256_CBC_SHA:
    // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA:
    // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA:
    // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:
    // case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:
    // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:
    // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:
    //   return TLS_SHA1_MAC_SIZE;
    // case TLS_RSA_WITH_AES_128_CBC_SHA256:
    // case TLS_RSA_WITH_AES_256_CBC_SHA256:
    case TLS_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA256:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA256:
    case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
    case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
    case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
    case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_AES_128_GCM_SHA256:
    case TLS_CHACHA20_POLY1305_SHA256:
    case TLS_AES_128_CCM_SHA256:
    case TLS_AES_128_CCM_8_SHA256:
      return TLS_SHA256_MAC_SIZE;
    case TLS_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
      // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
    case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
    case TLS_AES_256_GCM_SHA384:
      return TLS_SHA384_MAC_SIZE;
  }
  return 0;
}

int _private_tls13_key(struct TLSContext *context, int handshake) {
  tls_init();

  int key_length = _private_tls_key_length(context);
  unsigned int mac_length = _private_tls_mac_length(context);

  if ((!context->premaster_key) || (!context->premaster_key_len)) return 0;

  if ((!key_length) || (!mac_length)) {
    DEBUG_PRINT("KEY EXPANSION FAILED, KEY LENGTH: %i, MAC LENGTH: %i\n",
                key_length, mac_length);
    return 0;
  }

  unsigned char *clientkey = NULL;
  unsigned char *serverkey = NULL;
  unsigned char *clientiv = NULL;
  unsigned char *serveriv = NULL;
  int is_aead = _private_tls_is_aead(context);

  unsigned char local_keybuffer[TLS_V13_MAX_KEY_SIZE];
  unsigned char local_ivbuffer[TLS_V13_MAX_IV_SIZE];
  unsigned char remote_keybuffer[TLS_V13_MAX_KEY_SIZE];
  unsigned char remote_ivbuffer[TLS_V13_MAX_IV_SIZE];

  unsigned char prk[TLS_MAX_HASH_SIZE];
  unsigned char hash[TLS_MAX_HASH_SIZE];
  static unsigned char earlysecret[TLS_MAX_HASH_SIZE];

  const char *server_key = "s ap traffic";
  const char *client_key = "c ap traffic";
  if (handshake) {
    server_key = "s hs traffic";
    client_key = "c hs traffic";
  }

  unsigned char salt[TLS_MAX_HASH_SIZE];

  hash_state md;
  if (mac_length == TLS_SHA384_MAC_SIZE) {
    sha384_init(&md);
    sha384_done(&md, hash);
  } else {
    sha256_init(&md);
    sha256_done(&md, hash);
  }
  // extract secret "early"
  if ((context->master_key) && (context->master_key_len) && (!handshake)) {
    DEBUG_DUMP_HEX_LABEL("USING PREVIOUS SECRET", context->master_key,
                         context->master_key_len);
    _private_tls_hkdf_expand_label(mac_length, salt, mac_length,
                                   context->master_key, context->master_key_len,
                                   "derived", 7, hash, mac_length);
    DEBUG_DUMP_HEX_LABEL("salt", salt, mac_length);
    _private_tls_hkdf_extract(mac_length, prk, mac_length, salt, mac_length,
                              earlysecret, mac_length);
  } else {
    _private_tls_hkdf_extract(mac_length, prk, mac_length, NULL, 0, earlysecret,
                              mac_length);
    // derive secret for handshake "tls13 derived":
    DEBUG_DUMP_HEX_LABEL("null hash", hash, mac_length);
    _private_tls_hkdf_expand_label(mac_length, salt, mac_length, prk,
                                   mac_length, "derived", 7, hash, mac_length);
    // extract secret "handshake":
    DEBUG_DUMP_HEX_LABEL("salt", salt, mac_length);
    _private_tls_hkdf_extract(mac_length, prk, mac_length, salt, mac_length,
                              context->premaster_key,
                              context->premaster_key_len);
  }

  if (!is_aead) {
    DEBUG_PRINT("KEY EXPANSION FAILED, NON AEAD CIPHER\n");
    return 0;
  }

  unsigned char secret[TLS_MAX_MAC_SIZE];
  unsigned char hs_secret[TLS_MAX_HASH_SIZE];

  int hash_size;
  if (handshake)
    hash_size = _private_tls_get_hash(context, hash);
  else
    hash_size = _private_tls_done_hash(context, hash);
  DEBUG_DUMP_HEX_LABEL("messages hash", hash, hash_size);

  if (context->is_server) {
    _private_tls_hkdf_expand_label(
        mac_length, hs_secret, mac_length, prk, mac_length, server_key, 12,
        context->server_finished_hash ? context->server_finished_hash : hash,
        hash_size);
    DEBUG_DUMP_HEX_LABEL(server_key, hs_secret, mac_length);
    serverkey = local_keybuffer;
    serveriv = local_ivbuffer;
    clientkey = remote_keybuffer;
    clientiv = remote_ivbuffer;
  } else {
    _private_tls_hkdf_expand_label(
        mac_length, hs_secret, mac_length, prk, mac_length, client_key, 12,
        context->server_finished_hash ? context->server_finished_hash : hash,
        hash_size);
    DEBUG_DUMP_HEX_LABEL(client_key, hs_secret, mac_length);
    serverkey = remote_keybuffer;
    serveriv = remote_ivbuffer;
    clientkey = local_keybuffer;
    clientiv = local_ivbuffer;
  }

  int iv_length = TLS_13_AES_GCM_IV_LENGTH;
  if (is_aead == 2) iv_length = TLS_CHACHA20_IV_LENGTH;

  _private_tls_hkdf_expand_label(mac_length, local_keybuffer, key_length,
                                 hs_secret, mac_length, "key", 3, NULL, 0);
  _private_tls_hkdf_expand_label(mac_length, local_ivbuffer, iv_length,
                                 hs_secret, mac_length, "iv", 2, NULL, 0);
  if (context->is_server)
    _private_tls_hkdf_expand_label(
        mac_length, secret, mac_length, prk, mac_length, client_key, 12,
        context->server_finished_hash ? context->server_finished_hash : hash,
        hash_size);
  else
    _private_tls_hkdf_expand_label(
        mac_length, secret, mac_length, prk, mac_length, server_key, 12,
        context->server_finished_hash ? context->server_finished_hash : hash,
        hash_size);

  _private_tls_hkdf_expand_label(mac_length, remote_keybuffer, key_length,
                                 secret, mac_length, "key", 3, NULL, 0);
  _private_tls_hkdf_expand_label(mac_length, remote_ivbuffer, iv_length, secret,
                                 mac_length, "iv", 2, NULL, 0);

  DEBUG_DUMP_HEX_LABEL("CLIENT KEY", clientkey, key_length)
  DEBUG_DUMP_HEX_LABEL("CLIENT IV", clientiv, iv_length)
  DEBUG_DUMP_HEX_LABEL("SERVER KEY", serverkey, key_length)
  DEBUG_DUMP_HEX_LABEL("SERVER IV", serveriv, iv_length)

  TLS_FREE(context->finished_key);
  TLS_FREE(context->remote_finished_key);
  if (handshake) {
    context->finished_key = (unsigned char *)TLS_MALLOC(mac_length);
    context->remote_finished_key = (unsigned char *)TLS_MALLOC(mac_length);

    if (context->finished_key) {
      _private_tls_hkdf_expand_label(mac_length, context->finished_key,
                                     mac_length, hs_secret, mac_length,
                                     "finished", 8, NULL, 0);
      DEBUG_DUMP_HEX_LABEL("FINISHED", context->finished_key, mac_length)
    }

    if (context->remote_finished_key) {
      _private_tls_hkdf_expand_label(mac_length, context->remote_finished_key,
                                     mac_length, secret, mac_length, "finished",
                                     8, NULL, 0);
      DEBUG_DUMP_HEX_LABEL("REMOTE FINISHED", context->remote_finished_key,
                           mac_length)
    }
  } else {
    context->finished_key = NULL;
    context->remote_finished_key = NULL;
    TLS_FREE(context->server_finished_hash);
    context->server_finished_hash = NULL;
  }

  if (context->is_server) {
    if (is_aead == 2) {
      memcpy(context->crypto.ctx_remote_mac.remote_nonce, clientiv, iv_length);
      memcpy(context->crypto.ctx_local_mac.local_nonce, serveriv, iv_length);
    } else if (is_aead) {
      memcpy(context->crypto.ctx_remote_mac.remote_iv, clientiv, iv_length);
      memcpy(context->crypto.ctx_local_mac.local_iv, serveriv, iv_length);
    }
    if (_private_tls_crypto_create(context, key_length, serverkey, serveriv,
                                   clientkey, clientiv))
      return 0;
  } else {
    if (is_aead == 2) {
      memcpy(context->crypto.ctx_local_mac.local_nonce, clientiv, iv_length);
      memcpy(context->crypto.ctx_remote_mac.remote_nonce, serveriv, iv_length);
    } else if (is_aead) {
      memcpy(context->crypto.ctx_local_mac.local_iv, clientiv, iv_length);
      memcpy(context->crypto.ctx_remote_mac.remote_iv, serveriv, iv_length);
    }
    if (_private_tls_crypto_create(context, key_length, clientkey, clientiv,
                                   serverkey, serveriv))
      return 0;
  }
  context->crypto.created = 1 + is_aead;
  if (context->exportable) {
    TLS_FREE(context->exportable_keys);
    context->exportable_keys = (unsigned char *)TLS_MALLOC(key_length * 2);
    if (context->exportable_keys) {
      if (context->is_server) {
        memcpy(context->exportable_keys, serverkey, key_length);
        memcpy(context->exportable_keys + key_length, clientkey, key_length);
      } else {
        memcpy(context->exportable_keys, clientkey, key_length);
        memcpy(context->exportable_keys + key_length, serverkey, key_length);
      }
      context->exportable_size = key_length * 2;
    }
  }
  TLS_FREE(context->master_key);
  context->master_key = (unsigned char *)TLS_MALLOC(mac_length);
  if (context->master_key) {
    memcpy(context->master_key, prk, mac_length);
    context->master_key_len = mac_length;
  }
  context->local_sequence_number = 0;
  context->remote_sequence_number = 0;

  // extract client_mac_key(mac_key_length)
  // extract server_mac_key(mac_key_length)
  // extract client_key(enc_key_length)
  // extract server_key(enc_key_length)
  // extract client_iv(fixed_iv_lengh)
  // extract server_iv(fixed_iv_length)
  return 1;
}

int _private_tls_expand_key(struct TLSContext *context) {
  unsigned char key[TLS_MAX_KEY_EXPANSION_SIZE];
  if ((context->version == TLS_V13) || (context->version == DTLS_V13)) return 0;

  if ((!context->master_key) || (!context->master_key_len)) return 0;

  int key_length = _private_tls_key_length(context);
  int mac_length = _private_tls_mac_length(context);

  if ((!key_length) || (!mac_length)) {
    DEBUG_PRINT("KEY EXPANSION FAILED, KEY LENGTH: %i, MAC LENGTH: %i\n",
                key_length, mac_length);
    return 0;
  }
  unsigned char *clientkey = NULL;
  unsigned char *serverkey = NULL;
  unsigned char *clientiv = NULL;
  unsigned char *serveriv = NULL;
  int iv_length = TLS_AES_IV_LENGTH;
  int is_aead = _private_tls_is_aead(context);
  if (context->is_server)
    _private_tls_prf(context, key, sizeof(key), context->master_key,
                     context->master_key_len, (unsigned char *)"key expansion",
                     13, context->local_random, TLS_SERVER_RANDOM_SIZE,
                     context->remote_random, TLS_CLIENT_RANDOM_SIZE);
  else
    _private_tls_prf(context, key, sizeof(key), context->master_key,
                     context->master_key_len, (unsigned char *)"key expansion",
                     13, context->remote_random, TLS_SERVER_RANDOM_SIZE,
                     context->local_random, TLS_CLIENT_RANDOM_SIZE);

  DEBUG_DUMP_HEX_LABEL("LOCAL RANDOM ", context->local_random,
                       TLS_SERVER_RANDOM_SIZE);
  DEBUG_DUMP_HEX_LABEL("REMOTE RANDOM", context->remote_random,
                       TLS_CLIENT_RANDOM_SIZE);
  DEBUG_PRINT("\n=========== EXPANSION ===========\n");
  DEBUG_DUMP_HEX(key, TLS_MAX_KEY_EXPANSION_SIZE);
  DEBUG_PRINT("\n");

  int pos = 0;
  if (is_aead == 2) {
    iv_length = TLS_CHACHA20_IV_LENGTH;
  } else if (is_aead) {
    iv_length = TLS_AES_GCM_IV_LENGTH;
  } else {
    if (context->is_server) {
      memcpy(context->crypto.ctx_remote_mac.remote_mac, &key[pos], mac_length);
      pos += mac_length;
      memcpy(context->crypto.ctx_local_mac.local_mac, &key[pos], mac_length);
      pos += mac_length;
    } else {
      memcpy(context->crypto.ctx_local_mac.local_mac, &key[pos], mac_length);
      pos += mac_length;
      memcpy(context->crypto.ctx_remote_mac.remote_mac, &key[pos], mac_length);
      pos += mac_length;
    }
  }

  clientkey = &key[pos];
  pos += key_length;
  serverkey = &key[pos];
  pos += key_length;
  clientiv = &key[pos];
  pos += iv_length;
  serveriv = &key[pos];
  pos += iv_length;
  DEBUG_PRINT("EXPANSION %i/%i\n", (int)pos, (int)TLS_MAX_KEY_EXPANSION_SIZE);
  DEBUG_DUMP_HEX_LABEL("CLIENT KEY", clientkey, key_length)
  DEBUG_DUMP_HEX_LABEL("CLIENT IV", clientiv, iv_length)
  DEBUG_DUMP_HEX_LABEL("CLIENT MAC KEY",
                       context->is_server
                           ? context->crypto.ctx_remote_mac.remote_mac
                           : context->crypto.ctx_local_mac.local_mac,
                       mac_length)
  DEBUG_DUMP_HEX_LABEL("SERVER KEY", serverkey, key_length)
  DEBUG_DUMP_HEX_LABEL("SERVER IV", serveriv, iv_length)
  DEBUG_DUMP_HEX_LABEL("SERVER MAC KEY",
                       context->is_server
                           ? context->crypto.ctx_local_mac.local_mac
                           : context->crypto.ctx_remote_mac.remote_mac,
                       mac_length)

  if (context->is_server) {
    if (is_aead == 2) {
      memcpy(context->crypto.ctx_remote_mac.remote_nonce, clientiv, iv_length);
      memcpy(context->crypto.ctx_local_mac.local_nonce, serveriv, iv_length);
    } else if (is_aead) {
      memcpy(context->crypto.ctx_remote_mac.remote_aead_iv, clientiv,
             iv_length);
      memcpy(context->crypto.ctx_local_mac.local_aead_iv, serveriv, iv_length);
    }
    if (_private_tls_crypto_create(context, key_length, serverkey, serveriv,
                                   clientkey, clientiv))
      return 0;
  } else {
    if (is_aead == 2) {
      memcpy(context->crypto.ctx_local_mac.local_nonce, clientiv, iv_length);
      memcpy(context->crypto.ctx_remote_mac.remote_nonce, serveriv, iv_length);
    } else if (is_aead) {
      memcpy(context->crypto.ctx_local_mac.local_aead_iv, clientiv, iv_length);
      memcpy(context->crypto.ctx_remote_mac.remote_aead_iv, serveriv,
             iv_length);
    }
    if (_private_tls_crypto_create(context, key_length, clientkey, clientiv,
                                   serverkey, serveriv))
      return 0;
  }

  if (context->exportable) {
    TLS_FREE(context->exportable_keys);
    context->exportable_keys = (unsigned char *)TLS_MALLOC(key_length * 2);
    if (context->exportable_keys) {
      if (context->is_server) {
        memcpy(context->exportable_keys, serverkey, key_length);
        memcpy(context->exportable_keys + key_length, clientkey, key_length);
      } else {
        memcpy(context->exportable_keys, clientkey, key_length);
        memcpy(context->exportable_keys + key_length, serverkey, key_length);
      }
      context->exportable_size = key_length * 2;
    }
  }

  // extract client_mac_key(mac_key_length)
  // extract server_mac_key(mac_key_length)
  // extract client_key(enc_key_length)
  // extract server_key(enc_key_length)
  // extract client_iv(fixed_iv_lengh)
  // extract server_iv(fixed_iv_length)
  return 1;
}

int _private_tls_compute_key(struct TLSContext *context, unsigned int key_len) {
  if ((context->version == TLS_V13) || (context->version == DTLS_V13)) return 0;
  if ((!context->premaster_key) || (!context->premaster_key_len) ||
      (key_len < 48)) {
    DEBUG_PRINT("CANNOT COMPUTE MASTER SECRET\n");
    return 0;
  }
  unsigned char master_secret_label[] = "master secret";
  if (!tls_cipher_is_ephemeral(context)) {
    unsigned short version = ntohs(*(unsigned short *)context->premaster_key);
    // this check is not true for DHE/ECDHE ciphers
    if (context->version > version) {
      DEBUG_PRINT("Mismatch protocol version 0x(%x)\n", version);
      return 0;
    }
  }
  TLS_FREE(context->master_key);
  context->master_key_len = 0;
  context->master_key = NULL;
  if ((context->version == TLS_V13) || (context->version == TLS_V12) ||
      (context->version == TLS_V11) || (context->version == TLS_V10) ||
      (context->version == DTLS_V13) || (context->version == DTLS_V12) ||
      (context->version == DTLS_V10)) {
    context->master_key = (unsigned char *)TLS_MALLOC(key_len);
    if (!context->master_key) return 0;
    context->master_key_len = key_len;
    if (context->is_server) {
      _private_tls_prf(context, context->master_key, context->master_key_len,
                       context->premaster_key, context->premaster_key_len,
                       master_secret_label, 13, context->remote_random,
                       TLS_CLIENT_RANDOM_SIZE, context->local_random,
                       TLS_SERVER_RANDOM_SIZE);
    } else {
      _private_tls_prf(context, context->master_key, context->master_key_len,
                       context->premaster_key, context->premaster_key_len,
                       master_secret_label, 13, context->local_random,
                       TLS_CLIENT_RANDOM_SIZE, context->remote_random,
                       TLS_SERVER_RANDOM_SIZE);
    }
    TLS_FREE(context->premaster_key);
    context->premaster_key = NULL;
    context->premaster_key_len = 0;
    DEBUG_PRINT("\n=========== Master key ===========\n");
    DEBUG_DUMP_HEX(context->master_key, context->master_key_len);
    DEBUG_PRINT("\n");
    _private_tls_expand_key(context);
    return 1;
  }
  return 0;
}

unsigned char *tls_pem_decode(const unsigned char *data_in,
                              unsigned int input_length, int cert_index,
                              unsigned int *output_len) {
  unsigned int i;
  *output_len = 0;
  int alloc_len = input_length / 4 * 3;
  unsigned char *output = (unsigned char *)TLS_MALLOC(alloc_len);
  if (!output) return NULL;
  unsigned int start_at = 0;
  unsigned int idx = 0;
  for (i = 0; i < input_length; i++) {
    if ((data_in[i] == '\n') || (data_in[i] == '\r')) continue;

    if (data_in[i] != '-') {
      // read entire line
      while ((i < input_length) && (data_in[i] != '\n')) i++;
      continue;
    }

    if (data_in[i] == '-') {
      unsigned int end_idx = i;
      // read until end of line
      while ((i < input_length) && (data_in[i] != '\n')) i++;
      if (start_at) {
        if (cert_index > 0) {
          cert_index--;
          start_at = 0;
        } else {
          idx = _private_b64_decode((const char *)&data_in[start_at],
                                    end_idx - start_at, output);
          break;
        }
      } else
        start_at = i + 1;
    }
  }
  *output_len = idx;
  if (!idx) {
    TLS_FREE(output);
    return NULL;
  }
  return output;
}

int _is_oid(const unsigned char *oid, const unsigned char *compare_to,
            int compare_to_len) {
  int i = 0;
  while ((oid[i]) && (i < compare_to_len)) {
    if (oid[i] != compare_to[i]) return 0;

    i++;
  }
  return 1;
}

int _is_oid2(const unsigned char *oid, const unsigned char *compare_to,
             int compare_to_len, int oid_len) {
  int i = 0;
  if (oid_len < compare_to_len) compare_to_len = oid_len;
  while (i < compare_to_len) {
    if (oid[i] != compare_to[i]) return 0;

    i++;
  }
  return 1;
}

struct TLSCertificate *tls_create_certificate() {
  struct TLSCertificate *cert =
      (struct TLSCertificate *)TLS_MALLOC(sizeof(struct TLSCertificate));
  if (cert) memset(cert, 0, sizeof(struct TLSCertificate));
  return cert;
}

int tls_certificate_valid_subject_name(const unsigned char *cert_subject,
                                       const char *subject) {
  // no subjects ...
  if (((!cert_subject) || (!cert_subject[0])) && ((!subject) || (!subject[0])))
    return 0;

  if ((!subject) || (!subject[0])) return bad_certificate;

  if ((!cert_subject) || (!cert_subject[0])) return bad_certificate;

  // exact match
  if (!strcmp((const char *)cert_subject, subject)) return 0;

  const char *wildcard = strchr((const char *)cert_subject, '*');
  if (wildcard) {
    // 6.4.3 (1) The client SHOULD NOT attempt to match a presented identifier
    // in which the wildcard character comprises a label other than the
    // left-most label
    if (!wildcard[1]) {
      // subject is [*]
      // or
      // subject is [something*] .. invalid
      return bad_certificate;
    }
    wildcard++;
    const char *match = strstr(subject, wildcard);
    if ((!match) && (wildcard[0] == '.')) {
      // check *.domain.com agains domain.com
      wildcard++;
      if (!strcasecmp(subject, wildcard)) return 0;
    }
    if (match) {
      uintptr_t offset = (uintptr_t)match - (uintptr_t)subject;
      if (offset) {
        // check for foo.*.domain.com against *.domain.com (invalid)
        if (memchr(subject, '.', offset)) return bad_certificate;
      }
      // check if exact match
      if (!strcasecmp(match, wildcard)) return 0;
    }
  }

  return bad_certificate;
}

int tls_certificate_valid_subject(struct TLSCertificate *cert,
                                  const char *subject) {
  int i;
  if (!cert) return certificate_unknown;
  int err = tls_certificate_valid_subject_name(cert->subject, subject);
  if ((err) && (cert->san)) {
    for (i = 0; i < cert->san_length; i++) {
      err = tls_certificate_valid_subject_name(cert->san[i], subject);
      if (!err) return err;
    }
  }
  return err;
}

int tls_certificate_is_valid(struct TLSCertificate *cert) {
  if (!cert) return certificate_unknown;
  if (!cert->not_before) return certificate_unknown;
  if (!cert->not_after) return certificate_unknown;
  // 20160224182300Z//
  char current_time[16];
  time_t t = time(NULL);
  struct tm *utct = gmtime(&t);
  if (utct) {
    current_time[0] = 0;
    snprintf(current_time, sizeof(current_time), "%04d%02d%02d%02d%02d%02dZ",
             1900 + utct->tm_year, utct->tm_mon + 1, utct->tm_mday,
             utct->tm_hour, utct->tm_min, utct->tm_sec);
    if (strcasecmp((char *)cert->not_before, current_time) > 0) {
      DEBUG_PRINT("Certificate is not yer valid, now: %s (validity: %s - %s)\n",
                  current_time, cert->not_before, cert->not_after);
      return certificate_expired;
    }
    if (strcasecmp((char *)cert->not_after, current_time) < 0) {
      DEBUG_PRINT("Expired certificate, now: %s (validity: %s - %s)\n",
                  current_time, cert->not_before, cert->not_after);
      return certificate_expired;
    }
    DEBUG_PRINT("Valid certificate, now: %s (validity: %s - %s)\n",
                current_time, cert->not_before, cert->not_after);
  }
  return 0;
}

void tls_certificate_set_copy(unsigned char **member, const unsigned char *val,
                              int len) {
  if (!member) return;
  TLS_FREE(*member);
  if (len) {
    *member = (unsigned char *)TLS_MALLOC(len + 1);
    if (*member) {
      memcpy(*member, val, len);
      (*member)[len] = 0;
    }
  } else
    *member = NULL;
}

void tls_certificate_set_copy_date(unsigned char **member,
                                   const unsigned char *val, int len) {
  if (!member) return;
  TLS_FREE(*member);
  if (len > 4) {
    *member = (unsigned char *)TLS_MALLOC(len + 3);
    if (*member) {
      if (val[0] == '9') {
        (*member)[0] = '1';
        (*member)[1] = '9';
      } else {
        (*member)[0] = '2';
        (*member)[1] = '0';
      }
      memcpy(*member + 2, val, len);
      (*member)[len] = 0;
    }
  } else
    *member = NULL;
}

void tls_certificate_set_key(struct TLSCertificate *cert,
                             const unsigned char *val, int len) {
  if ((!val[0]) && (len % 2)) {
    val++;
    len--;
  }
  tls_certificate_set_copy(&cert->pk, val, len);
  if (cert->pk) cert->pk_len = len;
}

void tls_certificate_set_priv(struct TLSCertificate *cert,
                              const unsigned char *val, int len) {
  tls_certificate_set_copy(&cert->priv, val, len);
  if (cert->priv) cert->priv_len = len;
}

void tls_certificate_set_sign_key(struct TLSCertificate *cert,
                                  const unsigned char *val, int len) {
  if ((!val[0]) && (len % 2)) {
    val++;
    len--;
  }
  tls_certificate_set_copy(&cert->sign_key, val, len);
  if (cert->sign_key) cert->sign_len = len;
}

char *tls_certificate_to_string(struct TLSCertificate *cert, char *buffer,
                                int len) {
  unsigned int i;
  if (!buffer) return NULL;
  buffer[0] = 0;
  if (cert->version) {
    int res = snprintf(buffer, len,
                       "X.509v%i certificate\n  Issued by: [%s]%s (%s)\n  "
                       "Issued to: [%s]%s (%s, %s)\n  Subject: %s\n  Validity: "
                       "%s - %s\n  OCSP: %s\n  Serial number: ",
                       (int)cert->version, cert->issuer_country,
                       cert->issuer_entity, cert->issuer_subject, cert->country,
                       cert->entity, cert->state, cert->location, cert->subject,
                       cert->not_before, cert->not_after, cert->ocsp);
    if (res > 0) {
      for (i = 0; i < cert->serial_len; i++)
        res += snprintf(buffer + res, len - res, "%02x",
                        (int)cert->serial_number[i]);
    }
    if ((cert->san) && (cert->san_length)) {
      res += snprintf(buffer + res, len - res, "\n  Alternative subjects: ");
      for (i = 0; i < cert->san_length; i++) {
        if (i)
          res += snprintf(buffer + res, len - res, ", %s", cert->san[i]);
        else
          res += snprintf(buffer + res, len - res, "%s", cert->san[i]);
      }
    }
    res += snprintf(buffer + res, len - res, "\n  Key (%i bits, ",
                    cert->pk_len * 8);
    if (res > 0) {
      switch (cert->key_algorithm) {
        case TLS_RSA_SIGN_RSA:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_RSA");
          break;
        case TLS_RSA_SIGN_MD5:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_MD5");
          break;
        case TLS_RSA_SIGN_SHA1:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA1");
          break;
        case TLS_RSA_SIGN_SHA256:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA256");
          break;
        case TLS_RSA_SIGN_SHA384:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA384");
          break;
        case TLS_RSA_SIGN_SHA512:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA512");
          break;
        case TLS_EC_PUBLIC_KEY:
          res += snprintf(buffer + res, len - res, "EC_PUBLIC_KEY");
          break;
        default:
          res += snprintf(buffer + res, len - res, "not supported (%i)",
                          (int)cert->key_algorithm);
      }
    }
    if ((res > 0) && (cert->ec_algorithm)) {
      switch (cert->ec_algorithm) {
        case TLS_EC_prime192v1:
          res += snprintf(buffer + res, len - res, " prime192v1");
          break;
        case TLS_EC_prime192v2:
          res += snprintf(buffer + res, len - res, " prime192v2");
          break;
        case TLS_EC_prime192v3:
          res += snprintf(buffer + res, len - res, " prime192v3");
          break;
        case TLS_EC_prime239v2:
          res += snprintf(buffer + res, len - res, " prime239v2");
          break;
        case TLS_EC_secp256r1:
          res += snprintf(buffer + res, len - res, " EC_secp256r1");
          break;
        case TLS_EC_secp224r1:
          res += snprintf(buffer + res, len - res, " EC_secp224r1");
          break;
        case TLS_EC_secp384r1:
          res += snprintf(buffer + res, len - res, " EC_secp384r1");
          break;
        case TLS_EC_secp521r1:
          res += snprintf(buffer + res, len - res, " EC_secp521r1");
          break;
        default:
          res += snprintf(buffer + res, len - res, " unknown(%i)",
                          (int)cert->ec_algorithm);
      }
    }
    res += snprintf(buffer + res, len - res, "):\n");
    if (res > 0) {
      for (i = 0; i < cert->pk_len; i++)
        res += snprintf(buffer + res, len - res, "%02x", (int)cert->pk[i]);
      res += snprintf(buffer + res, len - res, "\n  Signature (%i bits, ",
                      cert->sign_len * 8);
      switch (cert->algorithm) {
        case TLS_RSA_SIGN_RSA:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_RSA):\n");
          break;
        case TLS_RSA_SIGN_MD5:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_MD5):\n");
          break;
        case TLS_RSA_SIGN_SHA1:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA1):\n");
          break;
        case TLS_RSA_SIGN_SHA256:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA256):\n");
          break;
        case TLS_RSA_SIGN_SHA384:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA384):\n");
          break;
        case TLS_RSA_SIGN_SHA512:
          res += snprintf(buffer + res, len - res, "RSA_SIGN_SHA512):\n");
          break;
        case TLS_EC_PUBLIC_KEY:
          res += snprintf(buffer + res, len - res, "EC_PUBLIC_KEY):\n");
          break;
        default:
          res += snprintf(buffer + res, len - res, "not supported: %i):\n",
                          cert->algorithm);
      }

      for (i = 0; i < cert->sign_len; i++)
        res +=
            snprintf(buffer + res, len - res, "%02x", (int)cert->sign_key[i]);
    }
  } else if ((cert->priv) && (cert->priv_len)) {
    int res = snprintf(buffer, len, "X.509 private key\n");
    res += snprintf(buffer + res, len - res, "  Private Key: ");
    if (res > 0) {
      for (i = 0; i < cert->priv_len; i++)
        res += snprintf(buffer + res, len - res, "%02x", (int)cert->priv[i]);
    }
  } else
    snprintf(buffer, len, "Empty ASN1 file");
  return buffer;
}

void tls_certificate_set_exponent(struct TLSCertificate *cert,
                                  const unsigned char *val, int len) {
  tls_certificate_set_copy(&cert->exponent, val, len);
}

void tls_certificate_set_serial(struct TLSCertificate *cert,
                                const unsigned char *val, int len) {
  tls_certificate_set_copy(&cert->serial_number, val, len);
  if (cert->serial_number) cert->serial_len = len;
}

void tls_certificate_set_algorithm(unsigned int *algorithm,
                                   const unsigned char *val, int len) {
  if ((len == 7) && (_is_oid(val, TLS_EC_PUBLIC_KEY_OID, 7))) {
    *algorithm = TLS_EC_PUBLIC_KEY;
    return;
  }
  if (len == 8) {
    if (_is_oid(val, TLS_EC_prime192v1_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime192v1) ");
      *algorithm = TLS_EC_prime192v1;
      return;
    }
    if (_is_oid(val, TLS_EC_prime192v2_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime192v2) ");
      *algorithm = TLS_EC_prime192v2;
      return;
    }
    if (_is_oid(val, TLS_EC_prime192v3_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime192v3) ");
      *algorithm = TLS_EC_prime192v3;
      return;
    }
    if (_is_oid(val, TLS_EC_prime239v1_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime239v1) ");
      *algorithm = TLS_EC_prime239v1;
      return;
    }
    if (_is_oid(val, TLS_EC_prime239v2_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime239v2) ");
      *algorithm = TLS_EC_prime239v2;
      return;
    }
    if (_is_oid(val, TLS_EC_prime239v3_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime239v3) ");
      *algorithm = TLS_EC_prime239v3;
      return;
    }
    if (_is_oid(val, TLS_EC_prime256v1_OID, len)) {
      DEBUG_PRINT(" (SETTING ALGORITHM: TLS_EC_prime256v1) ");
      *algorithm = TLS_EC_prime256v1;
      return;
    }
    DEBUG_PRINT(" (ERROR: L8 ALGO DOESN'T MATCH ANY OID) ");
  }
  if (len == 5) {
    if (_is_oid2(val, TLS_EC_secp224r1_OID, len,
                 sizeof(TLS_EC_secp224r1_OID) - 1)) {
      *algorithm = TLS_EC_secp224r1;
      return;
    }
    if (_is_oid2(val, TLS_EC_secp384r1_OID, len,
                 sizeof(TLS_EC_secp384r1_OID) - 1)) {
      *algorithm = TLS_EC_secp384r1;
      return;
    }
    if (_is_oid2(val, TLS_EC_secp521r1_OID, len,
                 sizeof(TLS_EC_secp521r1_OID) - 1)) {
      *algorithm = TLS_EC_secp521r1;
      return;
    }
  }
  if (len != 9) return;

  if (_is_oid(val, TLS_RSA_SIGN_SHA256_OID, 9)) {
    DEBUG_PRINT(" (SETTING ALGORITHM: TLS_RSA_SIGN_SHA256) ");
    *algorithm = TLS_RSA_SIGN_SHA256;
    return;
  }

  if (_is_oid(val, TLS_RSA_SIGN_RSA_OID, 9)) {
    DEBUG_PRINT(" (SETTING ALGORITHM: TLS_RSA_SIGN_RSA) ");
    *algorithm = TLS_RSA_SIGN_RSA;
    return;
  }

  if (_is_oid(val, TLS_RSA_SIGN_SHA1_OID, 9)) {
    DEBUG_PRINT(" (SETTING ALGORITHM: TLS_RSA_SIGN_SHA1) ");
    *algorithm = TLS_RSA_SIGN_SHA1;
    return;
  }

  if (_is_oid(val, TLS_RSA_SIGN_SHA512_OID, 9)) {
    DEBUG_PRINT(" (SETTING ALGORITHM: TLS_RSA_SIGN_SHA512) ");
    *algorithm = TLS_RSA_SIGN_SHA512;
    return;
  }

  if (_is_oid(val, TLS_RSA_SIGN_SHA384_OID, 9)) {
    DEBUG_PRINT(" (SETTING ALGORITHM: TLS_RSA_SIGN_SHA384) ");
    *algorithm = TLS_RSA_SIGN_SHA384;
    return;
  }

  if (_is_oid(val, TLS_RSA_SIGN_MD5_OID, 9)) {
    DEBUG_PRINT(" (SETTING ALGORITHM: TLS_RSA_SIGN_MD5) ");
    *algorithm = TLS_RSA_SIGN_MD5;
    return;
  }

  DEBUG_PRINT(" (ERROR: L9 ALGO DOESN'T MATCH ANY OID) ");
}

void tls_destroy_certificate(struct TLSCertificate *cert) {
  if (cert) {
    int i;
    TLS_FREE(cert->exponent);
    TLS_FREE(cert->pk);
    TLS_FREE(cert->issuer_country);
    TLS_FREE(cert->issuer_state);
    TLS_FREE(cert->issuer_location);
    TLS_FREE(cert->issuer_entity);
    TLS_FREE(cert->issuer_subject);
    TLS_FREE(cert->country);
    TLS_FREE(cert->state);
    TLS_FREE(cert->location);
    TLS_FREE(cert->subject);
    for (i = 0; i < cert->san_length; i++) {
      TLS_FREE(cert->san[i]);
    }
    TLS_FREE(cert->san);
    TLS_FREE(cert->ocsp);
    TLS_FREE(cert->serial_number);
    TLS_FREE(cert->entity);
    TLS_FREE(cert->not_before);
    TLS_FREE(cert->not_after);
    TLS_FREE(cert->sign_key);
    TLS_FREE(cert->priv);
    TLS_FREE(cert->der_bytes);
    TLS_FREE(cert->bytes);
    TLS_FREE(cert->fingerprint);
    TLS_FREE(cert);
  }
}

struct TLSPacket *tls_create_packet(struct TLSContext *context,
                                    unsigned char type, unsigned short version,
                                    int payload_size_hint) {
  struct TLSPacket *packet =
      (struct TLSPacket *)TLS_MALLOC(sizeof(struct TLSPacket));
  if (!packet) return NULL;
  packet->broken = 0;
  if (payload_size_hint > 0)
    packet->size = payload_size_hint + 10;
  else
    packet->size = TLS_BLOB_INCREMENT;
  packet->buf = (unsigned char *)TLS_MALLOC(packet->size);
  packet->context = context;
  if (!packet->buf) {
    TLS_FREE(packet);
    return NULL;
  }
  if ((context) && (context->dtls))
    packet->len = 13;
  else
    packet->len = 5;
  packet->buf[0] = type;
  switch (version) {
    case TLS_V13:
      // check if context is not null. If null, is a tls_export_context call
      if (context)
        *(unsigned short *)(packet->buf + 1) =
            0x0303;  // no need to reorder (same bytes)
      else
        *(unsigned short *)(packet->buf + 1) = htons(version);
      break;
    case DTLS_V13:
      *(unsigned short *)(packet->buf + 1) = htons(DTLS_V13);
      break;
    default:
      *(unsigned short *)(packet->buf + 1) = htons(version);
  }
  return packet;
}

void tls_destroy_packet(struct TLSPacket *packet) {
  if (packet) {
    if (packet->buf) TLS_FREE(packet->buf);
    TLS_FREE(packet);
  }
}

int _private_tls_crypto_create(struct TLSContext *context, int key_length,
                               unsigned char *localkey, unsigned char *localiv,
                               unsigned char *remotekey,
                               unsigned char *remoteiv) {
  if (context->crypto.created) {
    // if (context->crypto.created == 1) {
    //   cbc_done(&context->crypto.ctx_remote.aes_remote);
    //   cbc_done(&context->crypto.ctx_local.aes_local);
    // } else {
    if (context->crypto.created == 2) {
      unsigned char dummy_buffer[32];
      unsigned long tag_len = 0;
      gcm_done(&context->crypto.ctx_remote.aes_gcm_remote, dummy_buffer,
               &tag_len);
      gcm_done(&context->crypto.ctx_local.aes_gcm_local, dummy_buffer,
               &tag_len);
    }
    // }
    context->crypto.created = 0;
  }
  tls_init();
  int is_aead = _private_tls_is_aead(context);
  int cipherID = find_cipher("aes");
  DEBUG_PRINT("Using cipher ID: %x\n", (int)context->cipher);
  if (is_aead == 2) {
    unsigned int counter = 1;

    chacha_keysetup(&context->crypto.ctx_local.chacha_local, localkey,
                    key_length * 8);
    chacha_ivsetup_96bitnonce(&context->crypto.ctx_local.chacha_local, localiv,
                              (unsigned char *)&counter);

    chacha_keysetup(&context->crypto.ctx_remote.chacha_remote, remotekey,
                    key_length * 8);
    chacha_ivsetup_96bitnonce(&context->crypto.ctx_remote.chacha_remote,
                              remoteiv, (unsigned char *)&counter);

    context->crypto.created = 3;
  } else if (is_aead) {
    int res1 = gcm_init(&context->crypto.ctx_local.aes_gcm_local, cipherID,
                        localkey, key_length);
    int res2 = gcm_init(&context->crypto.ctx_remote.aes_gcm_remote, cipherID,
                        remotekey, key_length);

    if ((res1) || (res2)) return TLS_GENERIC_ERROR;
    context->crypto.created = 2;
  }
  // else {
  //   int res1 = cbc_start(cipherID, localiv, localkey, key_length, 0,
  //                        &context->crypto.ctx_local.aes_local);
  //   int res2 = cbc_start(cipherID, remoteiv, remotekey, key_length, 0,
  //                        &context->crypto.ctx_remote.aes_remote);

  //   if ((res1) || (res2)) return TLS_GENERIC_ERROR;
  //   context->crypto.created = 1;
  // }
  return 0;
}

// int _private_tls_crypto_encrypt(struct TLSContext *context, unsigned char
// *buf,
//                                unsigned char *ct, unsigned int len) {
//  // if (context->crypto.created == 1)
//  //   return cbc_encrypt(buf, ct, len, &context->crypto.ctx_local.aes_local);
//
//  memset(ct, 0, len);
//  return TLS_GENERIC_ERROR;
//}
//
// int _private_tls_crypto_decrypt(struct TLSContext *context, unsigned char
// *buf,
//                                unsigned char *pt, unsigned int len) {
//  // if (context->crypto.created == 1)
//  //   return cbc_decrypt(buf, pt, len,
//  &context->crypto.ctx_remote.aes_remote);
//
//  memset(pt, 0, len);
//  return TLS_GENERIC_ERROR;
//}

void _private_tls_crypto_done(struct TLSContext *context) {
  unsigned char dummy_buffer[32];
  unsigned long tag_len = 0;
  switch (context->crypto.created) {
    // case 1:
    //   cbc_done(&context->crypto.ctx_remote.aes_remote);
    //   cbc_done(&context->crypto.ctx_local.aes_local);
    //   break;
    case 2:
      gcm_done(&context->crypto.ctx_remote.aes_gcm_remote, dummy_buffer,
               &tag_len);
      gcm_done(&context->crypto.ctx_local.aes_gcm_local, dummy_buffer,
               &tag_len);
      break;
  }
  context->crypto.created = 0;
}

void tls_packet_update(struct TLSPacket *packet) {
  if ((packet) && (!packet->broken)) {
    int footer_size = 0;
    if ((packet->context) &&
        ((packet->context->version == TLS_V13) ||
         (packet->context->version == DTLS_V13)) &&
        (packet->context->cipher_spec_set) &&
        (packet->context->crypto.created)) {
      // type
      tls_packet_uint8(packet, packet->buf[0]);
      // no padding
      // tls_packet_uint8(packet, 0);
      footer_size = 1;
    }
    unsigned int header_size = 5;
    if ((packet->context) && (packet->context->dtls)) {
      header_size = 13;
      *(unsigned short *)(packet->buf + 3) =
          htons(packet->context->dtls_epoch_local);
      uint64_t sequence_number = packet->context->local_sequence_number;
      packet->buf[5] = (unsigned char)(sequence_number / 0x10000000000LL);
      sequence_number %= 0x10000000000LL;
      packet->buf[6] = (unsigned char)(sequence_number / 0x100000000LL);
      sequence_number %= 0x100000000LL;
      packet->buf[7] = (unsigned char)(sequence_number / 0x1000000);
      sequence_number %= 0x1000000;
      packet->buf[8] = (unsigned char)(sequence_number / 0x10000);
      sequence_number %= 0x10000;
      packet->buf[9] = (unsigned char)(sequence_number / 0x100);
      sequence_number %= 0x100;
      packet->buf[10] = (unsigned char)sequence_number;

      *(unsigned short *)(packet->buf + 11) = htons(packet->len - header_size);
    } else
      *(unsigned short *)(packet->buf + 3) = htons(packet->len - header_size);
    if (packet->context) {
      if (packet->buf[0] != TLS_CHANGE_CIPHER) {
        if ((packet->buf[0] == TLS_HANDSHAKE) && (packet->len > header_size)) {
          unsigned char handshake_type = packet->buf[header_size];
          if ((handshake_type != 0x00) && (handshake_type != 0x03))
            _private_tls_update_hash(packet->context, packet->buf + header_size,
                                     packet->len - header_size - footer_size);
        }
        if (((packet->context->cipher_spec_set) ||
             (packet->context->false_start)) &&
            (packet->context->crypto.created)) {
          int block_size = TLS_AES_BLOCK_SIZE;
          int mac_size = 0;
          unsigned int length = 0;
          unsigned char padding = 0;
          unsigned int pt_length = packet->len - header_size;

          if (packet->context->crypto.created == 1) {
            mac_size = _private_tls_mac_length(packet->context);
            if (packet->context->version == TLS_V10)
              length = packet->len - header_size + mac_size;
            else
              length = packet->len - header_size + TLS_AES_IV_LENGTH + mac_size;
            padding = block_size - length % block_size;
            length += padding;
          } else if (packet->context->crypto.created == 3) {
            mac_size = POLY1305_TAGLEN;
            length = packet->len - header_size + mac_size;
          } else {
            mac_size = TLS_GCM_TAG_LEN;
            length = packet->len - header_size + 8 + mac_size;
          }
          if (packet->context->crypto.created == 1) {
            unsigned char *buf = (unsigned char *)TLS_MALLOC(length);
            if (buf) {
              unsigned char *ct =
                  (unsigned char *)TLS_MALLOC(length + header_size);
              if (ct) {
                unsigned int buf_pos = 0;
                memcpy(ct, packet->buf, header_size - 2);
                *(unsigned short *)&ct[header_size - 2] = htons(length);
                if (packet->context->version != TLS_V10) {
                  tls_random(buf, TLS_AES_IV_LENGTH);
                  buf_pos += TLS_AES_IV_LENGTH;
                }
                // copy payload
                memcpy(buf + buf_pos, packet->buf + header_size,
                       packet->len - header_size);
                buf_pos += packet->len - header_size;
                if (packet->context->dtls) {
                  unsigned char temp_buf[5];
                  memcpy(temp_buf, packet->buf, 3);
                  *(unsigned short *)(temp_buf + 3) =
                      *(unsigned short *)&packet->buf[header_size - 2];
                  uint64_t dtls_sequence_number =
                      ntohll(*(uint64_t *)&packet->buf[3]);
                  _private_tls_hmac_message(
                      1, packet->context, temp_buf, 5,
                      packet->buf + header_size, packet->len - header_size,
                      buf + buf_pos, mac_size, dtls_sequence_number);
                } else
                  _private_tls_hmac_message(1, packet->context, packet->buf,
                                            packet->len, NULL, 0, buf + buf_pos,
                                            mac_size, 0);
                buf_pos += mac_size;

                memset(buf + buf_pos, padding - 1, padding);
                buf_pos += padding;

                // DEBUG_DUMP_HEX_LABEL("PT BUFFER", buf, length);
                //                _private_tls_crypto_encrypt(packet->context,
                //                buf,
                //                                            ct + header_size,
                //                                            length);
                TLS_FREE(packet->buf);
                packet->buf = ct;
                packet->len = length + header_size;
                packet->size = packet->len;
              } else {
                // invalidate packet
                memset(packet->buf, 0, packet->len);
              }
              TLS_FREE(buf);
            } else {
              // invalidate packet
              memset(packet->buf, 0, packet->len);
            }
          } else if (packet->context->crypto.created >= 2) {
            // + 1 = type
            int ct_size = length + header_size + 12 + TLS_MAX_TAG_LEN + 1;
            unsigned char *ct = (unsigned char *)TLS_MALLOC(ct_size);
            if (ct) {
              memset(ct, 0, ct_size);
              // AEAD
              // sequence number (8 bytes)
              // content type (1 byte)
              // version (2 bytes)
              // length (2 bytes)
              unsigned char aad[13];
              int aad_size = sizeof(aad);
              unsigned char *sequence = aad;
              if ((packet->context->version == TLS_V13) ||
                  (packet->context->version == DTLS_V13)) {
                aad[0] = TLS_APPLICATION_DATA;
                aad[1] = packet->buf[1];
                aad[2] = packet->buf[2];
                if (packet->context->crypto.created == 3)
                  *((unsigned short *)(aad + 3)) =
                      htons(packet->len + POLY1305_TAGLEN - header_size);
                else
                  *((unsigned short *)(aad + 3)) =
                      htons(packet->len + TLS_GCM_TAG_LEN - header_size);
                aad_size = 5;
                sequence = aad + 5;
                if (packet->context->dtls)
                  *((uint64_t *)sequence) = *(uint64_t *)&packet->buf[3];
                else
                  *((uint64_t *)sequence) =
                      htonll(packet->context->local_sequence_number);
              } else {
                if (packet->context->dtls)
                  *((uint64_t *)aad) = *(uint64_t *)&packet->buf[3];
                else
                  *((uint64_t *)aad) =
                      htonll(packet->context->local_sequence_number);
                aad[8] = packet->buf[0];
                aad[9] = packet->buf[1];
                aad[10] = packet->buf[2];
                *((unsigned short *)(aad + 11)) =
                    htons(packet->len - header_size);
              }
              int ct_pos = header_size;
              if (packet->context->crypto.created == 3) {
                unsigned int counter = 1;
                unsigned char poly1305_key[POLY1305_KEYLEN];
                chacha_ivupdate(
                    &packet->context->crypto.ctx_local.chacha_local,
                    packet->context->crypto.ctx_local_mac.local_aead_iv,
                    sequence, (u8 *)&counter);
                chacha20_poly1305_key(
                    &packet->context->crypto.ctx_local.chacha_local,
                    poly1305_key);
                ct_pos += chacha20_poly1305_aead(
                    &packet->context->crypto.ctx_local.chacha_local,
                    packet->buf + header_size, pt_length, aad, aad_size,
                    poly1305_key, ct + ct_pos);
              } else {
                unsigned char iv[TLS_13_AES_GCM_IV_LENGTH];
                if ((packet->context->version == TLS_V13) ||
                    (packet->context->version == DTLS_V13)) {
                  memcpy(iv, packet->context->crypto.ctx_local_mac.local_iv,
                         TLS_13_AES_GCM_IV_LENGTH);
                  int i;
                  int offset = TLS_13_AES_GCM_IV_LENGTH - 8;
                  for (i = 0; i < 8; i++)
                    iv[offset + i] = packet->context->crypto.ctx_local_mac
                                         .local_iv[offset + i] ^
                                     sequence[i];
                } else {
                  memcpy(iv,
                         packet->context->crypto.ctx_local_mac.local_aead_iv,
                         TLS_AES_GCM_IV_LENGTH);
                  tls_random(iv + TLS_AES_GCM_IV_LENGTH, 8);
                  memcpy(ct + ct_pos, iv + TLS_AES_GCM_IV_LENGTH, 8);
                  ct_pos += 8;
                }

                gcm_reset(&packet->context->crypto.ctx_local.aes_gcm_local);
                gcm_add_iv(&packet->context->crypto.ctx_local.aes_gcm_local, iv,
                           12);
                gcm_add_aad(&packet->context->crypto.ctx_local.aes_gcm_local,
                            aad, aad_size);
                gcm_process(&packet->context->crypto.ctx_local.aes_gcm_local,
                            packet->buf + header_size, pt_length, ct + ct_pos,
                            GCM_ENCRYPT);
                ct_pos += pt_length;

                unsigned long taglen = TLS_GCM_TAG_LEN;
                gcm_done(&packet->context->crypto.ctx_local.aes_gcm_local,
                         ct + ct_pos, &taglen);
                ct_pos += taglen;
              }
              if ((packet->context->version == TLS_V13) ||
                  (packet->context->version == DTLS_V13)) {
                ct[0] = TLS_APPLICATION_DATA;
                *(unsigned short *)&ct[1] = htons(
                    packet->context->version == TLS_V13 ? TLS_V12 : DTLS_V12);
                // is dtls ?
                if (header_size != 5)
                  memcpy(ct, packet->buf + 3, header_size - 2);
              } else
                memcpy(ct, packet->buf, header_size - 2);
              *(unsigned short *)&ct[header_size - 2] =
                  htons(ct_pos - header_size);
              TLS_FREE(packet->buf);
              packet->buf = ct;
              packet->len = ct_pos;
              packet->size = ct_pos;
            } else {
              // invalidate packet
              memset(packet->buf, 0, packet->len);
            }
          } else {
            // invalidate packet (never reached)
            memset(packet->buf, 0, packet->len);
          }
        }
      } else
        packet->context->dtls_epoch_local++;
      packet->context->local_sequence_number++;
    }
  }
}

int tls_packet_append(struct TLSPacket *packet, const unsigned char *buf,
                      unsigned int len) {
  if ((!packet) || (packet->broken)) return -1;

  if (!len) return 0;

  unsigned int new_len = packet->len + len;

  if (new_len > packet->size) {
    packet->size = (new_len / TLS_BLOB_INCREMENT + 1) * TLS_BLOB_INCREMENT;
    packet->buf = (unsigned char *)TLS_REALLOC(packet->buf, packet->size);
    if (!packet->buf) {
      packet->size = 0;
      packet->len = 0;
      packet->broken = 1;
      return -1;
    }
  }
  memcpy(packet->buf + packet->len, buf, len);
  packet->len = new_len;
  return new_len;
}

int tls_packet_uint8(struct TLSPacket *packet, unsigned char i) {
  return tls_packet_append(packet, &i, 1);
}

int tls_packet_uint16(struct TLSPacket *packet, unsigned short i) {
  unsigned short ni = htons(i);
  return tls_packet_append(packet, (unsigned char *)&ni, 2);
}

int tls_packet_uint24(struct TLSPacket *packet, unsigned int i) {
  unsigned char buf[3];
  buf[0] = i / 0x10000;
  i %= 0x10000;
  buf[1] = i / 0x100;
  i %= 0x100;
  buf[2] = i;

  return tls_packet_append(packet, buf, 3);
}

int tls_random(unsigned char *key, unsigned long len) {
  FILE *fp = fopen("/dev/urandom", "r");
  if (fp) {
    int key_len = fread(key, 1, len, fp);
    fclose(fp);
    if (key_len == len) return 1;
  }
  return 0;
}

TLSHash *_private_tls_ensure_hash(struct TLSContext *context) {
  TLSHash *hash = context->handshake_hash;
  if (!hash) {
    hash = (TLSHash *)TLS_MALLOC(sizeof(TLSHash));
    if (hash) memset(hash, 0, sizeof(TLSHash));
    context->handshake_hash = hash;
  }
  return hash;
}

void _private_tls_destroy_hash(struct TLSContext *context) {
  if (context) {
    TLS_FREE(context->handshake_hash);
    context->handshake_hash = NULL;
  }
}

void _private_tls_create_hash(struct TLSContext *context) {
  if (!context) return;

  TLSHash *hash = _private_tls_ensure_hash(context);
  if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
      (context->version == TLS_V13) || (context->version == DTLS_V13)) {
    int hash_size = _private_tls_mac_length(context);
    if (hash->created) {
      unsigned char temp[TLS_MAX_SHA_SIZE];
      if (hash_size == TLS_SHA384_MAC_SIZE)
        sha384_done(&hash->hash, temp);
      else
        sha256_done(&hash->hash, temp);
    }
    if (hash_size == TLS_SHA384_MAC_SIZE)
      sha384_init(&hash->hash);
    else
      sha256_init(&hash->hash);
    hash->created = 1;
  } else {
    // TLS_V11
    if (hash->created) {
      unsigned char temp[TLS_V11_HASH_SIZE];
      md5_done(&hash->hash, temp);
      sha1_done(&hash->hash2, temp);
    }
    md5_init(&hash->hash);
    sha1_init(&hash->hash2);
    hash->created = 1;
  }
}

int _private_tls_update_hash(struct TLSContext *context,
                             const unsigned char *in, unsigned int len) {
  if (!context) return 0;
  TLSHash *hash = _private_tls_ensure_hash(context);
  if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
      (context->version == TLS_V13) || (context->version == DTLS_V13)) {
    if (!hash->created) {
      _private_tls_create_hash(context);
      // cache first hello in case of protocol downgrade
      if ((!context->is_server) && (!context->cached_handshake) && (len)) {
        context->cached_handshake = (unsigned char *)TLS_MALLOC(len);
        if (context->cached_handshake) {
          memcpy(context->cached_handshake, in, len);
          context->cached_handshake_len = len;
        }
      }
    }
    int hash_size = _private_tls_mac_length(context);
    if (hash_size == TLS_SHA384_MAC_SIZE) {
      sha384_process(&hash->hash, in, len);
    } else {
      sha256_process(&hash->hash, in, len);
      hash_size = TLS_SHA256_MAC_SIZE;
    }
  } else {
    if (!hash->created) _private_tls_create_hash(context);
    md5_process(&hash->hash, in, len);
    sha1_process(&hash->hash2, in, len);
  }

  return 0;
}

int _private_tls_done_hash(struct TLSContext *context, unsigned char *hout) {
  if (!context) return 0;

  TLSHash *hash = _private_tls_ensure_hash(context);
  if (!hash->created) return 0;

  int hash_size = 0;
  if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
      (context->version == TLS_V13) || (context->version == DTLS_V13)) {
    unsigned char temp[TLS_MAX_SHA_SIZE];
    if (!hout) hout = temp;
    // TLS_HASH_DONE(&hash->hash, hout);
    hash_size = _private_tls_mac_length(context);
    if (hash_size == TLS_SHA384_MAC_SIZE)
      sha384_done(&hash->hash, hout);
    else {
      sha256_done(&hash->hash, hout);
      hash_size = TLS_SHA256_MAC_SIZE;
    }
  } else {
    // TLS_V11
    unsigned char temp[TLS_V11_HASH_SIZE];
    if (!hout) hout = temp;
    md5_done(&hash->hash, hout);
    sha1_done(&hash->hash2, hout + 16);
    hash_size = TLS_V11_HASH_SIZE;
  }
  hash->created = 0;
  if (context->cached_handshake) {
    // not needed anymore
    TLS_FREE(context->cached_handshake);
    context->cached_handshake = NULL;
    context->cached_handshake_len = 0;
  }
  return hash_size;
}

int _private_tls_get_hash_idx(struct TLSContext *context) {
  if (!context) return -1;
  switch (_private_tls_mac_length(context)) {
    case TLS_SHA256_MAC_SIZE:
      return find_hash("sha256");
    case TLS_SHA384_MAC_SIZE:
      return find_hash("sha384");
    case TLS_SHA1_MAC_SIZE:
      return find_hash("sha1");
  }
  return -1;
}

int _private_tls_get_hash(struct TLSContext *context, unsigned char *hout) {
  if (!context) return 0;

  TLSHash *hash = _private_tls_ensure_hash(context);
  if (!hash->created) return 0;

  int hash_size = 0;
  if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
      (context->version == TLS_V13) || (context->version == DTLS_V13)) {
    hash_size = _private_tls_mac_length(context);
    hash_state prec;
    memcpy(&prec, &hash->hash, sizeof(hash_state));
    if (hash_size == TLS_SHA384_MAC_SIZE)
      sha384_done(&hash->hash, hout);
    else {
      hash_size = TLS_SHA256_MAC_SIZE;
      sha256_done(&hash->hash, hout);
    }
    memcpy(&hash->hash, &prec, sizeof(hash_state));
  } else {
    // TLS_V11
    hash_state prec;

    memcpy(&prec, &hash->hash, sizeof(hash_state));
    md5_done(&hash->hash, hout);
    memcpy(&hash->hash, &prec, sizeof(hash_state));

    memcpy(&prec, &hash->hash2, sizeof(hash_state));
    sha1_done(&hash->hash2, hout + 16);
    memcpy(&hash->hash2, &prec, sizeof(hash_state));

    hash_size = TLS_V11_HASH_SIZE;
  }
  return hash_size;
}

int _private_tls_write_packet(struct TLSPacket *packet) {
  if (!packet) return -1;
  struct TLSContext *context = packet->context;
  if (!context) return -1;

  if (context->tls_buffer) {
    int len = context->tls_buffer_len + packet->len;
    context->tls_buffer =
        (unsigned char *)TLS_REALLOC(context->tls_buffer, len);
    if (!context->tls_buffer) {
      context->tls_buffer_len = 0;
      return -1;
    }
    memcpy(context->tls_buffer + context->tls_buffer_len, packet->buf,
           packet->len);
    context->tls_buffer_len = len;
    int written = packet->len;
    tls_destroy_packet(packet);
    return written;
  }
  context->tls_buffer_len = packet->len;
  context->tls_buffer = packet->buf;
  packet->buf = NULL;
  packet->len = 0;
  packet->size = 0;
  tls_destroy_packet(packet);
  return context->tls_buffer_len;
}

int _private_tls_write_app_data(struct TLSContext *context,
                                const unsigned char *buf,
                                unsigned int buf_len) {
  if (!context) return -1;
  if ((!buf) || (!buf_len)) return 0;

  int len = context->application_buffer_len + buf_len;
  context->application_buffer =
      (unsigned char *)TLS_REALLOC(context->application_buffer, len);
  if (!context->application_buffer) {
    context->application_buffer_len = 0;
    return -1;
  }
  memcpy(context->application_buffer + context->application_buffer_len, buf,
         buf_len);
  context->application_buffer_len = len;
  return buf_len;
}

const unsigned char *tls_get_write_buffer(struct TLSContext *context,
                                          unsigned int *outlen) {
  if (!outlen) return NULL;
  if (!context) {
    *outlen = 0;
    return NULL;
  }
  // check if any error
  if (context->sleep_until) {
    if (context->sleep_until < time(NULL)) {
      *outlen = 0;
      return NULL;
    }
    context->sleep_until = 0;
  }
  *outlen = context->tls_buffer_len;
  return context->tls_buffer;
}

// const unsigned char *tls_get_message(struct TLSContext *context,
//                                     unsigned int *outlen,
//                                     unsigned int offset) {
//  if (!outlen) return NULL;
//  if ((!context) || (!context->tls_buffer)) {
//    *outlen = 0;
//    return NULL;
//  }
//
//  if (offset >= context->tls_buffer_len) {
//    *outlen = 0;
//    return NULL;
//  }
//  // check if any error
//  if (context->sleep_until) {
//    if (context->sleep_until < time(NULL)) {
//      *outlen = 0;
//      return NULL;
//    }
//    context->sleep_until = 0;
//  }
//  unsigned char *tls_buffer = &context->tls_buffer[offset];
//  unsigned int tls_buffer_len = context->tls_buffer_len - offset;
//  unsigned int len = 0;
//  if (context->dtls) {
//    if (tls_buffer_len < 13) {
//      *outlen = 0;
//      return NULL;
//    }
//
//    len = ntohs(*(unsigned short *)&tls_buffer[11]) + 13;
//  } else {
//    if (tls_buffer_len < 5) {
//      *outlen = 0;
//      return NULL;
//    }
//    len = ntohs(*(unsigned short *)&tls_buffer[3]) + 5;
//  }
//  if (len > tls_buffer_len) {
//    *outlen = 0;
//    return NULL;
//  }
//
//  *outlen = len;
//  return tls_buffer;
//}

void tls_buffer_clear(struct TLSContext *context) {
  if ((context) && (context->tls_buffer)) {
    TLS_FREE(context->tls_buffer);
    context->tls_buffer = NULL;
    context->tls_buffer_len = 0;
  }
}

int tls_established(struct TLSContext *context) {
  if (context) {
    if (context->critical_error) return -1;

    if (context->connection_status == 0xFF) return 1;

    // allow false start
    if ((!context->is_server) && (context->version == TLS_V12) &&
        (context->false_start))
      return 1;
  }
  return 0;
}

int tls_read(struct TLSContext *context, unsigned char *buf,
             unsigned int size) {
  if (!context) return -1;
  if ((context->application_buffer) && (context->application_buffer_len)) {
    if (context->application_buffer_len < size)
      size = context->application_buffer_len;

    memcpy(buf, context->application_buffer, size);
    if (context->application_buffer_len == size) {
      TLS_FREE(context->application_buffer);
      context->application_buffer = NULL;
      context->application_buffer_len = 0;
      return size;
    }
    context->application_buffer_len -= size;
    memmove(context->application_buffer, context->application_buffer + size,
            context->application_buffer_len);
    return size;
  }
  return 0;
}

struct TLSContext *tls_create_context(unsigned char is_server,
                                      unsigned short version) {
  struct TLSContext *context =
      (struct TLSContext *)TLS_MALLOC(sizeof(struct TLSContext));
  if (context) {
    memset(context, 0, sizeof(struct TLSContext));
    context->is_server = is_server;
    if ((version == DTLS_V13) || (version == DTLS_V12) || (version == DTLS_V10))
      context->dtls = 1;
    context->version = version;
  }
  return context;
}

struct TLSContext *tls_accept(struct TLSContext *context) {
  if ((!context) || (!context->is_server)) return NULL;

  struct TLSContext *child =
      (struct TLSContext *)TLS_MALLOC(sizeof(struct TLSContext));
  if (child) {
    memset(child, 0, sizeof(struct TLSContext));
    child->is_server = 1;
    child->is_child = 1;
    child->dtls = context->dtls;
    child->version = context->version;
    child->certificates = context->certificates;
    child->certificates_count = context->certificates_count;
    child->private_key = context->private_key;
    child->ec_private_key = context->ec_private_key;
    child->exportable = context->exportable;
    child->root_certificates = context->root_certificates;
    child->root_count = context->root_count;
    child->default_dhe_p = context->default_dhe_p;
    child->default_dhe_g = context->default_dhe_g;
    child->curve = context->curve;
    child->alpn = context->alpn;
    child->alpn_count = context->alpn_count;
  }
  return child;
}

void _private_tls_dhe_free(struct TLSContext *context) {
  if (context->dhe) {
    _private_tls_dh_clear_key(context->dhe);
    TLS_FREE(context->dhe);
    context->dhe = NULL;
  }
}

void _private_tls_dhe_create(struct TLSContext *context) {
  _private_tls_dhe_free(context);
  context->dhe = (DHKey *)TLS_MALLOC(sizeof(DHKey));
  if (context->dhe) memset(context->dhe, 0, sizeof(DHKey));
}

void _private_tls_ecc_dhe_free(struct TLSContext *context) {
  if (context->ecc_dhe) {
    ecc_free(context->ecc_dhe);
    TLS_FREE(context->ecc_dhe);
    context->ecc_dhe = NULL;
  }
}

void _private_tls_ecc_dhe_create(struct TLSContext *context) {
  _private_tls_ecc_dhe_free(context);
  context->ecc_dhe = (ecc_key *)TLS_MALLOC(sizeof(ecc_key));
  memset(context->ecc_dhe, 0, sizeof(ecc_key));
}

int tls_alpn_contains(struct TLSContext *context, const char *alpn,
                      unsigned char alpn_size) {
  if ((!context) || (!alpn) || (!alpn_size)) return 0;

  if (context->alpn) {
    int i;
    for (i = 0; i < context->alpn_count; i++) {
      const char *alpn_local = context->alpn[i];
      if (alpn_local) {
        int len = strlen(alpn_local);
        if (alpn_size == len) {
          if (!memcmp(alpn_local, alpn, alpn_size)) return 1;
        }
      }
    }
  }
  return 0;
}

void tls_destroy_context(struct TLSContext *context) {
  unsigned int i;
  if (!context) return;
  if (!context->is_child) {
    if (context->certificates) {
      for (i = 0; i < context->certificates_count; i++)
        tls_destroy_certificate(context->certificates[i]);
    }
    if (context->root_certificates) {
      for (i = 0; i < context->root_count; i++)
        tls_destroy_certificate(context->root_certificates[i]);
      TLS_FREE(context->root_certificates);
      context->root_certificates = NULL;
    }
    if (context->private_key) tls_destroy_certificate(context->private_key);
    if (context->ec_private_key)
      tls_destroy_certificate(context->ec_private_key);
    TLS_FREE(context->certificates);
    TLS_FREE(context->default_dhe_p);
    TLS_FREE(context->default_dhe_g);
    if (context->alpn) {
      for (i = 0; i < context->alpn_count; i++) TLS_FREE(context->alpn[i]);
      TLS_FREE(context->alpn);
    }
  }

  TLS_FREE(context->master_key);
  TLS_FREE(context->premaster_key);
  if (context->crypto.created) _private_tls_crypto_done(context);
  TLS_FREE(context->message_buffer);
  _private_tls_done_hash(context, NULL);
  _private_tls_destroy_hash(context);
  TLS_FREE(context->tls_buffer);
  TLS_FREE(context->application_buffer);
  // zero out the keys before free
  if ((context->exportable_keys) && (context->exportable_size))
    memset(context->exportable_keys, 0, context->exportable_size);
  TLS_FREE(context->exportable_keys);
  TLS_FREE(context->sni);
  TLS_FREE(context->dtls_cookie);
  TLS_FREE(context->cached_handshake);
  _private_tls_dhe_free(context);
  _private_tls_ecc_dhe_free(context);
  TLS_FREE(context->negotiated_alpn);
  TLS_FREE(context->finished_key);
  TLS_FREE(context->remote_finished_key);
  TLS_FREE(context->server_finished_hash);
  TLS_FREE(context->client_secret);
  TLS_FREE(context);
}

int tls_cipher_supported(struct TLSContext *context, unsigned short cipher) {
  if (!context) return 0;

  // 128 bit only included so we can talk to servers that require it
  // if we are a server, set all 128 to invalid cipher IDs

  switch (cipher) {
    case TLS_AES_128_GCM_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_AES_256_GCM_SHA384:
    case TLS_CHACHA20_POLY1305_SHA256:
      if ((context->version == TLS_V13) || (context->version == DTLS_V13))
        return 1;
      return 0;
    case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
      if ((context->version == TLS_V12) || (context->version == DTLS_V12)) {
        if ((context) &&
            (((context->certificates) && (context->certificates_count) &&
              (context->ec_private_key)) ||
             (!context->is_server)))
          return 1;
      }
      return 0;
    case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_RSA_WITH_AES_128_GCM_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_RSA_WITH_AES_256_GCM_SHA384:
      if ((context->version == TLS_V12) || (context->version == DTLS_V12))
        return 1;
      return 0;
  }
  return 0;
}

int tls_cipher_is_fs(struct TLSContext *context, unsigned short cipher) {
  if (!context) return 0;
  if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
    switch (cipher) {
      case TLS_AES_128_GCM_SHA256:
        if (context->is_server) return 0;
        // fall through
      case TLS_AES_256_GCM_SHA384:
      case TLS_CHACHA20_POLY1305_SHA256:
        return 1;
    }
    return 0;
  }
  switch (cipher) {
    // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:
    // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:
    //   if (context->is_server) return 0;
    //   // fall through
    case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
      if ((context) && (context->certificates) &&
          (context->certificates_count) && (context->ec_private_key))
        return 1;
      return 0;
      // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
      // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
    case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
      if ((context->version == TLS_V12) || (context->version == DTLS_V12)) {
        if ((context) && (context->certificates) &&
            (context->certificates_count) && (context->ec_private_key))
          return 1;
      }
      return 0;
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA:
      // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:
      // case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:
      //   if (context->is_server) return 0;
      //   return 1;
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA256:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA256:
    case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
      // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
      if (context->is_server) return 0;
      // fall through
    case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
    case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
    case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
      if ((context->version == TLS_V12) || (context->version == DTLS_V12))
        return 1;
      break;
  }
  return 0;
}

// int _private_tls_prefer_ktls(struct TLSContext *context,
//                              unsigned short cipher) {
//   if ((context->version == TLS_V13) || (context->version == DTLS_V13) ||
//       ((context->version != TLS_V12) && (context->version != DTLS_V12)))
//     return 0;

//   switch (cipher) {
//     case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
//       if ((context->version == TLS_V13) || (context->version == DTLS_V13) ||
//           (context->version == TLS_V12) || (context->version == DTLS_V12)) {
//         if ((context->certificates) && (context->certificates_count) &&
//             (context->ec_private_key))
//           return 1;
//       }
//       break;
//     case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
//     case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
//       return 1;
//   }
//   return 0;
// }

int tls_choose_cipher(struct TLSContext *context, const unsigned char *buf,
                      int buf_len, int *scsv_set) {
  int i;
  if (scsv_set) *scsv_set = 0;
  if (!context) return 0;
  int selected_cipher = TLS_NO_COMMON_CIPHER;
  // for (i = 0; i < buf_len; i += 2) {
  //   unsigned short cipher = ntohs(*(unsigned short *)&buf[i]);
  //   if (_private_tls_prefer_ktls(context, cipher)) {
  //     selected_cipher = cipher;
  //     break;
  //   }
  // }
  if (selected_cipher == TLS_NO_COMMON_CIPHER) {
    for (i = 0; i < buf_len; i += 2) {
      unsigned short cipher = ntohs(*(unsigned short *)&buf[i]);
      if (tls_cipher_is_fs(context, cipher)) {
        selected_cipher = cipher;
        break;
      }
    }
  }
  for (i = 0; i < buf_len; i += 2) {
    unsigned short cipher = ntohs(*(unsigned short *)&buf[i]);
    if (cipher == TLS_FALLBACK_SCSV) {
      if (scsv_set) *scsv_set = 1;
      if (selected_cipher != TLS_NO_COMMON_CIPHER) break;
    }
  }
  return selected_cipher;
}

int tls_cipher_is_ephemeral(struct TLSContext *context) {
  if (context) {
    switch (context->cipher) {
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA:
      // case TLS_DHE_RSA_WITH_AES_128_CBC_SHA256:
      // case TLS_DHE_RSA_WITH_AES_256_CBC_SHA256:
      case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
      case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
      case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
        return 1;
        // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:
        // case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:
        // case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
      case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
      case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
      case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
        // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:
        // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:
        // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
        // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
      case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
      case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
      case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
        return 2;
      case TLS_AES_128_GCM_SHA256:
      case TLS_CHACHA20_POLY1305_SHA256:
      case TLS_AES_128_CCM_SHA256:
      case TLS_AES_128_CCM_8_SHA256:
      case TLS_AES_256_GCM_SHA384:
        if (context->dhe) return 1;
        return 2;
    }
  }
  return 0;
}

const char *tls_cipher_name(struct TLSContext *context) {
  if (context) {
    switch (context->cipher) {
      case TLS_RSA_WITH_AES_128_CBC_SHA:
        return "RSA-AES128CBC-SHA";
      case TLS_RSA_WITH_AES_256_CBC_SHA:
        return "RSA-AES256CBC-SHA";
      case TLS_RSA_WITH_AES_128_CBC_SHA256:
        return "RSA-AES128CBC-SHA256";
      case TLS_RSA_WITH_AES_256_CBC_SHA256:
        return "RSA-AES256CBC-SHA256";
      case TLS_RSA_WITH_AES_128_GCM_SHA256:
        return "RSA-AES128GCM-SHA256";
      case TLS_RSA_WITH_AES_256_GCM_SHA384:
        return "RSA-AES256GCM-SHA384";
      case TLS_DHE_RSA_WITH_AES_128_CBC_SHA:
        return "DHE-RSA-AES128CBC-SHA";
      case TLS_DHE_RSA_WITH_AES_256_CBC_SHA:
        return "DHE-RSA-AES256CBC-SHA";
      case TLS_DHE_RSA_WITH_AES_128_CBC_SHA256:
        return "DHE-RSA-AES128CBC-SHA256";
      case TLS_DHE_RSA_WITH_AES_256_CBC_SHA256:
        return "DHE-RSA-AES256CBC-SHA256";
      case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
        return "DHE-RSA-AES128GCM-SHA256";
      case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
        return "DHE-RSA-AES256GCM-SHA384";
      case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:
        return "ECDHE-RSA-AES128CBC-SHA";
      case TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:
        return "ECDHE-RSA-AES256CBC-SHA";
      case TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:
        return "ECDHE-RSA-AES128CBC-SHA256";
      case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
        return "ECDHE-RSA-AES128GCM-SHA256";
      case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
        return "ECDHE-RSA-AES256GCM-SHA384";
      case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:
        return "ECDHE-ECDSA-AES128CBC-SHA";
      case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:
        return "ECDHE-ECDSA-AES256CBC-SHA";
      case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
        return "ECDHE-ECDSA-AES128CBC-SHA256";
      case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
        return "ECDHE-ECDSA-AES256CBC-SHA384";
      case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
        return "ECDHE-ECDSA-AES128GCM-SHA256";
      case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
        return "ECDHE-ECDSA-AES256GCM-SHA384";
      case TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
        return "ECDHE-RSA-CHACHA20-POLY1305-SHA256";
      case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
        return "ECDHE-ECDSA-CHACHA20-POLY1305-SHA256";
      case TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256:
        return "ECDHE-DHE-CHACHA20-POLY1305-SHA256";
      case TLS_AES_128_GCM_SHA256:
        return "TLS-AES-128-GCM-SHA256";
      case TLS_AES_256_GCM_SHA384:
        return "TLS-AES-256-GCM-SHA384";
      case TLS_CHACHA20_POLY1305_SHA256:
        return "TLS-CHACHA20-POLY1305-SHA256";
      case TLS_AES_128_CCM_SHA256:
        return "TLS-AES-128-CCM-SHA256";
      case TLS_AES_128_CCM_8_SHA256:
        return "TLS-AES-128-CCM-8-SHA256";
    }
  }
  return "UNKNOWN";
}

int _private_tls_dh_export_Y(unsigned char *Ybuf, unsigned long *Ylen,
                             DHKey *key) {
  unsigned long len;

  if ((Ybuf == NULL) || (Ylen == NULL) || (key == NULL))
    return TLS_GENERIC_ERROR;

  len = mp_unsigned_bin_size(key->y);
  if (len > *Ylen) return TLS_GENERIC_ERROR;

  *Ylen = len;
  return 0;
}

int _private_tls_dh_export_pqY(unsigned char *pbuf, unsigned long *plen,
                               unsigned char *gbuf, unsigned long *glen,
                               unsigned char *Ybuf, unsigned long *Ylen,
                               DHKey *key) {
  unsigned long len;
  int err;

  if ((pbuf == NULL) || (plen == NULL) || (gbuf == NULL) || (glen == NULL) ||
      (Ybuf == NULL) || (Ylen == NULL) || (key == NULL))
    return TLS_GENERIC_ERROR;

  len = mp_unsigned_bin_size(key->y);
  if (len > *Ylen) return TLS_GENERIC_ERROR;

  if ((err = mp_to_unsigned_bin(key->y, Ybuf)) != CRYPT_OK) return err;

  *Ylen = len;

  len = mp_unsigned_bin_size(key->p);
  if (len > *plen) return TLS_GENERIC_ERROR;

  if ((err = mp_to_unsigned_bin(key->p, pbuf)) != CRYPT_OK) return err;

  *plen = len;

  len = mp_unsigned_bin_size(key->g);
  if (len > *glen) return TLS_GENERIC_ERROR;

  if ((err = mp_to_unsigned_bin(key->g, gbuf)) != CRYPT_OK) return err;

  *glen = len;

  return 0;
}

void _private_tls_dh_clear_key(DHKey *key) {
  mp_clear_multi(key->g, key->p, key->x, key->y, NULL);
  key->g = NULL;
  key->p = NULL;
  key->x = NULL;
  key->y = NULL;
}

int _private_tls_dh_make_key(int keysize, DHKey *key, const char *pbuf,
                             const char *gbuf, int pbuf_len, int gbuf_len) {
  unsigned char *buf;
  int err;
  if (!key) return TLS_GENERIC_ERROR;

  DEBUG_PRINT("DH_MAKE_KEY: START RNG\n");

  static prng_state prng;
  int wprng = find_prng("sprng");
  if ((err = prng_is_valid(wprng)) != CRYPT_OK) return err;

  DEBUG_PRINT("DH_MAKE_KEY: FOUND RNG (SPRNG)\n");

  buf = (unsigned char *)TLS_MALLOC(keysize);
  if (!buf) return TLS_NO_MEMORY;

  DEBUG_PRINT("DH_MAKE_KEY: RNG ALLOCED KSz\n");

  if (rng_make_prng(keysize, wprng, &prng, NULL) != CRYPT_OK) {
    DEBUG_PRINT("DH_MAKE_KEY: FAILED RNG CONS\n");
    TLS_FREE(buf);
    return TLS_GENERIC_ERROR;
  }

  DEBUG_PRINT("DH_MAKE_KEY: RNG CONS SUCCESS\n");

  if (prng_descriptor[wprng].read(buf, keysize, &prng) !=
      (unsigned long)keysize) {
    DEBUG_PRINT("DH_MAKE_KEY: FAILED RNG READ\n");
    TLS_FREE(buf);
    return TLS_GENERIC_ERROR;
  }

  if ((err = mp_init_multi(&key->g, &key->p, &key->x, &key->y, NULL)) !=
      CRYPT_OK) {
    TLS_FREE(buf);

    return TLS_GENERIC_ERROR;
  }

  if (gbuf_len <= 0) {
    if ((err = mp_read_radix(key->g, gbuf, 16)) != CRYPT_OK) {
      TLS_FREE(buf);
      _private_tls_dh_clear_key(key);
      return TLS_GENERIC_ERROR;
    }
  } else {
    DEBUG_PRINT("DH_MAKE_KEY: mp_read_unsigned_bin WHEN gbuf_len > 0\n");
    if ((err = mp_read_unsigned_bin(key->g, (unsigned char *)gbuf, gbuf_len)) !=
        CRYPT_OK) {
      TLS_FREE(buf);
      _private_tls_dh_clear_key(key);
      return TLS_GENERIC_ERROR;
    }
  }

  if (pbuf_len <= 0) {
    if ((err = mp_read_radix(key->p, pbuf, 16)) != CRYPT_OK) {
      TLS_FREE(buf);
      _private_tls_dh_clear_key(key);
      return TLS_GENERIC_ERROR;
    }
  } else {
    DEBUG_PRINT("DH_MAKE_KEY: mp_read_unsigned_bin WHEN pbuf_len > 0\n");
    if ((err = mp_read_unsigned_bin(key->p, (unsigned char *)pbuf, pbuf_len)) !=
        CRYPT_OK) {
      TLS_FREE(buf);
      _private_tls_dh_clear_key(key);
      return TLS_GENERIC_ERROR;
    }
  }

  DEBUG_PRINT("DH_MAKE_KEY: mp_read_unsigned_bin ON buf\n");
  if ((err = mp_read_unsigned_bin(key->x, buf, keysize)) != CRYPT_OK) {
    TLS_FREE(buf);
    _private_tls_dh_clear_key(key);
    return TLS_GENERIC_ERROR;
  }

  if ((err = mp_exptmod(key->g, key->x, key->p, key->y)) != CRYPT_OK) {
    TLS_FREE(buf);
    _private_tls_dh_clear_key(key);
    return TLS_GENERIC_ERROR;
  }

  TLS_FREE(buf);
  return 0;
}

int tls_is_ecdsa(struct TLSContext *context) {
  if (!context) return 0;
  switch (context->cipher) {
    // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:
    // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:
    // case TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:
    // case TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:
    case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
    case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
    case TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:
      return 1;
  }
  if (context->ec_private_key) return 1;
  return 0;
}

struct TLSPacket *tls_build_client_key_exchange(struct TLSContext *context) {
  if (context->is_server) {
    DEBUG_PRINT("CANNOT BUILD CLIENT KEY EXCHANGE MESSAGE FOR SERVERS\n");
    return NULL;
  }

  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version, 0);
  tls_packet_uint8(packet, 0x10);
  int ephemeral = tls_cipher_is_ephemeral(context);
  if ((ephemeral) && (context->premaster_key) && (context->premaster_key_len)) {
    if (ephemeral == 1) {
      unsigned char dh_Ys[0xFFF];
      unsigned char dh_p[0xFFF];
      unsigned char dh_g[0xFFF];
      unsigned long dh_p_len = sizeof(dh_p);
      unsigned long dh_g_len = sizeof(dh_g);
      unsigned long dh_Ys_len = sizeof(dh_Ys);

      if (_private_tls_dh_export_pqY(dh_p, &dh_p_len, dh_g, &dh_g_len, dh_Ys,
                                     &dh_Ys_len, context->dhe)) {
        DEBUG_PRINT("ERROR EXPORTING DHE KEY %p\n", context->dhe);
        TLS_FREE(packet);
        _private_tls_dhe_free(context);
        return NULL;
      }
      _private_tls_dhe_free(context);
      DEBUG_DUMP_HEX_LABEL("Yc", dh_Ys, dh_Ys_len);
      tls_packet_uint24(packet, dh_Ys_len + 2);
      if (context->dtls)
        _private_dtls_handshake_data(context, packet, dh_Ys_len + 2);
      tls_packet_uint16(packet, dh_Ys_len);
      tls_packet_append(packet, dh_Ys, dh_Ys_len);
    } else if (context->ecc_dhe) {
      unsigned char out[TLS_MAX_RSA_KEY];
      unsigned long out_len = TLS_MAX_RSA_KEY;

      if (ecc_ansi_x963_export(context->ecc_dhe, out, &out_len)) {
        DEBUG_PRINT("Error exporting ECC key\n");
        TLS_FREE(packet);
        return NULL;
      }
      _private_tls_ecc_dhe_free(context);
      tls_packet_uint24(packet, out_len + 1);
      if (context->dtls) {
        _private_dtls_handshake_data(context, packet, out_len + 1);
        context->dtls_seq++;
      }
      tls_packet_uint8(packet, out_len);
      tls_packet_append(packet, out, out_len);
    } else if ((context->is_server) && (context->curve == &x25519) &&
               (context->client_secret)) {
      static const unsigned char basepoint[32] = {9};
      unsigned char shared_key[32];
      curve25519(shared_key, context->client_secret, basepoint);
      tls_packet_uint24(packet, 32 + 1);
      tls_packet_uint8(packet, 32);
      tls_packet_append(packet, shared_key, 32);
      TLS_FREE(context->client_secret);
      context->client_secret = NULL;
    }
    _private_tls_compute_key(context, 48);
  } else
    _private_tls_build_random(packet);
  context->connection_status = 2;
  tls_packet_update(packet);
  return packet;
}

void _private_dtls_handshake_data(struct TLSContext *context,
                                  struct TLSPacket *packet,
                                  unsigned int framelength) {
  // message seq
  tls_packet_uint16(packet, context->dtls_seq);
  // fragment offset
  tls_packet_uint24(packet, 0);
  // fragment length
  tls_packet_uint24(packet, framelength);
}

void _private_dtls_handshake_copyframesize(struct TLSPacket *packet) {
  packet->buf[22] = packet->buf[14];
  packet->buf[23] = packet->buf[15];
  packet->buf[24] = packet->buf[16];
}

struct TLSPacket *tls_build_server_key_exchange(struct TLSContext *context,
                                                int method) {
  if (!context->is_server) {
    DEBUG_PRINT("CANNOT BUILD SERVER KEY EXCHANGE MESSAGE FOR CLIENTS\n");
    return NULL;
  }

  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version, 0);
  tls_packet_uint8(packet, 0x0C);
  unsigned char dummy[3];
  tls_packet_append(packet, dummy, 3);
  if (context->dtls) _private_dtls_handshake_data(context, packet, 0);
  int start_len = packet->len;
  if (method == KEA_dhe_rsa) {
    tls_init();
    _private_tls_dhe_create(context);

    const char *default_dhe_p = context->default_dhe_p;
    const char *default_dhe_g = context->default_dhe_g;
    int key_size;
    if ((!default_dhe_p) || (!default_dhe_g)) {
      default_dhe_p = TLS_DH_DEFAULT_P;
      default_dhe_g = TLS_DH_DEFAULT_G;
      key_size = TLS_DHE_KEY_SIZE / 8;
    } else {
      key_size = strlen(default_dhe_p);
    }
    if (_private_tls_dh_make_key(key_size, context->dhe, default_dhe_p,
                                 default_dhe_g, 0, 0)) {
      DEBUG_PRINT("ERROR CREATING DHE KEY\n");
      TLS_FREE(packet);
      TLS_FREE(context->dhe);
      context->dhe = NULL;
      return NULL;
    }

    unsigned char dh_Ys[0xFFF];
    unsigned char dh_p[0xFFF];
    unsigned char dh_g[0xFFF];
    unsigned long dh_p_len = sizeof(dh_p);
    unsigned long dh_g_len = sizeof(dh_g);
    unsigned long dh_Ys_len = sizeof(dh_Ys);

    if (_private_tls_dh_export_pqY(dh_p, &dh_p_len, dh_g, &dh_g_len, dh_Ys,
                                   &dh_Ys_len, context->dhe)) {
      DEBUG_PRINT("ERROR EXPORTING DHE KEY\n");
      TLS_FREE(packet);
      return NULL;
    }

    DEBUG_PRINT("LEN: %lu (%lu, %lu)\n", dh_Ys_len, dh_p_len, dh_g_len);
    DEBUG_DUMP_HEX_LABEL("DHE PK", dh_Ys, dh_Ys_len);
    DEBUG_DUMP_HEX_LABEL("DHE P", dh_p, dh_p_len);
    DEBUG_DUMP_HEX_LABEL("DHE G", dh_g, dh_g_len);

    tls_packet_uint16(packet, dh_p_len);
    tls_packet_append(packet, dh_p, dh_p_len);

    tls_packet_uint16(packet, dh_g_len);
    tls_packet_append(packet, dh_g, dh_g_len);

    tls_packet_uint16(packet, dh_Ys_len);
    tls_packet_append(packet, dh_Ys, dh_Ys_len);
    // dh_p
    // dh_g
    // dh_Ys
  } else if (method == KEA_ec_diffie_hellman) {
    // 3 = named curve
    if (!context->curve) context->curve = default_curve;
    tls_packet_uint8(packet, 3);
    tls_packet_uint16(packet, context->curve->iana);
    tls_init();
    _private_tls_ecc_dhe_create(context);

    const ltc_ecc_set_type *dp = &context->curve->dp;

    if (ecc_make_key_ex(NULL, find_prng("sprng"), context->ecc_dhe, dp)) {
      TLS_FREE(context->ecc_dhe);
      context->ecc_dhe = NULL;
      DEBUG_PRINT("Error generating ECC key\n");
      TLS_FREE(packet);
      return NULL;
    }
    unsigned char out[TLS_MAX_RSA_KEY];
    unsigned long out_len = TLS_MAX_RSA_KEY;
    if (ecc_ansi_x963_export(context->ecc_dhe, out, &out_len)) {
      DEBUG_PRINT("Error exporting ECC key\n");
      TLS_FREE(packet);
      return NULL;
    }
    tls_packet_uint8(packet, out_len);
    tls_packet_append(packet, out, out_len);
  } else {
    TLS_FREE(packet);
    DEBUG_PRINT("Unsupported ephemeral method: %i\n", method);
    return NULL;
  }

  // signature
  unsigned int params_len = packet->len - start_len;
  unsigned int message_len =
      params_len + TLS_CLIENT_RANDOM_SIZE + TLS_SERVER_RANDOM_SIZE;
  unsigned char *message = (unsigned char *)TLS_MALLOC(message_len);
  if (message) {
    unsigned char out[TLS_MAX_RSA_KEY];
    unsigned long out_len = TLS_MAX_RSA_KEY;

    int hash_algorithm;
    if ((context->version != TLS_V13) && (context->version != DTLS_V13) &&
        (context->version != TLS_V12) && (context->version != DTLS_V12)) {
      hash_algorithm = _md5_sha1;
    } else {
      if ((context->version == TLS_V13) || (context->version == DTLS_V13) ||
          (context->version == TLS_V12) || (context->version == DTLS_V12))
        hash_algorithm = sha256;
      else
        hash_algorithm = sha1;

      if (tls_is_ecdsa(context)) {
        if ((context->version == TLS_V13) || (context->version == DTLS_V13) ||
            (context->version == TLS_V12) || (context->version == DTLS_V12))
          hash_algorithm = sha512;
        tls_packet_uint8(packet, hash_algorithm);
        tls_packet_uint8(packet, ecdsa);
      } else {
        tls_packet_uint8(packet, hash_algorithm);
        tls_packet_uint8(packet, rsa_sign);
      }
    }

    memcpy(message, context->remote_random, TLS_CLIENT_RANDOM_SIZE);
    memcpy(message + TLS_CLIENT_RANDOM_SIZE, context->local_random,
           TLS_SERVER_RANDOM_SIZE);
    memcpy(message + TLS_CLIENT_RANDOM_SIZE + TLS_SERVER_RANDOM_SIZE,
           packet->buf + start_len, params_len);
    if (tls_is_ecdsa(context)) {
      if (_private_tls_sign_ecdsa(context, hash_algorithm, message, message_len,
                                  out, &out_len) == 1) {
        DEBUG_PRINT("Signing OK! (ECDSA, length %lu)\n", out_len);
        tls_packet_uint16(packet, out_len);
        tls_packet_append(packet, out, out_len);
      }
    } else if (_private_tls_sign_rsa(context, hash_algorithm, message,
                                     message_len, out, &out_len) == 1) {
      DEBUG_PRINT("Signing OK! (length %lu)\n", out_len);
      tls_packet_uint16(packet, out_len);
      tls_packet_append(packet, out, out_len);
    }
    TLS_FREE(message);
  }
  if ((!packet->broken) && (packet->buf)) {
    int remaining = packet->len - start_len;
    int payload_pos = 6;
    if (context->dtls) payload_pos = 14;
    packet->buf[payload_pos] = remaining / 0x10000;
    remaining %= 0x10000;
    packet->buf[payload_pos + 1] = remaining / 0x100;
    remaining %= 0x100;
    packet->buf[payload_pos + 2] = remaining;
    if (context->dtls) {
      _private_dtls_handshake_copyframesize(packet);
      context->dtls_seq++;
    }
  }
  tls_packet_update(packet);
  return packet;
}

void _private_tls_set_session_id(struct TLSContext *context) {
  if (((context->version == TLS_V13) || (context->version == DTLS_V13)) &&
      (context->session_size == TLS_MAX_SESSION_ID))
    return;
  if (tls_random(context->session, TLS_MAX_SESSION_ID))
    context->session_size = TLS_MAX_SESSION_ID;
  else
    context->session_size = 0;
}

struct TLSPacket *tls_build_hello(struct TLSContext *context,
                                  int tls13_downgrade) {
  tls_init();
  if (context->connection_status == 4) {
    static unsigned char sha256_helloretryrequest[] = {
        0xCF, 0x21, 0xAD, 0x74, 0xE5, 0x9A, 0x61, 0x11, 0xBE, 0x1D, 0x8C,
        0x02, 0x1E, 0x65, 0xB8, 0x91, 0xC2, 0xA2, 0x11, 0x16, 0x7A, 0xBB,
        0x8C, 0x5E, 0x07, 0x9E, 0x09, 0xE2, 0xC8, 0xA8, 0x33, 0x9C};
    memcpy(context->local_random, sha256_helloretryrequest, 32);
    unsigned char header[4] = {0xFE, 0, 0, 0};
    unsigned char hash[TLS_MAX_SHA_SIZE];
    int hash_len = _private_tls_done_hash(context, hash);
    header[3] = (unsigned char)hash_len;
    _private_tls_update_hash(context, header, sizeof(header));
    _private_tls_update_hash(context, hash, hash_len);
  } else if ((!context->is_server) ||
             ((context->version != TLS_V13) && (context->version != DTLS_V13)))
    if (!tls_random(context->local_random, context->is_server
                                               ? TLS_SERVER_RANDOM_SIZE
                                               : TLS_CLIENT_RANDOM_SIZE))
      return NULL;
  if (!context->is_server)
    *(unsigned int *)context->local_random = htonl((unsigned int)time(NULL));

  if ((context->is_server) && (tls13_downgrade)) {
    if ((tls13_downgrade == TLS_V12) || (tls13_downgrade == DTLS_V12))
      memcpy(context->local_random + TLS_SERVER_RANDOM_SIZE - 8, "DOWNGRD\x01",
             8);
    else
      memcpy(context->local_random + TLS_SERVER_RANDOM_SIZE - 8, "DOWNGRD\x00",
             8);
  }
  unsigned short packet_version = context->version;
  unsigned short version = context->version;
  if (context->version == TLS_V13)
    version = TLS_V12;
  else if (context->version == DTLS_V13)
    version = DTLS_V12;
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, packet_version, 0);
  if (packet) {
    // hello
    if (context->is_server)
      tls_packet_uint8(packet, 0x02);
    else
      tls_packet_uint8(packet, 0x01);
    unsigned char dummy[3];
    tls_packet_append(packet, dummy, 3);

    if (context->dtls) _private_dtls_handshake_data(context, packet, 0);

    int start_len = packet->len;
    tls_packet_uint16(packet, version);
    if (context->is_server)
      tls_packet_append(packet, context->local_random, TLS_SERVER_RANDOM_SIZE);
    else
      tls_packet_append(packet, context->local_random, TLS_CLIENT_RANDOM_SIZE);

    // ignore sessions
    tls_packet_uint8(packet, 0);

    int extension_len = 0;
    int alpn_len = 0;
    int alpn_negotiated_len = 0;
    int i;
    unsigned char shared_key[TLS_MAX_RSA_KEY];
    unsigned long shared_key_len = TLS_MAX_RSA_KEY;
    unsigned short shared_key_short = 0;
    int selected_group = 0;
    if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
      if (context->connection_status == 4) {
        // connection_status == 4 => hello retry request
        extension_len += 6;
      } else if (context->is_server) {
        if (context->curve == &x25519) {
          extension_len += 8 + 32;
          shared_key_short = (unsigned short)32;
          if (context->finished_key) {
            memcpy(shared_key, context->finished_key, 32);
            TLS_FREE(context->finished_key);
            context->finished_key = NULL;
          }
          selected_group = context->curve->iana;
          // make context->curve NULL (x25519 is a different implementation)
          context->curve = NULL;
        } else if (context->ecc_dhe) {
          if (ecc_ansi_x963_export(context->ecc_dhe, shared_key,
                                   &shared_key_len)) {
            DEBUG_PRINT("Error exporting ECC DHE key\n");
            tls_destroy_packet(packet);
            return tls_build_alert(context, 1, internal_error);
          }
          _private_tls_ecc_dhe_free(context);
          extension_len += 8 + shared_key_len;
          shared_key_short = (unsigned short)shared_key_len;
          if (context->curve) selected_group = context->curve->iana;
        } else if (context->dhe) {
          selected_group = context->dhe->iana;
          _private_tls_dh_export_Y(shared_key, &shared_key_len, context->dhe);
          _private_tls_dhe_free(context);
          extension_len += 8 + shared_key_len;
          shared_key_short = (unsigned short)shared_key_len;
        }
      }
      // supported versions
      if (context->is_server)
        extension_len += 6;
      else
        extension_len += 9;
    }
    if ((context->is_server) && (context->negotiated_alpn) &&
        (context->version != TLS_V13) && (context->version != DTLS_V13)) {
      alpn_negotiated_len = strlen(context->negotiated_alpn);
      alpn_len = alpn_negotiated_len + 1;
      extension_len += alpn_len + 6;
    } else if ((!context->is_server) && (context->alpn_count)) {
      for (i = 0; i < context->alpn_count; i++) {
        if (context->alpn[i]) {
          int len = strlen(context->alpn[i]);
          if (len) alpn_len += len + 1;
        }
      }
      if (alpn_len) extension_len += alpn_len + 6;
    }

    // ciphers
    if (context->is_server) {
      // fallback ... this should never happen
      if (!context->cipher) context->cipher = TLS_DHE_RSA_WITH_AES_128_CBC_SHA;

      tls_packet_uint16(packet, context->cipher);
      // no compression
      tls_packet_uint8(packet, 0);
      if ((context->version == TLS_V13) || (context->version == DTLS_V13) ||
          (context->version == TLS_V12) || (context->version == DTLS_V12)) {
        // extensions size
        if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
          tls_packet_uint16(packet, extension_len);
        } else {
          tls_packet_uint16(packet, 5 + extension_len);
          // secure renegotation
          // advertise it, but refuse renegotiation
          tls_packet_uint16(packet, 0xff01);
          tls_packet_uint16(packet, 1);
          tls_packet_uint8(packet, 0);
        }
        if (alpn_len) {
          tls_packet_uint16(packet, 0x10);
          tls_packet_uint16(packet, alpn_len + 2);
          tls_packet_uint16(packet, alpn_len);

          tls_packet_uint8(packet, alpn_negotiated_len);
          tls_packet_append(packet, (unsigned char *)context->negotiated_alpn,
                            alpn_negotiated_len);
        }
      }
    } else {
      // we're in a client context and can send all the ciphers!

      if (context->dtls) {
        tls_packet_uint8(packet, context->dtls_cookie_len);
        if (context->dtls_cookie_len)
          tls_packet_append(packet, context->dtls_cookie,
                            context->dtls_cookie_len);
      }

      if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
        tls_packet_uint16(packet, TLS_CIPHERS_SIZE(9, 0));
        tls_packet_uint16(packet, TLS_AES_128_GCM_SHA256);
        tls_packet_uint16(packet, TLS_AES_256_GCM_SHA384);
        tls_packet_uint16(packet, TLS_CHACHA20_POLY1305_SHA256);
        tls_packet_uint16(packet, TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256);
        tls_packet_uint16(packet,
                          TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256);
        tls_packet_uint16(packet, TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256);
        tls_packet_uint16(packet, TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256);
        tls_packet_uint16(packet, TLS_DHE_RSA_WITH_AES_128_GCM_SHA256);
        tls_packet_uint16(packet, TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256);
      } else if ((context->version == TLS_V12) ||
                 (context->version == DTLS_V12)) {
        tls_packet_uint16(packet, TLS_CIPHERS_SIZE(6, 5));
        tls_packet_uint16(packet, TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256);
        tls_packet_uint16(packet,
                          TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256);
        tls_packet_uint16(packet, TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256);
        tls_packet_uint16(packet, TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256);
        tls_packet_uint16(packet, TLS_DHE_RSA_WITH_AES_128_GCM_SHA256);
        // tls_packet_uint16(packet, TLS_DHE_RSA_WITH_AES_256_CBC_SHA256);
        tls_packet_uint16(packet, TLS_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256);
      }
      // compression
      tls_packet_uint8(packet, 1);
      // no compression
      tls_packet_uint8(packet, 0);
      if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
          (context->version == TLS_V13) || (context->version == DTLS_V13)) {
        int sni_len = 0;
        if (context->sni) sni_len = strlen(context->sni);

        extension_len += 12;
        if (sni_len) extension_len += sni_len + 9;
        if ((!context->is_server) &&
            ((context->version == TLS_V13) || (context->version == DTLS_V13))) {
          // secp256r1 produces 65 bytes export
          extension_len += 103;
          // extension_len += 70;
        }
        tls_packet_uint16(packet, extension_len);

        if (sni_len) {
          // sni extension
          tls_packet_uint16(packet, 0x00);
          // sni extension len
          tls_packet_uint16(packet, sni_len + 5);
          // sni len
          tls_packet_uint16(packet, sni_len + 3);
          // sni type
          tls_packet_uint8(packet, 0);
          // sni host len
          tls_packet_uint16(packet, sni_len);
          tls_packet_append(packet, (unsigned char *)context->sni, sni_len);
        }
        // supported groups
        tls_packet_uint16(packet, 0x0A);
        tls_packet_uint16(packet, 8);
        // 3 curves x 2 bytes
        tls_packet_uint16(packet, 6);
        tls_packet_uint16(packet, secp256r1.iana);
        tls_packet_uint16(packet, secp384r1.iana);

        if (context->is_server)
          tls_packet_uint16(packet, x25519.iana);
        else
          tls_packet_uint16(packet, secp224r1.iana);

        if (alpn_len) {
          tls_packet_uint16(packet, 0x10);
          tls_packet_uint16(packet, alpn_len + 2);
          tls_packet_uint16(packet, alpn_len);

          for (i = 0; i < context->alpn_count; i++) {
            if (context->alpn[i]) {
              int len = strlen(context->alpn[i]);
              if (len) {
                tls_packet_uint8(packet, len);
                tls_packet_append(packet, (unsigned char *)context->alpn[i],
                                  len);
              }
            }
          }
        }
      }
    }
    if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
      // supported versions
      tls_packet_uint16(packet, 0x2B);
      if (context->is_server) {
        tls_packet_uint16(packet, 2);
        if (context->version == TLS_V13)
          tls_packet_uint16(packet, context->tls13_version
                                        ? context->tls13_version
                                        : TLS_V13);
        else
          tls_packet_uint16(packet, context->version);
      } else {
        tls_packet_uint16(packet, 5);
        tls_packet_uint8(packet, 4);
        tls_packet_uint16(packet, TLS_V13);
        tls_packet_uint16(packet, 0x7F1C);
      }
      if (context->connection_status == 4) {
        // fallback to the mandatory secp256r1
        tls_packet_uint16(packet, 0x33);
        tls_packet_uint16(packet, 2);
        tls_packet_uint16(packet, (unsigned short)secp256r1.iana);
      }
      if (((shared_key_short) && (selected_group)) || (!context->is_server)) {
        // key share
        tls_packet_uint16(packet, 0x33);
        if (context->is_server) {
          tls_packet_uint16(packet, shared_key_short + 4);
          tls_packet_uint16(packet, (unsigned short)selected_group);
          tls_packet_uint16(packet, shared_key_short);
          tls_packet_append(packet, (unsigned char *)shared_key,
                            shared_key_short);
        } else {
          // make key
          shared_key_short = 65;
          tls_packet_uint16(packet, shared_key_short + 6);
          tls_packet_uint16(packet, shared_key_short + 4);

          _private_tls_ecc_dhe_create(context);
          ltc_ecc_set_type *dp = (ltc_ecc_set_type *)&secp256r1.dp;

          if (ecc_make_key_ex(NULL, find_prng("sprng"), context->ecc_dhe, dp)) {
            TLS_FREE(context->ecc_dhe);
            context->ecc_dhe = NULL;
            DEBUG_PRINT("Error generating ECC key\n");
            TLS_FREE(packet);
            return NULL;
          }
          unsigned char out[TLS_MAX_RSA_KEY];
          unsigned long out_len = shared_key_short;
          if (ecc_ansi_x963_export(context->ecc_dhe, out, &out_len)) {
            DEBUG_PRINT("Error exporting ECC key\n");
            TLS_FREE(packet);
            return NULL;
          }

          tls_packet_uint16(packet, (unsigned short)secp256r1.iana);
          tls_packet_uint16(packet, out_len);
          tls_packet_append(packet, (unsigned char *)out, shared_key_short);
        }
      }
      if (!context->is_server) {
        // signature algorithms
        tls_packet_uint16(packet, 0x0D);
        tls_packet_uint16(packet, 24);
        tls_packet_uint16(packet, 22);
        tls_packet_uint16(packet, 0x0403);
        tls_packet_uint16(packet, 0x0503);
        tls_packet_uint16(packet, 0x0603);
        tls_packet_uint16(packet, 0x0804);
        tls_packet_uint16(packet, 0x0805);
        tls_packet_uint16(packet, 0x0806);
        tls_packet_uint16(packet, 0x0401);
        tls_packet_uint16(packet, 0x0501);
        tls_packet_uint16(packet, 0x0601);
        tls_packet_uint16(packet, 0x0203);
        tls_packet_uint16(packet, 0x0201);
      }
    }

    if ((!packet->broken) && (packet->buf)) {
      int remaining = packet->len - start_len;
      int payload_pos = 6;
      if (context->dtls) payload_pos = 14;
      packet->buf[payload_pos] = remaining / 0x10000;
      remaining %= 0x10000;
      packet->buf[payload_pos + 1] = remaining / 0x100;
      remaining %= 0x100;
      packet->buf[payload_pos + 2] = remaining;
      if (context->dtls) {
        _private_dtls_handshake_copyframesize(packet);
        context->dtls_seq++;
      }
    }
    tls_packet_update(packet);
  }
  return packet;
}

// struct TLSPacket *tls_certificate_request(struct TLSContext *context) {
//  if ((!context) || (!context->is_server)) return NULL;
//
//  unsigned short packet_version = context->version;
//  struct TLSPacket *packet =
//      tls_create_packet(context, TLS_HANDSHAKE, packet_version, 0);
//  if (packet) {
//    // certificate request
//    tls_packet_uint8(packet, 0x0D);
//    unsigned char dummy[3];
//    tls_packet_append(packet, dummy, 3);
//    if (context->dtls) _private_dtls_handshake_data(context, packet, 0);
//    int start_len = packet->len;
//    if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
//      // certificate request context
//      tls_packet_uint8(packet, 0);
//      // extensions
//      tls_packet_uint16(packet, 18);
//      // signature algorithms
//      tls_packet_uint16(packet, 0x0D);
//      tls_packet_uint16(packet, 14);
//      tls_packet_uint16(packet, 12);
//      // rsa_pkcs1_sha256
//      // tls_packet_uint16(packet, 0x0401);
//      // rsa_pkcs1_sha384
//      // tls_packet_uint16(packet, 0x0501);
//      // rsa_pkcs1_sha512
//      // tls_packet_uint16(packet, 0x0601);
//
//      // ecdsa_secp256r1_sha256
//      tls_packet_uint16(packet, 0x0403);
//      // ecdsa_secp384r1_sha384
//      tls_packet_uint16(packet, 0x0503);
//      // ecdsa_secp521r1_sha512
//      tls_packet_uint16(packet, 0x0604);
//      // rsa_pss_rsae_sha256
//      tls_packet_uint16(packet, 0x0804);
//      // rsa_pss_rsae_sha384
//      tls_packet_uint16(packet, 0x0805);
//      // rsa_pss_rsae_sha512
//      tls_packet_uint16(packet, 0x0806);
//    } else {
//      tls_packet_uint8(packet, 1);
//      tls_packet_uint8(packet, rsa_sign);
//      if ((context->version == TLS_V12) || (context->version == DTLS_V12)) {
//        // 10 pairs or 2 bytes
//        tls_packet_uint16(packet, 10);
//        tls_packet_uint8(packet, sha256);
//        tls_packet_uint8(packet, rsa);
//        tls_packet_uint8(packet, sha1);
//        tls_packet_uint8(packet, rsa);
//        tls_packet_uint8(packet, sha384);
//        tls_packet_uint8(packet, rsa);
//        tls_packet_uint8(packet, sha512);
//        tls_packet_uint8(packet, rsa);
//        tls_packet_uint8(packet, md5);
//        tls_packet_uint8(packet, rsa);
//      }
//      // no DistinguishedName yet
//      tls_packet_uint16(packet, 0);
//    }
//    if (!packet->broken) {
//      int remaining = packet->len - start_len;
//      int payload_pos = 6;
//      if (context->dtls) payload_pos = 14;
//      packet->buf[payload_pos] = remaining / 0x10000;
//      remaining %= 0x10000;
//      packet->buf[payload_pos + 1] = remaining / 0x100;
//      remaining %= 0x100;
//      packet->buf[payload_pos + 2] = remaining;
//
//      if (context->dtls) {
//        _private_dtls_handshake_copyframesize(packet);
//        context->dtls_seq++;
//      }
//    }
//    tls_packet_update(packet);
//  }
//  return packet;
//}

int _private_dtls_build_cookie(struct TLSContext *context) {
  if ((!context->dtls_cookie) || (!context->dtls_cookie_len)) {
    context->dtls_cookie = (unsigned char *)TLS_MALLOC(DTLS_COOKIE_SIZE);
    if (!context->dtls_cookie) return 0;

    if (!tls_random(context->dtls_cookie, DTLS_COOKIE_SIZE)) {
      TLS_FREE(context->dtls_cookie);
      context->dtls_cookie = NULL;
      return 0;
    }
    context->dtls_cookie_len = DTLS_COOKIE_SIZE;
  }
  return 1;
}

struct TLSPacket *tls_build_verify_request(struct TLSContext *context) {
  if ((!context->is_server) || (!context->dtls)) return NULL;

  if ((!context->dtls_cookie) || (!context->dtls_cookie_len)) {
    if (!_private_dtls_build_cookie(context)) return NULL;
  }

  unsigned short packet_version = context->version;
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, packet_version, 0);
  if (packet) {
    // verify request
    tls_packet_uint8(packet, 0x03);
    // 24-bit length
    tls_packet_uint24(packet, context->dtls_cookie_len + 3);
    // 16-bit message_sequence
    tls_packet_uint16(packet, 0);
    // 24-bit fragment_offset
    tls_packet_uint24(packet, 0);
    // 24-bit fragment_offset
    tls_packet_uint24(packet, context->dtls_cookie_len + 3);
    // server_version
    tls_packet_uint16(packet, context->version);
    tls_packet_uint8(packet, context->dtls_cookie_len);
    tls_packet_append(packet, context->dtls_cookie, context->dtls_cookie_len);
    tls_packet_update(packet);
  }
  return packet;
}

int _private_dtls_check_packet(const unsigned char *buf, int buf_len) {
  CHECK_SIZE(11, buf_len, TLS_NEED_MORE_DATA)

  unsigned int bytes_to_follow = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  // not used: unsigned short message_seq = ntohs(*(unsigned short *)&buf[3]);
  unsigned int fragment_offset = buf[5] * 0x10000 + buf[6] * 0x100 + buf[7];
  unsigned int fragment_length = buf[8] * 0x10000 + buf[9] * 0x100 + buf[10];

  if ((fragment_offset) || (fragment_length != bytes_to_follow)) {
    DEBUG_PRINT("FRAGMENTED PACKETS NOT SUPPORTED\n");
    return TLS_FEATURE_NOT_SUPPORTED;
  }
  return bytes_to_follow;
}

void _private_dtls_reset(struct TLSContext *context) {
  context->dtls_epoch_local = 0;
  context->dtls_epoch_remote = 0;
  context->dtls_seq = 0;
  _private_tls_destroy_hash(context);
  context->connection_status = 0;
}

int tls_parse_verify_request(struct TLSContext *context,
                             const unsigned char *buf, int buf_len,
                             unsigned int *write_packets) {
  *write_packets = 0;
  if ((context->connection_status != 0) || (!context->dtls)) {
    DEBUG_PRINT("UNEXPECTED VERIFY REQUEST MESSAGE\n");
    return TLS_UNEXPECTED_MESSAGE;
  }
  int res = 11;
  int bytes_to_follow = _private_dtls_check_packet(buf, buf_len);
  if (bytes_to_follow < 0) return bytes_to_follow;

  CHECK_SIZE(bytes_to_follow, buf_len - res, TLS_NEED_MORE_DATA)
  // not used: unsigned short version = ntohs(*(unsigned short *)&buf[res]);
  res += 2;
  unsigned char len = buf[res];
  res++;
  TLS_FREE(context->dtls_cookie);
  context->dtls_cookie_len = 0;
  if (len) {
    CHECK_SIZE(len, buf_len - res, TLS_NEED_MORE_DATA)
    context->dtls_cookie = (unsigned char *)TLS_MALLOC(len);
    if (!context->dtls_cookie) return TLS_NO_MEMORY;
    context->dtls_cookie_len = len;
    memcpy(context->dtls_cookie, &buf[res], len);
    res += len;
    *write_packets = 4;
  }

  // reset context
  _private_dtls_reset(context);
  return res;
}

void _private_dtls_reset_cookie(struct TLSContext *context) {
  TLS_FREE(context->dtls_cookie);
  context->dtls_cookie = NULL;
  context->dtls_cookie_len = 0;
}

int _private_tls_parse_key_share(struct TLSContext *context,
                                 const unsigned char *buf, int buf_len) {
  DEBUG_PRINT("PARSE KEY SHARE\n");
  int i = 0;
  struct ECCCurveParameters *curve = 0;
  DHKey *dhkey = 0;
  int dhe_key_size = 0;
  const unsigned char *buffer = NULL;
  unsigned char *out2;
  unsigned long out_size;
  unsigned short key_size = 0;
  while (buf_len >= 4) {
    unsigned short named_group = ntohs(*(unsigned short *)&buf[i]);
    i += 2;
    buf_len -= 2;

    key_size = ntohs(*(unsigned short *)&buf[i]);
    i += 2;
    buf_len -= 2;

    if (key_size > buf_len) return TLS_BROKEN_PACKET;

    switch (named_group) {
      case 0x0017:
        curve = &secp256r1;
        buffer = &buf[i];
        DEBUG_PRINT("KEY SHARE => secp256r1\n");
        buf_len = 0;
        continue;
      case 0x0018:
        // secp384r1
        curve = &secp384r1;
        buffer = &buf[i];
        DEBUG_PRINT("KEY SHARE => secp384r1\n");
        buf_len = 0;
        continue;
      case 0x0019:
        // secp521r1
        DEBUG_PRINT("UNSUPPORTED KEY SHARE => secp521r1\n");
        break;
      case 0x001D:
        // x25519
        if (key_size != 32) {
          DEBUG_PRINT("INVALID x25519 KEY SIZE (%i)\n", key_size);
          continue;
        }
        curve = &x25519;
        buffer = &buf[i];
        DEBUG_PRINT("KEY SHARE => x25519\n");
        buf_len = 0;
        continue;
        break;
      case 0x001E:
        // x448
        DEBUG_PRINT("UNSUPPORTED KEY SHARE => x448\n");
        break;
      case 0x0100:
        dhkey = &ffdhe2048;
        dhe_key_size = 2048;
        break;
      case 0x0101:
        dhkey = &ffdhe3072;
        dhe_key_size = 3072;
        break;
      case 0x0102:
        dhkey = &ffdhe4096;
        dhe_key_size = 4096;
        break;
      case 0x0103:
        dhkey = &ffdhe6144;
        dhe_key_size = 6144;
        break;
      case 0x0104:
        dhkey = &ffdhe8192;
        dhe_key_size = 8192;
        break;
    }
    i += key_size;
    buf_len -= key_size;

    if (!context->is_server) break;
  }
  tls_init();
  if (curve) {
    DEBUG_PRINT("KEY SHARE HANDLER: CURVE\n");
    context->curve = curve;
    if (context->is_server && curve == &x25519) {
      if ((context->is_server) &&
          (!tls_random(context->local_random, TLS_SERVER_RANDOM_SIZE)))
        return TLS_GENERIC_ERROR;
      unsigned char secret[32];
      static const unsigned char basepoint[32] = {9};

      if ((context->is_server) || (!context->client_secret)) {
        tls_random(secret, 32);

        secret[0] &= 248;
        secret[31] &= 127;
        secret[31] |= 64;

        // use finished key to store public key
        TLS_FREE(context->finished_key);
        context->finished_key = (unsigned char *)TLS_MALLOC(32);
        if (!context->finished_key) return TLS_GENERIC_ERROR;

        curve25519(context->finished_key, secret, basepoint);

        TLS_FREE(context->premaster_key);
        context->premaster_key = (unsigned char *)TLS_MALLOC(32);
        if (!context->premaster_key) return TLS_GENERIC_ERROR;

        curve25519(context->premaster_key, secret, buffer);
        context->premaster_key_len = 32;
      } else {
        TLS_FREE(context->premaster_key);
        context->premaster_key = (unsigned char *)TLS_MALLOC(32);
        if (!context->premaster_key) return TLS_GENERIC_ERROR;

        curve25519(context->premaster_key, context->client_secret, buffer);
        context->premaster_key_len = 32;

        TLS_FREE(context->client_secret);
        context->client_secret = NULL;
      }
      DEBUG_DUMP_HEX_LABEL("x25519 KEY", context->premaster_key,
                           context->premaster_key_len);

      return 0;
    }
    if (context->is_server) {
      _private_tls_ecc_dhe_create(context);
      if (ecc_make_key_ex(NULL, find_prng("sprng"), context->ecc_dhe,
                          &context->curve->dp)) {
        TLS_FREE(context->ecc_dhe);
        context->ecc_dhe = NULL;
        DEBUG_PRINT("Error generating ECC DHE key\n");
        return TLS_GENERIC_ERROR;
      }
    }

    const ltc_ecc_set_type *dp = &context->curve->dp;

    if ((context->is_server) &&
        (!tls_random(context->local_random, TLS_SERVER_RANDOM_SIZE)))
      return TLS_GENERIC_ERROR;

    ecc_key client_key;
    memset(&client_key, 0, sizeof(client_key));
    if (ecc_ansi_x963_import_ex(buffer, key_size, &client_key, dp)) {
      DEBUG_PRINT("Error importing ECC DHE key\n");
      return TLS_GENERIC_ERROR;
    }
    out2 = (unsigned char *)TLS_MALLOC(key_size);
    out_size = key_size;

    int err = ecc_shared_secret(context->ecc_dhe, &client_key, out2, &out_size);
    ecc_free(&client_key);

    if (err) {
      DEBUG_PRINT("ECC DHE DECRYPT ERROR %i\n", err);
      TLS_FREE(out2);
      return TLS_GENERIC_ERROR;
    }
    DEBUG_PRINT("OUT_SIZE: %lu\n", out_size);
    DEBUG_DUMP_HEX_LABEL("ECC DHE", out2, out_size);

    TLS_FREE(context->premaster_key);
    context->premaster_key = out2;
    context->premaster_key_len = out_size;
    return 0;
  } else if (dhkey) {
    DEBUG_PRINT("KEY SHARE HANDLER: DHE - BEGIN\n");

    _private_tls_dhe_create(context);
    DEBUG_PRINT("KEY SHARE HANDLER: DHE - CREATED\n");

    if (!tls_random(context->local_random, TLS_SERVER_RANDOM_SIZE)) {
      DEBUG_PRINT("KEY SHARE HANDLER: DHE - !!ERROR!! TLS RANDOM\n");
      return TLS_GENERIC_ERROR;
    }

    DEBUG_PRINT("KEY SHARE HANDLER: DHE - GOT RANDOM\n");

    if (_private_tls_dh_make_key(dhe_key_size / 8, context->dhe,
                                 (const char *)dhkey->p, (const char *)dhkey->g,
                                 0, 0)) {
      TLS_FREE(context->dhe);
      context->dhe = NULL;
      DEBUG_PRINT("Error generating DHE key\n");
      return TLS_GENERIC_ERROR;
    }

    DEBUG_PRINT("KEY SHARE HANDLER: DHE - KEY GENERATED\n");

    unsigned int dhe_out_size;
    out2 =
        _private_tls_decrypt_dhe(context, buffer, key_size, &dhe_out_size, 0);
    if (!out2) {
      DEBUG_PRINT("Error generating DHE shared key\n");
      return TLS_GENERIC_ERROR;
    }

    DEBUG_PRINT("KEY SHARE HANDLER: DHE - SHARED KEY GENERATED\n");
    TLS_FREE(context->premaster_key);
    context->premaster_key = out2;
    context->premaster_key_len = dhe_out_size;
    if (context->dhe) context->dhe->iana = dhkey->iana;
    return 0;
  }
  DEBUG_PRINT("NO COMMON KEY SHARE SUPPORTED\n");
  return TLS_NO_COMMON_CIPHER;
}

int tls_parse_hello(struct TLSContext *context, const unsigned char *buf,
                    int buf_len, unsigned int *write_packets,
                    unsigned int *dtls_verified) {
  *write_packets = 0;
  *dtls_verified = 0;
  if ((context->connection_status != 0) && (context->connection_status != 4)) {
    // ignore multiple hello on dtls
    if (context->dtls) {
      DEBUG_PRINT("RETRANSMITTED HELLO MESSAGE RECEIVED\n");
      return 1;
    }
    DEBUG_PRINT("UNEXPECTED HELLO MESSAGE\n");
    return TLS_UNEXPECTED_MESSAGE;
  }

  int res = 0;
  int downgraded = 0;
  int hello_min_size =
      context->dtls ? TLS_CLIENT_HELLO_MINSIZE + 8 : TLS_CLIENT_HELLO_MINSIZE;
  CHECK_SIZE(hello_min_size, buf_len, TLS_NEED_MORE_DATA)
  // big endian
  unsigned int bytes_to_follow = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  res += 3;
  if (context->dtls) {
    int dtls_check = _private_dtls_check_packet(buf, buf_len);
    if (dtls_check < 0) return dtls_check;
    // 16 bit message seq + 24 bit fragment offset + 24 bit fragment length
    res += 8;
  }
  CHECK_SIZE(bytes_to_follow, buf_len - res, TLS_NEED_MORE_DATA)

  CHECK_SIZE(2, buf_len - res, TLS_NEED_MORE_DATA)
  unsigned short version = ntohs(*(unsigned short *)&buf[res]);
  unsigned short cipher = 0;

  res += 2;
  VERSION_SUPPORTED(version, TLS_NOT_SAFE)
  DEBUG_PRINT("VERSION REQUIRED BY REMOTE %x, VERSION NOW %x\n", (int)version,
              (int)context->version);

  memcpy(context->remote_random, &buf[res], TLS_CLIENT_RANDOM_SIZE);
  res += TLS_CLIENT_RANDOM_SIZE;

  unsigned char session_len = buf[res++];
  CHECK_SIZE(session_len, buf_len - res, TLS_NEED_MORE_DATA)
  if ((session_len) && (session_len <= TLS_MAX_SESSION_ID)) {
    memcpy(context->session, &buf[res], session_len);
    context->session_size = session_len;
    DEBUG_DUMP_HEX_LABEL("REMOTE SESSION ID: ", context->session,
                         context->session_size);
  } else
    context->session_size = 0;
  res += session_len;

  const unsigned char *cipher_buffer = NULL;
  unsigned short cipher_len = 0;
  int scsv_set = 0;
  if (context->is_server) {
    if (context->dtls) {
      CHECK_SIZE(1, buf_len - res, TLS_NEED_MORE_DATA)
      unsigned char tls_cookie_len = buf[res++];
      if (tls_cookie_len) {
        CHECK_SIZE(tls_cookie_len, buf_len - res, TLS_NEED_MORE_DATA)
        if ((!context->dtls_cookie_len) || (!context->dtls_cookie))
          _private_dtls_build_cookie(context);

        if ((context->dtls_cookie_len != tls_cookie_len) ||
            (!context->dtls_cookie)) {
          *dtls_verified = 2;
          _private_dtls_reset_cookie(context);
          DEBUG_PRINT("INVALID DTLS COOKIE\n");
          return TLS_BROKEN_PACKET;
        }
        if (memcmp(context->dtls_cookie, &buf[res], tls_cookie_len)) {
          *dtls_verified = 3;
          _private_dtls_reset_cookie(context);
          DEBUG_PRINT("MISMATCH DTLS COOKIE\n");
          return TLS_BROKEN_PACKET;
        }
        _private_dtls_reset_cookie(context);
        context->dtls_seq++;
        *dtls_verified = 1;
        res += tls_cookie_len;
      } else {
        *write_packets = 2;
        return buf_len;
      }
    }
    CHECK_SIZE(2, buf_len - res, TLS_NEED_MORE_DATA)
    cipher_len = ntohs(*(unsigned short *)&buf[res]);
    res += 2;
    CHECK_SIZE(cipher_len, buf_len - res, TLS_NEED_MORE_DATA)
    // faster than cipher_len % 2
    if (cipher_len & 1) return TLS_BROKEN_PACKET;

    cipher_buffer = &buf[res];
    res += cipher_len;

    CHECK_SIZE(1, buf_len - res, TLS_NEED_MORE_DATA)
    unsigned char compression_list_size = buf[res++];
    CHECK_SIZE(compression_list_size, buf_len - res, TLS_NEED_MORE_DATA)

    // no compression support
    res += compression_list_size;
  } else {
    CHECK_SIZE(2, buf_len - res, TLS_NEED_MORE_DATA)
    cipher = ntohs(*(unsigned short *)&buf[res]);
    res += 2;
    context->cipher = cipher;
    CHECK_SIZE(1, buf_len - res, TLS_NEED_MORE_DATA)
    unsigned char compression = buf[res++];
    if (compression != 0) {
      DEBUG_PRINT("COMPRESSION NOT SUPPORTED\n");
      return TLS_COMPRESSION_NOT_SUPPORTED;
    }
  }

  if (res > 0) {
    if (context->is_server) *write_packets = 2;
    if (context->connection_status != 4) context->connection_status = 1;
  }

  if (res > 2) res += 2;
  const unsigned char *key_share = NULL;
  unsigned short key_size = 0;
  while (buf_len - res >= 4) {
    // have extensions
    unsigned short extension_type = ntohs(*(unsigned short *)&buf[res]);
    res += 2;
    unsigned short extension_len = ntohs(*(unsigned short *)&buf[res]);
    res += 2;
    DEBUG_PRINT("Extension: 0x0%x (%i), len: %i\n", (int)extension_type,
                (int)extension_type, (int)extension_len);
    if (extension_len) {
      // SNI extension
      CHECK_SIZE(extension_len, buf_len - res, TLS_NEED_MORE_DATA)
      if (extension_type == 0x00) {
        // unsigned short sni_len = ntohs(*(unsigned short *)&buf[res]);
        // unsigned char sni_type = buf[res + 2];
        unsigned short sni_host_len = ntohs(*(unsigned short *)&buf[res + 3]);
        CHECK_SIZE(sni_host_len, buf_len - res - 5, TLS_NEED_MORE_DATA)
        if (sni_host_len) {
          TLS_FREE(context->sni);
          context->sni = (char *)TLS_MALLOC(sni_host_len + 1);
          if (context->sni) {
            memcpy(context->sni, &buf[res + 5], sni_host_len);
            context->sni[sni_host_len] = 0;
            DEBUG_PRINT("SNI HOST INDICATOR: [%s]\n", context->sni);
          }
        }
      } else if (extension_type == 0x0A) {
        // supported groups
        if (buf_len - res > 2) {
          unsigned short group_len = ntohs(*(unsigned short *)&buf[res]);
          if (buf_len - res >= group_len + 2) {
            DEBUG_DUMP_HEX_LABEL("SUPPORTED GROUPS", &buf[res + 2], group_len);
            int i;
            int selected = 0;
            for (i = 0; i < group_len; i += 2) {
              unsigned short iana_n =
                  ntohs(*(unsigned short *)&buf[res + 2 + i]);
              switch (iana_n) {
                case 23:
                  context->curve = &secp256r1;
                  selected = 1;
                  break;
                case 24:
                  context->curve = &secp384r1;
                  selected = 1;
                  break;
                  // // needs different implementation
                  // case 29:
                  //   context->curve = &x25519;
                  //   selected = 1;
                  //   break;
              }
              if (selected) {
                DEBUG_PRINT("SELECTED CURVE %s\n", context->curve->name);
                break;
              }
            }
          }
        }
      } else if ((extension_type == 0x10) && (context->alpn) &&
                 (context->alpn_count)) {
        if (buf_len - res > 2) {
          unsigned short alpn_len = ntohs(*(unsigned short *)&buf[res]);
          if ((alpn_len) && (alpn_len <= extension_len - 2)) {
            unsigned char *alpn = (unsigned char *)&buf[res + 2];
            int alpn_pos = 0;
            while (alpn_pos < alpn_len) {
              unsigned char alpn_size = alpn[alpn_pos++];
              if (alpn_size + alpn_pos >= extension_len) break;
              if ((alpn_size) &&
                  (tls_alpn_contains(context, (char *)&alpn[alpn_pos],
                                     alpn_size))) {
                TLS_FREE(context->negotiated_alpn);
                context->negotiated_alpn = (char *)TLS_MALLOC(alpn_size + 1);
                if (context->negotiated_alpn) {
                  memcpy(context->negotiated_alpn, &alpn[alpn_pos], alpn_size);
                  context->negotiated_alpn[alpn_size] = 0;
                  DEBUG_PRINT("NEGOTIATED ALPN: %s\n",
                              context->negotiated_alpn);
                }
                break;
              }
              alpn_pos += alpn_size;
              // ServerHello contains just one alpn
              if (!context->is_server) break;
            }
          }
        }
      } else if (extension_type == 0x0D) {
        // supported signatures
        DEBUG_DUMP_HEX_LABEL("SUPPORTED SIGNATURES", &buf[res], extension_len);
      } else if (extension_type == 0x0B) {
        // supported point formats
        DEBUG_DUMP_HEX_LABEL("SUPPORTED POINT FORMATS", &buf[res],
                             extension_len);
      } else if (extension_type == 0x2B) {
        // supported versions
        if ((context->is_server) && (buf[res] == extension_len - 1)) {
          if (extension_len > 4) {
            DEBUG_DUMP_HEX_LABEL("SUPPORTED VERSIONS", &buf[res],
                                 extension_len);
            int i;
            int limit = (int)buf[res];
            if (limit == extension_len - 1) {
              limit--;
              for (i = 1; i < limit; i += 2) {
                if ((ntohs(*(unsigned short *)&buf[res + i]) == TLS_V13) ||
                    (ntohs(*(unsigned short *)&buf[res + i]) == 0x7F1C)) {
                  context->version = TLS_V13;
                  context->tls13_version =
                      ntohs(*(unsigned short *)&buf[res + i]);
                  DEBUG_PRINT("TLS 1.3 SUPPORTED\n");
                  break;
                }
              }
            }
          }
        } else if ((!context->is_server) && (extension_len == 2)) {
          if ((ntohs(*(unsigned short *)&buf[res]) == TLS_V13) ||
              (ntohs(*(unsigned short *)&buf[res]) == 0x7F1C)) {
            context->version = TLS_V13;
            context->tls13_version = ntohs(*(unsigned short *)&buf[res]);
            DEBUG_PRINT("TLS 1.3 SUPPORTED\n");
          }
        }
      } else if (extension_type == 0x2A) {
        // early data
        DEBUG_DUMP_HEX_LABEL("EXTENSION, EARLY DATA", &buf[res], extension_len);
      } else if (extension_type == 0x29) {
        // pre shared key
        DEBUG_DUMP_HEX_LABEL("EXTENSION, PRE SHARED KEY", &buf[res],
                             extension_len);
      } else if (extension_type == 0x33) {
        // key share
        if (context->is_server) {
          key_size = ntohs(*(unsigned short *)&buf[res]);
          if ((context->is_server) && (key_size > extension_len - 2)) {
            DEBUG_PRINT("BROKEN KEY SHARE\n");
            return TLS_BROKEN_PACKET;
          }
        } else {
          key_size = extension_len;
        }
        DEBUG_DUMP_HEX_LABEL("EXTENSION, KEY SHARE", &buf[res], extension_len);
        if (context->is_server)
          key_share = &buf[res + 2];
        else
          key_share = &buf[res];
      } else if (extension_type == 0x0D) {
        // signature algorithms
        DEBUG_DUMP_HEX_LABEL("EXTENSION, SIGNATURE ALGORITHMS", &buf[res],
                             extension_len);
      } else if (extension_type == 0x2D) {
        // psk key exchange modes
        DEBUG_DUMP_HEX_LABEL("EXTENSION, PSK KEY EXCHANGE MODES", &buf[res],
                             extension_len);
      }
      res += extension_len;
    }
  }
  if (buf_len != res) return TLS_NEED_MORE_DATA;
  if ((context->is_server) && (cipher_buffer) && (cipher_len)) {
    int chooseCipher =
        tls_choose_cipher(context, cipher_buffer, cipher_len, &scsv_set);
    if (chooseCipher < 0) {
      DEBUG_PRINT("NO COMMON CIPHERS\n");
      return chooseCipher;
    }
    if ((downgraded) && (scsv_set)) {
      DEBUG_PRINT("NO DOWNGRADE (SCSV SET)\n");
      _private_tls_write_packet(
          tls_build_alert(context, 1, inappropriate_fallback));
      context->critical_error = 1;
      return TLS_NOT_SAFE;
    }
    context->cipher = chooseCipher;
  }
  if (!context->is_server) {
    if (!tls_cipher_supported(context, cipher)) {
      context->cipher = 0;
      DEBUG_PRINT("NO CIPHER SUPPORTED\n");
      return TLS_NO_COMMON_CIPHER;
    }
    DEBUG_PRINT("CIPHER: %s\n", tls_cipher_name(context));
  }

  if ((key_share) && (key_size) &&
      ((context->version == TLS_V13) || (context->version == DTLS_V13))) {
    int key_share_err =
        _private_tls_parse_key_share(context, key_share, key_size);
    if (key_share_err) {
      DEBUG_PRINT("keyshare error: %i\n", key_share_err);
      // request hello retry
      if (context->connection_status != 4) {
        *write_packets = 5;
        context->hs_messages[1] = 0;
        context->connection_status = 4;
        return res;
      } else
        return key_share_err;
    }
    DEBUG_PRINT("keyshare success: %i\n", key_share_err);
    // we have key share
    if (context->is_server)
      context->connection_status = 3;
    else
      context->connection_status = 2;
  }
  return res;
}

int tls_parse_certificate(struct TLSContext *context, const unsigned char *buf,
                          int buf_len, int is_client) {
  int res = 0;
  CHECK_SIZE(3, buf_len, TLS_NEED_MORE_DATA)
  unsigned int size_of_all_certificates =
      buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];

  if (size_of_all_certificates <= 4) return 3 + size_of_all_certificates;
  res += 3;
  if (context->dtls) {
    int dtls_check = _private_dtls_check_packet(buf, buf_len);
    if (dtls_check < 0) return dtls_check;
    res += 8;
  }
  if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
    int context_size = buf[res];
    res++;
    // must be 0
    if (context_size) res += context_size;
  }

  CHECK_SIZE(size_of_all_certificates, buf_len - res, TLS_NEED_MORE_DATA);
  int size = size_of_all_certificates;

  int idx = 0;
  int valid_certificate = 0;
  while (size > 0) {
    idx++;
    CHECK_SIZE(3, buf_len - res, TLS_NEED_MORE_DATA);
    unsigned int certificate_size =
        buf[res] * 0x10000 + buf[res + 1] * 0x100 + buf[res + 2];
    res += 3;
    CHECK_SIZE(certificate_size, buf_len - res, TLS_NEED_MORE_DATA)
    // load chain
    int certificates_in_chain = 0;
    int res2 = res;
    unsigned int remaining = certificate_size;
    do {
      if (remaining <= 3) break;
      certificates_in_chain++;
      unsigned int certificate_size2 =
          buf[res2] * 0x10000 + buf[res2 + 1] * 0x100 + buf[res2 + 2];
      res2 += 3;
      remaining -= 3;
      if (certificate_size2 > remaining) {
        DEBUG_PRINT("Invalid certificate size (%i from %i bytes remaining)\n",
                    certificate_size2, remaining);
        break;
      }
      remaining -= certificate_size2;

      struct TLSCertificate *cert =
          asn1_parse(context, &buf[res2], certificate_size2, is_client);
      if (cert) {
        if (certificate_size2) {
          cert->bytes = (unsigned char *)TLS_MALLOC(certificate_size2);
          if (cert->bytes) {
            cert->len = certificate_size2;
            memcpy(cert->bytes, &buf[res2], certificate_size2);
          }
        }
        // valid certificate
        if (is_client) {
        } else {
          context->certificates = (struct TLSCertificate **)TLS_REALLOC(
              context->certificates, (context->certificates_count + 1) *
                                         sizeof(struct TLSCertificate *));
          context->certificates[context->certificates_count] = cert;
          context->certificates_count++;
          if ((cert->pk) || (cert->priv))
            valid_certificate = 1;
          else if (!context->is_server)
            valid_certificate = 1;
        }
      }
      res2 += certificate_size2;
      // extension
      if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
        if (remaining >= 2) {
          // ignore extensions
          remaining -= 2;
          unsigned short sz = ntohs(*(unsigned short *)&buf[res2]);
          if ((sz) && (sz >= remaining)) {
            res2 += sz;
            remaining -= sz;
          }
        }
      }
    } while (remaining > 0);
    if (remaining) {
      DEBUG_PRINT("Extra %i bytes after certificate\n", remaining);
    }
    size -= certificate_size + 3;
    res += certificate_size;
  }
  if (!valid_certificate) return TLS_UNSUPPORTED_CERTIFICATE;
  if (res != buf_len) {
    DEBUG_PRINT("Warning: %i bytes read from %i byte buffer\n", (int)res,
                (int)buf_len);
  }
  return res;
}

int _private_tls_parse_dh(const unsigned char *buf, int buf_len,
                          const unsigned char **out, int *out_size) {
  int res = 0;
  *out = NULL;
  *out_size = 0;
  CHECK_SIZE(2, buf_len, TLS_NEED_MORE_DATA)
  unsigned short size = ntohs(*(unsigned short *)buf);
  res += 2;
  CHECK_SIZE(size, buf_len - res, TLS_NEED_MORE_DATA)
  DEBUG_DUMP_HEX(&buf[res], size);
  *out = &buf[res];
  *out_size = size;
  res += size;
  return res;
}

int _private_tls_parse_random(struct TLSContext *context,
                              const unsigned char *buf, int buf_len) {
  int res = 0;
  int ephemeral = tls_cipher_is_ephemeral(context);
  unsigned short size;
  if (ephemeral == 2) {
    CHECK_SIZE(1, buf_len, TLS_NEED_MORE_DATA)
    size = buf[0];
    res += 1;
  } else {
    CHECK_SIZE(2, buf_len, TLS_NEED_MORE_DATA)
    size = ntohs(*(unsigned short *)buf);
    res += 2;
  }

  CHECK_SIZE(size, buf_len - res, TLS_NEED_MORE_DATA)
  unsigned int out_len = 0;
  unsigned char *random = NULL;
  switch (ephemeral) {
    case 1:
      random = _private_tls_decrypt_dhe(context, &buf[res], size, &out_len, 1);
      break;
    case 2:
      random =
          _private_tls_decrypt_ecc_dhe(context, &buf[res], size, &out_len, 1);
      break;
    default:
      random = _private_tls_decrypt_rsa(context, &buf[res], size, &out_len);
  }

  if ((random) && (out_len > 2)) {
    DEBUG_DUMP_HEX_LABEL("PRE MASTER KEY", random, out_len);
    TLS_FREE(context->premaster_key);
    context->premaster_key = random;
    context->premaster_key_len = out_len;
    _private_tls_compute_key(context, 48);
  } else {
    TLS_FREE(random);
    return 0;
  }
  res += size;
  return res;
}

int _private_tls_build_random(struct TLSPacket *packet) {
  int res = 0;
  unsigned char rand_bytes[48];
  int bytes = 48;
  if (!tls_random(rand_bytes, bytes)) return TLS_GENERIC_ERROR;

  // max supported version
  if (packet->context->is_server)
    *(unsigned short *)rand_bytes = htons(packet->context->version);
  else if (packet->context->dtls)
    *(unsigned short *)rand_bytes = htons(DTLS_V12);
  else
    *(unsigned short *)rand_bytes = htons(TLS_V12);
  // DEBUG_DUMP_HEX_LABEL("PREMASTER KEY", rand_bytes, bytes);

  TLS_FREE(packet->context->premaster_key);
  packet->context->premaster_key = (unsigned char *)TLS_MALLOC(bytes);
  if (!packet->context->premaster_key) return TLS_NO_MEMORY;

  packet->context->premaster_key_len = bytes;
  memcpy(packet->context->premaster_key, rand_bytes,
         packet->context->premaster_key_len);

  unsigned int out_len;
  unsigned char *random =
      _private_tls_encrypt_rsa(packet->context, packet->context->premaster_key,
                               packet->context->premaster_key_len, &out_len);

  _private_tls_compute_key(packet->context, bytes);
  if ((random) && (out_len > 2)) {
    tls_packet_uint24(packet, out_len + 2);
    if (packet->context->dtls)
      _private_dtls_handshake_data(packet->context, packet, out_len + 2);
    tls_packet_uint16(packet, out_len);
    tls_packet_append(packet, random, out_len);
  } else
    res = TLS_GENERIC_ERROR;
  TLS_FREE(random);
  if (res) return res;

  return out_len + 2;
}

const unsigned char *_private_tls_parse_signature(
    struct TLSContext *context, const unsigned char *buf, int buf_len,
    int *hash_algorithm, int *sign_algorithm, int *sig_size, int *offset) {
  int res = 0;
  CHECK_SIZE(2, buf_len, NULL)
  *hash_algorithm = _md5_sha1;
  *sign_algorithm = rsa_sign;
  *sig_size = 0;
  if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
      (context->version == TLS_V13) || (context->version == DTLS_V13)) {
    *hash_algorithm = buf[res];
    res++;
    *sign_algorithm = buf[res];
    res++;
  }
  unsigned short size = ntohs(*(unsigned short *)&buf[res]);
  res += 2;
  CHECK_SIZE(size, buf_len - res, NULL)
  DEBUG_DUMP_HEX(&buf[res], size);
  *sig_size = size;
  *offset = res + size;
  return &buf[res];
}

int tls_parse_server_key_exchange(struct TLSContext *context,
                                  const unsigned char *buf, int buf_len) {
  int res = 0;
  int dh_res = 0;
  CHECK_SIZE(3, buf_len, TLS_NEED_MORE_DATA)
  unsigned int size = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  res += 3;
  if (context->dtls) {
    int dtls_check = _private_dtls_check_packet(buf, buf_len);
    if (dtls_check < 0) return dtls_check;
    res += 8;
  }
  const unsigned char *packet_ref = buf + res;
  CHECK_SIZE(size, buf_len - res, TLS_NEED_MORE_DATA);

  if (!size) return res;

  unsigned char has_ds_params = 0;
  unsigned int key_size = 0;
  const struct ECCCurveParameters *curve = NULL;
  const unsigned char *pk_key = NULL;
  int ephemeral = tls_cipher_is_ephemeral(context);
  if (ephemeral) {
    if (ephemeral == 1) {
      has_ds_params = 1;
    } else {
      if (buf[res++] != 3) {
        // named curve
        // any other method is not supported
        return 0;
      }
      CHECK_SIZE(3, buf_len - res, TLS_NEED_MORE_DATA);
      int iana_n = ntohs(*(unsigned short *)&buf[res]);
      res += 2;
      key_size = buf[res];
      res++;
      CHECK_SIZE(key_size, buf_len - res, TLS_NEED_MORE_DATA);
      DEBUG_PRINT("IANA CURVE NUMBER: %i\n", iana_n);
      switch (iana_n) {
        case 19:
          curve = &secp192r1;
          break;
        case 20:
          curve = &secp224k1;
          break;
        case 21:
          curve = &secp224r1;
          break;
        case 22:
          curve = &secp256k1;
          break;
        case 23:
          curve = &secp256r1;
          break;
        case 24:
          curve = &secp384r1;
          break;
        case 25:
          curve = &secp521r1;
          break;
        case 29:
          curve = &x25519;
          break;
        default:
          DEBUG_PRINT("UNSUPPORTED CURVE\n");
          return TLS_GENERIC_ERROR;
      }
      pk_key = &buf[res];
      res += key_size;
      context->curve = curve;
    }
  }
  const unsigned char *dh_p = NULL;
  int dh_p_len = 0;
  const unsigned char *dh_g = NULL;
  int dh_g_len = 0;
  const unsigned char *dh_Ys = NULL;
  int dh_Ys_len = 0;
  if (has_ds_params) {
    DEBUG_PRINT("          dh_p: ");
    dh_res = _private_tls_parse_dh(&buf[res], buf_len - res, &dh_p, &dh_p_len);
    if (dh_res <= 0) return TLS_BROKEN_PACKET;
    res += dh_res;
    DEBUG_PRINT("\n");

    DEBUG_PRINT("          dh_q: ");
    dh_res = _private_tls_parse_dh(&buf[res], buf_len - res, &dh_g, &dh_g_len);
    if (dh_res <= 0) return TLS_BROKEN_PACKET;
    res += dh_res;
    DEBUG_PRINT("\n");

    DEBUG_PRINT("          dh_Ys: ");
    dh_res =
        _private_tls_parse_dh(&buf[res], buf_len - res, &dh_Ys, &dh_Ys_len);
    if (dh_res <= 0) return TLS_BROKEN_PACKET;
    res += dh_res;
    DEBUG_PRINT("\n");
  }
  int sign_size;
  int hash_algorithm;
  int sign_algorithm;
  int packet_size = res - 3;
  if (context->dtls) packet_size -= 8;
  int offset = 0;
  DEBUG_PRINT("          SIGNATURE (%i/%i/%i): ", packet_size, dh_res,
              key_size);
  const unsigned char *signature = _private_tls_parse_signature(
      context, &buf[res], buf_len - res, &hash_algorithm, &sign_algorithm,
      &sign_size, &offset);
  DEBUG_PRINT("\n");
  if ((sign_size <= 0) || (!signature)) return TLS_BROKEN_PACKET;
  res += offset;
  // check signature
  unsigned int message_len =
      packet_size + TLS_CLIENT_RANDOM_SIZE + TLS_SERVER_RANDOM_SIZE;
  unsigned char *message = (unsigned char *)TLS_MALLOC(message_len);
  if (message) {
    memcpy(message, context->local_random, TLS_CLIENT_RANDOM_SIZE);
    memcpy(message + TLS_CLIENT_RANDOM_SIZE, context->remote_random,
           TLS_SERVER_RANDOM_SIZE);
    memcpy(message + TLS_CLIENT_RANDOM_SIZE + TLS_SERVER_RANDOM_SIZE,
           packet_ref, packet_size);
    if (tls_is_ecdsa(context)) {
      if (_private_tls_verify_ecdsa(context, hash_algorithm, signature,
                                    sign_size, message, message_len,
                                    NULL) != 1) {
        DEBUG_PRINT("ECC Server signature FAILED!\n");
        TLS_FREE(message);
        return TLS_BROKEN_PACKET;
      }
    } else {
      if (_private_tls_verify_rsa(context, hash_algorithm, signature, sign_size,
                                  message, message_len) != 1) {
        DEBUG_PRINT("Server signature FAILED!\n");
        TLS_FREE(message);
        return TLS_BROKEN_PACKET;
      }
    }
    TLS_FREE(message);
  }

  if (buf_len - res) {
    DEBUG_PRINT("EXTRA %i BYTES AT THE END OF MESSAGE\n", buf_len - res);
    DEBUG_DUMP_HEX(&buf[res], buf_len - res);
    DEBUG_PRINT("\n");
  }
  if (ephemeral == 1) {
    _private_tls_dhe_create(context);
    DEBUG_DUMP_HEX_LABEL("DHP", dh_p, dh_p_len);
    DEBUG_DUMP_HEX_LABEL("DHG", dh_g, dh_g_len);
    int dhe_key_size = dh_p_len;
    if (dh_g_len > dh_p_len) dhe_key_size = dh_g_len;
    if (_private_tls_dh_make_key(dhe_key_size, context->dhe, (const char *)dh_p,
                                 (const char *)dh_g, dh_p_len, dh_g_len)) {
      DEBUG_PRINT("ERROR CREATING DHE KEY\n");
      TLS_FREE(context->dhe);
      context->dhe = NULL;
      return TLS_GENERIC_ERROR;
    }

    unsigned int dh_key_size = 0;
    unsigned char *key =
        _private_tls_decrypt_dhe(context, dh_Ys, dh_Ys_len, &dh_key_size, 0);
    DEBUG_DUMP_HEX_LABEL("DH COMMON SECRET", key, dh_key_size);
    if ((key) && (dh_key_size)) {
      TLS_FREE(context->premaster_key);
      context->premaster_key = key;
      context->premaster_key_len = dh_key_size;
    }
  } else if ((ephemeral == 2) && (curve) && (pk_key) && (key_size)) {
    if (context->is_server && curve == &x25519) {
      if (key_size != 32) {
        DEBUG_PRINT("INVALID X25519 PUBLIC SIZE");
        return TLS_GENERIC_ERROR;
      }

      TLS_FREE(context->client_secret);
      context->client_secret = (unsigned char *)TLS_MALLOC(32);
      if (!context->client_secret) {
        DEBUG_PRINT("ERROR IN TLS_MALLOC");
        return TLS_GENERIC_ERROR;
      }

      tls_random(context->client_secret, 32);

      context->client_secret[0] &= 248;
      context->client_secret[31] &= 127;
      context->client_secret[31] |= 64;

      TLS_FREE(context->premaster_key);
      context->premaster_key = (unsigned char *)TLS_MALLOC(32);
      if (!context->premaster_key) return TLS_GENERIC_ERROR;

      curve25519(context->premaster_key, context->client_secret, pk_key);
      context->premaster_key_len = 32;
    } else {
      tls_init();
      _private_tls_ecc_dhe_create(context);

      const ltc_ecc_set_type *dp = &curve->dp;
      if (ecc_make_key_ex(NULL, find_prng("sprng"), context->ecc_dhe, dp)) {
        TLS_FREE(context->ecc_dhe);
        context->ecc_dhe = NULL;
        DEBUG_PRINT("Error generating ECC key\n");
        return TLS_GENERIC_ERROR;
      }

      TLS_FREE(context->premaster_key);
      context->premaster_key_len = 0;

      unsigned int out_len = 0;
      context->premaster_key =
          _private_tls_decrypt_ecc_dhe(context, pk_key, key_size, &out_len, 0);
      if (context->premaster_key) context->premaster_key_len = out_len;
    }
  }
  return res;
}

int tls_parse_client_key_exchange(struct TLSContext *context,
                                  const unsigned char *buf, int buf_len) {
  if (context->connection_status != 1) {
    DEBUG_PRINT(
        "UNEXPECTED CLIENT KEY EXCHANGE MESSAGE (connections status: %i)\n",
        (int)context->connection_status);
    return TLS_UNEXPECTED_MESSAGE;
  }

  int res = 0;
  int dh_res = 0;
  CHECK_SIZE(3, buf_len, TLS_NEED_MORE_DATA)

  unsigned int size = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  res += 3;
  if (context->dtls) {
    int dtls_check = _private_dtls_check_packet(buf, buf_len);
    if (dtls_check < 0) return dtls_check;
    res += 8;
  }

  CHECK_SIZE(size, buf_len - res, TLS_NEED_MORE_DATA);

  if (!size) return res;

  dh_res = _private_tls_parse_random(context, &buf[res], size);
  if (dh_res <= 0) {
    DEBUG_PRINT("broken key\n");
    return TLS_BROKEN_PACKET;
  }
  DEBUG_PRINT("\n");

  res += size;
  context->connection_status = 2;
  return res;
}

int tls_parse_server_hello_done(struct TLSContext *context,
                                const unsigned char *buf, int buf_len) {
  int res = 0;
  CHECK_SIZE(3, buf_len, TLS_NEED_MORE_DATA)

  unsigned int size = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  res += 3;
  if (context->dtls) {
    int dtls_check = _private_dtls_check_packet(buf, buf_len);
    if (dtls_check < 0) return dtls_check;
    res += 8;
  }

  CHECK_SIZE(size, buf_len - res, TLS_NEED_MORE_DATA);

  res += size;
  return res;
}

int tls_parse_finished(struct TLSContext *context, const unsigned char *buf,
                       int buf_len, unsigned int *write_packets) {
  if ((context->connection_status < 2) ||
      (context->connection_status == 0xFF)) {
    DEBUG_PRINT("UNEXPECTED FINISHED MESSAGE\n");
    return TLS_UNEXPECTED_MESSAGE;
  }

  int res = 0;
  *write_packets = 0;
  CHECK_SIZE(3, buf_len, TLS_NEED_MORE_DATA)

  unsigned int size = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  res += 3;
  if (context->dtls) {
    int dtls_check = _private_dtls_check_packet(buf, buf_len);
    if (dtls_check < 0) return dtls_check;
    res += 8;
  }

  if (size < TLS_MIN_FINISHED_OPAQUE_LEN) {
    DEBUG_PRINT("Invalid finished pachet size: %i\n", size);
    return TLS_BROKEN_PACKET;
  }

  CHECK_SIZE(size, buf_len - res, TLS_NEED_MORE_DATA);

  unsigned char hash[TLS_MAX_SHA_SIZE];
  unsigned int hash_len = _private_tls_get_hash(context, hash);

  if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
    unsigned char hash_out[TLS_MAX_SHA_SIZE];
    unsigned long out_size = TLS_MAX_SHA_SIZE;
    if ((!context->remote_finished_key) || (!hash_len)) {
      DEBUG_PRINT("NO FINISHED KEY COMPUTED OR NO HANDSHAKE HASH\n");
      return TLS_NOT_VERIFIED;
    }

    DEBUG_DUMP_HEX_LABEL("HS HASH", hash, hash_len);
    DEBUG_DUMP_HEX_LABEL("HS FINISH", context->finished_key, hash_len);
    DEBUG_DUMP_HEX_LABEL("HS REMOTE FINISH", context->remote_finished_key,
                         hash_len);

    out_size = hash_len;
    hmac_state hmac;
    hmac_init(&hmac, _private_tls_get_hash_idx(context),
              context->remote_finished_key, hash_len);
    hmac_process(&hmac, hash, hash_len);
    hmac_done(&hmac, hash_out, &out_size);

    if ((size != out_size) || (memcmp(hash_out, &buf[res], size))) {
      DEBUG_PRINT(
          "Finished validation error (sequence number, local: %i, remote: "
          "%i)\n",
          (int)context->local_sequence_number,
          (int)context->remote_sequence_number);
      DEBUG_DUMP_HEX_LABEL("FINISHED OPAQUE", &buf[res], size);
      DEBUG_DUMP_HEX_LABEL("VERIFY", hash_out, out_size);
      return TLS_NOT_VERIFIED;
    }
    if (context->is_server) {
      context->connection_status = 0xFF;
      DEBUG_PRINT("CONNECTION STATUS: FF (tls_parse_finished)\n");
      res += size;
      _private_tls13_key(context, 0);
      context->local_sequence_number = 0;
      context->remote_sequence_number = 0;
      return res;
    }
  } else {
    // verify
    unsigned char *out = (unsigned char *)TLS_MALLOC(size);
    if (!out) {
      DEBUG_PRINT("Error in TLS_MALLOC (%i bytes)\n", (int)size);
      return TLS_NO_MEMORY;
    }

    // server verifies client's message
    if (context->is_server)
      _private_tls_prf(
          context, out, size, context->master_key, context->master_key_len,
          (unsigned char *)"client finished", 15, hash, hash_len, NULL, 0);
    else
      _private_tls_prf(
          context, out, size, context->master_key, context->master_key_len,
          (unsigned char *)"server finished", 15, hash, hash_len, NULL, 0);

    if (memcmp(out, &buf[res], size)) {
      TLS_FREE(out);
      DEBUG_PRINT(
          "Finished validation error (sequence number, local: %i, remote: "
          "%i)\n",
          (int)context->local_sequence_number,
          (int)context->remote_sequence_number);
      DEBUG_DUMP_HEX_LABEL("FINISHED OPAQUE", &buf[res], size);
      DEBUG_DUMP_HEX_LABEL("VERIFY", out, size);
      return TLS_NOT_VERIFIED;
    }
    TLS_FREE(out);
  }
  if (context->is_server)
    *write_packets = 3;
  else {
    DEBUG_PRINT("CONNECTION STATUS: FF (tls_parse_finished 2)\n");
    context->connection_status = 0xFF;
  }

  res += size;
  return res;
}

int tls_parse_verify_tls13(struct TLSContext *context, const unsigned char *buf,
                           int buf_len) {
  CHECK_SIZE(7, buf_len, TLS_NEED_MORE_DATA)
  unsigned int size = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];

  if (size < 2) return buf_len;

  unsigned char signing_data[TLS_MAX_HASH_SIZE + 98];
  int signing_data_len;

  // first 64 bytes to 0x20 (32)
  memset(signing_data, 0x20, 64);
  // context string 33 bytes
  if (context->is_server)
    memcpy(signing_data + 64, "TLS 1.3, server CertificateVerify", 33);
  else
    memcpy(signing_data + 64, "TLS 1.3, client CertificateVerify", 33);
  // a single 0 byte separator
  signing_data[97] = 0;
  signing_data_len = 98;

  signing_data_len += _private_tls_get_hash(context, signing_data + 98);
  DEBUG_DUMP_HEX_LABEL("signature data", signing_data, signing_data_len);
  unsigned short signature = ntohs(*(unsigned short *)&buf[3]);
  unsigned short signature_size = ntohs(*(unsigned short *)&buf[5]);
  int valid = 0;
  CHECK_SIZE(7 + size, buf_len, TLS_NEED_MORE_DATA)
  switch (signature) {
    case 0x0403:
      // secp256r1 + sha256
      valid =
          _private_tls_verify_ecdsa(context, sha256, buf + 7, signature_size,
                                    signing_data, signing_data_len, &secp256r1);
      break;
    case 0x0503:
      // secp384r1 + sha384
      valid =
          _private_tls_verify_ecdsa(context, sha384, buf + 7, signature_size,
                                    signing_data, signing_data_len, &secp384r1);
      break;
    case 0x0603:
      // secp521r1 + sha512
      valid =
          _private_tls_verify_ecdsa(context, sha512, buf + 7, signature_size,
                                    signing_data, signing_data_len, &secp521r1);
      break;
    case 0x0804:
      valid = _private_tls_verify_rsa(context, sha256, buf + 7, signature_size,
                                      signing_data, signing_data_len);
      break;
    default:
      DEBUG_PRINT("Unsupported signature: %x\n", (int)signature);
      return TLS_UNSUPPORTED_CERTIFICATE;
  }
  if (valid != 1) {
    DEBUG_PRINT("Signature FAILED!\n");
    return TLS_DECRYPTION_FAILED;
  }
  return buf_len;
}

int tls_parse_verify(struct TLSContext *context, const unsigned char *buf,
                     int buf_len) {
  if ((context->version == TLS_V13) || (context->version == DTLS_V13))
    return tls_parse_verify_tls13(context, buf, buf_len);
  CHECK_SIZE(7, buf_len, TLS_BAD_CERTIFICATE)
  unsigned int bytes_to_follow = buf[0] * 0x10000 + buf[1] * 0x100 + buf[2];
  CHECK_SIZE(bytes_to_follow, buf_len - 3, TLS_BAD_CERTIFICATE)
  int res = -1;

  if ((context->version == TLS_V12) || (context->version == DTLS_V12) ||
      (context->version == TLS_V13) || (context->version == DTLS_V13)) {
    unsigned int hash = buf[3];
    unsigned int algorithm = buf[4];
    if (algorithm != rsa) return TLS_UNSUPPORTED_CERTIFICATE;
    unsigned short size = ntohs(*(unsigned short *)&buf[5]);
    CHECK_SIZE(size, bytes_to_follow - 4, TLS_BAD_CERTIFICATE)
    DEBUG_PRINT("ALGORITHM %i/%i (%i)\n", hash, algorithm, (int)size);
    DEBUG_DUMP_HEX_LABEL("VERIFY", &buf[7], bytes_to_follow - 7);

    res = _private_tls_verify_rsa(context, hash, &buf[7], size,
                                  context->cached_handshake,
                                  context->cached_handshake_len);
  } else {
    unsigned short size = ntohs(*(unsigned short *)&buf[3]);
    CHECK_SIZE(size, bytes_to_follow - 2, TLS_BAD_CERTIFICATE)
    res = _private_tls_verify_rsa(context, md5, &buf[5], size,
                                  context->cached_handshake,
                                  context->cached_handshake_len);
  }
  if (context->cached_handshake) {
    // not needed anymore
    TLS_FREE(context->cached_handshake);
    context->cached_handshake = NULL;
    context->cached_handshake_len = 0;
  }
  if (res == 1) {
    DEBUG_PRINT("Signature OK\n");
    context->client_verified = 1;
  } else {
    DEBUG_PRINT("Signature FAILED\n");
    context->client_verified = 0;
  }
  return 1;
}

int tls_parse_payload(struct TLSContext *context, const unsigned char *buf,
                      int buf_len, tls_validation_function certificate_verify) {
  int orig_len = buf_len;
  if (context->connection_status == 0xFF) {
    // renegotiation disabled (emit warning alert)
    _private_tls_write_packet(tls_build_alert(context, 0, no_renegotiation));
    return 1;
  }

  while ((buf_len >= 4) && (!context->critical_error)) {
    int payload_res = 0;
    unsigned char update_hash = 1;
    CHECK_SIZE(1, buf_len, TLS_NEED_MORE_DATA)
    unsigned char type = buf[0];
    unsigned int write_packets = 0;
    unsigned int dtls_cookie_verified = 0;
    int certificate_verify_alert = no_error;
    unsigned int payload_size = buf[1] * 0x10000 + buf[2] * 0x100 + buf[3] + 3;
    if (context->dtls) payload_size += 8;
    CHECK_SIZE(payload_size + 1, buf_len, TLS_NEED_MORE_DATA)
    switch (type) {
      // hello request
      case 0x00:
        CHECK_HANDSHAKE_STATE(context, 0, 1);
        DEBUG_PRINT(" => HELLO REQUEST (RENEGOTIATION?)\n");
        if (context->dtls) context->dtls_seq = 0;
        if (context->is_server)
          payload_res = TLS_UNEXPECTED_MESSAGE;
        else {
          if (context->connection_status == 0xFF) {
            // renegotiation
            payload_res = TLS_NO_RENEGOTIATION;
          } else
            payload_res = TLS_UNEXPECTED_MESSAGE;
        }
        // no payload
        break;
        // client hello
      case 0x01:
        CHECK_HANDSHAKE_STATE(context, 1, (context->dtls ? 2 : 1));
        DEBUG_PRINT(" => CLIENT HELLO\n");
        if (context->is_server) {
          payload_res = tls_parse_hello(context, buf + 1, payload_size,
                                        &write_packets, &dtls_cookie_verified);
          DEBUG_PRINT(" => DTLS COOKIE VERIFIED: %i (%i)\n",
                      dtls_cookie_verified, payload_res);
          if ((context->dtls) && (payload_res > 0) && (!dtls_cookie_verified) &&
              (context->connection_status == 1)) {
            // wait client hello
            context->connection_status = 3;
            update_hash = 0;
          }
        } else
          payload_res = TLS_UNEXPECTED_MESSAGE;
        break;
        // server hello
      case 0x02:
        CHECK_HANDSHAKE_STATE(context, 2, 1);
        DEBUG_PRINT(" => SERVER HELLO\n");
        if (context->is_server)
          payload_res = TLS_UNEXPECTED_MESSAGE;
        else
          payload_res = tls_parse_hello(context, buf + 1, payload_size,
                                        &write_packets, &dtls_cookie_verified);
        break;
        // hello verify request
      case 0x03:
        DEBUG_PRINT(" => VERIFY REQUEST\n");
        CHECK_HANDSHAKE_STATE(context, 3, 1);
        if ((context->dtls) && (!context->is_server)) {
          payload_res = tls_parse_verify_request(context, buf + 1, payload_size,
                                                 &write_packets);
          update_hash = 0;
        } else
          payload_res = TLS_UNEXPECTED_MESSAGE;
        break;
        // certificate
      case 0x0B:
        CHECK_HANDSHAKE_STATE(context, 4, 1);
        DEBUG_PRINT(" => CERTIFICATE\n");
        if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
          if (context->connection_status == 2) {
            payload_res = tls_parse_certificate(context, buf + 1, payload_size,
                                                context->is_server);
            if (context->is_server) {
              // empty certificates are permitted for client
              if (payload_res <= 0) payload_res = 1;
            }
          } else
            payload_res = TLS_UNEXPECTED_MESSAGE;
        } else if (context->connection_status == 1) {
          if (context->is_server) {
            // client certificate
            payload_res =
                tls_parse_certificate(context, buf + 1, payload_size, 1);

            // empty certificates are permitted for client
            if (payload_res <= 0) payload_res = 1;
          } else {
            payload_res =
                tls_parse_certificate(context, buf + 1, payload_size, 0);
            if ((certificate_verify) && (context->certificates_count))
              certificate_verify_alert = certificate_verify(
                  context, context->certificates, context->certificates_count);
          }
        } else
          payload_res = TLS_UNEXPECTED_MESSAGE;
        break;
        // server key exchange
      case 0x0C:
        CHECK_HANDSHAKE_STATE(context, 5, 1);
        DEBUG_PRINT(" => SERVER KEY EXCHANGE\n");
        if (context->is_server)
          payload_res = TLS_UNEXPECTED_MESSAGE;
        else
          payload_res =
              tls_parse_server_key_exchange(context, buf + 1, payload_size);
        break;
        // certificate request
      case 0x0D:
        CHECK_HANDSHAKE_STATE(context, 6, 1);
        // server to client
        if (context->is_server)
          payload_res = TLS_UNEXPECTED_MESSAGE;
        else
          context->client_verified = 2;
        DEBUG_PRINT(" => CERTIFICATE REQUEST\n");
        break;
        // server hello done
      case 0x0E:
        CHECK_HANDSHAKE_STATE(context, 7, 1);
        DEBUG_PRINT(" => SERVER HELLO DONE\n");
        if (context->is_server) {
          payload_res = TLS_UNEXPECTED_MESSAGE;
        } else {
          payload_res =
              tls_parse_server_hello_done(context, buf + 1, payload_size);
          if (payload_res > 0) write_packets = 1;
        }
        break;
        // certificate verify
      case 0x0F:
        CHECK_HANDSHAKE_STATE(context, 8, 1);
        DEBUG_PRINT(" => CERTIFICATE VERIFY\n");
        if (context->connection_status == 2)
          payload_res = tls_parse_verify(context, buf + 1, payload_size);
        else
          payload_res = TLS_UNEXPECTED_MESSAGE;
        break;
        // client key exchange
      case 0x10:
        CHECK_HANDSHAKE_STATE(context, 9, 1);
        DEBUG_PRINT(" => CLIENT KEY EXCHANGE\n");
        if (context->is_server)
          payload_res =
              tls_parse_client_key_exchange(context, buf + 1, payload_size);
        else
          payload_res = TLS_UNEXPECTED_MESSAGE;
        break;
        // finished
      case 0x14:
        if (context->cached_handshake) {
          TLS_FREE(context->cached_handshake);
          context->cached_handshake = NULL;
          context->cached_handshake_len = 0;
        }
        CHECK_HANDSHAKE_STATE(context, 10, 1);
        DEBUG_PRINT(" => FINISHED\n");
        payload_res =
            tls_parse_finished(context, buf + 1, payload_size, &write_packets);
        if (payload_res > 0)
          memset(context->hs_messages, 0, sizeof(context->hs_messages));
        if ((!context->is_server) &&
            ((context->version == TLS_V13) || (context->version == DTLS_V13))) {
          update_hash = 0;
          DEBUG_PRINT("<= SENDING FINISHED\n");
          _private_tls_update_hash(context, buf, payload_size + 1);
          _private_tls_write_packet(tls_build_finished(context));
          _private_tls13_key(context, 0);
          context->connection_status = 0xFF;
          DEBUG_PRINT("CONNECTION STATUS: FF (tls_parse_payload)\n");
          context->local_sequence_number = 0;
          context->remote_sequence_number = 0;
        }
        break;
      case 0x08:
        // encrypted extensions ... ignore it for now
        break;
      default:
        DEBUG_PRINT(" => NOT UNDERSTOOD PAYLOAD TYPE: %x\n", (int)type);
        return TLS_NOT_UNDERSTOOD;
    }
    if ((type != 0x00) && (update_hash))
      _private_tls_update_hash(context, buf, payload_size + 1);

    if (certificate_verify_alert != no_error) {
      _private_tls_write_packet(
          tls_build_alert(context, 1, certificate_verify_alert));
      context->critical_error = 1;
    }

    if (payload_res < 0) {
      switch (payload_res) {
        case TLS_UNEXPECTED_MESSAGE:
          _private_tls_write_packet(
              tls_build_alert(context, 1, unexpected_message));
          break;
        case TLS_COMPRESSION_NOT_SUPPORTED:
          _private_tls_write_packet(
              tls_build_alert(context, 1, decompression_failure));
          break;
        case TLS_BROKEN_PACKET:
          _private_tls_write_packet(tls_build_alert(context, 1, decode_error));
          break;
        case TLS_NO_MEMORY:
          _private_tls_write_packet(
              tls_build_alert(context, 1, internal_error));
          break;
        case TLS_NOT_VERIFIED:
          _private_tls_write_packet(
              tls_build_alert(context, 1, bad_record_mac));
          break;
        case TLS_BAD_CERTIFICATE:
          if (context->is_server) {
            // bad client certificate, continue
            _private_tls_write_packet(
                tls_build_alert(context, 0, bad_certificate));
            payload_res = 0;
          } else
            _private_tls_write_packet(
                tls_build_alert(context, 1, bad_certificate));
          break;
        case TLS_UNSUPPORTED_CERTIFICATE:
          _private_tls_write_packet(
              tls_build_alert(context, 1, unsupported_certificate));
          break;
        case TLS_NO_COMMON_CIPHER:
          _private_tls_write_packet(
              tls_build_alert(context, 1, insufficient_security));
          break;
        case TLS_NOT_UNDERSTOOD:
          _private_tls_write_packet(
              tls_build_alert(context, 1, internal_error));
          break;
        case TLS_NO_RENEGOTIATION:
          _private_tls_write_packet(
              tls_build_alert(context, 0, no_renegotiation));
          payload_res = 0;
          break;
        case TLS_DECRYPTION_FAILED:
          _private_tls_write_packet(
              tls_build_alert(context, 1, decryption_failed_RESERVED));
          break;
      }
      if (payload_res < 0) return payload_res;
    }
    if (certificate_verify_alert != no_error) payload_res = TLS_BAD_CERTIFICATE;

    // except renegotiation
    switch (write_packets) {
      case 1:
        if (context->client_verified == 2) {
          DEBUG_PRINT("<= Building CERTIFICATE \n");
          _private_tls_write_packet(tls_build_certificate(context));
          context->client_verified = 0;
        }
        // client handshake
        DEBUG_PRINT("<= Building KEY EXCHANGE\n");
        _private_tls_write_packet(tls_build_client_key_exchange(context));
        DEBUG_PRINT("<= Building CHANGE CIPHER SPEC\n");
        _private_tls_write_packet(tls_build_change_cipher_spec(context));
        context->cipher_spec_set = 1;
        context->local_sequence_number = 0;
        DEBUG_PRINT("<= Building CLIENT FINISHED\n");
        _private_tls_write_packet(tls_build_finished(context));
        context->cipher_spec_set = 0;
        if ((!context->is_server) && (context->version == TLS_V12)) {
          // https://tools.ietf.org/html/rfc7918
          // 5.1.  Symmetric Cipher
          // Clients MUST NOT use the False Start protocol modification in a
          // handshake unless the cipher suite uses a symmetric cipher that is
          // considered cryptographically strong.
          switch (context->cipher) {
            case TLS_RSA_WITH_AES_128_GCM_SHA256:
            case TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:
            case TLS_DHE_RSA_WITH_AES_128_GCM_SHA256:
            case TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:
            case TLS_RSA_WITH_AES_256_GCM_SHA384:
            case TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:
            case TLS_DHE_RSA_WITH_AES_256_GCM_SHA384:
            case TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:
              context->false_start = 1;
              break;
          }
        }
        break;
      case 2:
        // server handshake
        if ((context->dtls) && (dtls_cookie_verified == 0)) {
          _private_tls_write_packet(tls_build_verify_request(context));
          _private_dtls_reset(context);
        } else {
          DEBUG_PRINT("<= SENDING SERVER HELLO\n");
          if (context->connection_status == 3) {
            context->connection_status = 2;
            _private_tls_write_packet(tls_build_hello(context, 0));
            _private_tls_write_packet(tls_build_change_cipher_spec(context));
            _private_tls13_key(context, 1);
            context->cipher_spec_set = 1;
            DEBUG_PRINT("<= SENDING ENCRYPTED EXTENSIONS\n");
            _private_tls_write_packet(tls_build_encrypted_extensions(context));

            DEBUG_PRINT("<= SENDING CERTIFICATE\n");
            _private_tls_write_packet(tls_build_certificate(context));
            DEBUG_PRINT("<= SENDING CERTIFICATE VERIFY\n");
            _private_tls_write_packet(tls_build_certificate_verify(context));
            DEBUG_PRINT("<= SENDING FINISHED\n");
            _private_tls_write_packet(tls_build_finished(context));
            // new key
            TLS_FREE(context->server_finished_hash);
            context->server_finished_hash =
                (unsigned char *)TLS_MALLOC(_private_tls_mac_length(context));
            if (context->server_finished_hash)
              _private_tls_get_hash(context, context->server_finished_hash);
            break;
          }
          _private_tls_write_packet(tls_build_hello(context, 0));
          DEBUG_PRINT("<= SENDING CERTIFICATE\n");
          _private_tls_write_packet(tls_build_certificate(context));
          int ephemeral_cipher = tls_cipher_is_ephemeral(context);
          if (ephemeral_cipher) {
            DEBUG_PRINT("<= SENDING EPHEMERAL DH KEY\n");
            _private_tls_write_packet(tls_build_server_key_exchange(
                context,
                ephemeral_cipher == 1 ? KEA_dhe_rsa : KEA_ec_diffie_hellman));
          }

          DEBUG_PRINT("<= SENDING DONE\n");
          _private_tls_write_packet(tls_build_done(context));
        }
        break;
      case 3:
        // finished
        _private_tls_write_packet(tls_build_change_cipher_spec(context));
        _private_tls_write_packet(tls_build_finished(context));
        context->connection_status = 0xFF;
        DEBUG_PRINT("CONNECTION STATUS: FF (tls_parse_payload 2)\n");
        break;
      case 4:
        // dtls only
        context->dtls_seq = 1;
        _private_tls_write_packet(tls_build_hello(context, 0));
        break;
      case 5:
        // hello retry request
        DEBUG_PRINT("<= SENDING HELLO RETRY REQUEST\n");
        _private_tls_write_packet(tls_build_hello(context, 0));
        break;
    }
    payload_size++;
    buf += payload_size;
    buf_len -= payload_size;
  }
  return orig_len;
}

unsigned int _private_tls_hmac_message(unsigned char local,
                                       struct TLSContext *context,
                                       const unsigned char *buf, int buf_len,
                                       const unsigned char *buf2, int buf_len2,
                                       unsigned char *out, unsigned int outlen,
                                       uint64_t remote_sequence_number) {
  hmac_state hash;
  int mac_size = outlen;
  int hash_idx;
  if (mac_size == TLS_SHA1_MAC_SIZE)
    hash_idx = find_hash("sha1");
  else if (mac_size == TLS_SHA384_MAC_SIZE)
    hash_idx = find_hash("sha384");
  else
    hash_idx = find_hash("sha256");

  if (hmac_init(&hash, hash_idx,
                local ? context->crypto.ctx_local_mac.local_mac
                      : context->crypto.ctx_remote_mac.remote_mac,
                mac_size))
    return 0;

  uint64_t squence_number;
  if (context->dtls)
    squence_number = htonll(remote_sequence_number);
  else if (local)
    squence_number = htonll(context->local_sequence_number);
  else
    squence_number = htonll(context->remote_sequence_number);

  if (hmac_process(&hash, (unsigned char *)&squence_number, sizeof(uint64_t)))
    return 0;

  if (hmac_process(&hash, buf, buf_len)) return 0;
  if ((buf2) && (buf_len2)) {
    if (hmac_process(&hash, buf2, buf_len2)) return 0;
  }
  unsigned long ref_outlen = outlen;
  if (hmac_done(&hash, out, &ref_outlen)) return 0;

  return (unsigned int)ref_outlen;
}

int tls_parse_message(struct TLSContext *context, unsigned char *buf,
                      int buf_len, tls_validation_function certificate_verify) {
  int res = 5;
  if (context->dtls) res = 13;
  int header_size = res;
  int payload_res = 0;

  CHECK_SIZE(res, buf_len, TLS_NEED_MORE_DATA)

  unsigned char type = *buf;

  int buf_pos = 1;

  unsigned short version = ntohs(*(unsigned short *)&buf[buf_pos]);
  buf_pos += 2;

  uint64_t dtls_sequence_number = 0;
  if (context->dtls) {
    CHECK_SIZE(buf_pos + 8, buf_len, TLS_NEED_MORE_DATA)
    dtls_sequence_number = ntohll(*(uint64_t *)&buf[buf_pos]);
    buf_pos += 8;
  }

  VERSION_SUPPORTED(version, TLS_NOT_SAFE)
  unsigned short length;
  length = ntohs(*(unsigned short *)&buf[buf_pos]);
  buf_pos += 2;

  unsigned char *pt = NULL;
  const unsigned char *ptr = buf + buf_pos;

  CHECK_SIZE(buf_pos + length, buf_len, TLS_NEED_MORE_DATA)
  DEBUG_PRINT("Message type: %0x, length: %i\n", (int)type, (int)length);
  if ((context->cipher_spec_set) && (type != TLS_CHANGE_CIPHER)) {
    DEBUG_DUMP_HEX_LABEL("encrypted", &buf[header_size], length);
    if (!context->crypto.created) {
      DEBUG_PRINT("Encryption context not created\n");
      _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
      return TLS_BROKEN_PACKET;
    }
    pt = (unsigned char *)TLS_MALLOC(length);
    if (!pt) {
      DEBUG_PRINT("Error in TLS_MALLOC (%i bytes)\n", (int)length);
      _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
      return TLS_NO_MEMORY;
    }

    unsigned char aad[16];
    int aad_size = sizeof(aad);
    unsigned char *sequence = aad;

    if (context->crypto.created == 2) {
      int delta = 8;
      int pt_length;
      unsigned char iv[TLS_13_AES_GCM_IV_LENGTH];
      gcm_reset(&context->crypto.ctx_remote.aes_gcm_remote);

      if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
        aad[0] = TLS_APPLICATION_DATA;
        aad[1] = 0x03;
        aad[2] = 0x03;
        *((unsigned short *)(aad + 3)) = htons(buf_len - header_size);
        aad_size = 5;
        sequence = aad + 5;
        if (context->dtls)
          *((uint64_t *)sequence) = *(uint64_t *)(buf + 3);
        else
          *((uint64_t *)sequence) = htonll(context->remote_sequence_number);
        memcpy(iv, context->crypto.ctx_remote_mac.remote_iv,
               TLS_13_AES_GCM_IV_LENGTH);
        int i;
        int offset = TLS_13_AES_GCM_IV_LENGTH - 8;
        for (i = 0; i < 8; i++)
          iv[offset + i] =
              context->crypto.ctx_remote_mac.remote_iv[offset + i] ^
              sequence[i];
        pt_length = buf_len - header_size - TLS_GCM_TAG_LEN;
        delta = 0;
      } else {
        aad_size = 13;
        pt_length = length - 8 - TLS_GCM_TAG_LEN;
        // build aad and iv
        if (context->dtls)
          *((uint64_t *)aad) = htonll(dtls_sequence_number);
        else
          *((uint64_t *)aad) = htonll(context->remote_sequence_number);
        aad[8] = buf[0];
        aad[9] = buf[1];
        aad[10] = buf[2];

        memcpy(iv, context->crypto.ctx_remote_mac.remote_aead_iv, 4);
        memcpy(iv + 4, buf + header_size, 8);
        *((unsigned short *)(aad + 11)) = htons((unsigned short)pt_length);
      }
      if (pt_length < 0) {
        DEBUG_PRINT("Invalid packet length");
        TLS_FREE(pt);
        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        return TLS_BROKEN_PACKET;
      }
      DEBUG_DUMP_HEX_LABEL("aad", aad, aad_size);
      DEBUG_DUMP_HEX_LABEL("aad iv", iv, 12);

      int res0 = gcm_add_iv(&context->crypto.ctx_remote.aes_gcm_remote, iv, 12);
      int res1 = gcm_add_aad(&context->crypto.ctx_remote.aes_gcm_remote, aad,
                             aad_size);
      memset(pt, 0, length);
      DEBUG_PRINT("PT SIZE: %i\n", pt_length);
      int res2 = gcm_process(&context->crypto.ctx_remote.aes_gcm_remote, pt,
                             pt_length, buf + header_size + delta, GCM_DECRYPT);
      unsigned char tag[32];
      unsigned long taglen = 32;
      int res3 =
          gcm_done(&context->crypto.ctx_remote.aes_gcm_remote, tag, &taglen);
      if ((res0) || (res1) || (res2) || (res3) || (taglen != TLS_GCM_TAG_LEN)) {
        DEBUG_PRINT(
            "ERROR: gcm_add_iv: %i, gcm_add_aad: %i, gcm_process: %i, "
            "gcm_done: %i\n",
            res0, res1, res2, res3);
        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        return TLS_BROKEN_PACKET;
      }
      DEBUG_DUMP_HEX_LABEL("decrypted", pt, pt_length);
      DEBUG_DUMP_HEX_LABEL("tag", tag, taglen);
      // check tag
      if (memcmp(buf + header_size + delta + pt_length, tag, taglen)) {
        DEBUG_PRINT("INTEGRITY CHECK FAILED (msg length %i)\n", pt_length);
        DEBUG_DUMP_HEX_LABEL("TAG RECEIVED",
                             buf + header_size + delta + pt_length, taglen);
        DEBUG_DUMP_HEX_LABEL("TAG COMPUTED", tag, taglen);
        TLS_FREE(pt);
        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        _private_tls_write_packet(tls_build_alert(context, 1, bad_record_mac));
        return TLS_INTEGRITY_FAILED;
      }
      ptr = pt;
      length = (unsigned short)pt_length;
    } else if (context->crypto.created == 3) {
      int pt_length = length - POLY1305_TAGLEN;
      unsigned int counter = 1;
      unsigned char poly1305_key[POLY1305_KEYLEN];
      unsigned char trail[16];
      unsigned char mac_tag[POLY1305_TAGLEN];
      aad_size = 16;
      if (pt_length < 0) {
        DEBUG_PRINT("Invalid packet length");
        TLS_FREE(pt);
        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        return TLS_BROKEN_PACKET;
      }
      if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
        aad[0] = TLS_APPLICATION_DATA;
        aad[1] = 0x03;
        aad[2] = 0x03;
        *((unsigned short *)(aad + 3)) = htons(buf_len - header_size);
        aad_size = 5;
        sequence = aad + 5;
        if (context->dtls)
          *((uint64_t *)sequence) = *(uint64_t *)(buf + 3);
        else
          *((uint64_t *)sequence) = htonll(context->remote_sequence_number);
      } else {
        if (context->dtls)
          *((uint64_t *)aad) = htonll(dtls_sequence_number);
        else
          *((uint64_t *)aad) = htonll(context->remote_sequence_number);
        aad[8] = buf[0];
        aad[9] = buf[1];
        aad[10] = buf[2];
        *((unsigned short *)(aad + 11)) = htons((unsigned short)pt_length);
        aad[13] = 0;
        aad[14] = 0;
        aad[15] = 0;
      }

      chacha_ivupdate(&context->crypto.ctx_remote.chacha_remote,
                      context->crypto.ctx_remote_mac.remote_aead_iv, sequence,
                      (unsigned char *)&counter);

      chacha_encrypt_bytes(&context->crypto.ctx_remote.chacha_remote,
                           buf + header_size, pt, pt_length);
      DEBUG_DUMP_HEX_LABEL("decrypted", pt, pt_length);
      ptr = pt;
      length = (unsigned short)pt_length;

      chacha20_poly1305_key(&context->crypto.ctx_remote.chacha_remote,
                            poly1305_key);
      poly1305_context ctx;
      private_tls_poly1305_init(&ctx, poly1305_key);
      private_tls_poly1305_update(&ctx, aad, aad_size);
      static unsigned char zeropad[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                        0, 0, 0, 0, 0, 0, 0};
      int rem = aad_size % 16;
      if (rem) private_tls_poly1305_update(&ctx, zeropad, 16 - rem);
      private_tls_poly1305_update(&ctx, buf + header_size, pt_length);
      rem = pt_length % 16;
      if (rem) private_tls_poly1305_update(&ctx, zeropad, 16 - rem);

      private_tls_U32TO8(&trail[0], aad_size == 5 ? 5 : 13);
      *(int *)&trail[4] = 0;
      private_tls_U32TO8(&trail[8], pt_length);
      *(int *)&trail[12] = 0;

      private_tls_poly1305_update(&ctx, trail, 16);
      private_tls_poly1305_finish(&ctx, mac_tag);
      if (memcmp(mac_tag, buf + header_size + pt_length, POLY1305_TAGLEN)) {
        DEBUG_PRINT("INTEGRITY CHECK FAILED (msg length %i)\n", length);
        DEBUG_DUMP_HEX_LABEL("POLY1305 TAG RECEIVED",
                             buf + header_size + pt_length, POLY1305_TAGLEN);
        DEBUG_DUMP_HEX_LABEL("POLY1305 TAG COMPUTED", mac_tag, POLY1305_TAGLEN);
        TLS_FREE(pt);

        // silently ignore packet for DTLS
        if (context->dtls) return header_size + length;

        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        _private_tls_write_packet(tls_build_alert(context, 1, bad_record_mac));
        return TLS_INTEGRITY_FAILED;
      }
    } else {
      unsigned char padding_byte = pt[length - 1];
      unsigned char padding = padding_byte + 1;

      // poodle check
      int padding_index = length - padding;
      if (padding_index > 0) {
        int i;
        int limit = length - 1;
        for (i = length - padding; i < limit; i++) {
          if (pt[i] != padding_byte) {
            TLS_FREE(pt);
            DEBUG_PRINT("BROKEN PACKET (POODLE ?)\n");
            _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
            _private_tls_write_packet(
                tls_build_alert(context, 1, decrypt_error));
            return TLS_BROKEN_PACKET;
          }
        }
      }

      unsigned int decrypted_length = length;
      if (padding < decrypted_length) decrypted_length -= padding;

      DEBUG_DUMP_HEX_LABEL("decrypted", pt, decrypted_length);
      ptr = pt;
      if ((context->version != TLS_V10) &&
          (decrypted_length > TLS_AES_IV_LENGTH)) {
        decrypted_length -= TLS_AES_IV_LENGTH;
        ptr += TLS_AES_IV_LENGTH;
      }
      length = decrypted_length;

      unsigned int mac_size = _private_tls_mac_length(context);
      if ((length < mac_size) || (!mac_size)) {
        TLS_FREE(pt);
        DEBUG_PRINT("BROKEN PACKET\n");
        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        _private_tls_write_packet(tls_build_alert(context, 1, decrypt_error));
        return TLS_BROKEN_PACKET;
      }

      length -= mac_size;

      const unsigned char *message_hmac = &ptr[length];
      unsigned char hmac_out[TLS_MAX_MAC_SIZE];
      unsigned char temp_buf[5];
      memcpy(temp_buf, buf, 3);
      *(unsigned short *)(temp_buf + 3) = htons(length);
      unsigned int hmac_out_len =
          _private_tls_hmac_message(0, context, temp_buf, 5, ptr, length,
                                    hmac_out, mac_size, dtls_sequence_number);
      if ((hmac_out_len != mac_size) ||
          (memcmp(message_hmac, hmac_out, mac_size))) {
        DEBUG_PRINT("INTEGRITY CHECK FAILED (msg length %i)\n", length);
        DEBUG_DUMP_HEX_LABEL("HMAC RECEIVED", message_hmac, mac_size);
        DEBUG_DUMP_HEX_LABEL("HMAC COMPUTED", hmac_out, hmac_out_len);
        TLS_FREE(pt);

        // silently ignore packet for DTLS
        if (context->dtls) return header_size + length;

        _private_random_sleep(context, TLS_MAX_ERROR_SLEEP_uS);
        _private_tls_write_packet(tls_build_alert(context, 1, bad_record_mac));

        return TLS_INTEGRITY_FAILED;
      }
    }
  }
  context->remote_sequence_number++;
  if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
    if (/*(context->connection_status == 2) && */ (type ==
                                                   TLS_APPLICATION_DATA) &&
        (context->crypto.created)) {
      do {
        length--;
        type = ptr[length];
      } while (!type);
    }
  }
  switch (type) {
    // application data
    case TLS_APPLICATION_DATA:
      if (context->connection_status != 0xFF) {
        DEBUG_PRINT("UNEXPECTED APPLICATION DATA MESSAGE\n");
        payload_res = TLS_UNEXPECTED_MESSAGE;
        _private_tls_write_packet(
            tls_build_alert(context, 1, unexpected_message));
      } else {
        DEBUG_PRINT("APPLICATION DATA MESSAGE (TLS VERSION: %x):\n",
                    (int)context->version);
        DEBUG_DUMP(ptr, length);
        DEBUG_PRINT("\n");
        _private_tls_write_app_data(context, ptr, length);
      }
      break;
      // handshake
    case TLS_HANDSHAKE:
      DEBUG_PRINT("HANDSHAKE MESSAGE\n");
      payload_res = tls_parse_payload(context, ptr, length, certificate_verify);
      break;
      // change cipher spec
    case TLS_CHANGE_CIPHER:
      context->dtls_epoch_remote++;
      if (context->connection_status != 2) {
        if (context->connection_status == 4) {
          DEBUG_PRINT(
              "IGNORING CHANGE CIPHER SPEC MESSAGE (HELLO RETRY REQUEST)\n");
          break;
        }
        DEBUG_PRINT("UNEXPECTED CHANGE CIPHER SPEC MESSAGE (%i)\n",
                    context->connection_status);
        _private_tls_write_packet(
            tls_build_alert(context, 1, unexpected_message));
        payload_res = TLS_UNEXPECTED_MESSAGE;
      } else {
        DEBUG_PRINT("CHANGE CIPHER SPEC MESSAGE\n");
        context->cipher_spec_set = 1;
        // reset sequence numbers
        context->remote_sequence_number = 0;
      }
      if (!context->is_server) _private_tls13_key(context, 1);
      break;
      // alert
    case TLS_ALERT:
      DEBUG_PRINT("ALERT MESSAGE\n");
      if (length >= 2) {
        DEBUG_DUMP_HEX(ptr, length);
        int level = ptr[0];
        int code = ptr[1];
        if (level == TLS_ALERT_CRITICAL) {
          context->critical_error = 1;
          res = TLS_ERROR_ALERT;
        }
        context->error_code = code;
      }
      break;
    default:
      DEBUG_PRINT("NOT UNDERSTOOD MESSAGE TYPE: %x\n", (int)type);
      TLS_FREE(pt);
      return TLS_NOT_UNDERSTOOD;
  }
  TLS_FREE(pt);

  if (payload_res < 0) return payload_res;

  if (res > 0) return header_size + length;

  return res;
}

unsigned int asn1_get_len(const unsigned char *buffer, int buf_len,
                          unsigned int *octets) {
  *octets = 0;

  if (buf_len < 1) return 0;

  unsigned char size = buffer[0];
  int i;
  if (size & 0x80) {
    *octets = size & 0x7F;
    if ((int)*octets > buf_len - 1) return 0;
    // max 32 bits
    unsigned int ref_octets = *octets;
    if (*octets > 4) ref_octets = 4;
    if ((int)*octets > buf_len - 1) return 0;
    unsigned int long_size = 0;
    unsigned int coef = 1;

    for (i = ref_octets; i > 0; i--) {
      long_size += buffer[i] * coef;
      coef *= 0x100;
    }
    ++*octets;
    return long_size;
  }
  ++*octets;
  return size;
}

void print_index(const unsigned int *fields) {
  int i = 0;
  while (fields[i]) {
    if (i) DEBUG_PRINT(".");
    DEBUG_PRINT("%i", fields[i]);
    i++;
  }
  while (i < 6) {
    DEBUG_PRINT("  ");
    i++;
  }
}

int _is_field(const unsigned int *fields, const unsigned int *prefix) {
  int i = 0;
  while (prefix[i]) {
    if (fields[i] != prefix[i]) return 0;
    i++;
  }
  return 1;
}

int _private_tls_hash_len(int algorithm) {
  switch (algorithm) {
    case TLS_RSA_SIGN_MD5:
      return 16;
    case TLS_RSA_SIGN_SHA1:
      return 20;
    case TLS_RSA_SIGN_SHA256:
      return 32;
    case TLS_RSA_SIGN_SHA384:
      return 48;
    case TLS_RSA_SIGN_SHA512:
      return 64;
  }
  return 0;
}

unsigned char *_private_tls_compute_hash(int algorithm,
                                         const unsigned char *message,
                                         unsigned int message_len) {
  unsigned char *hash = NULL;
  if ((!message) || (!message_len)) return hash;
  int err;
  hash_state state;
  switch (algorithm) {
    case TLS_RSA_SIGN_MD5:
      DEBUG_PRINT("SIGN MD5\n");
      hash = (unsigned char *)TLS_MALLOC(16);
      if (!hash) return NULL;

      err = md5_init(&state);
      if (!err) {
        err = md5_process(&state, message, message_len);
        if (!err) err = md5_done(&state, hash);
      }
      break;
    case TLS_RSA_SIGN_SHA1:
      DEBUG_PRINT("SIGN SHA1\n");
      hash = (unsigned char *)TLS_MALLOC(20);
      if (!hash) return NULL;

      err = sha1_init(&state);
      if (!err) {
        err = sha1_process(&state, message, message_len);
        if (!err) err = sha1_done(&state, hash);
      }
      break;
    case TLS_RSA_SIGN_SHA256:
      DEBUG_PRINT("SIGN SHA256\n");
      hash = (unsigned char *)TLS_MALLOC(32);
      if (!hash) return NULL;

      err = sha256_init(&state);
      if (!err) {
        err = sha256_process(&state, message, message_len);
        if (!err) err = sha256_done(&state, hash);
      }
      break;
    case TLS_RSA_SIGN_SHA384:
      DEBUG_PRINT("SIGN SHA384\n");
      hash = (unsigned char *)TLS_MALLOC(48);
      if (!hash) return NULL;

      err = sha384_init(&state);
      if (!err) {
        err = sha384_process(&state, message, message_len);
        if (!err) err = sha384_done(&state, hash);
      }
      break;
    case TLS_RSA_SIGN_SHA512:
      DEBUG_PRINT("SIGN SHA512\n");
      hash = (unsigned char *)TLS_MALLOC(64);
      if (!hash) return NULL;

      err = sha512_init(&state);
      if (!err) {
        err = sha512_process(&state, message, message_len);
        if (!err) err = sha512_done(&state, hash);
      }
      break;
    default:
      DEBUG_PRINT("UNKNOWN SIGNATURE ALGORITHM\n");
  }
  return hash;
}

int tls_certificate_verify_signature(struct TLSCertificate *cert,
                                     struct TLSCertificate *parent) {
#ifdef DEBUG
  char out_buf[0xFFFF];

  fprintf(stderr, "--- CERT DUMP BEGIN ---\n%s\n--- CERT DUMP END ---\n\n",
          tls_certificate_to_string(cert, out_buf, 0xFFFF));
#endif

  if (!cert) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: cert\n");
    return 0;
  }
  if (!parent) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: parent\n");
    return 0;
  }
  if (!cert->sign_key) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: cert->sign_key\n");
    return 0;
  }
  if (!cert->fingerprint) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: cert->fingerprint\n");
    return 0;
  }
  if (!cert->sign_len) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: cert->sign_len\n");
    return 0;
  }
  if (!parent->der_bytes) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: parent->der_bytes\n");
    return 0;
  }
  if (!parent->der_len) {
    DEBUG_PRINT("CANNOT VERIFY SIGNATURE: parent->der_len\n");
    return 0;
  }

  tls_init();
  int hash_len = _private_tls_hash_len(cert->algorithm);
  if (hash_len <= 0) return 0;

  int hash_index = -1;
  switch (cert->algorithm) {
    case TLS_RSA_SIGN_MD5:
      hash_index = find_hash("md5");
      break;
    case TLS_RSA_SIGN_SHA1:
      hash_index = find_hash("sha1");
      break;
    case TLS_RSA_SIGN_SHA256:
      hash_index = find_hash("sha256");
      break;
    case TLS_RSA_SIGN_SHA384:
      hash_index = find_hash("sha384");
      break;
    case TLS_RSA_SIGN_SHA512:
      hash_index = find_hash("sha512");
      break;
    default:
      DEBUG_PRINT("UNKNOWN SIGNATURE ALGORITHM\n");
      return 0;
  }

  rsa_key key;
  int err = rsa_import(parent->der_bytes, parent->der_len, &key);
  if (err) {
    DEBUG_PRINT("Error importing RSA certificate (code: %i)\n", err);
    DEBUG_DUMP_HEX_LABEL("CERTIFICATE", parent->der_bytes, parent->der_len);
    return 0;
  }
  int rsa_stat = 0;
  unsigned char *signature = cert->sign_key;
  int signature_len = cert->sign_len;
  if (!signature[0]) {
    signature++;
    signature_len--;
  }
  err =
      rsa_verify_hash_ex(signature, signature_len, cert->fingerprint, hash_len,
                         LTC_PKCS_1_V1_5, hash_index, 0, &rsa_stat, &key);
  rsa_free(&key);
  if (err) {
    DEBUG_PRINT("HASH VERIFY ERROR %i\n", err);
    return 0;
  }
  DEBUG_PRINT("CERTIFICATE VALIDATION: %i\n", rsa_stat);
  return rsa_stat;
}

int tls_certificate_chain_is_valid(struct TLSCertificate **certificates,
                                   int len) {
  if ((!certificates) || (!len)) return bad_certificate;

  int i;
  len--;

  // expired certificate or not yet valid ?
  if (tls_certificate_is_valid(certificates[0])) return bad_certificate;

  // check
  for (i = 0; i < len; i++) {
    // certificate in chain is expired ?
    if (tls_certificate_is_valid(certificates[i + 1])) return bad_certificate;
    if (!tls_certificate_verify_signature(certificates[i], certificates[i + 1]))
      return bad_certificate;
  }
  return 0;
}

int tls_certificate_chain_is_valid_root(struct TLSContext *context,
                                        struct TLSCertificate **certificates,
                                        int len) {
  if ((!certificates) || (!len) || (!context->root_certificates) ||
      (!context->root_count))
    return bad_certificate;
  int i;
  unsigned int j;
  for (i = 0; i < len; i++) {
    for (j = 0; j < context->root_count; j++) {
      // check if root certificate expired
      if (tls_certificate_is_valid(context->root_certificates[j])) continue;
      // if any root validates any certificate in the chain, then is root
      // validated
      if (tls_certificate_verify_signature(certificates[i],
                                           context->root_certificates[j]))
        return 0;
    }
  }
  return bad_certificate;
}

int _private_is_oid(struct private_OID_chain *ref_chain,
                    const unsigned char *looked_oid, int looked_oid_len) {
  while (ref_chain) {
    if (ref_chain->oid) {
      if (_is_oid2(ref_chain->oid, looked_oid, 16, looked_oid_len)) return 1;
    }
    ref_chain = (struct private_OID_chain *)ref_chain->top;
  }
  return 0;
}

int _private_asn1_parse(struct TLSContext *context, struct TLSCertificate *cert,
                        const unsigned char *buffer, unsigned int size,
                        int level, unsigned int *fields, unsigned char *has_key,
                        int client_cert, unsigned char *top_oid,
                        struct private_OID_chain *chain) {
  struct private_OID_chain local_chain;
  local_chain.top = chain;
  unsigned int pos = 0;
  // X.690
  int idx = 0;
  unsigned char oid[16];
  memset(oid, 0, 16);
  local_chain.oid = oid;
  if (has_key) *has_key = 0;
  unsigned char local_has_key = 0;
  const unsigned char *cert_data = NULL;
  unsigned int cert_len = 0;
  while (pos < size) {
    unsigned int start_pos = pos;
    CHECK_SIZE(2, size - pos, TLS_NEED_MORE_DATA)
    unsigned char first = buffer[pos++];
    unsigned char type = first & 0x1F;
    unsigned char constructed = first & 0x20;
    unsigned char element_class = first >> 6;
    unsigned int octets = 0;
    unsigned int temp;
    idx++;
    if (level <= TLS_ASN1_MAXLEVEL) fields[level - 1] = idx;
    unsigned int length =
        asn1_get_len((unsigned char *)&buffer[pos], size - pos, &octets);
    if ((octets > 4) || (octets > size - pos)) {
      DEBUG_PRINT("CANNOT READ CERTIFICATE\n");
      return pos;
    }
    pos += octets;
    CHECK_SIZE(length, size - pos, TLS_NEED_MORE_DATA)
    // DEBUG_PRINT("FIRST: %x => %x (%i)\n", (int)first, (int)type, length);
    // sequence
    // DEBUG_PRINT("%2i: ", level);
#ifdef DEBUG
    DEBUG_INDEX(fields);
    int i1;
    for (i1 = 1; i1 < level; i1++) DEBUG_PRINT("  ");
#endif

    if ((length) && (constructed)) {
      switch (type) {
        case 0x03:
          DEBUG_PRINT("CONSTRUCTED BITSTREAM\n");
          break;
        case 0x10:
          DEBUG_PRINT("SEQUENCE\n");
          if ((level == 2) && (idx == 1)) {
            cert_len = length + (pos - start_pos);
            cert_data = &buffer[start_pos];
          }
          // private key on server or public key on client
          if ((!cert->version) && (_is_field(fields, priv_der_id))) {
            TLS_FREE(cert->der_bytes);
            temp = length + (pos - start_pos);
            cert->der_bytes = (unsigned char *)TLS_MALLOC(temp);
            if (cert->der_bytes) {
              memcpy(cert->der_bytes, &buffer[start_pos], temp);
              cert->der_len = temp;
            } else
              cert->der_len = 0;
          }
          break;
        case 0x11:
          DEBUG_PRINT("EMBEDDED PDV\n");
          break;
        case 0x00:
          if (element_class == 0x02) {
            DEBUG_PRINT("CONTEXT-SPECIFIC\n");
            break;
          }
        default:
          DEBUG_PRINT("CONSTRUCT TYPE %02X\n", (int)type);
      }
      local_has_key = 0;
      _private_asn1_parse(context, cert, &buffer[pos], length, level + 1,
                          fields, &local_has_key, client_cert, top_oid,
                          &local_chain);
      if ((((local_has_key) && (context) &&
            ((!context->is_server) || (client_cert))) ||
           (!context)) &&
          (_is_field(fields, pk_id))) {
        TLS_FREE(cert->der_bytes);
        temp = length + (pos - start_pos);
        cert->der_bytes = (unsigned char *)TLS_MALLOC(temp);
        if (cert->der_bytes) {
          memcpy(cert->der_bytes, &buffer[start_pos], temp);
          cert->der_len = temp;
        } else
          cert->der_len = 0;
      }
    } else {
      switch (type) {
        case 0x00:
          // end of content
          DEBUG_PRINT("END OF CONTENT\n");
          return pos;
          break;
        case 0x01:
          // boolean
          temp = buffer[pos];
          DEBUG_PRINT("BOOLEAN: %i\n", temp);
          break;
        case 0x02:
          // integer
          if (_is_field(fields, pk_id)) {
            if (has_key) *has_key = 1;

            if (idx == 1)
              tls_certificate_set_key(cert, &buffer[pos], length);
            else if (idx == 2)
              tls_certificate_set_exponent(cert, &buffer[pos], length);
          } else if (_is_field(fields, serial_id))
            tls_certificate_set_serial(cert, &buffer[pos], length);
          if (_is_field(fields, version_id)) {
            if (length == 1)
              cert->version = buffer[pos];
            else
              cert->version = 0;
            idx++;
          }
          if (level >= 2) {
            unsigned int fields_temp[3];
            fields_temp[0] = fields[level - 2];
            fields_temp[1] = fields[level - 1];
            fields_temp[2] = 0;
            if (_is_field(fields_temp, priv_id))
              tls_certificate_set_priv(cert, &buffer[pos], length);
          }
          DEBUG_PRINT("INTEGER(%i): ", length);
          DEBUG_DUMP_HEX(&buffer[pos], length);
          if ((chain) && (length > 2)) {
            if (_private_is_oid(chain, san_oid, sizeof(san_oid) - 1)) {
              cert->san = (unsigned char **)TLS_REALLOC(
                  cert->san, sizeof(unsigned char *) * (cert->san_length + 1));
              if (cert->san) {
                cert->san[cert->san_length] = NULL;
                tls_certificate_set_copy(&cert->san[cert->san_length],
                                         &buffer[pos], length);
                DEBUG_PRINT(" => SUBJECT ALTERNATIVE NAME: %s",
                            cert->san[cert->san_length]);
                cert->san_length++;
              } else
                cert->san_length = 0;
            }
          }
          DEBUG_PRINT("\n");
          break;
        case 0x03:
          if (_is_field(fields, pk_id)) {
            if (has_key) *has_key = 1;
          }
          // bitstream
          DEBUG_PRINT("BITSTREAM(%i): ", length);
          DEBUG_DUMP_HEX(&buffer[pos], length);
          DEBUG_PRINT("\n");
          if (_is_field(fields, sign_id)) {
            tls_certificate_set_sign_key(cert, &buffer[pos], length);
          } else if ((cert->ec_algorithm) && (_is_field(fields, pk_id))) {
            tls_certificate_set_key(cert, &buffer[pos], length);
          } else {
            if ((buffer[pos] == 0x00) && (length > 256))
              _private_asn1_parse(context, cert, &buffer[pos] + 1, length - 1,
                                  level + 1, fields, &local_has_key,
                                  client_cert, top_oid, &local_chain);
            else
              _private_asn1_parse(context, cert, &buffer[pos], length,
                                  level + 1, fields, &local_has_key,
                                  client_cert, top_oid, &local_chain);
            if (top_oid) {
              if (_is_oid2(top_oid, TLS_EC_prime256v1_OID, sizeof(oid),
                           sizeof(TLS_EC_prime256v1) - 1)) {
                cert->ec_algorithm = secp256r1.iana;
              } else if (_is_oid2(top_oid, TLS_EC_secp224r1_OID, sizeof(oid),
                                  sizeof(TLS_EC_secp224r1_OID) - 1)) {
                cert->ec_algorithm = secp224r1.iana;
              } else if (_is_oid2(top_oid, TLS_EC_secp384r1_OID, sizeof(oid),
                                  sizeof(TLS_EC_secp384r1_OID) - 1)) {
                cert->ec_algorithm = secp384r1.iana;
              } else if (_is_oid2(top_oid, TLS_EC_secp521r1_OID, sizeof(oid),
                                  sizeof(TLS_EC_secp521r1_OID) - 1)) {
                cert->ec_algorithm = secp521r1.iana;
              }
              if ((cert->ec_algorithm) && (!cert->pk))
                tls_certificate_set_key(cert, &buffer[pos], length);
            }
          }
          break;
        case 0x04:
          if ((top_oid) && (_is_field(fields, ecc_priv_id)) && (!cert->priv)) {
            DEBUG_PRINT("BINARY STRING(%i): ", length);
            DEBUG_DUMP_HEX(&buffer[pos], length);
            DEBUG_PRINT("\n");
            tls_certificate_set_priv(cert, &buffer[pos], length);
          } else
            _private_asn1_parse(context, cert, &buffer[pos], length, level + 1,
                                fields, &local_has_key, client_cert, top_oid,
                                &local_chain);
          break;
        case 0x05:
          DEBUG_PRINT("NULL\n");
          break;
        case 0x06:
          // object identifier
          if (_is_field(fields, pk_id)) {
            if ((length == 8) || (length == 5))
              tls_certificate_set_algorithm(&cert->ec_algorithm, &buffer[pos],
                                            length);
            else
              tls_certificate_set_algorithm(&cert->key_algorithm, &buffer[pos],
                                            length);
          }
          if (_is_field(fields, algorithm_id)) {
            DEBUG_PRINT(" !!!SIGN_ALGO!!! ");
            tls_certificate_set_algorithm(&cert->algorithm, &buffer[pos],
                                          length);
          }

          DEBUG_PRINT("OBJECT IDENTIFIER(%i): ", length);
          DEBUG_DUMP_HEX(&buffer[pos], length);
          DEBUG_PRINT("\n");
          // check previous oid
          if (_is_oid2(oid, ocsp_oid, 16, sizeof(ocsp_oid) - 1))
            tls_certificate_set_copy(&cert->ocsp, &buffer[pos], length);

          if (length < 16)
            memcpy(oid, &buffer[pos], length);
          else
            memcpy(oid, &buffer[pos], 16);
          if (top_oid) memcpy(top_oid, oid, 16);
          break;
        case 0x09:
          DEBUG_PRINT("REAL NUMBER(%i): ", length);
          DEBUG_DUMP_HEX(&buffer[pos], length);
          DEBUG_PRINT("\n");
          break;
        case 0x17:
          // utc time
          DEBUG_PRINT("UTC TIME: [");
          DEBUG_DUMP(&buffer[pos], length);
          DEBUG_PRINT("]\n");

          if (_is_field(fields, validity_id)) {
            if (idx == 1)
              tls_certificate_set_copy_date(&cert->not_before, &buffer[pos],
                                            length);
            else
              tls_certificate_set_copy_date(&cert->not_after, &buffer[pos],
                                            length);
          }
          break;
        case 0x18:
          // generalized time
          DEBUG_PRINT("GENERALIZED TIME: [");
          DEBUG_DUMP(&buffer[pos], length);
          DEBUG_PRINT("]\n");
          break;
        case 0x13:
          // printable string
        case 0x0C:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
          if (_is_field(fields, issurer_id)) {
            if (_is_oid(oid, country_oid, 3))
              tls_certificate_set_copy(&cert->issuer_country, &buffer[pos],
                                       length);
            else if (_is_oid(oid, state_oid, 3))
              tls_certificate_set_copy(&cert->issuer_state, &buffer[pos],
                                       length);
            else if (_is_oid(oid, location_oid, 3))
              tls_certificate_set_copy(&cert->issuer_location, &buffer[pos],
                                       length);
            else if (_is_oid(oid, entity_oid, 3))
              tls_certificate_set_copy(&cert->issuer_entity, &buffer[pos],
                                       length);
            else if (_is_oid(oid, subject_oid, 3))
              tls_certificate_set_copy(&cert->issuer_subject, &buffer[pos],
                                       length);
          } else if (_is_field(fields, owner_id)) {
            if (_is_oid(oid, country_oid, 3))
              tls_certificate_set_copy(&cert->country, &buffer[pos], length);
            else if (_is_oid(oid, state_oid, 3))
              tls_certificate_set_copy(&cert->state, &buffer[pos], length);
            else if (_is_oid(oid, location_oid, 3))
              tls_certificate_set_copy(&cert->location, &buffer[pos], length);
            else if (_is_oid(oid, entity_oid, 3))
              tls_certificate_set_copy(&cert->entity, &buffer[pos], length);
            else if (_is_oid(oid, subject_oid, 3))
              tls_certificate_set_copy(&cert->subject, &buffer[pos], length);
          }
          DEBUG_PRINT("STR: [");
          DEBUG_DUMP(&buffer[pos], length);
          DEBUG_PRINT("]\n");
          break;
        case 0x10:
          DEBUG_PRINT("EMPTY SEQUENCE\n");
          break;
        case 0xA:
          DEBUG_PRINT("ENUMERATED(%i): ", length);
          DEBUG_DUMP_HEX(&buffer[pos], length);
          DEBUG_PRINT("\n");
          break;
        default:
          DEBUG_PRINT("========> NOT SUPPORTED %x\n", (int)type);
          // not supported / needed
          break;
      }
    }
    pos += length;
  }
  if ((level == 2) && (cert->sign_key) && (cert->sign_len) && (cert_len) &&
      (cert_data)) {
    TLS_FREE(cert->fingerprint);
    cert->fingerprint =
        _private_tls_compute_hash(cert->algorithm, cert_data, cert_len);
#ifdef DEBUG
    if (cert->fingerprint) {
      DEBUG_DUMP_HEX_LABEL("FINGERPRINT", cert->fingerprint,
                           _private_tls_hash_len(cert->algorithm));
    }
#endif
  }
  return pos;
}

struct TLSCertificate *asn1_parse(struct TLSContext *context,
                                  const unsigned char *buffer,
                                  unsigned int size, int client_cert) {
  unsigned int fields[TLS_ASN1_MAXLEVEL];
  memset(fields, 0, sizeof(int) * TLS_ASN1_MAXLEVEL);
  struct TLSCertificate *cert = tls_create_certificate();
  if (cert) {
    if (client_cert < 0) {
      client_cert = 0;
      // private key
      unsigned char top_oid[16];
      memset(top_oid, 0, sizeof(top_oid));
      _private_asn1_parse(context, cert, buffer, size, 1, fields, NULL,
                          client_cert, top_oid, NULL);
    } else
      _private_asn1_parse(context, cert, buffer, size, 1, fields, NULL,
                          client_cert, NULL, NULL);
  }
  return cert;
}

int tls_load_certificates(struct TLSContext *context,
                          const unsigned char *pem_buffer,
                          unsigned long pem_size) {
  if (!context) return TLS_GENERIC_ERROR;

  unsigned int len;
  int idx = 0;
  do {
    unsigned char *data = tls_pem_decode(pem_buffer, pem_size, idx++, &len);
    if ((!data) || (!len)) break;
    struct TLSCertificate *cert = asn1_parse(context, data, len, 0);
    if (cert) {
      if ((cert->version == 2) || (cert->version == 0)) {
        TLS_FREE(cert->der_bytes);
        cert->der_bytes = data;
        cert->der_len = len;
        data = NULL;
        if (cert->priv) {
          DEBUG_PRINT(
              "WARNING - parse error (private key encountered in "
              "certificate)\n");
          TLS_FREE(cert->priv);
          cert->priv = NULL;
          cert->priv_len = 0;
        }
        context->certificates = (struct TLSCertificate **)TLS_REALLOC(
            context->certificates, (context->certificates_count + 1) *
                                       sizeof(struct TLSCertificate *));
        context->certificates[context->certificates_count] = cert;
        context->certificates_count++;
        DEBUG_PRINT("Loaded certificate: %i\n",
                    (int)context->certificates_count);
      } else {
        DEBUG_PRINT("WARNING - certificate version error (v%i)\n",
                    (int)cert->version);
        tls_destroy_certificate(cert);
      }
    }
    TLS_FREE(data);
  } while (1);
  return context->certificates_count;
}

int tls_load_private_key(struct TLSContext *context,
                         const unsigned char *pem_buffer,
                         unsigned long pem_size) {
  if (!context) return TLS_GENERIC_ERROR;

  unsigned int len;
  int idx = 0;
  do {
    unsigned char *data = tls_pem_decode(pem_buffer, pem_size, idx++, &len);
    if ((!data) || (!len)) break;
    struct TLSCertificate *cert = asn1_parse(context, data, len, -1);
    if (cert) {
      if (!cert->der_len) {
        TLS_FREE(cert->der_bytes);
        cert->der_bytes = data;
        cert->der_len = len;
      } else
        TLS_FREE(data);
      if ((cert) && (cert->priv) && (cert->priv_len)) {
        if (cert->ec_algorithm) {
          DEBUG_PRINT("Loaded ECC private key\n");
          if (context->ec_private_key)
            tls_destroy_certificate(context->ec_private_key);
          context->ec_private_key = cert;
          return 1;
        } else {
          DEBUG_PRINT("Loaded private key\n");
          if (context->private_key)
            tls_destroy_certificate(context->private_key);
          context->private_key = cert;
          return 1;
        }
      }
      tls_destroy_certificate(cert);
    } else
      TLS_FREE(data);
  } while (1);
  return 0;
}

int tls_clear_certificates(struct TLSContext *context) {
  unsigned int i;
  if ((!context) || (!context->is_server) || (context->is_child))
    return TLS_GENERIC_ERROR;

  if (context->root_certificates) {
    for (i = 0; i < context->root_count; i++)
      tls_destroy_certificate(context->root_certificates[i]);
  }
  context->root_certificates = NULL;
  context->root_count = 0;
  if (context->private_key) tls_destroy_certificate(context->private_key);
  context->private_key = NULL;
  if (context->ec_private_key) tls_destroy_certificate(context->ec_private_key);
  context->ec_private_key = NULL;
  TLS_FREE(context->certificates);
  context->certificates = NULL;
  context->certificates_count = 0;
  return 0;
}

struct TLSPacket *tls_build_certificate_verify(struct TLSContext *context) {
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version, 0);
  // certificate verify
  tls_packet_uint8(packet, 0x0F);
  unsigned int size_offset = packet->len;
  tls_packet_uint24(packet, 0);

  unsigned char out[TLS_MAX_RSA_KEY];
  unsigned long out_len = TLS_MAX_RSA_KEY;

  unsigned char signing_data[TLS_MAX_HASH_SIZE + 98];
  int signing_data_len;

  // first 64 bytes to 0x20 (32)
  memset(signing_data, 0x20, 64);
  // context string 33 bytes
  if (context->is_server)
    memcpy(signing_data + 64, "TLS 1.3, server CertificateVerify", 33);
  else
    memcpy(signing_data + 64, "TLS 1.3, client CertificateVerify", 33);
  // a single 0 byte separator
  signing_data[97] = 0;
  signing_data_len = 98;

  signing_data_len += _private_tls_get_hash(context, signing_data + 98);
  DEBUG_DUMP_HEX_LABEL("verify data", signing_data, signing_data_len);
  int hash_algorithm = sha256;
  if (tls_is_ecdsa(context)) {
    switch (context->ec_private_key->ec_algorithm) {
      case 23:
        // secp256r1 + sha256
        tls_packet_uint16(packet, 0x0403);
        break;
      case 24:
        // secp384r1 + sha384
        tls_packet_uint16(packet, 0x0503);
        hash_algorithm = sha384;
        break;
      case 25:
        // secp521r1 + sha512
        tls_packet_uint16(packet, 0x0603);
        hash_algorithm = sha512;
        break;
      default:
        DEBUG_PRINT("UNSUPPORTED CURVE (SIGNING)\n");
        packet->broken = 1;
        return packet;
    }
  } else {
    tls_packet_uint16(packet, 0x0804);
  }

  int packet_size = 2;
  if (tls_is_ecdsa(context)) {
    if (_private_tls_sign_ecdsa(context, hash_algorithm, signing_data,
                                signing_data_len, out, &out_len) == 1) {
      DEBUG_PRINT("ECDSA signing OK! (ECDSA, length %lu)\n", out_len);
      tls_packet_uint16(packet, out_len);
      tls_packet_append(packet, out, out_len);
      packet_size += out_len + 2;
    }
  } else if (_private_tls_sign_rsa(context, hash_algorithm, signing_data,
                                   signing_data_len, out, &out_len) == 1) {
    DEBUG_PRINT("RSA signing OK! (length %lu)\n", out_len);
    tls_packet_uint16(packet, out_len);
    tls_packet_append(packet, out, out_len);
    packet_size += out_len + 2;
  }
  packet->buf[size_offset] = packet_size / 0x10000;
  packet_size %= 0x10000;
  packet->buf[size_offset + 1] = packet_size / 0x100;
  packet_size %= 0x100;
  packet->buf[size_offset + 2] = packet_size;

  tls_packet_update(packet);
  return packet;
}

struct TLSPacket *tls_build_certificate(struct TLSContext *context) {
  int i;
  unsigned int all_certificate_size = 0;
  int certificates_count;
  struct TLSCertificate **certificates;
  certificates_count = context->certificates_count;
  certificates = context->certificates;
  int delta = 3;
  if ((context->version == TLS_V13) || (context->version == DTLS_V13))
    delta = 5;
  int is_ecdsa = tls_is_ecdsa(context);
  if (is_ecdsa) {
    for (i = 0; i < certificates_count; i++) {
      struct TLSCertificate *cert = certificates[i];
      if ((cert) && (cert->der_len) && (cert->ec_algorithm))
        all_certificate_size += cert->der_len + delta;
    }
  } else {
    for (i = 0; i < certificates_count; i++) {
      struct TLSCertificate *cert = certificates[i];
      if ((cert) && (cert->der_len) && (!cert->ec_algorithm))
        all_certificate_size += cert->der_len + delta;
    }
  }
  if (!all_certificate_size) {
    DEBUG_PRINT("NO CERTIFICATE SET\n");
  }
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version, 0);
  tls_packet_uint8(packet, 0x0B);
  if (all_certificate_size) {
    // context
    if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
      tls_packet_uint24(packet, all_certificate_size + 4);
      tls_packet_uint8(packet, 0);
    } else
      tls_packet_uint24(packet, all_certificate_size + 3);

    if (context->dtls)
      _private_dtls_handshake_data(context, packet, all_certificate_size + 3);

    tls_packet_uint24(packet, all_certificate_size);
    for (i = 0; i < certificates_count; i++) {
      struct TLSCertificate *cert = certificates[i];
      if ((cert) && (cert->der_len)) {
        // is RSA certificate ?
        if ((is_ecdsa) && (!cert->ec_algorithm)) continue;
        // is ECC certificate ?
        if ((!is_ecdsa) && (cert->ec_algorithm)) continue;
        // 2 times -> one certificate
        tls_packet_uint24(packet, cert->der_len);
        tls_packet_append(packet, cert->der_bytes, cert->der_len);
        // extension
        if ((context->version == TLS_V13) || (context->version == DTLS_V13))
          tls_packet_uint16(packet, 0);
      }
    }
  } else {
    tls_packet_uint24(packet, all_certificate_size);
    if ((context->version == TLS_V13) || (context->version == DTLS_V13))
      tls_packet_uint8(packet, 0);

    if (context->dtls)
      _private_dtls_handshake_data(context, packet, all_certificate_size);
  }
  tls_packet_update(packet);
  if (context->dtls) context->dtls_seq++;
  return packet;
}

struct TLSPacket *tls_build_encrypted_extensions(struct TLSContext *context) {
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version, 3);
  tls_packet_uint8(packet, 0x08);
  if (context->negotiated_alpn) {
    int alpn_negotiated_len = strlen(context->negotiated_alpn);
    int alpn_len = alpn_negotiated_len + 1;

    tls_packet_uint24(packet, alpn_len + 8);
    tls_packet_uint16(packet, alpn_len + 6);
    tls_packet_uint16(packet, 0x10);
    tls_packet_uint16(packet, alpn_len + 2);
    tls_packet_uint16(packet, alpn_len);

    tls_packet_uint8(packet, alpn_negotiated_len);
    tls_packet_append(packet, (unsigned char *)context->negotiated_alpn,
                      alpn_negotiated_len);
  } else {
    tls_packet_uint24(packet, 2);
    tls_packet_uint16(packet, 0);
  }
  tls_packet_update(packet);
  return packet;
}

struct TLSPacket *tls_build_finished(struct TLSContext *context) {
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version,
                        TLS_MIN_FINISHED_OPAQUE_LEN + 64);
  tls_packet_uint8(packet, 0x14);
  if ((context->version == TLS_V13) || (context->version == DTLS_V13))
    tls_packet_uint24(packet, _private_tls_mac_length(context));
  else
    tls_packet_uint24(packet, TLS_MIN_FINISHED_OPAQUE_LEN);
  if (context->dtls)
    _private_dtls_handshake_data(context, packet, TLS_MIN_FINISHED_OPAQUE_LEN);
  // verify
  unsigned char hash[TLS_MAX_HASH_SIZE];
  unsigned long out_size = TLS_MIN_FINISHED_OPAQUE_LEN;
  unsigned char out[TLS_MAX_HASH_SIZE];
  unsigned int hash_len;

  // server verifies client's message
  if (context->is_server) {
    if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
      hash_len = _private_tls_get_hash(context, hash);
      if ((!context->finished_key) || (!hash_len)) {
        DEBUG_PRINT("NO FINISHED KEY COMPUTED OR NO HANDSHAKE HASH\n");
        packet->broken = 1;
        return packet;
      }

      DEBUG_DUMP_HEX_LABEL("HS HASH", hash, hash_len);
      DEBUG_DUMP_HEX_LABEL("HS FINISH", context->finished_key, hash_len);
      DEBUG_DUMP_HEX_LABEL("HS REMOTE FINISH", context->remote_finished_key,
                           hash_len);

      out_size = hash_len;
      hmac_state hmac;
      hmac_init(&hmac, _private_tls_get_hash_idx(context),
                context->finished_key, hash_len);
      hmac_process(&hmac, hash, hash_len);
      hmac_done(&hmac, out, &out_size);
    } else {
      hash_len = _private_tls_done_hash(context, hash);
      _private_tls_prf(context, out, TLS_MIN_FINISHED_OPAQUE_LEN,
                       context->master_key, context->master_key_len,
                       (unsigned char *)"server finished", 15, hash, hash_len,
                       NULL, 0);
      _private_tls_destroy_hash(context);
    }
  } else {
    if ((context->version == TLS_V13) || (context->version == DTLS_V13)) {
      hash_len = _private_tls_get_hash(context, hash);
      if ((!context->finished_key) || (!hash_len)) {
        DEBUG_PRINT("NO FINISHED KEY COMPUTED OR NO HANDSHAKE HASH\n");
        packet->broken = 1;
        return packet;
      }
      DEBUG_DUMP_HEX_LABEL("HS HASH", hash, hash_len);
      DEBUG_DUMP_HEX_LABEL("HS FINISH", context->finished_key, hash_len);
      DEBUG_DUMP_HEX_LABEL("HS REMOTE FINISH", context->remote_finished_key,
                           hash_len);

      TLS_FREE(context->server_finished_hash);
      context->server_finished_hash = (unsigned char *)TLS_MALLOC(hash_len);
      if (context->server_finished_hash)
        memcpy(context->server_finished_hash, hash, hash_len);

      out_size = hash_len;
      hmac_state hmac;
      hmac_init(&hmac, _private_tls_get_hash_idx(context),
                context->finished_key, hash_len);
      hmac_process(&hmac, hash, hash_len);
      hmac_done(&hmac, out, &out_size);
    } else {
      hash_len = _private_tls_get_hash(context, hash);
      _private_tls_prf(context, out, TLS_MIN_FINISHED_OPAQUE_LEN,
                       context->master_key, context->master_key_len,
                       (unsigned char *)"client finished", 15, hash, hash_len,
                       NULL, 0);
    }
  }
  tls_packet_append(packet, out, out_size);
  tls_packet_update(packet);
  DEBUG_DUMP_HEX_LABEL("VERIFY DATA", out, out_size);
  return packet;
}

struct TLSPacket *tls_build_change_cipher_spec(struct TLSContext *context) {
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_CHANGE_CIPHER, context->version, 64);
  tls_packet_uint8(packet, 1);
  tls_packet_update(packet);
  context->local_sequence_number = 0;
  return packet;
}

struct TLSPacket *tls_build_done(struct TLSContext *context) {
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_HANDSHAKE, context->version, 0);
  tls_packet_uint8(packet, 0x0E);
  tls_packet_uint24(packet, 0);
  if (context->dtls) {
    _private_dtls_handshake_data(context, packet, 0);
    context->dtls_seq++;
  }
  tls_packet_update(packet);
  return packet;
}

struct TLSPacket *tls_build_message(struct TLSContext *context,
                                    const unsigned char *data,
                                    unsigned int len) {
  if ((!data) || (!len)) return 0;
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_APPLICATION_DATA, context->version, len);
  tls_packet_append(packet, data, len);
  tls_packet_update(packet);
  return packet;
}

int tls_client_connect(struct TLSContext *context) {
  if ((context->is_server) || (context->critical_error))
    return TLS_UNEXPECTED_MESSAGE;

  return _private_tls_write_packet(tls_build_hello(context, 0));
}

int tls_write(struct TLSContext *context, const unsigned char *data,
              unsigned int len) {
  if (!context) return TLS_GENERIC_ERROR;
  if ((context->connection_status != 0xFF) &&
      ((context->is_server) || (context->version != TLS_V12) ||
       (context->critical_error) || (!context->false_start)))
    return TLS_UNEXPECTED_MESSAGE;
  if (len > TLS_MAXTLS_APP_SIZE) len = TLS_MAXTLS_APP_SIZE;
  int actually_written =
      _private_tls_write_packet(tls_build_message(context, data, len));
  if (actually_written <= 0) return actually_written;
  return len;
}

struct TLSPacket *tls_build_alert(struct TLSContext *context, char critical,
                                  unsigned char code) {
  struct TLSPacket *packet =
      tls_create_packet(context, TLS_ALERT, context->version, 0);
  tls_packet_uint8(packet, critical ? TLS_ALERT_CRITICAL : TLS_ALERT_WARNING);
  if (critical) context->critical_error = 1;
  tls_packet_uint8(packet, code);
  tls_packet_update(packet);
  return packet;
}

int _private_tls_read_from_file(const char *fname, void *buf, int max_len) {
  FILE *f = fopen(fname, "rb");
  if (f) {
    int size = (int)fread(buf, 1, max_len, f);
    fclose(f);
    return size;
  }
  return 0;
}

int tls_consume_stream(struct TLSContext *context, const unsigned char *buf,
                       ssize_t buf_len,
                       tls_validation_function certificate_verify) {
  if (!context) return TLS_GENERIC_ERROR;

  if (context->critical_error) return TLS_BROKEN_CONNECTION;

  if (buf_len <= 0) {
    DEBUG_PRINT("tls_consume_stream called with buf_len %l\n", buf_len);
    return 0;
  }

  if (!buf) {
    DEBUG_PRINT("tls_consume_stream called NULL buffer\n");
    context->critical_error = 1;
    return TLS_NO_MEMORY;
  }

  unsigned int orig_len = context->message_buffer_len;
  context->message_buffer_len += buf_len;
  context->message_buffer = (unsigned char *)TLS_REALLOC(
      context->message_buffer, context->message_buffer_len);
  if (!context->message_buffer) {
    context->message_buffer_len = 0;
    return TLS_NO_MEMORY;
  }
  memcpy(context->message_buffer + orig_len, buf, buf_len);
  unsigned int index = 0;
  unsigned int tls_buffer_len = context->message_buffer_len;
  int err_flag = 0;

  int tls_header_size;
  int tls_size_offset;

  if (context->dtls) {
    tls_size_offset = 11;
    tls_header_size = 13;
  } else {
    tls_size_offset = 3;
    tls_header_size = 5;
  }
  while (tls_buffer_len >= 5) {
    unsigned int length =
        ntohs(*(unsigned short *)&context
                   ->message_buffer[index + tls_size_offset]) +
        tls_header_size;
    if (length > tls_buffer_len) {
      DEBUG_PRINT("NEED DATA: %i/%i\n", length, tls_buffer_len);
      break;
    }
    int consumed = tls_parse_message(context, &context->message_buffer[index],
                                     length, certificate_verify);
    DEBUG_PRINT("Consumed %i bytes\n", consumed);
    if (consumed < 0) {
      if (!context->critical_error) context->critical_error = 1;
      err_flag = consumed;
      break;
    }
    index += length;
    tls_buffer_len -= length;
    if (context->critical_error) {
      err_flag = TLS_BROKEN_CONNECTION;
      break;
    }
  }
  if (err_flag) {
    DEBUG_PRINT("ERROR IN CONSUME: %i\n", err_flag);
    context->message_buffer_len = 0;
    TLS_FREE(context->message_buffer);
    context->message_buffer = NULL;
    return err_flag;
  }
  if (index) {
    context->message_buffer_len -= index;
    if (context->message_buffer_len) {
      // no realloc here
      memmove(context->message_buffer, context->message_buffer + index,
              context->message_buffer_len);
    } else {
      TLS_FREE(context->message_buffer);
      context->message_buffer = NULL;
    }
  }
  return index;
}

void tls_close_notify(struct TLSContext *context) {
  if ((!context) || (context->critical_error)) return;
  context->critical_error = 1;
  DEBUG_PRINT("CLOSE\n");
  _private_tls_write_packet(tls_build_alert(context, 0, close_notify));
}

const char *tls_sni(struct TLSContext *context) {
  if (!context) return NULL;
  return context->sni;
}

int tls_sni_set(struct TLSContext *context, const char *sni) {
  if ((!context) || (context->is_server) || (context->critical_error) ||
      (context->connection_status != 0))
    return 0;
  TLS_FREE(context->sni);
  context->sni = NULL;
  if (sni) {
    size_t len = strlen(sni);
    if (len > 0) {
      context->sni = (char *)TLS_MALLOC(len + 1);
      if (context->sni) {
        context->sni[len] = 0;
        memcpy(context->sni, sni, len);
        return 1;
      }
    }
  }
  return 0;
}

int tls_load_root_certificates(struct TLSContext *context,
                               const unsigned char *pem_buffer,
                               size_t pem_size) {
  if (!context) return TLS_GENERIC_ERROR;

  unsigned int len;
  int idx = 0;

  do {
    unsigned char *data = tls_pem_decode(pem_buffer, pem_size, idx++, &len);
    if ((!data) || (!len)) break;
    struct TLSCertificate *cert = asn1_parse(NULL, data, len, 0);
    if (cert) {
      if ((cert->version == 2) || (cert->version == 0)) {
        if (cert->priv) {
          DEBUG_PRINT(
              "WARNING - parse error (private key encountered in "
              "certificate)\n");
          TLS_FREE(cert->priv);
          cert->priv = NULL;
          cert->priv_len = 0;
        }
        context->root_certificates = (struct TLSCertificate **)TLS_REALLOC(
            context->root_certificates,
            (context->root_count + 1) * sizeof(struct TLSCertificate *));
        if (!context->root_certificates) {
          context->root_count = 0;
          return TLS_GENERIC_ERROR;
        }
        context->root_certificates[context->root_count] = cert;
        context->root_count++;
        DEBUG_PRINT("Loaded certificate: %i\n", (int)context->root_count);
      } else {
        DEBUG_PRINT("WARNING - certificate version error (v%i)\n",
                    (int)cert->version);
        tls_destroy_certificate(cert);
      }
    }
    TLS_FREE(data);
  } while (1);
  return context->root_count;
}

#ifdef DEBUG
void tls_print_certificate(const char *fname) {
  unsigned char buf[0xFFFF];
  char out_buf[0xFFFF];
  int size = _private_tls_read_from_file(fname, buf, 0xFFFF);
  if (size > 0) {
    int idx = 0;
    unsigned int len;
    do {
      unsigned char *data;
      if (buf[0] == '-') {
        data = tls_pem_decode(buf, size, idx++, &len);
      } else {
        data = buf;
        len = size;
      }
      if ((!data) || (!len)) return;
      struct TLSCertificate *cert = asn1_parse(NULL, data, len, -1);
      if (data != buf) TLS_FREE(data);
      if (cert) {
        fprintf(stderr, "%s", tls_certificate_to_string(cert, out_buf, 0xFFFF));
        tls_destroy_certificate(cert);
      }
      if (data == buf) break;
    } while (1);
  }
}
#endif