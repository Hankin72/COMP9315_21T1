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
insert into tests select * from mySets;
update tests set iset = iset || '{}';
select * from tests;

drop table if exists tests;
create table tests (id integer primary key, iset intSet);
insert into tests select * from mySets;
update tests set iset = iset || '{10}';
select * from tests;

drop table if exists tests;
create table tests (id integer primary key, iset intSet);
insert into tests select * from mySets;
update tests set iset = iset || '{1, 2}';
select * from tests;

drop table if exists tests;
create table tests (id integer primary key, iset intSet);
insert into tests select * from mySets;
update tests set iset = iset || '{1, 3}';
select * from tests;

drop table if exists tests;
create table tests (id integer primary key, iset intSet);
insert into tests select * from mySets;
update tests set iset = iset || '{7,8,9,10}';
select * from tests;