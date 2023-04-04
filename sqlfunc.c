/*
** 2017 April 12 -- Keith Medcalf kmedcalf@dessus.com
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*/
#include <math.h>
#include <stdio.h>

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

#undef LONGDOUBLE_CONSTANT
#undef LONGDOUBLE_TYPE
#if defined(__GNUC__) && defined(_WIN64)
#include <quadmath.h>
#define LONGDOUBLE_TYPE __float128
#define LONGDOUBLE_CONSTANT(x) x##Q
#define powl powq
#define sqrtl sqrtq
#define logl logq
#define expl expq
#else
#define LONGDOUBLE_TYPE long double
#define LONGDOUBLE_CONSTANT(x) x##L
#endif

/*
** Use the Kahan Babushka Klein Neumaier method to compute an accurate sum
** also use to compute average (KBNSum KBNAvg)
*/

typedef struct KBNSumCtx KBNSumCtx;
struct KBNSumCtx
{
    double sum, c;
    sqlite_int64 n;
};

SQLITE_PRIVATE void KBNSumStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    KBNSumCtx* p = sqlite3_aggregate_context(context, sizeof(KBNSumCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        volatile double t, z;
        volatile double x, y;
        if (!p->n)
        {
            p->sum = 0.0;
            p->c = 0.0;
        }
        p->n++;
        x = sqlite3_value_double(argv[0]);
        y = x - p->c;
        t = p->sum + y;
        z = t - p->sum;
        p->c = z - y;
        p->sum = t;
    }
}

SQLITE_PRIVATE void KBNSumFinal(sqlite3_context *context)
{
    KBNSumCtx* p = sqlite3_aggregate_context(context, 0);
    sqlite3_result_double(context, p->sum + p->c);
}

SQLITE_PRIVATE void KBNAvgFinal(sqlite3_context *context)
{
    KBNSumCtx* p = sqlite3_aggregate_context(context, 0);
    sqlite3_result_double(context, (p->sum + p->c) / (LONGDOUBLE_TYPE)p->n);
}


/*
** Running Statistical Computations
** Using running approximations, compute the first, second, third and fourth moments
** Used to compute skew/skewP/kurt/kurtP
*/

typedef struct momentCtx momentCtx;
struct momentCtx
{
    LONGDOUBLE_TYPE m1;
    LONGDOUBLE_TYPE m2;
    LONGDOUBLE_TYPE m3;
    LONGDOUBLE_TYPE m4;
    sqlite_int64 n;
};

SQLITE_PRIVATE void momentStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    momentCtx* p = sqlite3_aggregate_context(context, sizeof(momentCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE nx, y;
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        p->n++;
        nx = (LONGDOUBLE_TYPE)p->n;
        if (p->n == 1)
        {
            y = x;
            p->m1 = y;
            y *= x;
            p->m2 = y;
            y *= x;
            p->m3 = y;
            y *= x;
            p->m4 = y;
        }
        else
        {
            y = x;
            p->m1 += (y - p->m1) / nx;
            y *= x;
            p->m2 += (y - p->m2) / nx;
            y *= x;
            p->m3 += (y - p->m3) / nx;
            y *= x;
            p->m4 += (y - p->m4) / nx;
        }
    }
}

SQLITE_PRIVATE void momentUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    momentCtx* p = sqlite3_aggregate_context(context, sizeof(momentCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE nx, y;
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        p->n--;
        nx = (LONGDOUBLE_TYPE)p->n;
        y = x;
        p->m1 -= (y - p->m1) / nx;
        y *= x;
        p->m2 -= (y - p->m2) / nx;
        y *= x;
        p->m3 -= (y - p->m3) / nx;
        y *= x;
        p->m4 -= (y - p->m4) / nx;
    }
}

/*
** Compute the interim result based on the moment approximations
** skew/skewP/kurt/kurtP
*/

SQLITE_PRIVATE void skewPFinal(sqlite3_context *context)
{
    register LONGDOUBLE_TYPE k2, k3, num, den;
    momentCtx* p = sqlite3_aggregate_context(context, 0);
    if (!p || p->n < 3)
        return;

//  k1 = p->m1;
    k2 = p->m2 - powl(p->m1, 2.0);
    k3 = 2. * powl(p->m1, 3.) - 3. * p->m1 * p->m2 + p->m3;
//  k4 = -6. * powl(p->m1, 4.) + 12. * powl(p->m1, 2.) * p->m2 - 3. * powl(p->m2, 2.) - 4. * p->m1 * p->m3 + p->m4;

    num = k3;
    den = powl(k2, 1.5);

    if (den != 0.0)
        sqlite3_result_double(context, num / den);
}

