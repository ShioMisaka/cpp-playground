# 常见问题排查

本文档介绍 Git 开发中的常见问题及解决方法。

[← 返回目录](./README.md)

---

## 一、撤销工作区修改

### 场景：修改了文件但未 add，想丢弃修改

```bash
# 撤销单个文件的修改
git restore path/to/file.cpp

# 旧语法（也有效）
git checkout -- path/to/file.cpp
```

```bash
# 撤销所有工作区的修改（谨慎！）
git restore .
```

> **警告**：此操作永久丢弃未暂存的更改，无法恢复！

### 查看当前状态

```bash
git status
git diff                # 工作区与暂存区的差异
git diff --staged       # 暂存区与上次提交的差异
```

---

## 二、撤销已暂存的修改

### 场景：已 add 但未 commit，想取消暂存

```bash
# 取消暂存某个文件
git restore --staged path/to/file.cpp

# 旧语法
git reset HEAD path/to/file.cpp
```

```bash
# 取消所有暂存
git restore --staged .
```

> **注意**：此操作不会丢失文件内容，只是取消暂存，文件仍保留在工作区。

---

## 三、撤销提交（未推送）

### 场景：已 commit 但未 push，想修改或撤销

#### 1. 修改最后一次提交

**添加遗漏的文件**：

```bash
git add forgotten_file.cpp
git commit --amend
```

**仅修改提交信息**：

```bash
git commit --amend -m "新的提交信息"
```

#### 2. 撤销最后一次提交（保留修改）

```bash
git reset --soft HEAD~1
```

提交被撤销，但代码改动仍在工作区，可重新编辑后再次提交。

#### 3. 撤销最后一次提交（丢弃修改）

```bash
git reset --hard HEAD~1
```

> **警告**：所有更改将永久丢失！

#### 4. 撤销多次提交

```bash
# 撤销最近 3 次提交（保留修改）
git reset --soft HEAD~3

# 撤销最近 3 次提交（丢弃修改）
git reset --hard HEAD~3
```

---

## 四、撤销已推送的提交

### 场景：已 push 到远程，需要撤销

**不要使用 reset --hard**！这会重写历史，影响协作者。

#### 推荐方法：使用 revert

revert 创建一个"反向提交"，安全且可协作：

```bash
# 撤销最新的提交
git revert HEAD

# 撤销指定提交
git revert abc1234

# 撤销多次提交
git revert HEAD~3..HEAD
```

rebase 后可能需要解决冲突，然后：

```bash
git commit
git push
```

> **优点**：不改变历史，适合多人协作
> **缺点**：会新增提交记录

#### 紧急情况：强制推送（谨慎使用）

如果是个人分支或确认无人基于你的提交工作：

```bash
git reset --hard HEAD~1
git push --force-with-lease origin branch-name
```

> **警告**：
> - `--force-with-lease` 比 `--force` 稍安全
> - 绝不要对共享分支（main/dev）强制推送
> - 确认后，通知团队成员重新克隆

---

## 五、回退到历史版本

### 场景：想回到某个旧版本

#### 1. 查看历史

```bash
# 简洁显示历史
git log --oneline

# 图形化显示
git log --oneline --graph --all

# 查看某次提交的详情
git show abc1234
```

#### 2. 回退方式

**保留工作区修改（可重新提交）**：

```bash
git reset --soft abc1234
```

**保留修改但未暂存**：

```bash
git reset --mixed abc1234
# 或简写
git reset abc1234
```

**完全回退（丢弃之后所有更改）**：

```bash
git reset --hard abc1234
```

#### 3. 创建新分支指向历史版本

**推荐**：不影响当前分支，可随时切换回最新：

```bash
git checkout -b old-version abc1234
```

---

## 六、解决合并冲突

### 场景：merge 或 rebase 时出现冲突

#### 冲突标记示例

```cpp
<<<<<<< HEAD
int x = 1;  // 当前分支的代码
=======
int x = 2;  // 要合并分支的代码
>>>>>>> feature-branch
```

#### 解决步骤

1. **查看冲突文件**

```bash
git status
```

2. **编辑冲突文件**

打开冲突文件，搜索 `<<<<<<<` 标记，选择需要的代码，删除标记：

```cpp
int x = 1;  // 保留这个
```

3. **标记冲突已解决**

```bash
git add path/to/file.cpp
```

4. **继续合并**

```bash
# 如果是 merge
git commit

# 如果是 rebase
git rebase --continue
```

#### 中止合并

如果不想合并了：

