#include "inputManager.h"
#include "input.h"

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {}

void InputManager::update() {

  SDL_Event event;

  while( SDL_PollEvent(&event) != 0 ){
    if( event.type ==  SDL_QUIT ){

      notifyAll("QUIT_REQUEST");
    }else if( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP ){

      // for each event, flip the corresponding bit on the inputEvent.inputEnum
      if(event.type == SDL_KEYDOWN){

        printf("bits being flipped on current frame of input\n");
        setBits(event);
      }else{

        clearBits(event);
      }
    }
  }
}

void InputManager::setBits(SDL_Event event) {

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

void InputManager::clearBits(SDL_Event event) {

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


uint32_t InputManager::getInputByte(){

  // get the most recent input for this game tick

  // copy everything else into input history list
  // 2 data structures and a copy because I cant think of a better way 
  // to look at the most recent input every game tick, confirm its been looked at
  // and save a history of inputs
  return inputEnum;
}

void InputManager::addObserver(Observer* observer){
  printf("Observer added to inputManager \n");
  observerList.push_back(observer);
};

void InputManager::removeObserver(Observer* observer){ };

void InputManager::notifyAll(const char* messageType){

  for( Observer* observer : observerList ){
    observer->onNotify(messageType);
  }


};
int InputManager::getInputHistorySize(){ return 0;};
