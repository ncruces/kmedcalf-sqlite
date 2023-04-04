#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <string.h>

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

#undef LONGDOUBLE_CONSTANT
#undef LONGDOUBLE_TYPE
#if defined(__GNUC__) && defined(_WIN64)
#include <quadmath.h>
#define LONGDOUBLE_TYPE __float128
#define LONGDOUBLE_CONSTANT(x) x##Q
#define acosl acosq
#define asinl asinq
#define atan2l atan2q
#define atanl atanq
#define acoshl acoshq
#define asinhl asinhq
#define atanhl atanhq
#define ceill ceilq
#define coshl coshq
#define cosl cosq
#define expl expq
#define floorl floorq
#define fmodl fmodq
#define hypotl hypotq
#define log10l log10q
#define log2l log2q
#define logl logq
#define powl powq
#define remainderl remainderq
#define sinhl sinhq
#define sinl sinq
#define sqrtl sqrtq
#define tanhl tanhq
#define tanl tanq
#else
#define LONGDOUBLE_TYPE long double
#define LONGDOUBLE_CONSTANT(x) x##L
#endif

/*
** Hooks into the MSVC/GCC Runtime
** Return Constants, call Runtime Functions
*/

#ifndef SQLITE_OMIT_FLOATING_POINT

typedef double (*dfdPtr)(double);
typedef double (*dfddPtr)(double, double);
typedef double (*dfidPtr)(int, double);
typedef double (*dfdiPtr)(double, int);

typedef LONGDOUBLE_TYPE (*lflPtr)(LONGDOUBLE_TYPE);
typedef LONGDOUBLE_TYPE (*lfllPtr)(LONGDOUBLE_TYPE, LONGDOUBLE_TYPE);
typedef LONGDOUBLE_TYPE (*lfilPtr)(int, LONGDOUBLE_TYPE);
typedef LONGDOUBLE_TYPE (*lfliPtr)(LONGDOUBLE_TYPE, int);

SQLITE_PRIVATE LONGDOUBLE_TYPE m_deg2rad, m_rad2deg, m_pi, m_pi_2, m_pi_4, m_e, m_ln2, m_ln10, m_log2e, m_log10e;
SQLITE_PRIVATE LONGDOUBLE_TYPE m_1_pi, m_2_pi, m_2_sqrtpi, m_sqrt2, m_sqrt1_2;

/*
** Return Mathematical Constants
*/

SQLITE_PRIVATE void _lfc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, *((LONGDOUBLE_TYPE*)sqlite3_user_data(context)));
}

/*
** Dispatch to Math Library (Runtime) Functions
*/

SQLITE_PRIVATE void _dfd(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(dfdPtr)sqlite3_user_data(context))(sqlite3_value_double(argv[0])) );
}

SQLITE_PRIVATE void _dfdd(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(dfddPtr)sqlite3_user_data(context))(sqlite3_value_double(argv[0]),sqlite3_value_double(argv[1])));
}

SQLITE_PRIVATE void _dfid(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(dfidPtr)sqlite3_user_data(context))(sqlite3_value_int(argv[0]),sqlite3_value_double(argv[1])));
}

SQLITE_PRIVATE void _dfdi(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(dfdiPtr)sqlite3_user_data(context))(sqlite3_value_double(argv[0]),sqlite3_value_int(argv[1])));
}

SQLITE_PRIVATE void _lfl(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(lflPtr)sqlite3_user_data(context))((LONGDOUBLE_TYPE)sqlite3_value_double(argv[0])) );
}

SQLITE_PRIVATE void _lfll(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(lfllPtr)sqlite3_user_data(context))((LONGDOUBLE_TYPE)sqlite3_value_double(argv[0]),(LONGDOUBLE_TYPE)sqlite3_value_double(argv[1])));
}

SQLITE_PRIVATE void _lfil(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(dfidPtr)sqlite3_user_data(context))(sqlite3_value_int(argv[0]),(LONGDOUBLE_TYPE)sqlite3_value_double(argv[1])));
}

SQLITE_PRIVATE void _lfli(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, (*(dfdiPtr)sqlite3_user_data(context))((LONGDOUBLE_TYPE)sqlite3_value_double(argv[0]),sqlite3_value_int(argv[1])));
}

