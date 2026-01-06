# 新手入门：配置到首次建仓

本文档适合首次使用 Git 的用户，从配置环境开始，到创建第一个项目并上传到 GitHub。

[← 返回目录](./README.md)

---

## 一、配置 SSH

SSH 密钥让你免密码推送代码到 GitHub。

### 1. 生成 SSH 密钥

```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```

生成后文件位置：
- 私钥：`~/.ssh/id_ed25519`
- 公钥：`~/.ssh/id_ed25519.pub`

### 2. 添加公钥到 GitHub

```bash
cat ~/.ssh/id_ed25519.pub
```

在 GitHub 上：
1. 点击右上角头像 → **Settings**
2. 左侧菜单 → **SSH and GPG keys**
3. 点击 **New SSH key**
4. 粘贴公钥内容 → **Add SSH key**

### 3. 测试连接

```bash
ssh -T git@github.com
```

成功输出：
```
Hi username! You've successfully authenticated...
```

---

## 二、配置用户信息

Git 需要知道你是谁，以便在提交时标注作者。

### 配置层级说明

| 层级 | 命令 | 存储位置 | 作用范围 |
|------|------|---------|---------|
| 本地 | `git config` | `.git/config` | 当前仓库 |
| 全局 | `git config --global` | `~/.gitconfig` | 当前用户所有仓库 |
| 系统 | `git config --system` | `/etc/gitconfig` | 系统所有用户 |

### 设置全局用户信息（推荐）

```bash
git config --global user.name "your_username"
git config --global user.email "your_email@example.com"
```

验证配置：

```bash
git config user.name
git config user.email
```

查看所有配置：

```bash
git config --list
```

> **注意**：邮箱应使用 GitHub 认证过的邮箱，这样提交会在 GitHub 上显示你的头像和用户名。

### 其他常用配置

```bash
# 设置默认编辑器
git config --global core.editor "vim"
git config --global core.editor "code --wait"   # VS Code

# 查看配置来源
git config --list --show-origin
```

---

## 三、创建新项目

### 场景：本地已有代码，想上传到 GitHub

#### 1. 在 GitHub 创建空仓库

1. GitHub 右上角 **+** → **New repository**
2. 填写仓库名称
3. **不要**勾选 "Add a README file"
4. 点击 **Create repository**

#### 2. 初始化本地 Git 仓库

```bash
cd your-project
git init
```

#### 3. 添加文件并首次提交

```bash
git add .
git commit -m "feat: 初始提交"
```

#### 4. 关联远程仓库

```bash
git remote add origin git@github.com:username/repo-name.git
```

#### 5. 推送到远程

```bash
# 首次推送，设置上游分支
git push -u origin main

# 如果 GitHub 创建的默认分支是 main，但本地是 master
git branch -M main
git push -u origin main
```

---

## 四、克隆现有项目

### 场景：从 GitHub 克隆项目到本地

```bash
# 克隆整个仓库
git clone git@github.com:username/repo-name.git

# 克隆并指定目录名
git clone git@github.com:username/repo-name.git my-folder

# 只克隆特定分支（完整历史）
git clone -b dev --single-branch git@github.com:username/repo-name.git

# 只克隆特定分支（最新代码，节省空间）
git clone -b dev --single-branch --depth 1 git@github.com:username/repo-name.git
```

---

## 五、初始分支结构建议

对于新项目，建议建立以下分支结构：

```bash
# 创建并切换到 dev 分支
git checkout -b dev

# 推送 dev 分支到远程
git push -u origin dev
```

典型分支模型：
- `main` - 生产环境，稳定版本
- `dev` - 开发环境，日常合并
- `feature/*` - 功能分支，从 `dev` 拉出

---

## 下一步

配置完成并熟悉基础操作后，继续阅读：

- [日常开发工作流](./02-daily-workflow.md) - 学习提交规范、分支管理
- [团队协作流程](./03-team-collaboration.md) - 作为协作者加入团队
- [为开源项目贡献](./04-contribution.md) - Fork 项目并贡献代码
