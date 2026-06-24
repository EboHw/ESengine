# CMakeLists.txt —— 顶层构建脚本
cmake_minimum_required(VERSION 3.16)          # 3.16 起对 C++20 与 target 特性支持完善
project(SearchEngine LANGUAGES CXX)           # 纯 C++ 项目

# ---- 语言标准：强制 C++20，不允许编译器擅自降级 ----
set(CMAKE_CXX_STANDARD 20)                    # 使用 C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)           # 不满足就直接报错，而不是悄悄用 C++17
set(CMAKE_CXX_EXTENSIONS OFF)                 # 关掉 GNU 扩展，用纯净 -std=c++20

# ---- 可执行文件统一输出到 bin/（官方约定）----
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ---- 默认 Release，但保留调试符号方便 perf/gdb ----
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -Wall -Wextra -fsanitize=address,undefined")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)         # 生成 compile_commands.json，给 clangd/IDE 用

# ---- 三方库的头文件搜索路径 ----
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/cppjieba/include
    ${CMAKE_SOURCE_DIR}/third_party/cppjieba/deps   # cppjieba 依赖的 limonp
    ${CMAKE_SOURCE_DIR}/third_party                  # simhash/Simhasher.hpp
    ${CMAKE_SOURCE_DIR}/third_party/tinyxml2-master
)

# ---- tinyxml2 不是 header-only，需要把它的 .cpp 一起编译 ----
add_library(tinyxml2 STATIC third_party/tinyxml2-master/tinyxml2.cpp)

# ---- 把我们自己的所有模块编成一个静态库，离线/在线程序都能复用 ----
add_library(se_core STATIC
    src/Config.cpp        src/DirScanner.cpp     src/Tokenizer.cpp
    src/DocExtractor.cpp  src/Deduplicator.cpp   src/PageStore.cpp
    src/MmapFile.cpp      src/InvertedIndex.cpp  src/FlatInvertedIndex.cpp
    src/DictProducer.cpp
)
target_link_libraries(se_core PUBLIC tinyxml2 pthread)  # 链接 tinyxml2 与线程库

# ---- 离线建库可执行程序 ----
add_executable(offline_builder src/main_offline.cpp)
target_link_libraries(offline_builder PRIVATE se_core)
