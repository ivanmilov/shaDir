cmake_minimum_required(VERSION 2.8)
project(sha CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(Boost COMPONENTS program_options system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(
    ${PROJECT_NAME}
    main.cpp
    sha.cpp
    reader.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
    crypto
)
