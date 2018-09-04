#include <iostream>
// TODO: abstract virtual controller into player 
#include "input/VirtualController.h"
// TODO: Abstract state and screen
#include "states/OpeningState.h"
#include "Game.h"

Game::Game(){}
Game::~Game(){}

void Game::init() {
  coreGraphics.init(640,480);
  inputManager.init();
  running = true;
  // register with input manager so we can catch quit messages
  inputManager.addObserver("game", this);


  printf("Successful intialization\n");

  changeState(new OpeningState(this));
}

void Game::update() {
  // dont wanna couple inputmanager to state here, but in the future virtualController will belong to a player object
  // and input manager will need to know of the player object, so might as well couple player object to this class
  // until I can feel out a better approach
  inputManager.update();

  // pass input to currentState. Might return a new state or stay in the same state
  GameState* newState = currentState->handleInput(inputManager.getVirtualController()->getState());
  if(newState){
    changeState(newState);
  }

  currentState->update();

  // the current state holds a pointer to the currrent screen
  // screen has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  // TODO: might need to decouple currentState and currentScreen?
  coreGraphics.clear();
  currentState->draw();
  coreGraphics.present();

}

void Game::changeState(GameState* newState) {
  // call exit on current currentState for cleanup logic
  if(currentState){
    currentState->exit();
    delete currentState;
  }
  newState->enter();
  currentState = newState;
}

GameState* Game::getCurrentState() {
  return currentState;
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
