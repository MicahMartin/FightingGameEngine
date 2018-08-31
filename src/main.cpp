#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#include <stdexcept>

#include "graphics/Graphics.h"
#include "graphics/ImageSurface.h"
#include "Game.h"


int main(int argc, char *argv[]){


  // Game.init
  Game myGame;

  try{
    myGame.init();
  }catch( const char* message ){
    printf("Error initializing subsystems %s\n", message);
    return -1;
  }
  double FPS = 1000/60;

  while ( myGame.stillRunning()) {

    double frameStart = SDL_GetTicks();

    myGame.update();

    double frameEnd = SDL_GetTicks();
    // we want to run the update stuff once every frame (16~ MS)
    // so we check how many MS its taken us to generate the current frame 
    // so if it took 3MS to render this frame, we have 13MS of free time before reading input and rendering again
    // def not percise but meh frn
    double delayLength = FPS - (frameEnd - frameStart);
    //printf("ticks this frame: %d\n", int(frameEnd - frameStart));

    if(int(delayLength) > 0){
      //printf("The delay length: %d\n", int(delayLength));
      SDL_Delay(int(delayLength));
    }
  }

  return 0;
}
