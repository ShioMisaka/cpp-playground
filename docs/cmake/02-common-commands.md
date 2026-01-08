# 常用命令参考

本文档介绍 CMake 开发中最常用的命令、变量和配置选项。

[← 返回目录](./README.md)

---

## 一、设置 C++ 标准

### 基本设置

```cmake
# 设置 C++ 标准（17）
set(CMAKE_CXX_STANDARD 17)

# 要求必须支持该标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁止编译器扩展（使用标准模式）
set(CMAKE_CXX_EXTENSIONS OFF)
```

### 支持的 C++ 标准

| 标准 | CMake 版本要求 | 说明 |
|------|---------------|------|
| C++11 | 3.1+ | 较旧的 C++ 标准 |
| C++14 | 3.4+ | 通用特性 |
| C++17 | 3.7+ | 推荐使用 |
| C++20 | 3.12+ | 最新特性 |

### 条件设置（兼容性）

```cmake
# 根据编译器支持情况自动选择
set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_CXX17_STANDARD_COMPILE_OPTION)
    set(CMAKE_CXX_STANDARD 14)
endif()
```

---

## 二、构建类型配置

### 常用构建类型

```cmake
# 未指定时默认 Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Debug 配置
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Release 配置
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# RelWithDebInfo 配置（优化 + 调试信息）
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# MinSizeRel 配置（最小体积）
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
```

### 编译选项对比

| 构建类型 | 优化级别 | 调试信息 | 断言 | 适用场景 |
|---------|---------|---------|------|---------|
| Debug | -O0 | 有 | 启用 | 开发调试 |
| Release | -O3 | 无 | 禁用 | 生产环境 |
| RelWithDebInfo | -O2 | 有 | 禁用 | 性能分析 |
| MinSizeRel | -Os | 无 | 禁用 | 资源受限 |

### 添加调试定义

```cmake
# Debug 模式下定义宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
endif()

# 或使用生成器表达式（推荐）
target_compile_definitions(my_target PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
)
```

---

## 三、添加可执行文件

### 基本用法

```cmake
# 单个源文件
add_executable(my_app main.cpp)

# 多个源文件
add_executable(my_app
    main.cpp
    utils.cpp
    helper.cpp
)
```

### 自动收集源文件

```cmake
# 收集目录下所有 .cpp 文件
file(GLOB SOURCES "src/*.cpp")
add_executable(my_app ${SOURCES})

# 递归收集（包括子目录）
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(my_app ${SOURCES})
```

> **注意**：GLOB 不感知新增文件，需要重新运行 cmake。推荐显式列出源文件。

### 设置输出目录

```cmake
# 设置所有可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 按构建类型分别设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)

# 特定目标的输出目录
set_target_properties(my_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build
)
```

---

## 四、链接库

### 查找系统库

```cmake
# 查找线程库
find_package(Threads REQUIRED)
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE Threads::Threads)

# 查找数学库
find_library(MATH_LIB m)
if(MATH_LIB)
    target_link_libraries(my_app PRIVATE ${MATH_LIB})
endif()
```

### 链接静态/动态库

```cmake
# 链接静态库
target_link_libraries(my_app PRIVATE /path/to/lib.a)

# 链接动态库
target_link_libraries(my_app PRIVATE /path/to/lib.so)

# 指定库搜索路径
link_directories(/path/to/lib/dir)

# 链接多个库
target_link_libraries(my_app PRIVATE
    lib1
    lib2
    /path/to/lib3.a
)
```

### 链接顺序说明

链接顺序很重要：被依赖的库放在后面

```cmake
# 正确：A 依赖 B，B 依赖 C
target_link_libraries(my_app PRIVATE A B C)

# 错误：可能导致链接错误
target_link_libraries(my_app PRIVATE C B A)
```

---

## 五、包含头文件

### 基本用法

```cmake
# 添加当前目录及子目录
target_include_directories(my_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 添加多个目录
target_include_directories(my_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include
)
```

### 作用域说明

| 作用域 | 说明 | 使用场景 |
|------|------|---------|
| PRIVATE | 仅当前目标使用 | 内部实现头文件 |
| PUBLIC | 当前目标及依赖者使用 | 库的公共 API 头文件 |
| INTERFACE | 仅依赖者使用 | 纯头文件库 |

### 示例

```cmake
# 库项目
add_library(my_lib src/lib.cpp)
target_include_directories(my_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 使用该库的可执行文件
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE my_lib)
# my_app 会自动获得 my_lib 的 PUBLIC include 目录
```

