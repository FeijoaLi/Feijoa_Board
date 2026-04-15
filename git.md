# 📖 Git 基础使用指南

欢迎查阅本指南！无论你是管理像 `Feijoa_Board` 这样的项目，还是其他的代码库，这里汇总了最常用的 Git 操作，助你轻松掌握版本控制。

## ⚙️ 1. 初始配置 (仅第一次使用需配置)

在开始使用 Git 之前，需要先告诉 Git 你是谁，这样每次提交代码时都会附带你的标识。

```bash
# 设置你的用户名
git config --global user.name "你的名字或昵称"

# 设置你的邮箱
git config --global user.email "你的邮箱@example.com"

# 查看当前配置是否成功
git config --list
```

## 🚀 2. 创建与连接代码库

如果你是在本地新建了一个文件夹并希望上传到云端（例如 GitHub），请按照以下步骤操作：

```bash
# 1. 在当前文件夹初始化 Git 仓库
git init

# 2. 将本地主分支重命名为 main（GitHub 推荐规范）
git branch -M main

# 3. 关联云端的远程仓库
git remote add origin [https://github.com/你的用户名/你的仓库名.git](https://github.com/你的用户名/你的仓库名.git)
```

## 🔄 3. 日常开发工作流 (高频操作)

这是你每天写代码时最常用到的三个命令，被称为“Git 三步曲”：

```bash
# 步骤一：查看当前状态（看看哪些文件被修改了）
git status

# 步骤二：将所有修改过的文件加入暂存区准备提交
git add .

# 步骤三：提交这些修改，并附上一句简短的说明
git commit -m "描述你这次修改了什么内容，比如：修复了界面的 bug"
```

## ☁️ 4. 与云端同步 (Push & Pull)

当你完成了本地的 `commit` 后，就可以把代码推送到 GitHub 上了，或者从上面拉取别人更新的代码。

```bash
# 将本地代码推送到云端 (第一次推送需要加 -u 记住默认分支)
git push -u origin main

# 之后的日常推送只需输入：
git push

# 从云端拉取最新的代码到本地
git pull
```

## 🛡️ 5. 文件忽略清单 (.gitignore)

项目中总有一些文件是不应该上传到云端的（比如编译出来的二进制文件、本地编辑器配置等）。
在项目根目录创建一个名为 `.gitignore` 的文件，Git 会自动忽略里面定义的内容。

**常用的忽略规则示例：**
```text
# 忽略所有的编译产物
*.exe
*.dll
*.class

# 忽略本地开发工具的配置文件夹
.idea/
.vscode/

# 忽略系统自动生成的隐藏文件
.DS_Store
```
> **提示**：如果在写 `.gitignore` 之前文件已经不小心被 `add` 进去了，可以使用 `git rm -r --cached <文件或文件夹名>` 将其从缓存中剔除，然后再重新提交。

## 🔀 6. 分支管理 (进阶)

为了不影响主干代码，开发新功能时通常会新建一个分支，开发完成后再合并回去。

```bash
# 查看当前有哪些分支
git branch

# 创建并切换到一个新分支 (例如叫 dev)
git checkout -b dev

# 将 dev 分支的更改合并回 main 分支 (需先切换回 main)
git checkout main
git merge dev
```