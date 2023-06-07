/*
** 2003 October 31
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains the C functions that implement date and time
** functions for SQLite.
**
** There is only one exported symbol in this file - the function
** sqlite3RegisterDateTimeFunctions() found at the bottom of the file.
** All other code has file scope.
**
** SQLite processes all times and dates as julian day numbers.  The
** dates and times are stored as the number of days since noon
** in Greenwich on November 24, 4714 B.C. according to the Gregorian
** calendar system.
**
** 1970-01-01 00:00:00 is JD 2440587.5
** 2000-01-01 00:00:00 is JD 2451544.5
**
** This implementation requires years to be expressed as a 4-digit number
** which means that only dates between 0000-01-01 and 9999-12-31 can
** be represented, even though julian day numbers allow a much wider
** range of dates.
**
** The Gregorian calendar system is used for all dates and times,
** even those that predate the Gregorian calendar.  Historians usually
** use the julian calendar for dates prior to 1582-10-15 and for some
** dates afterwards, depending on locale.  Beware of this difference.
**
** The conversion algorithms are implemented based on descriptions
** in the following text:
**
**      Jean Meeus
**      Astronomical Algorithms, 2nd Edition, 1998
**      ISBN 0-943396-61-1
**      Willmann-Bell, Inc
**      Richmond, Virginia (USA)
*/
/*
** 2022 January 15
**
** Modified by Keith Medcalf
**
** Modified to support localized datetime such that iJD is always relative to the Prime Meridian
** and the localization offset (timezone) from the Prime Meridian is used to generate the display
** datetime.  Output functions may include milliseconds or the localization offset (timezone) or
** both, and a default format specified at compile-time.
**
** The author disclaims copyright to this source code
*/
#include "sqliteInt.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#if !defined(SQLITE_DATETIME_DEFAULT)
#define SQLITE_DATETIME_DEFAULT 3
#endif

#ifndef SQLITE_OMIT_DATETIME_FUNCS

/*
** The MSVC CRT on Windows CE may not have a localtime() function.
** So declare a substitute.  The substitute function itself is
** defined in "os_win.c".
*/
#if !defined(SQLITE_OMIT_LOCALTIME) && defined(_WIN32_WCE) && \
    (!defined(SQLITE_MSVC_LOCALTIME_API) || !SQLITE_MSVC_LOCALTIME_API)
struct tm *__cdecl localtime(const time_t *);
#endif

/*
** A structure for holding a single date and time.
*/
typedef struct DateTime DateTime;
struct DateTime {
  sqlite3_int64 iJD;  /* The julian day number times 86400000 */
  double s;           /* Seconds */
  int h, m;           /* Hour and minutes */
  int Y, M, D;        /* Year, month, and day */
  int tz;             /* Timezone offset in minutes */
  int zone;           /* IANA ZoneID */
  char rawS;          /* Raw numeric value stored in s */
  char validJD;       /* True (1) if iJD is valid */
  char validYMD;      /* True (1) if Y,M,D are valid */
  char validHMS;      /* True (1) if h,m,s are valid */
  char validTZ;       /* True (1) if tz is valid */
  char isError;       /* An overflow has occurred */
  char useSubsec;     /* Display subsecond precision */
};

/*
** Define our Epoch dates iJD offsets
*/
static const long long iJDepoch[8] = {               0ll, /* ijd */
                                                     0ll, /* julian epoch   -4713-11-24 12:00:00.000 */
                                       148731163200000ll, /* common era      0001-01-01 00:00:00.000 */
                                       199222286400000ll, /* ansi epoch      1601-01-01 00:00:00.000 */
                                       207360000000000ll, /* reduced julian  1858-11-16 12:00:00.000 */
                                       207360043200000ll, /* modified julian 1858-11-17 00:00:00.000 */
                                       210866760000000ll, /* unix epoch      1970-01-01 00:00:00.000 */
                                       211845067200000ll  /* Third Millenium 2001-01-01 00:00:00.000 */
                                     };

static inline double timestamp2iJD(double ts, int epoch, int secstamp)
{
    return (ts * (secstamp ? 1000.0 : 86400000.0)) + iJDepoch[epoch];
}

/*
** Convert zDate into one or more integers according to the conversion
** specifier zFormat.
**
** zFormat[] contains 4 characters for each integer converted, except for
** the last integer which is specified by three characters.  The meaning
** of a four-character format specifiers ABCD is:
**
**    A:   number of digits to convert.  Always "2" or "4".
**    B:   minimum value.  Always "0" or "1".
**    C:   maximum value, decoded as:
**           a:  12
**           b:  14
**           c:  24
**           d:  31
**           e:  59
**           f:  9999
**    D:   the separator character, or \000 to indicate this is the
**         last number to convert.
**
** Example:  To translate an ISO-8601 date YYYY-MM-DD, the format would
** be "40f-21a-20c".  The "40f-" indicates the 4-digit year followed by "-".
** The "21a-" indicates the 2-digit month followed by "-".  The "20c" indicates
** the 2-digit day which is the last integer in the set.
**
** The function returns the number of successful conversions.
*/
static int getDigits(const char *zDate, const char *zFormat, ...){
  /* The aMx[] array translates the 3rd character of each format
  ** spec into a max size:    a   b   c   d   e     f */
  static const u16 aMx[] = { 12, 14, 24, 31, 59, 9999 };
  va_list ap;
  int cnt = 0;
  char nextC;
  va_start(ap, zFormat);
  do{
    char N = zFormat[0] - '0';
    char min = zFormat[1] - '0';
    int val = 0;
    u16 max;

    assert( zFormat[2]>='a' && zFormat[2]<='f' );
    max = aMx[zFormat[2] - 'a'];
    nextC = zFormat[3];
    val = 0;
    while( N-- ){
      if( !sqlite3Isdigit(*zDate) ){
        goto end_getDigits;
      }
      val = val*10 + *zDate - '0';
      zDate++;
    }
    if( val<(int)min || val>(int)max || (nextC!=0 && nextC!=*zDate) ){
      goto end_getDigits;
    }
    *va_arg(ap,int*) = val;
    zDate++;
    cnt++;
    zFormat += 4;
  }while( nextC );
end_getDigits:
  va_end(ap);
  return cnt;
}

/*
** Parse a timezone extension on the end of a date-time.
** The extension is of the form:
**
**        (+/-)HH:MM
**
** Or the "zulu" notation:
**
**        Z
**
** If the parse is successful, write the number of minutes
** of change in p->tz and return 0.  If a parser error occurs,
** return non-zero.
**
** A missing specifier is not considered an error.
*/
static int parseTimezone(const char *zDate, DateTime *p){
  int sgn = 0;
  int nHr, nMn;
  int c;
  while( sqlite3Isspace(*zDate) ){ zDate++; }
  p->tz = 0;
  p->validTZ = 0;
  c = *zDate;
  if( c=='-' ){
    sgn = -1;
  }else if( c=='+' ){
    sgn = +1;
  }else if( c=='Z' || c=='z' ){
    zDate++;
    goto zulu_time;
  }else{
    return c!=0;
  }
  zDate++;
  if( getDigits(zDate, "20b:20e", &nHr, &nMn)!=2 ){
    return 1;
  }
  zDate += 5;
  p->tz = sgn*(nMn + nHr*60);
zulu_time:
  while( sqlite3Isspace(*zDate) ){ zDate++; }
  p->validTZ = 1;
  p->validJD = 0;
  return *zDate!=0;
}

/*
** Parse times of the form HH:MM or HH:MM:SS or HH:MM:SS.FFFF.
** The HH, MM, and SS must each be exactly 2 digits.  The
** fractional seconds FFFF can be one or more digits.
**
** May be followed by a timezone, which will be parsed
**
** Return 1 if there is a parsing error and 0 on success.
*/
static int parseHhMmSs(const char *zDate, DateTime *p){
  int h, m, s;
  double ms = 0.0;
  if( getDigits(zDate, "20c:20e", &h, &m)!=2 ){
    return 1;
  }
  zDate += 5;
  if( *zDate==':' ){
    zDate++;
    if( getDigits(zDate, "20e", &s)!=1 ){
      return 1;
    }
    zDate += 2;
    if( *zDate=='.' && sqlite3Isdigit(zDate[1]) ){
      double rScale = 1.0;
      zDate++;
      while( sqlite3Isdigit(*zDate) ){
        ms = ms*10.0 + *zDate - '0';
        rScale *= 10.0;
        zDate++;
      }
      ms /= rScale;
    }
  }else{
    s = 0;
  }
  p->validJD = 0;
  p->rawS = 0;
  p->validHMS = 1;
  p->h = h;
  p->m = m;
  p->s = s + ms;
  if( parseTimezone(zDate, p) ) return 1;
  return 0;
}

/*
** Put the DateTime object into its error state.
*/
static void datetimeError(DateTime *p){
  memset(p, 0, sizeof(*p));
  p->isError = 1;
}

/*
** Convert from YYYY-MM-DD HH:MM:SS to julian day.  We always assume
** that the YYYY-MM-DD is according to the Gregorian calendar.
**
** Reference:  Meeus page 61
*/
static void computeJD(DateTime *p){
  int Y, M, D, A, B, X1, X2;

  if( p->validJD ) return;
  if( p->validYMD ){
    Y = p->Y;
    M = p->M;
    D = p->D;
  }else{
    Y = 2000;  /* If no YMD specified, assume 2000-Jan-01 */
    M = 1;
    D = 1;
  }
  if( Y<-4713 || Y>9999 || p->rawS ){
    datetimeError(p);
    return;
  }
  if( M<=2 ){
    Y--;
    M += 12;
  }
  A = Y/100;
  B = 2 - A + (A/4);
  X1 = 36525*(Y+4716)/100;
  X2 = 306001*(M+1)/10000;
  p->iJD = (sqlite3_int64)((X1 + X2 + D + B - 1524.5 ) * 86400000);
  if( p->validHMS )
    p->iJD += p->h*3600000 + p->m*60000 + (sqlite3_int64)(p->s*1000 + 0.5);
  if( p->validTZ )
    p->iJD -= p->tz*60000;
  p->validJD = 1;
}


/*
** Parse dates of the form
**
**     YYYY-MM-DD HH:MM:SS.FFF
**     YYYY-MM-DD HH:MM:SS
**     YYYY-MM-DD HH:MM
**     YYYY-MM-DD
**
** Also parses time and timezone if present
**
** Write the result into the DateTime structure and return 0
** on success and 1 if the input string is not a well-formed
** date.
*/
static int parseYyyyMmDd(const char *zDate, DateTime *p){
  int Y, M, D, neg;

  if( zDate[0]=='-' ){
    zDate++;
    neg = 1;
  }else{
    neg = 0;
  }
  if( getDigits(zDate, "40f-21a-21d", &Y, &M, &D)!=3 ){
    return 1;
  }
  zDate += 10;
  while( sqlite3Isspace(*zDate) || 'T'==*(u8*)zDate ){ zDate++; }
  if( parseHhMmSs(zDate, p)==0 ){
    /* We got the time */
  }else if( *zDate==0 ){
    p->validHMS = 0;
  }else{
    p->validHMS = 0;
    if( parseTimezone(zDate, p) ) return 1;
  }
  p->validJD = 0;
  p->Y = neg ? -Y : Y;
  p->M = M;
  p->D = D;
  p->validYMD = 1;
  return 0;
}

