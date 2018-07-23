#include "game.h"
// TODO: abstract virtual controller into player 
#include "virtualController.h"
// TODO: Abstract state and scene
#include "openingScene.h"
#include "openingState.h"
#include <iostream>

Game::Game(){}
Game::~Game(){}

void Game::init(){

  coreGraphics.init(640,480);
  inputManager.init();
  running = true;
  // register with input manager so we can catch quit messages
  inputManager.addObserver("game", this);


  printf("Successful intialization\n");
  // set initial state.. idk how i feel about doing it here
  // should probably have an 'engine' class?
  currentState = new OpeningState();
}

void Game::run(){

  // get input, send to currentState
  // TODO: build some kind of virtual controller, send input to virtual controller
  // dont wanna couple inputmanager to state here
  inputManager.update();
  // std::cout << "Heres the current inputs bit" << std::bitset<32>(lastInput.getKeyCode()) << std::endl;
  // currentState->update(&inputManager);
  // the current state holds a pointer to the currrent scene
  // scene has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  // currentScene gets updated by currentState
  // TODO: need to decouple currentState and currentScene
  // Scene* currentScene = currentState->getCurrentScene();
  // currentScene->update();
  coreGraphics.update();
}

void Game::onNotify(const char* eventName) {
  printf("Notification recieved: %s\n", eventName);

  // handle quit request
  // def need to make eventName an enum so I can switch on it
  // strcmp returns 0 on true, dumb
  if(std::strcmp(eventName, "QUIT_REQUEST") == 0){
    printf("Shutting down\n");
    running = false;
    // printf("Here is the input history size %d\n", inputManager.getInputHistorySize());
  }else{
    printf("Not quitting\n");
  }
  
}

bool Game::stillRunning(){
  return running;
}
