cmake_minimum_required(VERSION 3.0.0)

project(Simple_Http_Server
    DESCRIPTION "Simple C++ HTTP Server")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src/simple_server)

add_executable(main 
    src/main.cpp 
    src/simple_server/simple_server.cpp
    src/simple_server/http_request.cpp
    src/simple_server/http_response.cpp
    src/simple_server/utils.cpp)
