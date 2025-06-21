只暂存想push的文件  
`git add file1.txt`  
而不是使用全部提交的命令  
`git add . `

提交只暂存的代码块到本地仓库  
`git commit -m "upload file1.txt"`

将本地仓库的更新内容推送至github  
`git push origin main`

检查状态命令，查看哪些已缓存哪些未缓存  
`git status`

也可以使用.gitignore   
```
git add .gitignore      
git commit -m "添加 .gitignore 文件"    
git push origin main 
```
之后在gitignore文件之中加入忽略的文件即可  

工作区（本地代码文件夹）  
    ↓ `git add .`  
暂存区（Stage）  
    ↓ `git commit`  
本地仓库（Repository）  
    ↓ `git push`  
远程仓库（Remote Repository，如GitHub）

停止 Git 对 inspens.py 的跟踪，但保留本地文件：  
`git rm -r --cached inspens.py ` 
这是因为在.gitignore生效之前就已经生成了该文件的索引，所以需要删除 

或者可以采用清理 Git 缓存并重新添加文件：  
```
git rm -r --cached .  
git add .  
git commit -m "重新应用 .gitignore"  
```

当使用git log命令查看日志  
按Q退出


