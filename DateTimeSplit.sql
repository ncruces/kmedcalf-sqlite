drop table if exists DateTimeSplit;
create virtual table if not exists temp.DateTimeSplit using statement((
  with datetime(dt) as
       (
          select datetime(:datetime, '+0 seconds')
       ),
       days(WeekDay, DayName) as
       (
          values (0, 'Sunday'),
                 (1, 'Monday'),
                 (2, 'Tuesday'),
                 (3, 'Wednesday'),
                 (4, 'Thusday'),
                 (5, 'Friday'),
                 (6, 'Saturday')
       ),
       months(Month, MonthName) as
       (
          values ( 1, 'January'),
                 ( 2, 'February'),
                 ( 3, 'March'),
                 ( 4, 'April'),
                 ( 5, 'May'),
                 ( 6, 'June'),
                 ( 7, 'July'),
                 ( 8, 'August'),
                 ( 9, 'September'),
                 (10, 'October'),
                 (11, 'November'),
                 (12, 'December')
       )
select dt                                  as DT,
       cast(strftime('%Y', dt) as integer) as Year,
       months.Month                        as Month,
       cast(strftime('%d', dt) as integer) as Day,
       cast(strftime('%H', dt) as integer) as Hour,
       cast(strftime('%M', dt) as integer) as Minute,
       cast(strftime('%f', dt) as real)    as Second,
       strftime('%Z', dt)                  as Zone,
       cast(strftime('%z', dt) as integer) as Offset,
       cast(strftime('%j', dt) as integer) as DoY,
       cast(strftime('%W', dt) as integer) as Week,
       WeekDay                             as DoW,
       cast(strftime('%G', dt) as integer) as ISOYear,
       isomonths.Month                     as ISOMonth,
       cast(strftime('%V', dt) as integer) as ISOWeek,
       cast(strftime('%Q', dt) as integer) as ISOQuarter,
       ((WeekDay + 6) % 7 ) + 1            as ISODoW,
       unixepoch(dt)                       as UnixEpoch,
       julianday(dt)                       as JulianDay,
       DayName                             as DayName,
       months.MonthName                    as MonthName,
       isomonths.MonthName                 as ISOMonthName
  from datetime, days, months, months as isomonths
 where WeekDay == cast(strftime('%w', dt) as integer)
   and months.Month == cast(strftime('%m', dt) as integer)
   and isomonths.Month == cast(strftime('%x', dt) as integer)
));

