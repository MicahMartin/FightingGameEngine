#include "Game.h"
// TODO: abstract virtual controller into player 
#include "input/VirtualController.h"
// TODO: Abstract state and scene
#include "states/OpeningState.h"
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

  changeState(new OpeningState(this));
}

void Game::update(){

  // dont wanna couple inputmanager to state here, but in the future virtualController will belong to a player object
  // and input manager will need to know of the player object, so might as well couple player object to this class
  // until I can feel out a better approach
  inputManager.update();
  // std::cout << "Heres the current inputs bit" << std::bitset<32>(lastInput.getKeyCode()) << std::endl;
  // gameState is passed a reference to this object so it can call changeState and read input from getVirtualController();
  gameState->update();
  // the current state holds a pointer to the currrent scene
  // scene has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  // currentScene gets updated by currentState
  // TODO: might need to decouple currentState and currentScene?
  // Scene* currentScene = currentState->getCurrentScene();
  // currentScene->update();
  coreGraphics.update();
}

void Game::changeState(GameState* newState) {
  // call exit on current gameState for cleanup logic
  if(gameState){
    gameState->exit();
  }
  // call enter on next gameState for enter logic
  newState->enter();
  gameState = newState;
}

// TODO: push / pop state so we can have state stack.. useful for something like a pause state on top of a game state
//void Game::pushState(GameState* newState) {}
//void Game::popState(GameState* newState) {}

GameState* Game::getCurrentState() {

  return gameState;
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

bool Game::stillRunning(){
  return running;
}

InputManager* Game::getInputManager(){
  return &inputManager;
}
