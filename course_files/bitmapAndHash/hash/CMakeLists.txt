enable_testing()

add_executable(test_hashset test_yshash.cpp yshash.h)

add_executable(test_hashtable test_hashtable.cpp yshash.h)

add_test(NAME hashset COMMAND test_hashset)
add_test(NAME hashtable COMMAND test_hashtable)
