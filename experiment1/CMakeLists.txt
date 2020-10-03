# CMakeList.txt: 实验一 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project(experiment)

#生成静态链接库
add_library(hash_lib STATIC
	"src/HashTable.cpp"
)

target_include_directories(hash_lib
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
)

# 将源代码添加到此项目的可执行文件。
# add_executable (CMakeTarget "实验一.cpp" "实验一.h")
set(SOURCES
    "src/main.cpp"
)

add_executable(experiment ${SOURCES})

target_link_libraries(experiment
	PRIVATE
		hash_lib
)


# TODO: 如有需要，请添加测试并安装目标。
