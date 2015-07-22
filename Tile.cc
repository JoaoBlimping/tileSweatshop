#include <SDL2/SDL.h>

#include "Tile.h"


Tile::Tile(int pWidth,int pHeight):
width(pWidth),
height(pHeight)
{
  this->pixels = new int * [pWidth];

  for (int x = 0;x < pWidth;x++)
  {
    this->pixels[x] = new int[pHeight];
    for (int y = 0;y < pHeight;y++)
    {
      this->pixels[x][y] = 0;
    }
  }
};


Tile::Tile(int * * pPixels,int pWidth,int pHeight):
pixels(pPixels),
width(pWidth),
height(pHeight)
{};


Tile::~Tile()
{
  for (int x = 0;x < this->width;x++)
  {
    delete[] pixels[x];
  }
  delete[] pixels;
};
