#include "graphics/TextureManager.h"

void TextureManager::init(){

};

SDL_Texture* TextureManager::getTexture(const char* path){
  if (textureCache.count(std::string(path)) > 0) {
    return textureCache[std::string(path)];
  } else {
    SDL_Surface* img = IMG_Load(path);
    if(img == NULL){
      printf("Error loading image with path %s, error: %s", path, IMG_GetError());
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    textureCache[std::string(path)] = texture;
    return texture;
  }
}
