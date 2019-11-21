#include "graphics/TextureManager.h"

void TextureManager::init(){};

SDL_Texture* TextureManager::getTexture(const char* path){
  if (textureCache.find(std::string(path)) != textureCache.end()) {
    printf("returning cached texture %s\n", path);
    return textureCache[std::string(path)];
  } else {
    printf("creating new texture %s\n", path);
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
