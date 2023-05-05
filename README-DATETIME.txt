
The DateTime structure consists of the following elements:

    iJD         Integer             Number of Milliseconds since the
                                    Julian Epoch

    Y           Integer             Year (-4713 to 9999)
    M           Integer             Month of Year (01 to 12)
    D           Integer             Day of Month (01 to 31)
    h           Integer             Hour of Day (00 to 23)
    m           Integer             Minute of Hour (00 to 59)
    s           Real                Second of Minute (00.000 to 59.999)
    tz          Integer             Timezone Offset in Minutes
                                    (divide by 4 to get the Meridian,
                                     West is negative)

    validJD     Integer (Boolean)   The iJD contents are valid
    validYMD    Integer (Boolean)   The Y, M, and D contents are valid
    validHMS    Integer (Boolean)   THe h, m, and s contents are valid
    validTZ     Integer (Boolean)   The tz contents are valid
    zone        Integer             The Zone_ID of the IANA Zone (sticky)
    rawS        Integer (Boolean)   s contains a raw value
    useSubsec   Integer (Boolean)   display to full precision (milliseconds)

This version of the datetime functions attempts to always maintain
values relative to the Prime Meridian.  If an IANA timezone is
specified then that timezone is "sticky" (meaning that it automatically
adjusts the offset and that +- day operations keep the same "time" even if
the offset changes).

Specifically, if validTZ is True then the following conditions hold:

    iJD is the Number of Milliseconds since the Julian Epoch UT
    Y M D h m s tz is one of the many valid display formats for this point in time
    tz/4 is the Local Meridian for which Y M D h m s have/will be computed (negative is West)

and if validTZ is False, then

    iJD is the Number of Milliseconds since the Julian Epoch at some, unknown, Meridian
    Y M D h m s is the display format for this iJD at the Prime Meridian
    (that is, more clearly, that the display assumes the iJD is relative to the Prime
    Meridian for the purposes of calculating Y M D h m s, however, the rotational offset
    (Meridian) is unknown.

When a DateTime is loaded with a time number and the appropriate
modifier, or the current time (now/txn), then the reference is assumed
to be relative to UT (the Prime Meridian) and the tz is set to 0 and
validTZ is set to 1.

The modifier 'local' can be used in place of the pair 'now','localtime'
and will result in the iJD relative to UT, and the appropriate offset as
provided by the underlying OS being set (tz).

An IANA zone can be used in place of the pair 'now',IANAZone (that is
datetime('Canada/Mountain') is equivalent to
datetime('now','Canada/Mountain')) and will result in the iJD relative
to UT, and the appropriate offset as provided by the loaded IANA
Timezone database being set (tz).  The IANA Zone will be sticky.  If you
modify the datetime then the offset will be recomputed.  If you use one
of the 'start of x' modifiers, then a valid 'start of x' offset will be
set.

When an datetime string is loaded, it depends whether or not a timezone
is parsed.  If no timezone appears then validTZ is not set (the datetime
is naive).  If a timezone appears, then validTZ is set, the offset is
set to the specified timezone, iJD is relative to UT, and the "display
format" is not adjusted to UT (previously an input was assumed to be UT
and the display of an input "instant time" is converted to a display of
UT with a loss of localization) but the tz offset is maintained.

Also, "display format adjustments" are made automatically (no need to
add 0) such that Feb 29 in a non-leap year automatically becomes
Mar 01.  That is x IS datetime(x) is True if and only if the text format
is fully specified and complete and correct.

The operation of the 'localtime' and 'utc' modifiers is changed.

when validTZ is set then these functions (localtime / utc) merely set
the tz (offset) for "display format" and do not modify the iJD.

when validTZ is unset then 'localtime' modifier assumes that the iJD is
relative to UT and sets validTZ and the tz; the 'utc' modifier assumes
that the iJD is in localtime and sets validTZ and the tz and recomutes
iJD to be relative to UT.

Five new modifiers have been added:

naive       unsets the validTZ and clears the offset and recalculates
            the iJD relative to some arbitrary meridian (such that the
            display YMDhms is unchanged).

zone ...    sets the validTZ and offset of the current display time then
            recalculates iJD relative to the Prime Meridian

... offset  same as zone but uses minutes.
            'zone -07:00' is the same as '-420 offset'

pit         remove the sticky zone and convert to a point-in-time

unstick     remove the sticky zone and convert to a point-in-time


A new modifier that recognizes IANA timezones has been added.
This requires data stored in the database.

localtime <-> 'IANAZone'
utc       <-> '-IANAZone'

