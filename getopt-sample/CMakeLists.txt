cmake_minimum_required(VERSION 3.1.0)

project(getopt-sample)

## Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Populate a CMake variable with the sources
set(getopt_sample_SRCS
    main.c
    filesystem_tools.c
)


## Tell CMake to create the visca-dump executable
add_executable(sample ${getopt_sample_SRCS})

## Which libraries do we need...
#target_link_libraries(sample ${EXTERNAL_LIBRARIES})
