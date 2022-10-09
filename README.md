Bottom Bar
==========
A raw terminal can have the problem that the font doesn't
exactly line up with the screen. This program solves this by
displaying some graphics in the last bottom pixels.

Currently it can display both a battery indicator, and a
game of Tetris, but requires recompilation to change between
them.

Changing terminal font
----------------------
On Linux, run

    setfont <fontname>

Fonts will probably be found in `/usr/share/kbd/consolefonts/`.
