#include "menuState.h"
#include "game.h"
#include "virtualController.h"
#include <bitset>
#include <iostream>

MenuState::MenuState(Game* gameP){
  // set game pointer
  game = gameP;
  // get virtual controller pointer(s) from game pointer
  p1Controller = game->getInputManager()->getVirtualController();
}

MenuState::~MenuState(){}

void MenuState::update(){

  uint8_t stickState = p1Controller->getState();

  printf("inside the menu state heres current state of controller %d\n", stickState);

  if(stickState == VirtualController::DOWNLEFT){
    printf("Stick is in downleft state");
    return;
  }

}

void MenuState::draw(){
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
}

void MenuState::enter() {
  // setCurrentScene(new OpeningScene);
}

void MenuState::exit() { 

  printf("leaving the opening state! \n");
  // cleanup
}

void MenuState::pause() { }

void MenuState::resume() { }

//void MenuState::setCurrentScene(Scene* scene){

// currentScene = scene; 
//}

//Scene* MenuState::getCurrentScene(){

//  return currentScene;
//}
