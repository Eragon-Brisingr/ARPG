REM ## git sync
git pull
git submodule foreach "git checkout master"
git submodule foreach "git pull"

REM ## generate visual studio project
GenerateProjectFiles.bat ../ARPG.uproject