/*
** Set the time to the current time reported for the prepared statement
** that is currently executing.  The same time is reported for all
** invocations of this routine from within the same call to sqlite3_step().
**
** Or if bTxn is true, use the transaction time.
**
** Return the number of errors.
*/
static int setCurrentStmtTime(sqlite3_context *context, DateTime *p, int bTxn){
  p->iJD = sqlite3StmtCurrentTime(context, bTxn);
  if( p->iJD>0 ){
    p->validJD = 1;
    p->validYMD = 0;
    p->validHMS = 0;
    p->validTZ = 1;
    p->tz = 0;
    p->zone = 0;
    return 0;
  }else{
    return 1;
  }
}

/*
** Input "r" is a numeric quantity which might be a julian day number,
** or the number of seconds since 1970.  If the value if r is within
** range of a julian day number, install it as such and set validJD.
** If the value is a valid unix timestamp, put it in p->s and set p->rawS.
*/
static void setRawDateNumber(DateTime *p, double r){
  p->s = r;
  p->rawS = 1;
  if( r>=0.0 && r<5373484.5 ){
    p->iJD = (sqlite3_int64)(r*86400000.0 + 0.5);
    p->validJD = 1;
  }
  p->validYMD = 0;
  p->validHMS = 0;
  p->validTZ = 1;
  p->tz = 0;
  p->zone = 0;
}

/*
** Attempt to parse the given string into a julian day number.  Return
** the number of errors.
**
** The following are acceptable forms for the input string:
**
**      YYYY-MM-DD HH:MM:SS.FFF  +/-HH:MM
**      DDDD.DD
**      now
**      local
**      IANAZone (eg: Canada/Mountain)
**
** In the first form, the +/-HH:MM is always optional.  The fractional
** seconds extension (the ".FFF") is optional.  The seconds portion
** (":SS.FFF") is option.  The year and date can be omitted as long
** as there is a time string.  The time string can be omitted as long
** as there is a year and date.
*/
#ifndef SQLITE_OMIT_LOCALTIME
static int toLocaltime(DateTime*,sqlite3_context*); /* Forward Declaration */
#endif
static int _applyZone(sqlite3_context *,const char *,DateTime *); /* Forward Declaration */
static sqlite_int64 LookupIANAOffset(sqlite3_context *context, int zone, sqlite_int64 ts);
static sqlite_int64 LookupIANAOffsetLMT(sqlite3_context *context, int zone, sqlite_int64 ts);

static int parseDateOrTime(
  sqlite3_context *context,
  const char *zDate,
  DateTime *p
){
  double r;
  if( parseYyyyMmDd(zDate,p)==0 ){
    return 0;
  }else if( parseHhMmSs(zDate, p)==0 ){
    return 0;
  }else if( !context ){
    return 1;   /* bail if no context */
  }else if( sqlite3StrICmp(zDate,"now")==0 && sqlite3NotPureFunc(context) ){
    return setCurrentStmtTime(context, p, 0);
  }else if( sqlite3StrICmp(zDate,"txn")==0 && sqlite3NotPureFunc(context) ){
    return setCurrentStmtTime(context, p, 1);
#ifndef SQLITE_OMIT_LOCALTIME
  }else if( sqlite3StrICmp(zDate, "local")==0 && sqlite3NotPureFunc(context) ){
    if(setCurrentStmtTime(context, p, 0)==0) return toLocaltime(p, context);
#endif
  }else if( strchr(zDate, '/')>0 && sqlite3NotPureFunc(context) ){
    if( setCurrentStmtTime(context, p, 0)==0 )
        if (*zDate == '-') return 0;
        else return _applyZone(context, zDate, p);
  }else if( sqlite3AtoF(zDate, &r, sqlite3Strlen30(zDate), SQLITE_UTF8)>0 ){
    setRawDateNumber(p, r);
    return 0;
  }else if( (sqlite3StrICmp(zDate,"subsec")==0
             || sqlite3StrICmp(zDate,"subsecond")==0)
           && sqlite3NotPureFunc(context) ){
    p->useSubsec = 1;
    return setCurrentStmtTime(context, p, 0);
  }
  return 1;
}

/* The julian day number for 9999-12-31 23:59:59.999 is 5373484.4999999.
** Multiplying this by 86400000 gives 464269060799999 as the maximum value
** for DateTime.iJD.
**
** But some older compilers (ex: gcc 4.2.1 on older Macs) cannot deal with
** such a large integer literal, so we have to encode it.
*/
#define INT_464269060799999  ((((i64)0x1a640)<<32)|0x1072fdff)

/*
** Return TRUE if the given julian day number is within range.
**
** The input is the JulianDay times 86400000.
*/
static int validJulianDay(sqlite3_int64 iJD){
  return iJD>=0 && iJD<=INT_464269060799999;
}

/*
** Compute the Year, Month, and Day from the julian day number.
*/
static void computeYMD(DateTime *p, sqlite3_context* context){
  int Z, A, B, C, D, E, X1;
  if( p->validYMD ) return;
  if( !p->validJD ){
    p->Y = 2000;
    p->M = 1;
    p->D = 1;
  }else if( !validJulianDay(p->iJD) ){
    datetimeError(p);
    return;
  }else{
    if( p->zone ){
      p->tz = LookupIANAOffset(context, p->zone, p->iJD);                                    /**/
      p->validTZ = 1;
    }
    Z = (int)((p->iJD + p->validTZ*p->tz*60000 + 43200000)/86400000);
    A = (int)((Z - 1867216.25)/36524.25);
    A = Z + 1 + A - (A/4);
    B = A + 1524;
    C = (int)((B - 122.1)/365.25);
    D = (36525*(C&32767))/100;
    E = (int)((B-D)/30.6001);
    X1 = (int)(30.6001*E);
    p->D = B - D - X1;
    p->M = E<14 ? E-1 : E-13;
    p->Y = p->M>2 ? C - 4716 : C - 4715;
  }
  p->validYMD = 1;
}

/*
** Compute the Hour, Minute, and Seconds from the julian day number.
*/
static void computeHMS(DateTime *p){
  int s;
  if( p->validHMS ) return;
  computeJD(p);
  s = (int)((p->iJD + p->validTZ*p->tz*60000 + 43200000) % 86400000);
  p->s = s/1000.0;
  s = (int)p->s;
  p->s -= s;
  p->h = s/3600;
  s -= p->h*3600;
  p->m = s/60;
  p->rawS = 0;                                                                          /**/
  p->s += s - p->m*60;
  p->validHMS = 1;
}

/*
** Compute both YMD and HMS
*/
static void computeYMD_HMS(DateTime *p, sqlite3_context* context){
  computeYMD(p, context);
  computeHMS(p);
}

/*
** Clear the YMD and HMS
*/
static void clearYMD_HMS(DateTime *p){
  p->validYMD = 0;
  p->validHMS = 0;
}

#ifndef SQLITE_OMIT_LOCALTIME
/*
** On recent Windows platforms, the localtime_s() function is available
** as part of the "Secure CRT". It is essentially equivalent to
** localtime_r() available under most POSIX platforms, except that the
** order of the parameters is reversed.
**
** See http://msdn.microsoft.com/en-us/library/a442x3ye(VS.80).aspx.
**
** If the user has not indicated to use localtime_r() or localtime_s()
** already, check for an MSVC build environment that provides
** localtime_s().
*/
#if !HAVE_LOCALTIME_R && !HAVE_LOCALTIME_S \
    && defined(_MSC_VER) && defined(_CRT_INSECURE_DEPRECATE)
#undef  HAVE_LOCALTIME_S
#define HAVE_LOCALTIME_S 1
#endif

/*
** The following routine implements the rough equivalent of localtime_r()
** using whatever operating-system specific localtime facility that
** is available.  This routine returns 0 on success and
** non-zero on any kind of error.
**
** If the sqlite3GlobalConfig.bLocaltimeFault variable is non-zero then this
** routine will always fail.  If bLocaltimeFault is nonzero and
** sqlite3GlobalConfig.xAltLocaltime is not NULL, then xAltLocaltime() is
** invoked in place of the OS-defined localtime() function.
**
** EVIDENCE-OF: R-62172-00036 In this implementation, the standard C
** library function localtime_r() is used to assist in the calculation of
** local time.
*/
static int osLocaltime(time_t *t, struct tm *pTm){
  int rc;
#if !HAVE_LOCALTIME_R && !HAVE_LOCALTIME_S
  struct tm *pX;
#if SQLITE_THREADSAFE>0
  sqlite3_mutex *mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MAIN);
#endif
  sqlite3_mutex_enter(mutex);
  pX = localtime(t);
#ifndef SQLITE_UNTESTABLE
  if( sqlite3GlobalConfig.bLocaltimeFault ){
    if( sqlite3GlobalConfig.xAltLocaltime!=0
     && 0==sqlite3GlobalConfig.xAltLocaltime((const void*)t,(void*)pTm)
    ){
      pX = pTm;
    }else{
      pX = 0;
    }
  }
#endif
  if( pX ) *pTm = *pX;
#if SQLITE_THREADSAFE>0
  sqlite3_mutex_leave(mutex);
#endif
  rc = pX==0;
#else
#ifndef SQLITE_UNTESTABLE
  if( sqlite3GlobalConfig.bLocaltimeFault ){
    if( sqlite3GlobalConfig.xAltLocaltime!=0 ){
      return sqlite3GlobalConfig.xAltLocaltime((const void*)t,(void*)pTm);
    }else{
      return 1;
    }
  }
#endif
#if HAVE_LOCALTIME_R
  rc = localtime_r(t, pTm)==0;
#else
  rc = localtime_s(pTm, t);
#endif /* HAVE_LOCALTIME_R */
#endif /* HAVE_LOCALTIME_R || HAVE_LOCALTIME_S */
  return rc;
}
#endif /* SQLITE_OMIT_LOCALTIME */


