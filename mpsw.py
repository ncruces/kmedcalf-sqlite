"""
mpsw.py My Python SQLite3 Wrapper

        This package wraps the APSW package implementing some
        high level interfaces in a more pythonic manner.

        The Connection and Cursor classes subclass the APSW
        classes of the same name.

        Additions include:
            Data Converter/Adapter (ala pysqlite2 more or less)
            Provide "Pythonic" descriptors and utilities
            Wrap output rows in Row or MiniRow objects

        Default Data Converter/Adapters included:
            datetime [datetime]
            ipaddress and netaddr [ipaddress] [netaddr]
            uuid [guid] [uuid]
            boolean [bool] [boolean]
            complex [complex]

"""

import apsw
import apsw.ext

# Exportable Symbols defined in this file
__all__ = [
           'ConnectionPool',    'Connection',       'Cursor',
           'RowFactory',        'Row',              'MiniRow',
           'CArray',
           'getLogging',        'setLogging',
           'getTracing',        'setTracing',
           'setLogFile',
           'DataAdapter',       'DataConverter',
           'mpswversion',
           'DefaultVFS',
          ]

# Attributes from apsw to define in this module and publish
apswfunc = (
            'allow_missing_dict_bindings',
            'apswversion',
            'Backup',
            'Blob',
            'compile_options',
            'complete',
            'config',
            'connection_hooks',
            'enablesharedcache',
            'exceptionfor',
            'ExecTraceAbort',
            'fork_checker',
            'format_sql_value',
            'hard_heap_limit',
            'IndexInfo',
            'initialize',
            'keywords',
            'log',
            'memoryhighwater',
            'memoryused',
            'no_change',
            'randomness',
            'releasememory',
            'set_default_vfs',
            'shutdown',
            'softheaplimit',
            'sqlite3_sourceid',
            'sqlitelibversion',
            'status',
            'strglob',
            'stricmp',
            'strlike',
            'strnicmp',
            'unregister_vfs',
            'URIFilename',
            'using_amalgamation',
            'VFS',
            'VFSFile',
            'vfsnames',
            'VTCursor',
            'VTModule',
            'VTTable',
            'zeroblob',
           )

apswext =  (
            'make_virtual_module',
            'VTColumnAccess',
           )

# Publish attributes for Exceptions, Constants, and mappings from apsw to this module
for name in dir(apsw):
    if name.endswith('Error') or name.startswith('SQLITE_') or name.startswith('mapping_'):
        try:
            globals()[name] = getattr(apsw, name)
            __all__.append(name)
        except:
            pass

# Publish attributes from apsw to define in this module
for name in apswfunc:
    try:
        globals()[name] = getattr(apsw, name)
        __all__.append(name)
    except:
        pass

# Publish attributes from apsw.ext to define in this module
for name in apswext:
    try:
        globals()[name] = getattr(apsw.ext, name)
        __all__.append(name)
    except:
        pass

__all__.sort()
__all__ = tuple(__all__)

del name
del apswfunc
del apswext


def mpswversion():
    return '23.06.09.01'


# Override dir function to only show unpolluted namespace

def __dir__():
    return __all__


# Dummy Function to raise NotImplementedError for stubbing out class elements

def NotImplementedStub(*arg, **kwarg):
    raise NotImplementedError


# ---------- MiniRow ----------

class MiniRow(object):
    """Lightweight Row Object that permits access by either attribute or position
       and stores nothing other than the attribute name and the value

       row['attrname']      # attribute name as key
       row('attrname')
       row.attrname
       row[0]               # column number as key
       row(0)
       row()                # entire row tuple
    """
    __slots__ = ['_MiniRow__colnames', '_MiniRow__values']

    def __init__(self, values, colnames):
        self.__colnames = colnames
        self.__values = values

    def __dir__(self):
        return self.__colnames

    def __getitem__(self, item):
        if isinstance(item, str):
            try:
                return self.__values[self.__colnames.index(item)]
            except:
                pass
            raise KeyError(f'{item}: Field not in Row')
        elif isinstance(item, int):
            try:
                return self.__values[item]
            except:
                pass
            raise IndexError(f'Column {item} not in Row')
        else:
            raise ValueError('Field selector must be Integer or String')

    def __getattr__(self, item):
        try:
            return self.__values[self.__colnames.index(item)]
        except:
            pass
        raise AttributeError(f'{item}: Field not in Row')

    def __repr__(self):
        return 'MiniRow(' + ', '.join('%s=%r' % entry for entry in zip(self.__colnames, self.__values)) + ')'

    def __call__(self, index=None):
        if index is None:
            return self.__values
        else:
            return self.__getitem__(index)

    def __iter__(self):
        return iter(self.__values)

    def __len__(self):
        return len(self.__values)

    def __eq__(self, other):
        if not isinstance(other, MiniRow):
            return NotImplemented
        if len(self._MiniRow__colnames) == len(other._MiniRow__colnames) and \
           self._MiniRow__colnames == other._MiniRow__colnames and \
           self._MiniRow__values == other._MiniRow__values:
            return True
        return False

    def __ne__(self, other):
        return not self == other

    def __getstate__(self):
        return (self.__colnames, self.__values)

    def __setstate__(self, state):
        self.__colnames, self.__values = state


# ---------- Row ----------

class Row(object):
    """Row Object

       Underlying cursor column names are attributes (all lower case)
       Special Attributes:  Type       stores textual representation of Python type (NoneType, int, float, str, bytes, &c)
                            Datatype   stores datatype (SQL first declared type word) of column data
                            Origin     stores underlying origin tuple (schema, table, column)
                            Alias      stores the raw column alias
                            Description list of tuple(attrname, datatype, origin, alias)
       row.colname
       row.Type.colname
       row.Datatype.colname
       row.Origin.colname
       row.Alias.colname
       row.Description
       row()
       row(0)
       row[0]
       row[colname]
    """
    __slots__ = ['_Row__values', '_Row__colnames', '_Row__datatypes', '_Row__aliases', '_Row__origins',
                 '_Type', '_Datatype', '_Origin', '_Alias']

    def __init__(self, values, colnames, datatypes, aliases, origins):
        self.__values    = values
        self.__colnames  = colnames
        self.__datatypes = datatypes
        self.__aliases   = aliases
        self.__origins   = origins
        self._Type       = None
        self._Datatype   = None
        self._Origin     = None
        self._Alias      = None

    def __dir__(self):
        attrs = ['Type', 'Datatype', 'Origin', 'Alias', 'Description']
        attrs.extend(list(self.__colnames))
        return attrs

    def __getitem__(self, item):
        if isinstance(item, str):
            try:
                return self.__values[self.__colnames.index(item)]
            except:
                pass
            raise KeyError(f'{item}: Field not in Row')
        elif isinstance(item, int):
            try:
                return self.__values[item]
            except:
                pass
            raise IndexError(f'Column {item} not in Row')
        else:
            raise ValueError('Field selector must be Index or String')

    def __getattr__(self, item):
        try:
            return self.__values[self.__colnames.index(item)]
        except:
            pass
        raise AttributeError(f'{item}: Field not in Row')

    def __repr__(self):
        return 'Row(' + ', '.join('%s=%r' % entry for entry in zip(self.__colnames, self.__values)) + ')'

    def __call__(self, index=None):
        if index is None:
            return self.__values
        else:
            return self.__getitem__(index)

    def __iter__(self):
        return iter(self.__values)

    def __len__(self):
        return len(self.__values)

    def __eq__(self, other):
        if not isinstance(other, Row):
            return NotImplemented
        if len(self._Row__colnames) == len(other._Row__colnames) and \
           self._Row__colnames == other._row__colnames and \
           self._Row__values == other._row__values and \
           self._Row__datatypes == other._row__datatypes and \
           self._Row__origins == other._row__origins and \
           self._Row__aliases == other._row__aliases:
            return True
        return False

    def __ne__(self, other):
        return not self == other

    def __getstate__(self):
        return (self.__colnames, self.__values, self.__datatypes, self.__aliases, self.__origins)

    def __setstate__(self, state):
        colnames, values, datatypes, aliases, origins = state
        self.__colnames  = colnames
        self.__values    = values
        self.__datatypes = datatypes
        self.__origins   = origins
        self.__aliases   = aliases
        self._Type       = None
        self._Datatype   = None
        self._Origin     = None
        self._Alias      = None

    @property
    def Type(self):
        if self._Type is None:
            types = tuple(b[b.find("'")+1:b.rfind("'")] for b in map(repr, map(type, self.__values)))
            self._Type = MiniRow(types, self.__colnames)
        return self._Type

    @property
    def Datatype(self):
        if self._Datatype is None:
            self._Datatype   = MiniRow(self.__datatypes, self.__colnames)
        return self._Datatype

    @property
    def Origin(self):
        if self._Origin is None:
            self._Origin     = MiniRow(self.__origins, self.__colnames)
        return self._Origin

    @property
    def Alias(self):
        if self._Alias is None:
            self._Alias      = MiniRow(self.__aliases, self.__colnames)
        return self._Alias

    @property
    def Description(self):
        """Return the Cursor description tuple, one entry per column, each entry
        containing 4 attributes of the column (accessor attribute name, data type,
        underlying source, original column name)"""
        return tuple(zip(self.__colnames, self.__datatypes, self.__origins, self.__aliases))


