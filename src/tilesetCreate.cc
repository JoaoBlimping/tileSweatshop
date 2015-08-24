//this creates a nice little tileset for you funny fellows

#include <iostream>
#include <fstream>
#include <cstdlib>


int main(int argc,char * argv[])
{
  //make sure number of arguments is correct
  if (argc != 4)
  {
    std::cout << "usage is tilesetCreate tileWidth tileHeight filename " <<
                 std::endl;
    return 1;
  }

  //turn all the arguments into things
  int tileWidth = std::stoi(argv[1]);
  int tileHeight = std::stoi(argv[2]);
  char * filename = argv[3];

  //open the file
  std::ofstream tilesetStream(filename);

  //check it worked
  if (!tilesetStream.is_open())
  {
    std::cout << "couldn't write to " << filename << std::endl;
    return 1;
  }

  //write tile dimensions
  tilesetStream << tileWidth << std::endl;
  tilesetStream << tileHeight << std::endl;

  //write that there is currently one tile
  tilesetStream << 1 << std::endl;

  //write in emptiness for it
  for (int x = 0;x < tileWidth;x++)
  {
    for (int y = 0;y < tileHeight;y++)
    {
      tilesetStream << 0;
    }
  }

  //now close the stream
  tilesetStream.close();

  return 0;
}
