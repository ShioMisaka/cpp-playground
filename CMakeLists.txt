cmake_minimum_required(VERSION 3.20)

# 不创建顶层 project，不影响子 CMakeLists 的 PROJECT_NAME 定义
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 添加项目，vscode 的静态检查就可以工作了
foreach(subdir
    01_move_semantics
    02_RB_tree
    qt_demo/calculator
    qt_demo/note_book
)
    add_subdirectory(${subdir})
endforeach()
