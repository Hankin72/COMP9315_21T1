#chmod +x ./test.sh
# ./test.sh
make clean

make
clear


./create R simc 10000 6 1000

./gendata 10000 6 1234567 13 > R.in


./insert R < R.in

./stats R


#./select R '?,?,?,?,?,?' x | tail -6

# 不实用signature 进行查询

./select R '1234999,?,?,?,?,?' x

# 使用tuple signature 进行查询

#./select R '1234999,?,?,?,?,?' t

# 使用page signature 进行查询

#./select R '1234999,?,?,?,?,?' p

# 使用bit-sliced signature 进行查询

./select R '1234999,?,?,?,?,?' b
#grep 'a3-241,a4-158,a5-407' R.in
#
#./select R '?,?,a3-241,a4-158,a5-407,?' x
#
#./select R '?,?,a3-241,a4-158,a5-407,?' t
#
#./select R '?,?,a3-241,a4-158,a5-407,?' p
#
#./select R '?,?,a3-241,a4-158,a5-407,?' b


rm -f R.bsig R.data R.info R.psig R.tsig
printf "\n"

printf "make clean\n"
make clean



