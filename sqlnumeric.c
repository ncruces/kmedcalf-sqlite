/*
 * Implements ToBestType and strtoll SQL Function
 *
 * Takes one value parameter and returns the Best Type value
 *
 * NULL returns NULL
 * TEXT/BLOB that "looks like a number" returns the number as Integer or Real as appropriate
 * TEXT/BLOB that is zero-length returns NULL
 * Otherwise returns what was given
 *
 * Written 2020 by Keith Medcalf
 * Given to the Public Domain.  If you break it, you own all the pieces.
 */

#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#undef LONGDOUBLE_CONSTANT
#undef LONGDOUBLE_TYPE
#if defined(__GNUC__) && defined(_WIN64)
#include <quadmath.h>
#define LONGDOUBLE_TYPE __float128
#define LONGDOUBLE_CONSTANT(x) x##Q
#define modfl modfq
#define strtold strtoflt128
#else
#define LONGDOUBLE_TYPE long double
#define LONGDOUBLE_CONSTANT(x) x##L
#endif

/*
**
** Determine affinity of field
**
** ignore leading space
** then may have + or -
** then may have digits or . (if . found then type=real)
** then may have digits (if another . then not number)
** then may have e (if found then type=real)
** then may have + or -
** then may have digits
** then may have trailing space
*/
static int _isValidNumber(char *arg)
{
    char *start;
    char *stop;
    int isValid = 0;
    int hasDigit = 0;

    start = arg;
    stop = arg + strlen(arg) - 1;
    while (start <= stop && *start==' ')                // strip spaces from begining
    {
        start++;
    }
    while (start <= stop && *stop==' ')                 // strip spaces from end
    {
        stop--;
    }
    if (start > stop)
    {
        goto _end_isValidNumber;
    }
    if (start <= stop && (*start=='+' || *start=='-'))  // may have + or -
    {
        start++;
    }
    if (start <= stop && isdigit(*start))               // must have a digit to be valid
    {
        hasDigit = 1;
        isValid = 1;
    }
    while (start <= stop && isdigit(*start))            // bunch of digits
    {
        start++;
    }
    if (start <= stop && *start=='.')                   // may have .
    {
        isValid = 2;
        start++;
    }
    if (start <= stop && isdigit(*start))
    {
        hasDigit = 1;
    }
    while (start <= stop && isdigit(*start))            // bunch of digits
    {
        start++;
    }
    if (!hasDigit)                                      // no digits then invalid
    {
        isValid = 0;
        goto _end_isValidNumber;
    }
    if (start <= stop && (*start=='e' || *start=='E'))  // may have 'e' or 'E'
    {
        isValid = 3;
        start++;
    }
    if (start <= stop && isValid == 3 && (*start == '+' || *start == '-'))
    {
        start++;
    }
    if (start <= stop && isValid == 3 && isdigit(*start))
    {
        isValid = 2;
    }
    while (start <= stop && isdigit(*start))            // bunch of digits
    {
        start++;
    }
    if (isValid == 3)
    {
        isValid = 0;
    }
_end_isValidNumber:
    if (start <= stop)
    {
        isValid = 0;
    }
    return isValid;
}


static void _ToBestType(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    static int hasExtended = 0;
    LONGDOUBLE_TYPE dv, fp, ip;
    char *pCur;
    int fromfloat = 0;

    switch (sqlite3_value_type(argv[0]))
    {
        case SQLITE_NULL:
        {
            return;
        }
        case SQLITE_BLOB:
        {
            if (sqlite3_value_bytes(argv[0]) > 0)
                sqlite3_result_value(ctx, argv[0]);
            return;
        }
        case SQLITE_INTEGER:
        {
            sqlite3_result_value(ctx, argv[0]);
            return;
        }
        case SQLITE_TEXT:
        {
            pCur = (char*)sqlite3_value_text(argv[0]);
            switch (_isValidNumber(pCur))
            {
                case 0:
                {
                    sqlite3_result_value(ctx, argv[0]);
                    return;
                }
                case 1:
                {
                    sqlite3_int64 ival;

                    ival = strtoll(pCur, 0, 10);
                    if (ival > LLONG_MIN && ival < LLONG_MAX) {
                        sqlite3_result_int64(ctx, ival);
                        return;
                    }
                }
                case 2:
                {
                    dv = strtold(pCur, 0);
                    fromfloat = 0;
                    break;
                }
            }
        }
        case SQLITE_FLOAT:
        {
            dv = sqlite3_value_double(argv[0]);
            fromfloat = 1;
            break;
        }
    }

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

    fp = modfl(dv, &ip);
    if (hasExtended<0 || fromfloat)
    {
        if (fp==0.0L && ip >= -9007199254740991LL && dv <= 9007199254740991LL)
        {
            sqlite3_result_int64(ctx, (long long)ip);
        }
        else
        {
            sqlite3_result_double(ctx, dv);
        }
    }
    else
    {
        if (fp==0.0L && ip >= LLONG_MIN && ip <= LLONG_MAX)
        {
            sqlite3_result_int64(ctx, (long long)ip);
        }
        else
        {
            sqlite3_result_double(ctx, dv);
        }
    }
}

static void _strtoll_(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    sqlite_int64 result;
    int base = 0;
    char* start;
    char* stop;

    if (argc < 1 || argc > 2)
    {
        sqlite3_result_error(ctx, "Must have 1 or 2 parameters, second (base) defaults to 0", -1);
        return;
    }
    if (sqlite3_value_type(argv[0]) != SQLITE_TEXT)
        return;
    start = (char*)sqlite3_value_text(argv[0]);
    if (*start == 0)
        return;
    if (argc == 2)
        base = sqlite3_value_int(argv[1]);
    if (base<0 || base>36)
        return;
    result = strtoll(start, (char**)&stop, base);
    if (start == stop)
        return;
    sqlite3_result_int64(ctx, result);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlnumeric_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc;
    SQLITE_EXTENSION_INIT2(pApi);
    rc = sqlite3_create_function(db, "ToBestType", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _ToBestType, 0, 0);
    rc += sqlite3_create_function(db, "strtoll", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _strtoll_, 0, 0);
    if (rc)
        return SQLITE_ERROR;
    else
        return SQLITE_OK;
}

#undef modfl
#undef strtold

