add_library(labs)

target_sources(labs
  PRIVATE
        lab3/consolelog.cpp
        lab3/JobObject.cpp
        lab3/BackupJob.cpp
        lab3/RestorePoint.cpp
        lab3/Object.cpp
        lab3/Repository.cpp
  PUBLIC
        lab3/JobObject.h
        lab3/BackupJob.h
        lab3/RestorePoint.h
        lab3/Object.h
        lab3/Repository.h
        lab3/lab3_functions.h
        
  )

target_include_directories(labs
  PUBLIC
        .
  )

# we use this to get code coverage
# flags are only valid with the GNU compiler and on Linux
if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  target_compile_options(labs
    PUBLIC
      "--coverage"
    )
  target_link_options(labs
    INTERFACE
      "--coverage"
    )
endif()
