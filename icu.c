/*
** 2007 May 6
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** $Id: icu.c,v 1.7 2007/12/13 21:54:11 drh Exp $
**
** This file implements an integration between the ICU library
** ("International Components for Unicode", an open-source library
** for handling unicode data) and SQLite. The integration uses
** ICU to provide the following to SQLite:
**
**   * An implementation of the SQL regexp() function (and hence REGEXP
**     operator) using the ICU uregex_XX() APIs.
**
**   * Implementations of the SQL scalar upper() and lower() functions
**     for case mapping.
**
**   * Integration of ICU and SQLite collation sequences.
**
**   * An implementation of the LIKE operator that uses ICU to
**     provide case-independent matching.
*/

#if !defined(SQLITE_CORE)                  \
 || defined(SQLITE_ENABLE_ICU)             \
 || defined(SQLITE_ENABLE_ICU_COLLATIONS)

/* Include ICU headers */
#if !defined(SQLITE_OS_WIN)
# if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#  define SQLITE_OS_WIN 1
# else
#  define SQLITE_OS_WIN 0
# endif
#endif
#ifndef NTDDI_WIN10_RS3
# define NTDDI_WIN10_RS3 0x0A000004
#endif
#ifndef NTDDI_VERSION
# define NTDDI_VERSION 0x0000000
#endif
#if SQLITE_OS_WIN && (NTDDI_VERSION >= NTDDI_WIN10_RS3)
# include <icu.h>
#else
#include <unicode/utypes.h>
#include <unicode/uregex.h>
#include <unicode/ustring.h>
#include <unicode/ucol.h>
#endif

#ifndef SQLITE_CORE
  #include "sqlite3ext.h"
  SQLITE_EXTENSION_INIT1
#else
  #include "sqlite3.h"
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
** This function is called when an ICU function called from within
** the implementation of an SQL scalar function returns an error.
**
** The scalar function context passed as the first argument is
** loaded with an error message based on the following two args.
*/
static void icuFunctionError(
  sqlite3_context *pCtx,       /* SQLite scalar function context */
  const char *zName,           /* Name of ICU function that failed */
  UErrorCode e                 /* Error code returned by ICU function */
){
  char zBuf[128];
  sqlite3_snprintf(128, zBuf, "ICU error: %s(): %s", zName, u_errorName(e));
  zBuf[127] = '\0';
  sqlite3_result_error(pCtx, zBuf, -1);
}

#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_ICU)

/*
** Maximum length (in bytes) of the pattern in a LIKE or GLOB
** operator.
*/
#ifndef SQLITE_MAX_LIKE_PATTERN_LENGTH
# define SQLITE_MAX_LIKE_PATTERN_LENGTH 50000
#endif

/*
** Version of sqlite3_free() that is always a function, never a macro.
*/
static void xFree(void *p){
  sqlite3_free(p);
}

/*
** This lookup table is used to help decode the first byte of
** a multi-byte UTF8 character. It is copied here from SQLite source
** code file utf8.c.
*/
static const unsigned char icuUtf8Trans1[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x00, 0x00,
};

#define SQLITE_ICU_READ_UTF8(zIn, c)                       \
  c = *(zIn++);                                            \
  if( c>=0xc0 ){                                           \
    c = icuUtf8Trans1[c-0xc0];                             \
    while( (*zIn & 0xc0)==0x80 ){                          \
      c = (c<<6) + (0x3f & *(zIn++));                      \
    }                                                      \
  }

#define SQLITE_ICU_SKIP_UTF8(zIn)                          \
  assert( *zIn );                                          \
  if( *(zIn++)>=0xc0 ){                                    \
    while( (*zIn & 0xc0)==0x80 ){zIn++;}                   \
  }


