#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQLITE_PRIVATE
    #define SQLITE_PRIVATE static
#endif

#ifdef SQLITE_CORE
    #include "sqlite3.h"
#else
    #ifdef _HAVE_SQLITE_CONFIG_H
        #include "config.h"
    #endif
    #include "sqlite3ext.h"
    SQLITE_EXTENSION_INIT1
#endif

/* Make sure any Macro's used in this file defined only here are undefined */

#ifdef A00
#undef A00
#endif
#ifdef A01
#undef A01
#endif
#ifdef A02
#undef A02
#endif
#ifdef A03
#undef A03
#endif
#ifdef A04
#undef A04
#endif
#ifdef A10
#undef A10
#endif
#ifdef A11
#undef A11
#endif
#ifdef A12
#undef A12
#endif
#ifdef A13
#undef A13
#endif
#ifdef A14
#undef A14
#endif
#ifdef A20
#undef A20
#endif
#ifdef A21
#undef A21
#endif
#ifdef A22
#undef A22
#endif
#ifdef A23
#undef A23
#endif
#ifdef A24
#undef A24
#endif
#ifdef A30
#undef A30
#endif
#ifdef A31
#undef A31
#endif
#ifdef A32
#undef A32
#endif
#ifdef A33
#undef A33
#endif
#ifdef A34
#undef A34
#endif
#ifdef A40
#undef A40
#endif
#ifdef A41
#undef A41
#endif
#ifdef A42
#undef A42
#endif
#ifdef A43
#undef A43
#endif
#ifdef A44
#undef A44
#endif
#ifdef ADDINC128
#undef ADDINC128
#endif
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif
#ifdef BYTE_ORDER
#undef BYTE_ORDER
#endif
#ifdef Ch
#undef Ch
#endif
#ifdef DOSTORESTATE58
#undef DOSTORESTATE58
#endif
#ifdef DOSTORESTATE65
#undef DOSTORESTATE65
#endif
#ifdef DVMASKSIZE
#undef DVMASKSIZE
#endif
#ifdef F
#undef F
#endif
#ifdef F
#undef F
#endif
#ifdef FF
#undef FF
#endif
#ifdef G
#undef G
#endif
#ifdef G
#undef G
#endif
#ifdef GET
#undef GET
#endif
#ifdef GET
#undef GET
#endif
#ifdef GG
#undef GG
#endif
#ifdef H
#undef H
#endif
#ifdef H
#undef H
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND1_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND1_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND2_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND2_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND3_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND3_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND4_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND4_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW
#endif
#ifdef HH
#undef HH
#endif
#ifdef I
#undef I
#endif
#ifdef II
#undef II
#endif
#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif
#ifdef Maj
#undef Maj
#endif
#ifdef MD2_BLOCK_SIZE
#undef MD2_BLOCK_SIZE
#endif
#ifdef MD2_H
#undef MD2_H
#endif
#ifdef MD4OUT
#undef MD4OUT
#endif
#ifdef MD5_BLOCK_SIZE
#undef MD5_BLOCK_SIZE
#endif
#ifdef MD5_H
#undef MD5_H
#endif
#ifdef MEMCPY_BCOPY
#undef MEMCPY_BCOPY
#endif
#ifdef MEMCPY_BCOPY
#undef MEMCPY_BCOPY
#endif
#ifdef MEMSET_BZERO
#undef MEMSET_BZERO
#endif
#ifdef MEMSET_BZERO
#undef MEMSET_BZERO
#endif
#ifdef NOPROTO
#undef NOPROTO
#endif
#ifdef R
#undef R
#endif
#ifdef REVERSE32
#undef REVERSE32
#endif
#ifdef REVERSE64
#undef REVERSE64
#endif
#ifdef ROL64
#undef ROL64
#endif
#ifdef rotate_left
#undef rotate_left
#endif
#ifdef rotate_right
#undef rotate_right
#endif
#ifdef ROTLEFT
#undef ROTLEFT
#endif
#ifdef ROUND256
#undef ROUND256
#endif
#ifdef ROUND256_0_TO_15
#undef ROUND256_0_TO_15
#endif
#ifdef ROUND256_0_TO_15
#undef ROUND256_0_TO_15
#endif
#ifdef ROUND512
#undef ROUND512
#endif
#ifdef ROUND512_0_TO_15
#undef ROUND512_0_TO_15
#endif
#ifdef ROUND512_0_TO_15
#undef ROUND512_0_TO_15
#endif
#ifdef S32
#undef S32
#endif
#ifdef S64
#undef S64
#endif
#ifdef SET
#undef SET
#endif
#ifdef SET
#undef SET
#endif
#ifdef sha1_f1
#undef sha1_f1
#endif
#ifdef sha1_f2
#undef sha1_f2
#endif
#ifdef sha1_f3
#undef sha1_f3
#endif
#ifdef sha1_f4
#undef sha1_f4
#endif
#ifdef SHA1_RECOMPRESS
#undef SHA1_RECOMPRESS
#endif
#ifdef SHA1_STORE_STATE
#undef SHA1_STORE_STATE
#endif
#ifdef SHA2_UNROLL_TRANSFORM
#undef SHA2_UNROLL_TRANSFORM
#endif
#ifdef SHA2_USE_INTTYPES_H
#undef SHA2_USE_INTTYPES_H
#endif
#ifdef SHA2_USE_MEMSET_MEMCPY
#undef SHA2_USE_MEMSET_MEMCPY
#endif
#ifdef SHA256_BLOCK_LENGTH
#undef SHA256_BLOCK_LENGTH
#endif
#ifdef SHA256_DIGEST_LENGTH
#undef SHA256_DIGEST_LENGTH
#endif
#ifdef SHA256_DIGEST_STRING_LENGTH
#undef SHA256_DIGEST_STRING_LENGTH
#endif
#ifdef SHA256_SHORT_BLOCK_LENGTH
#undef SHA256_SHORT_BLOCK_LENGTH
#endif
#ifdef SHA384_BLOCK_LENGTH
#undef SHA384_BLOCK_LENGTH
#endif
#ifdef SHA384_DIGEST_LENGTH
#undef SHA384_DIGEST_LENGTH
#endif
#ifdef SHA384_DIGEST_STRING_LENGTH
#undef SHA384_DIGEST_STRING_LENGTH
#endif
#ifdef SHA384_SHORT_BLOCK_LENGTH
#undef SHA384_SHORT_BLOCK_LENGTH
#endif
#ifdef SHA512_BLOCK_LENGTH
#undef SHA512_BLOCK_LENGTH
#endif
#ifdef SHA512_DIGEST_LENGTH
#undef SHA512_DIGEST_LENGTH
#endif
#ifdef SHA512_DIGEST_STRING_LENGTH
#undef SHA512_DIGEST_STRING_LENGTH
#endif
#ifdef SHA512_SHORT_BLOCK_LENGTH
#undef SHA512_SHORT_BLOCK_LENGTH
#endif
#ifdef Sigma0_256
#undef Sigma0_256
#endif
#ifdef sigma0_256
#undef sigma0_256
#endif
#ifdef sigma0_512
#undef sigma0_512
#endif
#ifdef Sigma0_512
#undef Sigma0_512
#endif
#ifdef sigma1_256
#undef sigma1_256
#endif
#ifdef Sigma1_256
#undef Sigma1_256
#endif
#ifdef Sigma1_512
#undef Sigma1_512
#endif
#ifdef sigma1_512
#undef sigma1_512
#endif
#ifdef STEP
#undef STEP
#endif

/*
** Copyright (c) 2017 D. Richard Hipp
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the Simplified BSD License (also
** known as the "2-Clause License" or "FreeBSD License".)
**
** This program is distributed in the hope that it will be useful,
** but without any warranty; without even the implied warranty of
** merchantability or fitness for a particular purpose.
**
** Author contact information:
**   drh@hwaci.com
**   http://www.hwaci.com/drh/
**
*******************************************************************************
**
** This file contains an implementation of SHA3 (Keccak) hashing.
*/

#include <string.h>
#include <stdlib.h>

/*
** Macros to determine whether the machine is big or little endian,
** and whether or not that determination is run-time or compile-time.
**
** For best performance, an attempt is made to guess at the byte-order
** using C-preprocessor macros.  If that is unsuccessful, or if
** -DSHA3_BYTEORDER=0 is set, then byte-order is determined
** at run-time.
*/

#ifndef SHA3_BYTEORDER
# if defined(i386)     || defined(__i386__)   || defined(_M_IX86) ||    \
     defined(__x86_64) || defined(__x86_64__) || defined(_M_X64)  ||    \
     defined(_M_AMD64) || defined(_M_ARM)     || defined(__x86)   ||    \
     defined(__arm__)
#   define SHA3_BYTEORDER    1234
# elif defined(sparc)    || defined(__ppc__)
#   define SHA3_BYTEORDER    4321
# else
#   define SHA3_BYTEORDER 0
# endif
#endif

/*
** State structure for a SHA3 hash in progress
*/
typedef struct SHA3Context SHA3Context;
struct SHA3Context {
  union {
    unsigned long long s[25];                /* Keccak state. 5x5 lines of 64 bits each */
    unsigned char x[1600];    /* ... or 1600 bytes */
  } u;
  unsigned nRate;        /* Bytes of input accepted per Keccak iteration */
  unsigned nLoaded;      /* Input bytes loaded into u.x[] so far this cycle */
  unsigned ixMask;       /* Insert next input into u.x[nLoaded^ixMask]. */
  int iSize;       /* Remember the iSize */
};

/*
** A single step of the Keccak mixing function for a 1600-bit state
*/
static void KeccakF1600Step(SHA3Context *p){
  int i;
  unsigned long long B0, B1, B2, B3, B4;
  unsigned long long C0, C1, C2, C3, C4;
  unsigned long long D0, D1, D2, D3, D4;
  static const unsigned long long RC[] = {
    0x0000000000000001ULL,  0x0000000000008082ULL,
    0x800000000000808aULL,  0x8000000080008000ULL,
    0x000000000000808bULL,  0x0000000080000001ULL,
    0x8000000080008081ULL,  0x8000000000008009ULL,
    0x000000000000008aULL,  0x0000000000000088ULL,
    0x0000000080008009ULL,  0x000000008000000aULL,
    0x000000008000808bULL,  0x800000000000008bULL,
    0x8000000000008089ULL,  0x8000000000008003ULL,
    0x8000000000008002ULL,  0x8000000000000080ULL,
    0x000000000000800aULL,  0x800000008000000aULL,
    0x8000000080008081ULL,  0x8000000000008080ULL,
    0x0000000080000001ULL,  0x8000000080008008ULL
  };
# define A00 (p->u.s[0])
# define A01 (p->u.s[1])
# define A02 (p->u.s[2])
# define A03 (p->u.s[3])
# define A04 (p->u.s[4])
# define A10 (p->u.s[5])
# define A11 (p->u.s[6])
# define A12 (p->u.s[7])
# define A13 (p->u.s[8])
# define A14 (p->u.s[9])
# define A20 (p->u.s[10])
# define A21 (p->u.s[11])
# define A22 (p->u.s[12])
# define A23 (p->u.s[13])
# define A24 (p->u.s[14])
# define A30 (p->u.s[15])
# define A31 (p->u.s[16])
# define A32 (p->u.s[17])
# define A33 (p->u.s[18])
# define A34 (p->u.s[19])
# define A40 (p->u.s[20])
# define A41 (p->u.s[21])
# define A42 (p->u.s[22])
# define A43 (p->u.s[23])
# define A44 (p->u.s[24])
# define ROL64(a,x) ((a<<x)|(a>>(64-x)))

  for(i=0; i<24; i+=4){
    C0 = A00^A10^A20^A30^A40;
    C1 = A01^A11^A21^A31^A41;
    C2 = A02^A12^A22^A32^A42;
    C3 = A03^A13^A23^A33^A43;
    C4 = A04^A14^A24^A34^A44;
    D0 = C4^ROL64(C1, 1);
    D1 = C0^ROL64(C2, 1);
    D2 = C1^ROL64(C3, 1);
    D3 = C2^ROL64(C4, 1);
    D4 = C3^ROL64(C0, 1);

    B0 = (A00^D0);
    B1 = ROL64((A11^D1), 44);
    B2 = ROL64((A22^D2), 43);
    B3 = ROL64((A33^D3), 21);
    B4 = ROL64((A44^D4), 14);
    A00 =   B0 ^((~B1)&  B2 );
    A00 ^= RC[i];
    A11 =   B1 ^((~B2)&  B3 );
    A22 =   B2 ^((~B3)&  B4 );
    A33 =   B3 ^((~B4)&  B0 );
    A44 =   B4 ^((~B0)&  B1 );

    B2 = ROL64((A20^D0), 3);
    B3 = ROL64((A31^D1), 45);
    B4 = ROL64((A42^D2), 61);
    B0 = ROL64((A03^D3), 28);
    B1 = ROL64((A14^D4), 20);
    A20 =   B0 ^((~B1)&  B2 );
    A31 =   B1 ^((~B2)&  B3 );
    A42 =   B2 ^((~B3)&  B4 );
    A03 =   B3 ^((~B4)&  B0 );
    A14 =   B4 ^((~B0)&  B1 );

    B4 = ROL64((A40^D0), 18);
    B0 = ROL64((A01^D1), 1);
    B1 = ROL64((A12^D2), 6);
    B2 = ROL64((A23^D3), 25);
    B3 = ROL64((A34^D4), 8);
    A40 =   B0 ^((~B1)&  B2 );
    A01 =   B1 ^((~B2)&  B3 );
    A12 =   B2 ^((~B3)&  B4 );
    A23 =   B3 ^((~B4)&  B0 );
    A34 =   B4 ^((~B0)&  B1 );

    B1 = ROL64((A10^D0), 36);
    B2 = ROL64((A21^D1), 10);
    B3 = ROL64((A32^D2), 15);
    B4 = ROL64((A43^D3), 56);
    B0 = ROL64((A04^D4), 27);
    A10 =   B0 ^((~B1)&  B2 );
    A21 =   B1 ^((~B2)&  B3 );
    A32 =   B2 ^((~B3)&  B4 );
    A43 =   B3 ^((~B4)&  B0 );
    A04 =   B4 ^((~B0)&  B1 );

    B3 = ROL64((A30^D0), 41);
    B4 = ROL64((A41^D1), 2);
    B0 = ROL64((A02^D2), 62);
    B1 = ROL64((A13^D3), 55);
    B2 = ROL64((A24^D4), 39);
    A30 =   B0 ^((~B1)&  B2 );
    A41 =   B1 ^((~B2)&  B3 );
    A02 =   B2 ^((~B3)&  B4 );
    A13 =   B3 ^((~B4)&  B0 );
    A24 =   B4 ^((~B0)&  B1 );

    C0 = A00^A20^A40^A10^A30;
    C1 = A11^A31^A01^A21^A41;
    C2 = A22^A42^A12^A32^A02;
    C3 = A33^A03^A23^A43^A13;
    C4 = A44^A14^A34^A04^A24;
    D0 = C4^ROL64(C1, 1);
    D1 = C0^ROL64(C2, 1);
    D2 = C1^ROL64(C3, 1);
    D3 = C2^ROL64(C4, 1);
    D4 = C3^ROL64(C0, 1);

    B0 = (A00^D0);
    B1 = ROL64((A31^D1), 44);
    B2 = ROL64((A12^D2), 43);
    B3 = ROL64((A43^D3), 21);
    B4 = ROL64((A24^D4), 14);
    A00 =   B0 ^((~B1)&  B2 );
    A00 ^= RC[i+1];
    A31 =   B1 ^((~B2)&  B3 );
    A12 =   B2 ^((~B3)&  B4 );
    A43 =   B3 ^((~B4)&  B0 );
    A24 =   B4 ^((~B0)&  B1 );

    B2 = ROL64((A40^D0), 3);
    B3 = ROL64((A21^D1), 45);
    B4 = ROL64((A02^D2), 61);
    B0 = ROL64((A33^D3), 28);
    B1 = ROL64((A14^D4), 20);
    A40 =   B0 ^((~B1)&  B2 );
    A21 =   B1 ^((~B2)&  B3 );
    A02 =   B2 ^((~B3)&  B4 );
    A33 =   B3 ^((~B4)&  B0 );
    A14 =   B4 ^((~B0)&  B1 );

    B4 = ROL64((A30^D0), 18);
    B0 = ROL64((A11^D1), 1);
    B1 = ROL64((A42^D2), 6);
    B2 = ROL64((A23^D3), 25);
    B3 = ROL64((A04^D4), 8);
    A30 =   B0 ^((~B1)&  B2 );
    A11 =   B1 ^((~B2)&  B3 );
    A42 =   B2 ^((~B3)&  B4 );
    A23 =   B3 ^((~B4)&  B0 );
    A04 =   B4 ^((~B0)&  B1 );

    B1 = ROL64((A20^D0), 36);
    B2 = ROL64((A01^D1), 10);
    B3 = ROL64((A32^D2), 15);
    B4 = ROL64((A13^D3), 56);
    B0 = ROL64((A44^D4), 27);
    A20 =   B0 ^((~B1)&  B2 );
    A01 =   B1 ^((~B2)&  B3 );
    A32 =   B2 ^((~B3)&  B4 );
    A13 =   B3 ^((~B4)&  B0 );
    A44 =   B4 ^((~B0)&  B1 );

    B3 = ROL64((A10^D0), 41);
    B4 = ROL64((A41^D1), 2);
    B0 = ROL64((A22^D2), 62);
    B1 = ROL64((A03^D3), 55);
    B2 = ROL64((A34^D4), 39);
    A10 =   B0 ^((~B1)&  B2 );
    A41 =   B1 ^((~B2)&  B3 );
    A22 =   B2 ^((~B3)&  B4 );
    A03 =   B3 ^((~B4)&  B0 );
    A34 =   B4 ^((~B0)&  B1 );

    C0 = A00^A40^A30^A20^A10;
    C1 = A31^A21^A11^A01^A41;
    C2 = A12^A02^A42^A32^A22;
    C3 = A43^A33^A23^A13^A03;
    C4 = A24^A14^A04^A44^A34;
    D0 = C4^ROL64(C1, 1);
    D1 = C0^ROL64(C2, 1);
    D2 = C1^ROL64(C3, 1);
    D3 = C2^ROL64(C4, 1);
    D4 = C3^ROL64(C0, 1);

    B0 = (A00^D0);
    B1 = ROL64((A21^D1), 44);
    B2 = ROL64((A42^D2), 43);
    B3 = ROL64((A13^D3), 21);
    B4 = ROL64((A34^D4), 14);
    A00 =   B0 ^((~B1)&  B2 );
    A00 ^= RC[i+2];
    A21 =   B1 ^((~B2)&  B3 );
    A42 =   B2 ^((~B3)&  B4 );
    A13 =   B3 ^((~B4)&  B0 );
    A34 =   B4 ^((~B0)&  B1 );

    B2 = ROL64((A30^D0), 3);
    B3 = ROL64((A01^D1), 45);
    B4 = ROL64((A22^D2), 61);
    B0 = ROL64((A43^D3), 28);
    B1 = ROL64((A14^D4), 20);
    A30 =   B0 ^((~B1)&  B2 );
    A01 =   B1 ^((~B2)&  B3 );
    A22 =   B2 ^((~B3)&  B4 );
    A43 =   B3 ^((~B4)&  B0 );
    A14 =   B4 ^((~B0)&  B1 );

    B4 = ROL64((A10^D0), 18);
    B0 = ROL64((A31^D1), 1);
    B1 = ROL64((A02^D2), 6);
    B2 = ROL64((A23^D3), 25);
    B3 = ROL64((A44^D4), 8);
    A10 =   B0 ^((~B1)&  B2 );
    A31 =   B1 ^((~B2)&  B3 );
    A02 =   B2 ^((~B3)&  B4 );
    A23 =   B3 ^((~B4)&  B0 );
    A44 =   B4 ^((~B0)&  B1 );

    B1 = ROL64((A40^D0), 36);
    B2 = ROL64((A11^D1), 10);
    B3 = ROL64((A32^D2), 15);
    B4 = ROL64((A03^D3), 56);
    B0 = ROL64((A24^D4), 27);
    A40 =   B0 ^((~B1)&  B2 );
    A11 =   B1 ^((~B2)&  B3 );
    A32 =   B2 ^((~B3)&  B4 );
    A03 =   B3 ^((~B4)&  B0 );
    A24 =   B4 ^((~B0)&  B1 );

    B3 = ROL64((A20^D0), 41);
    B4 = ROL64((A41^D1), 2);
    B0 = ROL64((A12^D2), 62);
    B1 = ROL64((A33^D3), 55);
    B2 = ROL64((A04^D4), 39);
    A20 =   B0 ^((~B1)&  B2 );
    A41 =   B1 ^((~B2)&  B3 );
    A12 =   B2 ^((~B3)&  B4 );
    A33 =   B3 ^((~B4)&  B0 );
    A04 =   B4 ^((~B0)&  B1 );

    C0 = A00^A30^A10^A40^A20;
    C1 = A21^A01^A31^A11^A41;
    C2 = A42^A22^A02^A32^A12;
    C3 = A13^A43^A23^A03^A33;
    C4 = A34^A14^A44^A24^A04;
    D0 = C4^ROL64(C1, 1);
    D1 = C0^ROL64(C2, 1);
    D2 = C1^ROL64(C3, 1);
    D3 = C2^ROL64(C4, 1);
    D4 = C3^ROL64(C0, 1);

    B0 = (A00^D0);
    B1 = ROL64((A01^D1), 44);
    B2 = ROL64((A02^D2), 43);
    B3 = ROL64((A03^D3), 21);
    B4 = ROL64((A04^D4), 14);
    A00 =   B0 ^((~B1)&  B2 );
    A00 ^= RC[i+3];
    A01 =   B1 ^((~B2)&  B3 );
    A02 =   B2 ^((~B3)&  B4 );
    A03 =   B3 ^((~B4)&  B0 );
    A04 =   B4 ^((~B0)&  B1 );

    B2 = ROL64((A10^D0), 3);
    B3 = ROL64((A11^D1), 45);
    B4 = ROL64((A12^D2), 61);
    B0 = ROL64((A13^D3), 28);
    B1 = ROL64((A14^D4), 20);
    A10 =   B0 ^((~B1)&  B2 );
    A11 =   B1 ^((~B2)&  B3 );
    A12 =   B2 ^((~B3)&  B4 );
    A13 =   B3 ^((~B4)&  B0 );
    A14 =   B4 ^((~B0)&  B1 );

    B4 = ROL64((A20^D0), 18);
    B0 = ROL64((A21^D1), 1);
    B1 = ROL64((A22^D2), 6);
    B2 = ROL64((A23^D3), 25);
    B3 = ROL64((A24^D4), 8);
    A20 =   B0 ^((~B1)&  B2 );
    A21 =   B1 ^((~B2)&  B3 );
    A22 =   B2 ^((~B3)&  B4 );
    A23 =   B3 ^((~B4)&  B0 );
    A24 =   B4 ^((~B0)&  B1 );

    B1 = ROL64((A30^D0), 36);
    B2 = ROL64((A31^D1), 10);
    B3 = ROL64((A32^D2), 15);
    B4 = ROL64((A33^D3), 56);
    B0 = ROL64((A34^D4), 27);
    A30 =   B0 ^((~B1)&  B2 );
    A31 =   B1 ^((~B2)&  B3 );
    A32 =   B2 ^((~B3)&  B4 );
    A33 =   B3 ^((~B4)&  B0 );
    A34 =   B4 ^((~B0)&  B1 );

    B3 = ROL64((A40^D0), 41);
    B4 = ROL64((A41^D1), 2);
    B0 = ROL64((A42^D2), 62);
    B1 = ROL64((A43^D3), 55);
    B2 = ROL64((A44^D4), 39);
    A40 =   B0 ^((~B1)&  B2 );
    A41 =   B1 ^((~B2)&  B3 );
    A42 =   B2 ^((~B3)&  B4 );
    A43 =   B3 ^((~B4)&  B0 );
    A44 =   B4 ^((~B0)&  B1 );
  }
}

