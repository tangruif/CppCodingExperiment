# CMakeList.txt: 实验1.2 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。

project(experiment1_2)

set(SOURCES 
	"main.cpp"
	"src/BloomFilter.cpp"
)

add_executable(experiment1_2 ${SOURCES})

target_include_directories(experiment1_2 
	PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)

# TODO: 如有需要，请添加测试并安装目标。
