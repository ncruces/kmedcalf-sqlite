
-- SysObjects view requires database_info pragma in SQLite3 3.31.0 and later
-- SysTriggers view requires trigger_list pragma in SQLite3 3.31.0 and later
-- SysTables view requires table_list pragma in SQLite3 3.31.0 and later

-- all TEXT columns in all views have "collate icu_und_aici" attached to the
-- output columns to ensure that where conditions on retrievals are not
-- case sensitive, Column Names in views defined so as to not conflict with
-- keywords to ensure quoting when using the views is not required

drop view if exists sysDatabases;
drop view if exists sysForeignKeys;
drop view if exists sysTriggers;
drop view if exists sysIndexColumns;
drop view if exists sysIndexes;
drop view if exists sysColumns;
drop view if exists sysTables;
drop view if exists sysObjects;
drop view if exists sysFunctions;
drop view if exists sysCollations;
drop view if exists sysCompileOptions;
drop view if exists sysModules;
drop view if exists sysPragmas;
drop view if exists sysIcuLocales;

drop view if exists sysDatabases;
drop view if exists sysForeignKeys;
drop view if exists sysTriggers;
drop view if exists sysIndexColumns;
drop view if exists sysIndexes;
drop view if exists sysColumns;
drop view if exists sysTables;
drop view if exists sysObjects;
drop view if exists sysFunctions;
drop view if exists sysCollations;
drop view if exists sysCompileOptions;
drop view if exists sysModules;
drop view if exists sysPragmas;
drop view if exists sysIcuLocales;

create view if not exists temp.sysDatabases
as
    select Sequence,
           ObjectSchema   collate icu_und_aici,
           FileName       collate icu_und_aici,
           isReadOnly,
           txnState,
           Application_ID,
           (
            select group_concat(char(Application_ID / pow(256, 3 - value) % 256), '')
              from generate_series(0, 3)
             where Application_ID / pow(256, 3 - value) % 256 != 0
           ) collate icu_und_aici as AppID,
           User_Version,
           (
            select start
              from (
                    select strtoll(substr(hash, start, 8), 16) % 2147483647 as start,
                           hash,
                           length
                      from (
                            select strtoll(substr(hash, start, 2), 16) % (length - 8) as start,
                                   hash,
                                   length
                              from (
                                    select strtoll(substr(hash, 1, 2), 16) % (length - 2) as start,
                                           hash,
                                           length
                                      from (
                                            select hash,
                                                   length(hash) as length
                                              from (
                                                    select hex(sha256(eval('select group_concat(sql)
                                                                              from (
                                                                                      select sql
                                                                                        from "' || ObjectSchema || '".sqlite_master
                                                                                       where sql is not null
                                                                                    order by sql
                                                                                   )'
                                                                          )
                                                                     )
                                                              ) as hash
                                                   )
                                           )
                                   )
                           )
                   )
           ) as Current_Version
      from (
            select seq    as Sequence,
                   name   as ObjectSchema,
                   file   as FileName,
                   ro     as isReadOnly,
                   txn    as txnState
              from etable.pragma_database_list
           )
cross join etable.pragma_application_id(ObjectSchema)
cross join etable.pragma_user_version(ObjectSchema)
;

create trigger temp.sysDatabase_user_version instead of update of user_version on sysDatabases
when new.ObjectSchema == old.ObjectSchema and new.Sequence == old.Sequence
begin
    select eval('pragma "' || new.ObjectSchema || '".User_Version=' || new.user_version);
end;

create trigger temp.sysDatabase_appid instead of update of AppID on sysDatabases
when new.ObjectSchema == old.ObjectSchema and new.Sequence == old.Sequence
begin
    select eval('pragma "' || new.ObjectSchema || '".Application_ID=' || (select cast(sum(unicode(substr(param, value + 1, 1)) * pow(256, 3-value)) as integer) from generate_series(0, 3), (select new.appid as param)));
end;


create view if not exists temp.sysObjects
as
select ObjectSchema   collate icu_und_aici,
       ObjectType     collate icu_und_aici,
       ObjectName     collate icu_und_aici
  from (
        select schema as ObjectSchema,
               type   as ObjectType,
               name   as ObjectName
          from etable.pragma_database_info
       );

create view if not exists temp.sysTables
as
select ObjectSchema   collate icu_und_aici,
       ObjectType     collate icu_und_aici,
       ObjectName     collate icu_und_aici,
       NumCols,
       isReadOnly,
       isEphemeral,
       isShadow,
       hasPrimaryKey,
       isRowid,
       hasNamedRowid,
       hasAutoIncrement,
       isStrict
  from (
        select schema as ObjectSchema,
               type   as ObjectType,
               name   as ObjectName,
               ncols  as NumCols,
               ro     as isReadOnly,
               eph    as isEphemeral,
               rid    as isRowid,
               pk     as hasPrimaryKey,
               ai     as hasAutoIncrement,
               named  as hasNamedRowid,
               shad   as isShadow,
               strict as isStrict
          from etable.pragma_table_list
       );

create view if not exists temp.sysColumns
as
select ObjectSchema     collate icu_und_aici,
       ObjectType       collate icu_und_aici,
       ObjectName       collate icu_und_aici,
       ColumnID         collate icu_und_aici,
       ColumnName       collate icu_und_aici,
       Type             collate icu_und_aici,
       Affinity         collate icu_und_aici,
       Collation        collate icu_und_aici,
       isNotNull,
       DefaultValue,
       isPrimaryKey,
       isRowID,
       isAutoIncrement,
       isHidden
