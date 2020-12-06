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
  bool loadTexture(const char* path, int xCord, int yCord, int width, int height);
  bool setTexture(SDL_Texture* texture);
  bool setText(const char* message);
  void render();
  void render(SDL_Rect dest);
  void render(SDL_Rect src, SDL_Rect dest);
  void render(SDL_Rect dest, double angle);
  void render(bool flip);
  void setCamera(Camera* camera);

  void incTransperancy();
  void decTransperancy();
  void setTransperancy(uint8_t tran);
  void shake();
  void setColor(int red, int green, int blue);
  uint8_t getTransperancy();

  void setBlendMode(SDL_BlendMode blendmode);

  void setCords(int xCord, int yCord);
  void setXCord(int xCord);
  void setYCord(int yCord);
  void setDimensions(int xCord, int yCord, float width, float height);
  std::pair<int, int> getDimensions();
  std::pair<int, int> getFileDimensions();
  std::pair<int, int> getCords();

  bool cartesian = false;
  const char* filePath;
  int xCord, yCord, width, height;
  int halfWidth;
  int imgWidth, imgHeight;

private:
  Graphics* graphics = Graphics::getInstance();
  TextureManager* textureManager = TextureManager::getInstance();
  SDL_Renderer* renderer = graphics->getRenderer();
  Camera* camera;

  SDL_Texture* texture;
  SDL_FRect textRect;
  uint8_t alpha = 255;
  /* data */
};
#endif /* ifndef _GameTexture_h */
