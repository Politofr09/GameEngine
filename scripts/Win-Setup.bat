@echo off

pushd ..\
call vendor\bin\premake\windows\premake5.exe vs2022
popd

start ..\GameEngine.sln