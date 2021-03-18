

##source /srvr/z5216214/env
##source /srvr/z5216214/postgresql-12.5/src/tutorial/
#






# 复制本地文件到postgresql的目录下

#cp -rf intset.c /srvr/z5216214/postgresql-12.5/src/tutorial/
#cp -rf intset.source /srvr/z5216214/postgresql-12.5/src/tutorial/
#cp -rf Makefile /srvr/z5216214/postgresql-12.5/src/tutorial/
#cp -rf test.sql /srvr/z5216214/postgresql-12.5/src/tutorial/


cp -rf intset.c /srvr/z5216214/postgresql-12.5/src/tutorial/
cp -rf intset.source ~/postgresql-12.5/src/tutorial/

cp -rf Makefile ~/postgresql-12.5/src/tutorial/
cp -rf test.sql ~/postgresql-12.5/src/tutorial/


source /Users/guohaojin/postgresql/env

pg1
psql -l
#pg_ctl start -l $PGHOME/log
# 修改程序执行的目录
cd ~/postgresql-12.5/src/tutorial/


# 执行make
make

# 执行测试脚步

dropdb test
createdb test
psql test -f intset.sql
psql test -f test.sql

pg_ctl stop;

# 如果没有权限
# chmod a+x test.sh
# 执行./test.sh