from (
        select ObjectSchema,
               ObjectType,
               ObjectName,
               cid          as ColumnID,
               name         as ColumnName,
               type         as Type,
               aff          as Affinity,
               coll         as Collation,
               "notnull"    as isNotNull,
               dflt_value   as DefaultValue,
               pk           as isPrimaryKey,
               rowid        as isRowID,
               autoinc      as isAutoIncrement,
               Hidden       as isHidden
          from temp.sysObjects
    cross join etable.pragma_table_xinfo
            on schema = ObjectSchema
           and arg = ObjectName
         where ObjectType in ('vtable', 'etable', 'table', 'view')
     );

create view if not exists temp.sysIndexes
as
select ObjectSchema     collate icu_und_aici,
       ObjectType       collate icu_und_aici,
       ObjectName       collate icu_und_aici,
       IndexName        collate icu_und_aici,
       IndexID,
       isUniqueIndex,
       IndexOrigin      collate icu_und_aici,
       isPartialIndex
  from (
        select ObjectSchema,
               ObjectType,
               ObjectName,
               name         as IndexName,
               seq          as IndexID,
               "unique"     as isUniqueIndex,
               origin       as IndexOrigin,
               partial      as isPartialIndex
          from temp.sysObjects
    cross join etable.pragma_index_list
            on schema = ObjectSchema
           and arg = ObjectName
         where ObjectType in ('vtable', 'etable', 'table')
       );

create view if not exists temp.sysIndexColumns
as
select ObjectSchema         collate icu_und_aici,
       ObjectType           collate icu_und_aici,
       ObjectName           collate icu_und_aici,
       IndexName            collate icu_und_aici,
       IndexColumnSequence,
       ColumnID,
       ColumnName           collate icu_und_aici,
       isDescendingOrder,
       Collation            collate icu_und_aici,
       isPartOfKey
  from (
        select ObjectSchema,
               ObjectType,
               ObjectName,
               IndexName,
               seqno        as IndexColumnSequence,
               cid          as ColumnID,
               name         as ColumnName,
               "desc"       as isDescendingOrder,
               coll         as Collation,
               key          as isPartOfKey
          from temp.sysIndexes
    cross join etable.pragma_index_xinfo
            on schema = ObjectSchema
           and arg = IndexName
       );

create view if not exists temp.sysTriggers
as
select ObjectSchema         collate icu_und_aici,
       ObjectType           collate icu_und_aici,
       ObjectName           collate icu_und_aici,
       Sequence,
       TriggerName          collate icu_und_aici,
       TriggerWhen          collate icu_und_aici,
       TriggerOp            collate icu_und_aici
  from (
        select ObjectSchema,
               ObjectType,
               ObjectName,
               seq          as Sequence,
               name         as TriggerName,
               tm_tr        as TriggerWhen,
               op           as TriggerOp
          from temp.sysObjects
    cross join etable.pragma_trigger_list
            on arg == ObjectName
           and schema == ObjectSchema
         where ObjectType in ('vtable', 'etable', 'table')
       );

create view if not exists temp.sysForeignKeys
as
select ObjectSchema         collate icu_und_aici,
       ObjectType           collate icu_und_aici,
       ObjectName           collate icu_und_aici,
       Id,
       Sequence,
       FromColumn           collate icu_und_aici,
       ToTable              collate icu_und_aici,
       ToColumn             collate icu_und_aici,
       OnUpdate             collate icu_und_aici,
       OnDelete             collate icu_und_aici,
       Match                collate icu_und_aici
  from (
        select ObjectSchema,
               ObjectType,
               ObjectName,
               id           as Id,
               seq          as Sequence,
               "from"       as FromColumn,
               "table"      as ToTable,
               "to"         as ToColumn,
               on_update    as OnUpdate,
               on_delete    as OnDelete,
               match        as Match
          from temp.sysObjects
    cross join etable.pragma_foreign_key_list
            on arg == ObjectName
           and schema == ObjectSchema
         where ObjectType in ('vtable', 'etable', 'table')
       );

create view if not exists temp.sysFunctions
as
  select name         collate icu_und_aici,
         min(builtin) as builtin,
         iif(type='s','scalar',iif(type='a','aggregate','window')) collate icu_und_aici as type,
         enc          collate icu_und_aici,
         narg,
         format('0x%08x', flags) as flags,
         flags & 0x00000800 == 0x00000800 as CONSTANT,
         flags & 0x00080000 == 0x00080000 as DIRECT,
         flags & 0x00100000 == 0x00100000 as SUBTYPE,
         flags & 0x00200000 == 0x00200000 as INNOCUOUS
    from etable.pragma_function_list
group by name, enc, narg;

create view if not exists temp.sysCollations
as
  select seq,
         name collate icu_und_aici
    from etable.pragma_collation_list;

create view if not exists temp.sysCompileOptions
as
  with options(setting, length) as
       (
         select compile_options,
                instr(compile_options, '=')
           from etable.pragma_compile_options
       ),
       settings(option, value) as
       (
         select iif(length<1, setting, substr(setting, 1, length-1)),
                iif(length<1, 1, ToBestType(substr(setting, length+1)))
           from options
       )
  select option collate icu_und_aici,
         value
    from settings;

create view if not exists temp.sysModules
as
  select name collate icu_und_aici
    from etable.pragma_module_list
order by name collate icu_und_aici;

create view if not exists temp.sysPragmas
as
  select name collate icu_und_aici
    from etable.pragma_pragma_list
order by name collate icu_und_aici;

create view if not exists temp.sysIcuLocales
as select value as seq,
          replace(icu_enumlocale(value), '_', '-') as locale
     from generate_series(0, icu_enumlocale() - 1, 1);

