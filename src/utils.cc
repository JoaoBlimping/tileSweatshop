#include "utils.h"


int makeColour(double red,double green,double blue,bool alpha)
{
  int red8Bit = (int)(255 * red);
  int green8Bit = (int)(255 * green);
  int blue8Bit = (int)(255 * blue);
  int alpha1Bit = alpha ? 0 : 1;

  return (alpha1Bit << 24) | (red8Bit << 16) | (green8Bit << 8) | blue8Bit;
}
