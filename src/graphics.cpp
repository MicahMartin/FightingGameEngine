#include "graphics.h"
#include <cmath>
#include <stdexcept>

Graphics::Graphics(){}

Graphics::~Graphics(){
  SDL_DestroyWindow(window);
  SDL_Quit();
}

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

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if( renderer == NULL ){
    throw(std::runtime_error(SDL_GetError()));
  }

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  //Initialize PNG loading
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  if( !( IMG_Init(imgFlags) & imgFlags)){
    throw(std::runtime_error(IMG_GetError()));
  }

  // We want to show image surfaces inside of the window and in order to do that 
  // we need to get the entire image surface contained by the window
  // windowSurface = SDL_GetWindowSurface(window);
}

void Graphics::resizeWindow(int w, int h){}

void Graphics::update() {
  // Update the window surface
  // SDL_BlitSurface(surfaceToBlit, NULL, windowSurface, NULL); 
  // SDL_UpdateWindowSurface(window); 
}

int Graphics::getWindowWidth(){
  return width;
}

int Graphics::getWindowHeight(){
  return height;
}

SDL_Texture* Graphics::loadTexture(const char* path, SDL_Renderer* rend){
  SDL_Texture* text;

  SDL_Surface* img = IMG_Load(path);
  if(img == NULL){
    printf("Error loading image with path %s, error: %s", path, IMG_GetError());
  }
  SDL_FreeSurface(img);

  text = SDL_CreateTextureFromSurface(rend, img);
  return text;
  
}

//SDL_Surface* Graphics::getWindowSurface() {
//  if( windowSurface == NULL ){
//    throw( "Windowsurface pointer is null" );
//  }
//  return windowSurface;
//}
