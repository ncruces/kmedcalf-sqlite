/*
** 2022-11-16
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
**
** This is a utility for converting binary to base85 or vice-versa.
** It can be built as a standalone program or an SQLite3 extension.
**
** Much like base64 representations, base85 can be sent through a
** sane ASCII channel unmolested. It also plays nicely in CSV or
** written as TCL brace-enclosed literals or SQL string literals.
** It is not suited for unmodified use in XML-like documents.
**
** The encoding used resembles Ascii85, but was devised by the author
** (Larry Brasfield) before Mozilla, Adobe, ZMODEM or other Ascii85
** variant sources existed, in the 1984 timeframe on a VAX mainframe.
** Further, this is an independent implementation of a base85 system.
** Hence, the author can rightfully put this into the public domain.
**
** Base85 numerals are taken from the set of 7-bit ASCII codes,
** excluding control characters and Space ! " ' ( ) { | } ~ Del
** in code order representing digit values 0 to 84 (base 10.)
**
** Groups of 4 bytes, interpreted as little-endian 32-bit values,
** are represented as 5-digit base85 numbers with MS to LS digit
** order. Groups of 1-3 bytes are represented with 2-4 digits.
** (This is the reason for little-endian interpretation; it has
** a more natural transition to byte groups smaller than 4 bytes.)
** Groups of 0 bytes are represented with 0 digits and vice-versa.
**
** Any character not in the base85 numeral set delimits groups.
** When base85 is streamed or stored in containers of indefinite
** size, newline is used to separate it into sub-sequences of no
** more than 80 digits so that fgets() can be used to read it.
**
** Length limitations are not imposed except that the runtime
** SQLite string or blob length limits are respected. Otherwise,
** any length binary sequence can be represented and recovered.
** Base85 sequences can be concatenated by separating them with
** a non-base85 character; the conversion to binary will then
** be the concatenation of the represented binary sequences.

** The standalone program either converts base85 on stdin to create
** a binary file or converts a binary file to base85 on stdout.
** Read or make it blurt its help for invocation details.
**
** The SQLite3 extension creates a function, base85(x), which will
** either convert text base85 to a blob or a blob to text base85
** and return the result (or throw an error for other types.)
**
** To build the extension, define PP symbol SQLITE3_EXTENSION. Eg.:
** set SQDIR=<your favorite SQLite checkout directory>
** gcc -O2 -shared -I$SQDIR -DSQLITE3_EXTENSION -o base85.so base85.c
** gcc -O2 -shared -I%SQDIR% -DSQLITE3_EXTENSION -o base85.dll base85.c
** cl /Os -I%SQDIR% -DSQLITE3_EXTENSION base85.c -link -dll -out:base85.dll
** To build the standalone program, just compile and link it.
*/

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>

#ifndef SQLITE3_EXTENSION
#define SQLITE3_EXTENSION
#endif

#ifdef SQLITE3_EXTENSION
# include "sqlite3ext.h"

SQLITE_EXTENSION_INIT1;
#else
static char *zHelp =
  "Usage: base85 <dirFlag> <binFile>\n"
  " <dirFlag> is either -r or -w to read or write <binFile>.\n"
  " <binFile> names a binary file to be rendered or created\n"
  "   which will be converted to/from base85 on stdout/stdin.\n"
  ;

static void sayHelp(){
  printf("%s", zHelp);
}
#endif

/* Say whether c is a base85 numeral. Not yet used. */
#define IS_B85( c ) (3==((c)>='#')+((c)<='&')+((c)>='*')+((c)<='z'))
/* Classify c according to interval within ASCII set w.r.t. base85
 * Values of 1 and 3 are base85 numerals. Values of 0, 2, or 4 are not.
 */
#define B85_CLASS( c ) (((c)>='#')+((c)>'&')+((c)>='*')+((c)>'z'))
static unsigned char b85_cOffset[] = { 0, '#', 0, '*'-4, 0 };
#define B85_OS( c ) b85_cOffset[B85_CLASS(c)]

static unsigned char base85DigitValue( char c ){
  unsigned char rv = (unsigned char)(c - '#');
  if( rv>87 ) return 0xff;
  if( (rv & ~3)!=0 ) rv -= 3;
  return rv;
}

static char * skipNonB85( char *s ){
  char c;
  while( (c = *s) && B85_OS(c)==0 ) ++s;
  return s;
}

static char base85Numeral( unsigned char b ){
  return ((b & ~3)==0)? (char)(b + '#') : (char)(b - 4 + '*');
}