# ---------- Cursor Execution and Row Tracers ----------

def __exectrace__(cursor, sql, bindings):
    """Execution Tracer -- runs automatically

       Fires the exectrace if set
       Collects metadata if scanrows or outputrows
       Turns on rowtracing if required
    """
    if not isinstance(cursor, Cursor):
        return True

    global _Tracing_
    if _Tracing_:
        from apsw import log
        log(0x7fffffff, f'EXEC SQL: {sql}')
        if bindings:
            log(0x7fffffff, f'BINDINGS: {repr(bindings)}')
    exectrace = cursor._Cursor__exectrace
    if exectrace and (not exectrace(cursor, sql, bindings)):
        return False
    cursor._Cursor__makerow = cursor._Cursor__convert = None
    if (cursor._Cursor__outputrows and cursor._Cursor__rowfactory) or cursor._Cursor__scanrows:
        orgnames = list()
        colnames = list()
        coltypes = list()
        colorig  = list()
        for i, desc in enumerate(cursor.description):
            dtyp = None
            orgnames.append(desc[0])
            name = desc[0]
            # column names of the form "name [dtyp]" are parsed into the name and dtyp
            if '[' in name:
                name, dtyp = name.split('[', 1)
                if ']' in dtyp:
                    dtyp, junk = dtyp.split(']', 1)
                else:
                    name = desc[0]
                    dtyp = None
            # column names must be valid object attrbute names
            # replace ( , . with _
            # retain only ascii characters _ a-z 0-9
            # strip leading and trailing _
            # use _{colnum} if no column name
            # preface numeric column name with _
            # if not unique append _{j} where j is 1 or bigger as requried to render name unique
            name = name.strip('. ')
            name = name.replace('(', '_').replace(',', '_').replace('.', '_').lower()
            name = ''.join(c for c in name if c in 'abcdefghijklmnopqrstuvwxyz0123456789_').strip(' _')
            if (not name):
                name = f'_{i}'
            elif name[0] in '0123456789':
                name = f'_{name}'
            if name in colnames:
                j = 1
                while (f'{name}_{j}' in colnames):
                    j += 1
                name = f'{name}_{j}'
            colnames.append(name)
            # if we have received column origin data (schema, table, colname), set the origin
            # otherwise the origin is None
            if len(desc) >= 5:
                colorig.append(MiniRow((desc[2], desc[3], desc[4]), ('schema', 'table', 'column')))
            else:
                colorig.append(MiniRow((None, None, None), ('schema', 'table', 'column')))
            # if no column datatype provided, use the origin datatype
            if dtyp is None:
                dtyp = desc[1]
            # unknown datatype is called 'variant'
            dtyp = 'variant' if dtyp is None else dtyp.lower().strip()
            # keep only the first word of dtyp, remove extra and bracket suffixes
            if ' ' in dtyp:
                dtyp, junk = dtyp.split(' ', 1)
            if '(' in dtyp:
                dtyp, junk = dtyp.split('(', 1)
            coltypes.append(''.join(c for c in dtyp if c in 'abcdefghijklmnopqrstuvwxyz'))
        if len(colnames) > 0:
            colnames = tuple(colnames)
            coltypes = tuple(coltypes)
            orgnames = tuple(orgnames)
            colorig  = tuple(colorig)
            if cursor._Cursor__scanrows:
                # create conversion function dictionary {col: func} entry for each column requiring conversion
                cvtfunc = DataConverter.dict
                cursor._Cursor__convert = dict((idx, cvtfunc[coltype]) for idx, coltype in enumerate(coltypes) if coltype in cvtfunc)
            if cursor._Cursor__rowfactory and cursor._Cursor__outputrows:
                # define the function, if any, to convert a result tuple into a row object
                makerow = cursor._Cursor__rowfactory
                if cursor._Cursor__factoryarg == 5:
                    cursor._Cursor__makerow = lambda values: makerow(values, colnames, coltypes, orgnames, colorig)
                elif cursor._Cursor__factoryarg == 2:
                    cursor._Cursor__makerow = lambda values: makerow(values, colnames)
                elif cursor._Cursor__factoryarg == 1:
                    cursor._Cursor__makerow = lambda values: makerow(values)
                elif cursor._Cursor__factoryarg == 3:
                    cursor._Cursor__makerow = lambda values: makerow(values, colnames, coltypes)
                elif cursor._Cursor__factoryarg == 4:
                    cursor._Cursor__makerow = lambda values: makerow(values, colnames, coltypes, orgnames)
    if cursor._Cursor__makerow or cursor._Cursor__convert or cursor._Cursor__rowtrace:
        super(Cursor, cursor).setrowtrace(__rowtrace__)
    else:
        super(Cursor, cursor).setrowtrace(None)
    return True


def __rowtrace__(cursor, data):
    """Row Tracer -- runs automatically

       If scanrows and data conversions, do conversions
       If rowtrace set, execute it
       If have makerow function, convert data to row
    """
    if not isinstance(cursor, Cursor):
        return data
    rowtrace = cursor._Cursor__rowtrace
    convert = cursor._Cursor__convert
    makerow = cursor._Cursor__makerow
    if data:
        if convert:
            data = tuple((convert[idx](value) if ((idx in convert) and (value is not None)) else value) for idx, value in enumerate(data)) if len(convert) else data
        if rowtrace:
            data = rowtrace(cursor, data)
        if makerow:
            data = makerow(data)
    return data


# ---------- Cursor ----------

