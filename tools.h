//contains some tools

#ifndef TOOLS_H
#define TOOLS_H


#include "Tool.h"
#include "Tile.h"


//the tool functions
static void penFunction(int x,int y,int colour,Tile tile);


//the tools
Tool gPen = new Tool("pen",penFunction);

#endif
