@echo off

pushd ..
Vendor\Premake5\premake5.exe --file=Build.lua vs2022
popd
pause