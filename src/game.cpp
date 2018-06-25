#include "game.h"
#include "openingScene.h"
#include "openingState.h"

Game::Game(){}
Game::~Game(){}

void Game::init(){

  coreGraphics.init(640,480);
  inputManager.init();


  printf("Successful intialization\n");
  // set initial state.. idk how i feel about doing it here
  // should probably have an 'engine' class?
  currentState = new OpeningState();
}

void Game::run(){

  // get input, send to gameState

  currentState->update();
  coreGraphics.update();
}
