cmake_minimum_required(VERSION 3.20)
project(smart_pointers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(ENABLE_SANITIZERS "Build with sanitizers" OFF)
option(ENABLE_OPT        "Enable -O1"        OFF)

if(ENABLE_OPT)
    add_compile_options(-O2 -DNDEBUG)
endif()

if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

add_subdirectory(unique_ptr)
add_subdirectory(shared_ptr)

add_executable(${PROJECT_NAME} main.cpp bench.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE unique_ptr shared_ptr)
