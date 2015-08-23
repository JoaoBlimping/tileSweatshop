//this is the one that contains the palette

#ifndef PALETTE_CONTEXT_H
#define PALETTE_CONTEXT_H


#include "Context.h"
#include "Tile.h"


class PaletteContext:public Context
{
public:
  //gives you the current tile to do stuff to
  virtual Tile * getTile() const;

private:
  Tile * tile = new Tile(32,32);
};


#endif
