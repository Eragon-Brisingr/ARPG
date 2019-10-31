REM git同步
git pull
git submodule sync
git submodule init
git submodule update -f
git submodule foreach "git checkout master"
git submodule foreach "git pull"

REM 刷新VS项目
GenerateProjectFiles.bat ../ARPG.uproject