class Cursor(apsw.Cursor):
    """Cursor object defined on a Connection object.

       Defines a Delegate Cursor object for one or more real SQLite3
       statement objects which are provided by the underlying APSW
       library.  These Cursor objects are heavier than the underlying
       native APSW cursor/statement objects and have slightly different
       behaviour, including the ability to scan bindings and output rows,
       and to convert row tuples into Row objects."""

    __slots__ = ('_Cursor__scanbindings', '_Cursor__outputrows',
                 '_Cursor__exectrace',    '_Cursor__rowtrace',
                 '_Cursor__rowfactory',   '_Cursor__makerow',
                 '_Cursor__factoryarg',   '_Cursor__scanrows',
                 '_Cursor__convert',
                )

    __Hide = [
              'description_full', 'getdescription',
              'getconnection',
              'getexectrace', 'setexectrace',
              'getrowtrace',  'setrowtrace',
              '_Cursor__Hide','_Cursor__setexectrace',
             ]

    getdescription = NotImplementedStub
    getconnection = NotImplementedStub
    getexectrace = NotImplementedStub
    setexectrace = NotImplementedStub
    getrowtrace = NotImplementedStub
    setrowtrace = NotImplementedStub

    def __dir__(self):
        return [x for x in super().__dir__() if not (x in self.__Hide or x in self.__slots__)]

    def __init__(self, db):
        """Initialize a new Cursor from the Connection.

        Requires the Connection object to be provided as an argument"""

        if not isinstance(db, Connection):
            raise ValueError('Cursor can only be constructed on Connection object')
        self.__scanbindings = db.scanbindings
        self.__scanrows = db.scanrows
        self.__outputrows = db.outputrows
        self.__rowfactory = db.rowfactory
        self.__factoryarg = db._Connection__factoryarg
        self.__exectrace = db.exectrace
        self.__rowtrace = db.rowtrace
        super().__init__(db)
        self.__setexectrace()

    def __setexectrace(self):
        self.__makerow = None
        self.__convert = None
        super().setrowtrace(None)
        if (self.__outputrows and self.__rowfactory) or self.__scanrows or self.__exectrace or self.__rowtrace:
            super().setexectrace(__exectrace__)
        else:
            super().setexectrace(None)

    def execute(self, sql, bindings=None, *, can_cache: bool = True, prepare_flags: int = 0):
        """Executes an SQL statement with optional bindings.

           If scanbindings is in effect for this Cursor then scan the bindings
           before calling the underlying APSW execute so that data conversion
           may be done for additional Python types."""

        convert = DataAdapter.dict
        if bindings is not None and self.__scanbindings and len(convert):
            if isinstance(bindings, (tuple, list)):
                newbindings = tuple(convert[type(item)](item) if type(item) in convert else item for item in bindings)
            elif isinstance(bindings, dict):
                newbindings = {key: convert[type(item)](item) if type(item) in convert else item for key, item in bindings.items()}
            else:
                raise ValueError('Bindings must be a Sequence or Dictionary')
        else:
            newbindings = bindings
        self.__setexectrace()
        return super().execute(sql, newbindings, can_cache=can_cache, prepare_flags=prepare_flags)

    def executemany(self, sql, bindlist, *, can_cache: bool = True, prepare_flags: int = 0):
        """Executes an SQL statement with a list of bindings.

           If scanbindings is in effect for this Cursor then scan the bindings
           before calling the underlying APSW executemany so that data conversion
           may be done for additional Python types."""

        newbindlist = list(bindlist)
        convert = DataAdapter.dict
        if bindlist is not None and self.__scanbindings and len(convert):
            if isinstance(bindlist, (list, tuple)):
                for bidx, bindings in enumerate(bindlist):
                    if isinstance(bindings, (list, tuple)):
                        newbindings = tuple(convert[type(item)](item) if type(item) in convert else item for item in bindings)
                    elif isinstance(bindings, dict):
                        newbindings = {key: convert[type(item)](item) if type(item) in convert else item for key, item in bindings.items()}
                    else:
                        raise ValueError('Bindings must be Sequence or Dictionary')
                    newbindlist[bidx] = newbindings
            else:
                raise ValueError('Bindlist must be a Sequence of Bindings')
        self.__setexectrace()
        return super().executemany(sql, newbindlist, can_cache=can_cache, prepare_flags=prepare_flags)

    def executescript(self, sql, bindings=None, *, can_cache: bool = True, prepare_flags: int = 0):
        """same as .execute(...) but discards all result rows, returns None"""
        saved = (self.__scanrows, self.__exectrace, self.__rowtrace, self.__outputrows)
        self.__scanrows = self.__outputrows = False
        self.__exectrace = self.__rowtrace = None
        try:
            for row in self.execute(sql, bindings, can_cache=can_cache, prepare_flags=prepare_flags):
                pass
        finally:
            self.__scanrows, self.__exectrace, self.__rowtrace, self.__outputrows = saved

    def executescriptx(self, sql, *args, can_cache: bool = True, prepare_flags: int = 0, **kwargs):
        """.executescript with expanded bindings"""
        if len(args) and len(kwargs):
            raise ValueError('Cannot mix positional and named parameters')
        self.executescript(sql, args if len(args) else (kwargs if len(kwargs) else None), can_cache=can_cache, prepare_flags=prepare_flags)

    def executex(self, sql, *args, can_cache: bool = True, prepare_flags: int = 0, **kwargs):
        """.execute with expanded bindings"""
        if len(args) and len(kwargs):
            raise ValueError('Cannot mix positional and named parameters')
        return self.execute(sql, args if len(args) else (kwargs if len(kwargs) else None), can_cache=can_cache, prepare_flags=prepare_flags)

    @property
    def exectrace(self):
        """Get/Set the exectrace for this cursor."""
        return self.__exectrace

    @exectrace.setter
    def exectrace(self, value):
        if value is not None and not callable(value):
            raise ValueError('exectrace must be a callable or None')
        self.__exectrace = value

    @property
    def rowtrace(self):
        """Get/Set the rowtrace for this cursor."""
        return self.__rowtrace

    @rowtrace.setter
    def rowtrace(self, value):
        if value is not None and not callable(value):
            raise ValueError('rowtrace must be a callable or None')
        self.__rowtrace = value

    @property
    def rowfactory(self):
        """Get/Set the rowfactory for this cursor."""
        return self.__rowfactory

    @rowfactory.setter
    def rowfactory(self, value):
        if value is not None and not callable(value):
            raise ValueError('rowfactory must be a callable or None')
        self.__rowfactory = value
        if self.__rowfactory is not None:
            try:
                self.__factoryarg = self.__rowfactory.__init__.__code__.co_argcount - 1
            except:
                self.__factoryarg = self.__rowfactory.__code__.co_argcount
        else:
            self.__factoryarg = 0

    @property
    def scanbindings(self):
        """get/set whether or not to scan bindings (bool)."""
        return self.__scanbindings

    @scanbindings.setter
    def scanbindings(self, value):
        if not isinstance(value, bool):
            raise ValueError('scanbinding must be boolean')
        self.__scanbindings = value is True

    @property
    def scanrows(self):
        """get/set whether or not to scan rows"""
        return self.__scanrows

    @scanrows.setter
    def scanrows(self, value):
        if not isinstance(value, bool):
            raise ValueError('scanrows must be boolean')
        self.__scanrows = value

    @property
    def outputrows(self):
        """get/set whether or not to output row objects (bool)."""
        return self.__outputrows

    @outputrows.setter
    def outputrows(self, value):
        if not isinstance(value, bool):
            raise ValueError('outputrows must be boolean')
        self.__outputrows = value is True

    def fetchnext(self):
        "Fetch the next row"
        try:
            return super().__next__()
        except StopIteration:
            return None
        except:
            raise

    fetchone = fetchnext

    getone = property(fetchnext)

    @property
    def getall(self):
        "Same as .fetchall()"
        return self.fetchall()

    @property
    def scalar(self):
        "Return first column of first row or None"
        row = self.fetchone()
        return row if row is None else row[0]

    @property
    def scalars(self):
        "Return a tuple of the first column value of each row"
        return tuple(row[0] for row in self)

    @property
    def get(self):
        "Fetches rows with minimal wrapping"
        rows = self.fetchall()
        if len(rows) == 1:
            rows = rows[0]
        else:
            for i, row in enumerate(rows):
                if len(row) == 1:
                    rows[i] = row[0]
        if len(rows) == 1:
            rows = rows[0]
        return rows

    @property
    def description(self):
        """Get the row description from the underlying statement."""
        return super().description_full if hasattr(super(), 'description_full') else super().description

    @property
    def connection(self):
        """Get the parent Connection associated with the Cursor."""
        return super().getconnection()


# ---------- Connection ---------


_DefaultRow_ = None
_DefaultVFS_ = None

def RowFactory(obj=None):
    "Set the Default RowFactory for new connections"
    global _DefaultRow_
    if obj is not None and not callable(obj):
        raise ValueError('Rowfactory requires a callable or None')
    _DefaultRow_ = obj

def DefaultVFS(name=None):
    "Set default VFS for new connections and attachments"
    global _DefaultVFS_
    import platform
    _DefaultVFS_ = name or ('win32' if platform.system() == 'Windows' else 'unix')

RowFactory(Row)
DefaultVFS()


