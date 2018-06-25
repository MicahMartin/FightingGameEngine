#ifndef _scene_h
#define _scene_h 
#include <SDL2_image/SDL_image.h>

// Base class for all game scenes. 
// init method & update method
// up here to abstract general stuff out of scene subclasses?
// idk yet
class Scene{
// a scene should be the final surface thats written to for a current frame
// subclass of scene will basically represent the games current scene state
// so in the main class I can do something like Scene currentScene = new OpeningScene()
// openingscenes update method may return a new scene, idk, gotta find best way to think of state transitions
public:
  virtual void init() = 0;
  virtual void update() = 0;

  virtual SDL_Surface* getCurrentSurface() = 0;
  virtual void setSurface(SDL_Surface* surface) = 0;
};
#endif /* ifndef _scene_h */
