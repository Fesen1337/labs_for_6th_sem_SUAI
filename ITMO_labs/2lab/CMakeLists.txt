#To choose 'easy' version of homework you should remove '#' on the second line and remove the    third line at all
set(RUN_MODE "easy")



cmake_minimum_required(VERSION 3.9)
project(number-trie)

set(CMAKE_CXX_STANDARD 17)

include(testing/CMakeLists.txt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined,address -fno-sanitize-recover=all -D_GLIBCXX_DEBUG")


set(SOURCES number/number.cpp)
set(HEADERS number/number.h)
set(TESTS number/number-test.cpp)

if ("${RUN_MODE}" STREQUAL "hard")
	set(SOURCES ${SOURCES} trie/trie.cpp trie/trie-naive.cpp)
	set(HEADERS ${HEADERS} trie/trie.h trie/trie-naive.h)
	set(TESTS ${TESTS} trie/trie-test.cpp)
endif()


add_executable(public-tests ${SOURCES} ${HEADERS} ${TESTS})
target_link_libraries(public-tests gtest_main)
