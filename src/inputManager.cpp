#include "inputManager.h"

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {
  // TODO: this is gonna get called by the player object
  virtualController.init();
}

void InputManager::update() {

  SDL_Event event;

  while( SDL_PollEvent(&event) != 0 ){
    switch (event.type) {
      case SDL_KEYDOWN:
        virtualController.setBit(event);
      break;
      case SDL_KEYUP:
        virtualController.clearBit(event);
      break;
      case SDL_QUIT:
        notifyOne("game", "QUIT_REQUEST");
      break;
      default:
      break;
    }
  }
  // std::cout << "Heres the current byte of input" << std::bitset<16>(inputEnum) << std::endl;
  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  virtualController.update();
}

VirtualController* InputManager::getVirtualController(){
  return &virtualController;
}

// Subject 
void InputManager::addObserver(const char* observerName, Observer* observer){
  printf("Observer added to inputManager \n");
  observerList.insert(std::make_pair(observerName, observer));
};

void InputManager::removeObserver(const char* observerName){

  observerList.erase(observerName);
};

void InputManager::notifyAll(const char* eventName){

  for( auto const& [key, observer] : observerList ){
    observer->onNotify(eventName);
  }
};

void InputManager::notifyOne(const char* observerName, const char* eventName){
  Observer* observer = observerList.at(observerName);
  observer->onNotify(eventName);

}