---

## 六、编译选项

### 添加编译选项

```cmake
# 添加警告选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# MSVC 特定选项
if(MSVC)
    add_compile_options(/W4)
endif()
```

### 特定目标的编译选项

```cmake
# 使用 target_compile_options（推荐）
target_compile_options(my_app PRIVATE
    -Wall
    -Wextra
    $<$<CONFIG:Debug>:-O0>
)
```

### 预处理器定义

```cmake
# 全局定义
add_definitions(-DUSE_CUSTOM_FEATURE)

# 特定目标（推荐）
target_compile_definitions(my_app PRIVATE
    USE_CUSTOM_FEATURE
    VERSION="1.0.0"
)
```

---

## 七、条件判断

### 常用条件

```cmake
# 判断变量是否定义
if(DEFINED MY_VAR)
    message("MY_VAR is defined")
endif()

# 判断变量值
if(MY_VAR STREQUAL "value")
    message("MY_VAR equals value")
endif()

# 判断布尔值
if(MY_BOOL)
    message("MY_BOOL is true")
endif()

# 逻辑运算
if(MY_VAR AND MY_OTHER_VAR)
    message("Both are true")
endif()

if(MY_VAR OR MY_OTHER_VAR)
    message("At least one is true")
endif()

if(NOT MY_VAR)
    message("MY_VAR is false")
endif()
```

### 编译器判断

```cmake
# GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Using GCC")
endif()

# Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message("Using Clang")
endif()

# MSVC
if(MSVC)
    message("Using MSVC")
endif()
```

### 平台判断

```cmake
# Linux
if(UNIX AND NOT APPLE)
    message("Linux platform")
endif()

# macOS
if(APPLE)
    message("macOS platform")
endif()

# Windows
if(WIN32)
    message("Windows platform")
endif()
```

---

## 八、常用内置命令

### message 命令

```cmake
# 普通输出
message("Hello CMake")

# 状态信息（灰色）
message(STATUS "Building project: ${PROJECT_NAME}")

# 警告信息（黄色）
message(WARNING "This is a warning")

# 错误信息（红色，会中断生成）
message(FATAL_ERROR "Configuration error!")
```

### list 命令

```cmake
# 创建列表
set(MY_LIST item1 item2 item3)

# 获取长度
list(LENGTH MY_LIST len)
message("Length: ${len}")  # 输出: Length: 3

# 获取元素
list(GET MY_LIST 0 first_item)
message("First: ${first_item}")  # 输出: First: item1

# 追加元素
list(APPEND MY_LIST item4)

# 查找元素
list(FIND MY_LIST "item2" index)
message("Index: ${index}")  # 输出: Index: 1
```

### file 命令

```cmake
# 读取文件内容
file(READ filename.txt CONTENTS)
file(WRITE filename.txt "Content")
file(APPEND filename.txt "More content")

# 复制文件
file(COPY source.txt DESTINATION ${CMAKE_BINARY_DIR})

# 下载文件
file(DOWNLOAD http://example.com/file.txt ${CMAKE_BINARY_DIR}/file.txt)
```

---

## 九、生成器表达式

### 基本语法

```cmake
$<条件:真值>
$<条件:真值:假值>
```

### 常用生成器表达式

```cmake
# 根据配置类型
target_compile_definitions(my_app PRIVATE
    $<$<CONFIG:Debug>:DEBUG_MODE>
    $<$<CONFIG:Release>:NDEBUG>
)

# 根据编译器
target_compile_options(my_app PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# 根据平台
target_link_libraries(my_app PRIVATE
    $<$<PLATFORM_ID:Linux>:dl>
    $<$<PLATFORM_ID:Windows>:winmm>
)

# 条件连接
target_compile_definitions(my_app PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:DEBUG_LINUX>
)
```

---

## 十、完整示例模板

```cmake
cmake_minimum_required(VERSION 3.20)

project(MyProject VERSION 1.0.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 构建类型配置
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    add_compile_definitions(DEBUG_MODE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -pedantic)
    endif()
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)

# 源文件
set(SOURCES
    src/main.cpp
    src/utils.cpp
)

# 可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 链接库
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# 编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_VERSION="${PROJECT_VERSION}"
)
```

---

## 下一步

- [项目结构组织](./03-project-structure.md) - 多目录项目、库管理、依赖处理
