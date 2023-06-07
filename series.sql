drop table if exists iseries;
create virtual table if not exists temp.iseries using statement((
  with param(start, stop, step, count) as materialized
       (
           select start,
                  stop,
                  iif(start < stop, step, -step),
                  (abs(start - stop) / step) + 1
             from (
                   select cast(start as integer) as start,
                          cast(stop as integer) as stop,
                          cast(max(1, step) as integer) as step
                     from (
                           select coalesce(:start, 1) as start,
                                  coalesce(:stop, 9223372036854775807) as stop,
                                  abs(coalesce(:step, 1)) as step
                          )
                  )
       ),
       series(value) as
       (
           select start
             from param
        union all
           select value + (select step from param)
             from series
            limit (select count from param)
       )
select value
  from series
));
drop table if exists rseries;
create virtual table if not exists temp.rseries using statement((
  with param(start, stop, step, count) as materialized
       (
           select start,
                  stop,
                  iif(start < stop, step, -step),
                  min(9223372036854775807, cast((abs(start - stop) / step) as integer) + 1)
             from (
                   select sigdigits(start) as start,
                          sigdigits(stop) as stop,
                          sigdigits(iif(step == 0, 1, step)) as step
                     from (
                           select coalesce(:start, 1) as start,
                                  coalesce(:stop, 9223372036854775807) as stop,
                                  abs(coalesce(:step, 1)) as step
                          )
                  )
       ),
       series(value) as
       (
           select start
             from param
        union all
           select sigdigits(value + (select step from param))
             from series
            limit (select count from param)
       )
select value
  from series
));
