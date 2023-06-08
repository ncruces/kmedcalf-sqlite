
# Import the standard library modules that we require

import calendar
import datetime
import os
import sys

# Set the following to determine whether to use unicode or nocase

use_unicode = True

collation_unicode = 'icu_und_aici'
collation_ascii = 'nocase'

collation = collation_unicode if use_unicode else collation_ascii


# Create strip_accents function or a placebo that does nothing

if use_unicode:
    def strip_accents(s):
        return s
else:
    try:
        import unicodedata
        def strip_accents(s):
            from unicodedata import normalize, category
            return ''.join(c for c in normalize('NFD', s) if category(c) != 'Mn')
    except:
        def strip_accents(s):
            return s

# ISO 6709 LatLong Decoder Ring

def latlong(s):
    def iso6709(s):
        l = len(s)
        return   (((int(s[1:4])       if l >= 4 else 0) + \
                  ((int(s[4:6]) / 60) if l >= 6 else 0) + \
                  ((int(s[6:8]) / 3600) if l >= 8 else 0) + \
                  ((int(s[8:10]) / 216000) if l >= 10 else 0) + \
                  ((int(s[10:12]) / 12960000) if l >= 12 else 0)) * \
                  (-1 if s[0]=='-' else 1)) \
               if l % 2 == 0 else \
                 (((int(s[1:3])       if l >= 3 else 0) + \
                  ((int(s[3:5]) / 60) if l >= 5 else 0) + \
                  ((int(s[5:7]) / 3600) if l >= 7 else 0) + \
                  ((int(s[7:9]) / 216000) if l >= 9 else 0) * \
                  ((int(s[9:11]) / 12960000) if l >= 11 else 0)) * \
                  (-1 if s[0]=='-' else 1))
    for p in range(1, len(s)):
        if s[p] in '+-':
            return (iso6709(s[:p]), iso6709(s[p:]))
    return (None, None)

# Round to Minutes (Offset / Transitions)

def round60(x):
    return int(round(float(x) / 60.0) * 60.0)

# Process Command Line arguments

limityears = 9999
limitcount = 5
Extend = True
if '--limityears' in sys.argv:
    pos = list(sys.argv).index('--limityears')
    limityears = int(sys.argv[pos + 1])
    sys.argv.remove(sys.argv[pos])
    sys.argv.remove(sys.argv[pos])
if '--limitcount' in sys.argv:
    pos = list(sys.argv).index('--limitcount')
    limitcount = int(sys.argv[pos + 1])
    sys.argv.remove(sys.argv[pos])
    sys.argv.remove(sys.argv[pos])
if '--extend' in sys.argv:
    Extend = True
    sys.argv.remove('--extend')
if '--noextend' in sys.argv:
    Extend = False
    sys.argv.remove('--noextend')

# Resolve whether to use pytz, dateutil, tzdata, or zoneinfo (zoneinfo preferred, then tzdata, then pytz, then dateutil)
# Command Line flag [--zoneinfo|--tzdata|--pytz|--dateutil] allows forcing a specific source if available

usepytz = False
usedateutil = False
usetzdata = False
usezoneinfo = False
tzimpl = None

if '--zoneinfo' in sys.argv:
    sys.argv.remove('--zoneinfo')
    try:
        import zoneinfo
        import zoneinfo._zoneinfo
        from zoneinfo._tzpath import find_tzfile
        usezoneinfo = True
        tzimpl = 'zoneinfo'
    except:
        pass
elif '--tzdata' in sys.argv:
    sys.argv.remove('--tzdata')
    try:
        import tzdata
        import zoneinfo
        import zoneinfo._zoneinfo
        from zoneinfo._tzpath import find_tzfile
        usetzdata = True
        tzimpl = 'tzdata'
    except:
        pass
elif '--pytz' in sys.argv:
    sys.argv.remove('--pytz')
    try:
        import pytz
        usepytz = True
        tzimpl = 'pytz'
    except:
        pass
elif '--dateutil' in sys.argv:
    sys.argv.remove('--dateutil')
    try:
        import dateutil.tz
        import dateutil.zoneinfo
        usedateutil = True
        tzimpl = 'dateutil'
    except:
        pass

