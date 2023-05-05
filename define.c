/*
 * UDF DefineScalar and UnDefineScalar
 *
 * Written by Keith Medcalf, 2023; released into the Public Domain
 *
 * Define a scalar function using an SQL expression.  Compiled Statement is cached as auxdata
 * for the duration of a statement execution.  Functions are permanently defined in table
 * scalarfunctions which are created if the table exists when the module initialized, or
 * the table is created when first needed (thus permitting the extension to be loaded without
 * tromping on the ability to set the initial pagesize or encoding).
 *
 * eg: select DefineScalar('w16x9', 'sqrt(pow(:diag, 2) / 1.31640625)');
 *     select DefineScalar('h16x9', 'sqrt(pow(:diag, 2) / 1.31640625) / 16 * 9');
 *     select UndefineScalar('h16x9');
 *     select CopyScalarFunctions('someother.db');
 *
 * Deleted functions are removed from the table.
 *
 * Executing DefineScalar on a function that is already defined (and not deleted) will
 * update the function definition.  The updated definition will be used for all statements
 * commencing execution after the definition is changed.
 *
 * If a function is deleted, the connection must be closed and re-opened before it may be re-defined.
 *
 */

#include <stdint.h>
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

// SCLRSLOT = -(unicode('S')*16777216 + unicode('C')*65536 + unicode('L')*256 + unicode('R'))
#define SCLRSLOT -1396919378


typedef struct ScalarUserContext ScalarUserContext;
struct ScalarUserContext
{
    int id;
    char *name;
};


SQLITE_PRIVATE ScalarUserContext* _ScalarUserContext_(int id, const unsigned char *name)
{
    ScalarUserContext *p = sqlite3_malloc(sizeof(ScalarUserContext));
    p->id = id;
    p->name = sqlite3_mprintf("%s", name);
    return p;
}


SQLITE_PRIVATE void _ScalarUserContextDelete_(void* ptr)
{
    ScalarUserContext *p = (ScalarUserContext*)ptr;
    sqlite3_free(p->name);
    sqlite3_free(p);
}


SQLITE_PRIVATE void _ScalarFunction_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    ScalarUserContext *suc = (ScalarUserContext*)sqlite3_user_data(context);
    sqlite3_stmt *stmt;
    int slot = SCLRSLOT - suc->id;
    int nargs;

    stmt = sqlite3_get_auxdata(context, slot);

    if (!stmt) {
        sqlite3 *db = sqlite3_context_db_handle(context);
        char *select = 0;
        int rc;

        if (rc = sqlite3_prepare_v2(db, "select body from temp.scalarfunctions where id == ? and name == ?", -1, &stmt, 0)) {
            sqlite3_result_error_code(context, rc);
            sqlite3_result_error(context, sqlite3_errmsg(db), -1);
            return;
        }
        sqlite3_bind_int(stmt, 1, suc->id);
        sqlite3_bind_text(stmt, 2, suc->name, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            char *msg = sqlite3_mprintf("%s: Function has been deleted", suc->name);
            sqlite3_result_error(context, msg, -1);
            sqlite3_free(msg);
            sqlite3_finalize(stmt);
            return;
        }
        select = sqlite3_mprintf("select %s", sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        if (rc = sqlite3_prepare_v2(db, select, -1, &stmt, 0)) {
            sqlite3_result_error_code(context, rc);
            sqlite3_result_error(context, sqlite3_errmsg(db), -1);
            sqlite3_free(select);
            return;
        }
        sqlite3_free(select);
        sqlite3_set_auxdata(context, slot, stmt, (void*)sqlite3_finalize);
        stmt = sqlite3_get_auxdata(context, slot);
        if (!stmt) {
            sqlite3_result_error_nomem(context);
            return;
        }
    }
    nargs = sqlite3_bind_parameter_count(stmt);
    if (nargs != argc) {
        char* msg = sqlite3_mprintf("%s: %d arguments provided, %d expected", suc->name, argc, nargs);
        sqlite3_result_error(context, msg, -1);
        sqlite3_free(msg);
        return;
    }
    for (int i=0; i<argc; i++)
        sqlite3_bind_value(stmt, i+1, argv[i]);
    if (sqlite3_step(stmt)==SQLITE_ROW)
        sqlite3_result_value(context, sqlite3_column_value(stmt, 0));
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
}


