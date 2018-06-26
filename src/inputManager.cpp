#include "inputManager.h"
#include "input.h"

InputManager::InputManager(){
}

InputManager::~InputManager(){}

void InputManager::init() {
  
}
void InputManager::update() {

  while( SDL_PollEvent(&event) != 0 ){
    if( event.type ==  SDL_QUIT ){
      notifyAll("QUIT_REQUEST");
    }else if( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
      Input inputEvent(event);
      inputList.push_back(inputEvent);
    }
  }
}
Input InputManager::getLastItemFromInputArray() {
 return inputList.back(); 
}

void InputManager::addObserver(Observer* observer){
  printf(" Observer added to inputManager \n");
  observerList.push_back(observer);
};

void InputManager::removeObserver(Observer* observer){

};

void InputManager::notifyAll(const char* messageType){

  for( Observer* observer : observerList ){
    observer->onNotify(messageType);
  }


};
