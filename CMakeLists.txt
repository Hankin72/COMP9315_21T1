cmake_minimum_required(VERSION 3.15)
project(COMP9024 C)

#set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD 11)

#include_directories(SYSTEM"/fool/bar")

add_executable(COMP9024
        Codes/practice.c
        )

# x1的测试文件
# 缺少.h文件或者.c文件的可以自己加
add_executable(x1_test
        ass2/x1.c
        ass2/bits.h
        ass2/bits.c
        ass2/reln.h
        ass2/reln.c
        ass2/defs.h
        ass2/tuple.h
        ass2/tuple.c
        ass2/page.h
        ass2/page.c
        ass2/util.h
        ass2/util.c
        ass2/query.c
        ass2/query.h
        ass2/bsig.c
        ass2/bsig.h
        ass2/psig.c
        ass2/psig.h
        ass2/tsig.c
        ass2/tsig.h
        ass2/hash.c
        ass2/hash.h
        )


# x3的测试文件
# 缺少.h文件或者.c文件的可以自己加
add_executable(final

        final/q2/bufpool.c
        final/q2/bnl.c
        final/q2/bufpool.h

        )




# 考试测试
#add_executable(final
#        exam/past/q1/bits.c
#        exam/past/q1/bits.h
#        exam/past/q1/buckets.c
#
#
#        )
