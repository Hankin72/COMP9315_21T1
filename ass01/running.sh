
#ssh grieg

cp -rf intset.c /srvr/z5216214/postgresql-12.5/src/tutorial/
cp -rf intset.source /srvr/z5216214/postgresql-12.5/src/tutorial/




cd /srvr/z5216214/testing/
cp ../postgresql-12.5/src/tutorial/intset.c .
cp ../postgresql-12.5/src/tutorial/intset.source .


source /srvr/z5216214/env


#source /srvr/z5216214/env

./run_test.py

 # Lots of output
# Run all tests in the tests/ directory but only show ones that are failing: $
./run_test.py TRUE