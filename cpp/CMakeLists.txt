cmake_minimum_required(VERSION 3.13)

# project name
project(demos_cpp LANGUAGES CXX)

# Set the compiler options
set(CMAKE_CXX_STANDARD 17)
# msvc or gcc
if(MSVC)
add_compile_options("/utf-8")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

file(GLOB_RECURSE PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/temp/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/temp/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/temp/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/temp/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/temp/*.hpp
)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCES}
)
target_link_libraries(
    ${PROJECT_NAME}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
