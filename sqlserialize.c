
#include <string.h>

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1

#ifndef SQLITE_PRIVATE
    #define SQLITE_PRIVATE static
#endif


static void _serialize_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    long long plen = argc + 1;
    unsigned char out[256];
    char *p;
    char *buffer;

    if (argc < 1 || argc > 255)
    {
        sqlite3_result_error(context, "serialize: must have between 1 and 255 arguments", -1);
        return;
    }

    out[0] = (unsigned char)argc;
    for (int i=0; i<argc; i++)
    {
        unsigned char code;
        switch (sqlite3_value_type(argv[i]))
        {
            case SQLITE_NULL:
                code = 1;
                break;
            case SQLITE_INTEGER:
            {
                sqlite_int64 value = sqlite3_value_int64(argv[i]);
                if (value == 0) code = 2;
                else if (value == 1) code = 3;
                else if (value >= -128 && value <= 127) code = 4;
                else if (value >= -32768 && value <= 32767) code = 5;
                else if (value >= -2147483648 && value <= 2147483647) code = 6;
                else code = 7;
                if (code > 3) plen += ((code - 3) + (code == 6) + 4 * (code == 7));
                break;
            }
            case SQLITE_FLOAT:
                code = 8;
                plen += 8;
                break;
            case SQLITE_TEXT:
                code = 9;
                sqlite3_value_text(argv[i]);
                plen += (4 + sqlite3_value_bytes(argv[i]));
                break;
            case SQLITE_BLOB:
                code = 10;
                plen += (4 + sqlite3_value_bytes(argv[i]));
                break;
        }
        out[i+1] = code;
    }
    buffer = sqlite3_malloc(plen);
    memcpy(buffer, out, argc+1);
    p = buffer + argc + 1;

    for (int i=0; i<argc; i++)
    {
        switch (sqlite3_value_type(argv[i]))
        {
            case SQLITE_NULL:
            {
                break;
            }
            case SQLITE_INTEGER:
            {
                int code;
                char buf[8];
                sqlite_int64 value = sqlite3_value_int64(argv[i]);

                if (value == 0) break;
                else if (value == 1) break;
                else if (value >= -128 && value <= 127) code = 1;
                else if (value >= -32768 && value <= 32767) code = 2;
                else if (value >= -2147483648 && value <= 2147483647) code = 4;
                else code = 8;
                for(int j=0; j<8; j++)
                    buf[j] = ((char*)&value)[7-j];
                memcpy(p, &buf[8-code], code);
                p += code;
                break;
            }
            case SQLITE_FLOAT:
            {
                double value = sqlite3_value_double(argv[i]);
                memcpy(p, &value, 8); /* do not word swap mixed-endian */
                p += 8;
                break;
            }
            case SQLITE_TEXT:
            {
                char buf[4];
                long bytes = sqlite3_value_bytes(argv[i]);
                for(int j=0; j<4; j++)
                    buf[j] = ((char*)&bytes)[3-j];
                memcpy(p, buf, 4);
                p += 4;
                memcpy(p, sqlite3_value_text(argv[i]), bytes);
                p += bytes;
                break;
            }
            case SQLITE_BLOB:
            {
                char buf[4];
                long bytes = sqlite3_value_bytes(argv[i]);
                for(int j=0; j<4; j++)
                    buf[j] = ((char*)&bytes)[3-j];
                memcpy(p, buf, 4);
                p += 4;
                memcpy(p, sqlite3_value_text(argv[i]), bytes);
                p += bytes;
                break;
            }
        }
    }
    sqlite3_result_blob(context, buffer, plen, sqlite3_free);
}

typedef struct serialconcatCtx serialconcatCtx;
struct serialconcatCtx
{
    long long psize;
    char header[256];
    char *payload;
};


