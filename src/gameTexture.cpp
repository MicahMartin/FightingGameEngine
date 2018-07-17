#include "gameTexture.h"
#include <cmath>
#include <stdexcept>

GameTexture::GameTexture(){
}
GameTexture::~GameTexture(){
}

bool GameTexture::loadTexture(const char* path, SDL_Renderer* rend){

  SDL_Surface* img = IMG_Load(path);
  if(img == NULL){
    printf("Error loading image with path %s, error: %s", path, IMG_GetError());
    return false;
  }

  texture = SDL_CreateTextureFromSurface(rend, img);
  SDL_FreeSurface(img);

  return true;
}
