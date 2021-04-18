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

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '7663852,?,?,?'\n"
./select R '7663852,?,?,?'
printf "\n"

printf "./select R '7663852,?,?,?' t\n"
./select R '7663852,?,?,?' t
printf "\n"

printf "./select R '7663852,?,?,?' p\n"
./select R '7663852,?,?,?' p
printf "\n"

printf "./select R '7663852,?,?,?' b\n"
./select R '7663852,?,?,?' b
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?'\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?'
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' t\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' t
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' p\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' p
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' b\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' b
printf "\n"


printf "./select R '?,?,a3-242,a4-242'\n"
./select R '?,?,a3-242,a4-242'
printf "\n"

printf "./select R '?,?,a3-242,a4-242' t\n"
./select R '?,?,a3-242,a4-242' t
printf "\n"

printf "./select R '?,?,a3-242,a4-242' p\n"
./select R '?,?,a3-242,a4-242' p
printf "\n"

printf "./select R '?,?,a3-242,a4-242' b\n"
./select R '?,?,a3-242,a4-242' b
printf "\n"


printf "./select R '8765432,?,?,?'\n"
./select R '8765432,?,?,?'
printf "\n"

printf "./select R '8765432,?,?,?' t\n"
./select R '8765432,?,?,?' t
printf "\n"

printf "./select R '8765432,?,?,?' p\n"
./select R '8765432,?,?,?' p
printf "\n"

printf "./select R '8765432,?,?,?' b\n"
./select R '8765432,?,?,?' b
printf "\n"


printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?'\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?'
printf "\n"

printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' t\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' t
printf "\n"

printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' p\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' p
printf "\n"

printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' b\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' b
printf "\n"


rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "data3 with Catc\n"
printf "./create R catc 10100 4 1000\n"
./create R catc 10100 4 1000
printf "\n"

printf "time ./insert R < data3\n"
time ./insert R < data3
printf "\n"

printf "./stats R\n"
./stats R
printf "\n"


printf "./select R '7663852,?,?,?'\n"
./select R '7663852,?,?,?'
printf "\n"

printf "./select R '7663852,?,?,?' t\n"
./select R '7663852,?,?,?' t
printf "\n"

printf "./select R '7663852,?,?,?' p\n"
./select R '7663852,?,?,?' p
printf "\n"

printf "./select R '7663852,?,?,?' b\n"
./select R '7663852,?,?,?' b
printf "\n"


printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?'\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?'
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' t\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' t
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' p\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' p
printf "\n"

printf "./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' b\n"
./select R '7664096,PjZZsBYoEYAMzgpCgRKg,?,?' b
printf "\n"


printf "./select R '?,?,a3-242,a4-242'\n"
./select R '?,?,a3-242,a4-242'
printf "\n"

printf "./select R '?,?,a3-242,a4-242' t\n"
./select R '?,?,a3-242,a4-242' t
printf "\n"

printf "./select R '?,?,a3-242,a4-242' p\n"
./select R '?,?,a3-242,a4-242' p
printf "\n"

printf "./select R '?,?,a3-242,a4-242' b\n"
./select R '?,?,a3-242,a4-242' b
printf "\n"


printf "./select R '8765432,?,?,?'\n"
./select R '8765432,?,?,?'
printf "\n"

printf "./select R '8765432,?,?,?' t\n"
./select R '8765432,?,?,?' t
printf "\n"

printf "./select R '8765432,?,?,?' p\n"
./select R '8765432,?,?,?' p
printf "\n"

printf "./select R '8765432,?,?,?' b\n"
./select R '8765432,?,?,?' b
printf "\n"


printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?'\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?'
printf "\n"

printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' t\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' t
printf "\n"

printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' p\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' p
printf "\n"

printf "./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' b\n"
./select R '7664096,tRzgWRUlUEdoYPZjofYr,?,?' b
printf "\n"

rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "make clean\n"
make clean