if tzimpl is None:
    try:
        import zoneinfo
        import zoneinfo._zoneinfo
        from zoneinfo._tzpath import find_tzfile
        if zoneinfo.TZPATH:
            usezoneinfo = True
            tzimpl = 'zoneinfo'
    except:
        pass

if tzimpl is None:
    try:
        import tzdata
        import zoneinfo
        import zoneinfo._zoneinfo
        from zoneinfo._tzpath import find_tzfile
        usetzdata = True
        tzimpl = 'tzdata'
    except:
        pass

if tzimpl is None:
    try:
        import pytz
        usepytz = True
        tzimpl = 'pytz'
    except:
        pass

if tzimpl is None:
    try:
        import dateutil.tz
        import dateutil.zoneinfo
        usedateutil = True
        tzimpl = 'dateutil'
    except:
        pass

if not (usepytz or usedateutil or usetzdata or usezoneinfo):
    print('No IANA Timezone source found [--zoneinfo|--tzdata|--pytz|--dateutil]')
    raise SystemExit

# Resolve whether to use apsw or sqlite3 database connector

useapsw = False
usesqlite3 = False
dbimpl = None

if '--apsw' in sys.argv:
    sys.argv.remove('--apsw')
    try:
        import apsw
        useapsw = True
        dbimpl = 'apsw'
    except:
        pass
elif '--sqlite3' in sys.argv:
    sys.argv.remove('--sqlite3')
    try:
        import sqlite3
        usesqlite3 = True
        dbimpl = 'sqlite3'
    except:
        pass

if dbimpl is None:
    try:
        import apsw
        useapsw = True
        dbimpl = 'apsw'
    except:
        pass

if dbimpl is None:
    try:
        import sqlite3
        usesqlite3 = True
        dbimpl = 'sqlite3'
    except:
        pass

if not (useapsw or usesqlite3):
    print('No Database Interface (apsw/sqlite3) found [--sqlite3|--apsw]')
    raise SystemExit

print(sys.version)
print('Using', tzimpl, 'IANA Timzone data source')
print('Using', dbimpl, 'database connection library')

createdb = f"""
drop view  if exists TZ_AllZones;
drop view  if exists TZ_Lookup;
drop table if exists TZ_ZoneData;
drop table if exists TZ_CountriesZones;
drop table if exists TZ_Centroids;
drop table if exists TZ_Zones;
drop table if exists TZ_Countries;
drop table if exists TZ_Version;

pragma application_id=1415201858;

create table TZ_Version
(
    Version     text collate {collation},
    PIPVersion  text collate {collation},
    check (rowid == 1)
);

create table TZ_Countries
(
    Code            text not null collate {collation} primary key,
    Country         text not null collate {collation} unique
) WITHOUT ROWID;

create table TZ_Zones
(
    ID              INTEGER PRIMARY KEY,
    Country_Code    text collate {collation} references TZ_Countries(Code),
    Area            text collate {collation},
    Location        text collate {collation},
    City            text collate {collation},
    Zone            text not null collate {collation} unique
);

create table TZ_CountriesZones
(
    Country_Code    text not null collate {collation} references TZ_Countries(Code),
    Zone_ID         integer not null references TZ_Zones(ID),
    primary key     (Country_Code, Zone_ID),
    unique          (Zone_ID, Country_Code)
) WITHOUT ROWID;

create table TZ_Centroids
(
    Zone_ID         integer primary key references TZ_Zones(ID),
    lat             real,
    lon             real
) WITHOUT ROWID;

create table TZ_ZoneData
(
    Zone_ID         integer not null references TZ_Zones(ID),
    StartTime       integer not null,
    Offset          integer not null,
    isDST           integer not null,
    Abbreviation    text collate {collation} not null,
    primary key     (Zone_ID, StartTime)
) WITHOUT ROWID;

create        index TZ_ZonesCountry  on TZ_Zones (Country_Code);
create        index TZ_ZonesArea     on TZ_Zones (Area);
create        index TZ_ZonesLocation on TZ_Zones (Location);
create        index TZ_ZonesCity     on TZ_Zones (City);
create unique index TZ_ZoneOffset on TZ_ZoneData (Zone_ID, StartTime + Offset, StartTime, Offset);

create view TZ_AllZones
as
    select TZ_Zones.ID,
           TZ_Zones.Country_Code as Primary_Country_Code,
           TZ_Countries1.Country as Primary_Country,
           TZ_CountriesZones.Country_Code,
           TZ_Countries2.Country,
           TZ_Zones.Area,
           TZ_Zones.Location,
           TZ_Zones.City,
           TZ_Centroids.lat,
           TZ_Centroids.lon,
           TZ_Zones.Zone
      from TZ_Zones
 left join TZ_Centroids
        on TZ_Centroids.Zone_ID == TZ_Zones.ID
 left join TZ_CountriesZones
        on TZ_CountriesZones.Zone_ID == TZ_Zones.ID
 left join TZ_Countries as TZ_Countries1
        on TZ_Countries1.Code == TZ_Zones.Country_Code
 left join TZ_Countries as TZ_Countries2
        on TZ_Countries2.Code == TZ_CountriesZones.Country_Code;

create view TZ_Lookup
as
    select TZ_Zones.ID,
           TZ_Countries.Code as Country_Code,
           TZ_Countries.Country,
           TZ_Zones.Area,
           TZ_Zones.Location,
           TZ_Zones.City,
           TZ_Zones.Zone,
           TZ_ZoneData.Abbreviation,
           TZ_ZoneData.StartTime,
           TZ_ZoneData.Offset,
           TZ_ZoneData.isDST
      from TZ_Zones
      join TZ_ZoneData
        on TZ_Zones.ID == TZ_ZoneData.Zone_ID
 left join TZ_Countries
        on TZ_Countries.Code == TZ_Zones.Country_Code;
"""

