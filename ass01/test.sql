

drop table if exists mySets;
create table mySets (id integer primary key, iset intSet);
insert into mySets values (1, '{1,2,3}');
insert into mySets values (2, '{1,3,1,3,1}');
insert into mySets values (3, '{3,4,5}');
insert into mySets values (4, '{4,5}');
insert into mySets values (5, '{6}');
insert into mySets values (6, '{0  }');
select * from mySets order by id;

--
-- drop table if exists mySets;
-- create table mySets (id integer primary key, iset intSet);
-- insert into mySets values (0,'');
-- insert into mySets values (1, '0');
-- insert into mySets values (2, '0 1 2');
-- insert into mySets values (3, '0,1,2');
-- insert into mySets values (4, '{');
-- insert into mySets values (5, '{1');
-- insert into mySets values (6, '{1,2,3');
-- insert into mySets values (7, '{1,');
-- insert into mySets values (8, '{1,2,3,');
-- insert into mySets values (9, '}');
-- insert into mySets values (10, '1}');
-- insert into mySets values (20, '1,2,3}');
-- insert into mySets values (30, '{a,b,c}');
-- insert into mySets values (40, '{0,1,2,a,3,4,5}');
-- insert into mySets values (50, '{1,2,three,4,5}');
-- insert into mySets values (60, '{1,2,,4,5}');
-- insert into mySets values (70, '{1,2,4,5,}');
-- insert into mySets values (80, '{1,2,3.0,4,5}');
-- insert into mySets values (90, '{1,2,-3,4,5}');
-- insert into mySets values (100, '{{1,2,4,5}');
-- insert into mySets values (110, '{{1,2,4,5}}');
-- insert into mySets values (120, '{1,2,4,5}}');
-- insert into mySets values (130, '{1 2 4 5}');
-- insert into mySets values (150, '{0}');
-- select * from mySets;
