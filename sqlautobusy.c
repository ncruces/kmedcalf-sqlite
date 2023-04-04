/*
** This extension automatically sets the busy_timeout to 30 seconds when loaded
**
** It should be initialized with coreinit.c or other autoextension mechanism
*/

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlautobusy_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    SQLITE_EXTENSION_INIT2(pApi);

    return sqlite3_busy_timeout(db, 30000);
}

#ifdef __cplusplus
}
#endif
