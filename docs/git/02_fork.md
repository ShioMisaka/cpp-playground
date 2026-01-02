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

# 只克隆 dev 分支（完整历史）
git clone -b dev --single-branch https://github.com/{贡献者}/your-repo.git

# 只克隆 dev 分支（最新代码）
git clone -b dev --single-branch --depth 1 https://github.com/{贡献者}/your-repo.git

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

远程分支
```bash
git branch -r 
# 输出
    origin/HEAD -> origin/main
    origin/dev
    origin/main
    upstream/main

git fetch upstream main

git log --oneline
# 输出:
    c2395ef18 (HEAD -> dev, origin/main, origin/dev, origin/HEAD, main) test:delete test.py
    448916790 Merge branch 'dev' of github.com:ShioMisaka/ultralytics into dev
    9e80aac27 test:add bifpn_test
    79c6f1486 feature:添加了BiFPN
    432efee9a (upstream/main) Update pyproject.toml to zensical>=0.0.15 (#23052)
    ae859fbd8 Add note about background class for YOLO-World models (#23058)
```

将原仓库的 main 合并到你本地的 main 中
```bash
git checkout main 
git fetch upstream main

# 或用 rebase：
git rebase upstream/main

# 推送到自己的远程分支上
git push -u origin main  # -u 默认绑定到origin/main分支上
# 如果已经绑定到自己的仓库了，直接push
git push
```

### 4️⃣ 开发
切换到 `dev` 分支，或者用功能特性作为分支名（例如， `fix-issue-123`, `add-feature-xyz`）。
```bash
git chcekout -b dev # -b

git push -u origin dev # 给自己的远程仓库创建一个远程分支
```

### 5️⃣ 创建 Pull Request（PR）
完成了一个功能后，就可以向主仓库提交的你PR了。

在 GitHub 你的仓库页面：
1. base repository：你的仓库
2. base branch：dev
3. compare branch：贡献者的 feature 分支

📌 注意：请 PR 到 dev 分支，而是 main

---