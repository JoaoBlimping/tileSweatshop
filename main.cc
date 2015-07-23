#include <SDL2/SDL.h>
#include <stdio.h>

#include "Tile.h"


//screen size constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


//starts up SDL and creates window
//returns true iff it was successful
bool init();

//loads media
//returns true iff it was successful
bool loadMedia();

//Frees media and shuts down SDL
void close();


//the window
SDL_Window * gWindow = NULL;

//the window's surface
SDL_Surface * gScreenSurface = NULL;

//the image we will load to show on the screen
SDL_Surface * gHelloWorld = NULL;


bool init()
{
  //initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("sdl couldn't initialise, SDL_Error: %s\n",SDL_GetError());
    return false;
  }

  //create window
  gWindow = SDL_CreateWindow("Tile Sweatshop",SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
  if (gWindow == NULL)
  {
    printf("window couldn't be created, SDL_Error: %s\n",SDL_GetError());
    return false;
  }

  //get window surface
  gScreenSurface = SDL_GetWindowSurface(gWindow);

  //it worked!
  return true;
}


bool loadMedia()
{
  //load splash image
  gHelloWorld = SDL_LoadBMP("picture.bmp");
  if (gHelloWorld == NULL)
  {
    printf("unable to load image %s, SDL Error: %s\n","picture.bmp",
           SDL_GetError());
    return false;
  }

  //it worked <3 <3
  return true;
}


void close()
{
  //deallocate surface
  SDL_FreeSurface(gHelloWorld);
  gHelloWorld = NULL;

  //destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  //quit SDL subsystems
  SDL_Quit();
}


//this is where the magic happens, baby
int main(int argc,char * args[])
{
  //start up SDL and create window
  if (!init())
  {
    printf("failed to initialise\n");
    close();
    return 0;
  }

  //load media
  if (!loadMedia())
  {
    printf("failed to load media\n");
    close();
    return 0;
  }

  //main loop flag
  bool quit = false;

  //Event handler
  SDL_Event e;

  //while the program is running
  while (!quit)
  {
    //handle events on the queue
    while (SDL_PollEvent(&e) != 0)
    {
      //user requests quit
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
    }
    //apply the image
    SDL_BlitSurface(gHelloWorld,NULL,gScreenSurface,NULL);

    //update the surface
    SDL_UpdateWindowSurface(gWindow);
  }
  //free resources and close SDL
  close();

  return 0;
}