/*
** Compare two UTF-8 strings for equality where the first string is
** a "LIKE" expression. Return true (1) if they are the same and
** false (0) if they are different.
*/
static int icuLikeCompare(
  const uint8_t *zPattern,   /* LIKE pattern */
  const uint8_t *zString,    /* The UTF-8 string to compare against */
  const UChar32 uEsc         /* The escape character */
){
  static const uint32_t MATCH_ONE = (uint32_t)'_';
  static const uint32_t MATCH_ALL = (uint32_t)'%';

  int prevEscape = 0;     /* True if the previous character was uEsc */

  while( 1 ){

    /* Read (and consume) the next character from the input pattern. */
    uint32_t uPattern;
    SQLITE_ICU_READ_UTF8(zPattern, uPattern);
    if( uPattern==0 ) break;

    /* There are now 4 possibilities:
    **
    **     1. uPattern is an unescaped match-all character "%",
    **     2. uPattern is an unescaped match-one character "_",
    **     3. uPattern is an unescaped escape character, or
    **     4. uPattern is to be handled as an ordinary character
    */
    if( uPattern==MATCH_ALL && !prevEscape && uPattern!=(uint32_t)uEsc ){
      /* Case 1. */
      uint8_t c;

      /* Skip any MATCH_ALL or MATCH_ONE characters that follow a
      ** MATCH_ALL. For each MATCH_ONE, skip one character in the
      ** test string.
      */
      while( (c=*zPattern) == MATCH_ALL || c == MATCH_ONE ){
        if( c==MATCH_ONE ){
          if( *zString==0 ) return 0;
          SQLITE_ICU_SKIP_UTF8(zString);
        }
        zPattern++;
      }

      if( *zPattern==0 ) return 1;

      while( *zString ){
        if( icuLikeCompare(zPattern, zString, uEsc) ){
          return 1;
        }
        SQLITE_ICU_SKIP_UTF8(zString);
      }
      return 0;

    }else if( uPattern==MATCH_ONE && !prevEscape && uPattern!=(uint32_t)uEsc ){
      /* Case 2. */
      if( *zString==0 ) return 0;
      SQLITE_ICU_SKIP_UTF8(zString);

    }else if( uPattern==(uint32_t)uEsc && !prevEscape ){
      /* Case 3. */
      prevEscape = 1;

    }else{
      /* Case 4. */
      uint32_t uString;
      SQLITE_ICU_READ_UTF8(zString, uString);
      uString = (uint32_t)u_foldCase((UChar32)uString, U_FOLD_CASE_DEFAULT);
      uPattern = (uint32_t)u_foldCase((UChar32)uPattern, U_FOLD_CASE_DEFAULT);
      if( uString!=uPattern ){
        return 0;
      }
      prevEscape = 0;
    }
  }

  return *zString==0;
}

/*
** Implementation of the like() SQL function.  This function implements
** the build-in LIKE operator.  The first argument to the function is the
** pattern and the second argument is the string.  So, the SQL statements:
**
**       A LIKE B
**
** is implemented as like(B, A). If there is an escape character E,
**
**       A LIKE B ESCAPE E
**
** is mapped to like(B, A, E).
*/
static void icuLikeFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  const unsigned char *zA = sqlite3_value_text(argv[0]);
  const unsigned char *zB = sqlite3_value_text(argv[1]);
  UChar32 uEsc = 0;

  /* Limit the length of the LIKE or GLOB pattern to avoid problems
  ** of deep recursion and N*N behavior in patternCompare().
  */
  if( sqlite3_value_bytes(argv[0])>SQLITE_MAX_LIKE_PATTERN_LENGTH ){
    sqlite3_result_error(context, "LIKE or GLOB pattern too complex", -1);
    return;
  }


  if( argc==3 ){
    /* The escape character string must consist of a single UTF-8 character.
    ** Otherwise, return an error.
    */
    int nE= sqlite3_value_bytes(argv[2]);
    const unsigned char *zE = sqlite3_value_text(argv[2]);
    int i = 0;
    if( zE==0 ) return;
    U8_NEXT(zE, i, nE, uEsc);
    if( i!=nE){
      sqlite3_result_error(context,
          "ESCAPE expression must be a single character", -1);
      return;
    }
  }

  if( zA && zB ){
    sqlite3_result_int(context, icuLikeCompare(zA, zB, uEsc));
  }
}

/*
** Function to delete compiled regexp objects. Registered as
** a destructor function with sqlite3_set_auxdata().
*/
static void icuRegexpDelete(void *p){
  URegularExpression *pExpr = (URegularExpression *)p;
  uregex_close(pExpr);
}

