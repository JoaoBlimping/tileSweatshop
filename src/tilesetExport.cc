//converts a tileset file to a png file

#include <iostream>
#include <fstream>
#include <vector>

#include "lodepng.h"


void writeToFile(char const * filename,std::vector<unsigned char> & image,
                 int width,int height)
{
  //Encode the image
  unsigned error = lodepng::encode(filename,image,width,height);

  //if there's an error, display it
  if(error)
  {
    std::cout << "encoder error " << error << ": " <<
                 lodepng_error_text(error) << std::endl;
  }
}


int main(int argc, char *argv[])
{
  //check the number of arguments was correct
  if (argc != 3)
  {
    std::cout << "usage is tilesetExport inFile outFile " <<
                 std::endl;
    return 0;
  }

  //turn all the arguments into things
  char * inFile = argv[1];
  char * outFile = argv[2];

  //open the input file
  std::ifstream inputStream(inFile);

  //test that the file was opened correctly
  if (!inputStream.is_open())
  {
    std::cout << ("could not open %s\n",inFile) << std::endl;
    return 1;
  }

  //read in tileset properties
  int tileWidth;
  int tileHeight;
  int nTiles;
  inputStream >> tileWidth;
  inputStream >> tileHeight;
  inputStream >> nTiles;

  //create the empty vector
  std::vector<unsigned char> image;
  image.resize((tileWidth * nTiles * 4) * tileHeight);

  //fill it with the image data
  for (int i = 0;i < nTiles;i++)
  {
    for (int y = 0;y < tileHeight;y++)
    {
      for (int x = 0;x < tileWidth;x++)
      {
        //read the full colour
        int colour;
        inputStream >> colour;

        //break it down
        int offset = i * (tileWidth * 4) +
                     y * tileWidth * nTiles * 4 +
                     x * 4;

        image[offset] = (colour >> 16) & 0xFF;
        image[offset + 1] = (colour >> 8) & 0xFF;
        image[offset + 2] = colour & 0xFF;
        image[offset + 3] = ((colour >> 24) & 1) * 0xFF;
      }
    }
  }

  //close the input file
  inputStream.close();

  //write the image to the output file
  writeToFile(outFile,image,tileWidth * nTiles,tileHeight);
}