SQLITE_PRIVATE void skewFinal(sqlite3_context *context)
{
    register LONGDOUBLE_TYPE k2, k3, nx, num, den;
    momentCtx* p = sqlite3_aggregate_context(context, 0);
    if (!p || p->n < 3)
        return;

    nx = (LONGDOUBLE_TYPE)p->n;
//  k1 = p->m1;
    k2 = p->m2 - powl(p->m1, 2.0);
    k3 = 2. * powl(p->m1, 3.) - 3. * p->m1 * p->m2 + p->m3;
//  k4 = -6. * powl(p->m1, 4.) + 12. * powl(p->m1, 2.) * p->m2 - 3. * powl(p->m2, 2.) - 4. * p->m1 * p->m3 + p->m4;

    num = sqrtl(nx * (nx - 1.0)) / (nx - 2.0) * k3;
    den = powl(k2, 1.5);

    if (den != 0.0)
        sqlite3_result_double(context, num / den);
}

SQLITE_PRIVATE void kurtPFinal(sqlite3_context *context)
{
    register LONGDOUBLE_TYPE k2, k4, num, den, nx;
    momentCtx* p = sqlite3_aggregate_context(context, 0);
    if (!p || p->n < 3)
        return;

    nx = (LONGDOUBLE_TYPE)p->n;

//  k1 = p->m1;
    k2 = p->m2 - powl(p->m1, 2.0);
//  k3 = 2. * powl(p->m1, 3.) - 3. * p->m1 * p->m2 + p->m3;
    k4 = -6. * powl(p->m1, 4.) + 12. * powl(p->m1, 2.) * p->m2 - 3. * powl(p->m2, 2.) - 4. * p->m1 * p->m3 + p->m4;

    num = k4;
    den = powl(k2, 2.);

    if (den != 0.0)
        sqlite3_result_double(context, num / den);
}

SQLITE_PRIVATE void kurtFinal(sqlite3_context *context)
{
    momentCtx* p = sqlite3_aggregate_context(context, 0);
    register LONGDOUBLE_TYPE k2, k4, num, den, nx;
    if (!p || p->n < 3)
        return;

    nx = (LONGDOUBLE_TYPE)p->n;

//  k1 = p->m1;
    k2 = p->m2 - powl(p->m1, 2.0);
//  k3 = 2. * powl(p->m1, 3.) - 3. * p->m1 * p->m2 + p->m3;
    k4 = -6. * powl(p->m1, 4.) + 12. * powl(p->m1, 2.) * p->m2 - 3. * powl(p->m2, 2.) - 4. * p->m1 * p->m3 + p->m4;

    num = k4;
    den = powl(k2, 2.0);

    if (den == 0.0)
        return;

    sqlite3_result_double(context, (nx - 1.) / ((nx - 2.) * (nx - 3.)) * ((nx + 1.) * (num / den) + 6.));
}


/*
** Compute mean and variance using successive approxmation (single argument/unweighted)
** Used to compute avg/var/varP/stdev/stdevP/sem/ci/lcl/ucl
*/

typedef struct stdevCtx stdevCtx;
struct stdevCtx
{
    LONGDOUBLE_TYPE oldM, newM, oldS, newS;
    sqlite_int64 n;
};

SQLITE_PRIVATE void stdevStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    stdevCtx* p = sqlite3_aggregate_context(context, sizeof(stdevCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        p->n++;
        if (p->n == 1)
        {
            p->oldM = p->newM = x;
            p->oldS = p->newS = 0.0;
        }
        else
        {
            p->newM = p->oldM + (x - p->oldM) / (LONGDOUBLE_TYPE)p->n;
            p->newS = p->oldS + (x - p->oldM) * (x - p->newM);
            p->oldM = p->newM;
            p->oldS = p->newS;
        }
    }
}

