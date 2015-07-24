#include <stdio.h>

#include <SDL2/SDL.h>

#include "Screen.h"
#include "TilesetScreen.h"
#include "Tile.h"


//screen size constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//name of the program constant
char const * const TITLE = "Tile Sweatshop";


//starts up SDL and creates window
//returns true iff it was successful
bool init();

//Frees media and shuts down SDL
void close();


//the window
SDL_Window * gWindow = NULL;

//the window's surface
SDL_Surface * gScreenSurface = NULL;

//The window renderer
SDL_Renderer * gRenderer = NULL;

//the current screen
Screen * currentScreen = NULL;


bool init()
{
  //initialise SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("sdl couldn't initialise, SDL_Error: %s\n",SDL_GetError());
    return false;
  }

  //create window
  gWindow = SDL_CreateWindow(TITLE,SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
  if (gWindow == NULL)
  {
    printf("window couldn't be created, SDL_Error: %s\n",SDL_GetError());
    return false;
  }

  //get window surface
  gScreenSurface = SDL_GetWindowSurface(gWindow);

  gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
  if( gRenderer == NULL )
  {
    printf( "Renderer could not be created! SDL Error: %s\n",SDL_GetError());
    return false;
  }

  //Initialize renderer color
  SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);

  //create the first screen
  currentScreen = new TilesetScreen(32,32);

  //it worked!
  return true;
}


void close()
{
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

    //update the screen
    currentScreen->update();

    //render the screen
    currentScreen->render(gRenderer);

    //Update screen
    SDL_RenderPresent(gRenderer);
  }
  //free resources and close SDL
  close();

  return 0;
}