/*
** Initialize a new hash.  iSize determines the size of the hash
** in bits and should be one of 224, 256, 384, or 512.  Or iSize
** can be zero to use the default hash size of 224 bits.
*/
static void *SHA3Init(int iSize)
{
    SHA3Context *p;
    p = sqlite3_malloc(sizeof(SHA3Context));
    if (p==NULL)
        return NULL;
    memset(p, 0, sizeof(SHA3Context));
    if(iSize>=128 && iSize<=512)
    {
        p->nRate = (1600 - ((iSize + 31)&~31)*2)/8;
        p->iSize = iSize;
    }
    else
    {
        p->nRate = 144;
        p->iSize = 224;
    }
#if SHA3_BYTEORDER==1234
    /* Known to be little-endian at compile-time. No-op */
#elif SHA3_BYTEORDER==4321
    p->ixMask = 7;  /* Big-endian */
#else
    {
        static unsigned int one = 1;
        if( 1==*(unsigned char*)&one )
        {
            /* Little endian.  No byte swapping. */
            p->ixMask = 0;
        }
        else
        {
            /* Big endian.  Byte swap. */
            p->ixMask = 7;
        }
    }
#endif
    return (void*)p;
}

/*
** Make consecutive calls to the SHA3Update function to add new content
** to the hash
*/
static void SHA3Update(SHA3Context *p, const unsigned char *aData, unsigned long nData)
{
    unsigned int i = 0;
#if SHA3_BYTEORDER==1234
    if( (p->nLoaded % 8)==0 && ((aData - (const unsigned char*)0)&7)==0 )
    {
        for(; i+7<nData; i+=8)
        {
            p->u.s[p->nLoaded/8] ^= *(unsigned long long*)&aData[i];
            p->nLoaded += 8;
            if( p->nLoaded>=p->nRate )
            {
                KeccakF1600Step(p);
                p->nLoaded = 0;
            }
        }
    }
#endif
    for(; i<nData; i++)
    {
#if SHA3_BYTEORDER==1234
        p->u.x[p->nLoaded] ^= aData[i];
#elif SHA3_BYTEORDER==4321
        p->u.x[p->nLoaded^0x07] ^= aData[i];
#else
        p->u.x[p->nLoaded^p->ixMask] ^= aData[i];
#endif
        p->nLoaded++;
        if( p->nLoaded==p->nRate )
        {
            KeccakF1600Step(p);
            p->nLoaded = 0;
        }
    }
}

/*
** After all content has been added, invoke SHA3Final() to compute
** the final hash.  The function returns the length of the digest
** in bytes.
*/
static int SHA3Final(unsigned char hash[], SHA3Context *p)
{
    int hashsize;
    unsigned char *h;
    unsigned int i;
    unsigned char buffer[64];
    hashsize = p->iSize / 8;

    if( p->nLoaded==p->nRate-1 )
    {
        const unsigned char c1 = 0x86;
        SHA3Update(p, &c1, 1);
    }
    else
    {
        const unsigned char c2 = 0x06;
        const unsigned char c3 = 0x80;
        SHA3Update(p, &c2, 1);
        p->nLoaded = p->nRate - 1;
        SHA3Update(p, &c3, 1);
    }
    for(i=0; i<p->nRate; i++)
    {
        p->u.x[i+p->nRate] = p->u.x[i^p->ixMask];
    }
    h = &p->u.x[p->nRate];
    for (i=0; i<hashsize; i++)
        hash[i] = h[i];
    sqlite3_free(p);
    return hashsize;
}

#ifdef A00
#undef A00
#endif
#ifdef A01
#undef A01
#endif
#ifdef A02
#undef A02
#endif
#ifdef A03
#undef A03
#endif
#ifdef A04
#undef A04
#endif
#ifdef A10
#undef A10
#endif
#ifdef A11
#undef A11
#endif
#ifdef A12
#undef A12
#endif
#ifdef A13
#undef A13
#endif
#ifdef A14
#undef A14
#endif
#ifdef A20
#undef A20
#endif
#ifdef A21
#undef A21
#endif
#ifdef A22
#undef A22
#endif
#ifdef A23
#undef A23
#endif
#ifdef A24
#undef A24
#endif
#ifdef A30
#undef A30
#endif
#ifdef A31
#undef A31
#endif
#ifdef A32
#undef A32
#endif
#ifdef A33
#undef A33
#endif
#ifdef A34
#undef A34
#endif
#ifdef A40
#undef A40
#endif
#ifdef A41
#undef A41
#endif
#ifdef A42
#undef A42
#endif
#ifdef A43
#undef A43
#endif
#ifdef A44
#undef A44
#endif
#ifdef ROL64
#undef ROL64
#endif

/* End Of SHA3.C Include */


/* Include sha1hard.c */

/*
** The code in this file is the concatenation of several files
** copied out of
**
**     https://github.com/cr-marcstevens/sha1collisiondetection
**
** The copy was made on 2017-03-01.  Some minor formatting changes
** were made but otherwise the code is unchanged.  All
** original copyright claims are preserved.
**
** The code here implements a version of the SHA1 hash function that
** is not vulnerable to crypto-analysis based attacks.  If an input
** is detected that looks like it might have been the result of a
** crypto-analysis attack, then the hash is perturbed to generate a
** completely different hash.  The authors claim that the chance of
** a false-positive is vanishingly small.
*/
/*MAKEHEADERS-STOP*/

// #include "config.h"

#include <stdint.h>

/*************** File:  lib/sha1.c ****************/
/***
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow (danshu@microsoft.com)
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/
/*************** File:  LICENSE.txt ***************/
/*
** MIT License
**
** Copyright (c) 2017:
**     Marc Stevens
**     Cryptology Group
**     Centrum Wiskunde & Informatica
**     P.O. Box 94079, 1090 GB Amsterdam, Netherlands
**     marc@marc-stevens.nl
**
**     Dan Shumow
**     Microsoft Research
**     danshu@microsoft.com
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

#include <string.h>
#include <memory.h>
#include <stdio.h>

#define DVMASKSIZE 1
typedef struct { int dvType; int dvK; int dvB; int testt; int maski; int maskb; uint32_t dm[80]; } dv_info_t;
#define DOSTORESTATE58
#define DOSTORESTATE65
typedef void(*sha1_recompression_type)(uint32_t*, uint32_t*, const uint32_t*, const uint32_t*);
static void sha1_message_expansion(uint32_t W[80]);
static void sha1_compression(uint32_t ihv[5], const uint32_t m[16]);
static void sha1_compression_W(uint32_t ihv[5], const uint32_t W[80]);
static void sha1_compression_states(uint32_t ihv[5], const uint32_t W[80], uint32_t states[80][5]);
static sha1_recompression_type sha1_recompression_step[80];
typedef void(*collision_block_callback)(uint64_t, const uint32_t*, const uint32_t*, const uint32_t*, const uint32_t*);
typedef struct {
    uint64_t total;
    uint32_t ihv[5];
    unsigned char buffer[64];
    int bigendian;
    int found_collision;
    int safe_hash;
    int detect_coll;
    int ubc_check;
    int reduced_round_coll;
    collision_block_callback callback;

    uint32_t ihv1[5];
    uint32_t ihv2[5];
    uint32_t m1[80];
    uint32_t m2[80];
    uint32_t states[80][5];
} SHA1_CTX;

/******************** File: lib/ubc_check.c **************************/
/***
* Copyright 2017 Marc Stevens <marc@marc-stevens.nl>, Dan Shumow <danshu@microsoft.com>
* Distributed under the MIT Software License.
* See accompanying file LICENSE.txt or copy at
* https://opensource.org/licenses/MIT
***/

/*
** this file was generated by the 'parse_bitrel' program in the tools section
** using the data files from directory 'tools/data/3565'
**
** sha1_dvs contains a list of SHA-1 Disturbance Vectors (DV) to check
** dvType, dvK and dvB define the DV: I(K,B) or II(K,B) (see the paper)
** dm[80] is the expanded message block XOR-difference defined by the DV
** testt is the step to do the recompression from for collision detection
** maski and maskb define the bit to check for each DV in the dvmask returned by ubc_check
**
** ubc_check takes as input an expanded message block and verifies the unavoidable bitconditions for all listed DVs
** it returns a dvmask where each bit belonging to a DV is set if all unavoidable bitconditions for that DV have been met
** thus one needs to do the recompression check for each DV that has its bit set
**
** ubc_check is programmatically generated and the unavoidable bitconditions have been hardcoded
** a directly verifiable version named ubc_check_verify can be found in ubc_check_verify.c
** ubc_check has been verified against ubc_check_verify using the 'ubc_check_test' program in the tools section
*/

static const uint32_t DV_I_43_0_bit   = (uint32_t)(1) << 0;
static const uint32_t DV_I_44_0_bit   = (uint32_t)(1) << 1;
static const uint32_t DV_I_45_0_bit   = (uint32_t)(1) << 2;
static const uint32_t DV_I_46_0_bit   = (uint32_t)(1) << 3;
static const uint32_t DV_I_46_2_bit   = (uint32_t)(1) << 4;
static const uint32_t DV_I_47_0_bit   = (uint32_t)(1) << 5;
static const uint32_t DV_I_47_2_bit   = (uint32_t)(1) << 6;
static const uint32_t DV_I_48_0_bit   = (uint32_t)(1) << 7;
static const uint32_t DV_I_48_2_bit   = (uint32_t)(1) << 8;
static const uint32_t DV_I_49_0_bit   = (uint32_t)(1) << 9;
static const uint32_t DV_I_49_2_bit   = (uint32_t)(1) << 10;
static const uint32_t DV_I_50_0_bit   = (uint32_t)(1) << 11;
static const uint32_t DV_I_50_2_bit   = (uint32_t)(1) << 12;
static const uint32_t DV_I_51_0_bit   = (uint32_t)(1) << 13;
static const uint32_t DV_I_51_2_bit   = (uint32_t)(1) << 14;
static const uint32_t DV_I_52_0_bit   = (uint32_t)(1) << 15;
static const uint32_t DV_II_45_0_bit   = (uint32_t)(1) << 16;
static const uint32_t DV_II_46_0_bit   = (uint32_t)(1) << 17;
static const uint32_t DV_II_46_2_bit   = (uint32_t)(1) << 18;
static const uint32_t DV_II_47_0_bit   = (uint32_t)(1) << 19;
static const uint32_t DV_II_48_0_bit   = (uint32_t)(1) << 20;
static const uint32_t DV_II_49_0_bit   = (uint32_t)(1) << 21;
static const uint32_t DV_II_49_2_bit   = (uint32_t)(1) << 22;
static const uint32_t DV_II_50_0_bit   = (uint32_t)(1) << 23;
static const uint32_t DV_II_50_2_bit   = (uint32_t)(1) << 24;
static const uint32_t DV_II_51_0_bit   = (uint32_t)(1) << 25;
static const uint32_t DV_II_51_2_bit   = (uint32_t)(1) << 26;
static const uint32_t DV_II_52_0_bit   = (uint32_t)(1) << 27;
static const uint32_t DV_II_53_0_bit   = (uint32_t)(1) << 28;
static const uint32_t DV_II_54_0_bit   = (uint32_t)(1) << 29;
static const uint32_t DV_II_55_0_bit   = (uint32_t)(1) << 30;
static const uint32_t DV_II_56_0_bit   = (uint32_t)(1) << 31;

