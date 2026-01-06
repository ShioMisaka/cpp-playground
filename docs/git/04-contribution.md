# 为开源项目贡献

本文档介绍如何通过 Fork 为开源项目贡献代码。

[← 返回目录](./README.md)

---

## 一、Fork 工作流概述

```
原始仓库 (upstream)  →  你的仓库 (origin)  →  本地仓库
    ↑                        ↑                      ↑
    └────────────────────────┴──────────────────────┘
              Pull Request 的方向
```

角色说明：
- **upstream**：原作者的仓库（你 Fork 的来源）
- **origin**：你 Fork 后的仓库
- **local**：你本地的代码库

---

## 二、Fork 仓库

### 1. 在 GitHub 上 Fork

1. 访问目标仓库页面
2. 点击右上角 **Fork** 按钮
3. 选择你的账号作为目标
4. 等待 Fork 完成

Fork 后，你在自己的账号下拥有一份完整的仓库副本。

### 2. 了解仓库规范

在开始前，先阅读：
- `README.md` - 项目介绍
- `CONTRIBUTING.md` - 贡献指南
- `LICENSE` - 开源协议

查看是否有：
- Issue 模板
- PR 模板
- Code of Conduct

---

## 三、克隆你的仓库

### 克隆到本地

```bash
# 克隆你 Fork 的仓库
git clone git@github.com:your-username/project-name.git

# 进入项目目录
cd project-name
```

### 只克隆特定分支（可选）

如果仓库很大，可以只克隆需要的分支：

```bash
# 克隆 dev 分支（完整历史）
git clone -b dev --single-branch git@github.com:your-username/project-name.git

# 克隆 dev 分支（最新代码，节省空间）
git clone -b dev --single-branch --depth 1 git@github.com:your-username/project-name.git
```

---

## 四、添加上游仓库

添加 upstream 后，你可以获取原仓库的最新更新。

### 添加 upstream

```bash
git remote add upstream git@github.com:original-owner/project-name.git
```

### 验证远程仓库

```bash
git remote -v
```

输出示例：
```
origin    git@github.com:your-username/project-name.git (fetch)
origin    git@github.com:your-username/project-name.git (push)
upstream  git@github.com:original-owner/project-name.git (fetch)
upstream  git@github.com:original-owner/project-name.git (push)  # 无权限
```

注意：你对 `upstream` 没有推送权限，这是正常的。

---

## 五、同步上游代码

原仓库会不断更新，你需要定期同步到本地。

### 1. 获取上游更新

```bash
# 获取上游仓库的所有分支信息
git fetch upstream
```

### 2. 查看远程分支

```bash
git branch -r
```

输出示例：
```
origin/HEAD -> origin/main
origin/dev
origin/main
upstream/dev
upstream/main
```

### 3. 合并上游更新到本地

```bash
# 切换到主分支
git checkout main

# 合并上游的 main 分支
git merge upstream/main

# 或使用 rebase（推荐，保持历史整洁）
git rebase upstream/main
```

### 4. 推送到你的仓库

```bash
# 将更新推送到你的 origin
git push origin main
```

---

## 六、开发功能

### 1. 创建功能分支

从最新的 main 或 dev 创建分支：

```bash
# 确保本地 main 是最新的
git checkout main
git pull origin main

# 如果有 upstream，先同步
git fetch upstream
git rebase upstream/main
git push origin main

# 创建功能分支
git checkout -b feature/your-feature-name
```

分支命名建议：
- `feature/add-xxx` - 添加新功能
- `fix/xxx-bug` - 修复 Bug
- `docs/update-xxx` - 更新文档
- `refactor/xxx-module` - 重构模块

### 2. 开发并提交

```bash
# 进行开发...

# 查看修改
git status
git diff

# 暂存并提交
git add .
git commit -m "feat: 添加某个功能"
```

