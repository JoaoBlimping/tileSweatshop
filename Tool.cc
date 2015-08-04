#include "Tool.h"

#include "Tile.h"


Tool::Tool(char * pName,void (*pApplyFunction)(int,int,int,Tile))
{
  name = pName;
  applyFunction = pApplyFunction;
}


void Tool::apply(int x,int y,int colour,Tile tile)
{
  applyFunction(x,y,colour,tile);
}
