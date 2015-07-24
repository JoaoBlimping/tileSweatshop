//an interface for screens where stuff happens


#ifndef SCREEN_H
#define SCREEN_H


#include <SDL2/SDL.h>


class Screen
{
public:
  //updates the screen
  virtual void update() = 0;

  //renders the screen
  virtual void render(SDL_Renderer * renderer) = 0;
};

#endif
