set ProjectPath="%~dp0/../ARPG.uproject"
set UAT_Path="G:/EpicGames/UE_4.23/Engine/Build/BatchFiles/"
set OutputPath="%~dp0/../Package"

%UAT_Path%RunUAT BuildCookRun -project=%ProjectPath% -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory=%OutputPath%