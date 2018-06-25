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

  // get input, send to currentState
  // Input currentInput = inputManager.getLastItemFromInputArray();

  currentState->update();
  // the current state holds a pointer to the currrent scene
  // scene has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  coreGraphics.update(currentState->getCurrentScene()->getCurrentSurface());
}
