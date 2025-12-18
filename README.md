# cpp-learning

这是一个 C++ 学习项目，包含可运行的 demo 示例。

## 项目结构

```text
.
├── tool/ok-cpp     # C++ 项目管理工具（Git Submodule）
├── demos/          # 各类 C++ 示例
```

## 克隆项目

```bash
git clone --recurse-submodules git@github.com:ShioMisaka/cpp-playground.git
```

### 子项目
> 如果你没有带 --recurse-submodules，可以手动克隆一下
```bash
git submodule update --init --recursive
```

#### 更新子项目
```bash
git submodule update --remote
```
这一步做的是：
- 更新 ok-cpp 仓库到最新 commit
- 但 主项目还不知道这次变化，还需要提交更新

#### 提交子模块更新（非常关键）
```bash
git add tool/ok-cpp
git commit -m "chore: update ok-cpp submodule to latest"
```
📌 注意：
这里提交的不是 ok-cpp 的代码，而是：

> ok-cpp 当前 commit 的引用
---