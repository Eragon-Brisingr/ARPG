REM ## git sync
git pull
git submodule sync
git submodule init
git submodule update -f
git submodule foreach "git checkout master"
git submodule foreach "git pull"

REM ## generate visual studio project
GenerateProjectFiles.bat ../ARPG.uproject