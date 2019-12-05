#include <bitset>
#include "states/MenuState.h"
#include "states/FightState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
  // TODO: let the menu add its own texture
  mainMenu.addMenuItem("versus", "../data/images/versus.png", 240, 50, [this]{
    printf("pushing the fightState \n");
    stateManager->changeState(new FightState());
  });

  mainMenu.addMenuItem("config", "../data/images/config.png", 240, 50, [this]{
    activeMenu = &configMenu;
  });

  configMenu.addMenuItem("player1_button_config", "../data/images/config.png", 240, 50, [this]{
    VirtualController* vc = inputManager->getVirtualController(0);
    SDL_Event event;
    printf("waiting...\n");
    bool done = false;
    auto waitStart = std::chrono::high_resolution_clock::now();
    while(!done && SDL_WaitEvent(&event)){
      if (event.type == SDL_KEYDOWN) {
        done = true;
      }
    }
    auto waitEnd = std::chrono::high_resolution_clock::now();
    double delayLength = std::chrono::duration<double, std::ratio<100>>(waitEnd - waitStart).count();
  });

  configMenu.addMenuItem("player2_button_config", "../data/images/config.png", 240, 50, [this]{
    printf("not yet implemented\n");
  });

  mainMenu.vc = inputManager->getVirtualController(0);
  configMenu.vc = inputManager->getVirtualController(0);

  activeMenu = &mainMenu;
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
  if (vc->wasPressed(MK)) {
    stateManager->popState();
    return;
  }
  activeMenu->handleInput();
}

void MenuState::update(){
  menuScreen.update();
}

void MenuState::draw(){ 
  menuScreen.draw();
  activeMenu->draw();
}
