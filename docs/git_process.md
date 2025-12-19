# 一、作为贡献者开始
> 如果你是作为贡献者开发请阅读这一章

## 1️⃣ Fork 一下仓库
### 贡献者在GitHub上:
 1. 打开仓库
 2. 点击右上角的 Fork 
 3. 在自己的账号下生成一份仓库

    Fork后:
    - 管理者仓库 = upsream
    - 贡献者仓库 = origin

## 2️⃣ Clone 自己 Fork 的仓库
```bash
git clone https://github.com/{贡献者}/your-repo.git
cd your-repo
```

## 3️⃣ 添加 upstream
添加后才能不断同步主仓库的最新代码
```bash
git remote add upstream https://github.com/你用户名/your-repo.git
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
然后就可以开始日常开发啦，等到你的功能完成的差不多

---



# 三、日常开发

## ✨ 1. 开始一个新的功能
### 切换到 dev 并从远程更新

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

### 从 dev 创建功能分支(仅在本地有)
```bash
git checkout -b feature/user-login
```
命名建议：
- feature/xxx
- 用动词或模块名

---

## 🧑‍💻 2. 写代码 & 提交
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

## 🔀 3. 功能完成 -> 合并回 develop

```bash
git checkout dev
git pull

git merge feature/user-login
git push
```

### 删除功能分支(根据自己的需要)
```bash
git branch -d feature/user-login
```
---

## 🚀 4. 发布一个版本（dev → main）

当你觉得：
> “这个版本可以发布了”

### 切到 main
```bash
git checkout main
git pull
```

### 合并 dev
```
git merge dev
git push
```

### 打 Tag
```bash
git tag v1.0.0
git push origin v1.0.0
```
---

## 🛠️ 五、线上 Bug 修复流程
场景：
- main 已上线
- 发现严重 bug

### 从 main 拉修复分支
```bash
git checkout main
git pull
git checkout -b fix/login-crash
```

### 修复 & 提交
```bash
git commit -am "fix: 修复登录时崩溃问题"
```

### 合并回 main 并发布
```bash
git checkout main
git merge fix/login-crash
git push
```

### ⚠️ 同步回 dev（非常重要）
```bash
git checkout dev
git merge main
git push
```

---