static dv_info_t sha1_dvs[] =
{
  {1,43,0,58,0,0, { 0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164,0x00000408,0x800000e6,0x8000004c,0x00000803,0x80000161,0x80000599 } }
, {1,44,0,58,0,1, { 0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164,0x00000408,0x800000e6,0x8000004c,0x00000803,0x80000161 } }
, {1,45,0,58,0,2, { 0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164,0x00000408,0x800000e6,0x8000004c,0x00000803 } }
, {1,46,0,58,0,3, { 0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164,0x00000408,0x800000e6,0x8000004c } }
, {1,46,2,58,0,4, { 0xb0000040,0xd0000053,0xd0000022,0x20000000,0x60000032,0x60000043,0x20000040,0xe0000042,0x60000002,0x80000001,0x00000020,0x00000003,0x40000052,0x40000040,0xe0000052,0xa0000000,0x80000040,0x20000001,0x20000060,0x80000001,0x40000042,0xc0000043,0x40000022,0x00000003,0x40000042,0xc0000043,0xc0000022,0x00000001,0x40000002,0xc0000043,0x40000062,0x80000001,0x40000042,0x40000042,0x40000002,0x00000002,0x00000040,0x80000002,0x80000000,0x80000002,0x80000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000000,0x00000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000101,0x00000009,0x00000012,0x00000202,0x0000001a,0x00000124,0x0000040c,0x00000026,0x0000004a,0x0000080a,0x00000060,0x00000590,0x00001020,0x0000039a,0x00000132 } }
, {1,47,0,58,0,5, { 0xc8000010,0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164,0x00000408,0x800000e6 } }
, {1,47,2,58,0,6, { 0x20000043,0xb0000040,0xd0000053,0xd0000022,0x20000000,0x60000032,0x60000043,0x20000040,0xe0000042,0x60000002,0x80000001,0x00000020,0x00000003,0x40000052,0x40000040,0xe0000052,0xa0000000,0x80000040,0x20000001,0x20000060,0x80000001,0x40000042,0xc0000043,0x40000022,0x00000003,0x40000042,0xc0000043,0xc0000022,0x00000001,0x40000002,0xc0000043,0x40000062,0x80000001,0x40000042,0x40000042,0x40000002,0x00000002,0x00000040,0x80000002,0x80000000,0x80000002,0x80000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000000,0x00000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000101,0x00000009,0x00000012,0x00000202,0x0000001a,0x00000124,0x0000040c,0x00000026,0x0000004a,0x0000080a,0x00000060,0x00000590,0x00001020,0x0000039a } }
, {1,48,0,58,0,7, { 0xb800000a,0xc8000010,0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164,0x00000408 } }
, {1,48,2,58,0,8, { 0xe000002a,0x20000043,0xb0000040,0xd0000053,0xd0000022,0x20000000,0x60000032,0x60000043,0x20000040,0xe0000042,0x60000002,0x80000001,0x00000020,0x00000003,0x40000052,0x40000040,0xe0000052,0xa0000000,0x80000040,0x20000001,0x20000060,0x80000001,0x40000042,0xc0000043,0x40000022,0x00000003,0x40000042,0xc0000043,0xc0000022,0x00000001,0x40000002,0xc0000043,0x40000062,0x80000001,0x40000042,0x40000042,0x40000002,0x00000002,0x00000040,0x80000002,0x80000000,0x80000002,0x80000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000000,0x00000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000101,0x00000009,0x00000012,0x00000202,0x0000001a,0x00000124,0x0000040c,0x00000026,0x0000004a,0x0000080a,0x00000060,0x00000590,0x00001020 } }
, {1,49,0,58,0,9, { 0x18000000,0xb800000a,0xc8000010,0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018,0x00000164 } }
, {1,49,2,58,0,10, { 0x60000000,0xe000002a,0x20000043,0xb0000040,0xd0000053,0xd0000022,0x20000000,0x60000032,0x60000043,0x20000040,0xe0000042,0x60000002,0x80000001,0x00000020,0x00000003,0x40000052,0x40000040,0xe0000052,0xa0000000,0x80000040,0x20000001,0x20000060,0x80000001,0x40000042,0xc0000043,0x40000022,0x00000003,0x40000042,0xc0000043,0xc0000022,0x00000001,0x40000002,0xc0000043,0x40000062,0x80000001,0x40000042,0x40000042,0x40000002,0x00000002,0x00000040,0x80000002,0x80000000,0x80000002,0x80000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000000,0x00000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000101,0x00000009,0x00000012,0x00000202,0x0000001a,0x00000124,0x0000040c,0x00000026,0x0000004a,0x0000080a,0x00000060,0x00000590 } }
, {1,50,0,65,0,11, { 0x0800000c,0x18000000,0xb800000a,0xc8000010,0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202,0x00000018 } }
, {1,50,2,65,0,12, { 0x20000030,0x60000000,0xe000002a,0x20000043,0xb0000040,0xd0000053,0xd0000022,0x20000000,0x60000032,0x60000043,0x20000040,0xe0000042,0x60000002,0x80000001,0x00000020,0x00000003,0x40000052,0x40000040,0xe0000052,0xa0000000,0x80000040,0x20000001,0x20000060,0x80000001,0x40000042,0xc0000043,0x40000022,0x00000003,0x40000042,0xc0000043,0xc0000022,0x00000001,0x40000002,0xc0000043,0x40000062,0x80000001,0x40000042,0x40000042,0x40000002,0x00000002,0x00000040,0x80000002,0x80000000,0x80000002,0x80000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000000,0x00000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000101,0x00000009,0x00000012,0x00000202,0x0000001a,0x00000124,0x0000040c,0x00000026,0x0000004a,0x0000080a,0x00000060 } }
, {1,51,0,65,0,13, { 0xe8000000,0x0800000c,0x18000000,0xb800000a,0xc8000010,0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012,0x80000202 } }
, {1,51,2,65,0,14, { 0xa0000003,0x20000030,0x60000000,0xe000002a,0x20000043,0xb0000040,0xd0000053,0xd0000022,0x20000000,0x60000032,0x60000043,0x20000040,0xe0000042,0x60000002,0x80000001,0x00000020,0x00000003,0x40000052,0x40000040,0xe0000052,0xa0000000,0x80000040,0x20000001,0x20000060,0x80000001,0x40000042,0xc0000043,0x40000022,0x00000003,0x40000042,0xc0000043,0xc0000022,0x00000001,0x40000002,0xc0000043,0x40000062,0x80000001,0x40000042,0x40000042,0x40000002,0x00000002,0x00000040,0x80000002,0x80000000,0x80000002,0x80000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000000,0x00000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000101,0x00000009,0x00000012,0x00000202,0x0000001a,0x00000124,0x0000040c,0x00000026,0x0000004a,0x0000080a } }
, {1,52,0,65,0,15, { 0x04000010,0xe8000000,0x0800000c,0x18000000,0xb800000a,0xc8000010,0x2c000010,0xf4000014,0xb4000008,0x08000000,0x9800000c,0xd8000010,0x08000010,0xb8000010,0x98000000,0x60000000,0x00000008,0xc0000000,0x90000014,0x10000010,0xb8000014,0x28000000,0x20000010,0x48000000,0x08000018,0x60000000,0x90000010,0xf0000010,0x90000008,0xc0000000,0x90000010,0xf0000010,0xb0000008,0x40000000,0x90000000,0xf0000010,0x90000018,0x60000000,0x90000010,0x90000010,0x90000000,0x80000000,0x00000010,0xa0000000,0x20000000,0xa0000000,0x20000010,0x00000000,0x20000010,0x20000000,0x00000010,0x20000000,0x00000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000040,0x40000002,0x80000004,0x80000080,0x80000006,0x00000049,0x00000103,0x80000009,0x80000012 } }
, {2,45,0,58,0,16, { 0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b,0x0000011b,0x8000016d,0x8000041a,0x000002e4,0x80000054,0x00000967 } }
, {2,46,0,58,0,17, { 0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b,0x0000011b,0x8000016d,0x8000041a,0x000002e4,0x80000054 } }
, {2,46,2,58,0,18, { 0x90000070,0xb0000053,0x30000008,0x00000043,0xd0000072,0xb0000010,0xf0000062,0xc0000042,0x00000030,0xe0000042,0x20000060,0xe0000041,0x20000050,0xc0000041,0xe0000072,0xa0000003,0xc0000012,0x60000041,0xc0000032,0x20000001,0xc0000002,0xe0000042,0x60000042,0x80000002,0x00000000,0x00000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000001,0x00000060,0x80000003,0x40000002,0xc0000040,0xc0000002,0x80000000,0x80000000,0x80000002,0x00000040,0x00000002,0x80000000,0x80000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000105,0x00000089,0x00000016,0x0000020b,0x0000011b,0x0000012d,0x0000041e,0x00000224,0x00000050,0x0000092e,0x0000046c,0x000005b6,0x0000106a,0x00000b90,0x00000152 } }
, {2,47,0,58,0,19, { 0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b,0x0000011b,0x8000016d,0x8000041a,0x000002e4 } }
, {2,48,0,58,0,20, { 0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b,0x0000011b,0x8000016d,0x8000041a } }
, {2,49,0,58,0,21, { 0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b,0x0000011b,0x8000016d } }
, {2,49,2,58,0,22, { 0xf0000010,0xf000006a,0x80000040,0x90000070,0xb0000053,0x30000008,0x00000043,0xd0000072,0xb0000010,0xf0000062,0xc0000042,0x00000030,0xe0000042,0x20000060,0xe0000041,0x20000050,0xc0000041,0xe0000072,0xa0000003,0xc0000012,0x60000041,0xc0000032,0x20000001,0xc0000002,0xe0000042,0x60000042,0x80000002,0x00000000,0x00000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000001,0x00000060,0x80000003,0x40000002,0xc0000040,0xc0000002,0x80000000,0x80000000,0x80000002,0x00000040,0x00000002,0x80000000,0x80000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000105,0x00000089,0x00000016,0x0000020b,0x0000011b,0x0000012d,0x0000041e,0x00000224,0x00000050,0x0000092e,0x0000046c,0x000005b6 } }
, {2,50,0,65,0,23, { 0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b,0x0000011b } }
, {2,50,2,65,0,24, { 0xd0000072,0xf0000010,0xf000006a,0x80000040,0x90000070,0xb0000053,0x30000008,0x00000043,0xd0000072,0xb0000010,0xf0000062,0xc0000042,0x00000030,0xe0000042,0x20000060,0xe0000041,0x20000050,0xc0000041,0xe0000072,0xa0000003,0xc0000012,0x60000041,0xc0000032,0x20000001,0xc0000002,0xe0000042,0x60000042,0x80000002,0x00000000,0x00000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000001,0x00000060,0x80000003,0x40000002,0xc0000040,0xc0000002,0x80000000,0x80000000,0x80000002,0x00000040,0x00000002,0x80000000,0x80000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000105,0x00000089,0x00000016,0x0000020b,0x0000011b,0x0000012d,0x0000041e,0x00000224,0x00000050,0x0000092e,0x0000046c } }
, {2,51,0,65,0,25, { 0xc0000010,0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014,0x8000024b } }
, {2,51,2,65,0,26, { 0x00000043,0xd0000072,0xf0000010,0xf000006a,0x80000040,0x90000070,0xb0000053,0x30000008,0x00000043,0xd0000072,0xb0000010,0xf0000062,0xc0000042,0x00000030,0xe0000042,0x20000060,0xe0000041,0x20000050,0xc0000041,0xe0000072,0xa0000003,0xc0000012,0x60000041,0xc0000032,0x20000001,0xc0000002,0xe0000042,0x60000042,0x80000002,0x00000000,0x00000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000000,0x00000040,0x80000001,0x00000060,0x80000003,0x40000002,0xc0000040,0xc0000002,0x80000000,0x80000000,0x80000002,0x00000040,0x00000002,0x80000000,0x80000000,0x80000000,0x00000002,0x00000040,0x00000000,0x80000040,0x80000002,0x00000000,0x80000000,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000004,0x00000080,0x00000004,0x00000009,0x00000105,0x00000089,0x00000016,0x0000020b,0x0000011b,0x0000012d,0x0000041e,0x00000224,0x00000050,0x0000092e } }
, {2,52,0,65,0,27, { 0x0c000002,0xc0000010,0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089,0x00000014 } }
, {2,53,0,65,0,28, { 0xcc000014,0x0c000002,0xc0000010,0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107,0x00000089 } }
, {2,54,0,65,0,29, { 0x0400001c,0xcc000014,0x0c000002,0xc0000010,0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b,0x80000107 } }
, {2,55,0,65,0,30, { 0x00000010,0x0400001c,0xcc000014,0x0c000002,0xc0000010,0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046,0x4000004b } }
, {2,56,0,65,0,31, { 0x2600001a,0x00000010,0x0400001c,0xcc000014,0x0c000002,0xc0000010,0xb400001c,0x3c000004,0xbc00001a,0x20000010,0x2400001c,0xec000014,0x0c000002,0xc0000010,0xb400001c,0x2c000004,0xbc000018,0xb0000010,0x0000000c,0xb8000010,0x08000018,0x78000010,0x08000014,0x70000010,0xb800001c,0xe8000000,0xb0000004,0x58000010,0xb000000c,0x48000000,0xb0000000,0xb8000010,0x98000010,0xa0000000,0x00000000,0x00000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0x20000000,0x00000010,0x60000000,0x00000018,0xe0000000,0x90000000,0x30000010,0xb0000000,0x20000000,0x20000000,0xa0000000,0x00000010,0x80000000,0x20000000,0x20000000,0x20000000,0x80000000,0x00000010,0x00000000,0x20000010,0xa0000000,0x00000000,0x20000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000020,0x00000001,0x40000002,0x40000041,0x40000022,0x80000005,0xc0000082,0xc0000046 } }
, {0,0,0,0,0,0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}
};
static void  ubc_check(const uint32_t W[80], uint32_t dvmask[1])
{
  uint32_t mask = ~((uint32_t)(0));
  mask &= (((((W[44]^W[45])>>29)&1)-1) | ~(DV_I_48_0_bit|DV_I_51_0_bit|DV_I_52_0_bit|DV_II_45_0_bit|DV_II_46_0_bit|DV_II_50_0_bit|DV_II_51_0_bit));
  mask &= (((((W[49]^W[50])>>29)&1)-1) | ~(DV_I_46_0_bit|DV_II_45_0_bit|DV_II_50_0_bit|DV_II_51_0_bit|DV_II_55_0_bit|DV_II_56_0_bit));
  mask &= (((((W[48]^W[49])>>29)&1)-1) | ~(DV_I_45_0_bit|DV_I_52_0_bit|DV_II_49_0_bit|DV_II_50_0_bit|DV_II_54_0_bit|DV_II_55_0_bit));
  mask &= ((((W[47]^(W[50]>>25))&(1<<4))-(1<<4)) | ~(DV_I_47_0_bit|DV_I_49_0_bit|DV_I_51_0_bit|DV_II_45_0_bit|DV_II_51_0_bit|DV_II_56_0_bit));
  mask &= (((((W[47]^W[48])>>29)&1)-1) | ~(DV_I_44_0_bit|DV_I_51_0_bit|DV_II_48_0_bit|DV_II_49_0_bit|DV_II_53_0_bit|DV_II_54_0_bit));
  mask &= (((((W[46]>>4)^(W[49]>>29))&1)-1) | ~(DV_I_46_0_bit|DV_I_48_0_bit|DV_I_50_0_bit|DV_I_52_0_bit|DV_II_50_0_bit|DV_II_55_0_bit));
  mask &= (((((W[46]^W[47])>>29)&1)-1) | ~(DV_I_43_0_bit|DV_I_50_0_bit|DV_II_47_0_bit|DV_II_48_0_bit|DV_II_52_0_bit|DV_II_53_0_bit));
  mask &= (((((W[45]>>4)^(W[48]>>29))&1)-1) | ~(DV_I_45_0_bit|DV_I_47_0_bit|DV_I_49_0_bit|DV_I_51_0_bit|DV_II_49_0_bit|DV_II_54_0_bit));
  mask &= (((((W[45]^W[46])>>29)&1)-1) | ~(DV_I_49_0_bit|DV_I_52_0_bit|DV_II_46_0_bit|DV_II_47_0_bit|DV_II_51_0_bit|DV_II_52_0_bit));
  mask &= (((((W[44]>>4)^(W[47]>>29))&1)-1) | ~(DV_I_44_0_bit|DV_I_46_0_bit|DV_I_48_0_bit|DV_I_50_0_bit|DV_II_48_0_bit|DV_II_53_0_bit));
  mask &= (((((W[43]>>4)^(W[46]>>29))&1)-1) | ~(DV_I_43_0_bit|DV_I_45_0_bit|DV_I_47_0_bit|DV_I_49_0_bit|DV_II_47_0_bit|DV_II_52_0_bit));
  mask &= (((((W[43]^W[44])>>29)&1)-1) | ~(DV_I_47_0_bit|DV_I_50_0_bit|DV_I_51_0_bit|DV_II_45_0_bit|DV_II_49_0_bit|DV_II_50_0_bit));
  mask &= (((((W[42]>>4)^(W[45]>>29))&1)-1) | ~(DV_I_44_0_bit|DV_I_46_0_bit|DV_I_48_0_bit|DV_I_52_0_bit|DV_II_46_0_bit|DV_II_51_0_bit));
  mask &= (((((W[41]>>4)^(W[44]>>29))&1)-1) | ~(DV_I_43_0_bit|DV_I_45_0_bit|DV_I_47_0_bit|DV_I_51_0_bit|DV_II_45_0_bit|DV_II_50_0_bit));
  mask &= (((((W[40]^W[41])>>29)&1)-1) | ~(DV_I_44_0_bit|DV_I_47_0_bit|DV_I_48_0_bit|DV_II_46_0_bit|DV_II_47_0_bit|DV_II_56_0_bit));
  mask &= (((((W[54]^W[55])>>29)&1)-1) | ~(DV_I_51_0_bit|DV_II_47_0_bit|DV_II_50_0_bit|DV_II_55_0_bit|DV_II_56_0_bit));
  mask &= (((((W[53]^W[54])>>29)&1)-1) | ~(DV_I_50_0_bit|DV_II_46_0_bit|DV_II_49_0_bit|DV_II_54_0_bit|DV_II_55_0_bit));
  mask &= (((((W[52]^W[53])>>29)&1)-1) | ~(DV_I_49_0_bit|DV_II_45_0_bit|DV_II_48_0_bit|DV_II_53_0_bit|DV_II_54_0_bit));
  mask &= ((((W[50]^(W[53]>>25))&(1<<4))-(1<<4)) | ~(DV_I_50_0_bit|DV_I_52_0_bit|DV_II_46_0_bit|DV_II_48_0_bit|DV_II_54_0_bit));
  mask &= (((((W[50]^W[51])>>29)&1)-1) | ~(DV_I_47_0_bit|DV_II_46_0_bit|DV_II_51_0_bit|DV_II_52_0_bit|DV_II_56_0_bit));
  mask &= ((((W[49]^(W[52]>>25))&(1<<4))-(1<<4)) | ~(DV_I_49_0_bit|DV_I_51_0_bit|DV_II_45_0_bit|DV_II_47_0_bit|DV_II_53_0_bit));
  mask &= ((((W[48]^(W[51]>>25))&(1<<4))-(1<<4)) | ~(DV_I_48_0_bit|DV_I_50_0_bit|DV_I_52_0_bit|DV_II_46_0_bit|DV_II_52_0_bit));
  mask &= (((((W[42]^W[43])>>29)&1)-1) | ~(DV_I_46_0_bit|DV_I_49_0_bit|DV_I_50_0_bit|DV_II_48_0_bit|DV_II_49_0_bit));
  mask &= (((((W[41]^W[42])>>29)&1)-1) | ~(DV_I_45_0_bit|DV_I_48_0_bit|DV_I_49_0_bit|DV_II_47_0_bit|DV_II_48_0_bit));
  mask &= (((((W[40]>>4)^(W[43]>>29))&1)-1) | ~(DV_I_44_0_bit|DV_I_46_0_bit|DV_I_50_0_bit|DV_II_49_0_bit|DV_II_56_0_bit));
  mask &= (((((W[39]>>4)^(W[42]>>29))&1)-1) | ~(DV_I_43_0_bit|DV_I_45_0_bit|DV_I_49_0_bit|DV_II_48_0_bit|DV_II_55_0_bit));
  if (mask & (DV_I_44_0_bit|DV_I_48_0_bit|DV_II_47_0_bit|DV_II_54_0_bit|DV_II_56_0_bit))
    mask &= (((((W[38]>>4)^(W[41]>>29))&1)-1) | ~(DV_I_44_0_bit|DV_I_48_0_bit|DV_II_47_0_bit|DV_II_54_0_bit|DV_II_56_0_bit));
  mask &= (((((W[37]>>4)^(W[40]>>29))&1)-1) | ~(DV_I_43_0_bit|DV_I_47_0_bit|DV_II_46_0_bit|DV_II_53_0_bit|DV_II_55_0_bit));
  if (mask & (DV_I_52_0_bit|DV_II_48_0_bit|DV_II_51_0_bit|DV_II_56_0_bit))
    mask &= (((((W[55]^W[56])>>29)&1)-1) | ~(DV_I_52_0_bit|DV_II_48_0_bit|DV_II_51_0_bit|DV_II_56_0_bit));
  if (mask & (DV_I_52_0_bit|DV_II_48_0_bit|DV_II_50_0_bit|DV_II_56_0_bit))
    mask &= ((((W[52]^(W[55]>>25))&(1<<4))-(1<<4)) | ~(DV_I_52_0_bit|DV_II_48_0_bit|DV_II_50_0_bit|DV_II_56_0_bit));
  if (mask & (DV_I_51_0_bit|DV_II_47_0_bit|DV_II_49_0_bit|DV_II_55_0_bit))
    mask &= ((((W[51]^(W[54]>>25))&(1<<4))-(1<<4)) | ~(DV_I_51_0_bit|DV_II_47_0_bit|DV_II_49_0_bit|DV_II_55_0_bit));
  if (mask & (DV_I_48_0_bit|DV_II_47_0_bit|DV_II_52_0_bit|DV_II_53_0_bit))
    mask &= (((((W[51]^W[52])>>29)&1)-1) | ~(DV_I_48_0_bit|DV_II_47_0_bit|DV_II_52_0_bit|DV_II_53_0_bit));
  if (mask & (DV_I_46_0_bit|DV_I_49_0_bit|DV_II_45_0_bit|DV_II_48_0_bit))
    mask &= (((((W[36]>>4)^(W[40]>>29))&1)-1) | ~(DV_I_46_0_bit|DV_I_49_0_bit|DV_II_45_0_bit|DV_II_48_0_bit));
  if (mask & (DV_I_52_0_bit|DV_II_48_0_bit|DV_II_49_0_bit))
    mask &= ((0-(((W[53]^W[56])>>29)&1)) | ~(DV_I_52_0_bit|DV_II_48_0_bit|DV_II_49_0_bit));
  if (mask & (DV_I_50_0_bit|DV_II_46_0_bit|DV_II_47_0_bit))
    mask &= ((0-(((W[51]^W[54])>>29)&1)) | ~(DV_I_50_0_bit|DV_II_46_0_bit|DV_II_47_0_bit));
  if (mask & (DV_I_49_0_bit|DV_I_51_0_bit|DV_II_45_0_bit))
    mask &= ((0-(((W[50]^W[52])>>29)&1)) | ~(DV_I_49_0_bit|DV_I_51_0_bit|DV_II_45_0_bit));
  if (mask & (DV_I_48_0_bit|DV_I_50_0_bit|DV_I_52_0_bit))
    mask &= ((0-(((W[49]^W[51])>>29)&1)) | ~(DV_I_48_0_bit|DV_I_50_0_bit|DV_I_52_0_bit));
  if (mask & (DV_I_47_0_bit|DV_I_49_0_bit|DV_I_51_0_bit))
    mask &= ((0-(((W[48]^W[50])>>29)&1)) | ~(DV_I_47_0_bit|DV_I_49_0_bit|DV_I_51_0_bit));
  if (mask & (DV_I_46_0_bit|DV_I_48_0_bit|DV_I_50_0_bit))
    mask &= ((0-(((W[47]^W[49])>>29)&1)) | ~(DV_I_46_0_bit|DV_I_48_0_bit|DV_I_50_0_bit));
  if (mask & (DV_I_45_0_bit|DV_I_47_0_bit|DV_I_49_0_bit))
    mask &= ((0-(((W[46]^W[48])>>29)&1)) | ~(DV_I_45_0_bit|DV_I_47_0_bit|DV_I_49_0_bit));
  mask &= ((((W[45]^W[47])&(1<<6))-(1<<6)) | ~(DV_I_47_2_bit|DV_I_49_2_bit|DV_I_51_2_bit));
  if (mask & (DV_I_44_0_bit|DV_I_46_0_bit|DV_I_48_0_bit))
    mask &= ((0-(((W[45]^W[47])>>29)&1)) | ~(DV_I_44_0_bit|DV_I_46_0_bit|DV_I_48_0_bit));
  mask &= (((((W[44]^W[46])>>6)&1)-1) | ~(DV_I_46_2_bit|DV_I_48_2_bit|DV_I_50_2_bit));
  if (mask & (DV_I_43_0_bit|DV_I_45_0_bit|DV_I_47_0_bit))
    mask &= ((0-(((W[44]^W[46])>>29)&1)) | ~(DV_I_43_0_bit|DV_I_45_0_bit|DV_I_47_0_bit));
  mask &= ((0-((W[41]^(W[42]>>5))&(1<<1))) | ~(DV_I_48_2_bit|DV_II_46_2_bit|DV_II_51_2_bit));
  mask &= ((0-((W[40]^(W[41]>>5))&(1<<1))) | ~(DV_I_47_2_bit|DV_I_51_2_bit|DV_II_50_2_bit));
  if (mask & (DV_I_44_0_bit|DV_I_46_0_bit|DV_II_56_0_bit))
    mask &= ((0-(((W[40]^W[42])>>4)&1)) | ~(DV_I_44_0_bit|DV_I_46_0_bit|DV_II_56_0_bit));
  mask &= ((0-((W[39]^(W[40]>>5))&(1<<1))) | ~(DV_I_46_2_bit|DV_I_50_2_bit|DV_II_49_2_bit));
  if (mask & (DV_I_43_0_bit|DV_I_45_0_bit|DV_II_55_0_bit))
    mask &= ((0-(((W[39]^W[41])>>4)&1)) | ~(DV_I_43_0_bit|DV_I_45_0_bit|DV_II_55_0_bit));
  if (mask & (DV_I_44_0_bit|DV_II_54_0_bit|DV_II_56_0_bit))
    mask &= ((0-(((W[38]^W[40])>>4)&1)) | ~(DV_I_44_0_bit|DV_II_54_0_bit|DV_II_56_0_bit));
  if (mask & (DV_I_43_0_bit|DV_II_53_0_bit|DV_II_55_0_bit))
    mask &= ((0-(((W[37]^W[39])>>4)&1)) | ~(DV_I_43_0_bit|DV_II_53_0_bit|DV_II_55_0_bit));
  mask &= ((0-((W[36]^(W[37]>>5))&(1<<1))) | ~(DV_I_47_2_bit|DV_I_50_2_bit|DV_II_46_2_bit));
  if (mask & (DV_I_45_0_bit|DV_I_48_0_bit|DV_II_47_0_bit))
    mask &= (((((W[35]>>4)^(W[39]>>29))&1)-1) | ~(DV_I_45_0_bit|DV_I_48_0_bit|DV_II_47_0_bit));
  if (mask & (DV_I_48_0_bit|DV_II_48_0_bit))
    mask &= ((0-((W[63]^(W[64]>>5))&(1<<0))) | ~(DV_I_48_0_bit|DV_II_48_0_bit));
  if (mask & (DV_I_45_0_bit|DV_II_45_0_bit))
    mask &= ((0-((W[63]^(W[64]>>5))&(1<<1))) | ~(DV_I_45_0_bit|DV_II_45_0_bit));
  if (mask & (DV_I_47_0_bit|DV_II_47_0_bit))
    mask &= ((0-((W[62]^(W[63]>>5))&(1<<0))) | ~(DV_I_47_0_bit|DV_II_47_0_bit));
  if (mask & (DV_I_46_0_bit|DV_II_46_0_bit))
    mask &= ((0-((W[61]^(W[62]>>5))&(1<<0))) | ~(DV_I_46_0_bit|DV_II_46_0_bit));
  mask &= ((0-((W[61]^(W[62]>>5))&(1<<2))) | ~(DV_I_46_2_bit|DV_II_46_2_bit));
  if (mask & (DV_I_45_0_bit|DV_II_45_0_bit))
    mask &= ((0-((W[60]^(W[61]>>5))&(1<<0))) | ~(DV_I_45_0_bit|DV_II_45_0_bit));
  if (mask & (DV_II_51_0_bit|DV_II_54_0_bit))
    mask &= (((((W[58]^W[59])>>29)&1)-1) | ~(DV_II_51_0_bit|DV_II_54_0_bit));
  if (mask & (DV_II_50_0_bit|DV_II_53_0_bit))
    mask &= (((((W[57]^W[58])>>29)&1)-1) | ~(DV_II_50_0_bit|DV_II_53_0_bit));
  if (mask & (DV_II_52_0_bit|DV_II_54_0_bit))
    mask &= ((((W[56]^(W[59]>>25))&(1<<4))-(1<<4)) | ~(DV_II_52_0_bit|DV_II_54_0_bit));
  if (mask & (DV_II_51_0_bit|DV_II_52_0_bit))
    mask &= ((0-(((W[56]^W[59])>>29)&1)) | ~(DV_II_51_0_bit|DV_II_52_0_bit));
  if (mask & (DV_II_49_0_bit|DV_II_52_0_bit))
    mask &= (((((W[56]^W[57])>>29)&1)-1) | ~(DV_II_49_0_bit|DV_II_52_0_bit));
  if (mask & (DV_II_51_0_bit|DV_II_53_0_bit))
    mask &= ((((W[55]^(W[58]>>25))&(1<<4))-(1<<4)) | ~(DV_II_51_0_bit|DV_II_53_0_bit));
  if (mask & (DV_II_50_0_bit|DV_II_52_0_bit))
    mask &= ((((W[54]^(W[57]>>25))&(1<<4))-(1<<4)) | ~(DV_II_50_0_bit|DV_II_52_0_bit));
  if (mask & (DV_II_49_0_bit|DV_II_51_0_bit))
    mask &= ((((W[53]^(W[56]>>25))&(1<<4))-(1<<4)) | ~(DV_II_49_0_bit|DV_II_51_0_bit));
  mask &= ((((W[51]^(W[50]>>5))&(1<<1))-(1<<1)) | ~(DV_I_50_2_bit|DV_II_46_2_bit));
  mask &= ((((W[48]^W[50])&(1<<6))-(1<<6)) | ~(DV_I_50_2_bit|DV_II_46_2_bit));
  if (mask & (DV_I_51_0_bit|DV_I_52_0_bit))
    mask &= ((0-(((W[48]^W[55])>>29)&1)) | ~(DV_I_51_0_bit|DV_I_52_0_bit));
  mask &= ((((W[47]^W[49])&(1<<6))-(1<<6)) | ~(DV_I_49_2_bit|DV_I_51_2_bit));
  mask &= ((((W[48]^(W[47]>>5))&(1<<1))-(1<<1)) | ~(DV_I_47_2_bit|DV_II_51_2_bit));
  mask &= ((((W[46]^W[48])&(1<<6))-(1<<6)) | ~(DV_I_48_2_bit|DV_I_50_2_bit));
  mask &= ((((W[47]^(W[46]>>5))&(1<<1))-(1<<1)) | ~(DV_I_46_2_bit|DV_II_50_2_bit));
  mask &= ((0-((W[44]^(W[45]>>5))&(1<<1))) | ~(DV_I_51_2_bit|DV_II_49_2_bit));
  mask &= ((((W[43]^W[45])&(1<<6))-(1<<6)) | ~(DV_I_47_2_bit|DV_I_49_2_bit));
  mask &= (((((W[42]^W[44])>>6)&1)-1) | ~(DV_I_46_2_bit|DV_I_48_2_bit));
  mask &= ((((W[43]^(W[42]>>5))&(1<<1))-(1<<1)) | ~(DV_II_46_2_bit|DV_II_51_2_bit));
  mask &= ((((W[42]^(W[41]>>5))&(1<<1))-(1<<1)) | ~(DV_I_51_2_bit|DV_II_50_2_bit));
  mask &= ((((W[41]^(W[40]>>5))&(1<<1))-(1<<1)) | ~(DV_I_50_2_bit|DV_II_49_2_bit));
  if (mask & (DV_I_52_0_bit|DV_II_51_0_bit))
    mask &= ((((W[39]^(W[43]>>25))&(1<<4))-(1<<4)) | ~(DV_I_52_0_bit|DV_II_51_0_bit));
  if (mask & (DV_I_51_0_bit|DV_II_50_0_bit))
    mask &= ((((W[38]^(W[42]>>25))&(1<<4))-(1<<4)) | ~(DV_I_51_0_bit|DV_II_50_0_bit));
  if (mask & (DV_I_48_2_bit|DV_I_51_2_bit))
    mask &= ((0-((W[37]^(W[38]>>5))&(1<<1))) | ~(DV_I_48_2_bit|DV_I_51_2_bit));
  if (mask & (DV_I_50_0_bit|DV_II_49_0_bit))
    mask &= ((((W[37]^(W[41]>>25))&(1<<4))-(1<<4)) | ~(DV_I_50_0_bit|DV_II_49_0_bit));
  if (mask & (DV_II_52_0_bit|DV_II_54_0_bit))
    mask &= ((0-((W[36]^W[38])&(1<<4))) | ~(DV_II_52_0_bit|DV_II_54_0_bit));
  mask &= ((0-((W[35]^(W[36]>>5))&(1<<1))) | ~(DV_I_46_2_bit|DV_I_49_2_bit));
  if (mask & (DV_I_51_0_bit|DV_II_47_0_bit))
    mask &= ((((W[35]^(W[39]>>25))&(1<<3))-(1<<3)) | ~(DV_I_51_0_bit|DV_II_47_0_bit));
if (mask) {

  if (mask & DV_I_43_0_bit)
     if (
          !((W[61]^(W[62]>>5)) & (1<<1))
       || !(!((W[59]^(W[63]>>25)) & (1<<5)))
       || !((W[58]^(W[63]>>30)) & (1<<0))
     )  mask &= ~DV_I_43_0_bit;
  if (mask & DV_I_44_0_bit)
     if (
          !((W[62]^(W[63]>>5)) & (1<<1))
       || !(!((W[60]^(W[64]>>25)) & (1<<5)))
       || !((W[59]^(W[64]>>30)) & (1<<0))
     )  mask &= ~DV_I_44_0_bit;
  if (mask & DV_I_46_2_bit)
    mask &= ((~((W[40]^W[42])>>2)) | ~DV_I_46_2_bit);
  if (mask & DV_I_47_2_bit)
     if (
          !((W[62]^(W[63]>>5)) & (1<<2))
       || !(!((W[41]^W[43]) & (1<<6)))
     )  mask &= ~DV_I_47_2_bit;
  if (mask & DV_I_48_2_bit)
     if (
          !((W[63]^(W[64]>>5)) & (1<<2))
       || !(!((W[48]^(W[49]<<5)) & (1<<6)))
     )  mask &= ~DV_I_48_2_bit;
  if (mask & DV_I_49_2_bit)
     if (
          !(!((W[49]^(W[50]<<5)) & (1<<6)))
       || !((W[42]^W[50]) & (1<<1))
       || !(!((W[39]^(W[40]<<5)) & (1<<6)))
       || !((W[38]^W[40]) & (1<<1))
     )  mask &= ~DV_I_49_2_bit;
  if (mask & DV_I_50_0_bit)
    mask &= ((((W[36]^W[37])<<7)) | ~DV_I_50_0_bit);
  if (mask & DV_I_50_2_bit)
    mask &= ((((W[43]^W[51])<<11)) | ~DV_I_50_2_bit);
  if (mask & DV_I_51_0_bit)
    mask &= ((((W[37]^W[38])<<9)) | ~DV_I_51_0_bit);
  if (mask & DV_I_51_2_bit)
     if (
          !(!((W[51]^(W[52]<<5)) & (1<<6)))
       || !(!((W[49]^W[51]) & (1<<6)))
       || !(!((W[37]^(W[37]>>5)) & (1<<1)))
       || !(!((W[35]^(W[39]>>25)) & (1<<5)))
     )  mask &= ~DV_I_51_2_bit;
  if (mask & DV_I_52_0_bit)
    mask &= ((((W[38]^W[39])<<11)) | ~DV_I_52_0_bit);
  if (mask & DV_II_46_2_bit)
    mask &= ((((W[47]^W[51])<<17)) | ~DV_II_46_2_bit);
  if (mask & DV_II_48_0_bit)
     if (
          !(!((W[36]^(W[40]>>25)) & (1<<3)))
       || !((W[35]^(W[40]<<2)) & (1<<30))
     )  mask &= ~DV_II_48_0_bit;
  if (mask & DV_II_49_0_bit)
     if (
          !(!((W[37]^(W[41]>>25)) & (1<<3)))
       || !((W[36]^(W[41]<<2)) & (1<<30))
     )  mask &= ~DV_II_49_0_bit;
  if (mask & DV_II_49_2_bit)
     if (
          !(!((W[53]^(W[54]<<5)) & (1<<6)))
       || !(!((W[51]^W[53]) & (1<<6)))
       || !((W[50]^W[54]) & (1<<1))
       || !(!((W[45]^(W[46]<<5)) & (1<<6)))
       || !(!((W[37]^(W[41]>>25)) & (1<<5)))
       || !((W[36]^(W[41]>>30)) & (1<<0))
     )  mask &= ~DV_II_49_2_bit;
  if (mask & DV_II_50_0_bit)
     if (
          !((W[55]^W[58]) & (1<<29))
       || !(!((W[38]^(W[42]>>25)) & (1<<3)))
       || !((W[37]^(W[42]<<2)) & (1<<30))
     )  mask &= ~DV_II_50_0_bit;
  if (mask & DV_II_50_2_bit)
     if (
          !(!((W[54]^(W[55]<<5)) & (1<<6)))
       || !(!((W[52]^W[54]) & (1<<6)))
       || !((W[51]^W[55]) & (1<<1))
       || !((W[45]^W[47]) & (1<<1))
       || !(!((W[38]^(W[42]>>25)) & (1<<5)))
       || !((W[37]^(W[42]>>30)) & (1<<0))
     )  mask &= ~DV_II_50_2_bit;
  if (mask & DV_II_51_0_bit)
     if (
          !(!((W[39]^(W[43]>>25)) & (1<<3)))
       || !((W[38]^(W[43]<<2)) & (1<<30))
     )  mask &= ~DV_II_51_0_bit;
  if (mask & DV_II_51_2_bit)
     if (
          !(!((W[55]^(W[56]<<5)) & (1<<6)))
       || !(!((W[53]^W[55]) & (1<<6)))
       || !((W[52]^W[56]) & (1<<1))
       || !((W[46]^W[48]) & (1<<1))
       || !(!((W[39]^(W[43]>>25)) & (1<<5)))
       || !((W[38]^(W[43]>>30)) & (1<<0))
     )  mask &= ~DV_II_51_2_bit;
  if (mask & DV_II_52_0_bit)
     if (
          !(!((W[59]^W[60]) & (1<<29)))
       || !(!((W[40]^(W[44]>>25)) & (1<<3)))
       || !(!((W[40]^(W[44]>>25)) & (1<<4)))
       || !((W[39]^(W[44]<<2)) & (1<<30))
     )  mask &= ~DV_II_52_0_bit;
  if (mask & DV_II_53_0_bit)
     if (
          !((W[58]^W[61]) & (1<<29))
       || !(!((W[57]^(W[61]>>25)) & (1<<4)))
       || !(!((W[41]^(W[45]>>25)) & (1<<3)))
       || !(!((W[41]^(W[45]>>25)) & (1<<4)))
     )  mask &= ~DV_II_53_0_bit;
  if (mask & DV_II_54_0_bit)
     if (
          !(!((W[58]^(W[62]>>25)) & (1<<4)))
       || !(!((W[42]^(W[46]>>25)) & (1<<3)))
       || !(!((W[42]^(W[46]>>25)) & (1<<4)))
     )  mask &= ~DV_II_54_0_bit;
  if (mask & DV_II_55_0_bit)
     if (
          !(!((W[59]^(W[63]>>25)) & (1<<4)))
       || !(!((W[57]^(W[59]>>25)) & (1<<4)))
       || !(!((W[43]^(W[47]>>25)) & (1<<3)))
       || !(!((W[43]^(W[47]>>25)) & (1<<4)))
     )  mask &= ~DV_II_55_0_bit;
  if (mask & DV_II_56_0_bit)
     if (
          !(!((W[60]^(W[64]>>25)) & (1<<4)))
       || !(!((W[44]^(W[48]>>25)) & (1<<3)))
       || !(!((W[44]^(W[48]>>25)) & (1<<4)))
     )  mask &= ~DV_II_56_0_bit;
}

  dvmask[0]=mask;
}
/******************** End Of File: lib/ubc_check.c *******************/
/******************** Continue with: lib/sha1.c **********************/


