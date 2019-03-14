#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
  menuScreen.init();

  MenuItem versus;
  versus.title = "versus";
  versus.callBack = [this]{
    printf("Oh man this is a callback in c++! pushing the fightState \n");
    StateManager::getInstance()->pushState(&fightState);
  };

  MenuItem training;
  training.title = "training";
  training.callBack = []{
    printf("Oh man this is a callback in c++ baby\n");
    StateManager::getInstance()->popState();
  };

  menuScreen.addTexture(&mainMenu.menuCursor.cursorTexture);
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

void MenuState::handleInput(VirtualController* vc) {
  if(vc->wasPressed(InputManager::UP)){
    mainMenu.moveCursorUp();
  }
  if(vc->wasPressed(InputManager::DOWN)){
    mainMenu.moveCursorDown();
  }
  if(vc->wasPressed(InputManager::LEFT)){
    // go back to title
    stateManager->popState();
  }
  if(vc->wasPressed(InputManager::RIGHT)){
    mainMenu.activate();
  }
}

void MenuState::update(){
  menuScreen.update();
}

void MenuState::draw(){ 
  menuScreen.draw();
}