/*
** Implementation of SQLite REGEXP operator. This scalar function takes
** two arguments. The first is a regular expression pattern to compile
** the second is a string to match against that pattern. If either
** argument is an SQL NULL, then NULL Is returned. Otherwise, the result
** is 1 if the string matches the pattern, or 0 otherwise.
**
** SQLite maps the regexp() function to the regexp() operator such
** that the following two are equivalent:
**
**     zString REGEXP zPattern
**     regexp(zPattern, zString)
**
** Uses the following ICU regexp APIs:
**
**     uregex_open()
**     uregex_matches()
**     uregex_close()
*/
static void icuRegexpFunc(sqlite3_context *p, int nArg, sqlite3_value **apArg){
  UErrorCode status = U_ZERO_ERROR;
  URegularExpression *pExpr;
  UBool res;
  const UChar *zString = sqlite3_value_text16(apArg[1]);

  (void)nArg;  /* Unused parameter */

  /* If the left hand side of the regexp operator is NULL,
  ** then the result is also NULL.
  */
  if( !zString ){
    return;
  }

  pExpr = sqlite3_get_auxdata(p, 0);
  if( !pExpr ){
    const UChar *zPattern = sqlite3_value_text16(apArg[0]);
    if( !zPattern ){
      return;
    }
    pExpr = uregex_open(zPattern, -1, 0, 0, &status);
    if( U_FAILURE(status) ){
      icuFunctionError(p, "uregex_open", status);
      return;
    }
    sqlite3_set_auxdata(p, 0, pExpr, icuRegexpDelete);
    pExpr = sqlite3_get_auxdata(p, 0);
    if( !pExpr ){
        sqlite3_result_error_nomem(p);
        return;
    }
  }

  /* Configure the text that the regular expression operates on. */
  uregex_setText(pExpr, zString, -1, &status);
  if( !U_SUCCESS(status) ){
    icuFunctionError(p, "uregex_setText", status);
    return;
  }

  /* Attempt the match */
  res = uregex_matches(pExpr, 0, &status);
  if( !U_SUCCESS(status) ){
    icuFunctionError(p, "uregex_matches", status);
    return;
  }

  /* Set the text that the regular expression operates on to a NULL
  ** pointer. This is not really necessary, but it is tidier than
  ** leaving the regular expression object configured with an invalid
  ** pointer after this function returns.
  */
  uregex_setText(pExpr, 0, 0, &status);

  /* Return 1 or 0. */
  sqlite3_result_int(p, res ? 1 : 0);
}

/*
** Implementations of scalar functions for case mapping - upper() and
** lower(). Function upper() converts its input to upper-case (ABC).
** Function lower() converts to lower-case (abc).
**
** ICU provides two types of case mapping, "general" case mapping and
** "language specific". Refer to ICU documentation for the differences
** between the two.
**
** To utilise "general" case mapping, the upper() or lower() scalar
** functions are invoked with one argument:
**
**     upper('ABC') -> 'abc'
**     lower('abc') -> 'ABC'
**
** To access ICU "language specific" case mapping, upper() or lower()
** should be invoked with two arguments. The second argument is the name
** of the locale to use. Passing an empty string ("") or SQL NULL value
** as the second argument is the same as invoking the 1 argument version
** of upper() or lower().
**
**     lower('I', 'en_us') -> 'i'
**     lower('I', 'tr_tr') -> '\u131' (small dotless i)
**
** http://www.icu-project.org/userguide/posix.html#case_mappings
*/
static void icuCaseFunc16(sqlite3_context *p, int nArg, sqlite3_value **apArg){
  const UChar *zInput;            /* Pointer to input string */
  UChar *zOutput = 0;             /* Pointer to output buffer */
  int nInput;                     /* Size of utf-16 input string in bytes */
  int nOut;                       /* Size of output buffer in bytes */
  int cnt;
  int bToUpper;                   /* True for toupper(), false for tolower() */
  UErrorCode status;
  const char *zLocale = 0;

  assert(nArg==1 || nArg==2);
  bToUpper = (intptr_t)sqlite3_user_data(p);
  if( nArg==2 ){
    zLocale = (const char *)sqlite3_value_text(apArg[1]);
  }

  zInput = sqlite3_value_text16(apArg[0]);
  if( !zInput ){
    return;
  }
  nOut = nInput = sqlite3_value_bytes16(apArg[0]);
  if( nOut==0 ){
    sqlite3_result_text16(p, "", 0, SQLITE_STATIC);
    return;
  }

  for(cnt=0; cnt<2; cnt++){
    UChar *zNew = sqlite3_realloc(zOutput, nOut);
    if( zNew==0 ){
      sqlite3_free(zOutput);
      sqlite3_result_error_nomem(p);
      return;
    }
    zOutput = zNew;
    status = U_ZERO_ERROR;
    switch (bToUpper) {
        case 0: {
            nOut = 2*u_strToLower(zOutput,nOut/2,zInput,nInput/2,zLocale,&status);
            break;
        }
        case 1: {
            nOut = 2*u_strToUpper(zOutput,nOut/2,zInput,nInput/2,zLocale,&status);
            break;
        }
        case 2: {
            nOut = 2*u_strToTitle(zOutput,nOut/2,zInput,nInput/2,0,zLocale,&status);
            break;
        }
    }

    if( U_SUCCESS(status) ){
      sqlite3_result_text16(p, zOutput, nOut, xFree);
    }else if( status==U_BUFFER_OVERFLOW_ERROR ){
      assert( cnt==0 );
      continue;
    }else{
      char *desc[] = {"u_strToUpper", "u_strToLower", "u_strToTile"};
      icuFunctionError(p, desc[bToUpper], status);
    }
    return;
  }
  assert( 0 );     /* Unreachable */
}


