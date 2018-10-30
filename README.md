# waitkey  ![MIT License](https://img.shields.io/github/license/mashape/apistatus.svg) ![Compatible with GNU/Linux and Windows](https://img.shields.io/badge/compatible-gnu/linux%20%7C%20windows-lightgray.svg)


Single-file implementation of *waitKey* functionality compatible with GNU/Linux and Windows. No need for external dependencies.

The idea is to provide a function that waits for the user to press any key and return its code. The key pressed is not echoed on the screen.

```c
// adds the actual implementation with this inclusion
#define WK_IMPLEMENTATION
#include "waitkey.h"

...

int input = WkWaitKey();
if (input == WKK_UP) {
    printf("You pressed up arrow\n");
}
```

The `WkWaitKey` function can detect every printable ASCII character and some special keys (e.g. arrows, insert, page down).

Other functions:

* `WkGetKeyName`: returns the name of a key (e.g. "WKK_A" for WKK_A).
* `WkGetTerminal`: returns the name of the current terminal (e.g. "xterm").
* `WkSetColor`: set the current foreground and/or background color.
* `WkResetColor`: set the foreground and background colors to its default.
* `WkGetScreenSize`: returns the amount of rows and columns in the screen.

## Supported terminals

Currently this implementation can identify and work with the following terminals:

* xterm
* linux
* windows

By default, *xterm* (GNU/Linux) or *windows* (Windows) is used if no terminal can be identified.

## How to use

Just copy `waitkey.h` to your project and include it wherever needed. You must set the macro `WK_IMPLEMENTATION` before one (and *only* one) of the inclusions to add the actual implementation (because everything is inside the header file).

## Why not use the library *XYZ*?

The main goal here is to provide a minimal *waitkey* functionality without depending on extra libraries and provide support for both GNU/Linux and Windows. If you need something more ~~fancy~~ complete, you probably should use *ncurses* or other stuff.

## License

All code in this repository is available under [MIT License](https://opensource.org/licenses/MIT).