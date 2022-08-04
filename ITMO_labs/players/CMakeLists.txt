include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(SOURCES human-player.cpp simple-ai-player.cpp)
set(HEADERS human-player.h simple-ai-player.h player-interface.h)

if ("${RUN_MODE}" STREQUAL "hard")
	list(APPEND SOURCES "smart-ai-player.cpp")
	list(APPEND HEADERS "smart-ai-player.h")
endif ()

add_library(players STATIC ${SOURCES} ${HEADERS})
