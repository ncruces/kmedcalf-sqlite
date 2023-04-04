
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

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

SQLITE_PRIVATE void _mathtest_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    static int hasExtended = 0;
    intptr_t flag = (intptr_t)sqlite3_user_data(context);
    int rc = 0;

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
    if (flag == 0 || flag == 2)
        rc = (hasExtended == 1);
#if !defined(SQLITE_CORE)
    else
        rc = -1;
#endif
#if defined(SQLITE_CORE) && defined(__GNUC__) && defined(_WIN64)
    if (flag == 1 || flag == 2)
        rc = 1;
#endif
#if defined(SQLITE_CORE) && defined(SQLITE_USE_QUADMATH) && SQLITE_USE_QUADMATH == 1
    if (flag == 3)
        rc == 1;
#endif
    if (rc > -1)
        sqlite3_result_int(context, rc);
}


SQLITE_PRIVATE void _fpclassify_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int mask = (intptr_t)sqlite3_user_data(context);
#if defined(_MSC_VER) && !defined(__GNUC__)
    int class = fpclassify(sqlite3_value_double(argv[0]));
#else
    int class = fpclass(sqlite3_value_double(argv[0]));
#endif
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
#if defined(_MSC_VER) && !defined(__GNUC__)
    sqlite3_result_int(context, mask == 0 ? class : class == mask);
#else
    sqlite3_result_int(context, mask == 0 ? class : (class & mask) != 0);
#endif
}

#if defined(_MSC_VER) && !defined(__GNUC__)
SQLITE_PRIVATE void _isNZero_(sqlite3_context * context, int argc, sqlite3_value **argv)
{
    double r = sqlite3_value_double(argv[0]);
    sqlite_int64 flag = (intptr_t)sqlite3_user_data(context);
    sqlite_int64 a;
    int e;

    memcpy(&a, &r, sizeof(a));
    if (!a)
        e = 0;
    else
        e = a>>52;
    if (r == 0.0)
        sqlite3_result_int(context, flag ? e == -2048 : e == 0);
    else
        sqlite3_result_int(context, 0);
}

SQLITE_PRIVATE void _fpPclassify_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int mask = (intptr_t)sqlite3_user_data(context);
    int class = fpclassify(sqlite3_value_double(argv[0]));
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    if (sqlite3_value_double(argv[0]) > 0)
        sqlite3_result_int(context, mask == 0 ? class : class == mask);
    else
        sqlite3_result_int(context, 0);
}

SQLITE_PRIVATE void _fpNclassify_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int mask = (intptr_t)sqlite3_user_data(context);
    int class = fpclassify(sqlite3_value_double(argv[0]));
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    if (sqlite3_value_double(argv[0]) < 0)
        sqlite3_result_int(context, mask == 0 ? class : class == mask);
    else
        sqlite3_result_int(context, 0);
}
#endif


SQLITE_PRIVATE void _debugslug_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    printf("%s\n", sqlite3_value_text(argv[1]));
    sqlite3_result_value(context, argv[0]);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlmathtest_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

    nErr += sqlite3_create_function(db, "hasLongDouble", 0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)0, _mathtest_,  0, 0);
    nErr += sqlite3_create_function(db, "hasFloat128",   0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)1, _mathtest_,  0, 0);
    nErr += sqlite3_create_function(db, "hasPrecision",  0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)2, _mathtest_,  0, 0);
    nErr += sqlite3_create_function(db, "hasQuadmath",   0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)3, _mathtest_,  0, 0);

    nErr += sqlite3_create_function(db, "debugSlug",     2, SQLITE_UTF8|SQLITE_INNOCUOUS,                      (void*)0, _debugslug_, 0, 0);

#if defined(_MSC_VER) && !defined(__GNUC__)
    nErr += sqlite3_create_function(db, "fpclassify",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)0,          _fpclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isDenormal",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)-2,         _fpclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isInfinity",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_INFINITE,_fpclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isNormal",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NORMAL,  _fpclassify_, 0, 0);

    nErr += sqlite3_create_function(db, "isPInfinity",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_INFINITE,_fpPclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isNInfinity",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_INFINITE,_fpNclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isPZero",       1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)0,          _isNZero_,     0, 0);
    nErr += sqlite3_create_function(db, "isNZero",       1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)1,          _isNZero_,     0, 0);
    nErr += sqlite3_create_function(db, "isNDenormal",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)-2,         _fpNclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isPDenormal",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)-2,         _fpPclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isPNormal",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NORMAL,  _fpPclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isNNormal",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NORMAL,  _fpNclassify_, 0, 0);
#else
    nErr += sqlite3_create_function(db, "fpclassify",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)0,          _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isDenormal",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)(FP_PDENORM|FP_NDENORM), _fpclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isInfinity",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)(FP_PINF|FP_NINF),       _fpclassify_, 0, 0);
    nErr += sqlite3_create_function(db, "isNormal",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)(FP_PNORM|FP_NNORM),     _fpclassify_, 0, 0);

    nErr += sqlite3_create_function(db, "isPInfinity",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_PINF,    _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isNInfinity",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NINF,    _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isNZero",       1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NZERO,   _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isPZero",       1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_PZERO,   _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isNDenormal",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NDENORM, _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isPDenormal",   1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_PDENORM, _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isPNormal",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_PNORM,   _fpclassify_,  0, 0);
    nErr += sqlite3_create_function(db, "isNNormal",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)FP_NNORM,   _fpclassify_,  0, 0);
#endif

    return nErr > 0 ? 1 : 0;
}

