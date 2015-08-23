#include "Tools.h"

#include "Tile.h"


static bool sameColour(int first,int second)
{
  //test if they are both transparent
  int firstAlpha = (first >> 24) & 1;
  int secondAlpha = (second >> 24) & 1;

  if (firstAlpha == 0 && secondAlpha == 0)
  {
    return true;
  }

  //if they aren't both transparent
  //test if they are both exactly the same
  return (first == second);
}


void Tools::penFunction(int x,int y,int colour,Tile * const tile)
{
  //TODO: make this fill lines when it skips
  tile->setPixel(x,y,colour);
}


void Tools::fillFunction(int x,int y,int colour,Tile * const tile)
{
  int oldColour = tile->getPixel(x,y);

  //if they're the same colour, then lets get out of here
  if (sameColour(colour,oldColour))
  {
    return;
  }

  tile->setPixel(x,y,colour);

  //test if the sides should get it too
  if (sameColour(oldColour,tile->getPixel(x,y - 1)))
  {
    fillFunction(x,y - 1,colour,tile);
  }

  if (sameColour(oldColour,tile->getPixel(x,y + 1)))
  {
    fillFunction(x,y + 1,colour,tile);
  }

  if (sameColour(oldColour,tile->getPixel(x - 1,y)))
  {
    fillFunction(x - 1,y,colour,tile);
  }

  if (sameColour(oldColour,tile->getPixel(x + 1,y)))
  {
    fillFunction(x + 1,y,colour,tile);
  }
}


void Tools::replaceFunction(int x,int y,int colour,Tile * const tile)
{
  int oldColour = tile->getPixel(x,y);

  //if they're the same colour, then lets get out of here
  if (sameColour(colour,oldColour))
  {
    return;
  }

  //replace alll of the old colour with the new colour
  for (int pixelX = 0;pixelX < tile->width;pixelX++)
  {
    for (int pixelY = 0;pixelY < tile->height;pixelY++)
    {
      if (sameColour(oldColour,tile->getPixel(pixelX,pixelY)))
      {
        tile->setPixel(pixelX,pixelY,colour);
      }
    }
  }
}
