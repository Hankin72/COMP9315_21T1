#chmod +x ./test.sh
# ./test.sh
make clean
rm -f R.bsig R.data R.info R.psig R.tsig

printf "make\n"
make
clear

printf "\n"
printf "data3 with SIMC\n"
printf "./create R simc 10100 4 1000\n"
./create R simc 10100 4 1000
printf "\n"

printf "time ./insert R < data3\n"
time ./insert R < data3
printf "\n"

printf "./select R '?,IhEUMGcIRgasiuFAEhId,?,a4-011' t\n"
./select R '?,IhEUMGcIRgasiuFAEhId,?,a4-011' t
printf "\n"

printf "./select R '?,OwqnHeBAhcDrMhlyNQJg,?,a4-012' t\n"
./select R '?,OwqnHeBAhcDrMhlyNQJg,?,a4-012' t
printf "\n"


printf "./select R '?,WJbGFGgjNPndNfquMRwT,?,a4-013' t\n"
./select R '?,WJbGFGgjNPndNfquMRwT,?,a4-013' t
printf "\n"

printf "./select R '?,?,a3-006,a4-006' t\n"
./select R '?,?,a3-006,a4-006' t
printf "\n"

rm -f R.bsig R.data R.info R.psig R.tsig
printf "make clean\n"
make clean


