/*
** Implements some Windows-specific APIs as SQLite3 UDFs
**
** Uuid Functions were  removed because Windows is broken
** UuidCreateV4         returns a new UUID v4 BLOB
** UuidCreateV1         returns a new UUID v1 BLOB based on the MAC
** UuidStringCreateV4   returns a new UUID v4 String
** UuidStringCreateV1   returns a new UUID v1 String based on the MAC
** UuidToString         converts UUID from BLOB to String Format
** UuidFromString       converts UUID from String Format to BLOB
**
** ComputerName()       returns the Computer Name
** ComputerFQDN()       returns the Fully Qualified Computer Name
** UserName()           returns the current User Name
** UserSID()            returns the current User SID String
** LookupName()         returns the SID String for a User or Group Name
** LookupSID()          returns the User or Group name corresponding to the SID String
** TokenHasName()       returns whether the given Group/User name is included in the current process token
** TokenHasSID()        returns whether the given SID is included in the current process token
** GetFileAttributes()  return the attributes of the specified file
** DoesFileExist()      return whether the given file exists or not
** timeResolution()     adjust Multimedia timer resolution (clock update frequency)
** GetProcessImage*Path returns the File or Native path of the current process image
** GetLocaleName        returns the current user or system locale
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

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#pragma comment (lib, "advapi32")
#pragma comment (lib, "ntdll")
#pragma comment (lib, "rpcrt4")

#include <windows.h>

WINADVAPI BOOL WINAPI ConvertSidToStringSidW(PSID Sid, LPWSTR* StringSid);
WINADVAPI BOOL WINAPI ConvertStringSidToSidW(LPWSTR StringSid, PSID *Sid);
NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(OUT PULONG MinimumResolution,
                                               OUT PULONG MaximumResolution,
                                               OUT PULONG ActualResolution);
NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution,
                                             BOOLEAN SetResolution,
                                             PULONG CurrentResolution);
#if defined(__GNUC__)
WINBASEAPI WINBOOL WINAPI QueryFullProcessImageNameW (HANDLE hProcess, DWORD dwFlags, LPWSTR lpExeName, PDWORD lpdwSize);
#endif

#ifndef SQLITE_FUNC_SLOCHNG
#define SQLITE_FUNC_SLOCHNG 0x2000
#endif

/*
** Set the System Timer Frequency
**
** Magic Values:  >0 -- set to highest resolution (usually 0.0005000 seconds)
**                <0 -- set to lowest resolution  (usually 0.0156250 seconds)
**
** Passing no arguments or value 0 will not change the resolution
**
** Returns:      current update frequency in ms
*/

static void _periodFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
    ULONG MinResolution;
    ULONG MaxResolution;
    ULONG ActResolution;
    sqlite_int64 res;

    NtQueryTimerResolution(&MinResolution, &MaxResolution, &ActResolution);

    if (argc == 1 && sqlite3_value_type(argv[0]) != SQLITE_NULL)
    {
        res = sqlite3_value_int64(argv[0]);
        if (res>0) res = MaxResolution;
        if (res<0) res = MinResolution;
        if (res)
            NtSetTimerResolution(res, 1, &ActResolution);
    }
    sqlite3_result_double(context, ActResolution/1e7);
}

#if 0
/*
** UUID Functions
*/

/*
** The below two proxy functions are required to maintain correct linkage
** and stack cleanup for variable calls to RPC functions
*/

static long _UuidCreateV4(UUID* dest)
{
    return UuidCreate(dest);
}

static long _UuidCreateV1(UUID* dest)
{
    return UuidCreateSequential(dest);
}

SQLITE_PRIVATE void _NewUuidBlob(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    UUID buffer;
    long (*funcptr)(UUID*) = (void*)sqlite3_user_data(context);
    long rc = funcptr(&buffer);
    if (rc == 0)
        sqlite3_result_blob(context, (void*)&buffer, sizeof(UUID), SQLITE_TRANSIENT);
}

SQLITE_PRIVATE void _NewUuidString(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    UUID buffer;
    unsigned char *charbuf = 0;
    long (*funcptr)(UUID*) = (void*)sqlite3_user_data(context);
    long rc = funcptr(&buffer);
    if (rc == 0)
    {
        rc = UuidToStringA(&buffer, &charbuf);
        if (rc == 0)
            sqlite3_result_text(context, charbuf, -1, SQLITE_TRANSIENT);
    }
    if (charbuf != 0)
        RpcStringFreeA(&charbuf);
}

SQLITE_PRIVATE void _uuidBlobToString(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    unsigned char *charbuf = 0;
    UUID *uuidptr;
    long rc;
    if (sqlite3_value_type(argv[0]) != SQLITE_BLOB)
        return;
    if (sqlite3_value_bytes(argv[0]) != 16)
        return;
    uuidptr = (UUID*)sqlite3_value_blob(argv[0]);
    rc = UuidToStringA(uuidptr, &charbuf);
    if (rc == 0)
        sqlite3_result_text(context, charbuf, -1, SQLITE_TRANSIENT);
    if (charbuf != 0)
        RpcStringFreeA(&charbuf);
}

