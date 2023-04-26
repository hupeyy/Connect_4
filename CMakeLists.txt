cmake_minimum_required(VERSION 3.21)
project(Connect_4)

set(CMAKE_CXX_STANDARD 14)

set(SFML_DIR "C:/Users/alexa/Downloads/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(Connect_4 main.cpp)
target_link_libraries(Connect_4 sfml-graphics sfml-audio)