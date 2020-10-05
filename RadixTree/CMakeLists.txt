# 子项目，2叉压缩数
cmake_minimum_required (VERSION 3.8)

project(RadixTree)

# 添加需要编译的源文件
set(SOURCES
	"main.cpp"
	"src/RadixTree.cpp"
)

add_executable(RadixTree ${SOURCES})

# 添加头文件搜索路径
target_include_directories(RadixTree
	PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)
