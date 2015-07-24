//A tile that contains graphics information


#ifndef TILE_H
#define TILE_H


class Tile
{
public:
  //the width of the tile
  const int width;

  //the height of the tile
  const int height;

  //creates a tile with no existing data
  //pWidth is the width of the tile
  //pHeight is the height of the tile
  Tile(int pWidth,int pHeight);

  //creates a tile with existing data
  //pPixels is the data
  //pWidth is the width of the tile
  //pHeight is the height of the tile
  Tile(int * pPixels,int pWidth,int pHeight);

  //destroys the to death
  ~Tile();

  //gets the value of a pixel
  //x is the x position
  //y is the y position
  int getPixel(int x,int y);

  //sets the value of a pixel
  //x is the x position
  //y is the y position
  //value is the value to set it to
  void setPixel(int x,int y,int value);

  //displays the tile
  //xPos is the horizontal position
  //yPos is the vertical position
  //scale is the scale to display it at
  //renderer is the SDL rendering thingy
  void render(int xPos,int yPos,float scale,SDL_Renderer * const renderer);

private:
  //the tile's pixel data
  int * pixels;
};

#endif