SQLITE_PRIVATE void stdevUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    stdevCtx* p = sqlite3_aggregate_context(context, sizeof(stdevCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        p->n--;
        p->newM = p->oldM - (x - p->oldM) / (LONGDOUBLE_TYPE)p->n;
        p->newS = p->oldS - (x - p->oldM) * (x - p->newM);
        p->oldM = p->newM;
        p->oldS = p->newS;
    }
}


/*
** Compute the interim results for single argument avg/var/stdev/varP/stdevP/sem/ci/ucl/lcl
*/

SQLITE_PRIVATE void varianceFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 1 ? p->newS / (LONGDOUBLE_TYPE)(p->n - 1) : 0.0);
}

SQLITE_PRIVATE void variancePFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 0 ? p->newS / (LONGDOUBLE_TYPE)(p->n) : 0.0);
}

SQLITE_PRIVATE void stdevFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 1 ? sqrtl(p->newS / (LONGDOUBLE_TYPE)(p->n - 1)) : 0.0);
}

SQLITE_PRIVATE void stdevPFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 0 ? sqrtl(p->newS / (LONGDOUBLE_TYPE)(p->n)) : 0.0);
}

SQLITE_PRIVATE void semFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 1 ? sqrtl(p->newS / (LONGDOUBLE_TYPE)(p->n - 1)) / sqrtl(p->n) : 0.0);
}

SQLITE_PRIVATE void ciFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 1 ? sqrtl(p->newS / (LONGDOUBLE_TYPE)(p->n - 1)) / sqrtl(p->n) * 1.96 : 0.0);
}

SQLITE_PRIVATE void uclFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 0 ? p->newM + 3.0 * sqrtl(p->newS / (LONGDOUBLE_TYPE)(p->n)) : 0.0);
}

SQLITE_PRIVATE void lclFinal(sqlite3_context *context)
{
    stdevCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->n > 0 ? p->newM - 3.0 * sqrtl(p->newS / (LONGDOUBLE_TYPE)(p->n)) : 0.0);
}

/*
** Compute specialized range and median by as range -> maxx - minx, median -> (maxx+minx)/2
** This is an aggregate function, not a window function
*/

typedef struct rangeCtx rangeCtx;
struct rangeCtx
{
    LONGDOUBLE_TYPE minx;
    LONGDOUBLE_TYPE maxx;
    int flag;
};

SQLITE_PRIVATE void rangeStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    rangeCtx* p = sqlite3_aggregate_context(context, sizeof(rangeCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = (LONGDOUBLE_TYPE)sqlite3_value_double(argv[0]);
        if (p->flag == 0)
        {
            p->minx = x;
            p->maxx = x;
            p->flag = 1;
        }
        else
        {
            if (x < p->minx) p->minx = x;
            if (x > p->maxx) p->maxx = x;
        }
    }
}

SQLITE_PRIVATE void rangeFinal(sqlite3_context *context)
{
    rangeCtx* p = sqlite3_aggregate_context(context, 0);
    if (p)
        sqlite3_result_double(context, (p->maxx - p->minx) / 2.0L);
}

SQLITE_PRIVATE void medianFinal(sqlite3_context *context)
{
    rangeCtx* p = sqlite3_aggregate_context(context, 0);
    if (p)
        sqlite3_result_double(context, (p->maxx + p->minx) / 2.0L);
}

/*
** Compute the avg, aavg, gavg using successive approximation (unweighted, single argument)
*/

typedef struct meanCtx meanCtx;
struct meanCtx
{
    LONGDOUBLE_TYPE m;
    sqlite_int64 n;
};

