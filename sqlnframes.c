
#ifndef SQLITE_CORE
#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1
#endif

static void _txn_state_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3 *db = sqlite3_context_db_handle(context);

    sqlite3_result_int(context, sqlite3_txn_state(db, argc ? sqlite3_value_text(argv[0]) : 0));
}

static void _autoCommit_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3 *db = sqlite3_context_db_handle(context);

    sqlite3_result_int(context, sqlite3_get_autocommit(db));
}

static void _inTransaction_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3 *db = sqlite3_context_db_handle(context);

    sqlite3_result_int(context, (!sqlite3_get_autocommit(db)) || (sqlite3_txn_state(db, 0) ? 1 : 0));
}

#ifdef SQLITE_CORE
static void _vmFrames_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_int(context, context->pVdbe->nFrame);
}

static void _Recursing_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3_result_int(context, (context->pVdbe->nFrame > 1));
}
#endif

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlnframes_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

#ifndef SQLITE_CORE
    SQLITE_EXTENSION_INIT2(pApi);
#endif

#ifdef SQLITE_CORE
    nErr += sqlite3_create_function(db, "vmFrames",      0, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, _vmFrames_,      0, 0);
    nErr += sqlite3_create_function(db, "recursing",     0, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, _Recursing_,     0, 0);
#endif

    nErr += sqlite3_create_function(db, "autoCommit",    0, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, _autoCommit_,    0, 0);
    nErr += sqlite3_create_function(db, "txn_state",     0, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, _txn_state_,     0, 0);
    nErr += sqlite3_create_function(db, "txn_state",     1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, _txn_state_,     0, 0);
    nErr += sqlite3_create_function(db, "inTransaction", 0, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, _inTransaction_, 0, 0);

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

