#include "graphics/GameTexture.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

GameTexture::GameTexture(){ }
GameTexture::~GameTexture(){ }

bool GameTexture::loadTexture(const char* path) {
  texture = textureManager->getTexture(path);
  if(texture == NULL){
    return false;
  }
  return true;
}

void GameTexture::render() {
  SDL_RenderCopy(renderer, texture, NULL, &textRect);
}

void GameTexture::render(SDL_Rect dest) {
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void GameTexture::render(SDL_Rect dest, double angle) {
  SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

// TODO: cleaner signature using defaults
void GameTexture::render(bool faceRight) {
  Camera* cam = graphics->getCamera();
  SDL_Rect srcRect = textRect;
  srcRect.x -= cam->cameraRect.x;
  SDL_RendererFlip flipFlag = faceRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
  SDL_RenderCopyEx(renderer, texture, NULL, &srcRect, 0, NULL, flipFlag);
}

void GameTexture::incTransperancy(){
  alpha++;
  setTransperancy(alpha);
}

void GameTexture::decTransperancy(){
  alpha++;
  setTransperancy(alpha);

}

void GameTexture::setTransperancy(uint8_t tran){
  alpha = tran;
  SDL_SetTextureAlphaMod(texture, alpha);
}

void GameTexture::shake(){
}

uint8_t GameTexture::getTransperancy(){
  return alpha;
}

void GameTexture::setBlendMode(SDL_BlendMode mode){
  SDL_SetTextureBlendMode(texture, mode);
}


void GameTexture::setCords(int xCord, int yCord) { 
  textRect.x = xCord;
  cartesian ? textRect.y = yCord + (graphics->getWindowHeight() - textRect.h) : textRect.y = yCord;
}

void GameTexture::setXCord(int xCord) { 
  textRect.x = xCord;
}

void GameTexture::setYCord(int yCord) { 
  textRect.y = yCord;
  cartesian ? textRect.y = yCord + (graphics->getWindowHeight() - textRect.h) : textRect.y = yCord;
}

void GameTexture::setDimensions(int xCord, int yCord, int width, int height) { 
  textRect.x = xCord;
  cartesian ? textRect.y = yCord + (graphics->getWindowHeight() - height) : textRect.y = yCord;
  textRect.w = width;
  halfWidth = width/2;
  textRect.h = height;
}

std::pair<int, int> GameTexture::getDimensions() { 
  return std::make_pair(textRect.w, textRect.h);
}

std::pair<int, int> GameTexture::getCords() { 
  return std::make_pair(textRect.x, textRect.y);
}
