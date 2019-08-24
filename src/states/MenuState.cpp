#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
  fightState = new FightState();
  menuScreen = new MenuScreen();
  menuScreen->addTexture(&mainMenu.menuCursor.cursorTexture);

  MenuItem versus("versus", [this]{
    printf("pushing the fightState \n");
    stateManager->pushState(fightState);
  });

  MenuItem config("config", [this]{
    printf("not yet implemented\n");
    stateManager->popState();
  });

  mainMenu.menuItemArray.push_back(versus);
  mainMenu.menuItemArray.push_back(config);
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
  VirtualController* vc = inputManager->getVirtualController(0);
  if(vc->wasPressed(VirtualController::UP)){
    mainMenu.moveCursorUp();
  }
  if(vc->wasPressed(VirtualController::DOWN)){
    mainMenu.moveCursorDown();
  }
  if(vc->wasPressed(VirtualController::LEFT)){
    // go back to title
    stateManager->popState();
  }
  if(vc->wasPressed(VirtualController::RIGHT)){
    mainMenu.activate();
  }
}

void MenuState::update(){
  menuScreen->update();
}

void MenuState::draw(){ 
  menuScreen->draw();
}
