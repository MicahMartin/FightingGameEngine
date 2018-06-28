#include "input.h"


Input::Input() {}

//Input::Input(SDL_Event event_ref) {

//  event = event_ref;
//  keyName = SDL_GetKeyName(event.key.keysym.sym);
//}

Input::~Input() {
  
}

void Input::setBits(SDL_Event event) {

  switch (event.key.keysym.sym) {
    // Directional inputs
    case SDLK_DOWN:
      inputEnum |= DOWN;
      break;
    case SDLK_RIGHT:
      inputEnum |= RIGHT;
      break;
    case SDLK_LEFT:
      inputEnum |= LEFT;
    break;
    case SDLK_UP:
      inputEnum |= UP;
    break;
  }
}

//only until I can think of a cleaner approach
// this shit is actually useful outside of embeded

void Input::clearBits(SDL_Event event) {

  switch (event.key.keysym.sym) {
    // Directional inputs
    case SDLK_DOWN:
      inputEnum &= ~DOWN;
      break;
    case SDLK_RIGHT:
      inputEnum &= ~RIGHT;
      break;
    case SDLK_LEFT:
      inputEnum &= ~LEFT;
    break;
    case SDLK_UP:
      inputEnum &= ~UP;
    break;
  }
}

uint32_t Input::getKeyCode() {
  return inputEnum;
}
