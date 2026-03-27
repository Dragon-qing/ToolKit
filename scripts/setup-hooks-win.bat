@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

REM hook 源文件
SET HOOK_SRC=..\scripts\commit-msg

REM hook 目标
SET HOOK_DEST=..\.git\hooks\commit-msg

REM 检查目标文件是否存在，如果存在就删除
IF EXIST "%HOOK_DEST%" (
    DEL "%HOOK_DEST%"
    ECHO 旧的 commit-msg hook 已删除
)

REM 创建符号链接（Git for Windows 支持）
MKLINK "%HOOK_DEST%" "%HOOK_SRC%" >NUL 2>&1

REM 如果 MKLINK 失败（可能没有管理员权限），直接复制文件
IF ERRORLEVEL 1 (
    COPY "%HOOK_SRC%" "%HOOK_DEST%"
    ECHO commit-msg hook 已复制到 hooks 文件夹
) ELSE (
    ECHO commit-msg hook 已链接到 hooks 文件夹
)

ECHO 安装完成
ENDLOCAL
PAUSE
