# 团队协作流程

本文档适合作为项目协作者，介绍如何在团队中协作开发。

[← 返回目录](./README.md)

---

## 一、加入团队

### 1. 接受协作者邀请

仓库管理员会通过以下方式邀请你：

**个人仓库协作者**：
- GitHub → Settings → Collaborators and teams → Add people

**组织仓库**：
- Organization → Teams → 被加入相应团队

收到邀请后，检查邮箱并接受邀请。

### 2. 确认权限

```bash
git clone git@github.com:organization/project.git
cd project

# 查看远程仓库
git remote -v
```

输出示例：
```
origin  git@github.com:organization/project.git (fetch)
origin  git@github.com:organization/project.git (push)
```

作为协作者，你有权限直接推送到仓库。

---

## 二、团队分支策略

### 典型分支模型

```
main (生产) ←←←←←←←←←←← 稳定版本
  ↑
  └─ merge (PR/Code Review)
      ↑
      dev (开发) ←←←←←← 日常合并
        ↑
        └─ merge (PR/Code Review)
            ↑
            feature/*, fix/* ←← 开发分支
```

### 分支权限规范

| 分支 | 推送权限 | 合并方式 |
|------|---------|---------|
| `main` | 仅管理员 | PR + Code Review |
| `dev` | 受限或 PR | PR 或直接推送 |
| `feature/*` | 开发者 | PR 或直接推送 |

---

## 三、保护分支设置

仓库管理员应设置分支保护：

### 在 GitHub 设置

1. 进入仓库 → **Settings** → **Branches**
2. 点击 **Add branch protection rule**
3. 选择要保护的分支（`main`, `dev`）

### 推荐配置

```
✅ Require a pull request before merging
  ✅ Require approvals (1 review)
  ✅ Dismiss stale reviews

✅ Require status checks to pass before merging
  选择必需的 CI 检查

❌ Do not allow bypassing the above settings

✅ Require branches to be up to date before merging

✅ Require conversation resolution before merging

❌ Restrict who can push to matching branches (仅管理员)
```

### 禁止 Force Push

```
✅ Do not allow bypassing the above settings
```

这防止意外或恶意重写历史。

---

## 四、协作开发流程

### 1. 从远程克隆项目

```bash
# 克隆完整仓库
git clone git@github.com:organization/project.git

# 只克隆 dev 分支（节省空间）
git clone -b dev --single-branch git@github.com:organization/project.git

cd project
```

### 2. 设置跟踪分支

```bash
# 查看 upstream 跟踪关系
git branch -vv

# 如果没有跟踪，设置跟踪
git branch -u origin/dev

# 取消跟踪
git branch --unset-upstream
```

### 3. 创建功能分支

```bash
# 确保本地 dev 是最新的
git checkout dev
git pull

# 创建功能分支
git checkout -b feature/add-payment
```

### 4. 开发并推送

```bash
# 开发中...
git add .
git commit -m "feat: 添加支付功能"

# 推送到远程
git push -u origin feature/add-payment
```

---

## 五、Pull Request 工作流

使用 PR 进行代码审查是团队协作的最佳实践。

### 1. 创建 Pull Request

在 GitHub 上：
1. 切换到你刚推送的分支
2. 点击 **Compare & pull request**
3. 填写 PR 信息：
   - **Base**: `dev` (合并目标)
   - **Compare**: `feature/add-payment` (你的分支)
   - **Title**: 简短描述
   - **Description**: 详细说明变更内容

### 2. PR 标题规范

```
[类型] 简短描述

例：
[feat] 添加支付功能
[fix] 修复登录超时问题
[refactor] 重构用户模块
```

### 3. PR 描述模板

```markdown
## 变更类型
- [ ] feat (新功能)
- [ ] fix (修复)
- [ ] docs (文档)
- [ ] refactor (重构)
- [ ] test (测试)

## 变更说明
简要描述本次变更的内容和目的。

## 测试
- [ ] 已通过本地测试
- [ ] 已添加单元测试
- [ ] 已更新相关文档

## 截图（如有）
（添加截图或 GIF）

## 相关 Issue
Closes #123
```

### 4. 请求审查

在 PR 右侧 **Reviewers** 添加审查者。

### 5. 处理审查意见

- 根据反馈修改代码
- 提交修改会自动更新 PR
- 审查通过后等待合并

---

## 六、Code Review 注意事项

### 作为审查者

1. **检查代码质量**
   - 逻辑是否正确
   - 命名是否清晰
   - 是否有潜在 Bug

2. **检查测试覆盖**
   - 是否添加了测试
   - 测试是否充分

3. **检查文档**
   - API 文档是否更新
   - README 是否需要修改

### 作为被审查者

1. **保持开放心态**
   - 讨论技术，不针对个人
   - 解释为什么这样做

2. **及时响应**
   - 优先处理审查意见
   - 不确定的问题讨论后决定

---

## 七、解决合并冲突

### 场景：PR 合并时发生冲突

```
A---B---C  (dev)
         \
          D---E  (feature/xxx)
         /
    F---G  (dev 有新提交)
```

### 解决步骤

1. **更新你的分支**

```bash
git checkout feature/xxx
git fetch origin
git rebase origin/dev
```

2. **解决冲突**

Git 会提示冲突文件：

```bash
# 查看冲突文件
git status

# 编辑冲突文件，搜索 <<<<<<< 标记
vim conflict_file.cpp
```

冲突标记：
```cpp
<<<<<<< HEAD
// dev 分支的内容
int x = 1;
=======
// 你的分支的内容
int x = 2;
>>>>>>> feature/xxx
```

保留需要的代码，删除标记：

```cpp
int x = 1;  // 或 x = 2，根据需要选择
```

3. **标记冲突已解决**

```bash
git add conflict_file.cpp
git rebase --continue
```

4. **强制推送（rebase 后需要）**

```bash
git push --force-with-lease
```

> **注意**：使用 `--force-with-lease` 比 `--force` 更安全，它会检查远程是否有他人推送。

---

## 八、团队最佳实践

### 1. 提交前检查清单

- [ ] 代码已通过本地编译/测试
- [ ] 提交信息符合规范
- [ ] 没有调试代码（`console.log`、`printf` 等）
- [ ] 没有注释掉的代码
- [ ] 敏感信息已移除（密钥、密码）

### 2. 保持分支整洁

```bash
# 定期清理已合并的本地分支
git branch --merged | grep -v "main\|dev" | xargs git branch -d

# 清理已删除的远程分支引用
git remote prune origin
```

### 3. 及时同步

```bash
# 每天开始工作前
git checkout dev
git pull

# 每天结束工作前
git push
```

### 4. 沟通优先

- 大功能开始前在团队讨论
- 不确定的设计及时询问
- PR 描述写清楚变更原因

---

## 下一步

- [日常开发工作流](./02-daily-workflow.md) - 提交规范、分支管理
- [为开源项目贡献](./04-contribution.md) - Fork 开源项目
- [常见问题排查](./05-troubleshooting.md) - 撤销操作、解决冲突
