#include "graphics/GameTexture.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

GameTexture::GameTexture(SDL_Renderer* renderer):renderer(renderer){
}
GameTexture::~GameTexture(){
}

bool GameTexture::loadTexture(const char* path){
  SDL_Surface* img = IMG_Load(path);
  if(img == NULL){
    printf("Error loading image with path %s, error: %s", path, IMG_GetError());
    return false;
  }

  texture = SDL_CreateTextureFromSurface(renderer, img);
  SDL_FreeSurface(img);

  return true;
}

void GameTexture::render() {
  SDL_RenderCopy(renderer, texture, NULL, &textRect);
}

void GameTexture::setDimensions(int xCord, int yCord, int width, int height) { 
  textRect.x = xCord;
  textRect.y = yCord;
  textRect.w = width;
  textRect.h = height;
}

std::pair<int, int> GameTexture::getDimensions() { 
  return std::make_pair(textRect.w, textRect.h);
}

std::pair<int, int> GameTexture::getCords() { 
  return std::make_pair(textRect.x, textRect.y);
}
