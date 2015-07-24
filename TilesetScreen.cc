#include "TilesetScreen.h"

#include "Tile.h"


TilesetScreen::TilesetScreen(int pTileWidth,int pTileHeight):
tileWidth(pTileWidth),
tileHeight(pTileHeight)
{
  tiles.push_back(new Tile(pTileWidth,pTileHeight));
};


TilesetScreen::~TilesetScreen()
{
  //doesn't need to do anything at the moment
};


void TilesetScreen::update()
{
  //TODO: stuff
};


void TilesetScreen::render(SDL_Renderer * renderer)
{
  //Clear screen
  SDL_SetRenderDrawColor(renderer,0xFF,0x00,0x80,0xFF);
  SDL_RenderClear(renderer);
};
