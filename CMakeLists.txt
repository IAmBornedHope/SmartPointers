cmake_minimum_required(VERSION 3.15)
project(smart_pointers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(unique_ptr)
add_subdirectory(shared_ptr)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE unique_ptr shared_ptr)

