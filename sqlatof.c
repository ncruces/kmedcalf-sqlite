
#ifdef SQLITE_CORE
    #include "sqlite3.h"
#else
    #ifdef _HAVE_SQLITE_CONFIG_H
        #include "config.h"
    #endif
    #include "sqlite3ext.h"
    SQLITE_EXTENSION_INIT1
#endif
#ifndef SQLITE_PRIVATE
    #define SQLITE_PRIVATE static
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#undef LONGDOUBLE_CONSTANT
#undef LONGDOUBLE_TYPE
#if defined(__GNUC__) && defined(_WIN64)
#include <quadmath.h>
#define LONGDOUBLE_TYPE __float128
#define LONGDOUBLE_CONSTANT(x) x##Q
#else
#define LONGDOUBLE_TYPE long double
#define LONGDOUBLE_CONSTANT(x) x##L
#endif

#ifndef SQLITE_OMIT_FLOATING_POINT
static double __atof__(char* string) {
    LONGDOUBLE_TYPE fraction;       // LONGDOUBLE_TYPE fraction mantissa
    LONGDOUBLE_TYPE dblExp;         // LONGDOUBLE_TYPE adjusted exponent
    LONGDOUBLE_TYPE *d;             // powersOf10 pointer
    sqlite_uint64 frac = 0;         // mantissa value
    int sign = 1;                   // sign of the mantissa
    int expSign = 1;                // sign of the exponent
    int exp = 0;                    // input exponent
    int decPt = -1;                 // mantissa digits before decimal point
    int fracExp;                    // exponent due to decimal point
    int mantSize = 0;               // digits in mantissa.
    register char *p, c;            // character iterator

static LONGDOUBLE_TYPE powersOf10[9];   // powers of ten array
static int maxExponent = 511;       // Largest possible base 10 exponent.
static int potInitialize = 0;       // powersOf10 initialization flag

    // Initialize our powersOf10 array if not initialized

    if (!potInitialize) {
        fraction = LONGDOUBLE_CONSTANT(10.0);
        for(int i=0; i<9; i++) {
            powersOf10[i] = fraction;
            fraction *= fraction;
        }
    }
    potInitialize = 1;

    //  Skip leading blanks and process sign

    p = string;
    while (isspace(*p)) p++;
    if (*p == '-') {
        sign = -1;
        p++;
    } else {
        if (*p == '+') p++;
    }

    //  Process the mantissa digits and decimal point

    while (*p && frac <= 1844674407370955160ull ) {
        mantSize++;
        c = *p++;
        if (c == '.' && decPt < 0) {
            decPt = mantSize;
            continue;
        }
        if (!isdigit(c)) {
            mantSize--;
            p--;
            break;
        }
        frac = 10*frac + (c - '0');
    }
    if (!frac) return sign*0.0;
    fraction = (LONGDOUBLE_TYPE)frac;

    // Compute the exponent due to decimal point location

    if (decPt < 0) {
        for (fracExp=0; isdigit(*p); p++) fracExp++;
        if (*p == '.') p++;
    }
    else
        fracExp = decPt - mantSize;

    // Skip over any extra digits

    while (isdigit(*p)) p++;

    //  Process the exponent

    if ((*p == 'E') || (*p == 'e')) {
        p++;
        if (*p == '-') {
            expSign = -1;
            p++;
        } else {
            if (*p == '+') {
                p++;
            }
        }
        while (isdigit(*p)) {
            exp = exp * 10 + (*p - '0');
            p++;
        }
    }

    // Correct the exponent for decimal point in mantissa

    exp = fracExp + expSign*exp;
    if (exp < 0) {
        expSign = -1;
        exp = -exp;
    } else {
        expSign = 1;
    }
    if (exp > maxExponent) {
        exp = maxExponent;
    }

    //  Compute our multiplier

    dblExp = 1.0;
    for (d = powersOf10; exp != 0; exp >>= 1, d++) {
        if (exp & 1) {
            dblExp *= *d;
        }
    }

    //  Apply multiplier to the fraction

    if (expSign<0) {
        fraction /= dblExp;
    } else {
        fraction *= dblExp;
    }

    // Apply the sign and return result

    return (double)(sign*fraction);
}


SQLITE_PRIVATE void _sqlatof_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    static int hasExtended = 0;

#if defined(__GNUC__) && defined(_WIN64)
    if (!hasExtended) hasExtended = 1;
#else
    if (!hasExtended) {
        if (sizeof(long double) > sizeof(double)) {
            volatile unsigned long long i = ULLONG_MAX;
            volatile long double l;
            volatile double d;
            l = i;
            d = i;
            hasExtended = (d == l) ? -1 : 1;
        } else {
            hasExtended = -1;
        }
    }
#endif

    // Use fast extended precision if avaialble otherwise use c library function

    if (hasExtended>0)
        sqlite3_result_double(context, __atof__((char*)sqlite3_value_text(argv[0])));
    else
        sqlite3_result_double(context, strtod((char*)sqlite3_value_text(argv[0]), 0));
}


SQLITE_PRIVATE void _sqlftoa_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    static int hasExtended = 0;
    char txt[32];

#if defined(SQLITE_INTERNAL_FLOAT128)
    if (!hasExtended) hasExtended = 1;
#else
    if (!hasExtended) {
        if (sizeof(long double) > sizeof(double)) {
            volatile unsigned long long i = ULLONG_MAX;
            volatile long double l;
            volatile double d;
            l = i;
            d = i;
            hasExtended = (d == l) ? -1 : 1;
        } else {
            hasExtended = -1;
        }
    }
#endif

    // Use internal routine if extended precision available else use c library function

    if (hasExtended>0)
        sqlite3_snprintf(32, txt, "%!.17g", sqlite3_value_double(argv[0]));
    else
        snprintf(txt, 32, "%.17g", sqlite3_value_double(argv[0]));
    sqlite3_result_text(context, txt, -1, SQLITE_TRANSIENT);
}
#endif

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlatof_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

#ifndef SQLITE_OMIT_FLOATING_POINT
    nErr += sqlite3_create_function(db, "atof", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _sqlatof_, 0, 0);
    nErr += sqlite3_create_function(db, "ftoa", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _sqlftoa_, 0, 0);
#endif

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

