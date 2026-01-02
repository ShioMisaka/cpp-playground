## 基础 Git 身份配置
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

| 层级           | 命令                  | 存储位置          | 作用范围 |
| --------------| --------------------- | ---------------- | -------  |
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
git config --global user.name "Alice"           # 修改全局用户名

git config user.email "alice@work.com"          # 修改当前仓库邮箱

git config --list                               # 查看所有配置

git config --list --show-origin                 # 查看配置来源

git config --global --list                      # 查看全局配置

git config user.name                            # 查看某一项值

git config --global core.editor "vim"           # 修改默认打开编辑器
git config --global core.editor "code --wait"   
```