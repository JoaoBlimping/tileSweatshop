# Tile Sweatshop
A set of commandline based applications which will be able to make tile based graphics
and maps for games.

##Installation and Dependencies
tileSweatshop requires the following to work:
 - `pkg-config` for building gtk
 - `gtk+-3.0` for the ui
 - `g++` for compilation
 - `make` for building

## How it will be used
This is subject to change, but my initial plan for the commandline interface is
as follows:

`tilesetCreate tileWidth tileHeight filename` Creates a new tileset
file called 'filename' with tiles of 'tileWidth' width and 'tileHeight' height.

`tileSweatshop filename` opens the graphical editor for the tileset
called 'filename'.

`tilesetExport in out` converts the file 'in' to the picture
file 'out'.

`mapCreate name` creates a new map file called 'name'.

`mapSweatshop name` opens the graphical editor for the map called
'name'.

yeah actually maybe i'll just make these as four different executables since
only two of them actually even have a gui and have substantially different ones.

There is no exportMap command, since this will be different for each game and
users will have to write one of these themselves.
