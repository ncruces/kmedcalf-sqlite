drop table if exists DateTimeDiff;
create virtual table temp.DateTimeDiff using statement((
  /*
      There are two date.c modules this is designed to work with, either
      one may be used.

      The default date.c module distributed as part of standard
      distribution of SQLite3 makes certain assumptions about the
      formatting and maintains the internal JD number relative to an
      arbitrary Meridian and not relative to the Prime Meridian (UT).
      While localization (timezone) is parsed, it is used to compute the
      JD number relative to the Prime Meridian, and then the information
      is discarded.  datetime(x, '+0 seconds') == x is false if x is a
      properly formatted string which contains a timezone other than Z,
      -00:00 and +00:00. There is no capability for a datetime object to
      know its localization.

      There is a replacement date.c module which always maintains the JD
      number relative to the Prime Meridian (UT) and maintains the
      localization offset, whenever possible.  All datetime objects
      therefore identify (wherever possible) an instant-in-time.
      Additionally input/output is transparent and datetime(x, '+0
      seconds') == x is always true if x is a properly formed string
      complying with the ms and zone capabilities used.

      When using non-localized datetimes, they must both be relative to
      the same arbitrary Meridian or the result will include the
      rotational offset between the two arbitrary Meridians (4 minutes
      per degree). Provided that the start and stop datetime's are both
      relative to the same Meridian (Timezone) the results will be
      correct. Either the default (non-localized) or the localized
      date.c module may be used.
  */
  with recursive
       /*
           Ensure that the provided start and stop datetime's are valid
           or null.

           Use strftime '%Z' format code to test if the datetime module
           is localized. The localized strftime recogzines %Z and
           returns a text string containing the timezone offset or an
           empty string. The default non-localized strftime function
           does not recognize the '%Z' format specifier and returns
           NULL.

           If date.c is not localized, or if localized and a timezone is
           set, use the datetime given; otherwise if the datetime is not
           localized but the localized date.c is being used, set
           localization to the Prime Meridian (UT) which is the default
           assumption of the non-localized date.c

           Compute the direction to move from the final datetime values
       */
       timeTable(startDate, endDate, incr) as materialized
       (
            select startDate,
                   endDate,
                   iif(julianday(startDate) > julianday(endDate), -1, 1) as incr
              from (
                        select iif(startZone != '', startDate, datetime(startDate, '0 offset')) as startDate,
                               iif(endZone   != '', endDate,   datetime(endDate,   '0 offset')) as endDate
                          from (
                                   select startDate,
                                          coalesce(strftime('%Z', startDate), 'Z') as startZone,
                                          endDate,
                                          coalesce(strftime('%Z', endDate), 'Z') as endZone
                                     from (
                                                select datetime(:start, '+0 days') as startDate,
                                                       datetime(:stop,  '+0 days') as endDate
                                          )
                               )
                   )
       ),
       /*
           Recursively increment the years, months, days, hours, minutes
           Each substep terminates when resultDate is beyond the endDate
           This method appears to get past some computational problems
           and permits the entire range ('-4713-11-24 12:00:00.000 Z' to
           '9999-12-31 23:59:59.999 Z') of datetimes to be used.
       */
       ymdhmTable(startDate, endDate, incr, years, months, days, hours, minutes, resultDate) as
       (
           /*
               Initialize the recursion
           */
           select startDate,
                  endDate,
                  incr,
                  0 as years,
                  0 as months,
                  0 as days,
                  0 as hours,
                  0 as minutes,
                  startDate as resultDate
             from timeTable
        union all
           /*
               Increment the years if resultDate is not beyond endDate
               and the months/days/hours/minutes are zero.
           */
           select startDate,
                  endDate,
                  incr,
                  years + incr as years,
                  months,
                  days,
                  hours,
                  minutes,
                  datetime(startDate, format('%+d years',  years + incr)) as resultDate
             from ymdhmTable
            where case when incr > 0 then julianday(resultDate) < julianday(endDate)
                                     else julianday(resultDate) > julianday(endDate)
                  end
              and months = 0
              and days = 0
              and hours = 0
              and minutes = 0
        union all
           /*
               Increment the months if there is more than a month and
               less than a year between the resultDate and the endDate
               and the resultDate is not yet beyond the endDate and
               days/hours/minutes are zero.
           */
           select startDate,
                  endDate,
                  incr,
                  years,
                  months + incr as months,
                  days,
                  hours,
                  minutes,
                  datetime(startDate, format('%+d years',  years),
                                      format('%+d months', months + incr)) as resultDate
             from ymdhmTable
            where case when incr > 0 then julianday(resultDate) < julianday(endDate)
                                     else julianday(resultDate) > julianday(endDate)
                  end
              and (julianday(endDate) - julianday(resultDate))*incr between 28 and 366
              and days = 0
              and hours = 0
              and minutes = 0
        union all
           /*
               Increment the days if there is less that a month between
               the resultDate and the endDate and the resultDate is not
               yet beyond the endDate, and the hours and minutes are
               zero.
           */
           select startDate,
                  endDate,
                  incr,
                  years,
                  months,
                  days + incr as days,
                  hours,
                  minutes,
                  datetime(startDate, format('%+d years',  years),
                                      format('%+d months', months),
                                      format('%+d days',   days + incr)) as resultDate
             from ymdhmTable
            where case when incr > 0 then julianday(resultDate) < julianday(endDate)
                                     else julianday(resultDate) > julianday(endDate)
                  end
              and (julianday(endDate) - julianday(resultDate))*incr < 31
              and hours = 0
              and minutes = 0
        union all
           /*
               Increment the hours if there is less that a day between
               the resultDate and the endDate and the resultDate is not
               yet beyond the endDate, and the minutes are zero.
           */
           select startDate,
                  endDate,
                  incr,
                  years,
                  months,
                  days,
                  hours + incr as hours,
                  minutes,
                  datetime(startDate, format('%+d years',  years),
                                      format('%+d months', months),
                                      format('%+d days',   days),
                                      format('%+d hours',  hours + incr)) as resultDate
             from ymdhmTable
            where case when incr > 0 then julianday(resultDate) < julianday(endDate)
                                     else julianday(resultDate) > julianday(endDate)
                  end
              and cast((julianday(endDate) - julianday(resultDate))*incr*24.0 as integer) < 24
              and minutes = 0
        union all
           /*
               Increment the minutes if there is less than an hour between
               the resultDate and the endDate and the resultDate is not
               yet beyond the endDate.
           */
           select startDate,
                  endDate,
                  incr,
                  years,
                  months,
                  days,
                  hours,
                  minutes + incr as minutes,
                  datetime(startDate, format('%+d years',  years),
                                      format('%+d months', months),
                                      format('%+d days',   days),
                                      format('%+d hours',  hours),
                                      format('%+d minutes',minutes + incr)) as resultDate
             from ymdhmTable
            where case when incr > 0 then julianday(resultDate) < julianday(endDate)
                                     else julianday(resultDate) > julianday(endDate)
                  end
              and cast((julianday(endDate) - julianday(resultDate))*incr*1440.0 as integer) < 60
       ),
       /*
           The best estimate occurs where the total minutes is at it
           greatest value, all the values are in range and the
           resultDate is not past the endDate.

           Select that record from the recursion results and compute the
           seconds still remaining between the endDate and the
           resultDate then update the resultDate.
       */
       ymdhmsTable(startDate, endDate, incr, years, months, days, hours, minutes, seconds, resultDate) as
       (
           select startDate,
                  endDate,
                  incr,
                  years,
                  months,
                  days,
                  hours,
                  minutes,
                  seconds,
                  datetime(startDate, format('%+d years',  years),
                                      format('%+d months', months),
                                      format('%+d days',   days),
                                      format('%+d hours',  hours),
                                      format('%+d minutes',minutes),
                                      format('%+f seconds',seconds)) as resultDate
             from (
                      select startDate,
                             endDate,
                             incr,
                             years,
                             months,
                             days,
                             hours,
                             minutes,
                             round((julianday(endDate) - julianday(resultDate)) * 86400.0, 3) as seconds,
                             resultDate,
                             max(((((years * 12 + months) * 31 + days) * 24 + hours) * 60 + minutes) * incr)
                        from ymdhmTable
                       where case when incr > 0 then julianday(resultDate) <= julianday(endDate)
                                                else julianday(resultDate) >= julianday(endDate)
                             end
                         and months  between -11 and 11
                         and days    between -30 and 30
                         and hours   between -23 and 23
                         and minutes between -59 and 59
                         and (julianday(endDate) - julianday(resultDate)) * 86400.0 * incr < 60
                  )
       ),
       /*
            Sometimes there appears to be a rounding error.  This eliminates those errors.
       */
       finalTable(startDate, endDate, incr, years, months, days, hours, minutes, seconds, resultDate) as
       (
           select startDate,
                  endDate,
                  incr,
                  years,
                  months,
                  days,
                  hours,
                  minutes,
                  seconds + error,
                  datetime(startDate, format('%+d years',  years),
                                      format('%+d months', months),
                                      format('%+d days',   days),
                                      format('%+d hours',  hours),
                                      format('%+d minutes',minutes),
                                      format('%+f seconds',seconds + error)) as resultDate
             from (
                      select startDate,
                             endDate,
                             incr,
                             years,
                             months,
                             days,
                             hours,
                             minutes,
                             seconds,
                             resultDate,
                             round((julianday(endDate) - julianday(resultDate)) * 86400.0, 3) as error
                        from ymdhmsTable
                  )
       )
/*
** Return the result row and isValid indicator
*/
select startDate,
       endDate,
       incr,
       years,
       months,
       days,
       hours,
       minutes,
       seconds,
       resultDate,
       julianday(endDate) == julianday(resultDate) as isValid,
       round((julianday(endDate) - julianday(resultDate)) * 86400.0, 3) as error
  from finalTable
));

--select * from datetimediff where start='1963-04-26 12:00:00' and stop='2022-04-16 00:00:00';
--select * from datetimediff where start='2022-04-16 00:00:00' and stop='1963-04-26 12:00:00';
--select * from datetimediff where start='1963-04-26 12:00:00 -05:00' and stop=datetime('now');
--select * from datetimediff where start='1963-04-26 12:00:00 -05:00' and stop=datetime('now','localtime');

--.mode list
--.print Very Long Random Error checking in Progress -- only errors will be shown -- checking entire range
--     select *
--      from wholenumber
--cross join datetimediff
--     where value between 1 and 1000000
--       and start = datetime(randomv()*4967701.5, 'julianday')
--       and stop = datetime(randomv() *4967701.5, 'julianday')
--       and not (isValid or error == 0.0)
--;
--.print Very Long Random Error checking in Progress -- only errors will be shown -- checking CE only
--    select *
--      from wholenumber
--cross join datetimediff
--     where value between 1 and 1000000
--       and start = datetime(randomv()*3652059 + 1721425.5, 'julianday')
--       and stop = datetime(randomv() *3652059 + 1721425.5, 'julianday')
--       and not (error == 0.0 and isValid)
--;

