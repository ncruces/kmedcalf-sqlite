
/* We need the math header for the fabs and fmod functions used in _elapsedTime function */

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

/*
** The elaspsedTime function will take either an input text string or an
** integer/floating point value.
**
** Input text in the d:h:m:s format will return a floating point value
** which is the total number of seconds represented.  Each of d/h/m/s may
** be arbitrary floating point numbers.
**
** Note that the d:h:m:s result will be calculated based on the absolute
** value of each field and that the sign of the leftmost field determines
** the sign of the result.
**
** Input numbers (integer or float) are a total number of seconds (which
** must be in the range -1e21 to 1e21) and will return the elapsed time
** string as d:hh:mm:ss.sss where d may be of arbitrary length, hh will be
** zero filled and between 0 and 23, mm will be zero filled and between 0
** and 59.  ss.sss will be between 0.000 and 59.999, zero filled, with
** exactly three decimal places.
*/

static void _elapsedTime(sqlite3_context *context, int argc, sqlite3_value **argv) {

    const double maxspan = 464269060799.999;

    switch (sqlite3_value_type(argv[0]))
    {
        case SQLITE_NULL:
        {
            return;
        }

        case SQLITE_BLOB:
        {
            return;
        }

        case SQLITE_TEXT:
        {
            double factors[4] = {86400.0, 3600.0, 60.0, 1.0};
            double total = 0.0;
            double sgn = 1.0;
            char *start, *end;


            /* Force conversion to utf-8 and make a copy of the text arg so we can modify it */
            sqlite3_value_text(argv[0]);
            start = sqlite3_malloc(sqlite3_value_bytes(argv[0]) + 1);
            strcpy(start, sqlite3_value_text(argv[0]));
            end = start + strlen(start);

            /* Compute totalseconds by parsing colon separated floats from the right */
            for (int j=3; j >= 0; j--)
            {
                double value;
                char *i;

                for (i=end; ((*i != ':') && (i >= start)); i--) ;
                value = atof(i + 1);
                total += fabs(value * factors[j]);
                sgn = (value < 0) || (*(i + 1) == '-') ? -1.0 : 1.0;
                if (i > start)
                    *i = 0;
                else
                    break;
            }
            sqlite3_free(start);
            if (total > maxspan)
                return;
            total *= sgn;

            if ((intptr_t)sqlite3_user_data(context) == 0)
            {
                sqlite3_result_double(context, total);
            }
            else
            {
                char out[32];
                /* Output datetime modifier format */
                sqlite3_snprintf(sizeof(out), out, "%+.3f seconds", total);
                sqlite3_result_text(context, out, -1, SQLITE_TRANSIENT);
            }
            return;
        }

        default:
        {
            double s;
            int d, h, m;
            char out[32];
            char *sgn;

            /* Get our total seconds as a float */
            s = sqlite3_value_double(argv[0]);

            if (fabs(s) > maxspan)
                return;

            /* Return datetime modifier format */
            if ((intptr_t)sqlite3_user_data(context) == 1)
            {
                sqlite3_snprintf(sizeof(out), out, "%+.3f seconds", s);
                sqlite3_result_text(context, out, -1, SQLITE_TRANSIENT);
                return;
            }

            /* Save our sign and use only absolute value */
            sgn = s < 0 ? "-" : "";
            s = fabs(s);

            /* convert s to d/h/m/s */
            d = (int)(s / 86400.0);
            h = (int)(fmod(s, 86400.0) / 3600.0);
            m = (int)(fmod(s,  3600.0) / 60.0);
            s = fmod(s, 60.0);

            sqlite3_snprintf(sizeof(out), out, "%s%d:%02d:%02d:%06.3f", sgn, d, h, m, s);
            sqlite3_result_text(context, out, -1, SQLITE_TRANSIENT);
            return;
        }
    }
}

#ifndef _WIN32
static void GetSystemTimePreciseAsFileTime(sqlite_int64 *huns)
{
    struct timeval sNow;
    (void)gettimeofday(&sNow, 0);  /* Cannot fail given valid arguments */
    *huns = 10000000*(sqlite_int64)sNow.tv_sec + (sqlite_int64)sNow.tv_usec*10 + 116444736000000000LL;
}
#else
#include <windows.h>
#endif

static void _hunstamp_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_int64 huns;
    GetSystemTimePreciseAsFileTime((void*)&huns);
    sqlite3_result_int64(context, huns);
}
static void _unixstamp_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_int64 huns;
    GetSystemTimePreciseAsFileTime((void*)&huns);
    sqlite3_result_double(context, ((sqlite_int64)(huns/10000) - 11644473600000LL) / 1.0e3);
}
static void _jdstamp_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_int64 huns;
    GetSystemTimePreciseAsFileTime((void*)&huns);
    sqlite3_result_double(context, ((sqlite_int64)(huns/10000) + 199222286400000LL) / 86400.0e3);
}


/*
** This is the initialization function which must be called to initialize these functions
** on the current connection.
*/

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqltime_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

    nErr += sqlite3_create_function(db, "elapsedTime",     1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)0, _elapsedTime, 0, 0);
    nErr += sqlite3_create_function(db, "timeModifier",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)1, _elapsedTime, 0, 0);
#ifdef _WIN32
    nErr += sqlite3_create_function(db, "hunstamp",     0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _hunstamp_,  0, 0);
    nErr += sqlite3_create_function(db, "unixstamp",    0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _unixstamp_, 0, 0);
    nErr += sqlite3_create_function(db, "julianstamp",  0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _jdstamp_,   0, 0);
#endif
    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

#if 0
/*
** Sample SQLITE_EXTRA_INIT function (there can only be one)
**
** You can append this file to the end of the sqlite3.c amalgamation,
** change the #if 0 above to #if 1 to include this code, and compile with
** the SQLITE_EXTRA_INIT=core_init define to get the above functions
** activated on every connection.  Alternatively, you can leave this as it
** is and use your own EXTRA_INIT function to activate the functions on
** every connection, especially if you wish to have other functions
** included in the amalgamation by default.
**
*/

int core_init(const char* dummy)
{
    int nErr = 0;

    nErr += sqlite3_auto_extension((void*)sqlite3_sqltime_init);

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}
#endif

