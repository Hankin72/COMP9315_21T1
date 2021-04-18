#chmod +x ./test.sh
# ./test.sh
make clean
rm -f R.bsig R.data R.info R.psig R.tsig
printf "make\n"
make
clear

printf "\n"
printf "data2 with SIMC\n"
printf "./create R simc 5100 5 100\n"
./create R simc 5100 5 100
printf "\n"

printf "time ./insert R < data2\n"
time ./insert R < data2
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '1235736,?,?,?,?'\n"
./select R '1235736,?,?,?,?'
printf "\n"

printf "./select R '1235736,?,?,?,?'' t\n"
./select R '1235736,?,?,?,?' t
printf "\n"

printf "./select R '1235736,?,?,?,?' p\n"
./select R '1235736,?,?,?,?' p
printf "\n"

printf "./select R '1235736,?,?,?,?' b\n"
./select R '1235736,?,?,?,?' b
printf "\n"

printf "./select R '?,?,a3-027,?,?'\n"
./select R '?,?,a3-027,?,?'
printf "\n"

printf "./select R '?,?,a3-027,?,?' t\n"
./select R '?,?,a3-027,?,?' t
printf "\n"

printf "./select R '?,?,a3-027,?,?' p\n"
./select R '?,?,a3-027,?,?' p
printf "\n"

printf "./select R '?,?,a3-027,?,?' b\n"
./select R '?,?,a3-027,?,?' b
printf "\n"


rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "data2 with Catc\n"
printf "./create R catc 5100 5 100\n"
./create R catc 5100 5 100
printf "\n"

printf "time ./insert R < data2\n"
time ./insert R < data2
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '1235736,?,?,?,?'\n"
./select R '1235736,?,?,?,?'
printf "\n"

printf "./select R '1235736,?,?,?,?'' t\n"
./select R '1235736,?,?,?,?' t
printf "\n"

printf "./select R '1235736,?,?,?,?' p\n"
./select R '1235736,?,?,?,?' p
printf "\n"

printf "./select R '1235736,?,?,?,?' b\n"
./select R '1235736,?,?,?,?' b
printf "\n"

printf "./select R '?,?,a3-027,?,?'\n"
./select R '?,?,a3-027,?,?'
printf "\n"

printf "./select R '?,?,a3-027,?,?' t\n"
./select R '?,?,a3-027,?,?' t
printf "\n"

printf "./select R '?,?,a3-027,?,?' p\n"
./select R '?,?,a3-027,?,?' p
printf "\n"

printf "./select R '?,?,a3-027,?,?' b\n"
./select R '?,?,a3-027,?,?' b
printf "\n"

rm -f R.bsig R.data R.info R.psig R.tsig
printf "make clean\n"
make clean


