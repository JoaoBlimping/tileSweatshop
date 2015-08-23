//this is the one where you do the real painting

#ifndef PAINTING_CONTEXT_H
#define PAINTING_CONTEXT_H


#include "Context.h"
#include "Tile.h"


class PaintingContext:public Context
{
public:
  //set the tile that it is working with
  void setTile(Tile * tile);

  //gives you the current tile to do stuff to
  virtual Tile * getTile() const;

private:
  Tile * currentTile;
};


#endif