if len(sys.argv) != 2:
    print("tzExport <dbname>")
    print()
    print("Export the IANA Timezone data into the named SQLite3 database")
    raise SystemExit

if useapsw:
    db = apsw.Connection(sys.argv[1])
    db.cursor().execute('BEGIN IMMEDIATE;')
    db.cursor().execute(createdb)
elif usesqlite3:
    db = sqlite3.connect(sys.argv[1], isolation_level=None)
    db.executescript(createdb)
    try:
        db.executescript('BEGIN IMMEDIATE;')
    except:
        pass

print('Populating TZ_Version table ...')
if usepytz:
    db.cursor().execute('insert into TZ_Version values (?,?);', (pytz.OLSON_VERSION, pytz.VERSION))
elif usedateutil:
    db.cursor().execute('insert into TZ_Version values (?,?);', (dateutil.zoneinfo.gettz_db_metadata()['tzversion'], dateutil._version.version))
elif usetzdata:
    db.cursor().execute('insert into TZ_Version values (?,?)', (tzdata.IANA_VERSION, tzdata.__version__))
elif usezoneinfo:
    IANA_VERSION = None
    PIP_VERSION = None
    try:
        f = open(find_tzfile('tzdata.zi'), 'r')
        for line in f:
            if line.lower().startswith('# version'):
                IANA_VERSION = line.strip().split()[-1]
                break
    except:
        pass
    db.cursor().execute('insert into TZ_Version values (?,?)', (IANA_VERSION or 'Unknown', 'zoneinfo'))

IANA_VERSION, PIP_VERSION = db.cursor().execute('select * from TZ_Version').fetchone()

print(f'IANA Timezone source {tzimpl} is version {IANA_VERSION}, pip version is {PIP_VERSION}')

zonecount = 0
zoneloaded = 0

