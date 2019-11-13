#ifndef _GameTexture_h
#define _GameTexture_h 
#include <SDL2_image/SDL_image.h>
#include "Graphics.h"
#include "TextureManager.h"
#include "Camera.h"

class GameTexture {
public:
  // TODO: Count references to textureObj when adding/deleting from cache, 
  // free texture when no references are left
  GameTexture();
  ~GameTexture();

  bool loadTexture(const char* path);
  void render();
  void render(bool flip);
  void setCamera(Camera* camera);

  void incTransperancy();
  void decTransperancy();
  void setTransperancy(uint8_t tran);
  uint8_t getTransperancy();

  void setBlendMode(SDL_BlendMode blendmode);

  void setCords(int xCord, int yCord);
  void setDimensions(int xCord, int yCord, int width, int height);
  std::pair<int, int> getDimensions();
  std::pair<int, int> getCords();

  bool cartesian = false;
  int xCord, yCord, width, height;
  int halfWidth;

private:
  Graphics* graphics = Graphics::getInstance();
  TextureManager* textureManager = TextureManager::getInstance();
  SDL_Renderer* renderer = graphics->getRenderer();
  Camera* camera;

  SDL_Texture* texture;
  SDL_Rect textRect;
  uint8_t alpha = 255;
  /* data */
};
#endif /* ifndef _GameTexture_h */
