cmake_minimum_required(VERSION 3.1.0)

project(qt-cli-sample)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
get_target_property(QtCore_location Qt5::Core LOCATION)

# Populate a CMake variable with the sources
set(qt_cli_sample_SRCS
    main.cpp
)
# Tell CMake to create the helloworld executable
add_executable(qt-cli-sample ${qt_cli_sample_SRCS})
# Use the modules from Qt 5
target_link_libraries(qt-cli-sample Qt5::Core)