```bash
# 中止 merge
git merge --abort

# 中止 rebase
git rebase --abort
```

---

## 七、Rebase 冲突处理

### 场景：rebase 过程中遇到冲突

#### 处理步骤

```bash
# 开始 rebase
git rebase origin/dev

# 出现冲突，解决冲突文件
vim conflict_file.cpp

# 添加解决后的文件
git add conflict_file.cpp

# 继续 rebase
git rebase --continue

# 重复以上步骤，直到 rebase 完成
```

#### 跳过某个提交

如果某个提交不想应用：

```bash
git rebase --skip
```

#### 中止 rebase

回到 rebase 前的状态：

```bash
git rebase --abort
```

---

## 八、查看和恢复丢失的提交

### 场景：误删提交或 reset 过头

#### 查找丢失的提交

```bash
# 查看所有操作记录（包括被删除的提交）
git reflog

# 或查看最近的操作
git reflog -10
```

输出示例：
```
abc1234 HEAD@{0}: commit: 添加新功能
def5678 HEAD@{1}: rebase finished: returning to refs/heads/main
ghi9012 HEAD@{2}: commit: 另一个功能
```

#### 恢复丢失的提交

```bash
# 创建分支指向丢失的提交
git branch recovery-branch abc1234

# 或直接 reset（小心）
git reset --hard abc1234
```

---

## 九、清理仓库

### 1. 清理未跟踪的文件

```bash
# 查看将被删除的文件
git clean -n

# 删除未跟踪的文件
git clean -f

# 删除未跟踪的文件和目录
git clean -fd

# 删除被 .gitignore 忽略的文件
git clean -fdX
```

### 2. 清理已合并的本地分支

```bash
# 查看已合并的分支
git branch --merged

# 删除已合并的分支（保留 main 和 dev）
git branch --merged | grep -v "main\|dev" | xargs git branch -d
```

### 3. 清理远程分支引用

```bash
# 删除远程已不存在的本地分支引用
git remote prune origin
```

---

## 十、其他常见问题

### 1. 文件名大小写问题

Git 默认不区分大小写，导致重命名失败。

```bash
# 配置 Git 区分大小写
git config core.ignorecase false

# 然后重命名
git mv OldName.cpp NewName.cpp
git commit -m "refactor: 重命名文件"
```

### 2. 提交了敏感信息

```bash
# 方法1：使用 git-filter-branch（复杂）
git filter-branch --force --index-filter \
  "git rm --cached --ignore-unmatch path/to/secret" \
  --prune-empty --tag-name-filter cat -- --all

# 方法2：使用 BFG Repo-Cleaner（推荐，需要下载）
bfg --delete-files secret.txt
git reflog expire --expire=now --all
git gc --prune=now --aggressive

# 强制推送（通知团队成员重新克隆）
git push --force
```

### 3. .gitignore 不生效

文件已被跟踪，需要先移除：

```bash
# 从跟踪中移除（保留文件）
git rm --cached path/to/file

# 从跟踪中移除（删除文件）
git rm path/to/file

# 提交
git commit -m "chore: 添加到 .gitignore"
```

### 4. 恢复被删除的文件

```bash
# 从暂存区恢复
git restore path/to/deleted_file.cpp

# 从历史提交恢复
git checkout abc1234 -- path/to/deleted_file.cpp

# 查找文件在哪个提交中
git log --follow -- path/to/deleted_file.cpp
```

### 5. 查看某行代码是谁写的

```bash
# 查看文件的每一行是谁修改的
git blame path/to/file.cpp

# 查看特定行范围
git blame -L 10,20 path/to/file.cpp
```

---

## 十一、性能优化

### 1. 仓库过大

```bash
# 垃圾回收，压缩仓库
git gc

# 激进压缩（更彻底）
git gc --aggressive --prune=now

# 查看仓库大小
du -sh .git
```

### 2. 克隆大仓库慢

```bash
# 浅克隆（只获取最新提交）
git clone --depth 1 git@github.com:user/repo.git

# 只克隆特定分支
git clone -b main --single-branch git@github.com:user/repo.git

# 组合使用
git clone -b main --single-branch --depth 1 git@github.com:user/repo.git
```

### 3. 拉取太慢

```bash
# 只拉取特定分支
git fetch origin main

# 不拉取标签
git fetch --no-tags origin
```

---

## 下一步

- [日常开发工作流](./02-daily-workflow.md) - 正常的开发流程
- [团队协作流程](./03-team-collaboration.md) - Code Review 和协作
