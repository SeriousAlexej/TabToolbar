<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/TabToolbar/master/screenshots/logo/logotype-a.png">
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/TabToolbar/master/screenshots/screen.png">
</p>

-------------

[![Build status](https://ci.appveyor.com/api/projects/status/03acxnds340jgnm1?svg=true)](https://ci.appveyor.com/project/SeriousAlexej/tabtoolbar)
[![Build Status](https://travis-ci.org/SeriousAlexej/TabToolbar.svg?branch=master)](https://travis-ci.org/SeriousAlexej/TabToolbar)

As stated above, **TabToolbar** is a small library, meant to be used with **Qt**,
that provides a few classes for creating of tabbed toolbars.

Features:
  - **Cross-platform** - TabToolbar depends only on **Qt** and requires a **C++11** compliant compiler to be built.
  - **Native look and feel** - TabToolbar has a few predefined styles, which use current OS's palette to generate the best looking style possible. Moreover styles can be created and changed dynamically at runtime!
  - **Configurable** - UI configuration can be set directly from code, or loaded from JSON file! This enables easy UI fine-tuning and customization.
  - **Customizable** - UI can contain any custom widget! If you use JSON configs, all you have to do is to tell TabToolbar, how to create them, and you are good to go!
  - **Non-exclusive** - TabToolbar can be easily combined with classic menu bars! Consider a CAD application, that uses TabToolbar and places it's most used actions in it, while combining it with menu bar, which contains other less-frequently used actions.

Styles:
-------------

TabToolbar comes with four predefined styles:
  - **Vienna** (default for Windows 7 OS)
  - **Threshold** (default for all Windows >= 8 OSes)
  - **Kool** (default for Unix-like systems)
  - **White Mercy** a simple white reskin of Threshold style

All these styles use current OS palette and thus look great with any theme (light, dark, you name it).
Each style can be used on any OS.

Some examples:

<p align="center">
Vienna style on Windows 7:
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/TabToolbar/master/screenshots/win7.png">
</p>
<p align="center">
Kool style on Manjaro KDE with Breeze Dark theme:
  <img src="https://raw.githubusercontent.com/SeriousAlexej/TabToolbar/master/screenshots/kde.png">
</p>
<p align="center">
Threshold style on Manjaro LXQt with gtk2 theme:
  <img src="https://raw.githubusercontent.com/SeriousAlexej/TabToolbar/master/screenshots/lxqt.png">
</p>
<p align="center">
White Mercy style on Windows 10:
</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/SeriousAlexej/TabToolbar/master/screenshots/win10mercy.png">
</p>

Building
-------------

TabToolbar uses **CMake** and depends on **Qt >= 5.3**.
A simple example project of how to use TabToolbar is included, but is by default not included in build.

Usage
-------------

To link TabToolbar to your project, you can use provided `FindTabToolbar` cmake module, which defines `TabToolbar_LIBRARY` variable
that you can feed to your `target_link_libraries` command.

An example project is also included in **examples/Test** subdirectory, please refer to it if you have any troubles.
	
License
-------------

TabToolbar uses **LGPL** license either version **3** or any later version.

TabToolbar logo generously provided by ![Tobaloidee](https://github.com/Tobaloidee)
