# Tile Sweatshop
A commandline based application which will be able to make tile based graphics
and maps for games.

## How it will be used
This is subject to change, but my initial plan for the commandline interface is
as follows:

```tileSweatshop newTileset name``` Creates a new tileset file called 'name'.

```tileSweatshop editTileset name``` opens the graphical editor for the tileset
called 'name'.

```tileSweatshop exportTileset in out``` converts the file 'in' to the picture
file 'out'.

```tileSweatshop newMap name``` creates a new map file called 'name'.

```tileSweatshop editMap name``` opens the graphical editor for the map called
'name'.

There is no exportMap command, since this will be different for each game and
users will have to write one of these themselves.
