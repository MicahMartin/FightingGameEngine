#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "Game.h"

Game::Game(){}
Game::~Game(){}

void Game::init() {
  running = true;

  // init stuff
  coreGraphics.init(640,480);
  inputManager.init();

  printf("Successful Init baybe\n");

  // register with input manager so we can catch quit messages
  inputManager.addObserver("game", this);

  // set the state to the title screen
  stateManager.pushState(new OpeningState());
  printf("state pushed\n");
}

void Game::update() {
  gameTime++;
  // read input event stack for this frame and send to virtual controllers
  inputManager.update();

  // pass input to currentState. Might return a new state or stay in the same state
  GameState* currentState = stateManager.getState();
  currentState->handleInput(inputManager.getVirtualController()->getStickState());

  currentState->update(&stateManager);

  // the current state holds a pointer to the currrent screen
  // screen has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  // TODO: might need to decouple currentState and currentScreen?
  coreGraphics.clear();
  currentState->draw();
  coreGraphics.present();
}

void Game::onNotify(const char* eventName) {
  printf("Game: handling event - %s\n", eventName);

  // handle quit request
  // def need to make eventName a  bit flag enum so I can switch on it
  // strcmp returns 0 on true, dumb
  if(std::strcmp(eventName, "QUIT_REQUEST") == 0){
    printf("Shutting down\n");
    running = false;
    // printf("Here is the input history size %d\n", inputManager.getInputHistorySize());

  }else{
    printf("Not quitting\n");
  }
}

bool Game::stillRunning() {
  return running;
}

InputManager* Game::getInputManager() {
  return &inputManager;
}

Graphics* Game::getGraphics() {
  return &coreGraphics;
}

StateManager* Game::getStateManager() {
  return &stateManager;
}