if usepytz:

    print('Populating TZ_Countries table ...')
    for cc, name in pytz._CountryNameDict().items():
        db.cursor().execute('insert or ignore into TZ_Countries values (?,?);', (cc, name))

    print('Building internal zone to country dictionary')
    cc_tzname = {}
    for cc, tzlist in pytz.country_timezones.items():
        for tzname in tzlist:
            cc_tzname[tzname] = cc

    print('Populating TZ_Zones and TZ_ZoneData tables ...')
    for id, zone in enumerate(pytz.all_timezones):
        id += 1
        zonecount += 1
        tz = pytz.timezone(zone)
        cc = cc_tzname.get(zone, None)
        area = None
        location = None
        city = None
        if '/' in zone:
            area, location = zone.split('/', 1)
        if cc is not None:
            city = location
            if '/' in city:
                junk, city = city.split('/', 1)
        if (hasattr(tz, '_utc_transition_times') and hasattr(tz, '_transition_info')):
            db.cursor().execute('insert into TZ_Zones values (?,?,?,?,?,?);', (id, cc, area, location, city, zone))
            for i, tt in enumerate(tz._utc_transition_times):
                offset, isdst, shortname = tz._transition_info[i]
                offset = int(offset.total_seconds())
                isdst = 1 if isdst.total_seconds() else 0
                start = calendar.timegm(tt.timetuple())
                if start <= -62135596800:
                    start =  -210866760000
                db.cursor().execute('insert or ignore into TZ_ZoneData values (?,?,?,?,?);', (id, round60(start), round60(offset), isdst, shortname))
            zoneloaded += 1
        else:
            db.cursor().execute('insert into TZ_Zones values (?,?,?,?,?,?);', (id, cc, area, location, city, zone))
            offset = int(tz._utcoffset.total_seconds())
            isdst = 0
            start = -210866760000
            shortname = tz.tzname(0)
            db.cursor().execute('insert or ignore into TZ_ZoneData values (?,?,?,?,?);', (id, round60(start), round60(offset), isdst, shortname))
            zoneloaded += 1

elif usedateutil:

    print('Populating TZ_Zones and TZ_ZoneData tables ...')
    for id, zone in enumerate(dateutil.zoneinfo.ZoneInfoFile(dateutil.zoneinfo.getzoneinfofile_stream()).zones.keys()):
        id += 1
        zonecount += 1
        tz = dateutil.tz.gettz(zone)
        cc = None
        area = None
        location = None
        city = None
        if '/' in zone:
            area, location = zone.split('/', 1)
        if cc is not None:
            city = location
            if '/' in city:
                junk, city = city.split('/', 1)
        if (hasattr(tz, '_trans_list_utc') and hasattr(tz, '_trans_idx')) and tz._trans_list_utc:
            db.cursor().execute('insert into TZ_Zones values (?,?,?,?,?,?);', (id, cc, area, location, city, zone))
            for i, start in enumerate(tz._trans_list_utc):
                tt = tz._trans_idx[i]
                offset = tt.offset
                isdst = 1 if tt.dstoffset.total_seconds() else 0
                shortname = tt.abbr
                if start <= -2147483648:
                    start =  -210866760000
                db.cursor().execute('insert or ignore into TZ_ZoneData values (?,?,?,?,?);', (id, round60(start), round60(offset), isdst, shortname))
            zoneloaded += 1
        else:
            db.cursor().execute('insert into TZ_Zones values (?,?,?,?,?,?);', (id, cc, area, location, city, zone))
            offset = tz.utcoffset(0).total_seconds()
            isdst = 0
            start = -210866760000
            shortname = tz.tzname(0)
            db.cursor().execute('insert or ignore into TZ_ZoneData values (?,?,?,?,?);', (id, round60(start), round60(offset), isdst, shortname))
            zoneloaded += 1