static void icuTransliterateDelete(void* p)
{
    utrans_close((UTransliterator*)p);
}


static void icuTransliterate(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    UParseError pe;
    UTransliterator *t = 0;
    UErrorCode status = U_ZERO_ERROR;
    UChar *xform_id;
    UChar *buffer;
    int n, limit, bs;
    int slot = -1229149472; /* (unicode('I')*16777216 + unicode('C')*65536 + unicode('U')*256 + unicode(' ')) */
    const char *data;

    if (argc == 2) slot = 1;
    t = sqlite3_get_auxdata(ctx, slot);
    if (!t) {
        if (argc == 1) {
            xform_id = L"Latin;NFKD;[:Nonspacing Mark:]Remove;Lower;NFKC;ascii";
        } else {
            xform_id = (UChar*)sqlite3_value_text16(argv[1]);
        }
        t = utrans_openU(xform_id, -1, UTRANS_FORWARD, NULL, -1, &pe, &status);
        if (U_FAILURE(status)) {
            icuFunctionError(ctx, "utrans_openU", status);
            return;
        }
        sqlite3_set_auxdata(ctx, slot, t, icuTransliterateDelete);
        t = sqlite3_get_auxdata(ctx, slot);
        if (!t) {
           sqlite3_result_error_nomem(ctx);
           return;
       }
    }
    data = sqlite3_value_text16(argv[0]);
    n = sqlite3_value_bytes16(argv[0]);
    limit = n/2;
    bs = sizeof(UChar) * n;
    buffer = sqlite3_malloc(bs);
    memcpy(buffer, data, n);
    n /= 2;
    utrans_transUChars(t, buffer, &n, n*2, 0, &limit, &status);
    if (U_FAILURE(status)) {
        sqlite3_free(buffer);
        icuFunctionError(ctx, "utrans_transUChars", status);
        return;
    }
    sqlite3_result_text16(ctx, buffer, limit*2, SQLITE_TRANSIENT);
    sqlite3_free(buffer);
}


#endif /* !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_ICU) */

/*
** Collation sequence destructor function. The pCtx argument points to
** a UCollator structure previously allocated using ucol_open().
*/
static void icuCollationDel(void *pCtx){
  UCollator *p = (UCollator *)pCtx;
  ucol_close(p);
}

/*
** Collation sequence comparison function. The pCtx argument points to
** a UCollator structure previously allocated using ucol_open().
*/
static int icuCollationColl(
  void *pCtx,
  int nLeft,
  const void *zLeft,
  int nRight,
  const void *zRight
){
  UCollationResult res;
  UCollator *p = (UCollator *)pCtx;
  res = ucol_strcoll(p, (UChar *)zLeft, nLeft/2, (UChar *)zRight, nRight/2);
  switch( res ){
    case UCOL_LESS:    return -1;
    case UCOL_GREATER: return +1;
    case UCOL_EQUAL:   return 0;
  }
  assert(!"Unexpected return value from ucol_strcoll()");
  return 0;
}

static int icuCreateCollation(sqlite3_context* p, sqlite3* db, const char* zLocale, const char* zName)
{
  UErrorCode status = U_ZERO_ERROR;
  UCollator *pUCollator;
  int rc = SQLITE_OK;

  if( !zLocale || !zName ){
    return SQLITE_ERROR;
  }
  pUCollator = ucol_open(zLocale, &status);
  if( !U_SUCCESS(status) ){
    if (p)
        icuFunctionError(p, "ucol_open", status);
    return SQLITE_ERROR;
  }
  rc = sqlite3_create_collation_v2(db, zName, SQLITE_UTF16, (void *)pUCollator, icuCollationColl, icuCollationDel);
  if( rc!=SQLITE_OK ){
    ucol_close(pUCollator);
  } else {
    if (p)
        sqlite3_result_text(p, ucol_getLocaleByType(pUCollator, 1, &status), -1, SQLITE_TRANSIENT);
  }
  return rc;
}