class Connection(apsw.Connection):
    __slots__ = (
                 '_Connection__scanbindings','_Connection__outputrows',     '_Connection__exectrace',
                 '_Connection__rowtrace',   '_Connection__opencursors',     '_Connection__rowfactory',
                 '_Connection__factoryarg', '_Connection__scanrows',
                 '_Connection__colationneeded', '_Connection__authorizer',  '_Connection__commit',
                 '_Connection__profile',    '_Connection__rollback',        '_Connection__update',
                 '_Connection__wal',        '_Connection__steps',
                )

    __Hide =    [
                 'collationneeded',        'getautocommit',                'getexectrace',
                 'getrowtrace',            'set_last_insert_rowid',        'setauthorizer',
                 'setbusyhandler',         'setbusytimeout',               'setcommithook',
                 'setexectrace',           'setprofile',                   'setprogresshandler',
                 'setrollbackhook',        'setrowtrace',                  'setupdatehook',
                 'setwalhook',
                 '_Connection__Hide',      '_Connection__save_pragma_list',
                ]

    collationneeded = NotImplementedStub
    getautocommit = NotImplementedStub
    getexectrace = NotImplementedStub
    getrowtrace = NotImplementedStub
    set_last_insert_rowid = NotImplementedStub
    setauthorizer = NotImplementedStub
    setbusyhandler = NotImplementedStub
    setbusytimeout = NotImplementedStub
    setcommithook = NotImplementedStub
    setexectrace = NotImplementedStub
    setprofile = NotImplementedStub
    setprogresshandler = NotImplementedStub
    setrollbackhook = NotImplementedStub
    setrowtrace = NotImplementedStub
    setupdatehook = NotImplementedStub
    setwalhook = NotImplementedStub

    __save_pragma_list = (
                            'analysis_limit',
                            'auto_vacuum',
                            'automatic_index',
                            'busy_timeout',
                            'cache_size',
                            'cache_spill',
                            'case_sensitive_like',
                            'cell_size_check',
                            'checkpoint_fullfsync',
                            'defer_foreign_keys',
                            'foreign_keys',
                            'fullfsync',
                            'hard_heap_limit',
                            'ignore_check_constraints',
                            'journal_mode',
                            'journal_size_limit',
                            'legacy_alter_table',
                            'locking_mode',
                            'max_page_count',
                            'mmap_size',
                            'query_only',
                            'read_uncommitted',
                            'recursive_triggers',
                            'reverse_unordered_selects',
                            'secure_delete',
                            'soft_heap_limit',
                            'synchronous',
                            'temp_store',
                            'threads',
                            'trusted_schema',
                            'wal_autocheckpoint',
                            'writable_schema',
                         )

    def __dir__(self):
        return [x for x in super().__dir__() if not (x in self.__Hide or x in self.__slots__)]

    def __init__(self, filename='', vfs=None, statementcachesize=100, serialized=None, readonly=False, create=True, scanbindings=True, scanrows=True, outputrows=True, rowfactory=True):
        """Connection(filename='', vfs=None, statementcachesize=100, serialized=None, readonly=False, create=True, scanbindings=True, scanrows=True, outputrows=True, rowfactory=Default)"""
        global _DefaultVFS_
        from apsw import SQLITE_OPEN_READONLY, SQLITE_OPEN_READWRITE, SQLITE_OPEN_CREATE, SQLITE_OPEN_FULLMUTEX, SQLITE_OPEN_NOMUTEX, SQLITE_OPEN_URI
        self.__scanbindings = False
        self.__scanrows = False
        self.__outputrows = False
        self.__exectrace = None
        self.__rowtrace = None
        self.__opencursors = []
        self.__rowfactory = None
        self.__factoryarg = 0
        self.__colationneeded = None
        self.__authorizer = None
        self.__commit = None
        self.__profile = None
        self.__rollback = None
        self.__update = None
        self.__wal = None
        self.__steps = {}
        flags = 0
        if isinstance(readonly, bool):
            if readonly:
                create = False
                flags |= SQLITE_OPEN_READONLY
            else:
                flags |= SQLITE_OPEN_READWRITE
        else:
            raise ValueError('readonly must be True or False')
        if isinstance(create, bool):
            if create:
                flags |= SQLITE_OPEN_CREATE
        else:
            raise ValueError('create must be True or False')
        if serialized is not None:
            if isinstance(serialized, bool):
                flags |= (SQLITE_OPEN_FULLMUTEX if serialized else SQLITE_OPEN_NOMUTEX)
            else:
                raise ValueError('serialized must be True, False, or None')
        flags |= SQLITE_OPEN_URI
        if vfs is None:
            vfs = _DefaultVFS_
        super().__init__(filename, flags=flags, vfs=vfs, statementcachesize=statementcachesize)
        self.cursor_factory = Cursor
        if rowfactory is True:
            rowfactory = _DefaultRow_
        if rowfactory is not None:
            if not callable(rowfactory):
                raise ValueError(f'rowfactory must be callable')
            self.rowfactory = rowfactory
        if scanbindings is not None:
            if not isinstance(scanbindings, bool):
                raise ValueError('scanbindings must be boolean')
            self.__scanbindings = scanbindings
        if scanrows is not None:
            if not isinstance(scanrows, bool):
                raise ValueError('scanrows must be boolean')
            self.__scanrows = scanrows
        if outputrows is not None:
            if not isinstance(outputrows, bool):
                raise ValueError('outputrows must be boolean')
            self.__outputrows = outputrows is True


    def execute(self, sql, bindings=None, *, can_cache: bool = True, prepare_flags: int = 0):
        """same as .cursor().execute(...)"""
        return self.cursor().execute(sql, bindings, can_cache=can_cache, prepare_flags=prepare_flags)

    def executex(self, sql, *args, can_cache: bool = True, prepare_flags: int = 0, **kwargs):
        """.execute with expanded bindings"""
        if len(args) and len(kwargs):
            raise ValueError('Cannot mix positional and named parameters')
        return self.cursor().execute(sql, args if len(args) else (kwargs if len(kwargs) else None), can_cache=can_cache, prepare_flags=prepare_flags)

    def executemany(self, sql, bindlist=None, *, can_cache: bool = True, prepare_flags: int = 0):
        """same as .cursor().executemany(...)"""
        return self.cursor().executemany(sql, bindlist, can_cache=can_cache, prepare_flags=prepare_flags)

    def executescript(self, sql, bindings=None, *, can_cache: bool = True, prepare_flags: int = 0):
        """same as .cursor().execute(...) but discards all result rows, returns None"""
        self.cursor().executescript(sql, bindings, can_cache=can_cache, prepare_flags=prepare_flags)

    def executescriptx(self, sql, *args, can_cache: bool = True, prepare_flags: int = 0, **kwargs):
        """.executescript with expanded bindings"""
        if len(args) and len(kwargs):
            raise ValueError('Cannot mix positional and named parameters')
        self.cursor().executescript(sql, args if len(args) else (kwargs if len(kwargs) else None), can_cache=can_cache, prepare_flags=prepare_flags)


    def begin(self):
        """same as .cursor(raw=True).execute('BEGIN TRANSACTION')"""
        return self.cursor(raw=True).execute('BEGIN TRANSACTION')

    def begindeferred(self):
        """same as .cursor(raw=True).execute('BEGIN DEFERRED TRANSACTION')"""
        return self.cursor(raw=True).execute('BEGIN DEFERRED TRANSACTION')

    def beginimmediate(self):
        """same as .cursor(raw=True).execute('BEGIN IMMEDIATE TRANSACTION')"""
        return self.cursor(raw=True).execute('BEGIN IMMEDIATE TRANSACTION')

    def beginexclusive(self):
        """same as .cursor(raw=True).execute('BEGIN EXCLUSIVE TRANSACTION')"""
        return self.cursor(raw=True).execute('BEGIN EXCLUSIVE TRANSACTION')

    def commit(self):
        """same as .cursor(raw=True).execute('COMMIT TRANSACTION')"""
        return self.cursor(raw=True).execute('COMMIT TRANSACTION')

    def rollback(self, name=None):
        """same as .cursor(raw=True).execute('ROLLBACK TRANSACTION')
           if savepoint name given as parameter then this is the
           same as .cursor(raw=True).execute('ROLLBACK TRANSACTION TO SAVEPOINT "<name>"')"""
        if name is None:
            return self.cursor(raw=True).execute('ROLLBACK TRANSACTION')
        else:
            return self.cursor(raw=True).execute(f'ROLLBACK TRANSACTION TO SAVEPOINT "{name}"')

    def savepoint(self, name):
        """same as .cursor(raw=True).execute("SAVEPOINT '<name>'")"""
        return self.cursor(raw=True).execute(f"SAVEPOINT '{name}'")

    def release(self, name):
        """same as .cursor(raw=True).execute('RELEASE SAVEPOINT "<name>"')"""
        return self.cursor(raw=True).execute(f'RELEASE SAVEPOINT "{name}"')


    def attach(self, filename, schema, **kw):
        """attach the named file to the connection as the specified schema"""
        global _DefaultVFS_
        if not 'vfs' in kw:
            kw['vfs'] = _DefaultVFS_
        if kw:
            kws = '&'.join(f'{key}={value}' for key, value in kw.items())
            filename = f'{filename}?{kws}' if filename.startswith('file:') else f'file:{filename}?{kws}'
        self.cursor(raw=True).executescript(f"ATTACH '{filename}' as '{schema}'")

    def executescriptfile(self, filename, bindings=None):
        """Execute the contents of file as a script"""
        f = open(filename, 'r')
        script = f.read()
        f.close()
        self.cursor().executescript(script, bindings)


    def reloadschema(self, name=None):
        """Increment and reload specific schema or all schemata"""
        from apsw import ReadOnlyError
        schemata = self.db_names
        if name is not None:
            if name.lower() in schemata:
                schemata = [name]
            else:
                raise ValueError('Schema "%s" is not known' % (name,))
        for schema in schemata:
            try:
                version = self.execute(f'pragma "{schema}".schema_version').scalar + 1
                self.executescript(f'pragma "{schema}".schema_version={version}')
            except ReadOnlyError:
                pass
            except:
                raise

    @property
    def autocommit(self):
        """is connection in autocommit mode"""
        return super().getautocommit()

    @property
    def busy(self):
        """is a statement active on the current connection"""
        sql = 'select sql, busy from sqlite_stmt where busy'
        return True if any(row[1] for row in self.cursor(raw=True).execute(sql) if row[0] != sql) else False

    @property
    def changes(self):
        """get count of changes made by last statement on connection"""
        return super().changes()

    @property
    def collationneededhook(self):
        """get/set collation needed callback"""
        return self.__collationneeded

    @collationneededhook.setter
    def collationneededhook(self, value):
        if value is not None and not callable(value):
            raise ValueError('collationneededhook must be callable or None')
        self.__collationneeded = value
        super().collationneeded(self.__collationneeded)

    @property
    def authorizerhook(self):
        """set authorizer hook callback"""
        return self.__authorizer

    @authorizerhook.setter
    def authorizerhook(self, value):
        if value is not None and not callable(value):
            raise ValueError('authorizerhook must be callable or None')
        self.__authorizer = value
        super().setauthorizer(self.__authorizer)

    @property
    def commithook(self):
        """set commit hook callback"""
        return self.__commit

    @commithook.setter
    def commithook(self, value):
        if value is not None and not callable(value):
            raise ValueError('commithook must be callable or None')
        self.__commit = value
        super().setcommithook(self.__commit)

    @property
    def profilehook(self):
        """set profile hook callback"""
        return self.__profile

    @profilehook.setter
    def profilehook(self, value):
        if value is not None and not callable(value):
            raise ValueError('profilehook must be callable or None')
        self.__profile = value
        super().setprofile(self.__profile)

    @property
    def rollbackhook(self):
        """set rollback hook callback"""
        return self.__rollback

    @rollbackhook.setter
    def rollbackhook(self, value):
        if value is not None and not callable(value):
            raise ValueError('rollbackhook must be callable or None')
        self.__rollback = value
        super().setrollbackhook(self.__rollback)

    @property
    def updatehook(self):
        """set update hook callback"""
        return self.__update

    @updatehook.setter
    def updatehook(self, value):
        if value is not None and not callable(value):
            raise ValueError('updatehook must be callable or None')
        self.__update = value
        super().setupdatehook(self.__update)

    @property
    def walhook(self):
        """set wal checkpoint hook callback"""
        return self.__wal

    @walhook.setter
    def walhook(self, value):
        if value is not None and not callable(value):
            raise ValueError('walhook must be callable or None')
        self.__wal = value
        super().setwalhook(self.__wal)

    @property
    def totalchanges(self):
        """get total number of changes made on connection"""
        return super().totalchanges()

    @property
    def wal_autocheckpoint(self):
        """set wal autocheckpoint pages count"""
        raise NotImplementedError

    @wal_autocheckpoint.setter
    def wal_autocheckpoint(self, value):
        if not isinstance(value, int):
            raise ValueError('wal_autocheckpoint must be an integer')
        super().wal_autocheckpoint(value)

    @property
    def intransaction(self):
        """(not autocommit) or busy"""
        return (not self.autocommit) or (self.txn_state(None) > 0)

    @property
    def busyhandler(self):
        """get/set the the busy handler"""
        raise NotImplementedError

    @busyhandler.setter
    def busyhandler(self, value):
        if value is not None and not callable(value):
            raise ValueError('busyhandler must be a callable or None')
        super().setbusyhandler(value)

    @property
    def busytimeout(self):
        """get/set busy timeout"""
        return self.cursor(raw=True).execute('pragma busy_timeout').scalar

    @busytimeout.setter
    def busytimeout(self, value):
        if not isinstance(value, int):
            raise ValueError('busytimeout must be an integer')
        if value < 0 or value > 300000:
            raise ValueError('busytimeout must be between 0 and 300000')
        super().setbusytimeout(value)

    @property
    def exectrace(self):
        """get/set execution tracer"""
        return self.__exectrace

    @exectrace.setter
    def exectrace(self, value):
        if value is not None and not callable(value):
            raise ValueError('exectrace must be a callable or None')
        self.__exectrace = value

    @property
    def rowtrace(self):
        """get/set row tracer"""
        return self.__rowtrace

    @rowtrace.setter
    def rowtrace(self, value):
        if value is not None and not callable(value):
            raise ValueError('rowtrace must be a callable or None')
        self.__rowtace = value

    @property
    def scanbindings(self):
        """get/set whether or not to scan bindings"""
        return self.__scanbindings

    @scanbindings.setter
    def scanbindings(self, value):
        if not isinstance(value, bool):
            raise ValueError('scanbindings must be boolean')
        self.__scanbindings = value

    @property
    def scanrows(self):
        """get/set whether or not to scan rows"""
        return self.__scanrows

    @scanrows.setter
    def scanrows(self, value):
        if not isinstance(value, bool):
            raise ValueError('scanrows must be boolean')
        self.__scanrows = value

    @property
    def outputrows(self):
        """get/set whether or not to output row objects"""
        return self.__outputrows

    @outputrows.setter
    def outputrows(self, value):
        if not isinstance(value, bool):
            raise ValueError('outputrows must be boolean')
        self.__outputrows = value

    @property
    def last_insert_rowid(self):
        """get/set rowid of last insert on this connection"""
        return super().last_insert_rowid()

    @last_insert_rowid.setter
    def last_insert_rowid(self, value):
        if not isinstance(value, int):
            raise ValueError('rowid must be integer')
        super().set_last_insert_rowid(value)

    def cursor(self, scanbindings=None, scanrows=None, outputrows=None, rowfactory=None, raw=False, exectrace=None, rowtrace=None):
        """Create and return a Cursor object.  scanbindings/outputrows/rowfactory can be overridden"""
        from weakref import ref
        cr = super().cursor()
        self.__opencursors = [x for x in self.__opencursors if x() is not None]
        self.__opencursors.append(ref(cr))
        if isinstance(raw, bool):
            if raw:
                scanbindings = False
                scanrows = False
                outputrows = False
        else:
            raise ValueError('raw must be boolean')
        if scanbindings is not None:
            if not isinstance(scanbindings, bool):
                raise ValueError('scanbindings must be boolean')
            cr.scanbindings = scanbindings
        else:
            cr.scanbindings = self.scanbindings
        if scanrows is not None:
            if not isinstance(scanrows, bool):
                raise ValueError('scanrows must be boolean')
            cr.scanrows = scanrows
        else:
            cr.scanrows = self.scanrows
        if outputrows is not None:
            if not isinstance(outputrows, bool):
                raise ValueError('outputrows must be boolean')
            cr.outputrows = outputrows
        else:
            cr.outputrows = self.outputrows
        if rowfactory is not None:
            if not callable(rowfactory):
                raise ValueError('rowfactory must be callable')
            cr.rowfactory = rowfactory
        else:
            cr.rowfactory = self.rowfactory
        if exectrace is not None:
            if not callable(exectrace):
                raise ValueError('exectrace must be callable')
            cr.exectrace = exectrace
        else:
            cr.exectrace = self.exectrace
        if rowtrace is not None:
            if not callable(rowtrace):
                raise ValueError('rowtrace must be callable')
            cr.rowtrace = rowtrace
        else:
            cr.rowtrace = self.rowtrace
        return cr

    @property
    def opencursors(self):
        """return the number of cursors attached to this connection"""
        from gc import collect
        collect()
        self.__opencursors = [x for x in self.__opencursors if x() is not None]
        return len(self.__opencursors)

    def closecursors(self, force=False):
        """Close all cursors associated with this connection using force"""
        for cr in self.__opencursors:
            cr = cr()
            if cr is not None:
                cr.close(force)
        cr = None
        return self.opencursors

    def close(self, force=False):
        """Close all cursors attached to this connection, close the underlying apsw connection, using force"""
        self.closecursors(force)
        super().close(force)

    def get_application_id(self, schema=None):
        """Get the Application_id of the specified schema"""
        schema = 'main' if schema is None else schema
        return GetAppID(self.cursor(raw=True).execute(f'pragma "{schema}".application_id').scalar)

    def set_application_id(self, appid, schema=None):
        """Set the Application_id of the specified schema"""
        schema = 'main' if schema is None else schema
        self.cursor(raw=True).executescript(f'pragma "{schema}".application_id={MakeAppID(appid)}')

    def get_user_version(self, schema=None):
        """Get the user_version of the specified schema"""
        schema = 'main' if schema is None else schema
        return self.cursor(raw=True).execute(f'pragma "{schema}".user_version').scalar

    def set_user_version(self, v, schema=None):
        """Set the user_version of the specified schema"""
        schema = 'main' if schema is None else schema
        self.cursor(raw=True).executescript(f'pragma "{schema}".user_version={v}')

    def get_data_version(self, schema=None):
        """Get the data_version of the specified schema"""
        schema = 'main' if schema is None else schema
        return self.cursor(raw=True).execute(f'pragma "{schema}".data_version').scalar

    def get_schema_version(self, schema=None):
        """Get the schema_version of the specified schema"""
        schema = 'main' if schema is None else schema
        return self.cursor(raw=True).execute(f'pragma "{schema}".schema_version').scalar

    @property
    def application_id(self):
        """Get/Set the application_id for the main schema"""
        return GetAppID(self.cursor(raw=True).execute('pragma application_id').scalar)

    @application_id.setter
    def application_id(self, appid):
        self.cursor(raw=True).executescript(f'pragma application_id={MakeAppID(appid)}')

    @property
    def user_version(self):
        """Get/Set the user_version for the main schema"""
        return self.cursor(raw=True).execute('pragma user_version').scalar

    @user_version.setter
    def user_version(self, v):
        self.cursor(raw=True).executescript(f'pragma user_version={v}')

    @property
    def data_version(self):
        """Get the data_version for the main schema"""
        return self.cursor(raw=True).execute('pragma data_version').scalar

    @property
    def schema_version(self):
        """Get the schema_version for the main schema"""
        return self.cursor(raw=True).execute('pragma schema_version').scalar

    def saveconfig(self, schema=None):
        """Save pragma configuration"""
        config = {}
        schemalist = self.db_names
        if schema is None:
            cr = self.cursor(raw=True)
            for schema in schemalist:
                for pragma in self.__save_pragma_list:
                    param = f'{schema}.{pragma}'
                    try:
                        config[param] = cr.execute(f'pragma {param}').scalar
                    except:
                        pass
        else:
            if schema.lower() in schemalist:
                cr = self.cursor(raw=True)
                for pragma in self.__save_pragma_list:
                    try:
                        config[pragma] = cr.execute(f'pragma {schema}.{pragma}').scalar
                    except:
                        pass
            else:
                return ValueError(f'Schema does not exist: {schema}')
        return config

    def applyconfig(self, config, schema=None):
        """Apply saved schema configuration"""
        if schema is None:
            cr = self.cursor(raw=True)
            for pragma in config.keys():
                cr.executescript(f'pragma {pragma}={config[pragma]}')
        else:
            schemalist = self.db_names
            if schema.lower() in schemalist:
                prefix = f'{schema.lower()}.'
                cr = self.cursor(raw=True)
                for pragma in config.keys():
                    if pragma.startswith(prefix):
                        cr.executescript(f'pragma {pragma}={config[pragma]}')
                    elif not '.' in pragma:
                        cr.executescript(f'pragma {schema}.{pragma}={config[pragma]}')
            else:
                return ValueError(f'Schema does not exist: {schema}')

    def makeidkey(self, name, decltype=None):
        """Make id/value table as

        create table if not exists {name}s({name}id integer primary key, {name} {decltype} not null unique)

        if called with makeidkey('category', 'text collate nocase') then the table looks like
            create table if not exists categorys
            (
                categoryid integer primary key,
                category text collate nocase not null unique
            )
        """
        if decltype is None:
            decltype = ''
        self.execute(f'create table if not exists {name}s\n(\n  {name}id integer primary key,\n  {name} {decltype} unique\n)')

    def idkey(self, name, value):
        """Insert the value into the idkey table if not present, return the id corresponding to the value

        Table declaration looks like:
            create table {name}s ({name}id integer primary key, {name} not null unique);
        """
        return self.cursor().executex(f'insert or ignore into {name}s ({name}) values (?); select {name}id from {name}s where {name}==?', value, value).scalar

    @property
    def rowfactory(self):
        """Get/Set the Rowfactory used by this Cursor"""
        return self.__rowfactory

    @rowfactory.setter
    def rowfactory(self, value):
        """Get/Set the Rowfactory used by this Cursor"""
        if value is not None and not callable(value):
            raise ValueError('rowfactory must be a callable or None')
        self.__rowfactory = value
        if self.__rowfactory is not None:
            try:
                self.__factoryarg = self.__rowfactory.__init__.__code__.co_argcount - 1
            except:
                self.__factoryarg = self.__rowfactory.__code__.co_argcount
        else:
            self.__factoryarg = 0

    @property
    def autovacuum_pages(self):
        """Get/Set autovacuum_pages callback"""
        raise NotImplementedError

    @autovacuum_pages.setter
    def autovacuum_pages(self, value):
        if value is not None and not callable(value):
            raise ValueError('autovacuum_pages must be a callable or None')
        super().autovacuum_pages(value)

    def backup(self, dstdatabase, srcconnection, srcdatabase):
        """Return apsw backup object"""
        return super().backup(dstdatabase, srcconnection, srcdatabase)

    def blobopen(self, schema, table, column, rowid, writeable):
        """Return apsw Blob object"""
        return super().blobopen(schema, table, column, rowid, writeable)

    def config(self, op, *arg):
        """Configure the underlying connection"""
        return super().config(op, *arg)

    def createaggregatefunction(self, name, factory, numargs=-1):
        """Create an aggregate function"""
        if factory is not None and not callable(factory):
            raise ValueError('factory must be callable')
        return super().createaggregatefunction(name, factory, numargs)

    def createcollation(self, name, callback):
        """Create a Collation"""
        if callback is not None and not callable(callback):
            raise ValueError('createcollation callback must be callable or None')
        return super().createcollation(name, callback)

    def createmodule(self, name, src):
        """Create a Module"""
        return super().createmodule(name, src)

    def createscalarfunction(self, name, value, numargs=-1, deterministic=False):
        """Create a Scalar Function"""
        return self.apswconnection.createscalarfunction(name, value, numargs, deterministic)

    def db_filename(self, schema=None):
        """Get the filename for schema (default main)"""
        return super().db_filename(schema or 'main')

    def db_vfs(self, schema=None):
        from ctypes import c_char_p, addressof
        from apsw import SQLITE_FCNTL_VFSNAME
        ptr = c_char_p()
        if self.filecontrol(schema or 'main', SQLITE_FCNTL_VFSNAME, addressof(ptr)):
            return ptr.value.decode('utf8')
        return None

    @property
    def db_names(self):
        """Return the names of the schema's attached to this connection"""
        return tuple(map(str.lower,super().db_names()))

    def deserialize(self, name, contents):
        """Deserialize contents to overwrite the named database schema"""
        return super().deserialize(name, contents)

    @property
    def enableloadextension(self):
        """Set ability to load Extensions"""
        raise NotImplementedError

    @enableloadextension.setter
    def enableloadextension(self, value):
        if not isinstance(value, bool):
            raise ValueError('enableloadextension must be boolean')
        return super().enableloadextension(value)

    def filecontrol(self, dbname, op, ptr):
        """Perform filecontrol on underlying connection"""
        return super().filecontrol(dbname, op, ptr)

    def get_fcntl_version(self, dbname='main'):
        """Get the FCNTL_DATA_VERSION for the named schema"""
        from ctypes import c_int, addressof
        from apsw import SQLITE_FCNTL_DATA_VERSION
        val = c_int(-1)
        try:
            if super().filecontrol(dbname, SQLITE_FCNTL_DATA_VERSION, addressof(val)):
                return val.value
        except:
            pass
        return None

    @property
    def fcntl_version(self):
        """Return the FCNTL_DATA_VERSION of the main schema"""
        from ctypes import c_int, addressof
        from apsw import SQLITE_FCNTL_DATA_VERSION
        val = c_int(-1)
        if super().filecontrol('main', SQLITE_FCNTL_DATA_VERSION, addressof(val)):
            return val.value
        return None

    @property
    def fcntl_versions(self):
        """Return a dictionary of {schema: version}"""
        from ctypes import c_int, addressof
        from apsw import SQLITE_FCNTL_DATA_VERSION
        versions = dict()
        for schema in self.db_names:
            value = c_int(-1)
            try:
                if super().filecontrol(schema, SQLITE_FCNTL_DATA_VERSION, addressof(value)):
                    versions[schema] = value.value
            except:
                pass
        return versions

    @property
    def filename(self):
        """Filename of the main database"""
        return super().filename

    @property
    def vfs(self):
        from ctypes import c_char_p, addressof
        from apsw import SQLITE_FCNTL_VFSNAME
        ptr = c_char_p()
        if self.filecontrol('main', SQLITE_FCNTL_VFSNAME, addressof(ptr)):
            return ptr.value.decode('utf8')
        return None

    def interrupt(self):
        """Interrupt statements running on Connection"""
        return super().interrupt()

    def limit(self, id, newval=-1):
        """Adjust SQLite3 Limit"""
        return super().limit(id, newval)

    def loadextension(self, filename, entrypoint=None):
        """Load an SQLite3 extension"""
        return super().loadextension(filename, entrypoint)

    @property
    def open_flags(self):
        """Flags used to open connection"""
        return super().open_flags

    @property
    def open_vfs(self):
        """vfs used to open connection"""
        return super().open_vfs

    def overloadfunction(self, name, nargs):
        """Create overload function"""
        return super().overloadfunction(name, nargs)

    def readonly(self, schema=None):
        """Is schema (default main) readonly?"""
        return super().readonly(schema or 'main')

    def serialize(self, schema=None):
        """Serialize database to bytes"""
        return super().serialize(schema or 'main')

    def progresshandler(self, handler, steps=20):
        """Progress Handler Callback"""
        if handler is not None and not callable(handler):
            raise ValueError('progresshandler must be callable or None')
        super().progresshandler(handler, steps)

    @property
    def sqlite3pointer(self):
        """Underlying connection sqlite*"""
        return super().sqlite3pointer()

    def status(self, op, reset=False):
        """Return current and highwater measurements for the database"""
        return super().status(op, reset)

    def txn_state(self, schema=None):
        """Return the transaction state of the schema"""
        return super().txn_state(schema)

    def wal_checkpoint(dbname=None, mode=apsw.SQLITE_CHECKPOINT_PASSIVE):
        """Checkpoint the specified database"""
        return super().wal_checkpoint(dbname, mode)

    @property
    def rawcursors(self):
        saved = (self.scanbindings, self.scanrows, self.outputrows)
        self.scanbindings = self.scanrows = self.outputrows = False
        return saved

    @rawcursors.setter
    def rawcursors(self, saved):
        self.scanbindings, self.scanrows, self.outputrows = saved
        return


    def __stepbegin(self, stepname, ttype=None):
        stepname = stepname.lower()
        if self.autocommit:
            self.cursor(raw=True).execute(f'BEGIN {ttype} TRANSACTION')
            self.__steps = {stepname: True}
        else:
            if stepname in self.__steps:
                raise AttributeError(f'Step "{stepname}" already in use')
            self.cursor(raw=True).execute(f"SAVEPOINT '{stepname}'")
            self.__steps[stepname] = False

    def stepbegin(self, stepname):
        """Begin a Step by savepoint or deferred transaction"""
        self.__stepbegin(stepname, 'DEFERRED')

    def stepbegindeferred(self, stepname):
        """Begin a Step by savepoint or deferred transaction"""
        self.__stepbegin(stepname, 'DEFERRED')

    def stepbeginimmediate(self, stepname):
        """Begin a Step by savepoint or immediate transaction"""
        self.__stepbegin(stepname, 'IMMEDIATE')

    def stepbeginexclusive(self, stepname):
        """Begin a Step by savepoint or exclusive transaction"""
        self.__stepbegin(stepname, 'EXCLUSIVE')

    def stepcommit(self, stepname):
        """Commit the step -- either release the savepoint or commit the transaction"""
        stepname = stepname.lower()
        if not stepname in self.__steps:
            raise AttributeError(f'Step "{stepname}" is unknown')
        if self.__steps[stepname]:
            self.cursor(raw=True).execute(f'COMMIT TRANSACTION')
            self.__steps = {}
        else:
            self.cursor(raw=True).execute(f'RELEASE "{stepname}"')
            self.__steps.pop(stepname)

    def steprollback(self, stepname):
        """Rollback the step -- either rollback and release the savepoint or rollback the transaction"""
        stepname = stepname.lower()
        if not stepname in self.__steps:
            raise AttributeError(f'Step "{stepname}" is unknown')
        if self.__steps[stepname]:
            self.cursor(raw=True).execute('ROLLBACK TRANSACTION')
            self.__steps = {}
        else:
            self.cursor(raw=True).execute(f'ROLLBACK TO "{stepname}"')
            self.cursor(raw=True).execute(f'RELEASE "{stepname}"')
            self.__steps.pop(stepname)

    def schemahash(self, schema=None):
        schema = schema or 'main'
        return self.execute(f'select hex(sha256(group_concat(sql))) from (select sql from "{schema}".sqlite_master where sql is not null order by sql)').get

    def schemaslug(self, schema=None):
        hash = self.schemahash(schema)
        hashlen = len(hash)
        start = int(hash[:2],16) % (hashlen - 2)
        start = int(hash[start:start+2],16) % (hashlen - 8)
        return int(hash[start:start+8],16) % (pow(2,31)-1)

