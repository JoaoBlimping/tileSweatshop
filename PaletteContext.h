//this is the one that contains the palette

#ifndef PALETTE_CONTEXT_H
#define PALETTE_CONTEXT_H


#include "Tile.h"


class PaletteContext
{
public:
  //creates the context's inner tile
  PaletteContext();

  //gives you the current tile to do stuff to
  virtual Tile * getTile();

private:
  Tile * tile;
};


#endif
