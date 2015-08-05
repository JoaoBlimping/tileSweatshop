#include "PaletteContext.h"

#include "Tile.h"

PaletteContext::PaletteContext()
{
  tile = new Tile(32,32);
}

Tile * PaletteContext::getTile()
{
  return tile;
}
