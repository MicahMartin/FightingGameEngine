#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#include <stdexcept>

#include "graphics.h"
#include "game.h"
#include "imageSurface.h"

bool G_QUIT = false;

int main(int argc, char *argv[]){


  // Game.init
  Game myGame;

  try{
    myGame.init();
  }catch( const char* message ){
    printf("Error initializing subsystems %s\n", message);
    return -1;
  }

  while ( myGame.stillRunning()) {

    myGame.run();
  }
  //  // write to frame buffer
  //  megaMan->blitImg(coreGraphics->getWindowSurface());
  //  // swap buffers and render
  //  coreGraphics->updateWindowSurface();
  //
  return 0;
}
