cmake_minimum_required(VERSION 3.20)

# 不创建顶层 project，不影响子 CMakeLists 的 PROJECT_NAME 定义
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 添加项目，vscode 的静态检查就可以工作了
foreach(subdir
    cpp_notes/01_basics/01_ptr_ref/move_semantics
    cpp_notes/01_basics/01_ptr_ref/ptr_ref
    cpp_notes/01_basics/02_keywords/const
    cpp_notes/01_basics/03_string/nomal
)
    add_subdirectory(${subdir})
endforeach()