#ifndef SQLITE_OMIT_LOCALTIME
/*
** Assuming the input DateTime is UTC, move it to its localtime equivalent.
*/
static int toLocaltime(
  DateTime *p,                   /* Date at which to calculate offset */
  sqlite3_context *pCtx          /* Write error here if one occurs */
){
  DateTime x = *p;
  time_t t;
  struct tm sLocal;
  int iYearDiff;

  /* Initialize the contents of sLocal to avoid a compiler warning. */
  memset(&sLocal, 0, sizeof(sLocal));

  computeJD(p);
  computeJD(&x);
  p->zone = 0;
  x.tz = 0;
  x.validTZ = 0;
  x.zone = 0;
  clearYMD_HMS(&x);
  computeYMD_HMS(&x, pCtx);
  if( p->iJD<2108667600*(i64)100000 /* 1970-01-01 */
   || p->iJD>2130141456*(i64)100000 /* 2038-01-18 */
  ){
    /* EVIDENCE-OF: R-55269-29598 The localtime_r() C function normally only
    ** works for years between 1970 and 2037. For dates outside this range,
    ** SQLite attempts to map the year into an equivalent year within this
    ** range, do the calculation, then map the year back.
    */
    iYearDiff = (2000 + x.Y%4) - x.Y;
    x.Y += iYearDiff;
    x.validJD = 0;
    computeJD(&x);
    t = (time_t)(x.iJD/1000 -  21086676*(i64)10000);
  }else{
    iYearDiff = 0;
    t = (time_t)(p->iJD/1000 -  21086676*(i64)10000);
  }
  if( osLocaltime(&t, &sLocal) ){
    sqlite3_result_error(pCtx, "local time unavailable", -1);
    return SQLITE_ERROR;
  }
  x.Y = sLocal.tm_year + 1900;
  x.M = sLocal.tm_mon + 1;
  x.D = sLocal.tm_mday;
  x.h = sLocal.tm_hour;
  x.m = sLocal.tm_min;
  x.s = sLocal.tm_sec;
  x.validYMD = 1;
  x.validHMS = 1;
  x.validJD = 0;
  x.rawS = 0;
  x.validTZ = 0;
  x.isError = 0;
  computeJD(&x);
  p->tz = ((time_t)(x.iJD/1000 -  21086676*(i64)10000) - t) / 60;
  p->validTZ = 1;
  clearYMD_HMS(p);
  return SQLITE_OK;
}
#endif /* SQLITE_OMIT_LOCALTIME */

/*
** The following table defines various date transformations of the form
**
**            'NNN days'
**
** Where NNN is an arbitrary floating-point number and "days" can be one
** of several units of time.
*/
static const struct {
  u8 nName;           /* Length of the name */
  char zName[7];      /* Name of the transformation */
  float rLimit;       /* Maximum NNN value for this transform */
  float rXform;       /* Constant used for this transform */
} aXformType[] = {
  { 6, "second", 4.6427e+14,       1.0  },
  { 6, "minute", 7.7379e+12,      60.0  },
  { 4, "hour",   1.2897e+11,    3600.0  },
  { 3, "day",    5373485.0,    86400.0  },
  { 5, "month",  176546.0,   2592000.0  },
  { 4, "year",   14713.0,   31536000.0  },
};

/*
** If the DateTime p is raw number, try to figure out if it is
** a julian day number of a unix timestamp.  Set the p value
** appropriately.
*/
static void autoAdjustDate(DateTime *p){
  if( !p->rawS || p->validJD ){
    p->rawS = 0;
  }else if( p->s>=-21086676*(i64)10000        /* -4713-11-24 12:00:00 */
         && p->s<=(25340230*(i64)10000)+799   /*  9999-12-31 23:59:59 */
  ){
    double r = p->s*1000.0 + 210866760000000.0;
    clearYMD_HMS(p);
    p->iJD = (sqlite3_int64)(r + 0.5);
    p->validJD = 1;
    p->validTZ = 1;
    p->tz = 0;
    p->rawS = 0;
  }
}

/*
** _applyZone is an internal helper function that applies the IANA zone name
** offset to the specified datetime object by looking up the UTC offset and
** modifying the referenced DateTime structure
*/

/* IANASLOT == -(unicode('I')*16777216 + unicode('A')*65536 + unicode('N')*256 + unicode('A')) */
#if !defined(IANASLOT)
#define IANASLOT -1229016641
#endif
static int _applyZone(sqlite3_context *context, const char *tzargptr, DateTime *x)
{
    static const int slot = IANASLOT;
    sqlite3_stmt *stmt = 0;
    int len;

    computeJD(x);
    clearYMD_HMS(x);
    stmt = sqlite3_get_auxdata(context, slot);
    if (!stmt) {
        if (sqlite3_prepare_v2(context->pVdbe->db, "select Offset, Zone_ID from TZ_Zones join TZ_ZoneData on Zone_ID == ID where Zone == ? and StartTime <= ? order by StartTime desc limit 1;", -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_result_error(context, "Localize Prepare Failed -- Missing TZ_Zones and TZ_ZoneData tables?", -1);
            return 1;
        }
        sqlite3_set_auxdata(context, slot, (void*)stmt, (void*)sqlite3_finalize);
        stmt = sqlite3_get_auxdata(context, slot);
        if (!stmt) {
            sqlite3_result_error_nomem(context);
            return 1;
        }
    }
    len = (int)strlen(tzargptr);
    if ((len>1) && (tzargptr[len-1] == '/')) len--;
    sqlite3_bind_text(stmt, 1, tzargptr, len, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, (x->iJD - 210866760000000L) / 1000L);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_result_error(context, "Localize No Data Found -- Zone Name Error?", -1);
        return 1;
    }
    x->tz = sqlite3_column_int64(stmt, 0) / 60;
    x->zone = sqlite3_column_int(stmt, 1);
    if (!x->validTZ) {
        x->iJD -= x->tz*60000;
        x->validTZ = 1;
    }
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    return 0;
}

static sqlite_int64 LookupIANAOffset(sqlite3_context *context, int zone, sqlite_int64 ts)
{
    static const int slot = IANASLOT - 1;
    sqlite3_stmt *stmt = 0;
    sqlite_int64 offset;

    stmt = sqlite3_get_auxdata(context, slot);
    if (!stmt) {
        if (sqlite3_prepare_v2(context->pVdbe->db, "select Offset from TZ_ZoneData where Zone_ID == ? and StartTime <= ? order by StartTime desc limit 1;", -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_result_error(context, "Localize Prepare Failed -- Missing TZ_ZoneData table?", -1);
            return 0;
        }
        sqlite3_set_auxdata(context, slot, (void*)stmt, (void*)sqlite3_finalize);
        stmt = sqlite3_get_auxdata(context, slot);
        if (!stmt) {
            sqlite3_result_error_nomem(context);
            return 0;
        }
    }
    sqlite3_bind_int(stmt, 1, zone);
    sqlite3_bind_int64(stmt, 2, (ts - 210866760000000L) / 1000L);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_result_error(context, "Localize No Data Found -- Zone Name Error?", -1);
        return 0;
    }
    offset = sqlite3_column_int64(stmt, 0) / 60;
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    return offset;
}

/*
** Unlocalize functions are the inverse of the localize functions
** and convert "local" naive value back to UT1.  Instant-in-Time
** Timestrings which include the TZ simply return the corresponding
** UT1 timestring (the unlocalize processing is bypassed).
**
** Note that this procedure is funky during the transition overlap
** period since it is unknown whether the timestring during the change
** period refers to the period before or after the change.
**
** All naive datetime's by refer to the time after the change
**
** For this reason it is recommended that all datetime strings
** should always be stored as instant times (with the TZ offset)
** even if they are in the +00:00 (Zulu) timezone
*/

static int _unapplyZone(sqlite3_context *context, const char *tzargptr, DateTime *x)
{
    static const int slot = IANASLOT - 2;
    sqlite3_stmt *stmt;
    int len;


    sqlite3 *db = context->pVdbe->db;

    computeJD(x);
    if (x->validTZ)
    {
        clearYMD_HMS(x);
        x->tz = 0;
        x->zone = 0;
        return 0;
    }
    stmt = sqlite3_get_auxdata(context, slot);
    if (!stmt) {
        if (sqlite3_prepare_v2(context->pVdbe->db, "select Offset from TZ_Zones join TZ_ZoneData on Zone_ID == ID where Zone == ? and StartTime + Offset <= ? order by StartTime + Offset desc limit 1;", -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_result_error(context, "Unlocalize Prepare Failed -- Missing TZ_Zones or TZ_ZoneData tables?", -1);
            return 1;
        }
        sqlite3_set_auxdata(context, slot, (void*)stmt, (void*)sqlite3_finalize);
        stmt = sqlite3_get_auxdata(context, slot);
        if (!stmt) {
            sqlite3_result_error_nomem(context);
            return 1;
        }
    }
    len = (int)strlen(tzargptr);
    if ((len>1) && (tzargptr[len-1] == '/')) len--;
    sqlite3_bind_text(stmt, 1, tzargptr, len, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, (x->iJD - 210866760000000L) / 1000L);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_result_error(context, "Unlocalize No Data Found -- Zone Name Error?", -1);
        return 1;
    }
    x->iJD -= sqlite3_column_int64(stmt, 0) * 1000;
    x->tz = 0;
    x->validTZ = 1;
    x->zone = 0;
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    clearYMD_HMS(x);
    return 0;
}

static sqlite_int64 LookupIANAOffsetLMT(sqlite3_context *context, int zone, sqlite_int64 ts)
{
    static const int slot = IANASLOT - 3;
    sqlite3_stmt *stmt = 0;
    sqlite_int64 offset;

    stmt = sqlite3_get_auxdata(context, slot);
    if (!stmt) {
        if (sqlite3_prepare_v2(context->pVdbe->db, "select Offset from TZ_ZoneData where Zone_ID == ? and StartTime + Offset <= ? order by StartTime + Offset desc limit 1;", -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_result_error(context, "Localize Prepare Failed -- Missing TZ_ZoneData table?", -1);
            return 1;
        }
        sqlite3_set_auxdata(context, slot, (void*)stmt, (void*)sqlite3_finalize);
        stmt = sqlite3_get_auxdata(context, slot);
        if (!stmt) {
            sqlite3_result_error_nomem(context);
            return 1;
        }
    }
    sqlite3_bind_int(stmt, 1, zone);
    sqlite3_bind_int64(stmt, 2, (ts - 210866760000000L) / 1000L);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_result_error(context, "Localize No Data Found -- Zone Name Error?", -1);
        return 1;
    }
    offset = sqlite3_column_int64(stmt, 0) / 60;
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    return offset;
}
#if defined(IANASLOT)
#undef IANASLOT
#endif

