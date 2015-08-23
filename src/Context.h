//the context in which things are grooving


#ifndef CONTEXT_H
#define CONTEXT_H


#include "Tile.h"


class Context
{
public:
  //the scale the tile is drawn at
  float scale = 1;

  //the x position the tile is drawn at
  float tileX = 0;

  //the y position the tile is drawn at
  float tileY = 0;

  //the x position dragging started at
  float dragOriginX = 0;

  //the y position dragging started at
  float dragOriginY = 0;

  //gives you the current tile to do stuff to
  virtual Tile * getTile() const = 0;
};


#endif
