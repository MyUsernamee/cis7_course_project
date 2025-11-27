cmake_minimum_required(VERSION 3.23)
project(blackjack)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Used for LSP

file(GLOB_RECURSE SOURCES ./src/*.cpp)

# TESTING #
enable_testing()

file(GLOB_RECURSE TEST_SOURCES ./tests/*.cpp)
list(APPEND TEST_SOURCES ${SOURCES})
list(FILTER TEST_SOURCES EXCLUDE REGEX "main.cpp$")
add_executable(test_executable ${TEST_SOURCES})
target_link_libraries(test_executable GTest::gtest_main)
target_include_directories(test_executable PRIVATE src)

include(GoogleTest)
gtest_discover_tests(test_executable)
# END TESTING #

add_executable(${PROJECT_NAME} ${SOURCES})