static void _serialconcatstep_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    serialconcatCtx *ctx = sqlite3_aggregate_context(context, sizeof(serialconcatCtx));
    int counter;
    if (ctx && (unsigned char)ctx->header[0] == 255)
    {
        sqlite3_result_error(context, "serial_concat: cannot concatenate more than 255 items", -1);
        return;
    }
    (unsigned char)ctx->header[0]++;
    counter = (unsigned char)ctx->header[0];

    switch (sqlite3_value_type(argv[0]))
    {
        case SQLITE_NULL:
        {
            ctx->header[counter] = 1;
            break;
        }
        case SQLITE_INTEGER:
        {
            sqlite_int64 value = sqlite3_value_int64(argv[0]);
            char buf[8];
            long plen = 0;
            int code;

            if (value == 0) code = 2;
            else if (value == 1) code = 3;
            else if (value >= -128 && value <= 127) code = 4;
            else if (value >= -32768 && value <= 32767) code = 5;
            else if (value >= -2147483648 && value <= 2147483647) code = 6;
            else code = 7;
            ctx->header[counter] = code;
            if (code > 3) plen = ((code - 3) + (code == 6) + 4 * (code == 7));
            if (plen)
            {
                for(int j=0; j<8; j++)
                    buf[j] = ((char*)&value)[7-j];
                ctx->payload = sqlite3_realloc(ctx->payload, ctx->psize + plen);
                if (!ctx->payload)
                {
                    sqlite3_result_error_nomem(context);
                    return;
                }
                memcpy(ctx->payload + ctx->psize, &buf[8-plen], plen);
                ctx->psize += plen;
            }
            break;
        }
        case SQLITE_FLOAT:
        {
            double value = sqlite3_value_double(argv[0]);
            ctx->header[counter] = 8;
            ctx->payload = sqlite3_realloc(ctx->payload, ctx->psize + 8);
            if (!ctx->payload)
            {
                sqlite3_result_error_nomem(context);
                return;
            }
            memcpy(ctx->payload + ctx->psize, &value, 8); /* do not word swap mixed-endian */
            ctx->psize += 8;
            break;
        }
        case SQLITE_TEXT:
        {
            long bytes;
            char buf[4];
            ctx->header[counter] = 9;
            sqlite3_value_text(argv[0]);
            bytes = sqlite3_value_bytes(argv[0]);
            ctx->payload = sqlite3_realloc(ctx->payload, ctx->psize + bytes + 4);
            if (!ctx->payload)
            {
                sqlite3_result_error_nomem(context);
                return;
            }
            ctx->payload = sqlite3_realloc(ctx->payload, ctx->psize + bytes + 4);
            if (!ctx->payload)
            {
                sqlite3_result_error_nomem(context);
                return;
            }
            for(int j=0; j<4; j++)
                buf[j] = ((char*)&bytes)[3-j];
            memcpy(ctx->payload + ctx->psize, &buf, 4);
            memcpy(ctx->payload + ctx->psize + 4, sqlite3_value_text(argv[0]), bytes);
            ctx->psize += bytes + 4;
            break;
        }
        case SQLITE_BLOB:
        {
            long bytes;
            char buf[4];
            ctx->header[counter] = 9;
            bytes = sqlite3_value_bytes(argv[0]);
            ctx->payload = sqlite3_realloc(ctx->payload, ctx->psize + bytes + 4);
            if (!ctx->payload)
            {
                sqlite3_result_error_nomem(context);
                return;
            }
            ctx->payload = sqlite3_realloc(ctx->payload, ctx->psize + bytes + 4);
            if (!ctx->payload)
            {
                sqlite3_result_error_nomem(context);
                return;
            }
            for(int j=0; j<4; j++)
                buf[j] = ((char*)&bytes)[3-j];
            memcpy(ctx->payload + ctx->psize, &buf, 4);
            memcpy(ctx->payload + ctx->psize + 4, sqlite3_value_blob(argv[0]), bytes);
            ctx->psize += bytes + 4;
            break;
        }
    }
}

static void _serialconcatfinal_(sqlite3_context *context)
{
    serialconcatCtx *ctx = sqlite3_aggregate_context(context, 0);
    char *temp;

    if (!ctx)
        return;
    temp = sqlite3_malloc(ctx->psize + (unsigned char)ctx->header[0] + 1);
    if (!temp)
    {
        sqlite3_result_error_nomem(context);
        return;
    }
    memcpy(temp, ctx->header, (unsigned char)ctx->header[0] + 1);
    memcpy(temp + (unsigned char)ctx->header[0] + 1, ctx->payload, ctx->psize);
    sqlite3_result_blob(context, temp, ctx->psize + (unsigned char)ctx->header[0] + 1, sqlite3_free);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlserialize_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

    nErr += sqlite3_create_function(db, "serialize",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _serialize_,   0, 0);

    nErr += sqlite3_create_function(db, "serial_concat", 1, SQLITE_UTF8|SQLITE_INNOCUOUS,  0, 0,   _serialconcatstep_, _serialconcatfinal_);

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

