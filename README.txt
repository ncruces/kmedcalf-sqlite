
A bunch of SQLite3 extensions.

FileName        Description                                     Source
--------------- ----------------------------------------------- -------------
base64.c        Base64 encode/decode                            Keith Medcalf
base85.c        Base85 encode/decode                            Larry Brasfield
coreinit.c      Auto-Extensions (see below)                     Keith Medcalf
date.c          Drop-in Replacement for date.c                  Keith Medcalf
define.c        DefineScalar and UnDefineScalar UDFs            Keith Medcalf
icu.c           Drop-in Replacement for /etc/icu/icu.c          Keith Medcalf
interpolate.c   Interpolation functions and vtab                Third-party
ipaddress.c     Useful functions/collation for ipaddresses      Keith Medcalf
pivot_vtab.c    Pivot Virtual Table                             Third-Party
sqlatof.c       SQL AtoF function                               Keith Medcalf
sqlautobusy.c   Always set a default busy timeout               Keith Medcalf
sqlfcmp.c       Floating Point comparisons and rounding         Keith Medcalf
sqlframes.c     Internal Only functions to detect recursion     Keith Medcalf
sqlfunc.c       Various useful running Window Functions         Keith Medcalf
sqlfwin.c       Various useful Windows API interfaces           Keith Medcalf
sqlhash.c       MD2/MD4/MD5/SHA/SHA1/SHA2/SHA3                  Keith Medcalf
sqlhash.win.c   Example using Windows CryptoAPI                 Keith Medcalf
sqlite_mprint.c Memoize values                                  Third-party
sqlmath.c       Add Math Library functions                      Keith Medcalf
sqlmathtest.c   hasLongDouble() function                        Keith Medcalf
sqlnumeric.c    Implement ToBestType(x) function                Keith Medcalf
sqlserialize.c  Binary Serialization/Aggregate/Deserialize      Keith Medcalf
sqlsize.c       Compute record cell sizes                       Keith Medcalf
sqltext.c       Implement useful text functions                 Keith Medcalf
sqltime.c       Date and Time Additions                         Keith Medcalf
sqlunhex.c      Reverse of HEX function                         Keith Medcalf
statement_vtab.c Parameterized Statement Virtual Table          Third-Party
unifuzz.c       Unicode functions                               Third-party
uuid.c          RFC Type 4 big-endian UUIDs                     Keith Medcalf
vsv.c           Variably Separated Values vtab                  Keith Medcalf

coreinit.c is designed to be used with the -DSQLITE_EXTRA_INIT=core_init to permit
           additional extensions to be added to the SQLite3 core when the various
           extension source files are appended to the sqlite3.c amalgamation
           (coreinit.c must be appended last)

Except for the files originating from third-parties, all these files are released into
the public domain.  If you break them you own all the pieces.  Code may contain
bits that are under various other terms.  See the files for information.

base64.c    contains code as shown in source copyright notices
            blob argument is encoded to base64 text result
            text argument is base64 decoded to blob result
date.c      drop in replacement for SQlite3's date.c that implements proper localization
define.c    defineScalar(name, selectstmt) and undefinescalar(name) to define scalar
            functions using SQL:  select DefineScalar('diagonal', 'select sqrt(pow(?,2)+pow(?,2))')
icu.c       drop in replacement for SQLite3's icu.c
ipaddress.c contains code by Paul Vixie and the Internet Software Consortium, see source
sqlhash.c   contains code from various sources, see source

sqlsize.c   depends on current (documented) implementation details of SQLite3
sqltext.c   between functions LowerPrefix and UpperPrefix
sqltime.c   additional functions for datetime handling
uuid.c      modified copy of the distributed SQLite3 uuid.c extension
vsv.c       modified copy of the distributed SQLite3 csv.c extension
sqlnumeric.c ToBestType SQL Function same as affinity=numeric implementation
             from vsv.c
sqlframes.c is an internal only function adding vmFrames() and recursing()
            which ill return the number of frames attached to the current
            virtual machine.  (not recursing()) means that the current
            trigger was not fired recursively and recursing() means it was.
            vmFrames() returns the current frame count (<1 means not recursing,
            >1 means recursing).

