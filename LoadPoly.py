from __future__ import absolute_import, division, print_function, unicode_literals

# This loads a generic GeoJSON file into an SQLite3 database using the
# GeoPOLY extension.  Only Polygons can be processed and loaded and only
# outer polygons processed (the holes are ignored).

# Properties fields are loaded as attributes of the GeoPOLY table and are
# discovered automatically from the input GeoJSON file

# Timezone Geopoly Data can be found at:
# https://github.com/evansiroky/timezone-boundary-builder/releases

import datetime
import json
import math
import sys
import time

# This code knows how to use the sqlite3 module if apsw is not available

useapsw = None
try:
    import apsw
    useapsw = True
except:
    import sqlite3
    useapsw = False

# NumberType is the typestuples for the polygon co-ordinate data

if sys.version_info.major == 2:
    NumberType = (int, long, float)
else:
    NumberType = (int, float)
    import functools
    print = functools.partial(print, flush=True)

HMS = lambda x: (datetime.datetime.min + datetime.timedelta(seconds=x)).time().isoformat()

nauticalOverlay = False
if '--nautical' in sys.argv:
    nauticalOverlay = True
    sys.argv.remove('--nautical')

debug = False
if '--debug' in sys.argv:
    debug = True
    sys.argv.remove('--debug')

if len(sys.argv) != 4:
    print('LoadPoly.py <geojson> <dbname> <tablename> [--nautical] [--debug]')
    print('Load the <geojson> polygons to the <dbname> database geojson table <tablename>')
    print('  --nautical loads a full GMT/Nautical overlay for the tzid data')
    print('  -- debug   prints status information as the GeoJSON is traversed')
    print(' for loading the timezones use: LoadPoly combined.json tz.db tz_geopoly --nautical')
    raise SystemExit

if useapsw is None:
    print('Could not find interface to SQLite3 database')
    raise SystemExit

jsonfile = sys.argv[1]
dbname = sys.argv[2]
tablename = sys.argv[3]

st = time.time()
print(sys.version)

print()
print('Loading JSON file:', jsonfile, '...', end=' ')
bt = time.time()
f = open(jsonfile, 'rb')
data = json.load(f)
f.close()
print(HMS(time.time() - bt), 'seconds')

print()
print('Parsing Feature Properties ...', end=' ')
cols = set()
bt = time.time()
features = data['features']
for feature in features:
    for key in feature['properties'].keys():
        cols.add(key)
print(HMS(time.time() - bt), 'seconds')
print('The following Properties will be included:', ', '.join(cols))

print()
print('Preparing Database', dbname, '...')
bt = time.time()
if useapsw:
    db = apsw.Connection(dbname)
    print('SQLite3', apsw.sqlite3_sourceid())
else:
    db = sqlite3.connect(dbname, isolation_level=None)
    print('SQLite3', db.cursor().execute('select sqlite_source_id();').fetchone()[0])
db.cursor().execute('drop table if exists %s;' % tablename)
db.cursor().execute('create virtual table %s using geopoly(%s);' % (tablename, ', '.join(cols)))
db.cursor().execute('pragma timeout=900000;')
mode = db.cursor().execute('pragma journal_mode=wal;').fetchone()[0]
print('Database is in', mode, 'journal mode')
print('Database ready in', HMS(time.time() - bt), 'seconds')

