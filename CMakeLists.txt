cmake_minimum_required(VERSION 3.20)

# 不创建顶层 project，不影响子 CMakeLists 的 PROJECT_NAME 定义
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

foreach(subdir
    01_nomal
    02_multithread
    11_smart_pointer
    12_eigen
)
    add_subdirectory(${subdir})
endforeach()
