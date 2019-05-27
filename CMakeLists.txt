cmake_minimum_required(VERSION 3.13)
project(diplom)

set(CMAKE_CXX_STANDARD 17)

add_executable(diplom main.cpp Traverser.cpp TraverserByIterator.cpp File.h Tree.h Tuple.h Record.h Key.h ManySourceStrategy.h SourceStrategy.h)