#chmod +x ./test.sh
# ./test.sh
make clean
rm -f R.bsig R.data R.info R.psig R.tsig R.input
make 

printf "./create R simc 100000 5 100\n"
./create R simc 100000 5 100

printf "./gendata 90000 5 123456 13 > R.input\n"
./gendata 90000 5 123456 13 > R.input

printf "time ./insert R < R.input\n"
time ./insert R < R.input

rm -f R.bsig R.data R.info R.psig R.tsig R.input

printf "./create R simc 100000 5 100\n"
./create R simc 100000 5 100

printf "./gendata 90000 5 123456 13 > R.input\n"
./gendata 90000 5 123456 13 > R.input

printf "time ./insert R < R.input\n"
time ./insert R < R.input

rm -f R.bsig R.data R.info R.psig R.tsig R.input

printf "make clean\n"
make clean


