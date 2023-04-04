
//    MB/s Algo             (32-bit)
//   7.713 md2      1.000
// 160.199 sha256   1.000
// 177.285 sha512   1.000
// 177.368 sha384   1.000
// 291.325 sha1     1.000
// 402.604 md5      1.000
// 612.461 md4      1.000

//    MB/s Algo             (64-bit)
//   7.811 md2      1.000
// 173.183 sha256   1.000
// 285.443 sha512   1.000
// 285.835 sha384   1.000
// 408.461 sha1     1.000
// 412.746 md5      1.000
// 644.940 md4      1.000



#ifdef __cplusplus
extern "C" {
#endif


#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif
#include <windows.h>
#include <wincrypt.h>


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


// Hash sub ids

#ifndef ALG_SID_MD2
#define ALG_SID_MD2                     1
#endif
#ifndef ALG_SID_MD4
#define ALG_SID_MD4                     2
#endif
#ifndef ALG_SID_MD5
#define ALG_SID_MD5                     3
#endif
#ifndef ALG_SID_SHA1
#define ALG_SID_SHA1                    4
#endif
#ifndef ALG_SID_SHA_256
#define ALG_SID_SHA_256                 12
#endif
#ifndef ALG_SID_SHA_384
#define ALG_SID_SHA_384                 13
#endif
#ifndef ALG_SID_SHA_512
#define ALG_SID_SHA_512                 14
#endif

// algorithm identifier definitions

#ifndef CALG_MD2
#define CALG_MD2                (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_MD2)
#endif
#ifndef CALG_MD4
#define CALG_MD4                (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_MD4)
#endif
#ifndef CALG_MD5
#define CALG_MD5                (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_MD5)
#endif
#ifndef CALG_SHA1
#define CALG_SHA1               (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA1)
#endif
#ifndef CALG_SHA_256
#define CALG_SHA_256            (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_256)
#endif
#ifndef CALG_SHA_384
#define CALG_SHA_384            (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_384)
#endif
#ifndef CALG_SHA_512
#define CALG_SHA_512            (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA_512)
#endif

// define the RSA_AES provider

#ifndef PROV_RSA_AES
#define PROV_RSA_AES            24
#endif

//define the Enhanced RSA and AES Provider Strings

#ifndef MS_ENH_RSA_AES_PROV_A
#define MS_ENH_RSA_AES_PROV_A   "Microsoft Enhanced RSA and AES Cryptographic Provider"
#endif
#ifndef MS_ENH_RSA_AES_PROV_W
#define MS_ENH_RSA_AES_PROV_W   L"Microsoft Enhanced RSA and AES Cryptographic Provider"
#endif
#ifdef UNICODE
#ifndef MS_ENH_RSA_AES_PROV_XP
#define MS_ENH_RSA_AES_PROV_XP  MS_ENH_RSA_AES_PROV_XP_W
#endif
#ifndef MS_ENH_RSA_AES_PROV
#define MS_ENH_RSA_AES_PROV     MS_ENH_RSA_AES_PROV_W
#endif
#else
#ifndef MS_ENH_RSA_AES_PROV_XP
#define MS_ENH_RSA_AES_PROV_XP  MS_ENH_RSA_AES_PROV_XP_A
#endif
#ifndef MS_ENH_RSA_AES_PROV
#define MS_ENH_RSA_AES_PROV     MS_ENH_RSA_AES_PROV_A
#endif
#endif


typedef struct aggregateHash aggregateHash;
struct aggregateHash
{
    HCRYPTPROV cryptProv;
    HCRYPTHASH cryptHash;
    int iserror;
    int format;
};


SQLITE_PRIVATE void _HashFinal(sqlite3_context *context)
{
    aggregateHash* p = sqlite3_aggregate_context(context, sizeof(aggregateHash));
    char hex[16] = "0123456789ABCDEF";
    int i = 0;
    BYTE hash[128];
    DWORD cbHash = 128;
    char strHash[1024] = {0};

    if (p->iserror)
        return;

    if (!CryptGetHashParam(p->cryptHash, HP_HASHVAL, hash, &cbHash, 0))
    {
        CryptDestroyHash(p->cryptHash);
        CryptReleaseContext(p->cryptProv, 0);
        p->iserror = 1;
        return;
    }
    CryptDestroyHash(p->cryptHash);
    CryptReleaseContext(p->cryptProv, 0);
    if (p->format < 0)
    {
        sqlite3_result_blob(context, hash, cbHash, SQLITE_TRANSIENT);
        return;
    }
    for (i = 0; i < (long)cbHash; i++)
    {
        strHash[i * 2] = hex[hash[i] >> 4];
        strHash[(i * 2) + 1] = hex[hash[i] & 0xF];
    }
    sqlite3_result_text(context, strHash, cbHash*2, SQLITE_TRANSIENT);
    return;
}


SQLITE_PRIVATE void _HashStep(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    aggregateHash* p = sqlite3_aggregate_context(context, sizeof(aggregateHash));
    int datalen;
    void *data;
    intptr_t algo;
    int i;
    if (p->iserror)
        return;
    if ((p->cryptProv == 0) && (p->cryptHash == 0))
    {
        algo = (intptr_t)sqlite3_user_data(context);
        p->format = 1;
        if (algo < 0)
        {
            algo = -algo;
            p->format = -1;
        }
        if (!CryptAcquireContext(&(p->cryptProv), NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        {
            p->iserror = 1;
            return;
        }
        if (!CryptCreateHash(p->cryptProv, algo, 0, 0, &(p->cryptHash)))
        {
            CryptReleaseContext(p->cryptProv, 0);
            p->iserror = 1;
            return;
        }
    }
    for (i = 0; i < argc; i++)
    {
        data = (void*)sqlite3_value_text(argv[i]);
        datalen = sqlite3_value_bytes(argv[i]);
        if (datalen > 0)
        {
            if (!CryptHashData(p->cryptHash, data, datalen, 0))
            {
                CryptDestroyHash(p->cryptHash);
                CryptReleaseContext(p->cryptProv, 0);
                p->iserror = 1;
                return;
            }
        }
    }
}

SQLITE_PRIVATE void _ComputeHash(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    HCRYPTPROV cryptProv;
    HCRYPTHASH cryptHash;
    char hex[16] = "0123456789ABCDEF";
    int i = 0;
    int datalen;
    void *data;
    BYTE hash[128];
    DWORD cbHash = 128;
    char strHash[1024] = {0};
    int format = 1;
    intptr_t algo = (intptr_t)sqlite3_user_data(context);

    if (algo < 0)
    {
        algo = -algo;
        format = -1;
    }

    if (!CryptAcquireContext(&cryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
    {
        return;
    }
    if (!CryptCreateHash(cryptProv, algo, 0, 0, &cryptHash))
    {
        CryptReleaseContext(cryptProv, 0);
        return;
    }
    for (i = 0; i < argc; i++)
    {
        data = (void*)sqlite3_value_text(argv[i]);
        datalen = sqlite3_value_bytes(argv[i]);
        if (datalen > 0)
        {
            if (!CryptHashData(cryptHash, data, datalen, 0))
            {
                CryptDestroyHash(cryptHash);
                CryptReleaseContext(cryptProv, 0);
                return;
            }
        }
    }
    if (!CryptGetHashParam(cryptHash, HP_HASHVAL, hash, &cbHash, 0))
    {
        CryptDestroyHash(cryptHash);
        CryptReleaseContext(cryptProv, 0);
        return;
    }
    CryptDestroyHash(cryptHash);
    CryptReleaseContext(cryptProv, 0);
    if (format < 0)
    {
        sqlite3_result_blob(context, hash, cbHash, SQLITE_TRANSIENT);
        return;
    }
    for (i = 0; i < (long)cbHash; i++)
    {
        strHash[i * 2] = hex[hash[i] >> 4];
        strHash[(i * 2) + 1] = hex[hash[i] & 0xF];
    }
    sqlite3_result_text(context, strHash, cbHash*2, SQLITE_TRANSIENT);
    return;
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlhash_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

/*
** Scalar Functions
*/

    nErr += sqlite3_create_function(db, "md2",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD2,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "md4",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD4,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "md5",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD5,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha1",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA1,    _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha256",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_256, _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha384",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_384, _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "sha512",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_512, _ComputeHash, 0, 0);

    nErr += sqlite3_create_function(db, "hexmd2",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD2,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "hexmd4",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD4,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "hexmd5",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD5,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "hexsha1",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA1,    _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "hexsha256", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_256, _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "hexsha384", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_384, _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "hexsha512", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_512, _ComputeHash, 0, 0);

    nErr += sqlite3_create_function(db, "binmd2",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_MD2,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "binmd4",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_MD4,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "binmd5",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_MD5,     _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "binsha1",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA1,    _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "binsha256", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA_256, _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "binsha384", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA_384, _ComputeHash, 0, 0);
    nErr += sqlite3_create_function(db, "binsha512", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA_512, _ComputeHash, 0, 0);

/*
** Aggregate Functions by the same name
*/

    nErr += sqlite3_create_function(db, "md2",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD2,      0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "md4",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD4,      0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "md5",       -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD5,      0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "sha1",      -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA1,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "sha256",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_256,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "sha384",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_384,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "sha512",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_512,  0, _HashStep, _HashFinal);

    nErr += sqlite3_create_function(db, "hexmd2",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD2,      0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "hexmd4",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD4,      0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "hexmd5",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_MD5,      0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "hexsha1",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA1,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "hexsha256", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_256,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "hexsha384", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_384,  0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "hexsha512", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)CALG_SHA_512,  0, _HashStep, _HashFinal);

    nErr += sqlite3_create_function(db, "binmd2",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_MD2,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "binmd4",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_MD4,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "binmd5",    -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_MD5,     0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "binsha1",   -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA1,    0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "binsha256", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA_256, 0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "binsha384", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA_384, 0, _HashStep, _HashFinal);
    nErr += sqlite3_create_function(db, "binsha512", -1, SQLITE_UTF8|SQLITE_DETERMINISTIC,  (void*)-CALG_SHA_512, 0, _HashStep, _HashFinal);

/*
** If any error return an error
*/

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

#ifdef __cplusplus
}
#endif
