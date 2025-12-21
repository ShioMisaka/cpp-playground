# Git 开发流程（以 GitHub 为例）

## 一、基础 Git 身份配置
###  1️⃣ 配置 SSH
####  1. 本地生成 SSH Key

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```
生成后文件一般在：
```bash
~/.ssh/id_ed25519
~/.ssh/id_ed25519.pub
```

#### 2. 把公钥添加到 GitHub
```bash
cat ~/.ssh/id_ed25519.pub
```
GitHub：
- Settings → SSH and GPG keys
- New SSH key
- 粘贴内容 → Save

#### 3. 测试是否绑定成功
```bash
ssh -T git@github.com
```
输出：
```txt
Hi username! You've successfully authenticated.
```

### 2️⃣ 用户信息设置

在开发前还需要设置一下信息

| 层级         | 命令                    | 存储位置             | 作用范围 |
| ---------- | --------------------- | ---------------- | ---- |
| 本地（local）  | `git config`          | `.git/config`    | 当前仓库 |
| 全局（global） | `git config --global` | `~/.gitconfig`   | 当前用户 |
| 系统（system） | `git config --system` | `/etc/gitconfig` | 所有用户 |

- 设置用户信息
```bash
# 设置本地用户信息（local），需要进入任意Git仓库中设置
git config user.name "username"
git config user.email "github_email@gmail.com"

# 设置全局用户信息（global）
git config --global user.name "username"
git config user.email --global "github_email@gmail.com"

# 检查一下当前的 Git 用户信息
git config user.name
git config user.email
```

📌 注意：在设置 Git 的邮箱时，要将邮箱设置为自己在 Github 上认证过的邮箱，这样 GitHub 就会认为这个提交属于你，并在提交旁边显示你的头像和 GitHub 用户名。

> 其他指令
```bash
git config --global user.name "Alice"   # 修改全局用户名

git config user.email "alice@work.com"  # 修改当前仓库邮箱

git config --list                       # 查看所有配置

git config --list --show-origin         # 查看配置来源

git config --global --list              # 查看全局配置

git config user.name                    # 查看某一项值
```
## 二、作为 Fork 贡献者开始
> 如果你是作为贡献者开发请阅读这一章

### 1️⃣ Fork 一下仓库
### 贡献者在GitHub上:
 1. 打开仓库
 2. 点击右上角的 Fork 
 3. 在自己的账号下生成一份仓库

    Fork后:
    - 管理者仓库 = upsream
    - 贡献者仓库 = origin

### 2️⃣ Clone 自己 Fork 的仓库
```bash
git clone https://github.com/{贡献者}/your-repo.git
cd your-repo
```

### 3️⃣ 添加 upstream
添加后才能不断同步主仓库的最新代码
```bash
git remote add upstream https://github.com/{管理者}/your-repo.git
```

这时候执行 `git remote -v`
```bash
git remote -v
输出:
origin   https://github.com/{贡献者}/repo.git    (fetch)
origin   https://github.com/{贡献者}/repo.git    (push)
upstream https://github.org/{管理者}/repo.git    (fetch)
upstream https://github.org/{管理者}/repo.git    (push)  # ❌ 需要权限
```
### 4️⃣ 开发
然后就可以开始在自己的仓库中日常开发啦，跳转到第三章，等到你的功能完成的差不多

### 5️⃣ 创建 Pull Request（PR）
完成了一个功能后，就可以向主仓库提交的你PR了。

在 GitHub 你的仓库页面：
1. base repository：你的仓库
2. base branch：dev
3. compare branch：贡献者的 feature 分支

📌 注意：请 PR 到 dev 分支，而是 main

---

## 三、作为合作者&团队开始

### 1️⃣ 等仓库负责人邀请你

#### ✅ 邀请协作者
- GitHub → Settings → Collaborators
- 或 Organization → Teams
#### ✅ 设置分支保护
保护 `main` 和 `dev`：
- Require pull request before merging
- Require at least 1 review
- 禁止 force push

### 2️⃣ 克隆仓库
```bash
git clone https://github.com/yourname/project.git
cd project

git remote -v # 查看远程
```

然后就可以开始日常开发了

---



## 四、日常开发

### ✨ 1️⃣ 开始一个新的功能

#### 切换到 dev 并从远程更新

这里我们为了方便，可以让自己的本地分支跟踪远程的分支

```bash
git checkout dev
git pull  # 前提是跟踪了上游分支

git branch -vv # 查看上游分支跟踪情况

# 若没有跟踪上游分支则，直接手动选择需要pull的上游分支
git pull origin dev

# 进行跟踪
git branch -u origin/dev

# 取消跟踪
git branch --unset-upstream

