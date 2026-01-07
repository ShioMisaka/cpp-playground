# C++ Playground

一个面向实践的 C++ 学习项目，包含基础教程、算法实现和 STL 模拟。

## 快速开始

### 方式一：使用 ok-cpp 工具（推荐）

```bash
# 安装 ok-cpp 工具
cd tool/ok-cpp
chmod +x ./install.sh
sudo ./install.sh

# 从项目根目录运行任意教程
ok-cpp run cpp_notes/01_basics/02_keywords/static

# Debug 模式运行
ok-cpp run --debug cpp_notes/01_basics/02_keywords/static
```

### 方式二：手动编译

每个教程目录都是独立的，进入目录后直接构建：

```bash
# 例如：static 关键词教程
cd cpp_notes/01_basics/02_keywords/static
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 运行
./build/static
```

## 项目结构

```
cpp-playground/
├── cpp_notes/               # 教程代码
│   ├── 01_basics/          # 基础概念
│   │   ├── 01_ptr_ref/     # 指针、引用、移动语义
│   │   ├── 02_keywords/    # C++ 关键词 (const, static, inline...)
│   │   └── 03_string/      # 字符串处理
│   └── algorithm/          # 算法实现
│       ├── 02_tree/        # 树结构 (红黑树)
│       └── 03_string_match/# 字符串匹配 (KMP)
├── qt_demo/                # Qt 演示程序
├── mystl/                  # 自实现 STL (header-only)
├── docs/                   # 文档
└── tool/ok-cpp/            # 项目管理工具 (submodule)
```

## 学习路径

| 主题 | 路径 |
|------|------|
| 指针与引用 | `cpp_notes/01_basics/01_ptr_ref/` |
| const 关键词 | `cpp_notes/01_basics/02_keywords/const/` |
| static 关键词 | `cpp_notes/01_basics/02_keywords/static/` |
| inline 关键词 | `cpp_notes/01_basics/02_keywords/inline/` |
| 字符串 | `cpp_notes/01_basics/03_string/` |
| 红黑树 | `cpp_notes/algorithm/02_tree/rb_tree/` |
| KMP 算法 | `cpp_notes/algorithm/03_string_match/kmp/` |

## 克隆项目

```bash
# 克隆项目及子模块
git clone --recurse-submodules git@github.com:ShioMisaka/cpp-playground.git
```

如果忘记 `--recurse-submodules`，手动初始化：

```bash
git submodule update --init --recursive
```

## 更新子模块

```bash
# 更新 ok-cpp 到最新版本
git submodule update --remote

# 提交子模块引用更新
git add tool/ok-cpp
git commit -m "chore: update ok-cpp submodule"
```

## 分支说明

- `dev` - 开发分支（主分支）
- `main` - 稳定分支
- `doc/cmake` - 文档分支
- `note/basic` - 笔记分支