SQLITE_PRIVATE void _logFunc2(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, log(sqlite3_value_double(argv[1])) / log(sqlite3_value_double(argv[0])));
}

SQLITE_PRIVATE void _logFunc2l(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, logl((LONGDOUBLE_TYPE)sqlite3_value_double(argv[1])) / logl((LONGDOUBLE_TYPE)sqlite3_value_double(argv[0])));
}


/*
** Convert Text Argument which is hex string into decimal
*/

SQLITE_PRIVATE void _FromHex(sqlite3_context* context, int argc, sqlite3_value **argv)
{
    const char * src;
    unsigned long long value = 0;

    if (sqlite3_value_type(argv[0]) == SQLITE_TEXT)
    {
        src = sqlite3_value_text(argv[0]);
        if (sqlite3_value_bytes(argv[0]) > 16)
            return;
        while (*src != 0)
        {
            if (!isxdigit(*src))
                return;
            value = value << 4;
            value += (*src > '9' ? ((*src & 0x0f) + 9) : (*src & 0x0f));
            src++;
        }
        sqlite3_result_int64(context, (sqlite_int64)value);
    }
}


/*
** Specific Mathematical Functions
*/

SQLITE_PRIVATE void _TestNull(sqlite3_context * context, int argc, sqlite3_value **argv)
{
    if (sqlite3_value_type(argv[0]) != SQLITE_NULL)
        sqlite3_result_value(context, argv[0]);
    else
        sqlite3_result_error(context, "ErrorIfNull value is NULL", -1);
}

#if !defined(SQLITE_ENABLE_MATH_FUNCTIONS)
SQLITE_PRIVATE void _sign(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    double x = sqlite3_value_double(argv[0]);
    int res = 0;
    if (x < 0)
        res = -1;
    else if (x > 0)
        res = 1;
    sqlite3_result_int(context, res);
}
#endif

SQLITE_PRIVATE void _nexttowards_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    if (argc == 2)
        sqlite3_result_double(context, nextafter(sqlite3_value_double(argv[0]), sqlite3_value_double(argv[1])));
    else
        sqlite3_result_double(context, nextafter(sqlite3_value_double(argv[0]), (intptr_t)sqlite3_user_data(context) ? DBL_MAX : -DBL_MAX));
}

SQLITE_PRIVATE void _mantissa(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int exponent;
    sqlite3_result_double(context, frexp(sqlite3_value_double(argv[0]), &exponent));
}

SQLITE_PRIVATE void _exponent(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int exponent;
    double mantissa = frexp(sqlite3_value_double(argv[0]), &exponent);
    sqlite3_result_int(context, exponent);
}

SQLITE_PRIVATE void _fracpart(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    double ipart, fpart;

    fpart = modf(sqlite3_value_double(argv[0]), &ipart);
    sqlite3_result_double(context, fpart);
}

SQLITE_PRIVATE void _intpart(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    double ipart, fpart;

    fpart = modf(sqlite3_value_double(argv[0]), &ipart);
    sqlite3_result_double(context, ipart);
}

SQLITE_PRIVATE void _radians(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, sqlite3_value_double(argv[0]) * m_deg2rad);
}

SQLITE_PRIVATE void _degrees(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, sqlite3_value_double(argv[0]) * m_rad2deg);
}
#endif

/*
** Test (Set/Clear) Set/Clear bits
*/

/*
 * isset(value, bit, bit, bit ...)
 * return true if all bits are set in value
 */
SQLITE_PRIVATE void _isset(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 value = sqlite3_value_int64(argv[0]);
    sqlite_uint64 mask = 0;
    int bit, i;
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    for (i=1; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            mask |= (1ULL << bit);
    }
    sqlite3_result_int(context, (value & mask) == mask);
}

/*
 * isclr(value, bit, bit, bit ...)
 * return true if all bits are clr in value
 */
SQLITE_PRIVATE void _isclr(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 value = sqlite3_value_int64(argv[0]);
    sqlite_uint64 mask = 0;
    int bit, i;
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    for (i=1; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            mask |= (1ULL << bit);
    }
    sqlite3_result_int(context, (value & mask) == 0ULL);
}

/*
 * ismaskset(value, mask)
 * return true if all set bits in mask set in value
 */