#define rotate_right(x,n) (((x)>>(n))|((x)<<(32-(n))))
#define rotate_left(x,n)  (((x)<<(n))|((x)>>(32-(n))))

#define sha1_f1(b,c,d) ((d)^((b)&((c)^(d))))
#define sha1_f2(b,c,d) ((b)^(c)^(d))
#define sha1_f3(b,c,d) (((b) & ((c)|(d))) | ((c)&(d)))
#define sha1_f4(b,c,d) ((b)^(c)^(d))

#define HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, m, t) \
  { e += rotate_left(a, 5) + sha1_f1(b,c,d) + 0x5A827999 + m[t]; b = rotate_left(b, 30); }
#define HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, m, t) \
  { e += rotate_left(a, 5) + sha1_f2(b,c,d) + 0x6ED9EBA1 + m[t]; b = rotate_left(b, 30); }
#define HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, m, t) \
  { e += rotate_left(a, 5) + sha1_f3(b,c,d) + 0x8F1BBCDC + m[t]; b = rotate_left(b, 30); }
#define HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, m, t) \
  { e += rotate_left(a, 5) + sha1_f4(b,c,d) + 0xCA62C1D6 + m[t]; b = rotate_left(b, 30); }

#define HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, m, t) \
  { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f1(b,c,d) + 0x5A827999 + m[t]; }
#define HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, m, t) \
  { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f2(b,c,d) + 0x6ED9EBA1 + m[t]; }
#define HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, m, t) \
  { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f3(b,c,d) + 0x8F1BBCDC + m[t]; }
#define HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, m, t) \
  { b = rotate_right(b, 30); e -= rotate_left(a, 5) + sha1_f4(b,c,d) + 0xCA62C1D6 + m[t]; }

#define SHA1_STORE_STATE(i) states[i][0] = a; states[i][1] = b; states[i][2] = c; states[i][3] = d; states[i][4] = e;



static void  sha1_message_expansion(uint32_t W[80])
{
  unsigned i;
  for (i = 16; i < 80; ++i)
    W[i] = rotate_left(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
}

static void  sha1_compression(uint32_t ihv[5], const uint32_t m[16])
{
  uint32_t W[80];
  uint32_t a,b,c,d,e;
  unsigned i;

  memcpy(W, m, 16 * 4);
  for (i = 16; i < 80; ++i)
    W[i] = rotate_left(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);

  a = ihv[0]; b = ihv[1]; c = ihv[2]; d = ihv[3]; e = ihv[4];

  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 0);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 1);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 2);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 3);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 4);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 5);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 6);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 7);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 8);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 9);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 10);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 11);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 12);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 13);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 14);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 15);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 16);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 17);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 18);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 19);

  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 20);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 21);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 22);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 23);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 24);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 25);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 26);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 27);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 28);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 29);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 30);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 31);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 32);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 33);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 34);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 35);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 36);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 37);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 38);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 39);

  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 40);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 41);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 42);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 43);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 44);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 45);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 46);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 47);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 48);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 49);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 50);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 51);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 52);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 53);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 54);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 55);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 56);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 57);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 58);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 59);

  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 60);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 61);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 62);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 63);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 64);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 65);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 66);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 67);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 68);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 69);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 70);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 71);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 72);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 73);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 74);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 75);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 76);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 77);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 78);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 79);

  ihv[0] += a; ihv[1] += b; ihv[2] += c; ihv[3] += d; ihv[4] += e;
}



static void  sha1_compression_W(uint32_t ihv[5], const uint32_t W[80])
{
  uint32_t a = ihv[0], b = ihv[1], c = ihv[2], d = ihv[3], e = ihv[4];

  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 0);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 1);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 2);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 3);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 4);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 5);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 6);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 7);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 8);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 9);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 10);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 11);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 12);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 13);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 14);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 15);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 16);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 17);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 18);
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 19);

  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 20);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 21);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 22);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 23);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 24);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 25);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 26);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 27);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 28);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 29);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 30);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 31);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 32);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 33);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 34);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 35);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 36);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 37);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 38);
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 39);

  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 40);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 41);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 42);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 43);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 44);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 45);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 46);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 47);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 48);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 49);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 50);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 51);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 52);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 53);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 54);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 55);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 56);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 57);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 58);
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 59);

  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 60);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 61);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 62);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 63);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 64);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 65);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 66);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 67);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 68);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 69);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 70);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 71);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 72);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 73);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 74);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 75);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 76);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 77);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 78);
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 79);

  ihv[0] += a; ihv[1] += b; ihv[2] += c; ihv[3] += d; ihv[4] += e;
}



static void  sha1_compression_states(uint32_t ihv[5], const uint32_t W[80], uint32_t states[80][5])
{
  uint32_t a = ihv[0], b = ihv[1], c = ihv[2], d = ihv[3], e = ihv[4];

#ifdef DOSTORESTATE00
  SHA1_STORE_STATE(0)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 0);

#ifdef DOSTORESTATE01
  SHA1_STORE_STATE(1)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 1);

#ifdef DOSTORESTATE02
  SHA1_STORE_STATE(2)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 2);

#ifdef DOSTORESTATE03
  SHA1_STORE_STATE(3)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 3);

#ifdef DOSTORESTATE04
  SHA1_STORE_STATE(4)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 4);

#ifdef DOSTORESTATE05
  SHA1_STORE_STATE(5)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 5);

#ifdef DOSTORESTATE06
  SHA1_STORE_STATE(6)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 6);

#ifdef DOSTORESTATE07
  SHA1_STORE_STATE(7)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 7);

#ifdef DOSTORESTATE08
  SHA1_STORE_STATE(8)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 8);

#ifdef DOSTORESTATE09
  SHA1_STORE_STATE(9)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 9);

#ifdef DOSTORESTATE10
  SHA1_STORE_STATE(10)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 10);

#ifdef DOSTORESTATE11
  SHA1_STORE_STATE(11)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 11);

#ifdef DOSTORESTATE12
  SHA1_STORE_STATE(12)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 12);

#ifdef DOSTORESTATE13
  SHA1_STORE_STATE(13)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 13);

#ifdef DOSTORESTATE14
  SHA1_STORE_STATE(14)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 14);

#ifdef DOSTORESTATE15
  SHA1_STORE_STATE(15)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, W, 15);

#ifdef DOSTORESTATE16
  SHA1_STORE_STATE(16)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, W, 16);

#ifdef DOSTORESTATE17
  SHA1_STORE_STATE(17)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, W, 17);

#ifdef DOSTORESTATE18
  SHA1_STORE_STATE(18)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, W, 18);

#ifdef DOSTORESTATE19
  SHA1_STORE_STATE(19)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, W, 19);



#ifdef DOSTORESTATE20
  SHA1_STORE_STATE(20)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 20);

#ifdef DOSTORESTATE21
  SHA1_STORE_STATE(21)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 21);

#ifdef DOSTORESTATE22
  SHA1_STORE_STATE(22)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 22);

#ifdef DOSTORESTATE23
  SHA1_STORE_STATE(23)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 23);

#ifdef DOSTORESTATE24
  SHA1_STORE_STATE(24)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 24);

#ifdef DOSTORESTATE25
  SHA1_STORE_STATE(25)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 25);

#ifdef DOSTORESTATE26
  SHA1_STORE_STATE(26)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 26);

#ifdef DOSTORESTATE27
  SHA1_STORE_STATE(27)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 27);

#ifdef DOSTORESTATE28
  SHA1_STORE_STATE(28)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 28);

#ifdef DOSTORESTATE29
  SHA1_STORE_STATE(29)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 29);

#ifdef DOSTORESTATE30
  SHA1_STORE_STATE(30)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 30);

#ifdef DOSTORESTATE31
  SHA1_STORE_STATE(31)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 31);

#ifdef DOSTORESTATE32
  SHA1_STORE_STATE(32)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 32);

#ifdef DOSTORESTATE33
  SHA1_STORE_STATE(33)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 33);

#ifdef DOSTORESTATE34
  SHA1_STORE_STATE(34)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 34);

#ifdef DOSTORESTATE35
  SHA1_STORE_STATE(35)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, W, 35);

#ifdef DOSTORESTATE36
  SHA1_STORE_STATE(36)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, W, 36);

#ifdef DOSTORESTATE37
  SHA1_STORE_STATE(37)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, W, 37);

#ifdef DOSTORESTATE38
  SHA1_STORE_STATE(38)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, W, 38);

#ifdef DOSTORESTATE39
  SHA1_STORE_STATE(39)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, W, 39);



#ifdef DOSTORESTATE40
  SHA1_STORE_STATE(40)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 40);

#ifdef DOSTORESTATE41
  SHA1_STORE_STATE(41)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 41);

#ifdef DOSTORESTATE42
  SHA1_STORE_STATE(42)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 42);

#ifdef DOSTORESTATE43
  SHA1_STORE_STATE(43)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 43);

#ifdef DOSTORESTATE44
  SHA1_STORE_STATE(44)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 44);

#ifdef DOSTORESTATE45
  SHA1_STORE_STATE(45)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 45);

#ifdef DOSTORESTATE46
  SHA1_STORE_STATE(46)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 46);

#ifdef DOSTORESTATE47
  SHA1_STORE_STATE(47)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 47);

#ifdef DOSTORESTATE48
  SHA1_STORE_STATE(48)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 48);

#ifdef DOSTORESTATE49
  SHA1_STORE_STATE(49)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 49);

#ifdef DOSTORESTATE50
  SHA1_STORE_STATE(50)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 50);

#ifdef DOSTORESTATE51
  SHA1_STORE_STATE(51)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 51);

#ifdef DOSTORESTATE52
  SHA1_STORE_STATE(52)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 52);

#ifdef DOSTORESTATE53
  SHA1_STORE_STATE(53)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 53);

#ifdef DOSTORESTATE54
  SHA1_STORE_STATE(54)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 54);

#ifdef DOSTORESTATE55
  SHA1_STORE_STATE(55)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, W, 55);

#ifdef DOSTORESTATE56
  SHA1_STORE_STATE(56)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, W, 56);

#ifdef DOSTORESTATE57
  SHA1_STORE_STATE(57)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, W, 57);

#ifdef DOSTORESTATE58
  SHA1_STORE_STATE(58)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, W, 58);

#ifdef DOSTORESTATE59
  SHA1_STORE_STATE(59)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, W, 59);




#ifdef DOSTORESTATE60
  SHA1_STORE_STATE(60)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 60);

#ifdef DOSTORESTATE61
  SHA1_STORE_STATE(61)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 61);

#ifdef DOSTORESTATE62
  SHA1_STORE_STATE(62)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 62);

#ifdef DOSTORESTATE63
  SHA1_STORE_STATE(63)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 63);

#ifdef DOSTORESTATE64
  SHA1_STORE_STATE(64)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 64);

#ifdef DOSTORESTATE65
  SHA1_STORE_STATE(65)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 65);

#ifdef DOSTORESTATE66
  SHA1_STORE_STATE(66)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 66);

#ifdef DOSTORESTATE67
  SHA1_STORE_STATE(67)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 67);

#ifdef DOSTORESTATE68
  SHA1_STORE_STATE(68)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 68);

#ifdef DOSTORESTATE69
  SHA1_STORE_STATE(69)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 69);

#ifdef DOSTORESTATE70
  SHA1_STORE_STATE(70)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 70);

#ifdef DOSTORESTATE71
  SHA1_STORE_STATE(71)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 71);

#ifdef DOSTORESTATE72
  SHA1_STORE_STATE(72)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 72);

#ifdef DOSTORESTATE73
  SHA1_STORE_STATE(73)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 73);

#ifdef DOSTORESTATE74
  SHA1_STORE_STATE(74)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 74);

#ifdef DOSTORESTATE75
  SHA1_STORE_STATE(75)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, W, 75);

#ifdef DOSTORESTATE76
  SHA1_STORE_STATE(76)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, W, 76);

#ifdef DOSTORESTATE77
  SHA1_STORE_STATE(77)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, W, 77);

#ifdef DOSTORESTATE78
  SHA1_STORE_STATE(78)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, W, 78);

#ifdef DOSTORESTATE79
  SHA1_STORE_STATE(79)
#endif
  HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, W, 79);



  ihv[0] += a; ihv[1] += b; ihv[2] += c; ihv[3] += d; ihv[4] += e;
}




#define SHA1_RECOMPRESS(t) \
static void  sha1recompress_fast_ ## t (uint32_t ihvin[5], uint32_t ihvout[5], const uint32_t me2[80], const uint32_t state[5]) \
{ \
  uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4]; \
  if (t > 79) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 79); \
  if (t > 78) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 78); \
  if (t > 77) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 77); \
  if (t > 76) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 76); \
  if (t > 75) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 75); \
  if (t > 74) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 74); \
  if (t > 73) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 73); \
  if (t > 72) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 72); \
  if (t > 71) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 71); \
  if (t > 70) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 70); \
  if (t > 69) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 69); \
  if (t > 68) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 68); \
  if (t > 67) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 67); \
  if (t > 66) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 66); \
  if (t > 65) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 65); \
  if (t > 64) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(b, c, d, e, a, me2, 64); \
  if (t > 63) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(c, d, e, a, b, me2, 63); \
  if (t > 62) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(d, e, a, b, c, me2, 62); \
  if (t > 61) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(e, a, b, c, d, me2, 61); \
  if (t > 60) HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW(a, b, c, d, e, me2, 60); \
  if (t > 59) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 59); \
  if (t > 58) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 58); \
  if (t > 57) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 57); \
  if (t > 56) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 56); \
  if (t > 55) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 55); \
  if (t > 54) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 54); \
  if (t > 53) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 53); \
  if (t > 52) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 52); \
  if (t > 51) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 51); \
  if (t > 50) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 50); \
  if (t > 49) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 49); \
  if (t > 48) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 48); \
  if (t > 47) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 47); \
  if (t > 46) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 46); \
  if (t > 45) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 45); \
  if (t > 44) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(b, c, d, e, a, me2, 44); \
  if (t > 43) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(c, d, e, a, b, me2, 43); \
  if (t > 42) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(d, e, a, b, c, me2, 42); \
  if (t > 41) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(e, a, b, c, d, me2, 41); \
  if (t > 40) HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW(a, b, c, d, e, me2, 40); \
  if (t > 39) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 39); \
  if (t > 38) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 38); \
  if (t > 37) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 37); \
  if (t > 36) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 36); \
  if (t > 35) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 35); \
  if (t > 34) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 34); \
  if (t > 33) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 33); \
  if (t > 32) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 32); \
  if (t > 31) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 31); \
  if (t > 30) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 30); \
  if (t > 29) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 29); \
  if (t > 28) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 28); \
  if (t > 27) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 27); \
  if (t > 26) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 26); \
  if (t > 25) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 25); \
  if (t > 24) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(b, c, d, e, a, me2, 24); \
  if (t > 23) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(c, d, e, a, b, me2, 23); \
  if (t > 22) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(d, e, a, b, c, me2, 22); \
  if (t > 21) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(e, a, b, c, d, me2, 21); \
  if (t > 20) HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW(a, b, c, d, e, me2, 20); \
  if (t > 19) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 19); \
  if (t > 18) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 18); \
  if (t > 17) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 17); \
  if (t > 16) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 16); \
  if (t > 15) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 15); \
  if (t > 14) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 14); \
  if (t > 13) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 13); \
  if (t > 12) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 12); \
  if (t > 11) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 11); \
  if (t > 10) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 10); \
  if (t > 9) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 9); \
  if (t > 8) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 8); \
  if (t > 7) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 7); \
  if (t > 6) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 6); \
  if (t > 5) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 5); \
  if (t > 4) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(b, c, d, e, a, me2, 4); \
  if (t > 3) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(c, d, e, a, b, me2, 3); \
  if (t > 2) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(d, e, a, b, c, me2, 2); \
  if (t > 1) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(e, a, b, c, d, me2, 1); \
  if (t > 0) HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW(a, b, c, d, e, me2, 0); \
  ihvin[0] = a; ihvin[1] = b; ihvin[2] = c; ihvin[3] = d; ihvin[4] = e; \
  a = state[0]; b = state[1]; c = state[2]; d = state[3]; e = state[4]; \
  if (t <= 0) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 0); \
  if (t <= 1) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 1); \
  if (t <= 2) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 2); \
  if (t <= 3) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 3); \
  if (t <= 4) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 4); \
  if (t <= 5) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 5); \
  if (t <= 6) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 6); \
  if (t <= 7) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 7); \
  if (t <= 8) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 8); \
  if (t <= 9) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 9); \
  if (t <= 10) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 10); \
  if (t <= 11) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 11); \
  if (t <= 12) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 12); \
  if (t <= 13) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 13); \
  if (t <= 14) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 14); \
  if (t <= 15) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(a, b, c, d, e, me2, 15); \
  if (t <= 16) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(e, a, b, c, d, me2, 16); \
  if (t <= 17) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(d, e, a, b, c, me2, 17); \
  if (t <= 18) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(c, d, e, a, b, me2, 18); \
  if (t <= 19) HASHCLASH_SHA1COMPRESS_ROUND1_STEP(b, c, d, e, a, me2, 19); \
  if (t <= 20) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 20); \
  if (t <= 21) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 21); \
  if (t <= 22) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 22); \
  if (t <= 23) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 23); \
  if (t <= 24) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 24); \
  if (t <= 25) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 25); \
  if (t <= 26) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 26); \
  if (t <= 27) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 27); \
  if (t <= 28) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 28); \
  if (t <= 29) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 29); \
  if (t <= 30) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 30); \
  if (t <= 31) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 31); \
  if (t <= 32) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 32); \
  if (t <= 33) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 33); \
  if (t <= 34) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 34); \
  if (t <= 35) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(a, b, c, d, e, me2, 35); \
  if (t <= 36) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(e, a, b, c, d, me2, 36); \
  if (t <= 37) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(d, e, a, b, c, me2, 37); \
  if (t <= 38) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(c, d, e, a, b, me2, 38); \
  if (t <= 39) HASHCLASH_SHA1COMPRESS_ROUND2_STEP(b, c, d, e, a, me2, 39); \
  if (t <= 40) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 40); \
  if (t <= 41) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 41); \
  if (t <= 42) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 42); \
  if (t <= 43) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 43); \
  if (t <= 44) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 44); \
  if (t <= 45) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 45); \
  if (t <= 46) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 46); \
  if (t <= 47) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 47); \
  if (t <= 48) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 48); \
  if (t <= 49) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 49); \
  if (t <= 50) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 50); \
  if (t <= 51) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 51); \
  if (t <= 52) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 52); \
  if (t <= 53) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 53); \
  if (t <= 54) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 54); \
  if (t <= 55) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(a, b, c, d, e, me2, 55); \
  if (t <= 56) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(e, a, b, c, d, me2, 56); \
  if (t <= 57) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(d, e, a, b, c, me2, 57); \
  if (t <= 58) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(c, d, e, a, b, me2, 58); \
  if (t <= 59) HASHCLASH_SHA1COMPRESS_ROUND3_STEP(b, c, d, e, a, me2, 59); \
  if (t <= 60) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 60); \
  if (t <= 61) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 61); \
  if (t <= 62) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 62); \
  if (t <= 63) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 63); \
  if (t <= 64) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 64); \
  if (t <= 65) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 65); \
  if (t <= 66) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 66); \
  if (t <= 67) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 67); \
  if (t <= 68) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 68); \
  if (t <= 69) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 69); \
  if (t <= 70) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 70); \
  if (t <= 71) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 71); \
  if (t <= 72) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 72); \
  if (t <= 73) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 73); \
  if (t <= 74) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 74); \
  if (t <= 75) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(a, b, c, d, e, me2, 75); \
  if (t <= 76) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(e, a, b, c, d, me2, 76); \
  if (t <= 77) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(d, e, a, b, c, me2, 77); \
  if (t <= 78) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(c, d, e, a, b, me2, 78); \
  if (t <= 79) HASHCLASH_SHA1COMPRESS_ROUND4_STEP(b, c, d, e, a, me2, 79); \
  ihvout[0] = ihvin[0] + a; ihvout[1] = ihvin[1] + b; ihvout[2] = ihvin[2] + c; ihvout[3] = ihvin[3] + d; ihvout[4] = ihvin[4] + e; \
}

