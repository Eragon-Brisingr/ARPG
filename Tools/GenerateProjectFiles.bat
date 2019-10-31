 setlocal
 del /q gen_temp.txt
 powershell -command "& { (Get-ItemProperty 'Registry::HKEY_CLASSES_ROOT\Unreal.ProjectFile\shell\rungenproj' -Name 'Icon' ).'Icon' }" > gen_temp.txt
 set /p gen_bin=<gen_temp.txt
 %gen_bin% /projectfiles %cd%\%1%
 del /q gen_temp.txt