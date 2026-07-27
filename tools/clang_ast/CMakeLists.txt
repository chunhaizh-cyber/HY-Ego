cmake_minimum_required(VERSION 3.25)

project(ClangAST候选流程图 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LLVM_ROOT "C:/Program Files/LLVM" CACHE PATH "LLVM 安装目录")

find_path(CLANG_C_INCLUDE_DIR
    NAMES clang-c/Index.h
    PATHS "${LLVM_ROOT}/include"
    REQUIRED)

find_library(LIBCLANG_LIBRARY
    NAMES libclang
    PATHS "${LLVM_ROOT}/lib"
    REQUIRED)

add_executable(clang_ast_tool 提取.候选流程图.cpp)
set_target_properties(clang_ast_tool PROPERTIES OUTPUT_NAME "提取_候选流程图")
target_include_directories(clang_ast_tool PRIVATE "${CLANG_C_INCLUDE_DIR}")
target_link_libraries(clang_ast_tool PRIVATE "${LIBCLANG_LIBRARY}")

if (MSVC)
    target_compile_options(clang_ast_tool PRIVATE /utf-8)
endif()
