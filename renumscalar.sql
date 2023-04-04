begin immediate;
drop table if exists temp.r;
create temporary table if not exists r
(
   old primary key,
   new
)
without rowid;
insert into temp.r
     select id as old,
            row_number() over (order by id) as new
       from scalarfunctions
      where id > 0
   order by id;
insert into scalarfunctions
     select *
       from scalarfunctions
      where id in (
                   select old
                     from temp.r
                    where old != new
                  )
   order by id
on conflict do update
        set id = (
                  select new
                    from temp.r
                   where old == excluded.id
                 );
delete from temp.r;
insert into r
     select id as old,
            row_number() over (order by id) + (select count(*) from scalarfunctions where id < 1) as new
       from scalarfunctions
      where id > 0
   order by id;
insert into scalarfunctions
     select *
      from scalarfunctions
     where id in (
                  select old
                    from temp.r
                   where old != new
                 )
   order by id desc
on conflict do update
        set id = (
                  select new
                    from temp.r
                   where old == excluded.id
                 );
delete from temp.r;
insert into temp.r
     select id as old,
            row_number() over (order by id) as new
       from scalarfunctions
      where id < 1
   order by id;
insert into scalarfunctions
     select *
       from scalarfunctions
      where id in (
                   select old
                     from temp.r
                    where old != new
                  )
   order by id desc
on conflict do update
        set id = (
                  select new
                    from temp.r
                   where old == excluded.id
                 );
drop table if exists temp.r;
commit;