SQLITE_PRIVATE void _uuidStringToBlob(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    UUID buffer;
    unsigned char *charbuf = (void*)sqlite3_value_text(argv[0]);
    long rc = UuidFromStringA(charbuf, &buffer);
    if (rc == 0)
        sqlite3_result_blob(context, (void*)&buffer, 16, SQLITE_TRANSIENT);
}
#endif


/*
**  QueryFullProcessImageName
*/

SQLITE_PRIVATE void _ProcessImageName_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[4096];
    DWORD size = sizeof(buffer) - 2;

    if (QueryFullProcessImageNameW(GetCurrentProcess(), (intptr_t)sqlite3_user_data(context), buffer, &size) != 0)
        sqlite3_result_text16(context, buffer, -1, SQLITE_TRANSIENT);
}


/*
** Windows Security Functions
*/

SQLITE_PRIVATE void _ComputerName(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[1024];
    DWORD size = 1023;

    if (GetComputerNameExW((intptr_t)sqlite3_user_data(context), buffer, &size) != 0)
        sqlite3_result_text16(context, buffer, -1, SQLITE_TRANSIENT);
}

SQLITE_PRIVATE void _UserName(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[1024];
    DWORD size = 1023;
    wchar_t buffer2[1024];
    DWORD size2 = 1023;
    wchar_t domain[1024];
    DWORD dsize = 1023;
    char sidbuffer[1024];
    DWORD sidsize = 1023;
    SID_NAME_USE siduse = 0;
    int rc;
    rc = GetUserNameW(buffer, &size);
    if (rc != 0)
    {
        rc = LookupAccountNameW(NULL, buffer, (PSID)sidbuffer, &sidsize, domain, &dsize, &siduse);
        if (rc != 0)
        {
            dsize = 1023;
            rc = LookupAccountSidW(NULL, sidbuffer, buffer2, &size2, domain, &dsize, &siduse);
            if (rc != 0)
            {
                wcscat(domain, L"\\");
                wcscat(domain, buffer2);
                sqlite3_result_text16(context, domain, -1, SQLITE_TRANSIENT);
                return;
            }
        }
        sqlite3_result_text16(context, buffer, -1, SQLITE_TRANSIENT);
    }
}

SQLITE_PRIVATE void _UserSid(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[1024];
    DWORD size = 1023;
    wchar_t domain[1024];
    DWORD dsize = 1023;
    char sidbuffer[1024];
    DWORD sidsize = 1023;
    LPWSTR sidString = 0;
    SID_NAME_USE siduse = 0;
    int rc;
    rc = GetUserNameW(buffer, &size);
    if (rc != 0)
    {
        rc = LookupAccountNameW(NULL, buffer, (PSID)sidbuffer, &sidsize, domain, &dsize, &siduse);
        if (rc != 0)
        {
            rc = ConvertSidToStringSidW((PSID)sidbuffer, &sidString);
            if (rc != 0)
            {
                sqlite3_result_text16(context, sidString, -1, SQLITE_TRANSIENT);
                LocalFree(sidString);
            }
        }
    }
}

SQLITE_PRIVATE void _LookupName(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[1024];
    DWORD size = 1023;
    wchar_t domain[1024];
    DWORD dsize = 1023;
    SID_NAME_USE siduse = 0;
    LPWSTR sidString = 0;
    int rc;
    rc = LookupAccountNameW(NULL, sqlite3_value_text16(argv[0]), (PSID)buffer, &size, domain, &dsize, &siduse);
    if (rc != 0)
    {
        rc = ConvertSidToStringSidW((PSID)buffer, &sidString);
        if (rc != 0)
        {
            sqlite3_result_text16(context, sidString, -1, SQLITE_TRANSIENT);
            LocalFree(sidString);
        }
    }
}

SQLITE_PRIVATE void _LookupSid(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[1024];
    DWORD size = 1023;
    wchar_t domain[1024];
    DWORD dsize = 1023;
    SID_NAME_USE siduse = 0;
    PSID sidptr;
    LPWSTR sidString = 0;
    int rc;
    rc = ConvertStringSidToSidW((LPWSTR)sqlite3_value_text16(argv[0]), &sidptr);
    if (rc != 0)
    {
        rc = LookupAccountSidW(NULL, sidptr, buffer, &size, domain, &dsize, &siduse);
        LocalFree(sidptr);
        if (rc != 0)
        {
            wcscat(domain, L"\\");
            wcscat(domain, buffer);
            sqlite3_result_text16(context, domain, -1, SQLITE_TRANSIENT);
        }
    }
}

SQLITE_PRIVATE void _CheckTokenSID(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    PSID sidptr;
    int res;
    int rc;
    rc = ConvertStringSidToSidW((LPWSTR)sqlite3_value_text16(argv[0]), &sidptr);
    if (rc != 0)
    {
        rc = CheckTokenMembership(NULL, sidptr, &res);
        LocalFree(sidptr);
        if (rc != 0)
            sqlite3_result_int(context, res);
    }
}