print()
print('Loading Feature Polygons ...')
bt = time.time()
count = 0
igncount = 0
allpolygons = 0
dumpolygons = 0
features = data['features']
for feature in features:
    props = {}
    props.update(feature['properties'])
    if feature['geometry']['type'].lower() == 'polygon':    # Make polygon into a single entry multipolygon
        coordinates = [feature['geometry']['coordinates']]
    elif feature['geometry']['type'].lower() == 'multipolygon':
        coordinates = feature['geometry']['coordinates']
    else:
        print(feature['geometry']['type'], 'feature type has been skipped')
        continue

    # If we got here then coordinates is a list of polygons (a multipolygon)

    for polygons in coordinates:
        dumpolygons += 1
        while isinstance(polygons, list) and len(polygons) == 1:
            polygons = polygons[0]
        if isinstance(polygons, list) and isinstance(polygons[0], list) and isinstance(polygons[0][0], NumberType):
            polygons = [polygons]
        if not (isinstance(polygons, list) and isinstance(polygons[0], list) and isinstance(polygons[0][0], list) and isinstance(polygons[0][0][0], NumberType)):
            raise ValueError('Inconsistent Nesting of Polygon Lists that I cannot handle!')

        # We should now have a list of polygons of which the first should be the outside ring and the
        # subsequent polygons are holes inside the enclosed area

        for key in props.keys():
            if isinstance(props[key], (list, tuple, dict)):
                props[key] = json.dumps(props[key])

        for i, polygon in enumerate(polygons):
            allpolygons += 1
            if not (isinstance(polygon, list) and isinstance(polygon[0], list) and isinstance(polygon[0][0], NumberType)):
                raise ValueError('Somehow we got here but do not have a proper polygon')
            if not (polygon[0] == polygon[-1]):
                raise ValueError('Polygons are supposed to start and end at the same place!')
                continue

            # We now have a valid polygon so convert to a blob and find its area

            props['_shape'] = db.cursor().execute('select geopoly_blob(?);', (json.dumps(polygon),)).fetchone()[0]
            area = db.cursor().execute('select geopoly_area(:_shape);', props).fetchone()[0]

            if debug:
                print('Polygon', allpolygons, '[', dumpolygons, '/', i, ']', area, '*****' if (area<=0 and i==0) or (area>=0 and i>0) else '')

            if i > 0:
                igncount += 1
                continue
            if i == 0 and area < 0:
                props['_shape'] = db.cursor().execute('select geopoly_ccw(:_shape);', props).fetchone()[0]
            sql = 'insert into %s (%s) values (:%s);' % (tablename, ', '.join(props.keys()),  ', :'.join(props.keys()))
            db.cursor().execute('begin immediate transaction;')
            db.cursor().execute(sql, props)
            db.cursor().execute('commit;')
            count += 1

if nauticalOverlay and 'tzid' in cols:
    print('Loading Nautical Overlay')
    for lon in range(-180, 225, 15):
        hours = int(lon / -15)
        minlon = lon - 7.5
        maxlon = lon + 7.5
        # Adjust the boundaries so they do not overlap.
        # THIS IS AN SQLITE3 GEOPOLY IMPLEMENTATION DETAIL WHERE COORDIATES ARE FLOAT32
        eps = math.ldexp(1, math.frexp(maxlon)[1] - 24) # Compute the IEEE Float32 epsilon
        maxlon += (-eps) if maxlon > 0 else (eps)       # Adjust the boundry by one epsilon
        props = {}
        props['tzid'] = 'Etc/GMT%+d' % hours if hours else 'Etc/GMT'
        props['_shape'] = db.cursor().execute('select geopoly_blob(?);', (json.dumps([[minlon, -91], [maxlon, -91], [maxlon, 91], [minlon, 91], [minlon,-91]]),)).fetchone()[0]
        sql = 'insert into %s (%s) values (:%s);' % (tablename, ', '.join(props.keys()),  ', :'.join(props.keys()))
        db.cursor().execute('begin immediate transaction;')
        db.cursor().execute(sql, props)
        db.cursor().execute('commit;')
        count += 1
        allpolygons += 1
        dumpolygons += 1

print(count, 'Polygons loaded in', HMS(time.time() - bt), 'seconds')

print()
print('Analyze ...', end=' ')
db.cursor().execute('analyze;')

print('Vacuum ...', end=' ')
db.cursor().execute('vacuum;')

print('Finished')
db.close()

print()
print(jsonfile, 'parsed and loaded into database', dbname, 'table', tablename, 'in', HMS(time.time() - st), 'seconds')
print(allpolygons, 'polygons were examined of which', count, 'were loaded and', igncount, 'were ignored')

