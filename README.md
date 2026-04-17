<h1 align=center> <img align=center width=48 src="img/toolbox.ico" alt="CMake Logo"/> DQToolKits </h1>
<p align=center ><i>Self-use toolbox.</i></p>

<p align="center">
   <a href="https://github.com/Dragon-qing/ToolKit/actions"><img src="https://github.com/Dragon-qing/ToolKit/actions/workflows/build-windows.yml/badge.svg" alt="Github Actions"></a>
   <a href="https://www.qt.io/"><img src="https://img.shields.io/badge/Made%20with-Qt-1f425f.svg" alt="made-with-Qt"></a>
   <a href="https://github.com/Dragon-qing/ToolKit"><img src="https://img.shields.io/badge/Qt-5.15.2-green.svg" alt="Qt version"></a>
   <a href="https://github.com/Dragon-qing/ToolKit/blob/main/LICENSE"><img src="https://img.shields.io/badge/license-MIT-green" alt="license"></a>
   <a href="https://github.com/Dragon-qing/ToolKit/releases/"><img src="https://img.shields.io/github/release/Dragon-qing/ToolKit.svg" alt="Latest release"></a>
</p>

## :book: Introduction

这是一个用于收集我日常开发过程中自用工具的仓库，主要面向实际开发中遇到的各种效率问题与重复性工作。

工具箱以 C++ / Qt 开发环境 为核心，涵盖了一些轻量、实用、可复用的小工具与模块，旨在提升开发效率、简化流程，并减少重复劳动。


## :sparkles: Features
- 压缩包制作
- 数据文件分析
- xml编辑器

## :wrench: Build Environment

> - qt5.15.2
> - msvc2019_64
> - Cmake >= 3.16 or qmake
> - Ninja or jom

## :triangular_ruler: Structure

```mermaid
graph TD
  servicetools([服务类三方工具])
  datatools([数据类三方工具])
  widget([界面UI])
  Service([后台服务])
  Data([数   据   层])
  Database[(数据库)]
  logdt([日志])
  settings(设置)
  localfile@{ shape: docs, label: "数据文件" }

  Data--界面数据-->widget
  Data--服务数据-->Service
  Data--日志数据-->logdt
  Data--配置数据-->settings
  Database --> Data
  localfile --> Data
  servicetools --> widget
  servicetools --> Service
  datatools --> Data

  style Data font-size:32px
```
## :link: Acknowledgements
- <a href = "https://github.com/gabime/spdlog"><img src="https://img.shields.io/badge/spdlog-lib-green" style="vertical-align: middle; margin-right: 4px;"></a> : 轻量级日志库

- <a href="https://github.com/Skycoder42/QHotkey"><img src="https://img.shields.io/badge/qhotkey-lib-green" style="vertical-align: middle; margin-right: 4px;"></a> : Qt桌面应用的全局快捷键</p>

- <a href="https://github.com/kramolnic/EasyQtSql"><img src="https://img.shields.io/badge/EasyQtSql-header--only-darkgreen" style="vertical-align: middle; margin-right: 4px;"></a> : 轻量级QtSql头文件库

- <a href = "https://github.com/fmtlib/fmt"><img src="https://img.shields.io/badge/fmt-lib-green" style="vertical-align: middle; margin-right: 4px;"></a> :  现代化C++开源高性能格式化库
   