# ---------- ConnectionPool ----------


class ConnectionPool(object):
    """Create a connection pool of unlimited size where you can
       get and put Connection objects.  The parameters are the
       parameters to be used on the Connection method to create
       a new connection.

       Basic Usage:
        pool = ConnectionPool('mydatabase.db')
        db = pool.get()
        ...
        pool.put(db)
        db = None

       Keyword poolsize specifies the maximum number of connection
       objects kept in the pool.  More may be used but when put
       back in the pool only the poolsize number of connections
       are kept ready.

       Outstanding transactions are rolled back when a Connection
       is returned to the pool.  You should not use a Connection
       after returning it to the pool.

       If you do not return a Connection to the pool it will be
       closed when it goes out-of-scope.  Connections that you .get()
       from the pool are no longer kept in the pool and you have
       retrieved the sole reference to that Connection.
       """

    __slots__ = ('_ConnectionPool__pool', '_ConnectionPool__args', '_ConnectionPool__kwargs', '_ConnectionPool__initialize')

    def __init__(self, *args, **kwargs):
        """Initialize connection pool of given poolsize, rest of parameters are passed to Connection constructor"""
        import queue
        size = 5
        if 'poolsize' in kwargs:
            size = kwargs['poolsize']
            del kwargs['poolsize']
        size = max(size, 5)
        self.__pool = queue.Queue(size)
        self.__args = args
        self.__kwargs = kwargs
        self.__initialize = list()
        self.init_pragma('defer_foreign_keys', '0')
        self.init_pragma('foreign_keys','1')
        self.init_pragma('ignore_check_constraints','0')
        self.init_pragma('legacy_alter_table','0')
        self.init_pragma('recursive_triggers','1')
        self.init_pragma('trusted_schema','0')
        self.init_attach('C:/Python/SQLite/tz.db', '_tz_', mode='ro')
        self.init_scriptfile('C:/Python/SQLite/Functions.SQL')
        self.init_scriptfile('C:/Python/SQLite/Schema.SQL')
        self.init_scriptfile('C:/Python/SQLite/VirtualTablesPy.SQL')
        self.init_scriptfile('C:/Python/SQLite/WellKnownSids.sql')

    def put(self, db):
        """Put/Return a connection to the connection pool"""
        if not isinstance(db, Connection):
            raise ValueError('Can only put Connection objects in Connection pool')
        self.__pool.task_done()
        if db.intransaction:
            try:
                db.rollback()
            except:
                pass
        if db.opencursors:
            db.closecursors(True)
        if db.intransaction:
            try:
                db.rollback()
            except:
                pass
        if db.opencursors:
            db.closecursors(True)
        db.executescript('pragma temp_store=0')
        db.executescript('pragma temp_store=1')
        db.executescript('pragma temp_store=2')
        db.executescript('pragma temp_store=0')
        db.executescript('pragma temp_store=2')
        db.executescript('pragma temp_store=1')
        db.executescript('pragma temp_store=0')
        try:
            self.__pool.put_nowait(db)
        except:
            try:
                db.close(True)
            except:
                pass
        return None

    def init_clear(self):
        """Clear the initial processing list"""
        self.__initialize = list()

    def init_attach(self, *arg, **kw):
        """init_attach(filename, aliasname, ...)

           Ensure that the aliasname is attached, if not then attach it"""
        self.__initialize.append(('attach', arg, kw))

    def init_pragma(self, pragmaname, setting):
        """Execute the pragma pragmaname=setting on retrieved connections"""
        self.__initialize.append(('pragma', (pragmaname, setting), None))

    def init_executescript(self, arg, bindings=None):
        """Execute the script with the specified bindings on retrieved connections"""
        self.__initialize.append(('script', (arg, bindings), None))

    def init_scriptfile(self, filename, bindings=None):
        """Execute the scriptfile with the specified bindings on retrieved connections"""
        self.__initialize.append(('file', (filename, bindings), None))

    def get(self):
        """Retrieve a connection from the pool and initialize it"""
        db = None
        while db is None:
            if self.__pool.empty():
                self.__pool.put_nowait(Connection(*self.__args, **self.__kwargs))
            db = self.__pool.get_nowait()
        for action, arg, kw in self.__initialize:
            if action == 'attach' and not arg[1].lower() in db.db_names:
                db.attach(*arg, **kw)
            elif action == 'pragma':
                db.cursor(raw=True).execute(f'pragma {arg[0]}={arg[1]}')
            elif action == 'script':
                db.executescript(*arg)
            elif action == 'file':
                db.executescriptfile(*arg)
        return db

    @property
    def size(self):
        """Size of the pool"""
        return self.__pool.qsize()

    @property
    def maxsize(self):
        """Get/Set maximum size of the pool"""
        return self.__pool.maxsize

    @maxsize.setter
    def maxsize(self, size):
        self.__pool.maxsize = max(5, size)

    @property
    def outstanding(self):
        """How many connections checked out but not returned"""
        return self.__pool.unfinished_tasks

    def close(self):
        """Close all the connections in the pool / drain the pool"""
        try:
            while True:
                db = self.__pool.get_nowait()
                self.__pool.task_done()
                db.close(True)
        except:
            pass

    @property
    def connection(self):
        """Get or Return a connection"""
        return self.get()

    @connection.setter
    def connection(self, db):
        self.put(db)


