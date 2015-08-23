#include "PaintingContext.h"

#include <gtk/gtk.h>

#include "Tile.h"


void PaintingContext::setTile(Tile * tile)
{
  currentTile = tile;
}


Tile * PaintingContext::getTile() const
{
  return currentTile;
}