SHA1_RECOMPRESS(0)
SHA1_RECOMPRESS(1)
SHA1_RECOMPRESS(2)
SHA1_RECOMPRESS(3)
SHA1_RECOMPRESS(4)
SHA1_RECOMPRESS(5)
SHA1_RECOMPRESS(6)
SHA1_RECOMPRESS(7)
SHA1_RECOMPRESS(8)
SHA1_RECOMPRESS(9)

SHA1_RECOMPRESS(10)
SHA1_RECOMPRESS(11)
SHA1_RECOMPRESS(12)
SHA1_RECOMPRESS(13)
SHA1_RECOMPRESS(14)
SHA1_RECOMPRESS(15)
SHA1_RECOMPRESS(16)
SHA1_RECOMPRESS(17)
SHA1_RECOMPRESS(18)
SHA1_RECOMPRESS(19)

SHA1_RECOMPRESS(20)
SHA1_RECOMPRESS(21)
SHA1_RECOMPRESS(22)
SHA1_RECOMPRESS(23)
SHA1_RECOMPRESS(24)
SHA1_RECOMPRESS(25)
SHA1_RECOMPRESS(26)
SHA1_RECOMPRESS(27)
SHA1_RECOMPRESS(28)
SHA1_RECOMPRESS(29)

SHA1_RECOMPRESS(30)
SHA1_RECOMPRESS(31)
SHA1_RECOMPRESS(32)
SHA1_RECOMPRESS(33)
SHA1_RECOMPRESS(34)
SHA1_RECOMPRESS(35)
SHA1_RECOMPRESS(36)
SHA1_RECOMPRESS(37)
SHA1_RECOMPRESS(38)
SHA1_RECOMPRESS(39)

SHA1_RECOMPRESS(40)
SHA1_RECOMPRESS(41)
SHA1_RECOMPRESS(42)
SHA1_RECOMPRESS(43)
SHA1_RECOMPRESS(44)
SHA1_RECOMPRESS(45)
SHA1_RECOMPRESS(46)
SHA1_RECOMPRESS(47)
SHA1_RECOMPRESS(48)
SHA1_RECOMPRESS(49)

SHA1_RECOMPRESS(50)
SHA1_RECOMPRESS(51)
SHA1_RECOMPRESS(52)
SHA1_RECOMPRESS(53)
SHA1_RECOMPRESS(54)
SHA1_RECOMPRESS(55)
SHA1_RECOMPRESS(56)
SHA1_RECOMPRESS(57)
SHA1_RECOMPRESS(58)
SHA1_RECOMPRESS(59)

SHA1_RECOMPRESS(60)
SHA1_RECOMPRESS(61)
SHA1_RECOMPRESS(62)
SHA1_RECOMPRESS(63)
SHA1_RECOMPRESS(64)
SHA1_RECOMPRESS(65)
SHA1_RECOMPRESS(66)
SHA1_RECOMPRESS(67)
SHA1_RECOMPRESS(68)
SHA1_RECOMPRESS(69)

SHA1_RECOMPRESS(70)
SHA1_RECOMPRESS(71)
SHA1_RECOMPRESS(72)
SHA1_RECOMPRESS(73)
SHA1_RECOMPRESS(74)
SHA1_RECOMPRESS(75)
SHA1_RECOMPRESS(76)
SHA1_RECOMPRESS(77)
SHA1_RECOMPRESS(78)
SHA1_RECOMPRESS(79)

static sha1_recompression_type sha1_recompression_step[80] =
{
  sha1recompress_fast_0, sha1recompress_fast_1, sha1recompress_fast_2, sha1recompress_fast_3, sha1recompress_fast_4, sha1recompress_fast_5, sha1recompress_fast_6, sha1recompress_fast_7, sha1recompress_fast_8, sha1recompress_fast_9,
  sha1recompress_fast_10, sha1recompress_fast_11, sha1recompress_fast_12, sha1recompress_fast_13, sha1recompress_fast_14, sha1recompress_fast_15, sha1recompress_fast_16, sha1recompress_fast_17, sha1recompress_fast_18, sha1recompress_fast_19,
  sha1recompress_fast_20, sha1recompress_fast_21, sha1recompress_fast_22, sha1recompress_fast_23, sha1recompress_fast_24, sha1recompress_fast_25, sha1recompress_fast_26, sha1recompress_fast_27, sha1recompress_fast_28, sha1recompress_fast_29,
  sha1recompress_fast_30, sha1recompress_fast_31, sha1recompress_fast_32, sha1recompress_fast_33, sha1recompress_fast_34, sha1recompress_fast_35, sha1recompress_fast_36, sha1recompress_fast_37, sha1recompress_fast_38, sha1recompress_fast_39,
  sha1recompress_fast_40, sha1recompress_fast_41, sha1recompress_fast_42, sha1recompress_fast_43, sha1recompress_fast_44, sha1recompress_fast_45, sha1recompress_fast_46, sha1recompress_fast_47, sha1recompress_fast_48, sha1recompress_fast_49,
  sha1recompress_fast_50, sha1recompress_fast_51, sha1recompress_fast_52, sha1recompress_fast_53, sha1recompress_fast_54, sha1recompress_fast_55, sha1recompress_fast_56, sha1recompress_fast_57, sha1recompress_fast_58, sha1recompress_fast_59,
  sha1recompress_fast_60, sha1recompress_fast_61, sha1recompress_fast_62, sha1recompress_fast_63, sha1recompress_fast_64, sha1recompress_fast_65, sha1recompress_fast_66, sha1recompress_fast_67, sha1recompress_fast_68, sha1recompress_fast_69,
  sha1recompress_fast_70, sha1recompress_fast_71, sha1recompress_fast_72, sha1recompress_fast_73, sha1recompress_fast_74, sha1recompress_fast_75, sha1recompress_fast_76, sha1recompress_fast_77, sha1recompress_fast_78, sha1recompress_fast_79,
};





static void  sha1_process(SHA1_CTX* ctx, const uint32_t block[16])
{
  unsigned i, j;
  uint32_t ubc_dv_mask[DVMASKSIZE];
  uint32_t ihvtmp[5];
  for (i=0; i < DVMASKSIZE; ++i)
    ubc_dv_mask[i]=0;
  ctx->ihv1[0] = ctx->ihv[0];
  ctx->ihv1[1] = ctx->ihv[1];
  ctx->ihv1[2] = ctx->ihv[2];
  ctx->ihv1[3] = ctx->ihv[3];
  ctx->ihv1[4] = ctx->ihv[4];
  memcpy(ctx->m1, block, 64);
  sha1_message_expansion(ctx->m1);
  if (ctx->detect_coll && ctx->ubc_check)
  {
    ubc_check(ctx->m1, ubc_dv_mask);
  }
  sha1_compression_states(ctx->ihv, ctx->m1, ctx->states);
  if (ctx->detect_coll)
  {
    for (i = 0; sha1_dvs[i].dvType != 0; ++i)
    {
      if ((0 == ctx->ubc_check) || (((uint32_t)(1) << sha1_dvs[i].maskb) & ubc_dv_mask[sha1_dvs[i].maski]))
      {
        for (j = 0; j < 80; ++j)
          ctx->m2[j] = ctx->m1[j] ^ sha1_dvs[i].dm[j];
        (sha1_recompression_step[sha1_dvs[i].testt])(ctx->ihv2, ihvtmp, ctx->m2, ctx->states[sha1_dvs[i].testt]);
        /* to verify SHA-1 collision detection code with collisions for reduced-step SHA-1 */
        if ((ihvtmp[0] == ctx->ihv[0] && ihvtmp[1] == ctx->ihv[1] && ihvtmp[2] == ctx->ihv[2] && ihvtmp[3] == ctx->ihv[3] && ihvtmp[4] == ctx->ihv[4])
          || (ctx->reduced_round_coll && ctx->ihv1[0] == ctx->ihv2[0] && ctx->ihv1[1] == ctx->ihv2[1] && ctx->ihv1[2] == ctx->ihv2[2] && ctx->ihv1[3] == ctx->ihv2[3] && ctx->ihv1[4] == ctx->ihv2[4]))
        {
          ctx->found_collision = 1;
          /* TODO: call callback */
          if (ctx->callback != NULL)
            ctx->callback(ctx->total - 64, ctx->ihv1, ctx->ihv2, ctx->m1, ctx->m2);

          if (ctx->safe_hash)
          {
            sha1_compression_W(ctx->ihv, ctx->m1);
            sha1_compression_W(ctx->ihv, ctx->m1);
          }

          break;
        }
      }
    }
  }
}





static void  swap_bytes(uint32_t val[16])
{
  unsigned i;
  for (i = 0; i < 16; ++i)
  {
    val[i] = ((val[i] << 8) & 0xFF00FF00) | ((val[i] >> 8) & 0xFF00FF);
    val[i] = (val[i] << 16) | (val[i] >> 16);
  }
}

static void *SHA1DCInit()
{
  static const union { unsigned char bytes[4]; uint32_t value; } endianness = { { 0, 1, 2, 3 } };
  static const uint32_t littleendian = 0x03020100;
  SHA1_CTX * ctx;
  ctx = sqlite3_malloc(sizeof(SHA1_CTX));
  if (!ctx)
    return NULL;
  ctx->total = 0;
  ctx->ihv[0] = 0x67452301;
  ctx->ihv[1] = 0xEFCDAB89;
  ctx->ihv[2] = 0x98BADCFE;
  ctx->ihv[3] = 0x10325476;
  ctx->ihv[4] = 0xC3D2E1F0;
  ctx->found_collision = 0;
  ctx->safe_hash = 1;
  ctx->ubc_check = 1;
  ctx->detect_coll = 1;
  ctx->reduced_round_coll = 0;
  ctx->bigendian = (endianness.value != littleendian);
  ctx->callback = NULL;
  return ctx;
}

static void SHA1DCSetSafeHash(SHA1_CTX* ctx, int safehash)
{
  if (safehash)
    ctx->safe_hash = 1;
  else
    ctx->safe_hash = 0;
}


static void SHA1DCSetUseUBC(SHA1_CTX* ctx, int ubc_check)
{
  if (ubc_check)
    ctx->ubc_check = 1;
  else
    ctx->ubc_check = 0;
}

static void SHA1DCSetUseDetectColl(SHA1_CTX* ctx, int detect_coll)
{
  if (detect_coll)
    ctx->detect_coll = 1;
  else
    ctx->detect_coll = 0;
}

static void SHA1DCSetDetectReducedRoundCollision(SHA1_CTX* ctx, int reduced_round_coll)
{
  if (reduced_round_coll)
    ctx->reduced_round_coll = 1;
  else
    ctx->reduced_round_coll = 0;
}

static void SHA1DCSetCallback(SHA1_CTX* ctx, collision_block_callback callback)
{
  ctx->callback = callback;
}

static void SHA1DCUpdate(SHA1_CTX* ctx, const unsigned char* buf, unsigned long len)
{
  unsigned left, fill;
  if (len == 0)
    return;

  left = ctx->total & 63;
  fill = 64 - left;

  if (left && len >= fill)
  {
    ctx->total += fill;
    memcpy(ctx->buffer + left, buf, fill);
    if (!ctx->bigendian)
      swap_bytes((uint32_t*)(ctx->buffer));
    sha1_process(ctx, (uint32_t*)(ctx->buffer));
    buf += fill;
    len -= fill;
    left = 0;
  }
  while (len >= 64)
  {
    ctx->total += 64;
    if (!ctx->bigendian)
    {
      memcpy(ctx->buffer, buf, 64);
      swap_bytes((uint32_t*)(ctx->buffer));
      sha1_process(ctx, (uint32_t*)(ctx->buffer));
    }
    else
      sha1_process(ctx, (uint32_t*)(buf));
    buf += 64;
    len -= 64;
  }
  if (len > 0)
  {
    ctx->total += len;
    memcpy(ctx->buffer + left, buf, len);
  }
}

static const unsigned char sha1_padding[64] =
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int SHA1DCFinal(unsigned char output[20], SHA1_CTX *ctx)
{
  uint32_t last = ctx->total & 63;
  uint32_t padn = (last < 56) ? (56 - last) : (120 - last);
  uint64_t total;
  SHA1DCUpdate(ctx, sha1_padding, padn);

  total = ctx->total - padn;
  total <<= 3;
  ctx->buffer[56] = (unsigned char)(total >> 56);
  ctx->buffer[57] = (unsigned char)(total >> 48);
  ctx->buffer[58] = (unsigned char)(total >> 40);
  ctx->buffer[59] = (unsigned char)(total >> 32);
  ctx->buffer[60] = (unsigned char)(total >> 24);
  ctx->buffer[61] = (unsigned char)(total >> 16);
  ctx->buffer[62] = (unsigned char)(total >> 8);
  ctx->buffer[63] = (unsigned char)(total);
  if (!ctx->bigendian)
    swap_bytes((uint32_t*)(ctx->buffer));
  sha1_process(ctx, (uint32_t*)(ctx->buffer));
  output[0] = (unsigned char)(ctx->ihv[0] >> 24);
  output[1] = (unsigned char)(ctx->ihv[0] >> 16);
  output[2] = (unsigned char)(ctx->ihv[0] >> 8);
  output[3] = (unsigned char)(ctx->ihv[0]);
  output[4] = (unsigned char)(ctx->ihv[1] >> 24);
  output[5] = (unsigned char)(ctx->ihv[1] >> 16);
  output[6] = (unsigned char)(ctx->ihv[1] >> 8);
  output[7] = (unsigned char)(ctx->ihv[1]);
  output[8] = (unsigned char)(ctx->ihv[2] >> 24);
  output[9] = (unsigned char)(ctx->ihv[2] >> 16);
  output[10] = (unsigned char)(ctx->ihv[2] >> 8);
  output[11] = (unsigned char)(ctx->ihv[2]);
  output[12] = (unsigned char)(ctx->ihv[3] >> 24);
  output[13] = (unsigned char)(ctx->ihv[3] >> 16);
  output[14] = (unsigned char)(ctx->ihv[3] >> 8);
  output[15] = (unsigned char)(ctx->ihv[3]);
  output[16] = (unsigned char)(ctx->ihv[4] >> 24);
  output[17] = (unsigned char)(ctx->ihv[4] >> 16);
  output[18] = (unsigned char)(ctx->ihv[4] >> 8);
  output[19] = (unsigned char)(ctx->ihv[4]);
  sqlite3_free(ctx);
  return 20;
}


/* End of include sha1hard.c */

/* Insert sha2.c/h */

/*
 * FILE:    sha2.c
 * AUTHOR:  Aaron D. Gifford - http://www.aarongifford.com/
 *
 * Copyright (c) 2000-2001, Aaron D. Gifford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: sha2.c,v 1.1 2001/11/08 00:01:51 adg Exp adg $
 */

#include <string.h> /* memcpy()/memset() or bcopy()/bzero() */
#include <assert.h> /* assert() */

/*
 * FILE:    sha2.h
 * AUTHOR:  Aaron D. Gifford - http://www.aarongifford.com/
 *
 * Copyright (c) 2000-2001, Aaron D. Gifford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: sha2.h,v 1.1 2001/11/08 00:02:01 adg Exp adg $
 */

#ifndef __SHA2_H__
#define __SHA2_H__

#ifdef __cplusplus
extern "C" {
#endif


#define SHA2_USE_INTTYPES_H
#define SHA2_UNROLL_TRANSFORM
#define NOPROTO

/*
 * Import u_intXX_t size_t type definitions from system headers.  You
 * may need to change this, or define these things yourself in this
 * file.
 */
#include <sys/types.h>

#ifdef SHA2_USE_INTTYPES_H

#include <inttypes.h>

#endif /* SHA2_USE_INTTYPES_H */


/*** SHA-256/384/512 Various Length Definitions ***********************/
#define SHA256_BLOCK_LENGTH     64
#define SHA256_DIGEST_LENGTH        32
#define SHA256_DIGEST_STRING_LENGTH (SHA256_DIGEST_LENGTH * 2 + 1)
#define SHA384_BLOCK_LENGTH     128
#define SHA384_DIGEST_LENGTH        48
#define SHA384_DIGEST_STRING_LENGTH (SHA384_DIGEST_LENGTH * 2 + 1)
#define SHA512_BLOCK_LENGTH     128
#define SHA512_DIGEST_LENGTH        64
#define SHA512_DIGEST_STRING_LENGTH (SHA512_DIGEST_LENGTH * 2 + 1)


/*** SHA-256/384/512 Context Structures *******************************/
/* NOTE: If your architecture does not define either u_intXX_t types or
 * uintXX_t (from inttypes.h), you may need to define things by hand
 * for your system:
 */
#if 0
typedef unsigned char u_int8_t;     /* 1-byte  (8-bits)  */
typedef unsigned int u_int32_t;     /* 4-bytes (32-bits) */
typedef unsigned long long u_int64_t;   /* 8-bytes (64-bits) */
#endif
/*
 * Most BSD systems already define u_intXX_t types, as does Linux.
 * Some systems, however, like Compaq's Tru64 Unix instead can use
 * uintXX_t types defined by very recent ANSI C standards and included
 * in the file:
 *
 *   #include <inttypes.h>
 *
 * If you choose to use <inttypes.h> then please define:
 *
 *   #define SHA2_USE_INTTYPES_H
 *
 * Or on the command line during compile:
 *
 *   cc -DSHA2_USE_INTTYPES_H ...
 */
#ifdef SHA2_USE_INTTYPES_H

typedef struct _SHA256_CTX {
    uint32_t    state[8];
    uint64_t    bitcount;
    uint8_t buffer[SHA256_BLOCK_LENGTH];
} SHA256_CTX;
typedef struct _SHA512_CTX {
    uint64_t    state[8];
    uint64_t    bitcount[2];
    uint8_t buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;

#else /* SHA2_USE_INTTYPES_H */

typedef struct _SHA256_CTX {
    u_int32_t   state[8];
    u_int64_t   bitcount;
    u_int8_t    buffer[SHA256_BLOCK_LENGTH];
} SHA256_CTX;
typedef struct _SHA512_CTX {
    u_int64_t   state[8];
    u_int64_t   bitcount[2];
    u_int8_t    buffer[SHA512_BLOCK_LENGTH];
} SHA512_CTX;

#endif /* SHA2_USE_INTTYPES_H */

typedef SHA512_CTX SHA384_CTX;


/*** SHA-256/384/512 Function Prototypes ******************************/
#ifndef NOPROTO
#ifdef SHA2_USE_INTTYPES_H

void SHA256_Init(SHA256_CTX *);
void SHA256_Update(SHA256_CTX*, const uint8_t*, size_t);
void SHA256_Final(uint8_t[SHA256_DIGEST_LENGTH], SHA256_CTX*);
char* SHA256_End(SHA256_CTX*, char[SHA256_DIGEST_STRING_LENGTH]);
char* SHA256_Data(const uint8_t*, size_t, char[SHA256_DIGEST_STRING_LENGTH]);

void SHA384_Init(SHA384_CTX*);
void SHA384_Update(SHA384_CTX*, const uint8_t*, size_t);
void SHA384_Final(uint8_t[SHA384_DIGEST_LENGTH], SHA384_CTX*);
char* SHA384_End(SHA384_CTX*, char[SHA384_DIGEST_STRING_LENGTH]);
char* SHA384_Data(const uint8_t*, size_t, char[SHA384_DIGEST_STRING_LENGTH]);

void SHA512_Init(SHA512_CTX*);
void SHA512_Update(SHA512_CTX*, const uint8_t*, size_t);
void SHA512_Final(uint8_t[SHA512_DIGEST_LENGTH], SHA512_CTX*);
char* SHA512_End(SHA512_CTX*, char[SHA512_DIGEST_STRING_LENGTH]);
char* SHA512_Data(const uint8_t*, size_t, char[SHA512_DIGEST_STRING_LENGTH]);

#else /* SHA2_USE_INTTYPES_H */

void SHA256_Init(SHA256_CTX *);
void SHA256_Update(SHA256_CTX*, const u_int8_t*, size_t);
void SHA256_Final(u_int8_t[SHA256_DIGEST_LENGTH], SHA256_CTX*);
char* SHA256_End(SHA256_CTX*, char[SHA256_DIGEST_STRING_LENGTH]);
char* SHA256_Data(const u_int8_t*, size_t, char[SHA256_DIGEST_STRING_LENGTH]);

void SHA384_Init(SHA384_CTX*);
void SHA384_Update(SHA384_CTX*, const u_int8_t*, size_t);
void SHA384_Final(u_int8_t[SHA384_DIGEST_LENGTH], SHA384_CTX*);
char* SHA384_End(SHA384_CTX*, char[SHA384_DIGEST_STRING_LENGTH]);
char* SHA384_Data(const u_int8_t*, size_t, char[SHA384_DIGEST_STRING_LENGTH]);

void SHA512_Init(SHA512_CTX*);
void SHA512_Update(SHA512_CTX*, const u_int8_t*, size_t);
void SHA512_Final(u_int8_t[SHA512_DIGEST_LENGTH], SHA512_CTX*);
char* SHA512_End(SHA512_CTX*, char[SHA512_DIGEST_STRING_LENGTH]);
char* SHA512_Data(const u_int8_t*, size_t, char[SHA512_DIGEST_STRING_LENGTH]);

#endif /* SHA2_USE_INTTYPES_H */

#else /* NOPROTO */

//void SHA256_Init();
//void SHA256_Update();
//void SHA256_Final();
//char* SHA256_End();
//char* SHA256_Data();

//void SHA384_Init();
//void SHA384_Update();
//void SHA384_Final();
//char* SHA384_End();
//char* SHA384_Data();

//void SHA512_Init();
//void SHA512_Update();
//void SHA512_Final();
//char* SHA512_End();
//char* SHA512_Data();

#endif /* NOPROTO */

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* __SHA2_H__ */



/*
 * ASSERT NOTE:
 * Some sanity checking code is included using assert().  On my FreeBSD
 * system, this additional code can be removed by compiling with NDEBUG
 * defined.  Check your own systems manpage on assert() to see how to
 * compile WITHOUT the sanity checking code on your system.
 *
 * UNROLLED TRANSFORM LOOP NOTE:
 * You can define SHA2_UNROLL_TRANSFORM to use the unrolled transform
 * loop version for the hash transform rounds (defined using macros
 * later in this file).  Either define on the command line, for example:
 *
 *   cc -DSHA2_UNROLL_TRANSFORM -o sha2 sha2.c sha2prog.c
 *
 * or define below:
 *
 *   #define SHA2_UNROLL_TRANSFORM
 *
 */


/*** SHA-256/384/512 Machine Architecture Definitions *****************/
/*
 * BYTE_ORDER NOTE:
 *
 * Please make sure that your system defines BYTE_ORDER.  If your
 * architecture is little-endian, make sure it also defines
 * LITTLE_ENDIAN and that the two (BYTE_ORDER and LITTLE_ENDIAN) are
 * equivilent.
 *
 * If your system does not define the above, then you can do so by
 * hand like this:
 *
 *   #define LITTLE_ENDIAN 1234
 *   #define BIG_ENDIAN    4321
 *
 * And for little-endian machines, add:
 *
 *   #define BYTE_ORDER LITTLE_ENDIAN
 *
 * Or for big-endian machines:
 *
 *   #define BYTE_ORDER BIG_ENDIAN
 *
 * The FreeBSD machine this was written on defines BYTE_ORDER
 * appropriately by including <sys/types.h> (which in turn includes
 * <machine/endian.h> where the appropriate definitions are actually
 * made).
 */

#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN 4321
#define BYTE_ORDER SHA3_BYTEORDER


#if !defined(BYTE_ORDER) || (BYTE_ORDER != LITTLE_ENDIAN && BYTE_ORDER != BIG_ENDIAN)
#error Define BYTE_ORDER to be equal to either LITTLE_ENDIAN or BIG_ENDIAN
#endif

/*
 * Define the followingsha2_* types to types of the correct length on
 * the native archtecture.   Most BSD systems and Linux define u_intXX_t
 * types.  Machines with very recent ANSI C headers, can use the
 * uintXX_t definintions from inttypes.h by defining SHA2_USE_INTTYPES_H
 * during compile or in the sha.h header file.
 *
 * Machines that support neither u_intXX_t nor inttypes.h's uintXX_t
 * will need to define these three typedefs below (and the appropriate
 * ones in sha.h too) by hand according to their system architecture.
 *
 * Thank you, Jun-ichiro itojun Hagino, for suggesting using u_intXX_t
 * types and pointing out recent ANSI C support for uintXX_t in inttypes.h.
 */
#ifdef SHA2_USE_INTTYPES_H

typedef uint8_t  sha2_byte; /* Exactly 1 byte */
typedef uint32_t sha2_word32;   /* Exactly 4 bytes */
typedef uint64_t sha2_word64;   /* Exactly 8 bytes */

#else /* SHA2_USE_INTTYPES_H */

typedef u_int8_t  sha2_byte;    /* Exactly 1 byte */
typedef u_int32_t sha2_word32;  /* Exactly 4 bytes */
typedef u_int64_t sha2_word64;  /* Exactly 8 bytes */

#endif /* SHA2_USE_INTTYPES_H */


/*** SHA-256/384/512 Various Length Definitions ***********************/
/* NOTE: Most of these are in sha2.h */
#define SHA256_SHORT_BLOCK_LENGTH   (SHA256_BLOCK_LENGTH - 8)
#define SHA384_SHORT_BLOCK_LENGTH   (SHA384_BLOCK_LENGTH - 16)
#define SHA512_SHORT_BLOCK_LENGTH   (SHA512_BLOCK_LENGTH - 16)


/*** ENDIAN REVERSAL MACROS *******************************************/
#if BYTE_ORDER == LITTLE_ENDIAN
#define REVERSE32(w,x)  { \
    sha2_word32 tmp = (w); \
    tmp = (tmp >> 16) | (tmp << 16); \
    (x) = ((tmp & 0xff00ff00UL) >> 8) | ((tmp & 0x00ff00ffUL) << 8); \
}
#define REVERSE64(w,x)  { \
    sha2_word64 tmp = (w); \
    tmp = (tmp >> 32) | (tmp << 32); \
    tmp = ((tmp & 0xff00ff00ff00ff00ULL) >> 8) | \
          ((tmp & 0x00ff00ff00ff00ffULL) << 8); \
    (x) = ((tmp & 0xffff0000ffff0000ULL) >> 16) | \
          ((tmp & 0x0000ffff0000ffffULL) << 16); \
}
#endif /* BYTE_ORDER == LITTLE_ENDIAN */

