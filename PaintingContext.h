//this is the one where you do the real painting

#ifndef PAINTING_CONTEXT_H
#define PAINTING_CONTEXT_H


#include "Tile.h"


class PaintingContext
{
public:
  //set the tile that it is working with
  void setTile(Tile * tile);

  //gives you the current tile to do stuff to
  virtual Tile * getTile();

private:
  Tile * currentTile;
};


#endif
