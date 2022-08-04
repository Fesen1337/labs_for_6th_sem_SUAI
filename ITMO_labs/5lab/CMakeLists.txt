#To choose 'easy' version of homework you should remove '#' on the second line and remove the third line at all
set(RUN_MODE "easy")

include_directories(.)

cmake_minimum_required(VERSION 3.9)
project(sea-battle)

set(CMAKE_CXX_STANDARD 17)

include(testing/CMakeLists.txt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wno-sign-compare -pedantic -fsanitize=undefined,address,leak -fno-sanitize-recover=all -D_GLIBCXX_DEBUG")


set(SOURCES testing/mock-player.cpp testing/mock-player-data.cpp)
set(HEADERS testing/mock-player.h testing/mock-player-data.h)

add_subdirectory(players)
add_subdirectory(game)

set(TESTS main-game.cpp main-human.cpp)
if ("${RUN_MODE}" STREQUAL "hard")
	list(APPEND TESTS "main-competition.cpp")
endif ()

add_executable(tests ${SOURCES} ${HEADERS} ${TESTS})
target_link_libraries(tests gtest_main players game)