except that the IANA database is used for the offset lookup.  Setting
an IANAZone is "sticky" (though -IANAZone is not).

eg:  for a computer located in Calgary Alberta, then 'utc' is the same
as '-Canada/Mountain' and 'localtime' is the same as 'Canada/Mountain'
except that 'localtime' and 'utc' use mapped years whereas the IANA
tables are expected to be complete.  Using 'localtime' and 'utc' will
use the underlying OS localtime functions against mapped years so,
although the computed result will be correct, it may not be consistent
with reality as experienced by a direct observer looking at a
clock-on-the-wall at the specified instant time at the point on the geod
being referred to. (The instant time will be correct, the timezone
offset may not be as expected unless full IANA timezone tables are
used).

A compile time define has been added:  SQLITE_DATETIME_DEFAULT default is 3
  0 means output neither the milliseconds nor the timezone (like original)
  1 means Output Milliseconds (datetime, date, time, timestamp functions)
  2 means Output the Timezone (datetime, date, time; timestamp functions only output if validTZ)
  3 means do both of the above

Format 0 and 2 respect the usesubsec flag.
Format 1 and 3 act as if usesubsec always specified.

Calling a function using its basename will use the SQLITE_DATETIME_DEFAULT
settings.  Optionally a specifier may be appended to the basename to
generate specific output.
       Value                Suffix
SQLITE_DATETIME_DEFAULT
         0                    o
         1                    m
         2                    z
         3                    mz