/*
 * Macro for incrementally adding the unsigned 64-bit integer n to the
 * unsigned 128-bit integer (represented using a two-element array of
 * 64-bit words):
 */
#define ADDINC128(w,n)  { \
    (w)[0] += (sha2_word64)(n); \
    if ((w)[0] < (n)) { \
        (w)[1]++; \
    } \
}

/*
 * Macros for copying blocks of memory and for zeroing out ranges
 * of memory.  Using these macros makes it easy to switch from
 * using memset()/memcpy() and using bzero()/bcopy().
 *
 * Please define either SHA2_USE_MEMSET_MEMCPY or define
 * SHA2_USE_BZERO_BCOPY depending on which function set you
 * choose to use:
 */
#if !defined(SHA2_USE_MEMSET_MEMCPY) && !defined(SHA2_USE_BZERO_BCOPY)
/* Default to memset()/memcpy() if no option is specified */
#define SHA2_USE_MEMSET_MEMCPY  1
#endif
#if defined(SHA2_USE_MEMSET_MEMCPY) && defined(SHA2_USE_BZERO_BCOPY)
/* Abort with an error if BOTH options are defined */
#error Define either SHA2_USE_MEMSET_MEMCPY or SHA2_USE_BZERO_BCOPY, not both!
#endif

#ifdef SHA2_USE_MEMSET_MEMCPY
#define MEMSET_BZERO(p,l)   memset((p), 0, (l))
#define MEMCPY_BCOPY(d,s,l) memcpy((d), (s), (l))
#endif
#ifdef SHA2_USE_BZERO_BCOPY
#define MEMSET_BZERO(p,l)   bzero((p), (l))
#define MEMCPY_BCOPY(d,s,l) bcopy((s), (d), (l))
#endif


/*** THE SIX LOGICAL FUNCTIONS ****************************************/
/*
 * Bit shifting and rotation (used by the six SHA-XYZ logical functions:
 *
 *   NOTE:  The naming of R and S appears backwards here (R is a SHIFT and
 *   S is a ROTATION) because the SHA-256/384/512 description document
 *   (see http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf) uses this
 *   same "backwards" definition.
 */
/* Shift-right (used in SHA-256, SHA-384, and SHA-512): */
#define R(b,x)      ((x) >> (b))
/* 32-bit Rotate-right (used in SHA-256): */
#define S32(b,x)    (((x) >> (b)) | ((x) << (32 - (b))))
/* 64-bit Rotate-right (used in SHA-384 and SHA-512): */
#define S64(b,x)    (((x) >> (b)) | ((x) << (64 - (b))))

/* Two of six logical functions used in SHA-256, SHA-384, and SHA-512: */
#define Ch(x,y,z)   (((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)  (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/* Four of six logical functions used in SHA-256: */
#define Sigma0_256(x)   (S32(2,  (x)) ^ S32(13, (x)) ^ S32(22, (x)))
#define Sigma1_256(x)   (S32(6,  (x)) ^ S32(11, (x)) ^ S32(25, (x)))
#define sigma0_256(x)   (S32(7,  (x)) ^ S32(18, (x)) ^ R(3 ,   (x)))
#define sigma1_256(x)   (S32(17, (x)) ^ S32(19, (x)) ^ R(10,   (x)))

/* Four of six logical functions used in SHA-384 and SHA-512: */
#define Sigma0_512(x)   (S64(28, (x)) ^ S64(34, (x)) ^ S64(39, (x)))
#define Sigma1_512(x)   (S64(14, (x)) ^ S64(18, (x)) ^ S64(41, (x)))
#define sigma0_512(x)   (S64( 1, (x)) ^ S64( 8, (x)) ^ R( 7,   (x)))
#define sigma1_512(x)   (S64(19, (x)) ^ S64(61, (x)) ^ R( 6,   (x)))

/*** INTERNAL FUNCTION PROTOTYPES *************************************/
/* NOTE: These should not be accessed directly from outside this
 * library -- they are intended for private internal visibility/use
 * only.
 */
//void SHA512_Last(SHA512_CTX*);
//void SHA256_Transform(SHA256_CTX*, const sha2_word32*);
//void SHA512_Transform(SHA512_CTX*, const sha2_word64*);


/*** SHA-XYZ INITIAL HASH VALUES AND CONSTANTS ************************/
/* Hash constant words K for SHA-256: */
const static sha2_word32 K256[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
    0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
    0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
    0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
    0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Initial hash value H for SHA-256: */
const static sha2_word32 sha256_initial_hash_value[8] = {
    0x6a09e667UL,
    0xbb67ae85UL,
    0x3c6ef372UL,
    0xa54ff53aUL,
    0x510e527fUL,
    0x9b05688cUL,
    0x1f83d9abUL,
    0x5be0cd19UL
};

/* Hash constant words K for SHA-384 and SHA-512: */
const static sha2_word64 K512[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
    0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
    0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
    0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
    0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
    0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
    0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
    0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
    0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
    0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
    0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
    0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
    0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
    0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

/* Initial hash value H for SHA-384 */
const static sha2_word64 sha384_initial_hash_value[8] = {
    0xcbbb9d5dc1059ed8ULL,
    0x629a292a367cd507ULL,
    0x9159015a3070dd17ULL,
    0x152fecd8f70e5939ULL,
    0x67332667ffc00b31ULL,
    0x8eb44a8768581511ULL,
    0xdb0c2e0d64f98fa7ULL,
    0x47b5481dbefa4fa4ULL
};

/* Initial hash value H for SHA-512 */
const static sha2_word64 sha512_initial_hash_value[8] = {
    0x6a09e667f3bcc908ULL,
    0xbb67ae8584caa73bULL,
    0x3c6ef372fe94f82bULL,
    0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL,
    0x9b05688c2b3e6c1fULL,
    0x1f83d9abfb41bd6bULL,
    0x5be0cd19137e2179ULL
};

/*
 * Constant used by SHA256/384/512_End() functions for converting the
 * digest to a readable hexadecimal character string:
 */
static const char *sha2_hex_digits = "0123456789abcdef";


/*** SHA-256: *********************************************************/
static void * SHA256_Init() {
    SHA256_CTX *context;
    context = sqlite3_malloc(sizeof(SHA256_CTX));
    if (!context)
        return NULL;
    MEMCPY_BCOPY(context->state, sha256_initial_hash_value, SHA256_DIGEST_LENGTH);
    MEMSET_BZERO(context->buffer, SHA256_BLOCK_LENGTH);
    context->bitcount = 0;
    return context;
}

#ifdef SHA2_UNROLL_TRANSFORM

/* Unrolled SHA-256 round macros: */

#if BYTE_ORDER == LITTLE_ENDIAN

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)   \
    REVERSE32(*data++, W256[j]); \
    T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + \
             K256[j] + W256[j]; \
    (d) += T1; \
    (h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
    j++


#else /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)   \
    T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + \
         K256[j] + (W256[j] = *data++); \
    (d) += T1; \
    (h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
    j++

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND256(a,b,c,d,e,f,g,h)   \
    s0 = W256[(j+1)&0x0f]; \
    s0 = sigma0_256(s0); \
    s1 = W256[(j+14)&0x0f]; \
    s1 = sigma1_256(s1); \
    T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + K256[j] + \
         (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0); \
    (d) += T1; \
    (h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
    j++

static void  SHA256_Transform(SHA256_CTX* context, const sha2_word32* data) {
    sha2_word32 a, b, c, d, e, f, g, h, s0, s1;
    sha2_word32 T1, *W256;
    int     j;

    W256 = (sha2_word32*)context->buffer;

    /* Initialize registers with the prev. intermediate value */
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    f = context->state[5];
    g = context->state[6];
    h = context->state[7];

    j = 0;
    do {
        /* Rounds 0 to 15 (unrolled): */
        ROUND256_0_TO_15(a,b,c,d,e,f,g,h);
        ROUND256_0_TO_15(h,a,b,c,d,e,f,g);
        ROUND256_0_TO_15(g,h,a,b,c,d,e,f);
        ROUND256_0_TO_15(f,g,h,a,b,c,d,e);
        ROUND256_0_TO_15(e,f,g,h,a,b,c,d);
        ROUND256_0_TO_15(d,e,f,g,h,a,b,c);
        ROUND256_0_TO_15(c,d,e,f,g,h,a,b);
        ROUND256_0_TO_15(b,c,d,e,f,g,h,a);
    } while (j < 16);

    /* Now for the remaining rounds to 64: */
    do {
        ROUND256(a,b,c,d,e,f,g,h);
        ROUND256(h,a,b,c,d,e,f,g);
        ROUND256(g,h,a,b,c,d,e,f);
        ROUND256(f,g,h,a,b,c,d,e);
        ROUND256(e,f,g,h,a,b,c,d);
        ROUND256(d,e,f,g,h,a,b,c);
        ROUND256(c,d,e,f,g,h,a,b);
        ROUND256(b,c,d,e,f,g,h,a);
    } while (j < 64);

    /* Compute the current intermediate hash value */
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
    context->state[5] += f;
    context->state[6] += g;
    context->state[7] += h;

    /* Clean up */
    a = b = c = d = e = f = g = h = T1 = 0;
}

#else /* SHA2_UNROLL_TRANSFORM */

static void  SHA256_Transform(SHA256_CTX* context, const sha2_word32* data) {
    sha2_word32 a, b, c, d, e, f, g, h, s0, s1;
    sha2_word32 T1, T2, *W256;
    int     j;

    W256 = (sha2_word32*)context->buffer;

    /* Initialize registers with the prev. intermediate value */
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    f = context->state[5];
    g = context->state[6];
    h = context->state[7];

    j = 0;
    do {
#if BYTE_ORDER == LITTLE_ENDIAN
        /* Copy data while converting to host byte order */
        REVERSE32(*data++,W256[j]);
        /* Apply the SHA-256 compression function to update a..h */
        T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + W256[j];
#else /* BYTE_ORDER == LITTLE_ENDIAN */
        /* Apply the SHA-256 compression function to update a..h with copy */
        T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + (W256[j] = *data++);
#endif /* BYTE_ORDER == LITTLE_ENDIAN */
        T2 = Sigma0_256(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;

        j++;
    } while (j < 16);

    do {
        /* Part of the message block expansion: */
        s0 = W256[(j+1)&0x0f];
        s0 = sigma0_256(s0);
        s1 = W256[(j+14)&0x0f];
        s1 = sigma1_256(s1);

        /* Apply the SHA-256 compression function to update a..h */
        T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] +
             (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0);
        T2 = Sigma0_256(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;

        j++;
    } while (j < 64);

    /* Compute the current intermediate hash value */
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
    context->state[5] += f;
    context->state[6] += g;
    context->state[7] += h;

    /* Clean up */
    a = b = c = d = e = f = g = h = T1 = T2 = 0;
}

#endif /* SHA2_UNROLL_TRANSFORM */

static void SHA256_Update(SHA256_CTX* context, const sha2_byte *data, size_t len) {
    unsigned int    freespace, usedspace;

    if (len == 0) {
        /* Calling with no data is valid - we do nothing */
        return;
    }

    /* Sanity check: */
    assert(context != (SHA256_CTX*)0 && data != (sha2_byte*)0);

    usedspace = (context->bitcount >> 3) % SHA256_BLOCK_LENGTH;
    if (usedspace > 0) {
        /* Calculate how much free space is available in the buffer */
        freespace = SHA256_BLOCK_LENGTH - usedspace;

        if (len >= freespace) {
            /* Fill the buffer completely and process it */
            MEMCPY_BCOPY(&context->buffer[usedspace], data, freespace);
            context->bitcount += freespace << 3;
            len -= freespace;
            data += freespace;
            SHA256_Transform(context, (sha2_word32*)context->buffer);
        } else {
            /* The buffer is not yet full */
            MEMCPY_BCOPY(&context->buffer[usedspace], data, len);
            context->bitcount += len << 3;
            /* Clean up: */
            usedspace = freespace = 0;
            return;
        }
    }
    while (len >= SHA256_BLOCK_LENGTH) {
        /* Process as many complete blocks as we can */
        SHA256_Transform(context, (sha2_word32*)data);
        context->bitcount += SHA256_BLOCK_LENGTH << 3;
        len -= SHA256_BLOCK_LENGTH;
        data += SHA256_BLOCK_LENGTH;
    }
    if (len > 0) {
        /* There's left-overs, so save 'em */
        MEMCPY_BCOPY(context->buffer, data, len);
        context->bitcount += len << 3;
    }
    /* Clean up: */
    usedspace = freespace = 0;
}

static int SHA256_Final(sha2_byte digest[], SHA256_CTX* context) {
    sha2_word32 *d = (sha2_word32*)digest;
    unsigned int    usedspace;

    /* Sanity check: */
    assert(context != (SHA256_CTX*)0);

    /* If no digest buffer is passed, we don't bother doing this: */
    if (digest != (sha2_byte*)0) {
        usedspace = (context->bitcount >> 3) % SHA256_BLOCK_LENGTH;
#if BYTE_ORDER == LITTLE_ENDIAN
        /* Convert FROM host byte order */
        REVERSE64(context->bitcount,context->bitcount);
#endif
        if (usedspace > 0) {
            /* Begin padding with a 1 bit: */
            context->buffer[usedspace++] = 0x80;

            if (usedspace <= SHA256_SHORT_BLOCK_LENGTH) {
                /* Set-up for the last transform: */
                MEMSET_BZERO(&context->buffer[usedspace], SHA256_SHORT_BLOCK_LENGTH - usedspace);
            } else {
                if (usedspace < SHA256_BLOCK_LENGTH) {
                    MEMSET_BZERO(&context->buffer[usedspace], SHA256_BLOCK_LENGTH - usedspace);
                }
                /* Do second-to-last transform: */
                SHA256_Transform(context, (sha2_word32*)context->buffer);

                /* And set-up for the last transform: */
                MEMSET_BZERO(context->buffer, SHA256_SHORT_BLOCK_LENGTH);
            }
        } else {
            /* Set-up for the last transform: */
            MEMSET_BZERO(context->buffer, SHA256_SHORT_BLOCK_LENGTH);

            /* Begin padding with a 1 bit: */
            *context->buffer = 0x80;
        }
        /* Set the bit count: */
        *(sha2_word64*)&context->buffer[SHA256_SHORT_BLOCK_LENGTH] = context->bitcount;

        /* Final transform: */
        SHA256_Transform(context, (sha2_word32*)context->buffer);

#if BYTE_ORDER == LITTLE_ENDIAN
        {
            /* Convert TO host byte order */
            int j;
            for (j = 0; j < 8; j++) {
                REVERSE32(context->state[j],context->state[j]);
                *d++ = context->state[j];
            }
        }
#else
        MEMCPY_BCOPY(d, context->state, SHA256_DIGEST_LENGTH);
#endif
    }

    /* Clean up state data: */
    sqlite3_free(context);
    usedspace = 0;
    return SHA256_DIGEST_LENGTH;
}


/*** SHA-512: *********************************************************/
static void *SHA512_Init() {
    SHA512_CTX *context;
    context = sqlite3_malloc(sizeof(SHA512_CTX));
    if (!context)
        return NULL;
    MEMCPY_BCOPY(context->state, sha512_initial_hash_value, SHA512_DIGEST_LENGTH);
    MEMSET_BZERO(context->buffer, SHA512_BLOCK_LENGTH);
    context->bitcount[0] = context->bitcount[1] =  0;
    return context;
}

#ifdef SHA2_UNROLL_TRANSFORM

/* Unrolled SHA-512 round macros: */
#if BYTE_ORDER == LITTLE_ENDIAN

#define ROUND512_0_TO_15(a,b,c,d,e,f,g,h)   \
    REVERSE64(*data++, W512[j]); \
    T1 = (h) + Sigma1_512(e) + Ch((e), (f), (g)) + \
             K512[j] + W512[j]; \
    (d) += T1, \
    (h) = T1 + Sigma0_512(a) + Maj((a), (b), (c)), \
    j++


#else /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND512_0_TO_15(a,b,c,d,e,f,g,h)   \
    T1 = (h) + Sigma1_512(e) + Ch((e), (f), (g)) + \
             K512[j] + (W512[j] = *data++); \
    (d) += T1; \
    (h) = T1 + Sigma0_512(a) + Maj((a), (b), (c)); \
    j++

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND512(a,b,c,d,e,f,g,h)   \
    s0 = W512[(j+1)&0x0f]; \
    s0 = sigma0_512(s0); \
    s1 = W512[(j+14)&0x0f]; \
    s1 = sigma1_512(s1); \
    T1 = (h) + Sigma1_512(e) + Ch((e), (f), (g)) + K512[j] + \
             (W512[j&0x0f] += s1 + W512[(j+9)&0x0f] + s0); \
    (d) += T1; \
    (h) = T1 + Sigma0_512(a) + Maj((a), (b), (c)); \
    j++

static void  SHA512_Transform(SHA512_CTX* context, const sha2_word64* data) {
    sha2_word64 a, b, c, d, e, f, g, h, s0, s1;
    sha2_word64 T1, *W512 = (sha2_word64*)context->buffer;
    int     j;

    /* Initialize registers with the prev. intermediate value */
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    f = context->state[5];
    g = context->state[6];
    h = context->state[7];

    j = 0;
    do {
        ROUND512_0_TO_15(a,b,c,d,e,f,g,h);
        ROUND512_0_TO_15(h,a,b,c,d,e,f,g);
        ROUND512_0_TO_15(g,h,a,b,c,d,e,f);
        ROUND512_0_TO_15(f,g,h,a,b,c,d,e);
        ROUND512_0_TO_15(e,f,g,h,a,b,c,d);
        ROUND512_0_TO_15(d,e,f,g,h,a,b,c);
        ROUND512_0_TO_15(c,d,e,f,g,h,a,b);
        ROUND512_0_TO_15(b,c,d,e,f,g,h,a);
    } while (j < 16);

    /* Now for the remaining rounds up to 79: */
    do {
        ROUND512(a,b,c,d,e,f,g,h);
        ROUND512(h,a,b,c,d,e,f,g);
        ROUND512(g,h,a,b,c,d,e,f);
        ROUND512(f,g,h,a,b,c,d,e);
        ROUND512(e,f,g,h,a,b,c,d);
        ROUND512(d,e,f,g,h,a,b,c);
        ROUND512(c,d,e,f,g,h,a,b);
        ROUND512(b,c,d,e,f,g,h,a);
    } while (j < 80);

    /* Compute the current intermediate hash value */
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
    context->state[5] += f;
    context->state[6] += g;
    context->state[7] += h;

    /* Clean up */
    a = b = c = d = e = f = g = h = T1 = 0;
}

#else /* SHA2_UNROLL_TRANSFORM */

static void  SHA512_Transform(SHA512_CTX* context, const sha2_word64* data) {
    sha2_word64 a, b, c, d, e, f, g, h, s0, s1;
    sha2_word64 T1, T2, *W512 = (sha2_word64*)context->buffer;
    int     j;

    /* Initialize registers with the prev. intermediate value */
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    f = context->state[5];
    g = context->state[6];
    h = context->state[7];

    j = 0;
    do {
#if BYTE_ORDER == LITTLE_ENDIAN
        /* Convert TO host byte order */
        REVERSE64(*data++, W512[j]);
        /* Apply the SHA-512 compression function to update a..h */
        T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] + W512[j];
#else /* BYTE_ORDER == LITTLE_ENDIAN */
        /* Apply the SHA-512 compression function to update a..h with copy */
        T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] + (W512[j] = *data++);
#endif /* BYTE_ORDER == LITTLE_ENDIAN */
        T2 = Sigma0_512(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;

        j++;
    } while (j < 16);

    do {
        /* Part of the message block expansion: */
        s0 = W512[(j+1)&0x0f];
        s0 = sigma0_512(s0);
        s1 = W512[(j+14)&0x0f];
        s1 =  sigma1_512(s1);

        /* Apply the SHA-512 compression function to update a..h */
        T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] +
             (W512[j&0x0f] += s1 + W512[(j+9)&0x0f] + s0);
        T2 = Sigma0_512(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;

        j++;
    } while (j < 80);

    /* Compute the current intermediate hash value */
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
    context->state[5] += f;
    context->state[6] += g;
    context->state[7] += h;

    /* Clean up */
    a = b = c = d = e = f = g = h = T1 = T2 = 0;
}

#endif /* SHA2_UNROLL_TRANSFORM */

static void SHA512_Update(SHA512_CTX* context, const sha2_byte *data, size_t len) {
    unsigned int    freespace, usedspace;

    if (len == 0) {
        /* Calling with no data is valid - we do nothing */
        return;
    }

    /* Sanity check: */
    assert(context != (SHA512_CTX*)0 && data != (sha2_byte*)0);

    usedspace = (context->bitcount[0] >> 3) % SHA512_BLOCK_LENGTH;
    if (usedspace > 0) {
        /* Calculate how much free space is available in the buffer */
        freespace = SHA512_BLOCK_LENGTH - usedspace;

        if (len >= freespace) {
            /* Fill the buffer completely and process it */
            MEMCPY_BCOPY(&context->buffer[usedspace], data, freespace);
            ADDINC128(context->bitcount, freespace << 3);
            len -= freespace;
            data += freespace;
            SHA512_Transform(context, (sha2_word64*)context->buffer);
        } else {
            /* The buffer is not yet full */
            MEMCPY_BCOPY(&context->buffer[usedspace], data, len);
            ADDINC128(context->bitcount, len << 3);
            /* Clean up: */
            usedspace = freespace = 0;
            return;
        }
    }
    while (len >= SHA512_BLOCK_LENGTH) {
        /* Process as many complete blocks as we can */
        SHA512_Transform(context, (sha2_word64*)data);
        ADDINC128(context->bitcount, SHA512_BLOCK_LENGTH << 3);
        len -= SHA512_BLOCK_LENGTH;
        data += SHA512_BLOCK_LENGTH;
    }
    if (len > 0) {
        /* There's left-overs, so save 'em */
        MEMCPY_BCOPY(context->buffer, data, len);
        ADDINC128(context->bitcount, len << 3);
    }
    /* Clean up: */
    usedspace = freespace = 0;
}

static void SHA512_Last(SHA512_CTX* context) {
    unsigned int    usedspace;

    usedspace = (context->bitcount[0] >> 3) % SHA512_BLOCK_LENGTH;
#if BYTE_ORDER == LITTLE_ENDIAN
    /* Convert FROM host byte order */
    REVERSE64(context->bitcount[0],context->bitcount[0]);
    REVERSE64(context->bitcount[1],context->bitcount[1]);
#endif
    if (usedspace > 0) {
        /* Begin padding with a 1 bit: */
        context->buffer[usedspace++] = 0x80;

        if (usedspace <= SHA512_SHORT_BLOCK_LENGTH) {
            /* Set-up for the last transform: */
            MEMSET_BZERO(&context->buffer[usedspace], SHA512_SHORT_BLOCK_LENGTH - usedspace);
        } else {
            if (usedspace < SHA512_BLOCK_LENGTH) {
                MEMSET_BZERO(&context->buffer[usedspace], SHA512_BLOCK_LENGTH - usedspace);
            }
            /* Do second-to-last transform: */
            SHA512_Transform(context, (sha2_word64*)context->buffer);

            /* And set-up for the last transform: */
            MEMSET_BZERO(context->buffer, SHA512_BLOCK_LENGTH - 2);
        }
    } else {
        /* Prepare for final transform: */
        MEMSET_BZERO(context->buffer, SHA512_SHORT_BLOCK_LENGTH);

        /* Begin padding with a 1 bit: */
        *context->buffer = 0x80;
    }
    /* Store the length of input data (in bits): */
    *(sha2_word64*)&context->buffer[SHA512_SHORT_BLOCK_LENGTH] = context->bitcount[1];
    *(sha2_word64*)&context->buffer[SHA512_SHORT_BLOCK_LENGTH+8] = context->bitcount[0];

    /* Final transform: */
    SHA512_Transform(context, (sha2_word64*)context->buffer);
}

static int SHA512_Final(sha2_byte digest[], SHA512_CTX* context) {
    sha2_word64 *d = (sha2_word64*)digest;

    /* Sanity check: */
    assert(context != (SHA512_CTX*)0);

    /* If no digest buffer is passed, we don't bother doing this: */
    if (digest != (sha2_byte*)0) {
        SHA512_Last(context);

        /* Save the hash data for output: */
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            /* Convert TO host byte order */
            int j;
            for (j = 0; j < 8; j++) {
                REVERSE64(context->state[j],context->state[j]);
                *d++ = context->state[j];
            }
        }
#else
        MEMCPY_BCOPY(d, context->state, SHA512_DIGEST_LENGTH);
#endif
    }

    /* Zero out state data */
    sqlite3_free(context);
    return SHA512_DIGEST_LENGTH;
}


/*** SHA-384: *********************************************************/
static void *SHA384_Init() {
    SHA384_CTX *context;
    context = sqlite3_malloc(sizeof(SHA384_CTX));
    if (!context)
        return NULL;
    MEMCPY_BCOPY(context->state, sha384_initial_hash_value, SHA512_DIGEST_LENGTH);
    MEMSET_BZERO(context->buffer, SHA384_BLOCK_LENGTH);
    context->bitcount[0] = context->bitcount[1] = 0;
    return context;
}

static void SHA384_Update(SHA384_CTX* context, const sha2_byte* data, size_t len) {
    SHA512_Update((SHA512_CTX*)context, data, len);
}

static int SHA384_Final(sha2_byte digest[], SHA384_CTX* context) {
    sha2_word64 *d = (sha2_word64*)digest;

    /* Sanity check: */
    assert(context != (SHA384_CTX*)0);

    /* If no digest buffer is passed, we don't bother doing this: */
    if (digest != (sha2_byte*)0) {
        SHA512_Last((SHA512_CTX*)context);

        /* Save the hash data for output: */
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            /* Convert TO host byte order */
            int j;
            for (j = 0; j < 6; j++) {
                REVERSE64(context->state[j],context->state[j]);
                *d++ = context->state[j];
            }
        }
#else
        MEMCPY_BCOPY(d, context->state, SHA384_DIGEST_LENGTH);
#endif
    }

    /* Zero out state data */
    sqlite3_free(context);
    return SHA384_DIGEST_LENGTH;
}

/* End of Insert sha2.c/h */

/* Insert md2.h and md2.c */

/*********************************************************************
* Filename:   md2.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding MD2 implementation.
*********************************************************************/

#ifndef MD2_H
#define MD2_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define MD2_BLOCK_SIZE 16

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte

typedef struct {
   BYTE data[16];
   BYTE state[48];
   BYTE checksum[16];
   int len;
} MD2_CTX;

#endif   // MD2_H

/*********************************************************************
* Filename:   md2.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the MD2 hashing algorithm.
                  Algorithm specification can be found here:
                   * http://tools.ietf.org/html/rfc1319 .
              Input is  little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <memory.h>

/**************************** VARIABLES *****************************
static const BYTE s[256] = {
    41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
    19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
    76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
    138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
    245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
    148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
    39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
    181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
    150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
    112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
    96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
    85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
    234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
    129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
    8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
    203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
    166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
    31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};

 *********************** FUNCTION DEFINITIONS ***********************/
static void  md2_transform(MD2_CTX *ctx, BYTE data[])
{
    int j,k,t;
static const BYTE s[256] = {
    41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
    19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
    76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
    138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
    245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
    148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
    39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
    181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
    150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
    112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
    96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
    85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
    234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
    129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
    8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
    203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
    166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
    31, 26, 219, 153, 141, 51, 159, 17, 131, 20
};

    //memcpy(&ctx->state[16], data);
    for (j=0; j < 16; ++j) {
        ctx->state[j + 16] = data[j];
        ctx->state[j + 32] = (ctx->state[j+16] ^ ctx->state[j]);
    }

    t = 0;
    for (j = 0; j < 18; ++j) {
        for (k = 0; k < 48; ++k) {
            ctx->state[k] ^= s[t];
            t = ctx->state[k];
        }
        t = (t+j) & 0xFF;
    }

    t = ctx->checksum[15];
    for (j=0; j < 16; ++j) {
        ctx->checksum[j] ^= s[data[j] ^ t];
        t = ctx->checksum[j];
    }
}

static void *md2_init()
{
    int i;
    MD2_CTX *ctx;

    ctx = sqlite3_malloc(sizeof(MD2_CTX));
    if (!ctx)
        return NULL;
    for (i=0; i < 48; ++i)
        ctx->state[i] = 0;
    for (i=0; i < 16; ++i)
        ctx->checksum[i] = 0;
    ctx->len = 0;
    return ctx;
}

static void md2_update(MD2_CTX *ctx, const BYTE data[], unsigned long len)
{
    size_t i;

    for (i = 0; i < len; ++i) {
        ctx->data[ctx->len] = data[i];
        ctx->len++;
        if (ctx->len == MD2_BLOCK_SIZE) {
            md2_transform(ctx, ctx->data);
            ctx->len = 0;
        }
    }
}

static int md2_final(BYTE hash[], MD2_CTX *ctx)
{
    int to_pad;

    to_pad = MD2_BLOCK_SIZE - ctx->len;

    while (ctx->len < MD2_BLOCK_SIZE)
        ctx->data[ctx->len++] = to_pad;

    md2_transform(ctx, ctx->data);
    md2_transform(ctx, ctx->checksum);

    memcpy(hash, ctx->state, MD2_BLOCK_SIZE);
    sqlite3_free(ctx);
    return MD2_BLOCK_SIZE;
}

/* Insert md4.c md4.h */

/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD4 Message-Digest Algorithm (RFC 1320).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md4
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * This software was written by Alexander Peslyak in 2001.  No copyright is
 * claimed, and the software is hereby placed in the public domain.
 * In case this attempt to disclaim copyright and place the software in the
 * public domain is deemed null and void, then the software is
 * Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 * general public under the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 *
 * See md4.c for more information.
 */

#ifdef HAVE_OPENSSL
#include <openssl/md4.h>
#elif !defined(_MD4_H)
#define _MD4_H

/* Any 32-bit or wider unsigned integer data type will do */
typedef unsigned int MD4_u32plus;

typedef struct {
    MD4_u32plus lo, hi;
    MD4_u32plus a, b, c, d;
    unsigned char buffer[64];
    MD4_u32plus block[16];
} MD4_CTX;

#endif

/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security, Inc.
 * MD4 Message-Digest Algorithm (RFC 1320).
 *
 * Homepage:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md4
 *
 * Author:
 * Alexander Peslyak, better known as Solar Designer <solar at openwall.com>
 *
 * This software was written by Alexander Peslyak in 2001.  No copyright is
 * claimed, and the software is hereby placed in the public domain.
 * In case this attempt to disclaim copyright and place the software in the
 * public domain is deemed null and void, then the software is
 * Copyright (c) 2001 Alexander Peslyak and it is hereby released to the
 * general public under the following terms:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 *
 * (This is a heavily cut-down "BSD license".)
 *
 * This differs from Colin Plumb's older public domain implementation in that
 * no exactly 32-bit integer data type is required (any 32-bit or wider
 * unsigned integer data type will do), there's no compile-time endianness
 * configuration, and the function prototypes match OpenSSL's.  No code from
 * Colin Plumb's implementation has been reused; this comment merely compares
 * the properties of the two independent implementations.
 *
 * The primary goals of this implementation are portability and ease of use.
 * It is meant to be fast, but not as fast as possible.  Some known
 * optimizations are not included to reduce source code size and avoid
 * compile-time configuration.
 */

#ifndef HAVE_OPENSSL

#include <string.h>

/*
 * The basic MD4 functions.
 *
 * F and G are optimized compared to their RFC 1320 definitions, with the
 * optimization for F borrowed from Colin Plumb's MD5 implementation.
 */
#define F(x, y, z)          ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)          (((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z)          ((x) ^ (y) ^ (z))

/*
 * The MD4 transformation for all three rounds.
 */
#define STEP(f, a, b, c, d, x, s) \
    (a) += f((b), (c), (d)) + (x); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));

