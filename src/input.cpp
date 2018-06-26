#include "input.h"

Input::Input() {}

Input::Input(SDL_Event event_ref) {

  event = event_ref;
}
Input::~Input() {
  
}
