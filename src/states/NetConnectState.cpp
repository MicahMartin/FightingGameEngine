#include "states/NetConnectState.h"
#include "input/InputManager.h"
#include "states/FightState.h"

NetConnectState::NetConnectState(){
  mainMenu.vc = inputManager->getVirtualController(0);
  activeMenu = &mainMenu;

  mainMenu.addMenuItem("connect", "connect - enter melty code from clipboard", 240, 50, [this]{
    // printf("reading the IP address from user clipboard\n");
    // std::string inputText = "foobar";
    // inputText = SDL_GetClipboardText();
    // printf("%s\n", inputText.c_str());

    stateManager->setNetplay(true);
    stateManager->pushState(new FightState());
  }, [this]{});
}

NetConnectState::~NetConnectState(){}

void NetConnectState::enter() {
  printf("entered the netplay state\n");
}

void NetConnectState::exit() { 
  printf("leaving the netplay state!\n");
  Mix_PauseMusic();
  // netplaying = false;
  delete this;
}

void NetConnectState::pause() {
  printf("pausing the netplay state\n");
}

void NetConnectState::resume() { 
  printf("resuming the netplay state!\n");
}

void NetConnectState::handleInput() {
  VirtualController* vc = inputManager->getVirtualController(0);
  if (vc->wasPressed(MK)) {
    stateManager->popState();
    return;
  }
  activeMenu->handleInput();
}

void NetConnectState::update() {
  connectScreen.update();
}

void NetConnectState::draw() {
  connectScreen.draw();
  activeMenu->draw();
}