SQLITE_PRIVATE void meanStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->n++;
        if (p->n == 1)
            p->m = (LONGDOUBLE_TYPE)sqlite3_value_double(argv[0]);
        else
            p->m += ((LONGDOUBLE_TYPE)sqlite3_value_double(argv[0]) - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void meanUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->n--;
        p->m -= ((LONGDOUBLE_TYPE)sqlite3_value_double(argv[0]) - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void ameanStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->n++;
        if (p->n == 1)
            p->m = (LONGDOUBLE_TYPE)fabs(sqlite3_value_double(argv[0]));
        else
            p->m += ((LONGDOUBLE_TYPE)fabs(sqlite3_value_double(argv[0])) - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void ameanUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->n--;
        p->m -= ((LONGDOUBLE_TYPE)fabs(sqlite3_value_double(argv[0])) - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void gmeanStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->n++;
        if (p->n == 1)
            p->m = (LONGDOUBLE_TYPE)logl(sqlite3_value_double(argv[0]));
        else
            p->m += ((LONGDOUBLE_TYPE)logl(sqlite3_value_double(argv[0])) - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void gmeanUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->n--;
        p->m -= ((LONGDOUBLE_TYPE)logl(sqlite3_value_double(argv[0])) - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void meanFinal(sqlite3_context *context)
{
    meanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->m);
}

SQLITE_PRIVATE void gmeanFinal(sqlite3_context *context)
{
    meanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, expl(p->m));
}


/*
** Compute the rms by successive approximation (unweighted, single argument)
*/

SQLITE_PRIVATE void rmsStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        p->n++;
        if (p->n == 1)
            p->m = x * x;
        else
            p->m += (x * x - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void rmsUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    meanCtx* p = sqlite3_aggregate_context(context, sizeof(meanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        p->n--;
        p->m -= (x * x - p->m) / (LONGDOUBLE_TYPE)p->n;
    }
}

SQLITE_PRIVATE void rmsFinal(sqlite3_context *context)
{
    meanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, sqrtl(p->m));
}


/*
** Using successive approximation compute the weighted functions (2 arguments)
** avg/var/varP/stdev/stdevP/sem/ci
*/

typedef struct wmeanCtx wmeanCtx;
struct wmeanCtx
{
    sqlite_int64 n;
    LONGDOUBLE_TYPE m;
    LONGDOUBLE_TYPE w;
    LONGDOUBLE_TYPE s;
};

SQLITE_PRIVATE void wmeanStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, sizeof(wmeanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL && sqlite3_value_numeric_type(argv[1]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        register LONGDOUBLE_TYPE w = sqlite3_value_double(argv[1]);
        if (p->n)
        {
            register LONGDOUBLE_TYPE newW, delta, R;
            newW = w + p->w;
            delta = x - p->m;
            R = delta * w / newW;
            p->m += R;
            p->s += p->w * delta * R;
            p->w = newW;
        }
        else
        {
            p->m = x;
            p->w = w;
            p->s = 0.0;
        }
        p->n++;
    }
}

SQLITE_PRIVATE void wmeanUnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, sizeof(wmeanCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL && sqlite3_value_numeric_type(argv[1]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        register LONGDOUBLE_TYPE w = sqlite3_value_double(argv[1]);
        register LONGDOUBLE_TYPE newW, delta, R;
        newW = p->w - w;
        delta = x - p->m;
        R = delta * w / newW;
        p->m -= R;
        p->s -= p->w * delta * R;
        p->w = newW;
        p->n--;
    }
}

SQLITE_PRIVATE void wmeanFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->m);
}

SQLITE_PRIVATE void wvarPFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->s/p->w);
}

SQLITE_PRIVATE void wvarFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, p->s/p->w*(LONGDOUBLE_TYPE)p->n/(LONGDOUBLE_TYPE)(p->n - 1));
}

SQLITE_PRIVATE void wstdevPFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, sqrtl(p->s/p->w));
}

SQLITE_PRIVATE void wstdevFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, sqrtl(p->s/p->w*(LONGDOUBLE_TYPE)p->n/(LONGDOUBLE_TYPE)(p->n - 1)));
}

SQLITE_PRIVATE void wsemFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, sqrtl(p->s/p->w*(LONGDOUBLE_TYPE)p->n/(LONGDOUBLE_TYPE)(p->n - 1))/sqrtl(p->n));
}

SQLITE_PRIVATE void wciFinal(sqlite3_context *context)
{
    wmeanCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, sqrtl(p->s/p->w*(LONGDOUBLE_TYPE)p->n/(LONGDOUBLE_TYPE)(p->n - 1))/sqrtl(p->n)*1.96L);
}


/*
** Compute the covariance between x and y
** This is an aggregate function only
*/

typedef struct cvar2Ctx cvar2Ctx;
struct cvar2Ctx
{
    sqlite_int64 n;
    LONGDOUBLE_TYPE mx;
    LONGDOUBLE_TYPE my;
    LONGDOUBLE_TYPE c1;
    LONGDOUBLE_TYPE c2;
};

