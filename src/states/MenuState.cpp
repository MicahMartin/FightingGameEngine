#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
  menuScreen.init();

  MenuItem versus;
  versus.title = "versus";
  MenuItem training;
  training.title = "training";

  MenuItem config;
  config.title = "config";
  MenuItem about;
  about.title = "about";

  mainMenu.menuItemArray.push_back(versus);
  mainMenu.menuItemArray.push_back(training);
}

MenuState::~MenuState(){ }

void MenuState::enter() {
  printf("entered the menu state\n");
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
}

void MenuState::pause() {
  printf("pausing the menu state\n");
}

void MenuState::resume() { 
  printf("resuming the menu state! \n");
}

void MenuState::handleInput(StateManager* sm, VirtualController* vc) {
  if(vc->wasPressed(InputManager::RIGHT)){
    mainMenu.activate();
  }
  if(vc->wasPressed(InputManager::DOWN)){
    mainMenu.moveCursorDown();
  }
  if(vc->wasPressed(InputManager::UP)){
    mainMenu.moveCursorUp();
  }
  if(vc->wasPressed(InputManager::LEFT)){
    sm->popState();
  }
}

void MenuState::update(){
  menuScreen.update();
}

void MenuState::draw(){ 
  menuScreen.draw();
}