/*
** Process a modifier to a date-time stamp.  The modifiers are
** as follows:
**
**     NNN days
**     NNN hours
**     NNN minutes
**     NNN.NNNN seconds
**     NNN months
**     NNN years
**     start of month
**     start of year
**     start of week
**     start of day
**     weekday N
**     auto
**     unixepoch
**     unixday
**     julianday
**     ceday
**     ansiday
**     localtime
**     utc
**     IANAZone         IANA timezone name
**     -IANAZone        eg:  'Canada/Mountain'
**     naive            make datetime naive (timezone unaware)
**     zone tz          tz is 'Z' for Zulu or a timezone offset from UTC (+NN:NN) or (-NN:NN)
**     NNN offset       NNN is the offset in minutes
**     unstick          unstick the IANA Timezone
**     pit              convert to Point-in-Time (same as unstick)
**
** Return 0 on success and 1 if there is any kind of error. If the error
** is in a system call (i.e. localtime()), then an error message is written
** to context pCtx. If the error is an unrecognized modifier, no error is
** written to pCtx.
*/
static int parseModifier(
  sqlite3_context *pCtx,      /* Function context */
  const char *z,              /* The text of the modifier */
  int n,                      /* Length of zMod in bytes */
  DateTime *p,                /* The date/time value to be modified */
  int idx                     /* Parameter index of the modifier */
){
  int rc = 1;
  double r;

  if (strchr(z, '/')>0) {      /* Modifier with embeded / are IANA Zone names */
#if SQLITE_DATETIME_DEFAULT != 3
    if (!sqlite3NotPureFunc(pCtx)) return 1;
#endif
    if (*z == '-') return _unapplyZone(pCtx, z+1, p);
    if (p->validTZ) return _applyZone(pCtx, z, p);
    if (_unapplyZone(pCtx, z, p)) return 1;             // display time is localtime
    if (_applyZone(pCtx, z, p)) return 1;
    return 0;
  }

  switch(sqlite3UpperToLower[(u8)z[0]] ){
    case 'a': {
      /*
      **    auto
      **
      ** If rawS is available, then interpret as a julian day number, or
      ** a unix timestamp, depending on its magnitude.
      */
      if( sqlite3_stricmp(z, "auto")==0 ){
        if( idx>1 ) return 1; /* IMP: R-33611-57934 */
        if( !p->rawS || p->validJD ){
          rc = 0;
          p->rawS = 0;
        }else if( p->s>=-21086676*(i64)10000        /* -4713-11-24 12:00:00 */
               && p->s<=(25340230*(i64)10000)+799   /*  9999-12-31 23:59:59 */
        ){
          r = p->s*1000.0 + 210866760000000.0;
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      /*
      **    ansiday / ansisecond
      **
      ** Treat the current value of p->s as the number of
      ** days since 1601-01-01 00:00:00.
      **
      */
      else if( sqlite3_stricmp(z, "ansiday")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 3, 0);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "ansisecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 3, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
    case 'c': {
      /*
      **    ceday / cesecond
      **
      ** Treat the current value of p->s as the number of
      ** days since 0001-01-01 00:00:00.
      **
      */
      if( sqlite3_stricmp(z, "ceday")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 2, 0);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "cesecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 2, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
    case 'i': {
      if( sqlite3_stricmp(z, "ijd")==0  && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = p->s;
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
    case 'j': {
      /*
      **    julianday
      **
      ** Always interpret the prior number as a julian-day value.  If this
      ** is not the first modifier, or if the prior argument is not a numeric
      ** value in the allowed range of julian day numbers understood by
      ** SQLite (0..5373484.5) then the result will be NULL.
      */
      if( sqlite3_stricmp(z, "julianday")==0 ){
        if( idx>1 ) return 1;  /* IMP: R-31176-64601 */
        if( p->validJD && p->rawS ){
          rc = 0;
          p->rawS = 0;
        }
      }
      else if( sqlite3_stricmp(z, "juliansecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 1, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
#ifndef SQLITE_OMIT_LOCALTIME
    case 'l': {
      /*    localtime
      **
      ** Assuming the current time value is UTC (a.k.a. GMT), shift it to
      ** show local time.
      */
      if( sqlite3_stricmp(z, "localtime")==0 ){
#if SQLITE_DATETIME_DEFAULT != 3
    if (!sqlite3NotPureFunc(pCtx)) return 1;
#endif
        rc = toLocaltime(p, pCtx);
      }
      break;
    }
#endif
    case 'm': {
      /*
      **    mjulianday / mjuliansecond
      **
      ** Treat the current value of p->s as the number of
      ** days since 1858-11-17 00:00:00.000 Z
      **
      */
      if( sqlite3_stricmp(z, "mjulianday")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 5, 0);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "mjuliansecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 5, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
    case 'p': {
      if( sqlite3_stricmp(z, "pit")==0 ){
        p->zone = 0;
        rc = 0;
      }
      break;
    }
    case 'r': {
      /*
      **    rjulianday / rjuliansecond
      **
      ** Treat the current value of p->s as the number of
      ** days since 1858-11-16 12:00:00.000 Z
      **
      */
      if( sqlite3_stricmp(z, "rjulianday")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 4, 0);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "rjuliansecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 4, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
    case 't': {
      /*
      **    tmday / tmsecond
      **
      ** Treat the current value of p->s as the number of
      ** days since 2001-01-01 00:00:00.000 Z
      **
      */
      if( sqlite3_stricmp(z, "tmday")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 7, 0);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "tmsecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 7, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      break;
    }
    case 'n': {
      /*
      **    naive
      **
      ** Convert to a naive (unlocalized) datetime
      ** The internal julian day number is adjusted so that it is relative to
      ** Meridian indicated by the current timezone offset, rather than the
      ** Prime Meridian, and the timezone is unset.
      */
      if( sqlite3_stricmp(z, "naive")==0 ){
        computeJD(p);
        p->iJD += p->validTZ*p->tz*60000;
        p->validTZ = 0;
        p->tz = 0;
        p->zone = 0;
        clearYMD_HMS(p);
        rc = 0;
      }
      break;
    }
    case 'z': {
      /*
      **    zone tz
      **
      ** Set the current offset to the zone specified
      ** and recompute the iJD if required
      */
      if( sqlite3_strnicmp(z, "zone", 4)==0 && strlen(z)>4 ){
        DateTime x;
        x.validTZ = 0;
        x.tz = 0;
        x.zone = 0;
        rc = parseTimezone(z+4, &x);
        rc = rc || (!x.validTZ);
        if( rc ) break;
        computeJD(p);
        p->tz = x.tz;
        if (!p->validTZ) p->iJD -= p->tz*60000;
        p->validTZ = 1;
        p->zone = 0;
        clearYMD_HMS(p);
      }
      break;
    }
    case 'u': {
      /*
      **    unixepoch
      **
      ** Treat the current value of p->s as the number of
      ** seconds since 1970.  Convert to a real julian day number.
      */
      if( sqlite3_stricmp(z, "unixepoch")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 6, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "unixsecond")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 6, 1);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "unixday")==0 && p->rawS ){
        if( idx>1 ) return 1;  /* IMP: R-49255-55373 */
        r = timestamp2iJD(p->s, 6, 0);
        if( r>=0.0 && r<464269060800000.0 ){
          clearYMD_HMS(p);
          p->iJD = (sqlite3_int64)(r + 0.5);
          p->validJD = 1;
          p->rawS = 0;
          rc = 0;
        }
      }
      else if( sqlite3_stricmp(z, "unstick")==0 ){
          p->zone = 0;
          rc = 0;
      }
#ifndef SQLITE_OMIT_LOCALTIME
      else if( sqlite3_stricmp(z, "utc")==0 ){
#if SQLITE_DATETIME_DEFAULT != 3
    if (!sqlite3NotPureFunc(pCtx)) return 1;
#endif
        if( p->validTZ==0 ){
          i64 iOrigJD;              /* Original localtime */
          i64 iGuess;               /* Guess at the corresponding utc time */
          int cnt = 0;              /* Safety to prevent infinite loop */
          i64 iErr;                 /* Guess is off by this much */

          computeJD(p);
          iGuess = iOrigJD = p->iJD;
          iErr = 0;
          do{
            DateTime new;
            memset(&new, 0, sizeof(new));
            iGuess -= iErr;
            new.iJD = iGuess;
            new.validJD = 1;
            rc = toLocaltime(&new, pCtx);
            if( rc ) return rc;
            new.validTZ = 0;
            new.iJD += new.tz * 60000;
            iErr = new.iJD - iOrigJD;
          }while( iErr && cnt++<3 );
          memset(p, 0, sizeof(*p));
          p->iJD = iGuess;
          p->validJD = 1;
        }
        p->tz = 0;
        p->validTZ = 1;
        p->zone = 0;
        clearYMD_HMS(p);
        rc = SQLITE_OK;
      }
#endif
      break;
    }
    case 'w': {
      /*
      **    weekday N
      **
      ** Move the date to the same time on the next occurrence of
      ** weekday N where 0==Sunday, 1==Monday, and so forth.  If the
      ** date is already on the appropriate weekday, this is a no-op.
      ** if a persistent zone is in effect then HMS remains constant
      */
      if( sqlite3_strnicmp(z, "weekday ", 8)==0
               && sqlite3AtoF(&z[8], &r, sqlite3Strlen30(&z[8]), SQLITE_UTF8)>0
               && r>=0.0 && r<7.0 && (n=(int)r)==r ){
        sqlite3_int64 Z;
        computeJD(p);
        Z = ((p->iJD + p->validTZ*p->tz*60000 + 129600000)/86400000) % 7;
        if( Z>n ) Z -= 7;
        if( p->zone ){
          clearYMD_HMS(p);
          computeYMD_HMS(p, pCtx);
          p->D += (n - Z);
          p->validTZ = 0;
          p->validJD = 0;
          computeJD(p);
          p->tz = LookupIANAOffsetLMT(pCtx, p->zone, p->iJD);
          p->iJD -= p->tz*60000;
          p->validTZ = 1;
        }else{
          p->iJD += (n - Z)*86400000;
        }
        clearYMD_HMS(p);
        rc = 0;
      }
      break;
    }
    case 's': {
      /*
      **    start of TTTTT
      **
      ** Move the date backwards to the beginning of the current day,
      ** or month or year.
      **
      **    subsecond
      **    subsec
      **
      ** Show subsecond precision in the output of datetime() and
      ** unixepoch() and strftime('%s').
      */
      if( sqlite3_strnicmp(z, "start of ", 9)!=0 ){
        if( sqlite3_stricmp(z, "subsec")==0
         || sqlite3_stricmp(z, "subsecond")==0
        ){
          p->useSubsec = 1;
          rc = 0;
        }
        break;
      }
      if( !p->validJD && !p->validYMD && !p->validHMS ) break;
      z += 9;
      computeYMD(p, pCtx);
      p->h = p->m = 0;
      p->s = 0.0;
      p->rawS = 0;
      p->validHMS = 1;
      p->validJD = 0;
      if( sqlite3_stricmp(z,"month")==0 ){
        p->D = 1;
        rc = 0;
      }else if( sqlite3_stricmp(z,"year")==0 ){
        p->M = 1;
        p->D = 1;
        rc = 0;
      }else if( sqlite3_stricmp(z,"day")==0 ){
        rc = 0;
      }
      if( p->zone ){
        p->tz = 0;
        p->validTZ = 0;
        computeJD(p);
        p->tz = LookupIANAOffsetLMT(pCtx, p->zone, p->iJD);                  /***/
        p->iJD -= p->tz * 60000;
        p->validTZ = 1;
      }else{
        computeJD(p);
      }
      clearYMD_HMS(p);
      break;
    }
    case '+':
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      double rRounder;
      int i;
      int Y,M,D,h,m,x;
      const char *z2 = z;
      for(n=1; z[n]; n++){
        if( z[n]==':' ) break;
        if( sqlite3Isspace(z[n]) ) break;
        if( z[n]=='-' && n==5 && getDigits(&z[1], "40f", &Y)==1 ) break;
      }
      if( sqlite3AtoF(z, &r, n, SQLITE_UTF8)<=0 ){
        rc = 1;
        break;
      }
      if( z[n]=='-' ){
        /* A modifier of the form (+|-)YYYY-MM-DD adds or subtracts the
        ** specified number of years, months, and days.  MM is limited to
        ** the range 0-11 and DD is limited to 0-30.
        */
        if( z[0]!='+' && z[0]!='-' ) break;  /* Must start with +/- */
        if( NEVER(n!=5) ) break;             /* Must be 4-digit YYYY */
        if( getDigits(&z[1], "40f-20a-20d", &Y, &M, &D)!=3 ) break;
        if( M>=12 ) break;                   /* M range 0..11 */
        if( D>=31 ) break;                   /* D range 0..30 */
        computeYMD_HMS(p, pCtx);
        p->validJD = 0;
        if( z[0]=='-' ){
          p->Y -= Y;
          p->M -= M;
          D = -D;
        }else{
          p->Y += Y;
          p->M += M;
        }
        x = p->M>0 ? (p->M-1)/12 : (p->M-12)/12;
        p->Y += x;
        p->M -= x*12;
        computeJD(p);
        p->validHMS = 0;
        p->validYMD = 0;
        p->iJD += (i64)D*86400000;
        if( z[11]==0 ){
          rc = 0;
          break;
        }
        if( sqlite3Isspace(z[11])
         && getDigits(&z[12], "20c:20e", &h, &m)==2
        ){
          z2 = &z[12];
          n = 2;
        }else{
          break;
        }
      }
      if( z2[n]==':' ){
        /* A modifier of the form (+|-)HH:MM:SS.FFF adds (or subtracts) the
        ** specified number of hours, minutes, seconds, and fractional seconds
        ** to the time.  The ".FFF" may be omitted.  The ":SS.FFF" may be
        ** omitted.
        */

        DateTime tx;
        sqlite3_int64 day;
        if( !sqlite3Isdigit(*z2) ) z2++;
        memset(&tx, 0, sizeof(tx));
        if( parseHhMmSs(z2, &tx) ) break;
        computeJD(&tx);
        tx.iJD -= 43200000;
        day = tx.iJD/86400000;
        tx.iJD -= day*86400000;
        if( z[0]=='-' ) tx.iJD = -tx.iJD;
        computeJD(p);
        clearYMD_HMS(p);
        p->iJD += tx.iJD;
        rc = 0;
        break;
      }

      /* If control reaches this point, it means the transformation is
      ** one of the forms like "+NNN days".  */
      z += n;
      while( sqlite3Isspace(*z) ) z++;
      n = sqlite3Strlen30(z);
      if( n>10 || n<3 ) break;
      if( sqlite3UpperToLower[(u8)z[n-1]]=='s' ) n--;
      computeJD(p);
      if( sqlite3_strnicmp(z, "offset", n)==0 ){ /* process the (+|-)NNN offset */
        p->tz = (int)r;
        if (!p->validTZ) p->iJD -= p->tz*60000;
        p->validTZ = 1;
        p->zone = 0;
        clearYMD_HMS(p);
        rc = !(p->validJD && p->validTZ);
        break;
      }
      rc = 1;
      rRounder = r<0 ? -0.5 : +0.5;
      for(i=0; i<ArraySize(aXformType); i++){
        if( aXformType[i].nName==n
         && sqlite3_strnicmp(aXformType[i].zName, z, n)==0
         && r>-aXformType[i].rLimit && r<aXformType[i].rLimit
        ){
          switch( i ){
            case 3: { /* Special processing to add days */
              assert( strcmp(aXformType[i].zName,"day")==0 );
              computeYMD_HMS(p, pCtx);
              p->D += (int)r;
              p->validJD = 0;
              r -= (int)r;
              break;
            }
            case 4: { /* Special processing to add months */
              assert( strcmp(aXformType[i].zName,"month")==0 );
              computeYMD_HMS(p, pCtx);
              p->M += (int)r;
              x = p->M>0 ? (p->M-1)/12 : (p->M-12)/12;
              p->Y += x;
              p->M -= x*12;
              p->validJD = 0;
              r -= (int)r;
              break;
            }
            case 5: { /* Special processing to add years */
              int y = (int)r;
              assert( strcmp(aXformType[i].zName,"year")==0 );
              computeYMD_HMS(p, pCtx);
              p->Y += y;
              p->validJD = 0;
              r -= (int)r;
              break;
            }
          }
          if( i>2 && p->zone ){
            p->tz = 0;
            p->validTZ = 0;
            computeJD(p);
            p->tz = LookupIANAOffsetLMT(pCtx, p->zone, p->iJD);                  /***/
            p->iJD -= p->tz * 60000;
            p->validTZ = 1;
          }else{
            computeJD(p);
          }
          clearYMD_HMS(p);
          p->iJD += (sqlite3_int64)(r*1000.0*aXformType[i].rXform + rRounder);
          rc = 0;
          break;
        }
      }
      computeJD(p);                                                                 /**/
      clearYMD_HMS(p);                                                              /**/
      break;
    }
    default: {
      break;
    }
  }
  return rc;
}

/*
** Process time function arguments.  argv[0] is a date-time stamp.
** argv[1] and following are modifiers.  Parse them all and write
** the resulting time into the DateTime structure p.  Return 0
** on success and 1 if there are any errors.
**
** If there are zero parameters (if argc<=0) then assume a default
** value of "now" for argv[0] if argc==0 and "txn" if argc<0.  SQL
** functions will always have argc>=0, but the special implementations
** of CURRENT_TIME, CURRENT_DATE, and CURRENT_TIMESTAMP set argc to -1
** in order to force the use of 'txn' semantics.
*/
static int isDate(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv,
  DateTime *p
){
  int i, n;
  const unsigned char *z;
  int eType;
  memset(p, 0, sizeof(*p));
  if( argc<=0 ){
    if( !sqlite3NotPureFunc(context) ) return 1;
    return setCurrentStmtTime(context, p, argc<0);
  }
  if( (eType = sqlite3_value_type(argv[0]))==SQLITE_FLOAT
                   || eType==SQLITE_INTEGER ){
    setRawDateNumber(p, sqlite3_value_double(argv[0]));
  }else{
    z = sqlite3_value_text(argv[0]);
    if( !z || parseDateOrTime(context, (char*)z, p) ){
      return 1;
    }
  }
  if(!p->rawS) computeJD(p);
  if(p->validJD) clearYMD_HMS(p);
  for(i=1; i<argc; i++){
    z = sqlite3_value_text(argv[i]);
    n = sqlite3_value_bytes(argv[i]);
    if( z==0 || parseModifier(context, (char*)z, n, p, i) ) return 1;
  }
  computeJD(p);
  if( p->isError || !validJulianDay(p->iJD) ) return 1;
  return 0;
}

#if 0 // Deprecated Functions de-activvated but code included to prevert fossil explosions

/*
** The following routines implement the various date and time functions
** of SQLite.
*/

/*
**    julianday( TIMESTRING, MOD, MOD, ...)
**
** Return the julian day number of the date specified in the arguments
*/
static void juliandayFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  DateTime x;
  if( isDate(context, argc, argv, &x)==0 ){
    computeJD(&x);
    sqlite3_result_double(context, x.iJD/86400000.0);
  }
}

/*
**    unixepoch( TIMESTRING, MOD, MOD, ...)
**
** Return the number of seconds (including fractional seconds) since
** the unix epoch of 1970-01-01 00:00:00 GMT.
*/
static void unixepochFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  DateTime x;
  if( isDate(context, argc, argv, &x)==0 ){
    computeJD(&x);
    if( x.useSubsec ){
      sqlite3_result_double(context, (x.iJD - 21086676*(i64)10000000)/1000.0);
    }else{
      sqlite3_result_int64(context, x.iJD/1000 - 21086676*(i64)10000);
    }
  }
}

#endif

/*
**    datetime( TIMESTRING, MOD, MOD, ...)
**
** Return YYYY-MM-DD HH:MM:SS.SSS TZ
*/
static void datetimeFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  DateTime x;
  int flags = (intptr_t)sqlite3_user_data(context);
  if( isDate(context, argc, argv, &x)==0 ){
    int Y, s;
    int i = 0;
    char zBuf[32];

    if (x.validJD)
    {
        clearYMD_HMS(&x);
        computeYMD_HMS(&x, context);
    }
    Y = x.Y;
    if( Y<0 ){
      Y = -Y;
      zBuf[i++] = '-';
    }
    zBuf[i++] = '0' + (Y/1000)%10;
    zBuf[i++] = '0' + (Y/100)%10;
    zBuf[i++] = '0' + (Y/10)%10;
    zBuf[i++] = '0' + (Y)%10;
    zBuf[i++] = '-';
    zBuf[i++] = '0' + (x.M/10)%10;
    zBuf[i++] = '0' + (x.M)%10;
    zBuf[i++] = '-';
    zBuf[i++] = '0' + (x.D/10)%10;
    zBuf[i++] = '0' + (x.D)%10;
    zBuf[i++] = ' ';
    zBuf[i++] = '0' + (x.h/10)%10;
    zBuf[i++] = '0' + (x.h)%10;
    zBuf[i++] = ':';
    zBuf[i++] = '0' + (x.m/10)%10;
    zBuf[i++] = '0' + (x.m)%10;
    zBuf[i++] = ':';
    s = (int)x.s;
    zBuf[i++] = '0' + (s/10)%10;
    zBuf[i++] = '0' + (s)%10;
    if ((flags & 1) || (x.useSubsec)) {
        s = ((int)(x.s*1000.0+.5)) % 1000;
        zBuf[i++] = '.';
        zBuf[i++] = '0' + (s/100)%10;
        zBuf[i++] = '0' + (s/10)%10;
        zBuf[i++] = '0' + (s)%10;
    }
    if (x.validTZ && (flags & 2))
    {
        zBuf[i++] = ' ';
        if (x.tz == 0)
        {
            zBuf[i++] = 'Z';
        }
        else
        {
            s = x.tz / 60;
            if (x.tz < 0)
            {
                s = -s;
                zBuf[i++] = '-';
            }
            else
                zBuf[i++] = '+';
            zBuf[i++] = '0' + (s/10)%10;
            zBuf[i++] = '0' + (s)%10;
            zBuf[i++] = ':';
            s = x.tz % 60;
            if (s<0) s = -s;
            zBuf[i++] = '0' + (s/10)%10;
            zBuf[i++] = '0' + (s)%10;
        }
    }
    zBuf[i] = 0;
    sqlite3_result_text(context, zBuf, i, SQLITE_TRANSIENT);
  }
}

/*
**    time( TIMESTRING, MOD, MOD, ...)
**
** Return HH:MM:SS
*/
static void timeFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  DateTime x;
  int flags = (intptr_t)sqlite3_user_data(context);
  if( isDate(context, argc, argv, &x)==0 ){
    int s;
    int i = 0;
    char zBuf[32];
    if(x.validJD) clearYMD_HMS(&x);
    computeYMD_HMS(&x, context);
    zBuf[i++] = '0' + (x.h/10)%10;
    zBuf[i++] = '0' + (x.h)%10;
    zBuf[i++] = ':';
    zBuf[i++] = '0' + (x.m/10)%10;
    zBuf[i++] = '0' + (x.m)%10;
    zBuf[i++] = ':';
    s = (int)x.s;
    zBuf[i++] = '0' + (s/10)%10;
    zBuf[i++] = '0' + (s)%10;
    if ((flags & 1) || (x.useSubsec)) {
        s = ((int)(x.s*1000.0+.5)) % 1000;
        zBuf[i++] = '.';
        zBuf[i++] = '0' + (s/100)%10;
        zBuf[i++] = '0' + (s/10)%10;
        zBuf[i++] = '0' + (s)%10;
    }
    if (x.validTZ && (flags & 2))
    {
        zBuf[i++] = ' ';
        if (x.tz == 0)
        {
            zBuf[i++] = 'Z';
        }
        else
        {
            s = x.tz / 60;
            if (x.tz < 0)
            {
                s = -s;
                zBuf[i++] = '-';
            }
            else
                zBuf[i++] = '+';
            zBuf[i++] = '0' + (s/10)%10;
            zBuf[i++] = '0' + (s)%10;
            zBuf[i++] = ':';
            s = x.tz % 60;
            if (s<0) s = -s;
            zBuf[i++] = '0' + (s/10)%10;
            zBuf[i++] = '0' + (s)%10;
        }
    }
    zBuf[i] = 0;
    sqlite3_result_text(context, zBuf, i, SQLITE_TRANSIENT);
  }
}

/*
**    date( TIMESTRING, MOD, MOD, ...)
**
** Return YYYY-MM-DD
*/
static void dateFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  DateTime x;
  int i = 0;
  int flags = (intptr_t)sqlite3_user_data(context);
  if( isDate(context, argc, argv, &x)==0 ){
    int Y, s;
    char zBuf[32];
    if (x.validJD) clearYMD_HMS(&x);
    computeYMD(&x, context);
    Y = x.Y;
    if( Y<0 ){
        Y = -Y;
        zBuf[i++] = '-';
    }
    zBuf[i++] = '0' + (Y/1000)%10;
    zBuf[i++] = '0' + (Y/100)%10;
    zBuf[i++] = '0' + (Y/10)%10;
    zBuf[i++] = '0' + (Y)%10;
    zBuf[i++] = '-';
    zBuf[i++] = '0' + (x.M/10)%10;
    zBuf[i++] = '0' + (x.M)%10;
    zBuf[i++] = '-';
    zBuf[i++] = '0' + (x.D/10)%10;
    zBuf[i++] = '0' + (x.D)%10;
    if (x.validTZ && (flags & 2))
    {
        zBuf[i++] = ' ';
        if (x.tz == 0)
        {
            zBuf[i++] = 'Z';
        }
        else
        {
            s = x.tz / 60;
            if (x.tz < 0)
            {
                s = -s;
                zBuf[i++] = '-';
            }
            else
                zBuf[i++] = '+';
            zBuf[i++] = '0' + (s/10)%10;
            zBuf[i++] = '0' + (s)%10;
            zBuf[i++] = ':';
            s = x.tz % 60;
            if (s<0) s = -s;
            zBuf[i++] = '0' + (s/10)%10;
            zBuf[i++] = '0' + (s)%10;
        }
    }

    zBuf[i] = 0;
    sqlite3_result_text(context, zBuf, i, SQLITE_TRANSIENT);
  }
}

static inline int isLeapYear(int Y)
{
    return (((Y % 4 == 0)  &&  (Y % 100 != 0)) || (Y % 400 == 0));
}

/*
**    strftime( FORMAT, TIMESTRING, MOD, MOD, ...)
**
** Return a string described by FORMAT.  Conversions as follows:
**
**   %d  day of month
**   %f  ** fractional seconds  SS.SSS
**   %G  year 0000-9999 of current week's Thursday (ISO Year)
**   %H  hour 00-24
**   %i  ISO Date (yyyy-Www-d)
**   %I  ISO YearWeek (yyyy-Www)
**   %j  day of year 000-366
**   %J  ** julian day number
**   %m  month 01-12
**   %M  minute 00-59
**   %Q  ISO Quarter q (1-4)
**   %q  ISO Quarter stamp yyyy-Qq
**   %s  seconds since 1970-01-01
**   %S  seconds 00-59
**   %u  day of week 1-7  monday==1 (ISO Weekday)
**   %V  week of year 01-53 week belongs to year with most of its days (ISO Week)
**   %w  day of week 0-6  Sunday==0
**   %W  week of year 00-53
**   %X  ISO Month (yyyy-Mmm)
**   %x  ISO Month mm
**   %Y  year 0000-9999
**   %Z  localization (timezone)
**   %z  localization (offset)
**   %%  %
*/
static void strftimeFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  DateTime x;
  size_t i,j;
  sqlite3 *db;
  const char *zFmt;
  sqlite3_str sRes;


  if( argc==0 ) return;
  zFmt = (const char*)sqlite3_value_text(argv[0]);
  if( zFmt==0 || isDate(context, argc-1, argv+1, &x) ) return;
  db = sqlite3_context_db_handle(context);
  sqlite3StrAccumInit(&sRes, 0, 0, 0, db->aLimit[SQLITE_LIMIT_LENGTH]);

  computeJD(&x);
  if(x.validJD) clearYMD_HMS(&x);
  computeYMD_HMS(&x, context);
  for(i=j=0; zFmt[i]; i++){
    if( zFmt[i]!='%' ) continue;
    if( j<i ) sqlite3_str_append(&sRes, zFmt+j, (int)(i-j));
    i++;
    j = i + 1;
    switch( zFmt[i] ){
      case 'd': {
        sqlite3_str_appendf(&sRes, "%02d", x.D);
        break;
      }
      case 'f': {
        double s = x.s;
        if( s>59.999 ) s = 59.999;
        sqlite3_str_appendf(&sRes, "%06.3f", s);
        break;
      }
      case 'H': {
        sqlite3_str_appendf(&sRes, "%02d", x.h);
        break;
      }
      case 'W': /* Fall thru */
      case 'j': {
        int nDay;             /* Number of days since 1st day of year */
        DateTime y = x;
        y.validJD = 0;
        y.M = 1;
        y.D = 1;
        computeJD(&y);
        nDay = (int)((x.iJD-y.iJD+43200000)/86400000);
        if( zFmt[i]=='W' ){
          int wd;   /* 0=Monday, 1=Tuesday, ... 6=Sunday */
          wd = (int)(((x.iJD+(x.validTZ*x.tz*60000)+43200000)/86400000+7)%7);
          sqlite3_str_appendf(&sRes,"%02d",(nDay+7-wd)/7);
        }else{
          sqlite3_str_appendf(&sRes,"%03d",nDay+1);
        }
        break;
      }
      case 'J': {
        sqlite3_str_appendf(&sRes,"%.16g",x.iJD/86400000.0);
        break;
      }
      case 'm': {
        sqlite3_str_appendf(&sRes,"%02d",x.M);
        break;
      }
      case 'M': {
        sqlite3_str_appendf(&sRes,"%02d",x.m);
        break;
      }
      case 's': {
        if( x.useSubsec ){
          sqlite3_str_appendf(&sRes,"%.3f",
                (x.iJD - 21086676*(i64)10000000)/1000.0);
        }else{
          i64 iS = (i64)(x.iJD/1000 - 21086676*(i64)10000);
          sqlite3_str_appendf(&sRes,"%lld",iS);
        }
        break;
      }
      case 'S': {
        sqlite3_str_appendf(&sRes,"%02d",(int)x.s);
        break;
      }
      case 'u': {
        sqlite3_str_appendchar(&sRes, 1,
                       (char)(((x.iJD + (x.validTZ*x.tz*60000) + 648000000)/86400000) % 7) + '1');
        break;
      }
      case 'w': {
        sqlite3_str_appendchar(&sRes, 1,
                       (char)(((x.iJD + (x.validTZ*x.tz*60000) + 129600000)/86400000) % 7) + '0');
        break;
      }
      case 'i':  /* Fall thru yyyy-Www-d */
      case 'I':  /* Fall thru yyyy-Www   */
      case 'V':  /* Fall thru w          */
      case 'X':  /* Fall thru yyyy-Mmm   */
      case 'x':  /* Fall thru mm         */
      case 'G':  /* Fall thru yyyy       */
      case 'q':  /* Fall thru yyyy-Qq    */
      case 'Q':  /*           q          */ {
        DateTime y = x;
        computeJD(&y);
        y.validYMD = 0;
        /* Adjust date to Thursday this week:
           The number in parentheses is 0 for Monday, 3 for Thursday */
        y.iJD += (3 - (((y.iJD + (y.validTZ*y.tz*60000) + 648000000)/86400000) % 7))*86400000;
        computeYMD(&y, context);
        if( zFmt[i]=='G' ){
          sqlite3_str_appendf(&sRes,"%04d",y.Y);
        }else{
          sqlite3_int64 tJD = y.iJD;
          int nWeek;
          if( zFmt[i]=='i' || zFmt[i]=='I' )
            sqlite3_str_appendf(&sRes, "%04d-W", y.Y);
          else if( zFmt[i]=='q' )
            sqlite3_str_appendf(&sRes, "%04d-Q", y.Y);
          else if( zFmt[i]=='X' )
            sqlite3_str_appendf(&sRes, "%04d-M%02d", y.Y, y.M);
          else if( zFmt[i]=='x' )
            sqlite3_str_appendf(&sRes, "%02d", y.M);
          /* borrowed code from case 'j' */
          y.validJD = 0;
          y.M = 1;
          y.D = 1;
          computeJD(&y);
          nWeek = (int)((tJD-y.iJD+43200000)/86400000/7+1);
          if( zFmt[i]=='Q' || zFmt[i]=='q') {
            nWeek = (nWeek + 12) / 13;
            nWeek -= (nWeek == 5);
            sqlite3_str_appendchar(&sRes, 1, nWeek + '0');
          }
          else if( zFmt[i]=='V' || zFmt[i]=='I')
            sqlite3_str_appendf(&sRes,"%02d", nWeek);
          else if( zFmt[i]=='i' )
            sqlite3_str_appendf(&sRes, "%02d-%d", nWeek, ((x.iJD + (x.validTZ*x.tz*60000) + 648000000)/86400000) % 7 + 1);
        }
        break;
      }
      case 'Y': {
        sqlite3_str_appendf(&sRes,"%04d", x.Y);
        break;
      }
      case 'Z': {
        if (x.validTZ) {
            if (x.tz) {
                int sgn, h, m;
                sgn = x.tz<0 ? -1 : 1;
                m = x.tz * sgn;
                h = m / 60;
                m = m % 60;
                if (sgn<0)
                    sqlite3_str_appendchar(&sRes, 1, '-');
                else
                    sqlite3_str_appendchar(&sRes, 1, '+');
                sqlite3_str_appendf(&sRes, "%02d:%02d", h, m);
            } else sqlite3_str_appendchar(&sRes, 1, 'Z');
        } else  /* Chop trailing spaces (uses internal structure data) */
            while((sRes.nChar>0)&&(sRes.zText[sRes.nChar-1]==' '))
                sRes.nChar--;
        break;
      }
      case 'z': {
        if (x.validTZ)
            sqlite3_str_appendf(&sRes, "%d", x.tz);
        break;
      }
      case '%': {
        sqlite3_str_appendchar(&sRes, 1, '%');
        break;
      }
      default: {
        sqlite3_str_reset(&sRes);
        return;
      }
    }
  }
  if( j<i ) sqlite3_str_append(&sRes, zFmt+j, (int)(i-j));
  sqlite3ResultStrAccum(context, &sRes);
}

/*
** current_time()
**
** This function returns the same value as time('txn').
*/
static void ctimeFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **NotUsed2
){
  UNUSED_PARAMETER2(NotUsed, NotUsed2);
  timeFunc(context, -1, 0);
}

