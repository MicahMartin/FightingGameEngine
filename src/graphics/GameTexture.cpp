#include "graphics/GameTexture.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <fstream>

GameTexture::GameTexture(){ }
GameTexture::~GameTexture(){ }

bool GameTexture::loadTexture(const char* path) {
  filePath = path;
  texture = textureManager->getTexture(path);
  if(texture == NULL){
    return false;
  }
  return true;
}

bool GameTexture::loadTexture(const char* path, int xCord, int yCord, int width, int height) {
  filePath = path;
  texture = textureManager->getTexture(path);
  setDimensions(xCord, yCord, width, height);
  if(texture == NULL){
    return false;
  }
  return true;
}

bool GameTexture::setTexture(SDL_Texture* _texture){
  texture = _texture;
  return true;
}

bool GameTexture::setText(const char* message){
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(graphics->getFont(), message, {255,255,255});
  SDL_Texture* text = SDL_CreateTextureFromSurface(graphics->getRenderer(), surfaceMessage);
  SDL_FreeSurface(surfaceMessage);

  setTexture(text);
  return true;
}

void GameTexture::render() {
  SDL_RenderCopyF(renderer, texture, NULL, &textRect);
}

void GameTexture::render(SDL_Rect dest) {
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void GameTexture::render(SDL_Rect src, SDL_Rect dest) {
  SDL_RenderCopy(renderer, texture, &src, &dest);
}

void GameTexture::render(SDL_Rect dest, double angle) {
  SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

// TODO: cleaner signature using defaults
void GameTexture::render(bool faceRight) {
  Camera* cam = graphics->getCamera();
  SDL_FRect srcRect = textRect;
  srcRect.x -= cam->cameraRect.x;
  srcRect.y += cam->cameraRect.y;
  SDL_RendererFlip flipFlag = faceRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
  SDL_RenderCopyExF(renderer, texture, NULL, &srcRect, 0, NULL, flipFlag);
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

void GameTexture::setColor(int r, int g, int b){
  SDL_SetTextureColorMod(texture, r, g, b);
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

void GameTexture::setDimensions(int xCord, int yCord, float width, float height) { 
  textRect.x = xCord;
  cartesian ? textRect.y = yCord + (graphics->getWindowHeight() - height) : textRect.y = yCord;
  textRect.w = width;
  halfWidth = width/2;
  textRect.h = height;
}

std::pair<int, int> GameTexture::getDimensions() { 
  return std::make_pair(textRect.w, textRect.h);
}

std::pair<int, int> GameTexture::getFileDimensions() {
    std::pair<int, int> returnPair;
    unsigned int width, height;

    std::ifstream img(filePath);

    // width and height are offset by 16 bytes
    // ty TCP training, everything has a TLV
    img.seekg(16);
    img.read((char *)&width, 4);
    img.read((char *)&height, 4);

    returnPair.first = ntohl(width);
    returnPair.second = ntohl(height);
    img.close();

    imgWidth = returnPair.first;
    imgHeight = returnPair.second;
    return returnPair;
}

std::pair<int, int> GameTexture::getCords() { 
  return std::make_pair(textRect.x, textRect.y);
}
