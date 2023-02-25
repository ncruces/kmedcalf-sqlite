select quote(nextup(column1)) as NextUp,
       quote(nextup(column1)-column1) as UlpUp,
       quote(column1) as Exactly,
       cast(column1 as text) as CastToText,
       quote(nextdown(column1)-column1) as UlpDown,
       quote(nextdown(column1)) as NextDown,
       quote(ulp(column1)) as Ulp
  from (
        values (pi()),
               (13.0),
               (25.0)
       )
;