elif usetzdata or usezoneinfo:

    # Load the iso3166.tab and create the TZ_Countries table
    f = None
    if usetzdata:
        try:
            f = zoneinfo._common.load_tzdata('iso3166.tab')
        except:
            pass
    if not f:
        try:
            f = open(find_tzfile('iso3166.tab'), 'rb')
        except:
            pass
    if f:
        print('Loading iso3166.tab into TZ_Countries ...')
        for line in f:
            line = line.decode().strip()
            if (not line) or line.startswith('#'):
                continue
            key, value = line.split('\t')
            key = strip_accents(key).upper().strip()
            value = strip_accents(value).strip()
            db.cursor().execute('insert or ignore into TZ_Countries values (?,?)', (key, value))
        f.close()
    f = None

    # Loop through all the available zones to populate TZ_Zones and TZ_ZoneData tables
    epochdate = datetime.datetime(1970, 1, 1, tzinfo=zoneinfo.ZoneInfo('UTC'))
    print(f'Populating TZ_Zones and TZ_ZoneData tables ...')
    for id, zone in enumerate(sorted(zoneinfo.available_timezones())):
        id += 1
        zonecount += 1
        file_obj = None
        trans_idx = None
        if usetzdata:
            try:
                file_obj = zoneinfo._common.load_tzdata(zone)
            except:
                pass
        if not file_obj:
            try:
                file_obj = open(find_tzfile(zone),'rb')
            except:
                pass
        if not file_obj:
            continue
        try:
            trans_idx, trans_list_utc, utcoff, isdst, abbr, tz_str = zoneinfo._common.load_data(file_obj)
            file_obj.close()
            file_obj = None
        except:
            pass
        if trans_idx is None:
            continue

        cc = None
        area = None
        location = None
        city = None
        if '/' in zone:
            area, location = zone.split('/', 1)

        # create TZ_Zones and TZ_ZoneData LMT at julian epoch start
        db.cursor().execute('insert into TZ_Zones values (?,?,?,?,?,?);', (id, cc, area, location, city, zone))
        db.cursor().execute('insert or ignore into TZ_ZoneData values (?,?,?,?,?);', (id, -210866760000, round60(utcoff[0]), isdst[0], abbr[0]))

        # if there are transitions write them to TZ_ZoneData
        if len(trans_idx) > 0:
            for i in range(len(trans_idx)):
                j = trans_idx[i]
                try:
                    db.cursor().execute('insert or ignore into TZ_ZoneData(Zone_ID, StartTime, Offset, isDst, Abbreviation) values (?,?,?,?,?)',
                                        (id, round60(trans_list_utc[i]), round60(utcoff[j]), isdst[j], abbr[j]))
                except:
                    print('shit data ignored', zone, i, j)
                    pass

        if Extend:
            # try and add transitions into future using tz_str
            tz_after = None
            try:
                tz_after = zoneinfo._zoneinfo._parse_tz_str(tz_str.decode())
            except:
                pass
            if isinstance(tz_after, zoneinfo._zoneinfo._TZStr):
                maxtrans = max(trans_list_utc)
                stdoff = tz_after.std.utcoff.total_seconds()
                dstoff = tz_after.dst.utcoff.total_seconds()
                tzlocal = zoneinfo.ZoneInfo(zone)
                tzutc   = zoneinfo.ZoneInfo('utc')
                lastdst = False
                year = datetime.datetime.utcfromtimestamp(maxtrans).year - 1
                endyear = min(datetime.datetime.utcnow().year + limitcount, limityears) + 1
                while year <= endyear:
                    transition = tz_after.transitions(year)
                    dt = []
                    if len(transition) == 2:
                        dt.append((epochdate + datetime.timedelta(seconds=transition[0]-stdoff)).astimezone(tzlocal))
                        dt.append((epochdate + datetime.timedelta(seconds=transition[1]-dstoff)).astimezone(tzlocal))
                    else:
                        for transition in tz_after.transitions(year):
                            dt1 = (epochdate + datetime.timedelta(seconds=transition-stdoff)).astimezone(tzlocal)
                            dt2 = (epochdate + datetime.timedelta(seconds=transition-dstoff)).astimezone(tzlocal)
                            dt1dst = dt1.dst().total_seconds()
                            dt2dst = dt2.dst().total_seconds()
                            dtf = (dt1 if dt1.fold else (dt2 if dt2.fold else None))
                            if dtf is None:
                                if dt1dst and dt2dst:
                                    dtf = dt1 if dt1.timestamp() < dt2.timestamp() else dt2
                                else:
                                    dtf = (dt1 if dt1dst else (dt2 if dt2dst else None))
                            if dtf is None:
                                continue
                            dt.append(dtf)
                    dt.sort(key=datetime.datetime.timestamp)
                    for dtf in dt:
                        transition = int(dtf.timestamp())
                        if transition <= maxtrans:
                            continue
                        offset = tzlocal.utcoffset(dtf).total_seconds()
                        dst = tzlocal.dst(dtf).total_seconds() != 0
                        abbrev = tzlocal.tzname(dtf)
                        if year < endyear or (lastdst and not dst):
                            db.cursor().execute('insert or ignore into TZ_ZoneData(Zone_ID, StartTime, Offset, isdst, Abbreviation) values (?,?,?,?,?)',
                                                                                  (id, round60(transition), round60(offset), dst, abbrev))
                            lastdst = dst
                    year += 1
                transition = datetime.datetime(year, 1, 1, tzinfo=datetime.timezone(datetime.timedelta(seconds=utcoff[0]))).timestamp()
                if transition > maxtrans:
                    db.cursor().execute('insert or ignore into TZ_ZoneData(Zone_ID, StartTime, Offset, isdst, Abbreviation) values (?,?,?,?,?)',
                                                                          (id, round60(transition), round60(utcoff[0]), isdst[0], abbr[0]))
        zoneloaded += 1


    for zonetab in ['zone.tab', 'zone1970.tab']:
        f = None
        if usetzdata:
            try:
                f = zoneinfo._common.load_tzdata(zonetab)
            except:
                pass
        if not f:
            try:
                f = open(find_tzfile(zonetab), 'rb')
            except:
                pass
        if f:
            print(f'Loading {zonetab} building county:zone mapping')
            for line in f:
                line = line.decode().strip()
                if (not line) or line.startswith('#'):
                    continue
                ccs, ll, zone = line.split('\t')[:3]
                lat, lon = latlong(ll)
                if ',' in ccs:
                    cc = list(map(str.strip, ccs.split(',')))
                else:
                    cc = [ccs]
                db.cursor().execute('update TZ_Zones set Country_Code=? where Zone=?', (cc[0], zone))
                zone_id = db.cursor().execute('select id from TZ_Zones where zone=?', (zone,)).fetchone()[0]
                if lat is not None and lon is not None:
                    db.cursor().execute('insert or replace into TZ_Centroids values (?,?,?)', (zone_id, lat, lon))
                for icc in cc:
                    db.cursor().execute('insert or ignore into TZ_CountriesZones values (?,?)', (icc, zone_id))
    db.cursor().execute('update TZ_Zones set Country_Code=Code from TZ_Countries where country=area and country_code is null')
    db.cursor().execute('update TZ_Zones set Country_Code=Code from TZ_Countries where country=location and country_code is null')
    db.cursor().execute('update TZ_Zones set Country_Code=Code from TZ_Countries where country=city and country_code is null')
    db.cursor().execute('update TZ_Zones set Country_Code=?1 where Country_Code is null and area=?1', ('US',))
    db.cursor().execute('insert or ignore into TZ_CountriesZones select Country_Code, ID from TZ_Zones where Country_Code is not null')

    f = None
    if usetzdata:
        try:
            f = zoneinfo._common.load_tzdata('tzdata.zi')
        except:
            pass
    if not f:
        try:
            f = open(find_tzfile('tzdata.zi'), 'rb')
        except:
            pass
    if f:
        for line in f:
            line = line.decode().strip()
            if not line.startswith('L '):
                continue
            zones = line[2:].split()
            fromzone = zones[0]
            tozones = zones[1:]
            lat = lon = None
            try:
                lat, lon = db.cursor().execute('select lat, lon from TZ_Centroids where Zone_ID = (select ID from TZ_Zones where Zone=?)', (fromzone,)).fetchone()
            except:
                pass
            if lat is not None and lon is not None:
                for zone in tozones:
                    db.cursor().execute('insert or ignore into TZ_Centroids select ID, ?, ? from TZ_Zones where Zone=?', (lat, lon, zone))

    db.cursor().execute(
"""
update TZ_Zones
    set location = iif(instr(location, '/') > 0, substr(location, 1, instr(location, '/') - 1), null),
            city = iif(instr(location, '/') > 0, substr(location, instr(location, '/') + 1), location)
  where area is not null
    and area not in ('Canada', 'Etc', 'US')
    and city is null
    and location is not null
"""
)
    db.cursor().execute(
"""
delete from TZ_ZoneData
where (Zone_ID, StartTime) in (
                                select Zone_ID, StartTime
                                  from (
     select zone_id, Abbreviation, StartTime, Offset, isDST,
            lag(Abbreviation) over (partition by zone_id order by StartTime) as prevAbbrev,
            lag(StartTime) over (partition by zone_id order by StartTime) as prevStartTime,
            lag(Offset) over (partition by zone_id order by StartTime) as prevOffset,
            lag(isDST) over (partition by zone_id order by StartTime) as previsDST
       from tz_zonedata
                                       )
                                 where Offset == prevOffset
                                   and isDST == previsDST
                                   and Abbreviation == prevAbbrev
                              )
"""
)

print('Loaded', zoneloaded, 'of', zonecount, 'found')

print('Commit')
db.cursor().execute('COMMIT;')

print('Vacuum')
db.cursor().execute('VACUUM;')

#print('Analyze')
#db.cursor().execute('ANALYZE;')

print('Finished')
db.close()