func.patch  is a patch to func.c to make the sum intermediates LONGDOUBLE_TYPE
vdbe.patch  is a patch to vdbe.c to make 'now' statement stable vs step stable
where.patch is a patch to where.c to allow the path tracking to be changed
ConnCurrentTime.patch adds an iCurrentTime to the sqlite* structure so that the
                      date.c module included here can implement the 'txn' modifier.

precisetime.patch
            is a patch to os_win.c to use GetSystemTimePreciseAsFileTime so that the
            current time is not dependent on the timer frequency
            ** DEPRECATED adjust timeResolution for Windows with sqlfwin.c
                          SQL function timeResolution(..)
                          -1 for min updates/1 for max updates
                          0 or no arguments to just query

Full IANA Timezone support requires some tables to be accessible to the included date.c

These tables can be built as follows (see the code for details).  A tz.sql file is
included which will add the necessary tables to a database (2022b Database)

tzExport.py tz.db
    Create tz.db with IANA Timezone Tables

LoadPoly.py combined.json tz.db tz_geopoly --nautical
    Add Timezone Polygon data to tz.db

pragma.c.patch and mkpragmatab.tcl are patches for src\pragma.c and tool\mkpragmatab.tcl
which modify some existing pragma's and add some new ones so that the schema.sql views
will work properly across all attached files and schema's.

mpsw.py    and adds functionality to the connection and cursor
           classes.  See the code (or help) for details.
           Supports datatype adaption for datetime, uuid/guid, boolean, ipaddress/netaddr
            datetime.datetime objects are stored as ISO text %Y-%m-%d %H:%M:%f ((+|-)%H:%M|Z)?
            uuid objects are stored as a "blob"
            True/False are stored as 1 and 0
            ipaddress/netaddr objects are stored as text (presentation) strings
           Converter recognizes types datetime, uuid, guid, bool, boolean, ipaddress, netaddr, json, pickle
            datetime type converts ISO text to python datetime.datetime
            uuid/guid type converts "blob" to python uuid object
            bool/boolean type converts not zero to True, zero to false, null to None (using SQLite rules)
            ipaddress/netaddr converts ip presentation strings to ipaddress/netaddr objects
            json converts the json string to an object using json.loads
            pickle converts the pickle string to an object using pickle.loads

            select x as 'name [datatype]' from x      select x as 'name [datetime]' from x
            or
            create table x (x datatype basetype ...); create table x (x datetime text collate datetime)

DateLimits.sql          creates a statement_vtab that generates datetime boundaries
DateTimeDiff.sql        creates a statement_vtab that computes interval between dates
DateTimeSplit.sql       creates a statement_vtab that computes the attributes of a date
PragmaSettings/SQL      creates a statement_vtab that returns the pragma settings
renumscalar.sql         renumbers the scalar_functions table (use only on databases not in use)
Schema.sql              creates a set of data-dictionary tables
series.sql              iseries for generating integer series, rseries for generating real series
Split.sql               creates a statement_vtab that is the reverse of group_concat
tz.sql                  creates the tz tables needed by datetime
ianazones.sql           virtual table to find timezone by latitude/longitude

pivot_vtab.c    see https://github.com/jakethaw

icu.c adds an auto-create for ICU collation sequences.  names of the format ICU_<locale>[_<options>] will be created when required.
      <options> Accent Insensitive(AI); Case Insensitive(CI)

      eg:  icu_aici; icu_en_us_ai; icu_sv; icu_en_US_POSIX_AI;

      icu_trans(<transform>, <expr>) applies transformation to <expr> and returns result
      fold(<expr>) icu_trans using the transformation "Latin;NFKD;[:Nonspacing Mark:]Remove;Lower;NFKC;ascii"
      icu_locale([locale]) get/set the currently set locale
      icu_enumLocale([n])  returns number of locale/collation, with argument returns name of that locale


**NOTE** All the 64-bit extensions have been pre-compiled with quadmath enabled.