```

#### 从 dev 创建功能分支(仅在本地有)
```bash
git checkout -b feature/user-login
```
命名建议：
- feature/xxx
- 用动词或模块名

---

### 🧑‍💻 2️⃣ 写代码 & 提交

```bash
git add .
git commit -m "feat: 实现用户登录功能"
```
> 建议从现在开始就养成规范提交习惯

- `feat`: 新功能
- `fix`: 修复 bug
- `docs`: 文档
- `refactor`: 重构
- `test`: 测试
---

当开发途中需要同步：
```bash
git fetch origin
git rebase origin/dev
```

使用 `rebase` 来拉取新代码

当期你正在开发 D-E 的内容，但是 dev 有新的 F-G 内容。

```txt
dev:    A — B — C — F — G
                  ↘
feature:             D — E
```

使用 `rebase` 来同步
```bash
git checkout dev
git pull

git checkout feature
git rebase dev
```

同步后，D-E 就会

```
dev:    A — B — C — F — G
                          ↘
feature:                    D' — E'   ← D 和 E 被“重新应用”
```

✅ 历史是一条直线，清晰简洁

✅ 看起来就像你是在 G 之后才开始开发 feature

> 注意：D' 和 E' 是新的提交（哈希值变了）
>
> ⚠️ 黄金法则：
>
> 只对尚未推送到远程的提交，或只有你自己在用的分支，使用 rebase。
>
> 因为 rebase 会改变提交的哈希值，如果别人已经基于旧提交工作，会导致混乱。


### 🔀 3️⃣ 功能完成 -> 合并回 dev

```bash
git checkout dev
git pull

git merge feature/user-login
git push
```

#### 删除功能分支(根据自己的需要)
```bash
git branch -d feature/user-login
```
---

### 🚀 4️⃣ 发布一个版本（dev → main）

当你觉得：
> “这个版本可以发布了”

#### 切到 main
```bash
git checkout main
git pull
```

#### 合并 dev
```
git merge dev
git push
```

#### 打 Tag
```bash
git tag v1.0.0
git push origin v1.0.0
```
---

## 🛠️ 五、线上 Bug 修复流程
场景：
- main 已上线
- 发现严重 bug

#### 从 main 拉修复分支
```bash
git checkout main
git pull
git checkout -b fix/login-crash
```

#### 修复 & 提交
```bash
git commit -am "fix: 修复登录时崩溃问题"
```

#### 合并回 main 并发布
```bash
git checkout main
git merge fix/login-crash
git push
```

#### ⚠️ 同步回 dev（非常重要）
```bash
git checkout dev
git merge main
git push
```

---

## 六、撤销更改

### 1️⃣ 撤销工作区的修改（尚未 git add）

如果你修改了文件但还没 `git add`，想丢弃这些更改：

```bash
# 撤销单个文件的修改
git checkout -- <file>

# 或（推荐新语法）
git restore <file>
```

```bash
# 撤销所有工作区的修改（谨慎！）
git restore .
# 或
git checkout -- .
```
> ⚠️ 这会永久丢弃你未暂存的更改，无法恢复！

### 2️⃣ 撤销已暂存的更改（已 git add，但未提交）

如果你已经 `git add`，但想取消暂存，回到工作区：

```bash
# 取消暂存某个文件
git restore --staged <file>

# 或旧语法
git reset HEAD <file>
```
```bash
# 取消暂存某个文件
git restore --staged <file>

# 或旧语法
git reset HEAD <file>
```
> 此操作不会丢失文件内容，只是取消暂存，文件仍保留在工作区。

### 3️⃣ 撤销最近一次提交（已 commit，但未 push）

如果你刚提交了，但发现有问题，想修改这次提交：

#### 1. 修改最后一次提交（保留更改）
```bash
# 修改提交信息或添加遗漏文件
git add <forgotten-file>
git commit --amend
```

#### 2. 完全撤销最后一次提交，保留修改在工作区
```bash
git reset --soft HEAD~1
```
> 这样提交被撤销，但代码改动还在工作区，可以重新提交。

#### 3. 完全撤销最后一次提交并丢弃所有更改
```bash
git reset --hard HEAD~1
```
> ⚠️ 所有更改将永久丢失！


### 4️⃣ 撤销已推送的提交（已 push 到远程）

如果已经 `push` 到远程仓库，不建议使用 `reset --hard`，因为会重写历史，影响协作。

推荐使用 `git revert`（安全、可协作）：

```bash
# 撤销指定提交，生成一个“反向”提交
git revert <commit-hash>
```
例如：
```bash
git revert HEAD        # 撤销最新一次提交
git revert abc1234     # 撤销某次具体提交
```

> ✅ 优点：不会改变历史，适合多人协作。
>
>❌ 缺点：会新增一个提交记录。

### 5️⃣ 回退到某个历史版本（谨慎使用）

如果想回到某个旧版本，并放弃之后的所有更改：

```bash
# 查看历史
git log --oneline

# 回退到指定提交（保留工作区更改）
git reset --soft <commit>

# 彻底回退（丢弃之后所有更改）
git reset --hard <commit>

# 如果已推送，还需强制推送（危险！）
git push --force-with-lease origin <branch>
```
