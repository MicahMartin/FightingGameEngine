#ifndef _Scene_h
#define _Scene_h 

#include <SDL2_image/SDL_image.h>
#include "graphics/GameTexture.h"

// Base class for all game scenes. 
// init method & update method
// up here to abstract general stuff out of scene subclasses?
// idk yet
class Scene{
// a scene should be the final surface thats written to for a current frame
// subclass of scene will basically represent the games current scene state
// so in the main class I can do something like Scene currentScene = new OpeningScene()
public:
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  virtual void addTexture(GameTexture* gText) = 0;
  virtual void removeTexture(int index) = 0;

  
};
#endif /* ifndef _scene_h */
