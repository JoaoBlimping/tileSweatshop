//A tile that contains graphics information

class Tile
{
public:
  //the tile's pixel data
  int * * pixels;

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
  Tile(int * * pPixels,int pWidth,int pHeight);

  //destroys the to death
  ~Tile();

  //displays the tile
  render(int x,int y,float scale);
};