static void toBase85( unsigned char *pIn, int nbIn, char *pOut ){
  *pOut = 0;
  while( nbIn > 0 ){
    static signed char ncio[] = { 0, 2, 3, 4, 5 };
    int nbi = (nbIn > 4)? 4 : nbIn;
    unsigned long qv = 0L;
    int nbe = 0;
    signed char nco;
    while( nbe++ < nbi ){
      qv = (qv<<8) | *pIn++;
    }
    nco = ncio[nbi];
    nbIn -= nbi;
    pOut[nco] = 0;
    while( nco > 0 ){
      unsigned char dv = (unsigned char)(qv % 85);
      qv /= 85;
      pOut[--nco] = base85Numeral(dv);
    }
    pOut += ncio[nbi];
  }
}

static size_t fromBase85( char *pIn, int ncIn, unsigned char *pOut ){
  size_t nOut = 0;
  if( ncIn>0 && pIn[ncIn-1]=='\n' ) --ncIn;
  while( ncIn>0 ){
    static signed char nboi[] = { 0, 0, 1, 2, 3, 4 };
    char *pUse = skipNonB85(pIn);
    unsigned long qv = 0L;
    int nti, nbo;
    ncIn -= (pUse - pIn);
    if( ncIn==0 ) break;
    pIn = pUse;
    nti = (ncIn>5)? 5 : ncIn;
    nbo = nboi[nti];
    while( nti>0 ){
      unsigned char dv = base85DigitValue(*pIn++);
      --ncIn;
      if( dv>=85 ) break;
      qv = 85 * qv + dv;
      --nti;
    }
    nbo -= nti;
    nOut += nbo;
    while( nbo-- > 0 ){
      *pOut++ = (qv >> (8*nbo))&0xff;
    }
  }
  return nOut;
}

#ifdef SQLITE3_EXTENSION

static void base85(sqlite3_context *context, int na, sqlite3_value *av[]){
  int nb, nc, nv = sqlite3_value_bytes(av[0]);
  int nvMax = sqlite3_limit(sqlite3_context_db_handle(context),
                            SQLITE_LIMIT_LENGTH, -1);
  char *cBuf;
  unsigned char *bBuf;
  assert(na==1);
  switch( sqlite3_value_type(av[0]) ){
  case SQLITE_BLOB:
    nb = nv;
    nc = 5*(nv/4) + nv%4 + 2;
    if( nvMax < nc ){
      sqlite3_result_error(context, "blob expanded to base85 too big.", -1);
    }
    cBuf = sqlite3_malloc(nc);
    if( !cBuf ) goto memFail;
    bBuf = (unsigned char*)sqlite3_value_blob(av[0]);
    toBase85(bBuf, nb, cBuf);
    sqlite3_result_text(context, cBuf, nc, sqlite3_free);
    break;
  case SQLITE_TEXT:
    nc = nv;
    nb = 4*(nv/5) + nv%5; /* may overestimate */
    if( nvMax < nb ){
      sqlite3_result_error(context, "blob from base85 may be too big.", -1);
    }else if( nb<1 ){
      nb = 1;
    }
    bBuf = sqlite3_malloc(nb);
    if( !bBuf ) goto memFail;
    cBuf = (char *)sqlite3_value_text(av[0]);
    nb = (int)fromBase85(cBuf, nc, bBuf);
    sqlite3_result_blob(context, bBuf, nb, sqlite3_free);
    break;
  default:
    sqlite3_result_error(context, "base85 accepts only blob or text.", -1);
    break;
  }
  return;
 memFail:
  sqlite3_result_error(context, "base85 OOM", -1);
}

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_base85_init(sqlite3 *db, char **pzErr,
                        const sqlite3_api_routines *pApi){
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErr;
  return sqlite3_create_function
    (db, "base85", 1,
     SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS|SQLITE_UTF8, 0, base85, 0, 0);
}

#else /* standalone program */

int main(int na, char *av[]){
  int cin;
  int rc = 0;
  unsigned char bufIn[64];
  char bufOut[5*(sizeof(bufIn)/4)+2];
  size_t nio;
  char rw;
  FILE *fb;
  char fmode[3] = "xb";
  if( na < 3 || av[1][0]!='-' || (rw = av[1][1])==0 || (rw!='r' && rw!='w') ){
    sayHelp();
    return 0;
  }
  fmode[0] = rw;
  fb = fopen(av[2], fmode);
  if( !fb ){
    fprintf(stderr, "Cannot open %s for %c\n", av[2], rw);
    rc = 1;
  }else{
    switch( rw ){
    case 'r':
      while( (nio = fread( bufIn, 1, sizeof(bufIn), fb))>0 ){
        toBase85( bufIn, (int)nio, bufOut );
        fprintf(stdout, "%s\n", bufOut);
      }
      break;
    case 'w':
      while( 0 != fgets(bufOut, sizeof(bufOut), stdin) ){
        int nc = strlen(bufOut);
        size_t nbo = fromBase85( bufOut, nc, bufIn );
        if( 1 != fwrite(bufIn, nbo, 1, fb) ) rc = 1;
      }
      break;
    default:
      sayHelp();
      rc = 1;
    }
    fclose(fb);
  }
  return rc;
}

#endif

