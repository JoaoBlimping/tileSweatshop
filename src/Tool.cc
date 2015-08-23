#include "Tool.h"

#include "Tile.h"


Tool::Tool(char const * pName,void (*pApplyFunction)(int,int,int,Tile * const))
{
  name = pName;
  applyFunction = pApplyFunction;
}


void Tool::apply(int x,int y,int colour,Tile * const tile) const
{
  applyFunction(x,y,colour,tile);
}
