cmake_minimum_required(VERSION 3.10)

# set the project name
project(MyProject)

find_package(Threads REQUIRED)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# collect all source files
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/thread_pool/*.cpp" "src/process_pool/*.cpp" "src/socket/*.cpp" "src/echo_server/*.cpp")

# add the executable
add_executable(MyProject src/main.cpp ${SOURCES})

target_link_libraries(MyProject PRIVATE Threads::Threads)

# include directories
target_include_directories(MyProject PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/thread_pool
    ${PROJECT_SOURCE_DIR}/src/process_pool
    ${PROJECT_SOURCE_DIR}/src/socket
    ${PROJECT_SOURCE_DIR}/src/echo_server
    ${PROJECT_SOURCE_DIR}/src
)

# Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Collect all test files
file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "test/*.cpp")

# Add test executable
add_executable(MyProjectTests ${TEST_SOURCES}  ${SOURCES})

# Link test executable against gtest and gtest_main
target_link_libraries(MyProjectTests ${GTEST_LIBRARIES} Threads::Threads)

# Include directories for tests
target_include_directories(MyProjectTests PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/thread_pool
    ${PROJECT_SOURCE_DIR}/src/process_pool
    ${PROJECT_SOURCE_DIR}/src/socket
    ${PROJECT_SOURCE_DIR}/src/echo_server
    ${PROJECT_SOURCE_DIR}/src  # 添加这一行，确保测试文件能够找到所有头文件
)

# Enable CTest
enable_testing()

# Add a test
add_test(NAME MyProjectTests COMMAND MyProjectTests)