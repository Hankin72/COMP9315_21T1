
ssh grieg

source /srvr/z5216214/env
$ mkdir -p /srvr/z5216214/testing/
$ cd /srvr/z5216214/testing/
$ cp -P /web/cs9315/21T1/assignments/ass1/testing/tests .
$ cp -P /web/cs9315/21T1/assignments/ass1/testing/run_test.py .
$ cp -P /web/cs9315/21T1/assignments/ass1/testing/Makefile .
$ cp -P /web/cs9315/21T1/assignments/ass1/testing/intset.drop.sql .


cd srvr/z5216214/testing/

cp -rf intset.c /srvr/z5216214/postgresql-12.5/src/tutorial/
cp -rf intset.source /srvr/z5216214/postgresql-12.5/src/tutorial/


#ssh grieg
#cd /srvr/z5216214/testing/
source /srvr/z5216214/env
./run_test.py --timeout