Setting SQLITE_DATETIME_DEFAULT to use the timezone (2 or 3) also
affects the timestamp functions so that they only output
confirmed valid values, and NULL if the iJD is relative to an unknown
Meridian. (this is also different behaviour -- previously timestamp
functions would output the rotated iJD (relative to an arbitary Meridian
value, which is not a proper offset from UT).

The above define affects the default output format for the functions
if no suffix is appended.  By appending the appropriate suffix, behaviour
can be forced.

Datetime functions:

date
time
datetime

The strftime and ijd functions do not have suffixed versions.
ijd(...) outputs the actual iJD value.

The following epoch's are recognized:

Julian Epoch   -4713-11-24 12:00:00.000 Z   julian
Common Era      0001-01-01 00:00:00.000 Z   ce
ANSI Epoch      1601-01-01 00:00:00.000 Z   ansi
Reduced Julian  1858-11-16 12:00:00.000 Z   rjulian
Modified Julian 1858-11-17 00:00:00.000 Z   mjulian
Unix Epoch      1970-01-01 00:00:00.000 Z   unix
Third Millenium 2001-01-01 00:00:00.000 Z   tm

The following stamp types are recognized:

day
second

epoch (as in unixepoch) is an alias for second (but only for the unix epoch):  unixsecondmz <-> unixepochmz

A specific timestamp is requested by concatenating the epoch, stamp type, and optional format suffix (o/m/z/mz)

    ijd
    julianday     juliandayo     juliandaym     juliandayz     juliandaymz
    juliansecond  juliansecondo  juliansecondm  juliansecondz  juliansecondmz
    ceday         cedayo         cedaym         cedayz         cedaymz
    cesecond      cesecondo      cesecondm      cesecondz      cesecondmz
    ansiday       ansidayo       ansidaym       ansidayz       ansidaymz
    ansisecond    ansisecondo    ansisecondm    ansisecondz    ansisecondmz
    rjulianday    rjuliandayo    rjuliandaym    rjuliandayz    rjuliandaymz
    rjuliansecond rjuliansecondo rjuliansecondm rjuliansecondz rjuliansecondmz
    mjulianday    mjuliandayo    mjuliandaym    mjuliandayz    mjuliandaymz
    mjuliansecond mjuliansecondo mjuliansecondm mjuliansecondz mjuliansecondmz
    unixday       unixdayo       unixdaym       unixdayz       unixdaymz
    unixepoch     unixepocho     unixepochm     unixepochz     unixepochmz
    unixsecond    unixsecondo    unixsecondm    unixsecondz    unixsecondmz
    tmday         tmdayo         tmdaym         tmdayz         tmdaymz
    tmsecond      tmsecondo      tmsecondm      tmsecondz      tmsecondmz

The basenames above (leftmost column, and ijd) are also recognized as modifiers, for example:

    x = ceday(datestring)
    datestring = datetime(x, 'ceday')

Note that for day stamps milliseconds are always included (the option has no effect).
For second stamps, the m option causes floating point millisecond output, no m option
outputs the second stamp truncated to an integer.

The CURRENT_TIME, CURRENT_DATE, and CURRENT_TIMESTAMP will use
the formatting specified by SQLITE_DATETIME_DEFAULT.

The strftime function has the following additions:

%Z              output the timezone '' (naive), Z, (+|-)HH:MM
%z              output the offset   '' (naive), [-]mmm
%i              output ISO datestamp    yyyy-Www-d
%I              output ISO weekstamp    yyyy-Www
%q              output ISO quarterstamp yyyy-Qq
%G              output ISO year         yyyy
%V              output ISO week         ww
%u              output ISO day          d       (1=Mon...7=Sun)
%Q              output ISO quarter      q       (1...4)
%X              output ISO monthstamp   yyyy-Mmm
%x              output ISO month        mm


The equivalent format strings are:

0 - '%Y-%m-%d %H:%M:%S'
1 - '%Y-%m-%d %H:%M:%f'
2 - '%Y-%m-%d %H:%M:%S %Z'*
3 - '%Y-%m-%d %H:%M:%f %Z'*

*The strftime %Z modifier will trim all preceding trailing spaces from the output string so that
the ' %Z' will be entirely a nullity if no timezone is set.

This means that datetime(...) == strftime(fmt, ...) where the appropriate format is used.

If an undefined format specifier is used, NULL will be returned.


A collation (two) has been added that will sort datetime strings
correctly (that is, with a total ordering).  Lets assume we have a table
called test with a column dt containing datetime strings.  You may get
these in-order by specifying ORDER BY dt COLLATE DATETIMEZ.  The
datetimez collation has the following total order properties:

    first   NULLs
    second  binary sort of text that is not a parseable datetime
    third   all naive (no timezone) datetimes in iJD order
    fourth  all localized (with timezone) datetimes in iJD order

COLLATE DATETIME ignores the timezone distinction:

    first   NULLs
    second  binary sort of text that is not a parseable datetime
    third   all datetimes in iJD order


All input/output string formats will now parse a timezone specification,
even for dates.  Although dates do not usually include a timezone
indication, all the date/time/datetime/strftime use the same processing
of a full datetime internally so a timezone has been added to the date
function output.


Note that the following column definitions can be used, for example, to
ensure that a particular column always contains a valid datetime or
timestamp (using ceday as example)

For a datetime/text:

col     datetime text collate datetime          check (datetime(col) is col)
col     datetime text collate datetime not null check (datetime(col) is col)

Timestamp:

col     ceday real          check (ceday(col,'ceday') is col)
col     ceday real not null check (ceday(col,'ceday') is col)


** FOLD TIMES CAVEAT **

Note that operations which convert a naive local display time to utc MAY
have an incorrect UTC result if the naive time occurs during the "fold"
of a timezone-offset transition.  The "fold" is defined, for this
purpose only, as the local display time (sans timezone offset) which may
occur more than once.

For example, in the North America, many timezones transition each year
in the spring from standard time to daylight time.  THe spring
transition (the spring forward) results in times that do not exist.
These are assumed to belong to the previous (standard) offset, thus no
discrepance (other than display format and offset correction) occurs for
these times.

In the "fall back" period, however, there is an overlap of the local
display time (sans timezone offset).  It is impossible to know whether
the display "2022-11-06 02:30:00" means "2022-11-06 02:30:00 -06:00" or
"2022-11-16 02:30:00 -07:00" (that is, before or after the transition).

The result will assume that the time after the transition is meant, and
this is what will be returned.  This may cause problems if the "start
of" or "end of" modifiers land in a fold.  Generally speaking the
politicians and bureaucrats who make up such things try to avoid doing
them at such times as will cause general havoc, however, those people
are politicians and bureaucrats, and are well known to have a whimsical
and ignorant outlook on reality.

*** PURE FUNCTIONS AND DETERMINISM ***

Functions which access 'now' or 'txn' are impure.  All other uses are pure and deterministic
-- the computation always includes the timezone offset, datetime results are always a
deterministic point in time even when a timezone is applied, thus the point in time is always
constant even if the display representation may vary from statement to statement.

NOTE THAT USING AN SQLITE_DATETIME_DEFAULT OTHER THAN THE DEFAULT 3 WILL NULIFY THE PURITY
OF THE DATETIME FUNCTIONS IN THAT THE REPRESENTATION WILL NO LONGER MAINTAIN A CONSISTENT
POINT IN TIME.

To obtain a timestamp value that is pure, use the JulianNow() and JulianTxn() functions.
Even though these functions claim to be "constant" and "slowchange" they are statement
stable (at least) and therefore pure.  Be aware that these functions are only "pure" for
the duration of the statement and are subject to return different values for different
statement (or transactions).

