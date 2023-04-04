/*
** sqlite_mprint.c as referred to from sqlite users mailinglist
** "using mprint as global variable inside queries"  on 20 sept. 2015
** (e.g. http://osdir.com/ml/general/2015-09/msg28447.html)
**
** See http://sqlite.org/loadext.html for how to compile, install and
** load an SQLite extension.
**
** .load sqlite_mprint
** create table t(dt);
** insert into t values
**     ('2015-09-02 01:12:00'),
**     ('2015-08-03 03:00:00'),
**     ('2015-09-12 00:36:00');
** --assemble interval between rows
** select mprint() as dtfrom, --value from previous row
**         mprint(dt) as dtto  --new value
** from (select mprint(null)) --initial value
** join t
** group by dt;
** |2015-08-03 03:00:00
** 2015-08-03 03:00:00|2015-09-02 01:12:00
** 2015-09-02 01:12:00|2015-09-12 00:36:00
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <assert.h>


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

typedef struct remembered_value remembered_value;
struct remembered_value
{
    int t;              /* Value type */
    double r;           /* Real value */
    sqlite_int64 i;     /* Integer value */
    char *z;            /* String or BLOB value */
    int n;              /* size of String or BLOB value */
};

remembered_value *pval;
remembered_value *pval1;
remembered_value *pval2;
remembered_value *pval3;

static void mprintfunc(sqlite3_context *ctx, int argc, sqlite3_value **argv, remembered_value **ppval)
{
    sqlite3_value *arg;
    remembered_value *pval;
    pval=*ppval;
    if (argc)                                   /* if a parameter is set */
    {
        arg=argv[0];                            /* remember new value */
        if (sqlite3_value_type(arg)==SQLITE_NULL)
        {
            if (pval)
            {
                if (pval->n)
                    sqlite3_free(pval->z);
                sqlite3_free(pval);
                pval = 0;
            }
        }
        else
        {
            if (!pval)
            {
                pval = sqlite3_malloc(sizeof(*pval));
                if (!pval) return;              /* SQLITE_NOMEM;*/  /* TODO */
            }
            else
            {
                if (pval->n)
                    sqlite3_free(pval->z);
            }
            pval->t = sqlite3_value_type(arg);
            pval->n = 0;
            pval->z = "";
            switch (pval->t)
            {
                case SQLITE_INTEGER:
                    pval->i = sqlite3_value_int64(arg);
                    break;
                case SQLITE_FLOAT:
                    pval->r = sqlite3_value_double(arg);
                    break;
                case SQLITE_TEXT:
                    pval->n = sqlite3_value_bytes(arg);
                    if (pval->n)
                    {
                        pval->z = sqlite3_malloc(pval->n);
                        assert (pval->z);       /* TODO: SQLITE_NOMEM*/
                        memcpy(pval->z, sqlite3_value_text(arg), pval->n);
                    }
                    break;
                case SQLITE_BLOB:               /* sqlite3_value_blob instead of _text */
                    pval->n = sqlite3_value_bytes(arg);
                    if (pval->n)
                    {
                        pval->z = sqlite3_malloc(pval->n);
                        assert (pval->z);       /* TODO: SQLITE_NOMEM*/
                        memcpy(pval->z, sqlite3_value_blob(arg), pval->n);
                    }
                    break;
            }
        }
        *ppval = pval;
    }
    /* return value that was last set */
    if (!pval)
    {
        sqlite3_result_null(ctx);
    }
    else
    {
        switch(pval->t)
        {
            case SQLITE_INTEGER:
                sqlite3_result_int64(ctx, pval->i);
                break;
            case SQLITE_TEXT:
                sqlite3_result_text(ctx, pval->z, pval->n, SQLITE_TRANSIENT);
                break;
            case SQLITE_BLOB:
                sqlite3_result_blob(ctx, pval->z, pval->n, SQLITE_TRANSIENT);
                break;
            case SQLITE_FLOAT:
                sqlite3_result_double(ctx, pval->r);
                break;
        }
    }
}

static void mprint( sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    mprintfunc (ctx,argc,argv, &pval);
}
static void mprint1( sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    mprintfunc (ctx,argc,argv, &pval1);
}
static void mprint2( sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    mprintfunc (ctx,argc,argv, &pval2);
}
static void mprint3( sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    mprintfunc (ctx,argc,argv, &pval3);
}

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlitemprint_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    SQLITE_EXTENSION_INIT2(pApi);
    sqlite3_create_function(db, "mprint", 0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint, 0,0);
    sqlite3_create_function(db, "mprint", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint, 0,0);
    sqlite3_create_function(db, "mprint1", 0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint1, 0,0);
    sqlite3_create_function(db, "mprint1", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint1, 0,0);
    sqlite3_create_function(db, "mprint2", 0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint2, 0,0);
    sqlite3_create_function(db, "mprint2", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint2, 0,0);
    sqlite3_create_function(db, "mprint3", 0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint3, 0,0);
    sqlite3_create_function(db, "mprint3", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, mprint3, 0,0);
    return SQLITE_OK;
}

#ifdef __cplusplus
}
#endif

