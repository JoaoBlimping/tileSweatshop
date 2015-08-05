#include "PaintingContext.h"

#include "Tile.h"

void PaintingContext::setTile(Tile * tile)
{
  currentTile = tile;
}

Tile * PaintingContext::getTile()
{
  return currentTile;
}
