#include "graphics/Graphics.h"
#include <cmath>
#include <stdexcept>

void Graphics::init(int w, int h){
  width = w;
  height = h;

  // setup window. return -1 on error
  if( SDL_Init(SDL_INIT_VIDEO) != 0 ){
    throw( std::runtime_error(SDL_GetError()) );
  }

  window = SDL_CreateWindow("Boring Game",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      getWindowWidth(),
      getWindowHeight(),
      SDL_WINDOW_SHOWN);

  if( window == NULL ){
    throw(std::runtime_error(SDL_GetError()));
  }

  // create sdl renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if( renderer == NULL ){
    throw(std::runtime_error(SDL_GetError()));
  }

  // not sure what this does
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Initialize format loading
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  if( !( IMG_Init(imgFlags) & imgFlags)){
    throw(std::runtime_error(IMG_GetError()));
  }

  // We want to show image surfaces inside of the window and in order to do that 
  // we need to get the entire image surface contained by the window
  // windowSurface = SDL_GetWindowSurface(window);
}

void Graphics::update(){ }

void Graphics::clear() {
  // flip buffer
  SDL_RenderClear(renderer);
}

void Graphics::present() {
  // present back buffer
  // wrapping these methods for convenience. not sure how useful these 'hooks' will be in the future
  SDL_RenderPresent(renderer);
}

void Graphics::resizeWindow(int w, int h){
  // resize the window
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

