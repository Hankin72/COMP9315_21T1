
#source ~/Documents/COMP9315_21T1/postgresql-12.5
#
#pg_ctl start -; $PGDATA/log

#source /srvr/z5216214/env
#source /srvr/z5216214/postgresql-12.5/src/tutorial/
#cp -rf /srvr/z5216214/postgresql-12.5/src/tutorial/
# 复制本地文件到postgresql的目录下

cp -rf intset.c ~/Documents/COMP9315_21T1/postgresql-12.5/src/tutorial/
cp -rf intset.source ~/Documents/COMP9315_21T1/postgresql-12.5/src/tutorial/
cp -rf Makefile ~/Documents/COMP9315_21T1/postgresql-12.5/src/tutorial/
cp -rf test.sql ~/Documents/COMP9315_21T1/postgresql-12.5/src/tutorial/

# 修改程序执行的目录
cd ~/Documents/COMP9315_21T1/postgresql-12.5/src/tutorial/


# 执行make
make

# 执行测试脚步

dropdb test
createdb test
psql test -f intset.sql
psql test -f test.sql

# 如果没有权限
# chmod a+x test.sh

