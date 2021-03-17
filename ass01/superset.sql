drop table if exists mySets;
create table mySets (id integer primary key, iset intSet);
insert into mySets values (1, '{1,2,3}');
insert into mySets values (2, '{1,3,1,3,1}');
insert into mySets values (3, '{3,4,5}');
insert into mySets values (4, '{4,5}');
insert into mySets values (5, '{6}');
insert into mySets values (6, '{}');
select * from mySets order by id;

drop table if exists tests;
create table tests (id integer primary key, iset intSet);
insert into tests values (1, '{}');
insert into tests values (2, '{3}');
insert into tests values (3, '{1,2}');
select * from tests order by id;


select a.*, b.* from mySets a, tests b
where a.iset >@ b.iset and b.id = 1;

select a.*, b.* from mySets a, tests b
where a.iset >@ b.iset and b.id = 2;

select a.*, b.* from mySets a, tests b
where a.iset >@ b.iset and b.id = 3;