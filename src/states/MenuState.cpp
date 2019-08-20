#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
  menuScreen.init();

  MenuItem versus("versus", [this]{
    printf("pushing the fightState \n");
  });

  MenuItem training("training", []{
    printf("callback\n");
  });

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

void MenuState::handleInput() {
}

void MenuState::update(){
}

void MenuState::draw(){ 
}