/*
 * SET reads 4 input bytes in little-endian byte order and stores them in a
 * properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned memory
 * accesses is just an optimization.  Nothing will break if it fails to detect
 * a suitable architecture.
 *
 * Unfortunately, this optimization may be a C strict aliasing rules violation
 * if the caller's data buffer has effective type that cannot be aliased by
 * MD4_u32plus.  In practice, this problem may occur if these MD4 routines are
 * inlined into a calling function, or with future and dangerously advanced
 * link-time optimizations.  For the time being, keeping these MD4 routines in
 * their own translation unit avoids the problem.
 */
#if defined(__i386__) || defined(__x86_64__) || defined(__vax__)
#define SET(n) \
    (*(MD4_u32plus *)&ptr[(n) * 4])
#define GET(n) \
    SET(n)
#else
#define SET(n) \
    (ctx->block[(n)] = \
    (MD4_u32plus)ptr[(n) * 4] | \
    ((MD4_u32plus)ptr[(n) * 4 + 1] << 8) | \
    ((MD4_u32plus)ptr[(n) * 4 + 2] << 16) | \
    ((MD4_u32plus)ptr[(n) * 4 + 3] << 24))
#define GET(n) \
    (ctx->block[(n)])
#endif

/*
 * This processes one or more 64-byte data blocks, but does NOT update the bit
 * counters.  There are no alignment requirements.
 */
static const void  *MD4_body(MD4_CTX *ctx, const void *data, unsigned long size)
{
    const unsigned char *ptr;
    MD4_u32plus a, b, c, d;
    MD4_u32plus saved_a, saved_b, saved_c, saved_d;
    const MD4_u32plus ac1 = 0x5a827999, ac2 = 0x6ed9eba1;

    ptr = (const unsigned char *)data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

/* Round 1 */
        STEP(F, a, b, c, d, SET(0), 3)
        STEP(F, d, a, b, c, SET(1), 7)
        STEP(F, c, d, a, b, SET(2), 11)
        STEP(F, b, c, d, a, SET(3), 19)
        STEP(F, a, b, c, d, SET(4), 3)
        STEP(F, d, a, b, c, SET(5), 7)
        STEP(F, c, d, a, b, SET(6), 11)
        STEP(F, b, c, d, a, SET(7), 19)
        STEP(F, a, b, c, d, SET(8), 3)
        STEP(F, d, a, b, c, SET(9), 7)
        STEP(F, c, d, a, b, SET(10), 11)
        STEP(F, b, c, d, a, SET(11), 19)
        STEP(F, a, b, c, d, SET(12), 3)
        STEP(F, d, a, b, c, SET(13), 7)
        STEP(F, c, d, a, b, SET(14), 11)
        STEP(F, b, c, d, a, SET(15), 19)

/* Round 2 */
        STEP(G, a, b, c, d, GET(0) + ac1, 3)
        STEP(G, d, a, b, c, GET(4) + ac1, 5)
        STEP(G, c, d, a, b, GET(8) + ac1, 9)
        STEP(G, b, c, d, a, GET(12) + ac1, 13)
        STEP(G, a, b, c, d, GET(1) + ac1, 3)
        STEP(G, d, a, b, c, GET(5) + ac1, 5)
        STEP(G, c, d, a, b, GET(9) + ac1, 9)
        STEP(G, b, c, d, a, GET(13) + ac1, 13)
        STEP(G, a, b, c, d, GET(2) + ac1, 3)
        STEP(G, d, a, b, c, GET(6) + ac1, 5)
        STEP(G, c, d, a, b, GET(10) + ac1, 9)
        STEP(G, b, c, d, a, GET(14) + ac1, 13)
        STEP(G, a, b, c, d, GET(3) + ac1, 3)
        STEP(G, d, a, b, c, GET(7) + ac1, 5)
        STEP(G, c, d, a, b, GET(11) + ac1, 9)
        STEP(G, b, c, d, a, GET(15) + ac1, 13)

/* Round 3 */
        STEP(H, a, b, c, d, GET(0) + ac2, 3)
        STEP(H, d, a, b, c, GET(8) + ac2, 9)
        STEP(H, c, d, a, b, GET(4) + ac2, 11)
        STEP(H, b, c, d, a, GET(12) + ac2, 15)
        STEP(H, a, b, c, d, GET(2) + ac2, 3)
        STEP(H, d, a, b, c, GET(10) + ac2, 9)
        STEP(H, c, d, a, b, GET(6) + ac2, 11)
        STEP(H, b, c, d, a, GET(14) + ac2, 15)
        STEP(H, a, b, c, d, GET(1) + ac2, 3)
        STEP(H, d, a, b, c, GET(9) + ac2, 9)
        STEP(H, c, d, a, b, GET(5) + ac2, 11)
        STEP(H, b, c, d, a, GET(13) + ac2, 15)
        STEP(H, a, b, c, d, GET(3) + ac2, 3)
        STEP(H, d, a, b, c, GET(11) + ac2, 9)
        STEP(H, c, d, a, b, GET(7) + ac2, 11)
        STEP(H, b, c, d, a, GET(15) + ac2, 15)

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    return ptr;
}

static void *MD4_Init()
{
    MD4_CTX *ctx;
    ctx = sqlite3_malloc(sizeof(MD4_CTX));
    if (!ctx)
        return NULL;
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->lo = 0;
    ctx->hi = 0;
    return ctx;
}

static void MD4_Update(MD4_CTX *ctx, const void *data, unsigned long size)
{
    MD4_u32plus saved_lo;
    unsigned long used, available;

    saved_lo = ctx->lo;
    if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
        ctx->hi++;
    ctx->hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
        available = 64 - used;

        if (size < available) {
            memcpy(&ctx->buffer[used], data, size);
            return;
        }

        memcpy(&ctx->buffer[used], data, available);
        data = (const unsigned char *)data + available;
        size -= available;
        MD4_body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
        data = MD4_body(ctx, data, size & ~(unsigned long)0x3f);
        size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
}

#define MD4OUT(dst, src) \
    (dst)[0] = (unsigned char)(src); \
    (dst)[1] = (unsigned char)((src) >> 8); \
    (dst)[2] = (unsigned char)((src) >> 16); \
    (dst)[3] = (unsigned char)((src) >> 24);

static int MD4_Final(unsigned char *result, MD4_CTX *ctx)
{
    unsigned long used, available;

    used = ctx->lo & 0x3f;

    ctx->buffer[used++] = 0x80;

    available = 64 - used;

    if (available < 8) {
        memset(&ctx->buffer[used], 0, available);
        MD4_body(ctx, ctx->buffer, 64);
        used = 0;
        available = 64;
    }

    memset(&ctx->buffer[used], 0, available - 8);

    ctx->lo <<= 3;
    MD4OUT(&ctx->buffer[56], ctx->lo)
    MD4OUT(&ctx->buffer[60], ctx->hi)

    MD4_body(ctx, ctx->buffer, 64);

    MD4OUT(&result[0], ctx->a)
    MD4OUT(&result[4], ctx->b)
    MD4OUT(&result[8], ctx->c)
    MD4OUT(&result[12], ctx->d)

    sqlite3_free(ctx);
    return 16;
}

#endif


/* Insert MD5.c and md5.h */

/*********************************************************************
* Filename:   md5.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding MD5 implementation.
*********************************************************************/

#ifndef MD5_H
#define MD5_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define MD5_BLOCK_SIZE 16               // MD5 outputs a 16 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned long MD5WORD;          // 32-bit word, change to "long" for 16-bit machines

typedef struct {
   BYTE data[64];
   MD5WORD datalen;
   unsigned long long bitlen;
   MD5WORD state[4];
} MD5_CTX;

#endif   // MD5_H

/*********************************************************************
* Filename:   md5.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the MD5 hashing algorithm.
                  Algorithm specification can be found here:
                   * http://tools.ietf.org/html/rfc1321
                  This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdlib.h>
#include <memory.h>

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) ((a << b) | (a >> (32-b)))

#ifdef F
#undef F
#endif
#ifdef G
#undef G
#endif
#ifdef H
#undef H
#endif

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x ^ y ^ z)
#define I(x,y,z) (y ^ (x | ~z))

#define FF(a,b,c,d,m,s,t) { a += F(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); }
#define GG(a,b,c,d,m,s,t) { a += G(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); }
#define HH(a,b,c,d,m,s,t) { a += H(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); }
#define II(a,b,c,d,m,s,t) { a += I(b,c,d) + m + t; \
                            a = b + ROTLEFT(a,s); }

/*********************** FUNCTION DEFINITIONS ***********************/
static void  md5_transform(MD5_CTX *ctx, const BYTE data[])
{
    MD5WORD a, b, c, d, m[16], i, j;

    // MD5 specifies big endian byte order, but this implementation assumes a little
    // endian byte order CPU. Reverse all the bytes upon input, and re-reverse them
    // on output (in md5_final()).
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j]) + (data[j + 1] << 8) + (data[j + 2] << 16) + (data[j + 3] << 24);

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];

    FF(a,b,c,d,m[0],  7,0xd76aa478);
    FF(d,a,b,c,m[1], 12,0xe8c7b756);
    FF(c,d,a,b,m[2], 17,0x242070db);
    FF(b,c,d,a,m[3], 22,0xc1bdceee);
    FF(a,b,c,d,m[4],  7,0xf57c0faf);
    FF(d,a,b,c,m[5], 12,0x4787c62a);
    FF(c,d,a,b,m[6], 17,0xa8304613);
    FF(b,c,d,a,m[7], 22,0xfd469501);
    FF(a,b,c,d,m[8],  7,0x698098d8);
    FF(d,a,b,c,m[9], 12,0x8b44f7af);
    FF(c,d,a,b,m[10],17,0xffff5bb1);
    FF(b,c,d,a,m[11],22,0x895cd7be);
    FF(a,b,c,d,m[12], 7,0x6b901122);
    FF(d,a,b,c,m[13],12,0xfd987193);
    FF(c,d,a,b,m[14],17,0xa679438e);
    FF(b,c,d,a,m[15],22,0x49b40821);

    GG(a,b,c,d,m[1],  5,0xf61e2562);
    GG(d,a,b,c,m[6],  9,0xc040b340);
    GG(c,d,a,b,m[11],14,0x265e5a51);
    GG(b,c,d,a,m[0], 20,0xe9b6c7aa);
    GG(a,b,c,d,m[5],  5,0xd62f105d);
    GG(d,a,b,c,m[10], 9,0x02441453);
    GG(c,d,a,b,m[15],14,0xd8a1e681);
    GG(b,c,d,a,m[4], 20,0xe7d3fbc8);
    GG(a,b,c,d,m[9],  5,0x21e1cde6);
    GG(d,a,b,c,m[14], 9,0xc33707d6);
    GG(c,d,a,b,m[3], 14,0xf4d50d87);
    GG(b,c,d,a,m[8], 20,0x455a14ed);
    GG(a,b,c,d,m[13], 5,0xa9e3e905);
    GG(d,a,b,c,m[2],  9,0xfcefa3f8);
    GG(c,d,a,b,m[7], 14,0x676f02d9);
    GG(b,c,d,a,m[12],20,0x8d2a4c8a);

    HH(a,b,c,d,m[5],  4,0xfffa3942);
    HH(d,a,b,c,m[8], 11,0x8771f681);
    HH(c,d,a,b,m[11],16,0x6d9d6122);
    HH(b,c,d,a,m[14],23,0xfde5380c);
    HH(a,b,c,d,m[1],  4,0xa4beea44);
    HH(d,a,b,c,m[4], 11,0x4bdecfa9);
    HH(c,d,a,b,m[7], 16,0xf6bb4b60);
    HH(b,c,d,a,m[10],23,0xbebfbc70);
    HH(a,b,c,d,m[13], 4,0x289b7ec6);
    HH(d,a,b,c,m[0], 11,0xeaa127fa);
    HH(c,d,a,b,m[3], 16,0xd4ef3085);
    HH(b,c,d,a,m[6], 23,0x04881d05);
    HH(a,b,c,d,m[9],  4,0xd9d4d039);
    HH(d,a,b,c,m[12],11,0xe6db99e5);
    HH(c,d,a,b,m[15],16,0x1fa27cf8);
    HH(b,c,d,a,m[2], 23,0xc4ac5665);

    II(a,b,c,d,m[0],  6,0xf4292244);
    II(d,a,b,c,m[7], 10,0x432aff97);
    II(c,d,a,b,m[14],15,0xab9423a7);
    II(b,c,d,a,m[5], 21,0xfc93a039);
    II(a,b,c,d,m[12], 6,0x655b59c3);
    II(d,a,b,c,m[3], 10,0x8f0ccc92);
    II(c,d,a,b,m[10],15,0xffeff47d);
    II(b,c,d,a,m[1], 21,0x85845dd1);
    II(a,b,c,d,m[8],  6,0x6fa87e4f);
    II(d,a,b,c,m[15],10,0xfe2ce6e0);
    II(c,d,a,b,m[6], 15,0xa3014314);
    II(b,c,d,a,m[13],21,0x4e0811a1);
    II(a,b,c,d,m[4],  6,0xf7537e82);
    II(d,a,b,c,m[11],10,0xbd3af235);
    II(c,d,a,b,m[2], 15,0x2ad7d2bb);
    II(b,c,d,a,m[9], 21,0xeb86d391);

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
}

static void *md5_init()
{
    MD5_CTX *ctx;
    ctx = sqlite3_malloc(sizeof(MD5_CTX));
    if (!ctx)
        return NULL;
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    return ctx;
}

