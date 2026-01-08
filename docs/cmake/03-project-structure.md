# 项目结构组织

本文档介绍如何使用 CMake 组织多目录项目、管理库文件和处理依赖。

[← 返回目录](./README.md)

---

## 一、多目录项目

### 项目结构示例

```
my_project/
├── CMakeLists.txt           # 根 CMakeLists.txt
├── src/
│   ├── CMakeLists.txt       # src 子目录
│   ├── main.cpp
│   └── utils.cpp
└── lib/
    ├── CMakeLists.txt       # lib 子目录
    ├── mylib.cpp
    └── mylib.h
```

### 根 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# 添加子目录
add_subdirectory(lib)
add_subdirectory(src)
```

### lib/CMakeLists.txt

```cmake
# 创建库
add_library(mylib
    mylib.cpp
    mylib.h
)

# 导出头文件路径
target_include_directories(mylib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
```

### src/CMakeLists.txt

```cmake
# 创建可执行文件
add_executable(myapp
    main.cpp
    utils.cpp
)

# 链接库
target_link_libraries(myapp PRIVATE mylib)
```

---

## 二、创建和使用库

### 静态库

```cmake
# 创建静态库（默认）
add_library(mylib STATIC
    lib.cpp
    lib.h
)

# 或明确指定
add_library(mylib STATIC lib.cpp)
```

### 动态库

```cmake
# 创建动态库/共享库
add_library(mylib SHARED
    lib.cpp
    lib.h
)
```

### 对象库（不生成实际文件）

```cmake
# 对象库可用于链接到其他目标
add_library(mylib OBJECT
    lib.cpp
    lib.h
)

# 将对象库链接到可执行文件
add_executable(myapp main.cpp)
target_sources(myapp PRIVATE $<TARGET_OBJECTS:mylib>)
```

### 库类型对比

| 类型 | 文件扩展名 | 链接方式 | 优缺点 |
|------|----------|---------|--------|
| 静态库 | .a / .lib | 编译时链接 | 独立运行，体积大 |
| 动态库 | .so / .dll | 运行时链接 | 体积小，需部署库文件 |
| 对象库 | .o / .obj | 编译时链接 | 中间产物，不单独生成库文件 |

---

## 三、库的可见性设置

### target_link_libraries 作用域

```cmake
# PRIVATE: 仅 mylib 使用
target_link_libraries(mylib PRIVATE
    dependency_lib
)

# PUBLIC: mylib 及其依赖者都可使用
target_link_libraries(mylib PUBLIC
    dependency_lib
)

# INTERFACE: 仅依赖者可使用（mylib 本身不使用）
target_link_libraries(mylib INTERFACE
    dependency_lib
)
```

### 完整示例

```cmake
# 库 A 依赖库 B
add_library(libA libA.cpp)
target_link_libraries(libA PUBLIC libB)
# libA 的使用者也会自动链接 libB

# 可执行文件
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE libA)
# myapp 自动链接 libA 和 libB
```

### 传递包含目录

```cmake
add_library(mylib mylib.cpp)

# 公共包含目录（传递给使用者）
target_include_directories(mylib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 私有包含目录（不传递）
target_include_directories(mylib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 接口包含目录（仅传递给使用者）
target_include_directories(mylib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/api
)
```

---

## 四、查找第三方库

### 使用 find_package

```cmake
# 查找包
find_package(OpenCV REQUIRED)
find_package(Boost 1.70 REQUIRED COMPONENTS filesystem system)

# 链接找到的库
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE
    OpenCV::core
    OpenCV::imgproc
    Boost::filesystem
)
```

### 查找模式

```cmake
# Module 模式（查找 Find<package>.cmake）
find_package(OpenCV REQUIRED)

# Config 模式（查找 <package>Config.cmake）
find_package(OpenCV CONFIG REQUIRED)

# 两种模式都尝试
find_package(OpenCV REQUIRED)
```

### 自定义 Find 模块

创建 `cmake/FindMyLib.cmake`：

```cmake
# 查找头文件
find_path(MYLIB_INCLUDE_DIR
    NAMES mylib.h
    PATHS /usr/include /usr/local/include
)

# 查找库文件
find_library(MYLIB_LIBRARY
    NAMES mylib
    PATHS /usr/lib /usr/local/lib
)

# 设置结果变量
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MyLib
    REQUIRED_VARS MYLIB_LIBRARY MYLIB_INCLUDE_DIR
)

