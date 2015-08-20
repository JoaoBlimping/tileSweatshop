#include <stdio.h>
#include <iostream>

#include "Tile.h"

#include <gtk/gtk.h>


Tile::Tile(int pWidth,int pHeight):
width(pWidth),
height(pHeight)
{
  this->pixels = new int[pWidth * pHeight];
  for (int i = 0;i < pWidth * pHeight;i++)
  {
    this->pixels[i] = 0;
  }
};


Tile::Tile(int * pPixels,int pWidth,int pHeight):
pixels(pPixels),
width(pWidth),
height(pHeight)
{};

Tile::Tile(std::istream * pixelStream,int pWidth,int pHeight):
width(pWidth),
height(pHeight)
{
  this->pixels = new int[pWidth * pHeight];
  for (int i = 0;i < pWidth * pHeight;i++)
  {
    (*pixelStream) >> this->pixels[i];
  }
};


Tile::~Tile()
{
  delete[] pixels;
};


Tile * Tile::duplicate()
{
  Tile * child = new Tile(width,height);

  for (int i = 0;i < width * height;i++)
  {
    child->pixels[i] = pixels[i];
  }

  return child;
}


int Tile::getPixel(int x,int y)
{
  if (x < 0 || x >= width || y < 0 || y >= height)
  {
    return -1;
  }

  return pixels[y * height + x];
}


void Tile::setPixel(int x,int y,int value)
{
  if (x < 0 || x >= width || y < 0 || y >= height)
  {
    return;
  }

  pixels[y * width + x] = value;
}


void Tile::render(int xPos,int yPos,float scale,cairo_t * cr)
{
  for (int x = 0;x < width;x++)
  {
    for (int y = 0;y < height;y++)
    {
      int colourData = pixels[y * width + x];

      bool alpha = (colourData >> 24) & 1;

      //skip the rest if this pixel is meant to be unseen
      if (!alpha)
      {
        continue;
      }

      //get the rest of the colour data
      float red = (float)((colourData >> 16) & 0xFF) / 0xFF;
      float green = (float)((colourData >> 8) & 0xFF) / 0xFF;
      float blue = (float)(colourData & 0xFF) / 0xFF;

      cairo_set_source_rgb (cr,red,green,blue);
      cairo_rectangle(cr,xPos + x * scale,yPos + y * scale,scale,scale);
      cairo_fill (cr);
    }
  }
};
