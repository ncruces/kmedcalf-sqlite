drop table if exists BTreeInfo;
create virtual table if not exists temp.BTreeInfo using Statement((
    select zSchema  as ObjectSchema,
           type     as ObjectType,
           name     as ObjectName,
           tbl_name as TableName,
           rootpage as RootPage,
           hasRowid,
           nEntry,
           nPage,
           depth,
           szPage
      from sqlite_btreeinfo
     where zSchema == :Schema
));

