@echo off

REM === 设置 VS 路径（按你实际安装改）===
set VS_PATH=C:\Program Files\Microsoft Visual Studio\18\Enterprise

REM === 调用 MSVC 14.29 环境 ===
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64 -vcvars_ver=14.29

REM === 可选：打印确认 ===