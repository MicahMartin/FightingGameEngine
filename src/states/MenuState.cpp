#include <bitset>
#include "states/MenuState.h"
#include "states/FightState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
  // TODO: let the menu add its own texture
  MenuItem versus("versus", [this]{
    printf("pushing the fightState \n");
    stateManager->changeState(new FightState());
  });

  MenuItem config("config", [this]{
    printf("not yet implemented\n");
    stateManager->popState();
  });

  mainMenu.menuItemArray.push_back(versus);
  mainMenu.menuItemArray.push_back(config);
}

MenuState::~MenuState(){ 
  printf("menuState destructor\n");
}

void MenuState::enter() {
  printf("entered the menu state\n");
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
  delete this;
}

void MenuState::pause() {
  printf("pausing the menu state\n");
}

void MenuState::resume() { 
  printf("resuming the menu state! \n");
}

void MenuState::handleInput() {
  VirtualController* vc = inputManager->getVirtualController(0);
  if(vc->wasPressed(UP)){
    mainMenu.moveCursorUp();
  }
  if(vc->wasPressed(DOWN)){
    mainMenu.moveCursorDown();
  }
  if(vc->wasPressed(MK)){
    // go back to title
    stateManager->popState();
    // THIS IS GONE AFTER POPSTATE BE CAREFUL NOOB
  }
  if(vc->wasPressed(LP)){
    mainMenu.activate();
  }
}

void MenuState::update(){
  menuScreen.update();
}

void MenuState::draw(){ 
  menuScreen.draw();
  mainMenu.draw();
}
