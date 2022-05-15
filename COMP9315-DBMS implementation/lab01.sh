#!/bin/sh

echo "使用ssh zid@grieg.cse.unsw.edu.au登录到grieg，然后再使用该脚本"
echo "更多资料：请添加微信:marey_marey111"

if test -z "$1"
then
  echo "请输出你的学号！！命令执行格式是：./lab01.sh z8888888"
else
  pg_ctl stop
  rm -rf cd /srvr/$1
  priv srvr

  cd /srvr/$1
  tar xfj /web/cs9315/21T1/postgresql/postgresql-12.5.tar.bz2
  cd postgresql-12.5
  ./configure --prefix=/srvr/$1/pgsql
  make
  make install
  cp /web/cs9315/21T1/postgresql/env /srvr/$1/env
  source /srvr/$1/env
  which initdb
  /srvr/$1/pgsql/bin/initdb
  initdb
  ls $PGDATA

  sed -i "s/#listen_addresses = 'localhost'/listen_addresses = ''/g" $PGDATA/postgresql.conf
  sed -i "s/max_connections = 100/max_connections = 8/g" $PGDATA/postgresql.conf
  sed -i "s/#max_wal_senders = 10/max_wal_senders = 4/g" $PGDATA/postgresql.conf
  sed -i "s+#unix_socket_directories = '/tmp'+unix_socket_directories = '/srvr/$1/pgsql/data'+g" $PGDATA/postgresql.conf

  which pg_ctl
  pg_ctl start -l $PGDATA/log
  psql -l

fi

