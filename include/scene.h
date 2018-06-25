#ifndef _scene_h
#define _scene_h 
#include <SDL_image.h>

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
  Scene();
  virtual ~Scene();
  virtual void init();
  virtual void update();

  virtual SDL_Surface* getCurrentSurface();
  virtual void setSurface(SDL_Surface* surface);
};
#endif /* ifndef _scene_h */