# 创建导入目标
if(MYLIB_FOUND)
    add_library(MyLib::MyLib UNKNOWN IMPORTED)
    set_target_properties(MyLib::MyLib PROPERTIES
        IMPORTED_LOCATION "${MYLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${MYLIB_INCLUDE_DIR}"
    )
endif()
```

使用自定义查找模块：

```cmake
# 添加 cmake 模块搜索路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 查找自定义库
find_package(MyLib REQUIRED)
target_link_libraries(myapp PRIVATE MyLib::MyLib)
```

---

## 五、导入外部库

### 导入静态库

```cmake
# 创建导入目标
add_library(mylib STATIC IMPORTED)

# 设置库文件路径
set_target_properties(mylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/third_party/lib/libmylib.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/third_party/include
)

# 使用
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE mylib)
```

### 导入动态库

```cmake
# 导入共享库
add_library(mylib SHARED IMPORTED)

# 根据平台设置路径
if(WIN32)
    set_target_properties(mylib PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/third_party/bin/mylib.dll
        IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/third_party/lib/mylib.lib
    )
else()
    set_target_properties(mylib PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/third_party/lib/libmylib.so
    )
endif()

target_include_directories(mylib INTERFACE
    ${CMAKE_SOURCE_DIR}/third_party/include
)
```

### 多配置导入（Debug/Release）

```cmake
add_library(mylib STATIC IMPORTED)

# Debug 配置
set_target_properties(mylib PROPERTIES
    IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/lib/debug/mylibd.lib"
)

# Release 配置
set_target_properties(mylib PROPERTIES
    IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/lib/release/mylib.lib"
)

# 使用生成器表达式（推荐）
set_target_properties(mylib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/$<CONFIG>/mylib.lib"
)
```

---

## 六、FetchContent 下载依赖

### 基本用法

```cmake
include(FetchContent)

# 声明项目
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

# 获取内容
FetchContent_MakeAvailable(json)

# 使用
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE nlohmann_json::nlohmann_json)
```

### 多个依赖

```cmake
include(FetchContent)

# 声明多个项目
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)

# 批量获取
FetchContent_MakeAvailable(fmt spdlog)

# 使用
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE
    fmt::fmt
    spdlog::spdlog
)
```

### 覆盖默认选项

```cmake
include(FetchContent)

FetchContent_Declare(
    mylib
    GIT_REPOSITORY https://github.com/user/mylib.git
    GIT_TAG v1.0.0
)

# 覆盖项目选项
set(MYLIB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MYLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(mylib)
```

---

## 七、安装规则

### 安装可执行文件

```cmake
# 安装可执行文件
install(TARGETS myapp
    RUNTIME DESTINATION bin
)

# 安装到自定义路径
install(TARGETS myapp
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
```

### 安装库文件

```cmake
# 安装库和头文件
install(TARGETS mylib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    include/mylib.h
    include/mylib_api.h
    DESTINATION include
)
```

### 安装配置文件

```cmake
# 生成并安装配置文件
include(CMakePackageConfigHelpers)

# 生成版本文件
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mylibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# 生成配置文件
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/mylibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake"
    INSTALL_DESTINATION lib/cmake/mylib
)

# 安装配置文件
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mylibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/mylibConfigVersion.cmake"
    DESTINATION lib/cmake/mylib
)
```

---

## 八、完整项目示例

### 项目结构

```
my_project/
├── CMakeLists.txt
├── cmake/
│   └── FindThirdParty.cmake
├── lib/
│   ├── CMakeLists.txt
│   ├── mylib.cpp
│   └── include/
│       └── mylib.h
└── app/
    ├── CMakeLists.txt
    └── main.cpp
```

### 根 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)

project(MyProject VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 cmake 模块路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# 选项
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" ON)

# 查找依赖
find_package(Threads REQUIRED)

# 添加子目录
add_subdirectory(lib)
add_subdirectory(app)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
```

### lib/CMakeLists.txt

```cmake
add_library(mylib
    mylib.cpp
    include/mylib.h
)

target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(mylib PUBLIC Threads::Threads)
```

### app/CMakeLists.txt

```cmake
add_executable(myapp main.cpp)

target_link_libraries(myapp PRIVATE mylib)

# 安装规则
install(TARGETS myapp
    RUNTIME DESTINATION bin
)
```

---

## 下一步

您已经掌握了 CMake 的基础和进阶用法，可以：

1. 参考 [01-quick-start.md](./01-quick-start.md) 回顾基础
2. 参考 [02-common-commands.md](./02-common-commands.md) 查找具体命令
3. 查看 [CMake 官方文档](https://cmake.org/documentation/) 了解更多
