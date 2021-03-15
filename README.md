# Description
My fork of the osu! Bongo Cat overlay by [kuroni](https://github.com/kuroni).

I added a new mode (6). It is similar to the osu mode (1) but uses a custom png instead of the paw and will maybe let you add more keyboard interactions or something in the future. At the moment the hand should be called `hand.png` with a resolution of 115x350. The Rest of the images are the same as in osu mode. All of the images should be in the `img/osuh` directory. I'll add more useful instructions soon.

Because I'm lazy I will just leave the original description for now. However, I'm not sure if the build instructions will work right now since I had to use a dirty workaround for a linker issue I had with the mingw crosscompiler on Linux. If the build fails try removing the `include/wa` directory and removing the line `#include <wa/workaround.c>` from `main.cpp`.

# Original Description
An osu! Bongo Cat overlay with smooth paw movement and simple skinning ability, written in C++.

You can find how to configure the application in our [wiki](https://github.com/kuroni/bongocat-osu/wiki/Settings).

Download the program [here](https://github.com/kuroni/bongocat-osu/releases).

Hugs and kisses to [CSaratakij](https://github.com/CSaratakij) for creating the Linux port for this project!

Any suggestion and/or collaboration, especially that relating to sprites, is welcomed! Thank you!

[Original post](https://www.reddit.com/r/osugame/comments/9hrkte/i_know_bongo_cat_is_getting_old_but_heres_a_nicer/) by [Kuvster](https://github.com/Kuvster).

## Further information
In order to play with fullscreen on Windows 10, run both osu! and this application in Windows 7 compability mode.

Press Ctrl + R to reload configuration and images (will only reload configurations when the window is focused).

Supported operating system:
* Windows
* Linux (tested with Arch Linux with WINE Staging 5). Note: You **must** use WINE Staging, because for whatever reason on stable WINE bongocat-osu doesn't register keyboard input from other windows.

_Notice_: If you're using WINE on Linux, make sure that osu! and this application run in the same `WINEPREFIX`.

## For developers
This project uses [SFML](https://www.sfml-dev.org/index.php) and [JsonCpp](https://github.com/open-source-parsers/jsoncpp). JsonCpp libraries are directly included in the source using the provided `amalgamation.py` from the developers.

### Libraries and dependency

#### Windows and MinGW
To build the source, download the SFML libraries [here](https://www.sfml-dev.org/index.php), copy `Makefile.windows` to `Makefile`, then replace *`<SFML-folder>`* in `Makefile` with the desired folder.

#### Linux
You need to have these dependencies installed. Check with your package manager for the exact name of these dependencies on your distro:
- g++
- libxdo
- sdl2
- sfml
- x11
- xrandr

Then, copy `Makefile.linux` to `Makefile`.

### Building and testing
To build, run this command from the base directory:

```sh
make
```

To test the program, run this from the base directory:

```sh
make test
```

Alternatively, you can copy the newly-compiled `bin/bongo.exe` or `bin/bongo` into the base directory and execute it.

If you have troubles compiling, it can be due to version mismatch between your compiler and SFML. See [#43](https://github.com/kuroni/bongocat-osu/issues/43) for more information.

