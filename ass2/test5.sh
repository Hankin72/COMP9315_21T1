#chmod +x ./test.sh
# ./test.sh
make clean
rm -f R.bsig R.data R.info R.psig R.tsig

printf "make\n"
make
clear

printf "\n"
printf "data5 with SIMC\n"
printf "./create R simc 50100 3 1000\n"
./create R simc 50100 3 1000
printf "\n"

printf "time ./insert R < data5\n"
time ./insert R < data5
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '7704517,?,?'\n"
./select R '7704517,?,?'
printf "\n"

printf "./select R '7704517,?,?' t\n"
./select R '7704517,?,?' t
printf "\n"

printf "./select R '7704517,?,?' p\n"
./select R '7704517,?,?' p
printf "\n"

printf "./select R '7704517,?,?' b\n"
./select R '7704517,?,?' b
printf "\n"

printf "./select R '?,?,a3-136'\n"
./select R '?,?,a3-136'
printf "\n"

printf "./select R '?,?,a3-136' t\n"
./select R '?,?,a3-136' t
printf "\n"

printf "./select R '?,?,a3-136' p\n"
./select R '?,?,a3-136' p
printf "\n"

printf "./select R '?,?,a3-136' b\n"
./select R '?,?,a3-136' b
printf "\n"



rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "data5 with Catc\n"
printf "./create R catc 50100 3 1000\n"
./create R catc 50100 3 1000
printf "\n"

printf "time ./insert R < data5\n"
time ./insert R < data5
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '7704517,?,?'\n"
./select R '7704517,?,?'
printf "\n"

printf "./select R '7704517,?,?' t\n"
./select R '7704517,?,?' t
printf "\n"

printf "./select R '7704517,?,?' p\n"
./select R '7704517,?,?' p
printf "\n"

printf "./select R '7704517,?,?' b\n"
./select R '7704517,?,?' b
printf "\n"

printf "./select R '?,?,a3-136'\n"
./select R '?,?,a3-136'
printf "\n"

printf "./select R '?,?,a3-136' t\n"
./select R '?,?,a3-136' t
printf "\n"

printf "./select R '?,?,a3-136' p\n"
./select R '?,?,a3-136' p
printf "\n"

printf "./select R '?,?,a3-136' b\n"
./select R '?,?,a3-136' b
printf "\n"


rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "make clean\n"
make clean


