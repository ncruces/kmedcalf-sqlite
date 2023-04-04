/*
 * Implements lowerlimit, upperlimit functions
 *
 * Written 2022 by Keith Medcalf
 * Given to the Public Domain.  If you break it, you own all the pieces.
 */

#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>


/*
** Validate UTF-8
** Return -1 if invalid else length
*/
static long long _utf8IsValid_(const char *string)
{
    long long length = 0;
    unsigned char *start;
    int trailing = 0;
    unsigned char c;

    start = (unsigned char *)string;
    while ((c = *start))
    {
        if (trailing)
        {
            if ((c & 0xC0) == 0x80)
            {
                trailing--;
                start++;
                length++;
                continue;
            }
            else
            {
                length = -1;
                break;
            }
        }
        if ((c & 0x80) == 0)
        {
            start++;
            length++;
            continue;
        }
        if ((c & 0xE0) == 0xC0)
        {
            trailing = 1;
            start++;
            length++;
            continue;
        }
        if ((c & 0xF0) == 0xE0)
        {
            trailing = 2;
            start++;
            length++;
            continue;
        }
        if ((c & 0xF8) == 0xF0)
        {
            trailing = 3;
            start++;
            length++;
            continue;
        }
#if 0   // UTF-8 does not encode sequences longer than 4 bytes (yet)
        if ((c & 0xFC) == 0xF8)
        {
            trailing = 4;
            start++;
            length++;
            continue;
        }
        if ((c & 0xFE) == 0xFC)
        {
            trailing = 5;
            start++;
            length++;
            continue;
        }
        if ((c & 0xFF) == 0xFE)
        {
            trailing = 6;
            start++;
            length++;
            continue;
        }
        if ((c & 0xFF) == 0xFF)
        {
            trailing = 7;
            start++;
            length++;
            continue;
        }
#endif
        length = -1;
        break;
    }
    return length;
}


static void _istextvalid_(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    sqlite3_result_int(ctx, _utf8IsValid_(sqlite3_value_text(argv[0])) != -1);
}

static void _lowerprefix_(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    const char* in;
    char *ll;
    int bytes;

    in = sqlite3_value_text(argv[0]);
    bytes = sqlite3_value_bytes(argv[0]);
    ll = sqlite3_malloc(bytes);
    memcpy(ll, in, bytes);
    if (_utf8IsValid_(ll) == -1)
        sqlite3_result_error(ctx, "Invalid Text", -1);
    else
        sqlite3_result_text(ctx, ll, bytes, sqlite3_free);
}

static void _upperprefix_(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    char maxchar[] = {0xF4, 0x8F, 0xBF, 0xBF, 0x00}; // Max Unicode Codepoint and a null byte
    const char* in;
    char *ll;
    int bytes;

    in = sqlite3_value_text(argv[0]);
    bytes = sqlite3_value_bytes(argv[0]);
    ll = sqlite3_malloc(bytes+4);
    memcpy(ll, in, bytes);
    memcpy(&ll[bytes], maxchar, 5);
    if (_utf8IsValid_(ll) == -1)
        sqlite3_result_error(ctx, "Invalid Text", -1);
    else
        sqlite3_result_text(ctx, ll, bytes+4, sqlite3_free);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqltext_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = 0;
    SQLITE_EXTENSION_INIT2(pApi);
    rc = sqlite3_create_function(db, "LowerPrefix", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _lowerprefix_, 0, 0);
    rc += sqlite3_create_function(db, "UpperPrefix", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _upperprefix_, 0, 0);
    rc += sqlite3_create_function(db, "IsTextValid", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _istextvalid_, 0, 0);

    if (rc)
        return SQLITE_ERROR;
    else
        return SQLITE_OK;
}

