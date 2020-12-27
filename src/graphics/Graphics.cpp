#include "graphics/Graphics.h"
#include <SDL_ttf.h>
#include <cmath>
#include <stdexcept>

void Graphics::init(){
  // setup window. return -1 on error
  window = SDL_CreateWindow(windowName.c_str(),
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      1280,
      720,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if( window == NULL ){
    throw(std::runtime_error(SDL_GetError()));
  }
  SDL_GetWindowSize(window, &width, &height);
  printf("the window x:%d, y:%d\n", width, height);


  // create sdl renderer
  //
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, width, height);
  // SDL_SetWindowSize(window, 2560, 1440);
 
  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  if( renderer == NULL ){
    throw(std::runtime_error(SDL_GetError()));
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  // SDL_RenderSetLogicalSize(renderer, 1920, 1080);

  // Initialize format loading
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  if( !( IMG_Init(imgFlags) & imgFlags)){
    throw(std::runtime_error(IMG_GetError()));
  }

  // We want to show image surfaces inside of the window and in order to do that 
  // we need to get the entire image surface contained by the window
  // windowSurface = SDL_GetWindowSurface(window);
  if(TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
  }
  scpFont = TTF_OpenFont("../data/images/font/SourceCodePro-Black.ttf", 32); //this opens a font style and sets a size
  if (scpFont == NULL) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    exit(1);
  }
}

void Graphics::update(){ }

void Graphics::clear() {
  // flip buffer
  SDL_RenderClear(renderer);
}

void Graphics::present() {
  // present back buffer
  // wrapping these methods for convenience. not sure how useful these 'hooks' will be in the future
  double drawStart = SDL_GetTicks();
  // printf("how long does it take to resolve this pointer %f\n", drawStart);
  SDL_RenderPresent(renderer);
}

void Graphics::resizeWindow(int w, int h){
  SDL_SetWindowSize(window, w, h);
}

void Graphics::resizeWindow(bool fullScreen){
  // resize the window
  if (fullScreen) {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  }
}


SDL_Renderer* Graphics::getRenderer() {
  // return renderer pointer
  return renderer;
}

SDL_Window* Graphics::getWindow() {
  // return window pointer 
  return window;
}


int Graphics::getWindowWidth(){
  return width;
}

int Graphics::getWindowHeight(){
  return height;
}
void Graphics::setCamera(Camera* cam){
  camera = cam;
}

Camera* Graphics::getCamera(){
  return camera;
}


TTF_Font* Graphics::getFont(){
  return scpFont;
}

void Graphics::clearFlag(GraphicsFlag flag){

  flags &= (~flag);
}

void Graphics::setFlag(GraphicsFlag flag){

  flags |= flag;
}

bool Graphics::getFlag(GraphicsFlag flag){

  return (( flags & flag ) == flag);
}

void Graphics::setFPS(int _fps){
fps = _fps;
}
int Graphics::getFPS(){
  return fps;
}