/*
** current_date()
**
** This function returns the same value as date('txn').
*/
static void cdateFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **NotUsed2
){
  UNUSED_PARAMETER2(NotUsed, NotUsed2);
  dateFunc(context, -1, 0);
}

/*
** timediff(DATE1, DATE2)
**
** Return the amount of time that must be added to DATE2 in order to
** convert it into DATE2.  The time difference format is:
**
**     +YYYY-MM-DD HH:MM:SS.SSS
**
** The initial "+" becomes "-" if DATE1 occurs before DATE2.  For
** date/time values A and B, the following invariant should hold:
**
**     datetime(A) == (datetime(B, timediff(A,B))
**
** Both DATE arguments must be either a julian day number, or an
** ISO-8601 string.  The unix timestamps are not supported by this
** routine.
*/
static void timediffFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  char sign;
  int Y, M;
  DateTime d1, d2;
  sqlite3_str sRes;
  if( isDate(context, 1, argv, &d1)     ) return;
  if( isDate(context, 1, &argv[1], &d2) ) return;
  computeYMD_HMS(&d1, context);
  computeYMD_HMS(&d2, context);
  if( d1.iJD>=d2.iJD ){
    sign = '+';
    Y = d1.Y - d2.Y;
    if( Y ){
      d2.Y = d1.Y;
      d2.validJD = 0;
      computeJD(&d2);
    }
    M = d1.M - d2.M;
    if( M<0 ){
      Y--;
      M += 12;
    }
    if( M!=0 ){
      d2.M = d1.M;
      d2.validJD = 0;
      computeJD(&d2);
    }
    if( d1.iJD<d2.iJD ){
      M--;
      if( M<0 ){
        M = 11;
        Y--;
      }
      d2.M--;
      if( d2.M<1 ){
        d2.M = 12;
        d2.Y--;
      }
      d2.validJD = 0;
      computeJD(&d2);
    }
    d1.iJD -= d2.iJD;
    d1.iJD += 148699540800000;
  }else{
    sign = '-';
    Y = d2.Y - d1.Y;
    if( Y ){
      d2.Y = d1.Y;
      d2.validJD = 0;
      computeJD(&d2);
    }
    M = d2.M - d1.M;
    if( M<0 ){
      Y--;
      M += 12;
    }
    if( M!=0 ){
      d2.M = d1.M;
      d2.validJD = 0;
      computeJD(&d2);
    }
    if( d1.iJD>d2.iJD ){
      M--;
      if( M<0 ){
        M = 11;
        Y--;
      }
      d2.M++;
      if( d2.M>12 ){
        d2.M = 1;
        d2.Y++;
      }
      d2.validJD = 0;
      computeJD(&d2);
    }
    d1.iJD = d2.iJD - d1.iJD;
    d1.iJD += 148699540800000;
  }
  d1.validYMD = 0;
  d1.validHMS = 0;
  d1.validTZ = 0;
  computeYMD_HMS(&d1, context);
  sqlite3StrAccumInit(&sRes, 0, 0, 0, 100);
  sqlite3_str_appendf(&sRes, "%c%04d-%02d-%02d %02d:%02d:%06.3f",
       sign, Y, M, d1.D-1, d1.h, d1.m, d1.s);
  sqlite3ResultStrAccum(context, &sRes);
}


