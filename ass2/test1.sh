#chmod +x ./test.sh
# ./test.sh
make clean
rm -f R.bsig R.data R.info R.psig R.tsig

printf "make\n"
make
clear

printf "\n"
printf "data1 with SIMC\n"
printf "./create R simc 2100 3 100\n"
./create R simc 2100 3 100
printf "\n"

printf "time ./insert R < data1\n"
time ./insert R < data1
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '1234576,?,?'\n"
./select R '1234576,?,?'
printf "\n"

printf "./select R '1234576,?,?'' t\n"
./select R '1234576,?,?' t
printf "\n"

printf "./select R '1234576,?,?' p\n"
./select R '1234576,?,?' p
printf "\n"

printf "./select R '1234576,?,?' b\n"
./select R '1234576,?,?' b
printf "\n"

printf "./select R '?,?,a3-027'\n"
./select R '?,?,a3-027'
printf "\n"

printf "./select R '?,?,a3-027' t\n"
./select R '?,?,a3-027' t
printf "\n"

printf "./select R '?,?,a3-027' p\n"
./select R '?,?,a3-027' p
printf "\n"

printf "./select R '?,?,a3-027' b\n"
./select R '?,?,a3-027' b
printf "\n"


rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "data1 with Catc\n"
printf "./create R catc 2100 3 100\n"
./create R catc 2100 3 100
printf "\n"

printf "time ./insert R < data1\n"
time ./insert R < data1
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '1234576,?,?'\n"
./select R '1234576,?,?'
printf "\n"

printf "./select R '1234576,?,?'' t\n"
./select R '1234576,?,?' t
printf "\n"

printf "./select R '1234576,?,?' p\n"
./select R '1234576,?,?' p
printf "\n"

printf "./select R '1234576,?,?' b\n"
./select R '1234576,?,?' b
printf "\n"

printf "./select R '?,?,a3-027'\n"
./select R '?,?,a3-027'
printf "\n"

printf "./select R '?,?,a3-027' t\n"
./select R '?,?,a3-027' t
printf "\n"

printf "./select R '?,?,a3-027' p\n"
./select R '?,?,a3-027' p
printf "\n"

printf "./select R '?,?,a3-027' b\n"
./select R '?,?,a3-027' b
printf "\n"

rm -f R.bsig R.data R.info R.psig R.tsig
printf "make clean\n"
make clean


