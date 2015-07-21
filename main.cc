#include <SDL2/SDL.h>
#include <stdio.h>


//screen size constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main(int argc,char * args[])
{
  //the window
  SDL_Window * window = NULL;

  //the window's surface
  SDL_Surface * screenSurface = NULL;

  //initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL couldn't init, SDL_Error %s\n",SDL_GetError());
  }
  else
  {
    //create the window
    window = SDL_CreateWindow("Tile Sweatshop",SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,
                              SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      printf("window couldn't be created, SDL_Error: %s\n",SDL_GetError());
    }
    else
    {
      //get the window's surface
      screenSurface = SDL_GetWindowSurface(window);

      //fill the surface with white
      SDL_FillRect(screenSurface,NULL,SDL_MapRGB(screenSurface->format,0xFF,
                                                 0xFF,0xFF));
      //update the surface
      SDL_UpdateWindowSurface(window);

      //wait two seconds
      SDL_Delay(2000);
    }
  }
  //destroy the window
  SDL_DestroyWindow(window);

  //quit sdl subsystems
  SDL_Quit();

  return 0;
}