SQLITE_PRIVATE void _ScalarDefine_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    ScalarUserContext *p;
    sqlite3 *db = sqlite3_context_db_handle(context);
    sqlite3_stmt *stmt = 0;
    char *msg;
    int id = 0;
    int rc = 0;

    if ((sqlite3_value_type(argv[0]) != SQLITE_TEXT) || (sqlite3_value_type(argv[1]) != SQLITE_TEXT)) {
        sqlite3_result_error(context, "DefineScalar(name, expression):  Parameters must be text", -1);
        return;
    }
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS temp.scalarfunctions\n"
                     "(\n"
                     " id    integer primary key,\n"
                     " name  text not null collate nocase unique,\n"
                     " body  text not null\n"
                     ")",
                     0, 0, 0);
    if (rc = sqlite3_prepare_v2(db, "insert into temp.scalarfunctions (name, body) values (?,?)", -1, &stmt, 0)) {
        sqlite3_result_error_code(context, rc);
        sqlite3_result_error(context, sqlite3_errmsg(db), -1);
        return;
    }
    sqlite3_bind_value(stmt, 1, argv[0]);
    sqlite3_bind_value(stmt, 2, argv[1]);
    if ((sqlite3_step(stmt) & 0xff) == SQLITE_CONSTRAINT) {
        sqlite3_finalize(stmt);
        if (rc = sqlite3_prepare_v2(db, "update temp.scalarfunctions set body=? where name == ?", -1, &stmt, 0)) {
            sqlite3_result_error_code(context, rc);
            sqlite3_result_error(context, sqlite3_errmsg(db), -1);
            return;
        }
        sqlite3_bind_value(stmt, 1, argv[1]);
        sqlite3_bind_value(stmt, 2, argv[0]);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        msg = sqlite3_mprintf("%s: function body definition updated", sqlite3_value_text(argv[0]));
        sqlite3_result_text(context, msg, -1, sqlite3_free);
        return;
    };
    sqlite3_finalize(stmt);
    if (sqlite3_prepare_v2(db, "select id from temp.scalarfunctions where name == ?", -1, &stmt, 0)) {
        sqlite3_result_error_code(context, rc);
        sqlite3_result_error(context, sqlite3_errmsg(db), -1);
        return;
    }
    sqlite3_bind_value(stmt, 1, argv[0]);
    sqlite3_step(stmt);
    id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    p = _ScalarUserContext_(id, sqlite3_value_text(argv[0]));
    rc = sqlite3_create_function_v2(db, p->name, -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)p, _ScalarFunction_, 0, 0, _ScalarUserContextDelete_);
    if (rc)
        msg = sqlite3_mprintf("%s: %s", sqlite3_value_text(argv[0]), sqlite3_errmsg(db));
    else
        msg = sqlite3_mprintf("%s: Function defined", p->name);
    sqlite3_result_text(context, msg, -1, sqlite3_free);
}


SQLITE_PRIVATE void _ScalarUnDefine_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite3 *db = sqlite3_context_db_handle(context);
    sqlite3_stmt *stmt = 0;
    char *msg;
    int rc = 0;

    if ((sqlite3_value_type(argv[0]) != SQLITE_TEXT)) {
        sqlite3_result_error(context, "UnDefineScalar(name): Parameter must be text", -1);
        return;
    }
    if (rc = sqlite3_prepare_v2(db, "select name from temp.scalarfunctions where name == ?", -1, &stmt, 0)) {
        sqlite3_result_error_code(context, rc);
        sqlite3_result_error(context, sqlite3_errmsg(db), -1);
        return;
    }
    sqlite3_bind_value(stmt, 1, argv[0]);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        msg = sqlite3_mprintf("%s: not a scalar function or already deleted", sqlite3_value_text(argv[0]));
        sqlite3_result_error(context, msg, -1);
        sqlite3_free(msg);
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);
    if (rc = sqlite3_prepare_v2(db, "delete from temp.scalarfunctions where name == ?", -1, &stmt, 0)) {
        sqlite3_result_error_code(context, rc);
        sqlite3_result_error(context, sqlite3_errmsg(db), -1);
        return;
    }
    sqlite3_bind_value(stmt, 1, argv[0]);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    rc = sqlite3_create_function_v2(db, (void*)sqlite3_value_text(argv[0]), -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, 0, 0, 0, 0);
    if (rc)
        msg = sqlite3_mprintf("%s: deleted: %s", sqlite3_value_text(argv[0]), sqlite3_errmsg(db));
    else
        msg = sqlite3_mprintf("%s: deleted", sqlite3_value_text(argv[0]));
    sqlite3_result_text(context, msg, -1, sqlite3_free);
}


