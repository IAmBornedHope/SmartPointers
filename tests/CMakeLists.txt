cmake_minimum_required(VERSION 3.15)
project(smart_pointers_test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


option(ENABLE_SANITIZERS "Build with sanitizers" OFF)

if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../extern/googletest"
                 "${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
                 EXCLUDE_FROM_ALL)

                 
add_executable(smart_pointers_test
                main.cpp
                test_UniquePtr.cpp
                test_SharedPtr.cpp)
               
target_link_libraries(smart_pointers_test 
                      gtest_main)

target_include_directories(smart_pointers_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

