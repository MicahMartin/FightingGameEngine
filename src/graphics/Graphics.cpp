#include "graphics/Graphics.h"
#include <cmath>
#include <stdexcept>

Graphics::Graphics(int w, int h) {
  width = w;
  height = h;

  // Init SDL Video system
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    throw( std::runtime_error(SDL_GetError()) );
  }

  // Create the window
  window = SDL_CreateWindow("Boring Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

  // Window shouldn't be null
  if(window == NULL){
    throw(std::runtime_error(SDL_GetError()));
  }

  // Create sdl renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Renderer shouldn't be null
  if(renderer == NULL){
    throw(std::runtime_error(SDL_GetError()));
  }

  // Not sure what this does
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Initialize format loading
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  if(!(IMG_Init(imgFlags) & imgFlags)){
    throw(std::runtime_error(IMG_GetError()));
  }
}

void Graphics::clear() {
  // flip buffer
  SDL_RenderClear(renderer);
}

void Graphics::present() {
  // present back buffer
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

