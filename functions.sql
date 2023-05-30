
select DefineScalar('wdar','cosl(atanl(1./ar))*:diagonal from (select 1.0*:width/:height as ar)');

select DefineScalar('hdar','sinl(atanl(1./ar))*:diagonal from (select 1.0*:width/:height as ar)');

select DefineScalar('todec', 'dec(iif(typeof(:arg)==''real'', ftoa(:arg), :arg))');

select DefineScalar('dectoreal', 'atof(decstr(:arg))');

select DefineScalar('decipart', 'decsub(:arg, decremainder(:arg, 1))');

select DefineScalar('decfpart', 'decremainder(:arg, 1)');

select DefineScalar('dectrunc', 'decsub(:arg, decremainder(:arg, decpow(10, decsub(0, :places))))');

select DefineScalar('decround',
'decdiv(decadd(scaled, iif(frac > half, 1, iif(frac < half, 0, iif(decremainder(decdiv(scaled, 2), 1) == half, 1, 0)))), scale)
from (
        with scale(scale) as materialized
             (
              select decpow(10, ?2)
             ),
             scaled(scaled, scale) as materialized
             (
              select decmul(?1, scale),
                     scale
                from scale
             ),
             interim(scaled, frac, half, scale) as materialized
             (
              select decsub(scaled, decremainder(scaled, 1)) as scaled,
                     decremainder(scaled, 1) as frac,
                     decdiv(1, 2) as half,
                     scale
                from scaled
             )
      select *
        from interim
     )');

select DefineScalar('decmoney',
'decdiv(decadd(scaled, iif(frac > half, 1, iif(frac < half, 0, iif(decremainder(decdiv(scaled, 2), 1) == half, 1, 0)))), scale)
from (
        with scaled(scaled, scale) as materialized
             (
              select decmul(?1, 10000),
                     10000 as scale
             ),
             interim(scaled, frac, half, scale) as materialized
             (
              select decsub(scaled, decremainder(scaled, 1)) as scaled,
                     decremainder(scaled, 1) as frac,
                     decdiv(1, 2) as half,
                     scale
                from scaled
             )
      select *
        from interim
     )');

select DefineScalar('decfv','decmul(:amount, decpow(decadd(1, decdiv(:annualrate, :compoundfreq)), :periods))');

select DefineScalar('decpv','decdiv(:amount, decpow(decadd(1, decdiv(:annualrate, :compoundfreq)), :periods))');

select DefineScalar('decpvoa', 'decmul(:amount, decdiv(decsub(1, decpow(decadd(1, :rate), decsub(0, :n))), :rate))');

select DefineScalar('decpvad', 'decmul(:amount, decmul(decdiv(decsub(1, decpow(decadd(1, :rate), decsub(0, :n))), :rate), decadd(1, :rate)))');

select DefineScalar('decfvad', 'decmul(:amount, decmul(decdiv(decsub(decpow(decadd(1, :rate), :n), 1), :rate), decadd(1, :rate)))');

select DefineScalar('decfvoa', 'decmul(:amount, decdiv(decsub(decpow(decadd(1, :rate), :n), 1), :rate))');

select DefineScalar('decpvoa_pmt', 'decdiv(:amount, decdiv(decsub(1, decpow(decadd(1, :rate), decsub(0, :n))), :rate))');

select DefineScalar('decpvad_pmt', 'decdiv(:amount, decmul(decdiv(decsub(1, decpow(decadd(1, :rate), decsub(0, :n))), :rate), decadd(1, :rate)))');

select DefineScalar('decfvad_pmt', 'decdiv(:amount, decmul(decdiv(decsub(decpow(decadd(1, :rate), :n), 1), :rate), decadd(1, :rate)))');

select DefineScalar('decfvoa_pmt', 'decdiv(:amount, decdiv(decsub(decpow(decadd(1, :rate), :n), 1), :rate))');

select DefineScalar('fv',':amount *  powl(1.0 + (1.0 * :annualrate / :compoundfreq), :periods)');

select DefineScalar('pv',':amount / powl(1.0 + (1.0 * :annualrate / :compoundfreq), :periods)');

select DefineScalar('pvoa', ':amount * (1.0 - powl(1.0 + :rate, -:n)) / :rate');

select DefineScalar('pvad', ':amount * (1.0 - powl(1.0 + :rate, -:n)) / :rate * (1.0 + :rate)');

select DefineScalar('fvad', ':amount * (powl(1.0 + :rate, :n) - 1.0) / :rate * (1.0 + :rate)');

select DefineScalar('fvoa', ':amount * (powl(1.0 + :rate, :n) - 1.0) / :rate');

select DefineScalar('pvoa_pmt', ':amount / (1.0 - powl(1.0 + :rate, -:n)) * :rate');

select DefineScalar('pvad_pmt', ':amount / (1.0 - powl(1.0 + :rate, -:n)) * :rate / (1.0 + :rate)');

select DefineScalar('fvad_pmt', ':amount / (powl(1.0 + :rate, :n) - 1.0) * :rate / (1.0 + :rate)');

select DefineScalar('fvoa_pmt', ':amount / (powl(1 + :rate, :n) - 1) * :rate');

select DefineScalar('FixDate',
'format(''%04d-%02d-%02d'', cast(substr(:date, 1, instr(:date, :sep) - 1) as integer),
                         cast(trim(trim(:date, ''0123456789''), :sep) as integer),
                         cast(ltrim(ltrim(ltrim(ltrim(:date, ''0123456789''), :sep), ''0123456789''), :sep) as integer)
      )');

select DefineScalar('FixTime',
'format(''%02d:%02d:%06.3f'', cast(substr(:time, 1, instr(:time, :sep) - 1) as integer),
                           cast(trim(trim(:time, ''0123456789.''), :sep) as integer),
                           cast(ltrim(ltrim(ltrim(ltrim(:time, ''0123456789''), :sep), ''0123456789''), :sep) as real)
      )');

select DefineScalar('FixDateTime',
'format(''%s %s'', fixdate(substr(:datetime, 1, instr(:datetime, :dtsep) - 1), :datesep),
                fixtime(ltrim(substr(:datetime, instr(:datetime, :dtsep)), :dtsep), :timesep)
      )');

select DefineScalar('Right', 'substring(:value, -:position)');

select DefineScalar('Left', 'substring(:value, 1, :length)');

select DefineScalar('hostname',
'substring(fqdn, 1, iif(len < 1, length(fqdn), len - 1))
from (
        select :fqdn as fqdn,
               instr(:fqdn, ''.'') as len
     )');

select DefineScalar('SchemaSlug',
'start
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
                                        select hex(sha256(eval(''select group_concat(sql)
                                                                   from (
                                                                           select sql
                                                                             from "'' || :schema || ''".sqlite_master
                                                                            where sql is not null
                                                                         order by sql
                                                                        )''
                                                              )
                                                         )
                                                  ) as hash
                                       )
                               )
                       )
               )
       )');

select DefineScalar('AppID',
'case when typeof(param) == ''text''
      then cast(sum(unicode(substr(param, value + 1, 1)) * pow(256, 3-value)) as integer)
      else group_concat(char(param / pow(256, 3-value) % 256), '''')
  end
from generate_series(0, 3), (select :param as param )
where iif(typeof(param) != ''text'', param / pow(256, 3 - value) % 256 != 0, 1)');