/*
** current_timestamp()
**
** This function returns the same value as datetime('txn').
*/
static void ctimestampFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **NotUsed2
){
  UNUSED_PARAMETER2(NotUsed, NotUsed2);
  datetimeFunc(context, -1, 0);
}
#endif /* !defined(SQLITE_OMIT_DATETIME_FUNCS) */

#ifdef SQLITE_OMIT_DATETIME_FUNCS
/*
** If the library is compiled to omit the full-scale date and time
** handling (to get a smaller binary), the following minimal version
** of the functions current_time(), current_date() and current_timestamp()
** are included instead. This is to support column declarations that
** include "DEFAULT CURRENT_TIME" etc.
**
** This function uses the C-library functions time(), gmtime()
** and strftime(). The format string to pass to strftime() is supplied
** as the user-data for the function.
*/
static void currentTimeFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  time_t t;
  char *zFormat = (char *)sqlite3_user_data(context);
  sqlite3_int64 iT;
  struct tm *pTm;
  struct tm sNow;
  char zBuf[20];

  UNUSED_PARAMETER(argc);
  UNUSED_PARAMETER(argv);

  iT = sqlite3StmtCurrentTime(context, 1);
  if( iT<=0 ) return;
  t = iT/1000 - 10000*(sqlite3_int64)21086676;
