File Commander
==============

Qt-based cross-platform Total Commander-like dual panel file manager for Windows, Mac and Linux with support for plugins.

![Windows screenshot](/../gh-pages/screenshots/Windows/Clip.jpg?raw=true)


***Building***

* A compiler with C++ 0x/11 support is required (std::thread, lambda functions)
* Mac and Linux: open the project file in Qt Creator and build it.
* Windows: you can build using Qt Creator or Visual Studio for IDE. Visual Studio 2012 or 2013 is required (110 or 120 toolset). I have not tried building it with MinGW, but it should work as long as you enable C++ 11 support.
* Qt 5 only.