# ---------- CArray ----------

class CArray(dict):
    __immutable = False

    def __init__(self, name=None, args=None):
        import ctypes
        self.__immutable = False
        charconv = lambda x: f'{x}'.encode('utf8')
        if name and args and isinstance(name, str) and isinstance(args, list):
            if any(x is None for x in args):
                raise ValueError('List must not contain None items')
            arrtype = 'int32'
            cvtfunc = int
            ctype = ctypes.c_int32
            size = len(args)
            if any(isinstance(x, str) for x in args):
                arrtype = 'char*'
                cvtfunc = charconv
                ctype = ctypes.c_char_p
            elif any(isinstance(x, float) for x in args):
                arrtype = 'double'
                cvtfunc = float
                ctype = ctypes.c_double
            elif any((x < -9223372036854775808 or x > 9223372036854775807) for x in args):
                arrtype = 'double'
                cvtfunc = float
                ctype = ctypes.c_double
            elif any((x < -2147483648 or x > 2147483647) for x in args):
                arrtype = 'int64'
                cvtfunc = int
                ctype = ctypes.c_int64
            carray = (ctype * size)(*map(cvtfunc, args))
            address = ctypes.addressof(carray)
            self[f'{name}_address'] = address
            self[f'{name}_length'] = size
            self[f'{name}_type'] = arrtype
            self[f'{name}_carray'] = carray
            self.__immutable = True
        else:
            raise ValueError('Argument 1 must be object name (str), Argument 2 must be list')

    def __delattr__(self, *args, **kwargs):
        if self.__immutable:
            raise NotImplementedError('Object is Immutable')
        return super().__delattr__(*args, **kwargs)

    def __delitem__(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().__delitem__(*args, **kwargs)

    def __setattr__(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().__setattr__(*args, **kwargs)

    def __setitem__(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().__setitem__(*args, **kwargs)

    def clear(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().clear(*args, **kwargs)

    def pop(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().pop(*args, **kwargs)

    def popitem(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().popitem(*args, **kwargs)

    def setdefault(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().setdefault(*args, **kwargs)

    def update(self, *args, **kwargs):
        if self.__immutable:
            return NotImplementedError('Object is Immutable')
        return super().update(self, *args, **kwargs)


# ---------- Logging and Tracing ----------

_Logging_ = False
_Tracing_ = False
_LogFileName_ = 'SQLite3.log'

def getLogging():
    """Return whether or not Logging is active"""
    return _Logging_

def getTracing():
    """Return whether or not Tracing is Active"""
    return _Tracing_

def setLogging(bool=False):
    """Set Logging Status"""
    global _Logging_
    _Logging_ = True if bool else False

def setTracing(bool=False):
    """Set Tracing Status"""
    global _Tracing_
    _Tracing_ = True if bool else False

def setLogFile(FileName):
    """Set Log Filename"""
    global _LogFileName_
    if FileName:
        _LogFileName_ = FileName
    else:
        raise ValueError('filename must not be empty')

def _logFileHandler_(errcode, message):
    """INTERNAL USE: if Logging then write the errcode and message to the logfile"""
    from apsw import mapping_result_codes, mapping_extended_result_codes
    import datetime
    global _Logging_, _Tracing_
    if _Logging_ or (_Tracing_ and errcode == 0xffff):
        f = open(_LogFileName_, 'a', encoding='utf8')
        if errcode == 0x7fffffff:
            f.write(f"{datetime.datetime.now().isoformat(' ')}: TRACE: {message}")
        else:
            errstr = mapping_result_codes[errcode & 255]
            extended = errcode & ~ 255
            f.write("%s: %s (%d) %s %s\n" % (datetime.datetime.now().isoformat(' '), message, errcode, errstr, mapping_extended_result_codes.get(extended, "")))
        f.close()

apsw.config(apsw.SQLITE_CONFIG_LOG, _logFileHandler_)

del _logFileHandler_


# ---------- MakeAppID & GetAppID ----------


def MakeAppID(appid, length=4):
    id = 0
    for c in appid[:length]:
        id = (id << 8) + (ord(c) % 256)
    mask = 1 << ((8 * length) - 1)
    return (id & (mask - 1)) - (id & mask)


def GetAppID(appid, length=4):
    id = list()
    appid &= (1 << (length * 8)) - 1
    while appid > 0:
        id.insert(0, chr(appid % 256))
        appid //= 256
    return ''.join(id)

# ---------- DataAdapter and DataConverter ----------

class Adapter(object):
#    __slots__ = [ '_Adapter__DataAdapter', '_Adapter__NativeTypes' ]

    def __init__(self):
        self.__DataAdapter = {}
        self.__NativeTypes = (type(None), int, float, str, bytes, bytearray, bool)

    def __call__(self, pytype=None, pytosql=None):
        """Add or Remove a DataAdapter python -> sqlite3"""
        if isinstance(pytype, self.__NativeTypes) or pytype in self.__NativeTypes:
            raise ValueError('Adapter for a Native Type is not permitted')
        if pytosql is None and pytype in self.__DataAdapter:
            del self.__DataAdapter[pytype]
        elif pytosql is not None:
            if not callable(pytosql):
                raise ValueError('Adapter function must be callable or None')
            self.__DataAdapter[pytype] = pytosql

    @property
    def dict(self):
        return self.__DataAdapter

DataAdapter = Adapter()

del Adapter


class Converter(object):
#    __slots__ = [ '_Converter__DataConverter', '_Converter__SQLTypes' ]

    def __init__(self):
        self.__DataConverter = {}
        self.__SQLTypes = ('integer', 'real', 'float', 'text', 'blob', 'numeric', 'variant')

    def __call__(self, sqltype=None, sqltopy=None):
        """Add or Remove a DataConverter sqlite3 -> python"""
        if not isinstance(sqltype, str):
            raise ValueError('SQLite3 datatype must be a string')
        sqltype = sqltype.lower().strip()
        if sqltype in self.__SQLTypes:
            raise ValueError('Cannot register Converter for a Native Type')
        if sqltopy is None and sqltype in self.__DataConverter:
            del self.__DataConverter[sqltype]
        if sqltopy is not None:
            if not callable(sqltopy):
                raise ValueError('Converter function must be callable or None')
            self.__DataConverter[sqltype] = sqltopy

    @property
    def dict(self):
        return self.__DataConverter

DataConverter = Converter()

del Converter

# ---------- boolean ----------

def boolAdapter():
    def sqltopy(x):
        if isinstance(x, (int, float)):
            return True if float(x) != 0.0 else False
        b = '-+0123456789'
        if isinstance(x, bytes):
            b = b.encode()
        for n in range(len(x)):
            p = b.find(x[n])
            if p < 0:
                return False
            if p > 2:
                return True
        return False
    DataConverter('bool', sqltopy)
    DataConverter('boolean', sqltopy)

boolAdapter()

del boolAdapter

# ---------- datetime.datetime ----------

def datetimeAdapter():
    from datetime import datetime, timedelta, timezone
    unixepoch = datetime(1970, 1, 1, tzinfo=timezone.utc)
    def pytosql(obj):
        tz = ''
        if obj.tzinfo is not None:
            tz = int(obj.utcoffset().total_seconds()) // 60
            sgn = '-' if tz < 0 else '+'
            tz = abs(tz)
            tz = ' Z' if tz == 0 else f' {sgn}{(tz // 60):02d}:{(tz % 60):02d}'
        return f"{obj.replace(microsecond=0, tzinfo=None).isoformat(' ')}.{obj.microsecond // 1000:03d}{tz}"
    def sqltopy(obj):
        obj = obj.strip()
        if (not 'T' in obj) and ' ' in obj:
            obj = obj.replace(' ', 'T', 1)
        if obj.endswith('Z'):
            obj = obj[:-1] + '+00:00'
        return datetime.fromisoformat(obj.replace(' ', ''))
    DataAdapter(datetime, pytosql)
    DataConverter('datetime', sqltopy)
    DataConverter('unixepoch', lambda x: (unixepoch + timedelta(seconds=x)))
    DataConverter('julianday', lambda x: (unixepoch + timedelta(seconds=round((x-2440587.5)*86400, 3))))

datetimeAdapter()

del datetimeAdapter

# ---------- decimal.Decimal ----------

def decimalAdapter():
    from decimal import Decimal
    DataAdapter(Decimal, str)
    DataConverter('decimal', Decimal)

decimalAdapter()

del decimalAdapter

# ---------- ipAddress / netaddr ----------

def ipAdapter():
    import functools

    typelist = list()
    useipaddress = False
    usenetaddr = False
    try:
        from ipaddress import IPv4Address, IPv4Interface, IPv4Network, IPv6Address, IPv6Interface, IPv6Network, ip_address, ip_interface, ip_network
        typelist.append(IPv4Address)
        typelist.append(IPv4Interface)
        typelist.append(IPv4Network)
        typelist.append(IPv6Address)
        typelist.append(IPv6Interface)
        typelist.append(IPv6Network)
        useipaddress = True
    except:
        pass
    try:
        from netaddr import IPAddress, IPNetwork
        typelist.append(IPAddress)
        typelist.append(IPNetwork)
        usenetaddr = True
    except:
        pass
    if not (useipaddress or usenetaddr):
        return
    def sqltopy(which, obj):
        out = None
        for idx in [which, -which]:
            if idx > 0 and useipaddress and out is None:
                if '/' in obj:
                    try:
                        out = ip_network(obj)
                    except:
                        out = ip_interface(obj)
                else:
                    out = ip_address(obj)
            elif idx < 0 and usenetaddr and out is None:
                if '/' in obj:
                    out = IPNetwork(obj)
                else:
                    out = IPAddress(obj)
        return out or obj
    for cls in typelist:
        DataAdapter(cls, str)
    DataConverter('ipaddress', functools.partial(sqltopy, 1))
    DataConverter('netaddr', functools.partial(sqltopy, -1))

ipAdapter()

del ipAdapter

# ---------- jsonAdapter ----------

def jsonAdapter():
    from json import loads
    DataConverter('json', lambda x: x if x is None else loads(x))

jsonAdapter()

del jsonAdapter

# ---------- pickleAdapter ----------

def pickleAdapter():
    from pickle import loads
    DataConverter('pickle', lambda x: x if x is None else loads(x))

pickleAdapter()

del pickleAdapter

# ---------- uuidAdapter ----------

def uuidAdapter():
    from uuid import UUID
    DataAdapter(UUID, lambda obj: obj if obj is None else obj.bytes)
    sqltopy = lambda obj: obj if obj is None else UUID(bytes=obj)
    DataConverter('uuid', sqltopy)
    DataConverter('guid', sqltopy)

uuidAdapter()

del uuidAdapter

# ---------- _IOBase ----------

def IOBaseAdapter():
    from _io import _IOBase
    DataAdapter(_IOBase, lambda x: x.name)

IOBaseAdapter()

del IOBaseAdapter

# ---------- complex ----------

def complexAdapter():
    DataAdapter(complex, str)
    DataConverter('complex', complex)

complexAdapter()

del complexAdapter

# ---------- bytearray ----------

def bytearrayAdapter():
    DataConverter('bytearray', bytearray)

bytearrayAdapter()

del bytearrayAdapter

# ----------  ----------

# ---------- ----------

# ---------- ----------

# ---------- ----------

# ---------- ----------

# ---------- ----------

try:
    del apsw
except:
    pass
try:
    del sys
except:
    pass

