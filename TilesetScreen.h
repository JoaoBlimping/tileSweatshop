//a screen where you edit tilesets


#ifndef TILESET_SCREEN_H
#define TILESET_SCREEN_H


#include <vector>

#include <SDL2/SDL.h>

#include "Screen.h"
#include "Tile.h"


using namespace std;


class TilesetScreen:public Screen
{
public:
  //creates a TilesetScreen
  //tileWidth is the width of tiles
  TilesetScreen(int pTileWidth,int pTileHeight);

  //deletes the TilesetScreen
  ~TilesetScreen();

  //updates the TilesetScreen
  virtual void update();

  //renders the TilesetScreen
  virtual void render(SDL_Renderer * renderer);

private:
  int const tileWidth;
  int const tileHeight;
  vector<Tile *> tiles;
};

#endif