#if HAVE_GMTIME_R
  pTm = gmtime_r(&t, &sNow);
#else
  sqlite3_mutex_enter(sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MAIN));
  pTm = gmtime(&t);
  if( pTm ) memcpy(&sNow, pTm, sizeof(sNow));
  sqlite3_mutex_leave(sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MAIN));
#endif
  if( pTm ){
    strftime(zBuf, 20, zFormat, &sNow);
    sqlite3_result_text(context, zBuf, -1, SQLITE_TRANSIENT);
  }
}
#endif

#ifndef SQLITE_OMIT_DATETIME_FUNCS
static int _DateTimeZCollFunc(void *notUsed, int nKey1, const char *pKey1, int nKey2, const char *pKey2)
{
    DateTime x, y;
    int rc;
    char buf1[64];
    char buf2[64];
    int tKey1 = nKey1>63 ? 63 : nKey1;
    int tKey2 = nKey2>63 ? 63 : nKey2;

    memset(&x, 0, sizeof(x));
    memset(&y, 0, sizeof(y));
    /* Prepare arguments into zero-terminated strings (max first 63 bytes) */
    memcpy(buf1, pKey1, tKey1);
    buf1[tKey1+1] = 0;
    memcpy(buf2, pKey2, tKey2);
    buf2[tKey2+1] = 0;
    /* Process our datetime strings */
    if (!parseDateOrTime(0, buf1, &x)) computeJD(&x);
    if (!parseDateOrTime(0, buf2, &y)) computeJD(&y);
    /* Full Order:  validTZ=0 by iJD followed by validTZ=1 by iJD */
    if(x.validJD && y.validJD){
        if (x.validTZ == y.validTZ){
            if (x.iJD < y.iJD)
                return -1;
            if (x.iJD > y.iJD)
                return 1;
            return 0;
        }
        if (y.validTZ) return -1;
        return 1;
    }
    if(y.validJD && (!x.validJD)) return -1;
    if(x.validJD && (!y.validJD)) return 1;
    rc = memcmp(pKey1, pKey2, nKey1 < nKey2 ? nKey1 : nKey2);
    if(rc==0){
        if(nKey1<nKey2) return -1;
        if(nKey1>nKey2) return 1;
        return 0;
    }
    return rc;
}
static int _DateTimeCollFunc(void *notUsed, int nKey1, const char *pKey1, int nKey2, const char *pKey2)
{
    DateTime x, y;
    int rc;
    char buf1[64];
    char buf2[64];
    int tKey1 = nKey1>63 ? 63 : nKey1;
    int tKey2 = nKey2>63 ? 63 : nKey2;

    memset(&x, 0, sizeof(x));
    memset(&y, 0, sizeof(y));
    /* Prepare arguments into zero-terminated strings (max first 63 bytes) */
    memcpy(buf1, pKey1, tKey1);
    buf1[tKey1+1] = 0;
    memcpy(buf2, pKey2, tKey2);
    buf2[tKey2+1] = 0;
    /* Process our datetime strings */
    if (!parseDateOrTime(0, buf1, &x)) computeJD(&x);
    if (!parseDateOrTime(0, buf2, &y)) computeJD(&y);
    /* Order by iJD assume naive is utc */
    if(x.validJD && y.validJD){
            if (x.iJD < y.iJD)
                return -1;
            if (x.iJD > y.iJD)
                return 1;
            return 0;
    }
    if(y.validJD && (!x.validJD)) return -1;
    if(x.validJD && (!y.validJD)) return 1;
    rc = memcmp(pKey1, pKey2, nKey1 < nKey2 ? nKey1 : nKey2);
    if(rc==0){
        if(nKey1<nKey2) return -1;
        if(nKey1>nKey2) return 1;
        return 0;
    }
    return rc;
}

static int _DateTimeCollInsert_(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int nErr = 0;
    nErr += sqlite3_create_collation(db, "DATETIME", SQLITE_UTF8, 0, (void*)_DateTimeCollFunc);
    nErr += sqlite3_create_collation(db, "DATETIMEZ", SQLITE_UTF8, 0, (void*)_DateTimeZCollFunc);
    return nErr ? SQLITE_ERROR : SQLITE_OK;
}

/*
** timestampFunc returns a floating point stamp of the current time
**
** User Data Value determines the epoch and unit to use:
**
*/
static void timestampFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    sqlite_int64 jd = -1;
    int flag = (int)(intptr_t)sqlite3_user_data(context);
    double result = 0.0;

    if (argc)
    {
        DateTime x;
        memset(&x, 0, sizeof(x));
        if( isDate(context, argc, argv, &x) )
            return;
        if( x.validTZ || !(flag & 0x0002) )
            jd = x.iJD;
        flag = flag | x.useSubsec;
    }
    else
        if (sqlite3NotPureFunc(context))
            jd = sqlite3StmtCurrentTime(context,0);

    if (jd >= 0)
    {
        int ms    = (flag & 0x0001) != 0;
        int secs  = (flag & 0x0100) != 0;

        jd -= iJDepoch[(flag & 0x00f0) >> 4];
        if (secs)
        {
            result = jd / 1000.0;
            jd = result;
        }
        else result = jd / 86400000.0;

        if( flag && ((!secs) || ms) )
            sqlite3_result_double(context, result);
        else
            sqlite3_result_int64(context, jd);
    }
}

