#include "inputManager.h"
#include "input.h"
#include <experimental/optional>

InputManager::InputManager(){
}

InputManager::~InputManager(){}

void InputManager::init() {
  
}
void InputManager::update() {
  SDL_Event event;

  while( SDL_PollEvent(&event) != 0 ){
    if( event.type ==  SDL_QUIT ){

      notifyAll("QUIT_REQUEST");
    }else if( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP ){

      // for each event, flip the corresponding bit on the inputEvent.inputEnum
      if(event.type == SDL_KEYDOWN){

        printf("bits being flipped on current frame of input\n");
        inputEvent.setBits(event);
      }else{

        inputEvent.clearBits(event);
      }
      inputList.push_back(inputEvent);
    }
  }
}

std::experimental::optional<Input> InputManager::getLastInput(){

  if ( inputList.empty() ) {
    return std::experimental::nullopt; 
  }
  // get the most recent input for this game tick
  Input returnInput = inputList.back();

  // copy everything else into input history list
  // 2 data structures and a copy because I cant think of a better way 
  // to look at the most recent input every game tick, confirm its been looked at
  // and save a history of inputs
  inputHistory.insert(inputHistory.end(), inputList.begin(), inputList.end());
  inputList.clear();
  return returnInput;
}

void InputManager::addObserver(Observer* observer){
  printf("Observer added to inputManager \n");
  observerList.push_back(observer);
};

void InputManager::removeObserver(Observer* observer){

};

void InputManager::notifyAll(const char* messageType){

  for( Observer* observer : observerList ){
    observer->onNotify(messageType);
  }


};
int InputManager::getInputHistorySize(){
  return inputHistory.size();

};
