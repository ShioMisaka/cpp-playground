# 日常开发工作流

本文档介绍日常开发中的 Git 操作：提交代码、分支管理、版本发布等。

[← 返回目录](./README.md)

---

## 一、开始新功能

### 1. 更新本地 dev 分支

开始新功能前，先确保本地 `dev` 是最新的：

```bash
git checkout dev
git pull
```

### 2. 创建功能分支

从 `dev` 创建新分支：

```bash
git checkout -b feature/user-login
```

分支命名规范：
- `feature/xxx` - 新功能
- `fix/xxx` - Bug 修复
- `refactor/xxx` - 重构
- `docs/xxx` - 文档更新
- `test/xxx` - 测试相关

---

## 二、提交代码

### 1. 暂存修改

```bash
# 暂存单个文件
git add path/to/file.cpp

# 暂存多个文件
git add file1.cpp file2.cpp

# 暂存所有修改（包括删除）
git add -A

# 暂存所有修改（不包括删除）
git add .

# 交互式暂存（可选择性添加部分修改）
git add -i
```

### 2. 提交规范

使用约定式提交（Conventional Commits）格式：

```bash
git commit -m "feat: 实现用户登录功能"
git commit -m "fix: 修复登录时崩溃问题"
git commit -m "docs: 更新 README 说明"
```

提交类型：
| 类型 | 说明 | 示例 |
|------|------|------|
| `feat` | 新功能 | `feat: 添加用户头像上传` |
| `fix` | Bug 修复 | `fix: 修复登录超时问题` |
| `docs` | 文档变更 | `docs: 更新 API 文档` |
| `style` | 代码格式（不影响逻辑） | `style: 统一代码缩进` |
| `refactor` | 重构（非新功能也非修复） | `refactor: 重构配置模块` |
| `test` | 测试相关 | `test: 添加单元测试` |
| `chore` | 构建/工具变更 | `chore: 更新依赖版本` |

### 3. 修改最后一次提交

如果提交后发现遗漏或信息有误：

```bash
# 添加遗漏文件
git add forgotten_file.cpp
git commit --amend

# 仅修改提交信息
git commit --amend -m "fix: 正确的提交信息"
```

> **注意**：仅对未推送的提交使用 `--amend`，已推送的提交不要修改。

---

## 三、同步远程代码

开发过程中，远程可能有新提交，需要同步到本地。

### 1. 使用 rebase（推荐）

保持提交历史清晰：

```bash
# 查看当前分支状态
git status

# 拉取远程最新代码并 rebase
git pull --rebase
```

### 2. 手动 rebase

```bash
# 获取远程更新
git fetch origin

# 变基到目标分支
git rebase origin/dev
```

### 3. merge vs rebase

```
使用 merge:
dev:    A — B — C — F — G
                  ↘
feature:             D — E — H    (产生合并提交)

使用 rebase:
dev:    A — B — C — F — G
                        ↘
feature:                  D' — E'  (线性历史)
```

**rebase 优点**：历史更清晰，避免无意义的合并提交

**黄金法则**：只对未推送的提交使用 rebase

---

## 四、完成功能并合并

### 1. 推送功能分支

```bash
# 首次推送，设置上游分支
git push -u origin feature/user-login

# 后续推送
git push
```

### 2. 合并到 dev

```bash
# 切换到 dev
git checkout dev

# 更新 dev
git pull

# 合并功能分支
git merge feature/user-login

# 推送到远程
git push
```

### 3. 删除已完成的功能分支

```bash
# 删除本地分支
git branch -d feature/user-login

# 删除远程分支
git push origin --delete feature/user-login
```

---

## 五、发布版本

### 1. 从 dev 合并到 main

```bash
# 确保所有功能已合并到 dev
git checkout dev
git pull

# 切换到 main
git checkout main
git pull

# 合并 dev
git merge dev
git push
```

### 2. 创建版本标签

```bash
# 创建轻量标签
git tag v1.0.0

# 创建带注释的标签（推荐）
git tag -a v1.0.0 -m "Release version 1.0.0"

# 推送标签到远程
git push origin v1.0.0

# 推送所有标签
git push origin --tags
```

### 3. 版本号规范

建议使用语义化版本（Semantic Versioning）：

```
MAJOR.MINOR.PATCH

例：v1.2.3
- MAJOR（主版本）：不兼容的 API 变更
- MINOR（次版本）：向后兼容的功能新增
- PATCH（补丁）：向后兼容的 Bug 修复
```

---

## 六、紧急修复

### 场景：生产环境发现严重 Bug，需要快速修复

### 1. 从 main 创建修复分支

```bash
git checkout main
git pull
git checkout -b fix/critical-bug
```

### 2. 修复并提交

```bash
# 进行修复...
git add .
git commit -m "fix: 修复登录崩溃问题"
```

### 3. 合并到 main 并发布

```bash
git checkout main
git merge fix/critical-bug
git push

# 打补丁版本标签
git tag -a v1.0.1 -m "Hotfix: 修复登录崩溃"
git push origin v1.0.1
```

### 4. 同步回 dev（重要！）

```bash
git checkout dev
git merge main
git push
```

> **注意**：必须将修复同步回 dev，否则下次发布时 Bug 会重新出现。

---

## 七、查看历史和状态

### 1. 查看提交历史

```bash
# 完整历史
git log

# 简洁显示
git log --oneline

# 最近 N 条
git log --oneline -5

# 图形化显示分支
git log --oneline --graph --all

# 查看某文件的历史
git log path/to/file.cpp
```

### 2. 查看分支

```bash
# 列出本地分支
git branch

# 列出远程分支
git branch -r

# 列出所有分支（本地+远程）
git branch -a

# 查看分支跟踪关系
git branch -vv
```

### 3. 查看差异

```bash
# 查看工作区修改
git diff

# 查看已暂存的修改
git diff --staged

# 比较两个分支
git diff dev..main

# 查看某次提交的变更
git show <commit-hash>
```

---

## 下一步

- [团队协作流程](./03-team-collaboration.md) - 作为协作者加入团队
- [为开源项目贡献](./04-contribution.md) - Fork 项目并贡献代码
- [常见问题排查](./05-troubleshooting.md) - 撤销操作、解决冲突
