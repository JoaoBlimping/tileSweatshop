#include "PaintingContext.h"

#include "Tile.h"


PaintingContext::PaintingContext()
{
  currentTile = NULL;
}


void PaintingContext::setTile(Tile * tile)
{
  currentTile = tile;
}


Tile * PaintingContext::getTile()
{
  return currentTile;
}
