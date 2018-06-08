#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "graphics.h"


int main(int argc, char *argv[]){

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("we broke\n");
    return -1;
  };

  SDL_Window *window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
  void *glContext = SDL_GL_CreateContext(window);


  Graphics *graphics = new Graphics();
  graphics->init(800, 600);

  graphics->renderBackGround(1.0, 0.0, 0.0, 0.0);

  // this swaps the back(buffer thats being drawn to) and front(buffer thats being shown) buffer
  SDL_GL_SwapWindow(window);
  SDL_Delay(5000);


  graphics->renderBackGround(1.0, 0.0, 0.5, 1.0);
  SDL_GL_SwapWindow(window);
  SDL_Delay(5000);

  SDL_GL_DeleteContext(glContext);
  SDL_Quit();

  return 0;
}
