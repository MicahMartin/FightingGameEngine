#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
int main(int argc, char *argv[]){

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* displayWindow;
  SDL_Renderer* displayRenderer;
  SDL_RendererInfo displayRendererInfo;
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
  SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);

}