/*
 * JulianNow and JulianTxn are "deterministic" functions that return the current JulianDay 'now' or 'txn' respectively
 * and are intended to be used in "generated always" expressions where fully deterministic functions are required.
 */
static void purejulianday(sqlite3_context * context, int argc, sqlite3_value **argv)
{
    sqlite3_result_double(context, sqlite3StmtCurrentTime(context, (int)(intptr_t)sqlite3_user_data(context)) / 86400000.0);
}
#endif

/*
** This function registered all of the above C functions as SQL
** functions.  This should be the only routine in this file with
** external linkage.
*/
#define NEW_DFUNCTION(zName, nArg, iArg, bNC, xFunc) \
  {nArg, SQLITE_FUNC_BUILTIN|SQLITE_FUNC_SLOCHNG|SQLITE_UTF8, \
   SQLITE_INT_TO_PTR(iArg), 0, xFunc, 0, 0, 0, #zName, {0} }
#define NEW_PURE_DATE(zName, nArg, iArg, bNC, xFunc) \
  {nArg, SQLITE_FUNC_BUILTIN|\
         SQLITE_FUNC_SLOCHNG|SQLITE_UTF8|SQLITE_FUNC_CONSTANT, \
   SQLITE_INT_TO_PTR(iArg), 0, xFunc, 0, 0, 0, #zName, {0} }
#if (SQLITE_DATETIME_DEFAULT < 0)
#define SQLITE_DATETIME_DEFAULT 0
#endif
#if (SQLITE_DATETIME_DEFAULT > 3)
#define SQLITE_DATETIME_DEFAULT 3
#endif
void sqlite3RegisterDateTimeFunctions(void){
  static FuncDef aDateTimeFuncs[] = {
#ifndef SQLITE_OMIT_DATETIME_FUNCS
    NEW_PURE_DATE(date,             -1, SQLITE_DATETIME_DEFAULT, 0, dateFunc      ),
    NEW_PURE_DATE(dateo,            -1, 0,                       0, dateFunc      ),
    NEW_PURE_DATE(datem,            -1, 1,                       0, dateFunc      ),
    NEW_PURE_DATE(datez,            -1, 2,                       0, dateFunc      ),
    NEW_PURE_DATE(datemz,           -1, 3,                       0, dateFunc      ),
    NEW_PURE_DATE(time,             -1, SQLITE_DATETIME_DEFAULT, 0, timeFunc      ),
    NEW_PURE_DATE(timeo,            -1, 0,                       0, timeFunc      ),
    NEW_PURE_DATE(timem,            -1, 1,                       0, timeFunc      ),
    NEW_PURE_DATE(timez,            -1, 2,                       0, timeFunc      ),
    NEW_PURE_DATE(timemz,           -1, 3,                       0, timeFunc      ),
    NEW_PURE_DATE(datetime,         -1, SQLITE_DATETIME_DEFAULT, 0, datetimeFunc  ),
    NEW_PURE_DATE(datetimeo,        -1, 0,                       0, datetimeFunc  ),
    NEW_PURE_DATE(datetimem,        -1, 1,                       0, datetimeFunc  ),
    NEW_PURE_DATE(datetimez,        -1, 2,                       0, datetimeFunc  ),
    NEW_PURE_DATE(datetimemz,       -1, 3,                       0, datetimeFunc  ),
    NEW_PURE_DATE(strftime,         -1, 0,                       0, strftimeFunc  ),
    NEW_DFUNCTION(current_time,      0, SQLITE_DATETIME_DEFAULT, 0, ctimeFunc     ),
    NEW_DFUNCTION(current_timestamp, 0, SQLITE_DATETIME_DEFAULT, 0, ctimestampFunc),
    NEW_DFUNCTION(current_date,      0, SQLITE_DATETIME_DEFAULT, 0, cdateFunc     ),
    NEW_PURE_DATE(juliannow,        -1, 0,                                 0, purejulianday ),
    NEW_PURE_DATE(juliantxn,        -1, 1,                                 0, purejulianday ),
    NEW_PURE_DATE(ijd,              -1, 0x0000,                            0, timestampFunc ),
    NEW_PURE_DATE(julianday,        -1, 0x0010 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(juliandayo,       -1, 0x0010,                            0, timestampFunc ),
    NEW_PURE_DATE(juliandaym,       -1, 0x0011,                            0, timestampFunc ),
    NEW_PURE_DATE(juliandayz,       -1, 0x0012,                            0, timestampFunc ),
    NEW_PURE_DATE(juliandaymz,      -1, 0x0013,                            0, timestampFunc ),
    NEW_PURE_DATE(juliansecond,     -1, 0x0110 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(juliansecondo,    -1, 0x0110,                            0, timestampFunc ),
    NEW_PURE_DATE(juliansecondm,    -1, 0x0111,                            0, timestampFunc ),
    NEW_PURE_DATE(juliansecondz,    -1, 0x0112,                            0, timestampFunc ),
    NEW_PURE_DATE(juliansecondmz,   -1, 0x0113,                            0, timestampFunc ),
    NEW_PURE_DATE(ceday,            -1, 0x0020 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(cedayo,           -1, 0x0020,                            0, timestampFunc ),
    NEW_PURE_DATE(cedaym,           -1, 0x0021,                            0, timestampFunc ),
    NEW_PURE_DATE(cedayz,           -1, 0x0022,                            0, timestampFunc ),
    NEW_PURE_DATE(cedaymz,          -1, 0x0023,                            0, timestampFunc ),
    NEW_PURE_DATE(cesecond,         -1, 0x0120 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(cesecondo,        -1, 0x0120,                            0, timestampFunc ),
    NEW_PURE_DATE(cesecondm,        -1, 0x0121,                            0, timestampFunc ),
    NEW_PURE_DATE(cesecondz,        -1, 0x0122,                            0, timestampFunc ),
    NEW_PURE_DATE(cesecondmz,       -1, 0x0123,                            0, timestampFunc ),
    NEW_PURE_DATE(ansiday,          -1, 0x0030 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(ansidayo,         -1, 0x0030,                            0, timestampFunc ),
    NEW_PURE_DATE(ansidaym,         -1, 0x0031,                            0, timestampFunc ),
    NEW_PURE_DATE(ansidayz,         -1, 0x0032,                            0, timestampFunc ),
    NEW_PURE_DATE(ansidaymz,        -1, 0x0033,                            0, timestampFunc ),
    NEW_PURE_DATE(ansisecond,       -1, 0x0130 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(ansisecondo,      -1, 0x0130,                            0, timestampFunc ),
    NEW_PURE_DATE(ansisecondm,      -1, 0x0131,                            0, timestampFunc ),
    NEW_PURE_DATE(ansisecondz,      -1, 0x0132,                            0, timestampFunc ),
    NEW_PURE_DATE(ansisecondmz,     -1, 0x0133,                            0, timestampFunc ),
    NEW_PURE_DATE(rjulianday,       -1, 0x0040 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(rjuliandayo,      -1, 0x0040,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliandaym,      -1, 0x0041,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliandayz,      -1, 0x0042,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliandaymz,     -1, 0x0043,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliansecond,    -1, 0x0140 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(rjuliansecondo,   -1, 0x0140,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliansecondm,   -1, 0x0141,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliansecondz,   -1, 0x0142,                            0, timestampFunc ),
    NEW_PURE_DATE(rjuliansecondmz,  -1, 0x0143,                            0, timestampFunc ),
    NEW_PURE_DATE(mjulianday,       -1, 0x0050 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(mjuliandayo,      -1, 0x0050,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliandaym,      -1, 0x0051,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliandayz,      -1, 0x0052,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliandaymz,     -1, 0x0053,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliansecond,    -1, 0x0150 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(mjuliansecondo,   -1, 0x0150,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliansecondm,   -1, 0x0151,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliansecondz,   -1, 0x0152,                            0, timestampFunc ),
    NEW_PURE_DATE(mjuliansecondmz,  -1, 0x0153,                            0, timestampFunc ),
    NEW_PURE_DATE(unixday,          -1, 0x0060 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(unixdayo,         -1, 0x0060,                            0, timestampFunc ),
    NEW_PURE_DATE(unixdaym,         -1, 0x0061,                            0, timestampFunc ),
    NEW_PURE_DATE(unixdayz,         -1, 0x0062,                            0, timestampFunc ),
    NEW_PURE_DATE(unixdaymz,        -1, 0x0063,                            0, timestampFunc ),
    NEW_PURE_DATE(unixepoch,        -1, 0x0160 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(unixepocho,       -1, 0x0160,                            0, timestampFunc ),
    NEW_PURE_DATE(unixepochm,       -1, 0x0161,                            0, timestampFunc ),
    NEW_PURE_DATE(unixepochz,       -1, 0x0162,                            0, timestampFunc ),
    NEW_PURE_DATE(unixepochmz,      -1, 0x0163,                            0, timestampFunc ),
    NEW_PURE_DATE(unixsecond,       -1, 0x0160 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(unixsecondo,      -1, 0x0160,                            0, timestampFunc ),
    NEW_PURE_DATE(unixsecondm,      -1, 0x0161,                            0, timestampFunc ),
    NEW_PURE_DATE(unixsecondz,      -1, 0x0162,                            0, timestampFunc ),
    NEW_PURE_DATE(unixsecondmz,     -1, 0x0163,                            0, timestampFunc ),
    NEW_PURE_DATE(tmday,            -1, 0x0070 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(tmdayo,           -1, 0x0070,                            0, timestampFunc ),
    NEW_PURE_DATE(tmdaym,           -1, 0x0071,                            0, timestampFunc ),
    NEW_PURE_DATE(tmdayz,           -1, 0x0072,                            0, timestampFunc ),
    NEW_PURE_DATE(tmdaymz,          -1, 0x0073,                            0, timestampFunc ),
    NEW_PURE_DATE(tmsecond,         -1, 0x0170 + SQLITE_DATETIME_DEFAULT,  0, timestampFunc ),
    NEW_PURE_DATE(tmsecondo,        -1, 0x0170,                            0, timestampFunc ),
    NEW_PURE_DATE(tmsecondm,        -1, 0x0171,                            0, timestampFunc ),
    NEW_PURE_DATE(tmsecondz,        -1, 0x0172,                            0, timestampFunc ),
    NEW_PURE_DATE(tmsecondmz,       -1, 0x0173,                            0, timestampFunc ),
    PURE_DATE(timediff,          2, 0, 0, timediffFunc  ),
#else
    STR_FUNCTION(current_time,       0, "%H:%M:%S",          0, currentTimeFunc),
    STR_FUNCTION(current_date,       0, "%Y-%m-%d",          0, currentTimeFunc),
    STR_FUNCTION(current_timestamp,  0, "%Y-%m-%d %H:%M:%S", 0, currentTimeFunc),
#endif
  };
  sqlite3InsertBuiltinFuncs(aDateTimeFuncs, ArraySize(aDateTimeFuncs));
#ifndef SQLITE_OMIT_DATETIME_FUNCS
  sqlite3_auto_extension((void*)_DateTimeCollInsert_);
#endif
}