static void icuAutoCollation(void* udata, sqlite3 *db, int eTextRep, const char* collName)
{
    char* locale;
    char* flags;
    int valid = 0;
    int AI = 0;
    int CI = 0;
    int cnt;
    int length;


    if (strnicmp(collName, "icu", 3))
        return;

    length = strlen(collName);

    if (length>3 && collName[3] != '_')
        return;

//  printf("Processing Request for: %s\n", collName);

    flags = (char*)collName;
    if (length>3) {
        cnt = 0;
        flags += length;
        while (*flags != '_') flags--, cnt++;
        flags++, cnt--;
        if (cnt >= 2) {
            if (!strnicmp(flags, "AI", 2))
                AI = 1;
            if (!strnicmp(flags, "CI", 2))
                CI = 1;
        }
        if (cnt >= 4) {
            if (!strnicmp(flags+2, "AI", 2))
                AI = 1;
            if (!strnicmp(flags+2, "CI", 2))
                CI = 1;
        }
        if (!(AI || CI))
            flags = (char*)collName + length;
        else
            flags--;
    }

//  printf("AI=%d; CI=%d\n", AI, CI);

    cnt = flags - (char*)collName - 4;
    if (cnt < 2) {
        cnt = 3;
        locale = sqlite3_malloc(cnt+32);
        locale[0] = 0;
        strcat(locale, "und");
        valid = 1;
    } else {
        locale = sqlite3_malloc(cnt+32);
        locale[0] = 0;
        strncat(locale, collName + 4, cnt);
        for (int i=0; i<ucol_countAvailable() && valid==0; i++)
            valid = (stricmp(locale, ucol_getAvailable(i)) == 0);
        if (!valid) valid = (stricmp(locale, "root") == 0);
        if (!valid) valid = (stricmp(locale, "und") == 0);
        for (char *p=locale; *p; p++) if (*p=='_') *p = '-';
    }

    if (AI && CI)
        strcat(locale, "-u-ks-level1-kc-false");
    if (AI && !CI)
        strcat(locale, "-u-ks-level1-kc-true");
    if (!AI && CI)
        strcat(locale, "-u-ks-level2-kc-false");


//  printf("Requested=%s; locale=%s; valid=%d\n", collName, locale, valid);

    if (valid)
        icuCreateCollation(NULL, db, locale, collName);

    sqlite3_free(locale);
}


/*
** Implementation of the scalar function icu_load_collation().
**
** This scalar function is used to add ICU collation based collation
** types to an SQLite database connection. It is intended to be called
** as follows:
**
**     SELECT icu_load_collation(<locale>, <collation-name>);
**
** Where <locale> is a string containing an ICU locale identifier (i.e.
** "en_AU", "tr_TR" etc.) and <collation-name> is the name of the
** collation sequence to create.
*/
static void icuLoadCollation(
  sqlite3_context *p,
  int nArg,
  sqlite3_value **apArg
){
  sqlite3 *db = sqlite3_context_db_handle(p);
  const char *zLocale;      /* Locale identifier - (eg. "jp_JP") */
  const char *zName;        /* SQL Collation sequence name (eg. "japanese") */
  int rc;                   /* Return code from sqlite3_create_collation_x() */

  assert(nArg==2);
  (void)nArg; /* Unused parameter */
  zLocale = (const char *)sqlite3_value_text(apArg[0]);
  zName = (const char *)sqlite3_value_text(apArg[1]);

  rc = icuCreateCollation(p, db, zLocale, zName);

  if( rc!=SQLITE_OK ){
    sqlite3_result_error(p, "Error registering collation function", -1);
  }
}


static void icuDefaultLocale(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    UErrorCode status = U_ZERO_ERROR;
    if (argc == 1) {
        if (sqlite3_value_type(argv[0]) == SQLITE_TEXT) {
            uloc_setDefault(sqlite3_value_text(argv[0]), &status);
            if (U_FAILURE(status)) {
                icuFunctionError(ctx, "uloc_setDefault", status);
                return;
            }
        }
    }
    sqlite3_result_text(ctx, uloc_getDefault(), -1, SQLITE_TRANSIENT);
}


