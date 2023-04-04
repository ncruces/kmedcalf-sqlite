/*
** Function UNHEX(arg) -> blob
**
** Decodes the arg which must be an even number of hexadecimal characters into a blob and returns the blob
** Return NULL if arg wrong size or not all characters are hexadecimal
**
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef SQLITE_CORE
    #include "sqlite3.h"
#else
    #include "sqlite3ext.h"
    SQLITE_EXTENSION_INIT1
#endif

static void _unhexFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    unsigned char* data = (unsigned char*)sqlite3_value_text(argv[0]);
    unsigned char* blob;
    unsigned char* stuff;
    int olength = sqlite3_value_bytes(argv[0]);
    int length;
    unsigned char buffer[4] = {0};

    if ((olength % 2 != 0) || (olength < 2))
    {
        return;
    }
    blob = sqlite3_malloc(olength / 2);
    if (blob == 0)
    {
        sqlite3_result_error_nomem(context);
        return;
    }
    stuff = blob;
    length = olength;
    while (length > 0)
    {
        memcpy(buffer, data, 2);
        if ((!isxdigit(buffer[0])) || (!isxdigit(buffer[1])))
        {
            break;
        }
        *stuff = (unsigned char)strtol((void*)buffer, NULL, 16);
        stuff++;
        data += 2;
        length -= 2;
    }
    if (length == 0)
    {
        sqlite3_result_blob(context, blob, olength/2, sqlite3_free);
    }
    else
    {
        sqlite3_free(blob);
    }
}

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlunhex_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    SQLITE_EXTENSION_INIT2(pApi);

    return sqlite3_create_function(db, "UNHEX", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _unhexFunc,  0, 0);
}

#ifdef __cplusplus
}
#endif