提交规范遵循项目的约定（参考 [02-daily-workflow.md](./02-daily-workflow.md#提交规范)）

### 3. 推送到你的仓库

```bash
# 首次推送，设置上游分支
git push -u origin feature/your-feature-name

# 后续推送
git push
```

---

## 七、提交 Pull Request

### 1. 在 GitHub 上创建 PR

1. 访问你 Fork 的仓库页面
2. 点击 **Contribute** → **Open pull request**
   或手动：**Pull requests** → **New pull request**

3. 确保方向正确：
   - **base repository**: `original-owner/project-name`
   - **base**: `main` 或 `dev`（查看项目规范）
   - **compare**: `your-username:feature/your-feature-name`

4. 点击 **Create pull request**

### 2. 填写 PR 信息

标题示例：
```
[Feature] 添加某个新功能
[Fix] 修复某个 Bug
```

描述模板：
```markdown
## 变更类型
- [ ] feat (新功能)
- [ ] fix (修复)
- [ ] docs (文档)
- [ ] refactor (重构)
- [ ] test (测试)
- [ ] chore (构建/工具)

## 变更说明
简要描述本次变更的内容和目的。

## 相关 Issue
Closes #123  # 或 Fixes #123, Relates to #456

## 测试
- [ ] 已通过本地测试
- [ ] 添加了单元测试
- [ ] 更新了文档

## 截图
（如有 UI 变更，添加截图）
```

### 3. 等待审查

PR 提交后：
- 项目维护者会审查代码
- 可能会提出修改意见
- 根据反馈修改代码
- 修改会自动更新 PR

---

## 八、处理 PR 反馈

### 1. 修改代码

根据反馈修改代码：

```bash
git checkout feature/your-feature-name

# 进行修改...
git add .
git commit -m "fix: 根据反馈修改"
git push
```

### 2. 继续讨论

在 PR 页面：
- 回复审查意见
- 解释你的做法
- 必要时请求进一步指导

### 3. 更新 PR

如果你的分支落后于上游：

```bash
# 同步最新的 upstream
git fetch upstream
git rebase upstream/main

# 解决可能的冲突
# ...

# 强制推送（因为是你的分支）
git push --force-with-lease origin feature/your-feature-name
```

---

## 九、PR 合并后

### 1. 删除本地分支

```bash
# 删除已合并的功能分支
git branch -d feature/your-feature-name
```

### 2. 删除远程分支

PR 合并后，你可以在 GitHub 上删除分支，或：

```bash
git push origin --delete feature/your-feature-name
```

### 3. 继续贡献

```bash
# 同步最新的 upstream
git checkout main
git fetch upstream
git rebase upstream/main
git push origin main

# 开始新的贡献
git checkout -b feature/next-feature
```

---

## 十、最佳实践

### 1. 开始贡献前

- [ ] 阅读 CONTRIBUTING.md
- [ ] 查看 Issues，找未解决的问题
- [ ] 大改动先创建 Issue 讨论
- [ ] 确认项目仍在维护

### 2. 开发中

- [ ] 遵循项目的代码风格
- [ ] 添加必要的测试
- [ ] 更新相关文档
- [ ] 保持提交原子性（一个提交做一件事）

### 3. 提交 PR 前

- [ ] 确保代码能通过 CI
- [ ] 检查是否有合并冲突
- [ ] PR 描述清晰完整
- [ ] 关联相关 Issue

### 4. 定期同步

```bash
# 每次开始新功能前
git checkout main
git fetch upstream
git rebase upstream/main
git push origin main
```

---

## 常见问题

### Q: PR 被拒绝怎么办？

A：不要气馁，询问原因：
- 是否不符合项目方向
- 代码质量问题
- 需要更多测试

根据反馈改进后可以重新提交。

### Q: 长期未收到回复怎么办？

A：在 PR 中友好地 @ 维护者，或：
- 查看 Issues 是否有类似讨论
- 在项目讨论区提问
- 考虑是否项目已不再维护

### Q: 如何知道项目欢迎贡献？

A：查看：
- Issues 是否活跃
- 近期 PR 是否被处理
- README/CONTRIBUTING.md 的说明

---

## 下一步

- [日常开发工作流](./02-daily-workflow.md) - 提交规范、分支管理
- [常见问题排查](./05-troubleshooting.md) - 撤销操作、解决冲突
