# 快速入门：CMake 基础

本文档适合首次使用 CMake 的用户，从基础概念开始，到创建第一个 CMake 项目。

[← 返回目录](./README.md)

---

## 一、什么是 CMake

CMake 是一个跨平台的构建系统生成工具，它不直接构建代码，而是生成特定平台的构建文件（如 Unix Makefile、Visual Studio 项目文件等）。

### 为什么使用 CMake

| 特点 | 说明 |
|------|------|
| 跨平台 | 同一套 CMake 代码可在 Windows/Linux/macOS 上运行 |
| 简化构建 | 自动处理依赖关系、编译选项 |
- 可扩展 | 支持自定义命令和脚本 |
- 广泛支持 | 许多 C++ 项目（如 OpenCV、Boost）都使用 CMake |

### CMake 工作流程

```
CMakeLists.txt (源码)
       ↓
   cmake 配置
       ↓
  构建系统 (Makefile/VS方案)
       ↓
    编译链接
       ↓
  可执行文件/库
```

---

## 二、安装 CMake

### Linux

```bash
# Ubuntu/Debian
sudo apt install cmake

# CentOS/RHEL
sudo yum install cmake

# Arch Linux
sudo pacman -S cmake
```

### macOS

```bash
# Homebrew
brew install cmake
```

### Windows

从 [CMake 官网](https://cmake.org/download/) 下载安装包，或使用包管理器：

```bash
# Chocolatey
choco install cmake

# Scoop
scoop install cmake
```

### 验证安装

```bash
cmake --version
```

---

## 三、第一个 CMake 项目

### 项目结构

```
hello_cmake/
├── CMakeLists.txt
└── main.cpp
```

### 1. 编写 main.cpp

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, CMake!" << std::endl;
    return 0;
}
```

### 2. 编写 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)

project(HelloCMake)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(hello_cmake main.cpp)
```

### 3. 构建项目

```bash
# 创建构建目录（推荐源外构建）
mkdir build
cd build

# 配置项目（生成构建文件）
cmake ..

# 编译项目
cmake --build .

# 运行程序
./hello_cmake
```

输出：
```
Hello, CMake!
```

---

## 四、CMakeLists.txt 结构

### 基本语法规则

```cmake
# 注释以 # 开头

# 命令格式：命令名(参数)
# 参数用空格或分号分隔
set(MY_VAR "hello")
set(MY_VAR "hello")      # 等价

# 变量引用：${变量名}
message(${MY_VAR})       # 输出: hello
```

### 最小 CMakeLists.txt

```cmake
# 指定最低 CMake 版本
cmake_minimum_required(VERSION 3.20)

# 定义项目名称
project(ProjectName)

# 添加可执行文件
add_executable(executable_name source1.cpp source2.cpp)
```

### 推荐的基础模板

```cmake
cmake_minimum_required(VERSION 3.20)

project(MyProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置构建类型（未指定时默认 Release）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 编译选项配置
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    add_compile_definitions(DEBUG_MODE)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)
```

---

## 五、常用构建命令

### 配置阶段

```bash
# 基本配置
cmake -B build

# 指定构建类型
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 指定生成器
cmake -B build -G "Unix Makefiles"
cmake -B build -G "Visual Studio 17 2022"

# 指定 C++ 标准
cmake -B build -DCMAKE_CXX_STANDARD=20
```

### 构建阶段

```bash
# 基本构建
cmake --build build

# 指定构建类型
cmake --build build --config Debug
cmake --build build --config Release

# 并行编译（加速构建）
cmake --build build --parallel 4
cmake --build build -j4

# 只编译特定目标
cmake --build build --target hello_cmake
```

### 清理构建

```bash
# 清理构建产物
cmake --build build --target clean

# 或直接删除构建目录（更彻底）
rm -rf build
```

---

## 六、变量与作用域

### CMake 变量类型

| 类型 | 说明 | 示例 |
|------|------|------|
| 普通变量 | `set()` 定义，作用域内有效 | `set(MY_VAR "value")` |
| 缓存变量 | 存储在 CMakeCache.txt，跨作用域 | `set(MY_VAR "value" CACHE ...)` |
| 环境变量 | 访问系统环境变量 | `$ENV{PATH}` |
| 内置变量 | CMake 预定义变量 | `${PROJECT_NAME}`, `${CMAKE_CXX_COMPILER}` |

### 变量作用域

```cmake
# 全局作用域
set(GLOBAL_VAR "global")

function(my_func)
    # 函数内作用域，修改不影响外部
    set(GLOBAL_VAR "inside_function")
    message("Function: ${GLOBAL_VAR}")  # 输出: inside_function
endfunction()

my_func()
message("Global: ${GLOBAL_VAR}")        # 输出: global
```

### 常用内置变量

```cmake
# 项目信息
${PROJECT_NAME}           # 项目名称
${PROJECT_SOURCE_DIR}     # 项目根目录
${PROJECT_BINARY_DIR}     # 构建目录

# 目录信息
${CMAKE_CURRENT_SOURCE_DIR}  # 当前 CMakeLists.txt 所在目录
${CMAKE_CURRENT_BINARY_DIR}  # 当前构建目录
${CMAKE_SOURCE_DIR}          # 顶层源码目录
${CMAKE_BINARY_DIR}          # 顶层构建目录

# 编译器信息
${CMAKE_CXX_COMPILER}    # C++ 编译器路径
${CMAKE_CXX_STANDARD}    # C++ 标准版本
${CMAKE_CXX_FLAGS}       # C++ 编译选项
```

---

## 七、调试 CMake

### 查看变量值

```cmake
# 输出变量值（调试时非常有用）
message("MY_VAR = ${MY_VAR}")
message("Project dir: ${PROJECT_SOURCE_DIR}")
```

### 查看缓存变量

```bash
# 查看所有缓存变量
cmake -B build -LA

# 查看特定缓存变量
cmake -B build -LA | grep MY_VAR

# 编辑缓存变量
ccmake build    # 需要 curses 库
cmake-gui build # 图形界面
```

### 跟踪执行过程

```bash
# 输出 CMake 执行的详细信息
cmake -B build --trace

# 输出更多调试信息
cmake -B build --debug-output
```

---

## 下一步

掌握基础后，继续阅读：

- [常用命令参考](./02-common-commands.md) - 学习常用命令、编译选项、目标配置
- [项目结构组织](./03-project-structure.md) - 多目录项目、库管理、依赖处理
