#include <stdio.h>

#include "Tile.h"


Tile::Tile(int pWidth,int pHeight):
width(pWidth),
height(pHeight)
{
  this->pixels = new int[pWidth * pHeight];
  for (int i = 0;i < pWidth * pHeight;i++)
  {
    this->pixels[i] = 5;
  }
};


Tile::Tile(int * pPixels,int pWidth,int pHeight):
pixels(pPixels),
width(pWidth),
height(pHeight)
{};


Tile::~Tile()
{
  delete[] pixels;
};


int Tile::getPixel(int x,int y)
{
  return pixels[y * height + x];
}


void Tile::setPixel(int x,int y,int value)
{
  pixels[y * width + x] = value;
}


void Tile::render(int xPos,int yPos,float scale)
{
  for (int x = 0;x < width;x++)
  {
    for (int y = 0;y < height;y++)
    {
      int colourData = pixels[y * width + x];

      int red = colourData & 0xFF;
      int green = (colourData >> 8) & 0xFF;
      int blue = (colourData >> 16) & 0xFF;

      //dunno ayy
    }
  }
};
