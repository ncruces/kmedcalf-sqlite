drop table if exists split;
create virtual table if not exists temp.split using statement((
   with recursive
        input(data, sep) as
        (
           values (:data, coalesce(:sep, ','))
        ),
        tokens(token, data, sep, seplen, pos, isValid) as
        (
           select null,
                  data,
                  sep,
                  length(sep),
                  instr(data, sep),
                  false
             from input
         union all
            select substr(data, 1, pos - 1),
                   substr(data, pos + seplen),
                   sep,
                   seplen,
                   -1,
                   true
              from tokens
             where pos > 0
         union all
            select null,
                   data,
                   sep,
                   seplen,
                   instr(data, sep),
                   false
              from tokens
             where pos < 0
         union all
            select data,
                   null,
                   sep,
                   seplen,
                   null,
                   true
              from tokens
            where pos == 0
       )
select ToBestType(token) as value
  from tokens
 where isValid
));

--DROP TABLE IF EXISTS temp.tmpssv;
--CREATE TABLE IF NOT EXISTS temp.tmpssv
--(
--    ID      INTEGER PRIMARY KEY,
--    keyCol  TEXT,
--    ssvCol  TEXT
--);

--INSERT INTO temp.tmpssv (ID, keyCol, ssvCol) VALUES
--    (1, 'foo', '4 66 51 3009 2 678')
--  , (2, 'bar', 'Sputnik Discovery')
--  , (3, 'baz', '101 I-95 104')
--  , (4, 'foz', 'Amsterdam Beijing London Moscow NewYork Paris Tokyo')
--  , (5, 'one', 'John')
--;

--select id, keycol, value from tmpssv cross join split(ssvCol, ' ');