static void icuEnumLocale(sqlite3_context *ctx, int argc, sqlite3_value **argv)
{
    if (argc) {
        if (sqlite3_value_type(argv[0])==SQLITE_INTEGER) {
            int value = sqlite3_value_int(argv[0]);
            if (value > -1 && value < ucol_countAvailable())
                sqlite3_result_text(ctx, ucol_getAvailable(value), -1, SQLITE_TRANSIENT);
        }
    } else {
        sqlite3_result_int(ctx, ucol_countAvailable());
    }
}


/*
** Register the ICU extension functions with database db.
*/
int sqlite3IcuInit(sqlite3 *db){
# define SQLITEICU_EXTRAFLAGS (SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS)
  static const struct IcuScalar {
    const char *zName;                        /* Function name */
    unsigned char nArg;                       /* Number of arguments */
    unsigned int enc;                         /* Optimal text encoding */
    intptr_t iContext;                        /* sqlite3_user_data() context */
    void (*xFunc)(sqlite3_context*,int,sqlite3_value**);
  } scalars[] = {
    {"icu_load_collation",2,SQLITE_UTF8|SQLITE_DIRECTONLY, 0, icuLoadCollation},
    {"icu_locale",        0,SQLITE_UTF8|SQLITE_INNOCUOUS,  0, icuDefaultLocale},
    {"icu_locale",        1,SQLITE_UTF8|SQLITE_DIRECTONLY, 0, icuDefaultLocale},
    {"icu_enumLocale",    0,SQLITE_UTF8|SQLITE_INNOCUOUS,  0, icuEnumLocale},
    {"icu_enumLocale",    1,SQLITE_UTF8|SQLITE_INNOCUOUS,  0, icuEnumLocale},
#if !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_ICU)
    {"regexp", 2, SQLITE_ANY|SQLITEICU_EXTRAFLAGS,         0, icuRegexpFunc},
    {"lower",  1, SQLITE_UTF16|SQLITEICU_EXTRAFLAGS,       0, icuCaseFunc16},
    {"lower",  2, SQLITE_UTF16|SQLITEICU_EXTRAFLAGS,       0, icuCaseFunc16},
    {"upper",  1, SQLITE_UTF16|SQLITEICU_EXTRAFLAGS,       1, icuCaseFunc16},
    {"upper",  2, SQLITE_UTF16|SQLITEICU_EXTRAFLAGS,       1, icuCaseFunc16},
    {"title",  1, SQLITE_UTF16|SQLITEICU_EXTRAFLAGS,       2, icuCaseFunc16},
    {"title",  2, SQLITE_UTF16|SQLITEICU_EXTRAFLAGS,       2, icuCaseFunc16},
    {"lower",  1, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        0, icuCaseFunc16},
    {"lower",  2, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        0, icuCaseFunc16},
    {"upper",  1, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        1, icuCaseFunc16},
    {"upper",  2, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        1, icuCaseFunc16},
    {"title",  1, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        2, icuCaseFunc16},
    {"title",  2, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        2, icuCaseFunc16},
    {"like",   2, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        0, icuLikeFunc},
    {"like",   3, SQLITE_UTF8|SQLITEICU_EXTRAFLAGS,        0, icuLikeFunc},
    {"fold",   1, SQLITE_ANY|SQLITEICU_EXTRAFLAGS,         0, icuTransliterate},
    {"icu_trans",2,SQLITE_ANY|SQLITEICU_EXTRAFLAGS,        0, icuTransliterate},
#endif /* !defined(SQLITE_CORE) || defined(SQLITE_ENABLE_ICU) */
  };
  int rc = SQLITE_OK;
  int i;

  for(i=0; rc==SQLITE_OK && i<(int)(sizeof(scalars)/sizeof(scalars[0])); i++){
    const struct IcuScalar *p = &scalars[i];
    rc = sqlite3_create_function(
        db, p->zName, p->nArg, p->enc,
        (void*)p->iContext,
        p->xFunc, 0, 0
    );
  }
    sqlite3_collation_needed(db, NULL, icuAutoCollation);

  return rc;
}

#if !SQLITE_CORE
#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_icu_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi)
  return sqlite3IcuInit(db);
}
#endif

#if !SQLITE_CORE
#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_icuwin_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi)
  return sqlite3IcuInit(db);
}
#endif

#endif
