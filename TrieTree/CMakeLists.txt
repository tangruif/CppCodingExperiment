# TrieTree 的cmake项目构建配置

cmake_minimum_required (VERSION 3.8)

project(TrieTreeSearch)

# 将源代码添加到此项目的可执行文件
set(SOURCES 
	"main.cpp"
	"src/TrieTree.cpp"
)

add_executable(TrieTreeSearch ${SOURCES})

target_include_directories(TrieTreeSearch
	PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)