SQLITE_PRIVATE void _ismaskset(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 value = sqlite3_value_int64(argv[0]);
    sqlite_uint64 mask = sqlite3_value_int64(argv[1]);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    if (sqlite3_value_type(argv[1]) == SQLITE_NULL) return;
    sqlite3_result_int(context, (value & mask) == mask);
}

/*
 * ismaskclr(value, mask)
 * return true if all set bits set in mask are clr in value
 */
SQLITE_PRIVATE void _ismaskclr(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 value = sqlite3_value_int64(argv[0]);
    sqlite_uint64 mask = sqlite3_value_int64(argv[1]);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    if (sqlite3_value_type(argv[1]) == SQLITE_NULL) return;
    sqlite3_result_int(context, (value & mask) == 0ULL);
}

/*
 * bitmask(bit, bit, bit ...)
 * return value of bitmask with bits set
 */
SQLITE_PRIVATE void _bitmask(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 mask = 0;
    int bit, i;
    for (i=0; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            mask |= (1ULL << bit);
    }
    sqlite3_result_int64(context, (sqlite_int64)mask);
}

/* setbits(value, bit, bit, ...)
 * return value with bits set
 */
SQLITE_PRIVATE void _setbits(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 value = sqlite3_value_int64(argv[0]);
    int bit, i;
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    for (i=1; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            value |= (1ULL << bit);
    }
    sqlite3_result_int64(context, value);
}

/* clrbits(value, bit, bit, ...)
 * return value with bits cleared
 */
SQLITE_PRIVATE void _clrbits(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 value = sqlite3_value_int(argv[0]);
    int bit, i;
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL) return;
    for (i=1; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            value &= ~(1ULL << bit);
    }
    sqlite3_result_int64(context, value);
}

/*
** bitmask aggregate -- set bits and return resulting mask
*/

SQLITE_PRIVATE void _bitmaskFinal(sqlite3_context *context)
{
    sqlite_uint64 *mask = sqlite3_aggregate_context(context, sizeof(sqlite_uint64));
    sqlite3_result_int64(context, *mask);
}

SQLITE_PRIVATE void _bitmaskStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 *mask = sqlite3_aggregate_context(context, sizeof(sqlite_uint64));
    int bit, i;
    for (i=0; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            *mask |= (1ULL << bit);
    }
}

SQLITE_PRIVATE void _bitmaskInverse(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_uint64 *mask = sqlite3_aggregate_context(context, sizeof(sqlite_uint64));
    int bit, i;
    for (i=0; i<argc; i++)
    {
        if (sqlite3_value_type(argv[i]) == SQLITE_NULL)
            continue;
        bit = sqlite3_value_int(argv[i]);
        if ((bit >= 0) && (bit <= 63))
            *mask &= ~(1ULL << bit);
    }
}

SQLITE_PRIVATE void _TruthValue_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    if (sqlite3_value_type(argv[0]) != SQLITE_NULL)
    {
        if (sqlite3_value_double(argv[0]))
        {
            sqlite3_result_int(context, 1);
        }
        else
        {
            sqlite3_result_int(context, 0);
        }
    }
    else
    {
        sqlite3_result_null(context);
    }
}

/*
** If compiling as a builtin extension, don't export the initializer -- make it SQLITE_PRIVATE
** Change name of initializer to sqlite3_init_<nameOfExtension>
*/

/* SQLite invokes this routine once when it loads the extension.
** Create new functions, collating sequences, and virtual table
** modules here.  This is usually the only exported symbol in
** the shared library.
*/

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlmath_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

