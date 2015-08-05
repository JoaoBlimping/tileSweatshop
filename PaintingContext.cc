#include "PaintingContext.h"

#include <gtk/gtk.h>

#include "Tile.h"


PaintingContext::PaintingContext(Tile * tile)
{
  currentTile = tile;
}


void PaintingContext::setTile(Tile * tile)
{
  currentTile = tile;
}


Tile * PaintingContext::getTile() const
{
  return currentTile;
}
