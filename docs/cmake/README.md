# CMake 使用指南

本指南帮助您快速查找和执行 CMake 构建操作。

## 快速导航

### 我是新手，从零开始
[01-quick-start.md](./01-quick-start.md) - CMake 基础概念、第一个 CMake 项目

### 常用命令参考
[02-common-commands.md](./02-common-commands.md) - 常用命令、变量、配置选项

### 项目结构组织
[03-project-structure.md](./03-project-structure.md) - 多目录项目、库管理、依赖处理

---

## 常用操作速查

| 场景 | 查看文档 |
|------|---------|
| 创建第一个 CMake 项目 | [01-quick-start.md](./01-quick-start.md#第一个-cmake-项目) |
| 理解 CMakeLists.txt 结构 | [01-quick-start.md](./01-quick-start.md#cmakeliststxt-结构) |
| 设置 C++ 标准 | [02-common-commands.md](./02-common-commands.md#设置-c-标准) |
| 配置编译选项（Debug/Release） | [02-common-commands.md](./02-common-commands.md#构建类型配置) |
| 添加可执行文件 | [02-common-commands.md](./02-common-commands.md#添加可执行文件) |
| 链接库文件 | [02-common-commands.md](./02-common-commands.md#链接库) |
| 多目录项目组织 | [03-project-structure.md](./03-project-structure.md#多目录项目) |
| 查找并使用第三方库 | [03-project-structure.md](./03-project-structure.md#查找第三方库) |

---

## 文档结构说明

```
docs/cmake/
├── README.md                    # 本文件 - 快速导航索引
├── 01-quick-start.md           # 快速入门：基础概念、第一个项目
├── 02-common-commands.md       # 常用命令：变量、编译选项、目标
└── 03-project-structure.md     # 项目结构：多目录、库管理、依赖
```

---

## 使用建议

1. **第一次使用 CMake**：按顺序阅读 01 → 02
2. **需要查命令**：直接查阅 02 的对应章节
3. **组织复杂项目**：阅读 03
