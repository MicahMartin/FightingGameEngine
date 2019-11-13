#ifndef _TextureManager_h
#define _TextureManager_h

#include <unordered_map>
#include <string>
#include "Graphics.h"

// handle SDL init, window stuff, and video buffer swapping at the end of every frame
class TextureManager final {
public:
  static TextureManager* getInstance(){
    static TextureManager instance;
    return &instance;
  };

  void init();
  SDL_Texture* getTexture(const char* path);
private:
  TextureManager() = default;
  ~TextureManager(){ };
  TextureManager(const TextureManager&) = delete;
  TextureManager& operator=(const TextureManager&) = delete;
  TextureManager(TextureManager&&) = delete;
  TextureManager& operator=(TextureManager&&) = delete;

  Graphics* graphics = Graphics::getInstance();
  SDL_Renderer* renderer = graphics->getRenderer();
  std::unordered_map<std::string, SDL_Texture*> textureCache;
};


#endif
