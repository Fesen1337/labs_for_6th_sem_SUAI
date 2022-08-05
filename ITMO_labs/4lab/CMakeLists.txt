#To choose 'easy' version of homework you should remove '#' on the second line and remove the third line at all
set(RUN_MODE "easy")
#set(RUN_MODE "hard")

cmake_minimum_required(VERSION 3.9)
project(phone-book)

set(CMAKE_CXX_STANDARD 17)

include(testing/CMakeLists.txt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined,address,leak -fno-sanitize-recover=all -D_GLIBCXX_DEBUG")


set(SOURCES phone-book.cpp)
set(HEADERS phone-book.h utils.h)


set(TESTS main-easy.cpp)
if ("${RUN_MODE}" STREQUAL "hard")
	list(APPEND TESTS "main-hard.cpp")
endif ()

add_executable(tests ${SOURCES} ${HEADERS} ${TESTS})
target_link_libraries(tests gtest_main)
