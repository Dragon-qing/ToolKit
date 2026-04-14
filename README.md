<h1 align=center> <img align=center width=48 src="img/toolbox.ico" alt="CMake Logo"/> DQToolKits </h1>
<p align=center ><i>Self-use toolbox.</i></p>


## :book: Introduction

这是一个用于收集我日常开发过程中自用工具的仓库，主要面向实际开发中遇到的各种效率问题与重复性工作。

工具箱以 C++ / Qt 开发环境 为核心，涵盖了一些轻量、实用、可复用的小工具与模块，旨在提升开发效率、简化流程，并减少重复劳动。


## :sparkles: Features
- BTF包制作
- 数据文件分析
- xml编辑器

## :wrench: Build Environment

- qt5.15.2
- msvc2019_64
- Cmake >= 3.16 or qmake
- Ninja or jom

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
- [spdlog](https://github.com/gabime/spdlog) - 轻量级日志库  
- [qhotkey](https://github.com/Skycoder42/QHotkey) - Qt桌面应用的全局快捷键
- [EasyQtSql](https://github.com/kramolnic/EasyQtSql) - 轻量级QtSql头文件库
- [fmt](https://github.com/fmtlib/fmt) - 现代化C++开源高性能格式化库

[![fmt](https://img.shields.io/badge/fmt-library-blue)](https://github.com/fmtlib/fmt) [![spdlog](https://img.shields.io/badge/spdlog-lib-green)](https://github.com/gabime/spdlog) [![qhotkey](https://img.shields.io/badge/qhotkey-lib-green)](https://github.com/Skycoder42/QHotkey) [![EasyQtSql](https://img.shields.io/badge/EasyQtSql-header--only-darkgreen)](https://github.com/kramolnic/EasyQtSql)