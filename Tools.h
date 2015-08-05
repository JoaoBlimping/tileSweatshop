//contains some tools

#ifndef TOOLS_H
#define TOOLS_H


#include "Tool.h"
#include "Tile.h"


namespace Tools
{
  //the tool functions
  void penFunction(int x,int y,int colour,Tile * const tile);
  void fillFunction(int x,int y,int colour,Tile * const tile);
  void replaceFunction(int x,int y,int colour,Tile * const tile);

  //the tools
  Tool const * const pen = new Tool("pen",penFunction);
  Tool const * const fill = new Tool("fill",fillFunction);
  Tool const * const replace = new Tool("replace",replaceFunction);
}

#endif
