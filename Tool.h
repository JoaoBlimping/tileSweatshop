//this is like a thing that you can draw with and stuff

#ifndef TOOL_H
#define TOOL_H


#include "Tile.h"


class Tool
{
public:
  //creates a tool with all it's stuff
  //pName is the tool's name
  //pApplyFunction is the function the tool applies
  Tool(char * pName,void (*pApplyFunction)(int,int,int,Tile));

  //applies the tool to the tile
  //x is the horizontal position to apply to
  //y is the vertical position to apply to
  //colour is the colour, alpha included
  //tile is the tile to apply it to
  void apply(int x,int y,int colour,Tile tile)const;

  //the tool's name
  char const * name;

private:
  void (*applyFunction)(int,int,int,Tile);
};





#endif
