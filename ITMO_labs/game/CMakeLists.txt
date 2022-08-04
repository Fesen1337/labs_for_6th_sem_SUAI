include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(SOURCES sea-battle.cpp field.cpp)
set(HEADERS sea-battle.h field.h)

add_library(game STATIC ${HEADERS} ${SOURCES})