SQLITE_PRIVATE void covarStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    cvar2Ctx* p = sqlite3_aggregate_context(context, sizeof(cvar2Ctx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL && sqlite3_value_numeric_type(argv[1]) != SQLITE_NULL)
    {
        register LONGDOUBLE_TYPE x = sqlite3_value_double(argv[0]);
        register LONGDOUBLE_TYPE y = sqlite3_value_double(argv[1]);
        p->n++;
        if (p->n == 1)
        {
            p->mx = x;
            p->my = y;
            p->c1 = 0.0;
            p->c2 = 0.0;
        }
        else
        {
            register LONGDOUBLE_TYPE nx, ny, rn;
            rn = (LONGDOUBLE_TYPE)p->n;
            nx = p->mx + (x - p->mx) / rn;
            ny = p->my + (y - p->my) / rn;
            p->c1 += (x - nx) * (y - p->my);
            p->c2 += (x - p->mx) * (y - ny);
            p->mx = nx;
            p->my = ny;
        }
    }
}

SQLITE_PRIVATE void covarPFinal(sqlite3_context *context)
{
    cvar2Ctx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, (p->c1 + p->c2) / 2.0L / (LONGDOUBLE_TYPE)p->n);
}

SQLITE_PRIVATE void covarFinal(sqlite3_context *context)
{
    cvar2Ctx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->n > 0)
        sqlite3_result_double(context, (p->c1 + p->c2) / 2.0L / ((LONGDOUBLE_TYPE)p->n - 1.0));
}


/*
** Special window functions FirstNotNull/LastNotNull
*/

typedef struct fnnCtx fnnCtx;
struct fnnCtx
{
    double fnn;
    int flag;
};

SQLITE_PRIVATE void fnnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    fnnCtx* p = sqlite3_aggregate_context(context, sizeof(fnnCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL && p->flag == 0)
    {
        p->fnn = sqlite3_value_double(argv[0]);
        p->flag = 1;
    }
}

SQLITE_PRIVATE void fnnUnstep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    fnnCtx* p = sqlite3_aggregate_context(context, sizeof(fnnCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL && p->flag == 1)
    {
        p->flag = 0;
    }
}

SQLITE_PRIVATE void lnnStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    fnnCtx* p = sqlite3_aggregate_context(context, sizeof(fnnCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL)
    {
        p->fnn = sqlite3_value_double(argv[0]);
        p->flag = 1;
    }
}

SQLITE_PRIVATE void lnnUnstep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    fnnCtx* p = sqlite3_aggregate_context(context, sizeof(fnnCtx));
    if (p && sqlite3_value_numeric_type(argv[0]) != SQLITE_NULL && p->flag == 1)
    {
        p->flag = 0;
    }
}

