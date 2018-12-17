cmake_minimum_required(VERSION 3.10)
project(complier)

set(CMAKE_CXX_STANDARD 11)

add_executable(complier main.cpp lexer.h lexer.cpp syntaxer.cpp syntaxer.h error.cpp error.h quader.cpp quader.h asmer.cpp asmer.h main.h optimizer.cpp optimizer.h tools.cpp tools.h)