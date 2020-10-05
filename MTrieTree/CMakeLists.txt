# 子项目，m分支Trie树
cmake_minimum_required (VERSION 3.8)

project(MTrieTree)

# 需要进行编译链接的源文件
set(SOURCES
	"main.cpp"
	"src/MTrieTree.cpp"
)

add_executable(MTrieTree ${SOURCES})

target_include_directories(MTrieTree
	PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)
