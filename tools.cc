//functions the tools activate


#include "tools.h"


//draws in a single pixel
static void penFunction(int x,int y,int colour,Tile tile)
{
  tile->setPixel(drawX,drawY,drawColour);
}
