#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "graphics.h"


int main(int argc, char *argv[]){

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("we broke\n");
    return -1;
  };

  SDL_Window *window = SDL_CreateWindow("Hello World", 
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      800,
                                      600,
                                      SDL_WINDOW_OPENGL);

  SDL_GLContext renderContext = SDL_GL_CreateContext(window);

  Graphics *graphics = new Graphics();

  graphics->init(800,600);
  graphics->render();

  SDL_GL_SwapWindow(window);

  SDL_Delay(5000);
  SDL_Quit();

  return 0;
}
