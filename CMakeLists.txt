# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("experiment")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS__RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 包含子项目。
add_subdirectory ("BPlusTree")
add_subdirectory ("TrieTree")
add_subdirectory ("MTrieTree")
add_subdirectory ("RadixTree")
