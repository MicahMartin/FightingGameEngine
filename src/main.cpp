#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdexcept>

#include "graphics.h"
#include "imageSurface.h"


int main(int argc, char *argv[]){

  bool quit = false;

  printf("foobar\n");
  Graphics* coreGraphics = new Graphics();

  try{

    coreGraphics->init(640,480);
  }catch( const char* message ){

    printf("Here is the error from graphics init %s\n", message);
    return -1;
  }

  // render white to screen
  coreGraphics->clearWindow();
  coreGraphics->updateWindowSurface();
  SDL_Delay(2000);

  ImageSurface* megaMan;
  try{

    megaMan = new ImageSurface();
    megaMan->loadImg("../data/megaMan.bmp");
  }catch( const char* message ){

    megaMan = NULL;
    printf("Here is the error from tryna load megaman %s\n", message);
    return -1;
  }


  SDL_Event event;
  while ( !quit ) {

    while ( SDL_PollEvent(&event) != 0 ) {

      if ( event.type ==  SDL_QUIT ) {
        quit = true;
      }
    }

    // write to frame buffer
    megaMan->blitImg(coreGraphics->getWindowSurface());
    // swap buffers and render
    coreGraphics->updateWindowSurface();
  }

  delete coreGraphics;
  return 0;
}
