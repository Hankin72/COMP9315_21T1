#chmod +x ./test.sh
# ./test.sh
make clean
rm -f R.bsig R.data R.info R.psig R.tsig

printf "make\n"
make
clear

printf "\n"
printf "data4 with SIMC\n"
printf "./create R simc 20100 5 1000\n"
./create R simc 20100 5 1000
printf "\n"

printf "time ./insert R < data4\n"
time ./insert R < data4
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '4321263,?,?,?,?'\n"
./select R '4321263,?,?,?,?'
printf "\n"

printf "./select R '4321263,?,?,?,?' t\n"
./select R '4321263,?,?,?,?' t
printf "\n"

printf "./select R '4321263,?,?,?,?' p\n"
./select R '4321263,?,?,?,?' p
printf "\n"

printf "./select R '4321263,?,?,?,?' b\n"
./select R '4321263,?,?,?,?' b
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036'\n"
./select R '?,?,a3-036,?,a5-036'
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036' t\n"
./select R '?,?,a3-036,?,a5-036' t
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036' p\n"
./select R '?,?,a3-036,?,a5-036' p
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036' b\n"
./select R '?,?,a3-036,?,a5-036' b
printf "\n"



rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "data4 with Catc\n"
printf "./create R catc 20100 5 1000\n"
./create R catc 20100 5 1000
printf "\n"

printf "time ./insert R < data4\n"
time ./insert R < data4
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '4321263,?,?,?,?'\n"
./select R '4321263,?,?,?,?'
printf "\n"

printf "./select R '4321263,?,?,?,?' t\n"
./select R '4321263,?,?,?,?' t
printf "\n"

printf "./select R '4321263,?,?,?,?' p\n"
./select R '4321263,?,?,?,?' p
printf "\n"

printf "./select R '4321263,?,?,?,?' b\n"
./select R '4321263,?,?,?,?' b
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036'\n"
./select R '?,?,a3-036,?,a5-036'
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036' t\n"
./select R '?,?,a3-036,?,a5-036' t
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036' p\n"
./select R '?,?,a3-036,?,a5-036' p
printf "\n"

printf "./select R '?,?,a3-036,?,a5-036' b\n"
./select R '?,?,a3-036,?,a5-036' b
printf "\n"


rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "make clean\n"
make clean


