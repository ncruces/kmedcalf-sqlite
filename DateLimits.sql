drop table if exists datelimits;
create virtual table if not exists temp.Datelimits using statement((
  with inputs(year, zone, count, dow, iso) as
       (
          select cast(:year as integer),
                 coalesce(:zone, 'localtime'),
                 cast(coalesce(:count, 1) as integer),
                 cast(iif(:yearly, -3, iif(:quarterly, -2, iif(:monthly, -1, iif(:weekly and :dow is null, iif(:iso is True, 1, 0), :dow)))) as integer),
                 (:iso is True)
       ),
       constants(yearlower, yearupper, base, zone, unzone, zoneaware, ianazone, count, dow, jms, iso) as
       (
          select year,
                 year + count - 1,
                 julianday(format('%04d-01-01 00:00:00 Z', year), '-14 days'),
                 zone,
                 iif(instr(zone,'/'), '-' || zone, 'utc'),
                 strftime('%Z') is not null,
                 instr(zone,'/') > 0,
                 count * 365.25 + 28,
                 dow,
                 1.0 / 86400000.0,
                 iso
            from inputs
       ),
       jdall(jd) as
       (
           select distinct julianday(base, 'julianday', zone, value || ' day', 'start of day')
             from constants, generate_series
            where zoneaware and ianazone
              and start == 0
              and stop == count
        union all
           select distinct julianday(base, 'julianday', zone, value || ' day', 'start of day', 'naive', unzone)
             from constants, generate_series
            where zoneaware and not ianazone
              and start == 0
              and stop == count
        union all
           select distinct julianday(base, 'julianday', zone, value || ' day', 'start of day', unzone)
             from constants, generate_series
            where not zoneaware
              and start == 0
              and stop == count
       ),
       jds(jd, selector) as
       (
                -- default all days
                 select jd,
                        case when iso then strftime('%i', jd, 'julianday', zone)
                                      else strftime('%Y-%m-%d', jd, 'julianday', zone)
                        end
                   from constants, jdall
                  where dow is null
          union all
                -- dow
                 select jd,
                        case when dow == 0 then strftime('%Y-W%W', jd, 'julianday', zone)
                                           else strftime('%Y dow=%w', jd, 'julianday', zone)
                        end
                   from constants, jdall
                  where iso == 0
                    and dow > -1
                    and cast(strftime('%w', jd, 'julianday', zone) as integer) == dow
          union all
                -- monthly=True
                 select jd,
                        strftime('%Y-M%m', jd, 'julianday', zone)
                   from constants, jdall
                  where iso == 0
                    and dow == -1
                    and cast(strftime('%d', jd, 'julianday', zone) as integer) == 1
          union all
                -- quarterly=True
                 select jd,
                        strftime('%m%d', jd, 'julianday', zone)
                   from constants, jdall
                  where iso == 0
                    and dow == -2
                    and cast(strftime('%m%d', jd, 'julianday', zone) as integer) in (101, 401, 701, 1001)
          union all
                -- yearly=True
                 select jd,
                        strftime('%Y', jd, 'julianday', zone)
                   from constants, jdall
                  where iso == 0
                    and dow == -3
                    and cast(strftime('%m%d', jd, 'julianday', zone) as integer) == 101
          union all
                -- iso=True dow
                 select jd,
                        case when dow == 1 then strftime('%I', jd, 'julianday', zone)
                                           else strftime('%G dow=%w', jd, 'julianday', zone)
                        end
                   from constants, jdall
                  where iso == 1
                    and dow > 0
                    and cast(strftime('%u', jd, 'julianday', zone) as integer) == dow
          union all
                -- iso=True monthly=True
                 select min(jd),
                        strftime('%X', jd, 'julianday', zone)
                   from constants, jdall
                  where iso == 1
                    and dow == -1
               group by strftime('%X', jd, 'julianday', zone)
          union all
                -- iso=True quarterly=True
                 select min(jd),
                        strftime('%q', jd, 'julianday', zone)
                   from constants, jdall
                  where iso == 1
                    and dow == -2
               group by strftime('%q', jd, 'julianday', zone)
          union all
                -- iso=True yearly=True
                 select min(jd),
                        strftime('%G', jd, 'julianday', zone)
                   from constants, jdall
                  where iso == 1
                    and dow == -3
               group by cast(strftime('%G', jd, 'julianday', zone) as integer)
       ),
       boundaries(isISO, selector, startjd, stopjd) as
       (
          select iso is True,
                 case when iso == 0 and dow == -2
                      then strftime('%Y-Q', jd, 'julianday', zone) || iif(selector=='0101', '1', iif(selector=='0401', '2', iif(selector=='0701', '3', '4')))
                      else selector
                 end,
                 jd,
                 lead(jd) over () - jms
            from constants, jds
       ),
       limits(isISO, selector, start, stop, startjd, stopjd, inTransition) as
       (
          select isISO,
                 selector,
                 datetime(startjd, 'julianday', zone),
                 datetime(stopjd, 'julianday', zone),
                 startjd,
                 stopjd,
                 cast(((stopjd - startjd) * 86400.0 + 0.5) as integer) % 86400 != 0 as isTransition
            from constants, boundaries
           where (iso == 0 and (
                                   likely(cast(strftime('%Y', startjd, 'julianday', zone) as integer) between yearlower and yearupper)
                                or unlikely(cast(strftime('%Y', stopjd,  'julianday', zone) as integer) between yearlower and yearupper)
                               ))
              or (iso != 0 and cast(strftime('%G', startjd, 'julianday', zone) as integer) between yearlower and yearupper)
       )
select isIso,
       selector,
       start,
       stop,
       startjd,
       stopjd,
       inTransition
  from limits
));