SQLITE_PRIVATE void _CopyScalarFunctions_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    char *sql;
    sqlite3 *db = sqlite3_context_db_handle(context);
    sqlite3_stmt *stmt;

    if (sqlite3_value_type(argv[0]) != SQLITE_TEXT) {
        sqlite3_result_error(context, "Parameter must be text filename", -1);
        return;
    }

    sql = sqlite3_mprintf("attach %Q as tempFunctions", sqlite3_value_text(argv[0]));
    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
        sqlite3_result_error_code(context, sqlite3_extended_errcode(db));
        sqlite3_result_error(context, sqlite3_errmsg(db), -1);
        sqlite3_free(sql);
        return;
    }
    sqlite3_free(sql);

    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS temp.scalarfunctions\n"
                     "(\n"
                     " id    integer primary key,\n"
                     " name  text not null collate nocase unique,\n"
                     " body  text not null\n"
                     ")",
                     0, 0, 0);

    sqlite3_exec(db, "insert into temp.scalarfunctions (name, body) "
                     "select name, body from tempfunctions.scalarfunctions where true "
                     "on conflict (name) do update set body=excluded.body",
                         0, 0, 0);

    sqlite3_exec(db, "detach tempfunctions", 0, 0, 0);

    sqlite3_prepare_v2(db, "select id, name from temp.scalarfunctions", -1, &stmt, 0);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ScalarUserContext *p = _ScalarUserContext_(sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
        sqlite3_create_function_v2(db, (void*)sqlite3_column_text(stmt, 1), -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)p, _ScalarFunction_, 0, 0, 0);
    }
    sqlite3_finalize(stmt);
    sqlite3_result_text(context, sqlite3_mprintf("ScalarFunctions copied from %Q", sqlite3_value_text(argv[0])), -1, sqlite3_free);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_define_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
//  int wrSchema = 0;
//  int defensiveMode = 0;
    int nErr = 0;
    sqlite3_stmt *stmt;

    SQLITE_EXTENSION_INIT2(pApi);

//  sqlite3_db_config(db, SQLITE_DBCONFIG_DEFENSIVE, -1, &defensiveMode);
//  sqlite3_db_config(db, SQLITE_DBCONFIG_DEFENSIVE, 0, 0);
//  sqlite3_db_config(db, SQLITE_DBCONFIG_WRITABLE_SCHEMA, -1, &wrSchema);
//  sqlite3_db_config(db, SQLITE_DBCONFIG_WRITABLE_SCHEMA, 1, 0);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS temp.scalarfunctions\n"
                     "(\n"
                     " id    integer primary key,\n"
                     " name  text not null collate nocase unique,\n"
                     " body  text not null\n"
                     ")",
                     0, 0, 0);
//  sqlite3_db_config(db, SQLITE_DBCONFIG_WRITABLE_SCHEMA, wrSchema, 0);
//  sqlite3_db_config(db, SQLITE_DBCONFIG_DEFENSIVE, defensiveMode, 0);

    nErr += sqlite3_create_function(db, "DefineScalar", 2, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _ScalarDefine_, 0, 0);
    nErr += sqlite3_create_function(db, "UnDefineScalar", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _ScalarUnDefine_, 0, 0);
    nErr += sqlite3_create_function(db, "CopyScalarFunctions", 1, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _CopyScalarFunctions_, 0, 0);

    if (sqlite3_prepare_v2(db, "select id, name from temp.scalarfunctions", -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            ScalarUserContext *p = _ScalarUserContext_(sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1));
            nErr += sqlite3_create_function_v2(db, (void*)sqlite3_column_text(stmt, 1), -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)p, _ScalarFunction_, 0, 0, 0);
        }
        sqlite3_finalize(stmt);
    }
    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

