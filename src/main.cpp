#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "graphics.h"


int main(int argc, char *argv[]){

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("we broke\n");
    return -1;
  };

  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_RendererInfo rendererInfo;

  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer);
  SDL_GetRendererInfo(renderer, &rendererInfo);

  if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0){
    /*TODO: Handle this. We have no render surface and not accelerated. */
    printf("no render surface\n");
    return -1;
  }

  Graphics *graphics = new Graphics();

  graphics->init(800,600);
  graphics->render();
  SDL_RenderPresent(renderer);

  SDL_Delay(5000);
  SDL_Quit();

  return 0;
}
