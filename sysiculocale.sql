drop view if exists sysIcuLocales;
drop view if exists sysIcuLocales;
create view if not exists temp.sysIcuLocales
as select value as seq,
          icu_enumlocale(value) as locale
     from generate_series
    where start = 1
      and stop = icu_enumlocale();

