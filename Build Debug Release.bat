@echo off

RMDIR distdebug /S /Q

cmake -S . --preset=skyrim --check-stamp-file "build\skyrim\CMakeFiles\generate.stamp"
if %ERRORLEVEL% NEQ 0 exit 1
cmake --build build/skyrim --config Debug
if %ERRORLEVEL% NEQ 0 exit 1

xcopy "build\skyrim\release\*.dll" "distdebug\SKSE\Plugins\" /I /Y
xcopy "build\skyrim\release\*.pdb" "distdebug\SKSE\Plugins\" /I /Y

xcopy "package" "distdebug" /I /Y /E

pause