SQLITE_PRIVATE void _CheckTokenName(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[1024];
    DWORD size = 1023;
    wchar_t domain[1024];
    DWORD dsize = 1023;
    SID_NAME_USE siduse = 0;
    LPWSTR sidString = 0;
    int rc;
    int res;
    rc = LookupAccountNameW(NULL, sqlite3_value_text16(argv[0]), (PSID)buffer, &size, domain, &dsize, &siduse);
    if (rc != 0)
    {
        rc = CheckTokenMembership(NULL, (PSID)buffer, &res);
        if (rc != 0)
            sqlite3_result_int(context, res);
    }
}

SQLITE_PRIVATE void _GetFileAttributes(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    long rc;
    rc = GetFileAttributesW(sqlite3_value_text16(argv[0]));
    if (rc < 0)
        rc = - (long)GetLastError();
    sqlite3_result_int(context, rc);
}

SQLITE_PRIVATE void _DoesFileExist(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    HANDLE h;
    long rc = 1;
    h = CreateFileW(sqlite3_value_text16(argv[0]), 0, 7, NULL, 3, 0x80, NULL);
    if (h == INVALID_HANDLE_VALUE)
        rc = - (long)GetLastError();
    else
        CloseHandle(h);
    sqlite3_result_int(context, rc);
}

SQLITE_PRIVATE void _LocaleName_(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    wchar_t buffer[32] = {0};
    GetUserDefaultLocaleName(buffer, 32);
    if (buffer[2] == L'-') buffer[2] = L'_';
    sqlite3_result_text16(context, buffer, -1, SQLITE_TRANSIENT);
}


#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_sqlfwin_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;

    SQLITE_EXTENSION_INIT2(pApi);

    nErr += sqlite3_create_function(db, "timeResolution",   -1, SQLITE_UTF8|SQLITE_DIRECTONLY,                                         0,        _periodFunc,       0, 0);

    nErr += sqlite3_create_function(db, "GetProcessImageFilePath", 0, SQLITE_UTF8|SQLITE_INNOCUOUS, 0, _ProcessImageName_, 0, 0);
    nErr += sqlite3_create_function(db, "GetProcessImageNativePath", 0, SQLITE_UTF8|SQLITE_INNOCUOUS, (void*)1, _ProcessImageName_, 0, 0);

    nErr += sqlite3_create_function(db, "ComputerName",      0, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _ComputerName,     0, 0);
    nErr += sqlite3_create_function(db, "ComputerFQDN",      0, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, (void*)3, _ComputerName,     0, 0);
    nErr += sqlite3_create_function(db, "UserName",          0, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _UserName,         0, 0);
    nErr += sqlite3_create_function(db, "UserSid",           0, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _UserSid,          0, 0);
    nErr += sqlite3_create_function(db, "LookupName",        1, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _LookupName,       0, 0);
    nErr += sqlite3_create_function(db, "LookupSid",         1, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _LookupSid,        0, 0);
    nErr += sqlite3_create_function(db, "TokenHasSID",       1, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _CheckTokenSID,    0, 0);
    nErr += sqlite3_create_function(db, "TokenHasName",      1, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _CheckTokenName,   0, 0);
    nErr += sqlite3_create_function(db, "GetFileAttributes", 1, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _GetFileAttributes,0, 0);
    nErr += sqlite3_create_function(db, "DoesFileExist",     1, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _DoesFileExist,    0, 0);

    nErr += sqlite3_create_function(db, "GetLocaleName",     0, SQLITE_UTF8|SQLITE_FUNC_SLOCHNG|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0,        _LocaleName_,      0, 0);

#if 0
    nErr += sqlite3_create_function(db, "UuidCreateV4",      0, SQLITE_ANY|SQLITE_INNOCUOUS,    (void*)_UuidCreateV4,        _NewUuidBlob,      0, 0);
    nErr += sqlite3_create_function(db, "UuidCreateV1",      0, SQLITE_ANY|SQLITE_INNOCUOUS,    (void*)_UuidCreateV1,        _NewUuidBlob,      0, 0);
    nErr += sqlite3_create_function(db, "UuidStringCreateV4",0, SQLITE_ANY|SQLITE_INNOCUOUS,    (void*)_UuidCreateV4,        _NewUuidString,    0, 0);
    nErr += sqlite3_create_function(db, "UuidStringCreateV1",0, SQLITE_ANY|SQLITE_INNOCUOUS,    (void*)_UuidCreateV1,        _NewUuidString,    0, 0);
    nErr += sqlite3_create_function(db, "UuidToString",      1, SQLITE_ANY|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,   0,       _uuidBlobToString, 0, 0);
    nErr += sqlite3_create_function(db, "UuidFromString",    1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS,  0,       _uuidStringToBlob, 0, 0);
#endif

    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

#ifdef __cplusplus
}
#endif
