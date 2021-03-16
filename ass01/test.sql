create table mySets (id integer primary key, iset intSet);

insert into mySets(id, iset) values (1, '{1,2,3}');

insert into mySets(id, iset) values (2, '{1,3,1,3,1}');

insert into mySets(id, iset) values (3, '{3,4,5}');

insert into mySets(id, iset) values (4, '{4,5}');

select * from mySets order by id;

-- select id, iset, (#iset) as cardinality from mySets order by id;
--
--
-- select a.id, a.iset, b.id, b.iset, a.iset- b.iset from mySets a, mySets b where a.id<b.id;