#ifndef SQLITE_OMIT_FLOATING_POINT

    nErr += sqlite3_create_function(db, "m_e",          0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_e,           _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_log2e",      0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_log2e,       _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_log10e",     0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_log10e,      _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_ln2",        0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_ln2,         _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_ln10",       0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_ln10,        _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_pi",         0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_pi,          _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_pi_2",       0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_pi_2,        _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_pi_4",       0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_pi_4,        _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_1_pi",       0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_1_pi,        _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_2_pi",       0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_2_pi,        _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_2_sqrtpi",   0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_2_sqrtpi,    _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_sqrt2",      0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_sqrt2,       _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_sqrt1_2",    0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_sqrt1_2,     _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_deg2rad",    0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_deg2rad,     _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "m_rad2deg",    0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_rad2deg,     _lfc,            0, 0);

#if !defined(SQLITE_ENABLE_MATH_FUNCTIONS)
    nErr += sqlite3_create_function(db, "pi",           0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_pi,          _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "ceil",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  ceil,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "ceiling",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  ceil,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "floor",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  floor,          _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "ln",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "log",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log10,          _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "log",          2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _logFunc2,       0, 0);
    nErr += sqlite3_create_function(db, "log2",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log2,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "log10",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log10,          _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "exp",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  exp,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "pow",          2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  pow,            _dfdd,           0, 0);
    nErr += sqlite3_create_function(db, "power",        2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  pow,            _dfdd,           0, 0);
    nErr += sqlite3_create_function(db, "fmod",         2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  fmod,           _dfdd,           0, 0);
    nErr += sqlite3_create_function(db, "mod",          2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  fmod,           _dfdd,           0, 0);
    nErr += sqlite3_create_function(db, "acos",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  acos,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "asin",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  asin,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "atan",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  atan,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "atan2",        2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  atan2,          _dfdd,           0, 0);
    nErr += sqlite3_create_function(db, "cos",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  cos,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "sin",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  sin,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "tan",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  tan,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "cosh",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  cosh,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "sinh",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  sinh,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "tanh",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  tanh,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "acosh",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  acosh,          _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "asinh",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  asinh,          _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "atanh",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  atanh,          _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "sqrt",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  sqrt,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "radians",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _radians,        0, 0);
    nErr += sqlite3_create_function(db, "degrees",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _degrees,        0, 0);
    nErr += sqlite3_create_function(db, "sign",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _sign,           0, 0);
#endif

    nErr += sqlite3_create_function(db, "pil",           0, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  &m_pi,          _lfc,            0, 0);
    nErr += sqlite3_create_function(db, "ceill",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  ceill,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "ceilingl",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  ceill,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "floorl",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  floorl,         _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "lnl",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  logl,           _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "logl",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log10l,         _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "logl",          2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _logFunc2l,      0, 0);
    nErr += sqlite3_create_function(db, "log2l",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log2l,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "log10l",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  log10l,         _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "expl",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  expl,           _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "powl",          2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  powl,           _lfll,           0, 0);
    nErr += sqlite3_create_function(db, "powerl",        2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  powl,           _lfll,           0, 0);
    nErr += sqlite3_create_function(db, "fmodl",         2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  fmodl,          _lfll,           0, 0);
    nErr += sqlite3_create_function(db, "modl",          2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  fmodl,          _lfll,           0, 0);
    nErr += sqlite3_create_function(db, "acosl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  acosl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "asinl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  asinl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "atanl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  atanl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "atan2l",        2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  atan2l,         _lfll,           0, 0);
    nErr += sqlite3_create_function(db, "cosl",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  cosl,           _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "sinl",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  sinl,           _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "tanl",          1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  tanl,           _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "coshl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  coshl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "sinhl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  sinhl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "tanhl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  tanhl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "acoshl",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  acoshl,         _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "asinhl",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  asinhl,         _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "atanhl",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  atanhl,         _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "sqrtl",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  sqrtl,          _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "radiansl",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _radians,        0, 0);
    nErr += sqlite3_create_function(db, "degreesl",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _degrees,        0, 0);

    nErr += sqlite3_create_function(db, "j0",            1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _j0,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "j1",            1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _j1,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "y0",            1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _y0,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "y1",            1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _y1,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "jn",            2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _jn,            _dfid,           0, 0);
    nErr += sqlite3_create_function(db, "yn",            2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _yn,            _dfid,           0, 0);
    nErr += sqlite3_create_function(db, "hypot",         2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _hypot,         _dfdd,           0, 0);

#if defined(__GNUC__) && defined(_WIN64)
    nErr += sqlite3_create_function(db, "j0l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  j0q,            _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "j1l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  j1q,            _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "y0l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  y0q,            _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "y1l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  y1q,            _lfl,            0, 0);
    nErr += sqlite3_create_function(db, "jnl",           2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  jnq,            _lfil,           0, 0);
    nErr += sqlite3_create_function(db, "ynl",           2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  ynq,            _lfil,           0, 0);
#else
    nErr += sqlite3_create_function(db, "j0l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _j0,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "j1l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _j1,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "y0l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _y0,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "y1l",           1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _y1,            _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "jnl",           2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _jn,            _dfid,           0, 0);
    nErr += sqlite3_create_function(db, "ynl",           2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _yn,            _dfid,           0, 0);
#endif

    nErr += sqlite3_create_function(db, "hypotl",        2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  hypotl,         _lfll,           0, 0);

    nErr += sqlite3_create_function(db, "chgsign",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _chgsign,       _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "fabs",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  fabs,           _dfd,            0, 0);
    nErr += sqlite3_create_function(db, "copysign",     2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  _copysign,      _dfdd,           0, 0);

    nErr += sqlite3_create_function(db, "ldexp",        2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  ldexp,          _dfdi,           0, 0);

    nErr += sqlite3_create_function(db, "nextdown",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)0,       _nexttowards_,   0, 0);
    nErr += sqlite3_create_function(db, "nextup",       1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  (void*)1,       _nexttowards_,   0, 0);
    nErr += sqlite3_create_function(db, "nexttowards",  2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _nexttowards_,   0, 0);

    nErr += sqlite3_create_function(db, "mantissa",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _mantissa,       0, 0);
    nErr += sqlite3_create_function(db, "exponent",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _exponent,       0, 0);
    nErr += sqlite3_create_function(db, "trunc",        1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _intpart,        0, 0);
    nErr += sqlite3_create_function(db, "frac",         1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _fracpart,       0, 0);
    nErr += sqlite3_create_function(db, "remainder",    2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  remainder,      _dfdd,           0, 0);
    nErr += sqlite3_create_function(db, "remainderl",   2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  remainderl,     _lfll,           0, 0);

#endif

    nErr += sqlite3_create_function(db, "fromhex",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _FromHex,        0, 0);
    nErr += sqlite3_create_function(db, "isset",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _isset,          0, 0);
    nErr += sqlite3_create_function(db, "isclr",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _isclr,          0, 0);
    nErr += sqlite3_create_function(db, "setbits",     -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _setbits,        0, 0);
    nErr += sqlite3_create_function(db, "clrbits",     -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _clrbits,        0, 0);

    nErr += sqlite3_create_function(db, "bitmask",     -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _bitmask,        0, 0);

    nErr += sqlite3_create_function(db, "ismaskset",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _ismaskset,      0, 0);
    nErr += sqlite3_create_function(db, "ismaskclr",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _ismaskclr,      0, 0);

    nErr += sqlite3_create_function(db, "ErrorIfNull",  1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _TestNull,       0, 0);

    nErr += sqlite3_create_function(db, "Boolean",      1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,              _TruthValue_,    0, 0);

#if defined(SQLITE_OMIT_WINDOWFUNC)
    nErr += sqlite3_create_function(db, "aggbitmask",  -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0, 0, _bitmaskStep, _bitmaskFinal);
#else
    nErr += sqlite3_create_window_function(db, "aggbitmask", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _bitmaskStep, _bitmaskFinal, _bitmaskFinal, _bitmaskInverse, 0);
#endif

#ifndef SQLITE_OMIT_FLOATING_POINT

    {
        register LONGDOUBLE_TYPE x;
        x = asinl(1.0);
        m_pi_2 = x;
        m_pi_4 = x / 2.0;
        m_pi = x * 2.0;
        m_1_pi = 0.5 / x;
        m_2_pi = 1.0 / x;
        m_2_sqrtpi = 2.0 / sqrtl(x * 2.0);
        m_deg2rad = x / 90.0;
        m_rad2deg = 90.0 / x;
    }
    m_e = expl(1.0);
    m_log10e = log10l(expl(1.0));
    m_sqrt2 = sqrtl(2.0);
    m_ln2 = logl(2.0);
    m_ln10 = logl(10.0);
    m_log2e = 1.0 / logl(2.0);
    m_sqrt2 = sqrtl(2.0);
    m_sqrt1_2 = sqrtl(0.5);

#endif

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

#ifdef __cplusplus
}
#endif

#undef acosl
#undef asinl
#undef atan2l
#undef atanl
#undef ceill
#undef coshl
#undef cosl
#undef expl
#undef floorl
#undef fmodl
#undef hypotl
#undef log10l
#undef log2l
#undef logl
#undef powl
#undef remainderl
#undef sinhl
#undef sinl
#undef sqrtl
#undef tanhl
#undef tanl