SQLITE_PRIVATE void fnnFinal(sqlite3_context *context)
{
    fnnCtx* p = sqlite3_aggregate_context(context, 0);
    if (p && p->flag == 1)
        sqlite3_result_double(context, p->fnn);
    else
        sqlite3_result_null(context);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlfunc_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

#if defined(SQLITE_OMIT_WINDOWFUNC)
    nErr += sqlite3_create_function(db, "avg",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   meanStep,  meanFinal);
    nErr += sqlite3_create_function(db, "aavg",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   ameanStep, meanFinal);
    nErr += sqlite3_create_function(db, "gavg",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   gmeanStep, gmeanFinal);
    nErr += sqlite3_create_function(db, "rms",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   rmsStep,   rmsFinal);
    nErr += sqlite3_create_function(db, "var",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, varianceFinal);
    nErr += sqlite3_create_function(db, "varP",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, variancePFinal);
    nErr += sqlite3_create_function(db, "stdev",  1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, stdevFinal);
    nErr += sqlite3_create_function(db, "stdevP", 1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, stdevPFinal);
    nErr += sqlite3_create_function(db, "sem",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, semFinal);
    nErr += sqlite3_create_function(db, "ci",     1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, ciFinal);
    nErr += sqlite3_create_function(db, "lcl",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, lclFinal);
    nErr += sqlite3_create_function(db, "ucl",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   stdevStep, uclFinal);
    nErr += sqlite3_create_function(db, "skewP",  1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   momentStep,skewPFinal);
    nErr += sqlite3_create_function(db, "skew",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   momentStep,skewFinal);
    nErr += sqlite3_create_function(db, "kurtP",  1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   momentStep,kurtPFinal);
    nErr += sqlite3_create_function(db, "kurt",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   momentStep,kurtFinal);
    nErr += sqlite3_create_function(db, "avg",    2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wmeanFinal);
    nErr += sqlite3_create_function(db, "stdev",  2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wstdevFinal);
    nErr += sqlite3_create_function(db, "stdevP", 2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wstdevPFinal);
    nErr += sqlite3_create_function(db, "var",    2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wvarFinal);
    nErr += sqlite3_create_function(db, "varP",   2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wvarPFinal);
    nErr += sqlite3_create_function(db, "sem",    2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wsemFinal);
    nErr += sqlite3_create_function(db, "ci",     2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   wmeanStep, wciFinal);
#else
    nErr += sqlite3_create_window_function(db, "avg",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  meanStep,  meanFinal, meanFinal, meanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "aavg",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  ameanStep, meanFinal, meanFinal, ameanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "gavg",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  gmeanStep, gmeanFinal, gmeanFinal, gmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "rms",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  rmsStep,   rmsFinal, rmsFinal, rmsUnStep, 0);
    nErr += sqlite3_create_window_function(db, "var",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, varianceFinal, varianceFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "varP",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, variancePFinal, variancePFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "stdev",  1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, stdevFinal, stdevFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "stdevP", 1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, stdevPFinal, stdevPFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "sem",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, semFinal, semFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "ci",     1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, ciFinal, ciFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "lcl",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, lclFinal, lclFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "ucl",    1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  stdevStep, uclFinal, uclFinal, stdevUnStep, 0);
    nErr += sqlite3_create_window_function(db, "skewP",  1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  momentStep, skewPFinal, skewPFinal, momentUnStep, 0);
    nErr += sqlite3_create_window_function(db, "skew",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  momentStep, skewFinal, skewFinal, momentUnStep, 0);
    nErr += sqlite3_create_window_function(db, "kurtP",  1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  momentStep, kurtPFinal, kurtPFinal, momentUnStep, 0);
    nErr += sqlite3_create_window_function(db, "kurt",   1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  momentStep, kurtFinal, kurtFinal, momentUnStep, 0);
    nErr += sqlite3_create_window_function(db, "avg",    2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wmeanFinal, wmeanFinal, wmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "stdev",  2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wstdevFinal, wstdevFinal, wmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "stdevP", 2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wstdevPFinal, wstdevPFinal, wmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "var",    2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wvarFinal, wvarFinal, wmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "varP",   2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wvarPFinal, wvarPFinal, wmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "sem",    2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wsemFinal, wsemFinal, wmeanUnStep, 0);
    nErr += sqlite3_create_window_function(db, "ci",     2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0,  wmeanStep, wciFinal, wciFinal, wmeanUnStep, 0);
#endif


#if defined(SQLITE_OMIT_WINDOWFUNC)
    nErr += sqlite3_create_function(db, "FirstNotNull", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, 0, fnnStep, fnnFinal);
    nErr += sqlite3_create_function(db, "LastNotNull",  1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, 0, lnnStep, fnnFinal);
#else
    nErr += sqlite3_create_window_function(db, "FirstNotNull", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, fnnStep, fnnFinal, fnnFinal, fnnUnstep, 0);
    nErr += sqlite3_create_window_function(db, "LastNotNull",  1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, lnnStep, fnnFinal, fnnFinal, lnnUnstep, 0);
#endif

    nErr += sqlite3_create_function(db, "range",        1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   rangeStep, rangeFinal);
    nErr += sqlite3_create_function(db, "median",       1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   rangeStep, medianFinal);

    nErr += sqlite3_create_function(db, "covar",        2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   covarStep, covarFinal);
    nErr += sqlite3_create_function(db, "covarP",       2, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   covarStep, covarPFinal);

    nErr += sqlite3_create_function(db, "KBNSum",       1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   KBNSumStep, KBNSumFinal);
    nErr += sqlite3_create_function(db, "KBNAvg",       1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   KBNSumStep, KBNAvgFinal);

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

#undef powl
#undef sqrtl
#undef logl
#undef expl