static void md5_update(MD5_CTX *ctx, const BYTE data[], unsigned long len)
{
    size_t i;

    for (i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            md5_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

static int md5_final(BYTE hash[], MD5_CTX *ctx)
{
    size_t i;

    i = ctx->datalen;

    // Pad whatever data is left in the buffer.
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    }
    else if (ctx->datalen >= 56) {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        md5_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    // Append to the padding the total message's length in bits and transform.
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[56] = ctx->bitlen;
    ctx->data[57] = ctx->bitlen >> 8;
    ctx->data[58] = ctx->bitlen >> 16;
    ctx->data[59] = ctx->bitlen >> 24;
    ctx->data[60] = ctx->bitlen >> 32;
    ctx->data[61] = ctx->bitlen >> 40;
    ctx->data[62] = ctx->bitlen >> 48;
    ctx->data[63] = ctx->bitlen >> 56;
    md5_transform(ctx, ctx->data);

    // Since this implementation uses little endian byte ordering and MD uses big endian,
    // reverse all the bytes when copying the final state to the output hash.
    for (i = 0; i < 4; ++i) {
        hash[i]      = (ctx->state[0] >> (i * 8)) & 0x000000ff;
        hash[i + 4]  = (ctx->state[1] >> (i * 8)) & 0x000000ff;
        hash[i + 8]  = (ctx->state[2] >> (i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (i * 8)) & 0x000000ff;
    }
    sqlite3_free(ctx);
    return 16;
}
/* End of Inline hash code for md5 */

/***********************************************************************************************/

/* Make sure any Macro's used in this file defined only here are undefined */

#ifdef A00
#undef A00
#endif
#ifdef A01
#undef A01
#endif
#ifdef A02
#undef A02
#endif
#ifdef A03
#undef A03
#endif
#ifdef A04
#undef A04
#endif
#ifdef A10
#undef A10
#endif
#ifdef A11
#undef A11
#endif
#ifdef A12
#undef A12
#endif
#ifdef A13
#undef A13
#endif
#ifdef A14
#undef A14
#endif
#ifdef A20
#undef A20
#endif
#ifdef A21
#undef A21
#endif
#ifdef A22
#undef A22
#endif
#ifdef A23
#undef A23
#endif
#ifdef A24
#undef A24
#endif
#ifdef A30
#undef A30
#endif
#ifdef A31
#undef A31
#endif
#ifdef A32
#undef A32
#endif
#ifdef A33
#undef A33
#endif
#ifdef A34
#undef A34
#endif
#ifdef A40
#undef A40
#endif
#ifdef A41
#undef A41
#endif
#ifdef A42
#undef A42
#endif
#ifdef A43
#undef A43
#endif
#ifdef A44
#undef A44
#endif
#ifdef ADDINC128
#undef ADDINC128
#endif
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif
#ifdef BYTE_ORDER
#undef BYTE_ORDER
#endif
#ifdef Ch
#undef Ch
#endif
#ifdef DOSTORESTATE58
#undef DOSTORESTATE58
#endif
#ifdef DOSTORESTATE65
#undef DOSTORESTATE65
#endif
#ifdef DVMASKSIZE
#undef DVMASKSIZE
#endif
#ifdef F
#undef F
#endif
#ifdef F
#undef F
#endif
#ifdef FF
#undef FF
#endif
#ifdef G
#undef G
#endif
#ifdef G
#undef G
#endif
#ifdef GET
#undef GET
#endif
#ifdef GET
#undef GET
#endif
#ifdef GG
#undef GG
#endif
#ifdef H
#undef H
#endif
#ifdef H
#undef H
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND1_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND1_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND1_STEP_BW
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND2_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND2_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND2_STEP_BW
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND3_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND3_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND3_STEP_BW
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND4_STEP
#undef HASHCLASH_SHA1COMPRESS_ROUND4_STEP
#endif
#ifdef HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW
#undef HASHCLASH_SHA1COMPRESS_ROUND4_STEP_BW
#endif
#ifdef HH
#undef HH
#endif
#ifdef I
#undef I
#endif
#ifdef II
#undef II
#endif
#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif
#ifdef Maj
#undef Maj
#endif
#ifdef MD2_BLOCK_SIZE
#undef MD2_BLOCK_SIZE
#endif
#ifdef MD2_H
#undef MD2_H
#endif
#ifdef MD4OUT
#undef MD4OUT
#endif
#ifdef MD5_BLOCK_SIZE
#undef MD5_BLOCK_SIZE
#endif
#ifdef MD5_H
#undef MD5_H
#endif
#ifdef MEMCPY_BCOPY
#undef MEMCPY_BCOPY
#endif
#ifdef MEMCPY_BCOPY
#undef MEMCPY_BCOPY
#endif
#ifdef MEMSET_BZERO
#undef MEMSET_BZERO
#endif
#ifdef MEMSET_BZERO
#undef MEMSET_BZERO
#endif
#ifdef NOPROTO
#undef NOPROTO
#endif
#ifdef R
#undef R
#endif
#ifdef REVERSE32
#undef REVERSE32
#endif
#ifdef REVERSE64
#undef REVERSE64
#endif
#ifdef ROL64
#undef ROL64
#endif
#ifdef rotate_left
#undef rotate_left
#endif
#ifdef rotate_right
#undef rotate_right
#endif
#ifdef ROTLEFT
#undef ROTLEFT
#endif
#ifdef ROUND256
#undef ROUND256
#endif
#ifdef ROUND256_0_TO_15
#undef ROUND256_0_TO_15
#endif
#ifdef ROUND256_0_TO_15
#undef ROUND256_0_TO_15
#endif
#ifdef ROUND512
#undef ROUND512
#endif
#ifdef ROUND512_0_TO_15
#undef ROUND512_0_TO_15
#endif
#ifdef ROUND512_0_TO_15
#undef ROUND512_0_TO_15
#endif
#ifdef S32
#undef S32
#endif
#ifdef S64
#undef S64
#endif
#ifdef SET
#undef SET
#endif
#ifdef SET
#undef SET
#endif
#ifdef sha1_f1
#undef sha1_f1
#endif
#ifdef sha1_f2
#undef sha1_f2
#endif
#ifdef sha1_f3
#undef sha1_f3
#endif
#ifdef sha1_f4
#undef sha1_f4
#endif
#ifdef SHA1_RECOMPRESS
#undef SHA1_RECOMPRESS
#endif
#ifdef SHA1_STORE_STATE
#undef SHA1_STORE_STATE
#endif
#ifdef SHA2_UNROLL_TRANSFORM
#undef SHA2_UNROLL_TRANSFORM
#endif
#ifdef SHA2_USE_INTTYPES_H
#undef SHA2_USE_INTTYPES_H
#endif
#ifdef SHA2_USE_MEMSET_MEMCPY
#undef SHA2_USE_MEMSET_MEMCPY
#endif
#ifdef SHA256_BLOCK_LENGTH
#undef SHA256_BLOCK_LENGTH
#endif
#ifdef SHA256_DIGEST_LENGTH
#undef SHA256_DIGEST_LENGTH
#endif
#ifdef SHA256_DIGEST_STRING_LENGTH
#undef SHA256_DIGEST_STRING_LENGTH
#endif
#ifdef SHA256_SHORT_BLOCK_LENGTH
#undef SHA256_SHORT_BLOCK_LENGTH
#endif
#ifdef SHA384_BLOCK_LENGTH
#undef SHA384_BLOCK_LENGTH
#endif
#ifdef SHA384_DIGEST_LENGTH
#undef SHA384_DIGEST_LENGTH
#endif
#ifdef SHA384_DIGEST_STRING_LENGTH
#undef SHA384_DIGEST_STRING_LENGTH
#endif
#ifdef SHA384_SHORT_BLOCK_LENGTH
#undef SHA384_SHORT_BLOCK_LENGTH
#endif
#ifdef SHA512_BLOCK_LENGTH
#undef SHA512_BLOCK_LENGTH
#endif
#ifdef SHA512_DIGEST_LENGTH
#undef SHA512_DIGEST_LENGTH
#endif
#ifdef SHA512_DIGEST_STRING_LENGTH
#undef SHA512_DIGEST_STRING_LENGTH
#endif
#ifdef SHA512_SHORT_BLOCK_LENGTH
#undef SHA512_SHORT_BLOCK_LENGTH
#endif
#ifdef Sigma0_256
#undef Sigma0_256
#endif
#ifdef sigma0_256
#undef sigma0_256
#endif
#ifdef sigma0_512
#undef sigma0_512
#endif
#ifdef Sigma0_512
#undef Sigma0_512
#endif
#ifdef sigma1_256
#undef sigma1_256
#endif
#ifdef Sigma1_256
#undef Sigma1_256
#endif
#ifdef Sigma1_512
#undef Sigma1_512
#endif
#ifdef sigma1_512
#undef sigma1_512
#endif
#ifdef STEP
#undef STEP
#endif

/*
** Generic compute hash function.  Algorithm is encoded in the Usr Context Field
*/

SQLITE_PRIVATE void _ComputeHash(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    void *(*InitFunc)() = NULL;
    void (*UpdateFunc)(void *, void *, unsigned long) = NULL;
    int (*FinalFunc)(void *, void *) = NULL;
    void *ctx = NULL;
    int i = 0;
    unsigned long datalen = 0;
    void *data = NULL;
    int hashlen = 0;
    unsigned char hash[128] = {0};;
    int algo = (intptr_t)sqlite3_user_data(context);

    /*
    ** Decode the User Context to determine Algorithm and output format
    ** Set function pointers for the Init/Update/Final methods
    */

    switch (algo)
    {
        case 1: /* Hardened SHA1 */
            InitFunc = (void*)SHA1DCInit;
            UpdateFunc = (void*)SHA1DCUpdate;
            FinalFunc = (void*)SHA1DCFinal;
            algo = 1;
            break;
        case 2: /* MD2 */
            InitFunc = (void*)md2_init;
            UpdateFunc = (void*)md2_update;
            FinalFunc = (void*)md2_final;
            algo = 1;
            break;
        case 4: /* MD4 */
            InitFunc = (void*)MD4_Init;
            UpdateFunc = (void*)MD4_Update;
            FinalFunc = (void*)MD4_Final;
            algo = 1;
            break;
        case 5: /* MD5 */
            InitFunc = (void*)md5_init;
            UpdateFunc = (void*)md5_update;
            FinalFunc = (void*)md5_final;
            algo = 1;
            break;
        case 2256: /* SHA2-256 */
            InitFunc = (void*)SHA256_Init;
            UpdateFunc = (void*)SHA256_Update;
            FinalFunc = (void*)SHA256_Final;
            algo = 1;
            break;
        case 2384: /* SHA2-384 */
            InitFunc = (void*)SHA384_Init;
            UpdateFunc = (void*)SHA384_Update;
            FinalFunc = (void*)SHA384_Final;
            algo = 1;
            break;
        case 2512: /* SHA2-512 */
            InitFunc = (void*)SHA512_Init;
            UpdateFunc = (void*)SHA512_Update;
            FinalFunc = (void*)SHA512_Final;
            algo = 1;
            break;
        default: /* SHA3 */
            if ((algo >= 3128) && (algo <= 3512))
            {
                algo -= 3000;
                ctx = SHA3Init(algo);
                UpdateFunc = (void*)SHA3Update;
                FinalFunc = (void*)SHA3Final;
                algo = 0;
            }
            else
            {
                sqlite3_result_error(context, "Unknown Algorithm", -1);
                return;
            }
    }
    if (algo)
        ctx = InitFunc();
    if (!ctx)
    {
        sqlite3_result_error(context, "Algorithm could not allocate it's context", -1);
        return;
    }

    /*
    ** We now have a valid Algorith and Context initialized so Update the Contents
    ** with the data provided
    */

    for (i = 0; i < argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_BLOB)
            data = (void*)sqlite3_value_blob(argv[i]);
        else
            data = (void*)sqlite3_value_text(argv[i]);
        datalen = sqlite3_value_bytes(argv[i]);
        if (datalen > 0)
            UpdateFunc(ctx, data, datalen);
    }

    /*
    ** Retrieve the hash output Length and Digest, return as blob
    */

    hashlen = FinalFunc(hash, ctx);
    sqlite3_result_blob(context, hash, hashlen, SQLITE_TRANSIENT);
}

/*
** Generic Aggregate Function needs to save the Update and Final functions
** and the Algorithm Context
*/

typedef struct hashingCtx hashingCtx;
struct hashingCtx
{
    void *ctx;
    void (*UpdateFunc)(void *, void *, unsigned long);
    int (*FinalFunc)(void *, void *);
};


SQLITE_PRIVATE void _HashStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    void *(*InitFunc)() = NULL;;
    int i = 0;
    unsigned long datalen = 0;
    void *data = NULL;
    int algo = (intptr_t)sqlite3_user_data(context);
    hashingCtx *p = sqlite3_aggregate_context(context, sizeof(hashingCtx));

    if (!p)
    {
        sqlite3_result_error(context, "Cannot Allocate Aggregate Context", -1);
        return;
    }

    /*
    ** On the first pass (with a new Aggregate Context and thus no Hash Context) decode the
    ** algorithm and save  the Update/Final function pointers and the desired output format
    */

    if (!p->ctx)
    {
        switch (algo)
        {
            case 1: /* Hardened SHA1 */
                InitFunc = (void*)SHA1DCInit;
                p->UpdateFunc = (void*)SHA1DCUpdate;
                p->FinalFunc = (void*)SHA1DCFinal;
                algo = 1;
                break;
            case 2: /* MD2 */
                InitFunc = (void*)md2_init;
                p->UpdateFunc = (void*)md2_update;
                p->FinalFunc = (void*)md2_final;
                algo = 1;
                break;
            case 4: /* MD4 */
                InitFunc = (void*)MD4_Init;
                p->UpdateFunc = (void*)MD4_Update;
                p->FinalFunc = (void*)MD4_Final;
                algo = 1;
                break;
            case 5: /* MD5 */
                InitFunc = (void*)md5_init;
                p->UpdateFunc = (void*)md5_update;
                p->FinalFunc = (void*)md5_final;
                algo = 1;
                break;
            case 2256: /* SHA2-256 */
                InitFunc = (void*)SHA256_Init;
                p->UpdateFunc = (void*)SHA256_Update;
                p->FinalFunc = (void*)SHA256_Final;
                algo = 1;
                break;
            case 2384: /* SHA2-384 */
                InitFunc = (void*)SHA384_Init;
                p->UpdateFunc = (void*)SHA384_Update;
                p->FinalFunc = (void*)SHA384_Final;
                algo = 1;
                break;
            case 2512: /* SHA2-512 */
                InitFunc = (void*)SHA512_Init;
                p->UpdateFunc = (void*)SHA512_Update;
                p->FinalFunc = (void*)SHA512_Final;
                algo = 1;
                break;
            default: /* SHA3 */
                if ((algo >= 3128) && (algo <= 3512))
                {
                    algo -= 3000;
                    p->ctx = SHA3Init(algo);
                    p->UpdateFunc = (void*)SHA3Update;
                    p->FinalFunc = (void*)SHA3Final;
                    algo = 0;
                }
                else
                {
                    sqlite3_result_error(context, "Unknown Algorithm", -1);
                    return;
                }
        }
        if (algo)
            p->ctx = InitFunc();
        if (!p->ctx)
        {
            sqlite3_result_error(context, "Algorithm could not allocate it's context", -1);
            return;
        }
    }

    /*
    ** Update the Hash Algorithm Context with the Data provided
    */

    for (i = 0; i < argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_BLOB)
            data = (void*)sqlite3_value_blob(argv[i]);
        else
            data = (void*)sqlite3_value_text(argv[i]);
        datalen = sqlite3_value_bytes(argv[i]);
        if (datalen > 0)
            p->UpdateFunc(p->ctx, data, datalen);
    }
}


SQLITE_PRIVATE void _HashFinal(sqlite3_context *context)
{
    int hashlen = 0;
    int i = 0;
    hashingCtx *p = sqlite3_aggregate_context(context, 0);
    unsigned char hash[128] = {0};

    if (!p)
    {
        sqlite3_result_null(context);
        return;
    }

    /*
    ** Call the FInalFunc from the Aggregate Context to get the Digest and Length
    ** and return as a blob
    */

    hashlen = p->FinalFunc(hash, p->ctx);
    sqlite3_result_blob(context, hash, hashlen, SQLITE_TRANSIENT);
}


SQLITE_PRIVATE void _hashQueryFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3 *db = sqlite3_context_db_handle(context);
    const char *zSql = (const char*)sqlite3_value_text(argv[0]);
    sqlite3_stmt *pStmt = 0;
    int nCol;                   /* Number of columns in the result set */
    int i;                      /* Loop counter */
    int rc;
    int n;
    const char *z;
    int iSize = 256;
    void *(*InitFunc)() = NULL;
    void (*UpdateFunc)(void *, void *, unsigned long) = NULL;
    int (*FinalFunc)(void *, void *) = NULL;
    void *ctx = NULL;
    unsigned char hash[128] = {0};
    int hashlen = 0;
    int algo = (intptr_t)sqlite3_user_data(context);

    if (zSql==0)
        return;

    /*
    ** Decode the User Context to determine Algorithm and output format
    ** Set function pointers for the Init/Update/Final methods
    */

    if (argc == 2)
    {
        iSize = sqlite3_value_int(argv[1]);
        if (iSize!=224 && iSize!=256 && iSize!=384 && iSize!=512)
        {
            sqlite3_result_error(context, "Hashsize size should be one of: 224 256 "
                                    "384 512", -1);
            return;
        }
    }
    switch (algo)
    {
        case 1: /* Hardened SHA1 */
            InitFunc = (void*)SHA1DCInit;
            UpdateFunc = (void*)SHA1DCUpdate;
            FinalFunc = (void*)SHA1DCFinal;
            algo = 1;
            break;
        case 2: /* MD2 */
            InitFunc = (void*)md2_init;
            UpdateFunc = (void*)md2_update;
            FinalFunc = (void*)md2_final;
            algo = 1;
            break;
        case 4: /* MD4 */
            InitFunc = (void*)MD4_Init;
            UpdateFunc = (void*)MD4_Update;
            FinalFunc = (void*)MD4_Final;
            algo = 1;
            break;
        case 5: /* MD5 */
            InitFunc = (void*)md5_init;
            UpdateFunc = (void*)md5_update;
            FinalFunc = (void*)md5_final;
            algo = 1;
            break;
        case 2256: /* SHA2-256 */
            InitFunc = (void*)SHA256_Init;
            UpdateFunc = (void*)SHA256_Update;
            FinalFunc = (void*)SHA256_Final;
            algo = 1;
            break;
        case 2384: /* SHA2-384 */
            InitFunc = (void*)SHA384_Init;
            UpdateFunc = (void*)SHA384_Update;
            FinalFunc = (void*)SHA384_Final;
            algo = 1;
            break;
        case 2512: /* SHA2-512 */
            InitFunc = (void*)SHA512_Init;
            UpdateFunc = (void*)SHA512_Update;
            FinalFunc = (void*)SHA512_Final;
            algo = 1;
            break;
        default: /* SHA3 */
            if ((algo >= 3128) && (algo <= 3512))
            {
                ctx = SHA3Init(iSize);
                UpdateFunc = (void*)SHA3Update;
                FinalFunc = (void*)SHA3Final;
                algo = 0;
            }
            else
            {
                sqlite3_result_error(context, "Unknown Algorithm", -1);
                return;
            }
    }
    if (algo)
        ctx = InitFunc();
    if (!ctx)
    {
        sqlite3_result_error(context, "Algorithm could not allocate it's context", -1);
        return;
    }

    /* Now that we have a context and the function pointers setup, execute the SQL.
    ** retrieve the data, format and hash
    */

    while (zSql[0])
    {
        rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zSql);
        if (rc)
        {
            char *zMsg = sqlite3_mprintf("error SQL statement [%s]: %s",
                                          zSql, sqlite3_errmsg(db));
            sqlite3_finalize(pStmt);
            sqlite3_result_error(context, zMsg, -1);
            sqlite3_free(zMsg);
            sqlite3_free(ctx);
            return;
        }
        if (!sqlite3_stmt_readonly(pStmt))
        {
            char *zMsg = sqlite3_mprintf("non-query: [%s]", sqlite3_sql(pStmt));
            sqlite3_finalize(pStmt);
            sqlite3_result_error(context, zMsg, -1);
            sqlite3_free(zMsg);
            sqlite3_free(ctx);
            return;
        }
        {
            int n2;
            char zBuf[50];
            nCol = sqlite3_column_count(pStmt);
            z = sqlite3_sql(pStmt);
            if (!z)
            {
                n2 = (int)strlen(z);
                snprintf(zBuf, 50, "S%d:", n2);
                n = (int)strlen(zBuf);
                UpdateFunc(ctx, zBuf, n);
                UpdateFunc(ctx,(void*)z, n2);
            }
        }

        /* Compute a hash over the result of the query */

        while (SQLITE_ROW==sqlite3_step(pStmt))
        {
            UpdateFunc(ctx, "R", 1);
            for (i=0; i<nCol; i++)
            {
                switch (sqlite3_column_type(pStmt,i))
                {
                    case SQLITE_NULL:
                    {
                        UpdateFunc(ctx, "N", 1);
                        break;
                    }
                    case SQLITE_INTEGER:
                    {
                        sqlite3_uint64 u;
                        int j;
                        unsigned char x[9];
                        sqlite3_int64 v = sqlite3_column_int64(pStmt,i);
                        memcpy(&u, &v, 8);
                        for (j=8; j>=1; j--)
                        {
                            x[j] = u & 0xff;
                            u >>= 8;
                        }
                        x[0] = 'I';
                        UpdateFunc(ctx, x, 9);
                        break;
                    }
                    case SQLITE_FLOAT:
                    {
                        sqlite3_uint64 u;
                        int j;
                        unsigned char x[9];
                        double r = sqlite3_column_double(pStmt,i);
                        memcpy(&u, &r, 8);
                        for (j=8; j>=1; j--)
                        {
                            x[j] = u & 0xff;
                            u >>= 8;
                        }
                        x[0] = 'F';
                        UpdateFunc(ctx, x, 9);
                        break;
                    }
                    case SQLITE_TEXT:
                    {
                        int n2 = sqlite3_column_bytes(pStmt, i);
                        unsigned char *z2 = (void*)sqlite3_column_text(pStmt, i);
                        char zBuf[50];
                        snprintf(zBuf, 50, "T%d:", n2);
                        n = (int)strlen(zBuf);
                        UpdateFunc(ctx, zBuf, n);
                        UpdateFunc(ctx, z2, n2);
                        break;
                    }
                    case SQLITE_BLOB:
                    {
                        int n2 = sqlite3_column_bytes(pStmt, i);
                        unsigned char *z2 = (void*)sqlite3_column_blob(pStmt, i);
                        char zBuf[50];
                        snprintf(zBuf, 50, "B%d:", n2);
                        n = (int)strlen(zBuf);
                        UpdateFunc(ctx, zBuf, n);
                        UpdateFunc(ctx, z2, n2);
                        break;
                    }
                }
            }
        }
        sqlite3_finalize(pStmt);
    }

    /*
    ** Retrieve the hash digest and length, and return as a blob
    */

    hashlen = FinalFunc(hash, ctx);
    sqlite3_result_blob(context, hash, hashlen, SQLITE_TRANSIENT);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlhash_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

    /*
    **  Scalar Functions
    */

    nErr += sqlite3_create_function(db, "md2",         -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "md4",         -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)4,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "md5",         -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)5,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha1",        -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)1,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha256",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2256,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha384",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2384,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha512",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2512,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha2_256",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2256,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha2_384",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2384,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha2_512",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2512,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha3_224",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3224,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha3_256",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3256,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha3_384",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3384,  _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha3_512",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3512,  _ComputeHash, 0, 0);

    /*
    ** Query Functions
    */

    nErr += sqlite3_create_function(db, "md2_query",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2,     _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "md4_query",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)4,     _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "md5_query",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)5,     _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha1_query",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)1,     _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha256_query", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2256,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha384_query", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2384,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha512_query", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2512,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha2_256_query", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2256,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha2_384_query", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2384,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha2_512_query", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)2512,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha3_query",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)3512,  _hashQueryFunc, 0, 0);
    nErr += sqlite3_create_function(db, "sha3_query",   2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_DIRECTONLY,  (void*)3512,  _hashQueryFunc, 0, 0);

    /*
    ** Aggregate Functions
    */

    nErr += sqlite3_create_function(db, "aggmd2",         -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggmd4",         -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)4,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggmd5",         -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)5,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha1",        -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)1,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha256",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2256,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha384",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2384,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha512",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2512,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha2_256",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2256,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha2_384",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2384,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha2_512",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)2512,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha3_224",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3224,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha3_256",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3256,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha3_384",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3384,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "aggsha3_512",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)3512,  0, _HashStep, _HashFinal);

    /*
    */

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

#ifdef __cplusplus
}
#endif

