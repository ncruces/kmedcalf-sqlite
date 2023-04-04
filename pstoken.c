/*
  The below is a public domain SQLite extension for writing PostScript
  tokens as output. This might be useful if you want to export data from
  SQLite to use with PostScript, e.g. to make graphics. (The SQLite
  command shell can already output many other formats, but not
  PostScript, so here it is in case you want to use SQLite with
  PostScript.) Note that the PostScript interpreter may have limits to
  numeric values and lengths of strings which are less than SQLite.
  Write PSTOKEN(...) replace ... with any number of arguments. It is
  written as a executable array (procedure) token, which may be read by
  the "token" operator. (You may also write something like [(%stdin)
  run] if you want to just put it into an array.) Strings and blobs are
  both written as PostScript strings; if you need to distinguish it, you
  must add another column.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void add_token(sqlite3_str*str,sqlite3_value*v) {
  const unsigned char*c;
  int j,n;
  switch(sqlite3_value_type(v)) {
    case SQLITE_NULL:
      sqlite3_str_append(str," //null",7);
      break;
    case SQLITE_FLOAT:
      sqlite3_str_appendf(str," %!f",sqlite3_value_double(v));
      break;
    case SQLITE_INTEGER:
      sqlite3_str_appendf(str," %lld",sqlite3_value_int64(v));
      break;
    case SQLITE_TEXT:
      c=sqlite3_value_text(v);
      n=sqlite3_value_bytes(v);
      sqlite3_str_append(str," (",2);
      for(j=0;j<n;j++) {
        if(c[j]=='(' || c[j]==')' || c[j]=='\\') sqlite3_str_appendchar(str,1,'\\');
        if(c[j]>=32 && c[j]<127) sqlite3_str_appendchar(str,1,c[j]); else sqlite3_str_appendf(str,"\\%03o",c[j]);
      }
      sqlite3_str_appendchar(str,1,')');
      break;
    case SQLITE_BLOB:
      c=sqlite3_value_blob(v);
      n=sqlite3_value_bytes(v);
      sqlite3_str_append(str," <",2);
      for(j=0;j<n;j++) sqlite3_str_appendf(str,"%02X",c[j]);
      sqlite3_str_appendchar(str,1,'>');
      break;
  }
}

static void fn_pstoken(sqlite3_context*cxt,int argc,sqlite3_value**argv) {
  sqlite3_str*str=sqlite3_str_new(sqlite3_context_db_handle(cxt));
  int i;
  sqlite3_str_appendchar(str,1,'{');
  for(i=0;i<argc;i++) add_token(str,argv[i]);
  sqlite3_str_append(str," }",2);
  if(i=sqlite3_str_errcode(str)) {
    sqlite3_free(sqlite3_str_finish(str));
    if(i==SQLITE_TOOBIG) sqlite3_result_error_toobig(cxt);
    if(i==SQLITE_NOMEM) sqlite3_result_error_nomem(cxt);
  } else {
    sqlite3_result_text(cxt,sqlite3_str_finish(str),-1,sqlite3_free);
  }
}

static void fn_pstoken1(sqlite3_context*cxt,int argc,sqlite3_value**argv) {
  sqlite3_str*str=sqlite3_str_new(sqlite3_context_db_handle(cxt));
  int i;
  add_token(str,*argv);
  if(i=sqlite3_str_errcode(str)) {
    sqlite3_free(sqlite3_str_finish(str));
    if(i==SQLITE_TOOBIG) sqlite3_result_error_toobig(cxt);
    if(i==SQLITE_NOMEM) sqlite3_result_error_nomem(cxt);
  } else {
    sqlite3_result_text(cxt,sqlite3_str_finish(str),-1,sqlite3_free);
  }
}

static void st_pstokena(sqlite3_context*cxt,int argc,sqlite3_value**argv) {
  sqlite3_str**con=sqlite3_aggregate_context(cxt,sizeof(sqlite3_str*));
  sqlite3_str*str;
  if(!con) {
    sqlite3_result_error_nomem(cxt);
    return;
  }
  str=*con;
  if(!str) sqlite3_str_appendchar(str=*con=sqlite3_str_new(sqlite3_context_db_handle(cxt)),1,'{');
  add_token(str,*argv);
}

static void fi_pstokena(sqlite3_context*cxt) {
  sqlite3_str**con=sqlite3_aggregate_context(cxt,0);
  sqlite3_str*str;
  int i;
  if(!con || !*con) {
    sqlite3_result_text(cxt,"{}",-1,0);
    return;
  }
  str=*con;
  sqlite3_str_append(str," }",2);
  if(i=sqlite3_str_errcode(str)) {
    sqlite3_free(sqlite3_str_finish(str));
    if(i==SQLITE_TOOBIG) sqlite3_result_error_toobig(cxt);
    if(i==SQLITE_NOMEM) sqlite3_result_error_nomem(cxt);
  } else {
    sqlite3_result_text(cxt,sqlite3_str_finish(str),-1,sqlite3_free);
  }
}

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_pstoken_init(sqlite3*db,const char**err,const struct sqlite3_api_routines*api) {
  SQLITE_EXTENSION_INIT2(api);
  sqlite3_create_function(db,"PSTOKEN",-1,SQLITE_UTF8|SQLITE_DETERMINISTIC,0,fn_pstoken,0,0);
  sqlite3_create_function(db,"PSTOKEN1",1,SQLITE_UTF8|SQLITE_DETERMINISTIC,0,fn_pstoken1,0,0);
  sqlite3_create_function(db,"PSTOKENA",1,SQLITE_UTF8|SQLITE_DETERMINISTIC,0,0,st_pstokena,fi_pstokena);
  return SQLITE_OK;
}

