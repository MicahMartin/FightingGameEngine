#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "Game.h"

Game::Game(){}
Game::~Game(){}

void Game::init() {
  // init Graphics
  graphics->init(1280, 720);
  // register with input manager so we can catch quit messages
  inputManager->init();
  inputManager->addObserver("game", this);
  // add the initial virtual controller
  inputManager->addVirtualController(&virtualControllers[0]);
  inputManager->addVirtualController(&virtualControllers[1]);
  // set the state to the title screen
  stateManager->pushState(new OpeningState());
}

void Game::update() {
  ++gameTime;
  // read input event stack for this frame and send to virtual controller(s)
  double inputManagerStart = SDL_GetTicks();
  inputManager->update();
  double inputManagerEnd = SDL_GetTicks();
  inputLength = inputManagerEnd-inputManagerStart;

  // pass input to currentState. side effects inbound
  GameState* currentState = stateManager->getState();
  // this method modifies state stack
  double handleInputFrameStart = SDL_GetTicks();
  currentState->handleInput();
  double handleInputFrameEnd = SDL_GetTicks();
  handleInputLength = handleInputFrameEnd-handleInputFrameStart;

  double stateUpdateStart = SDL_GetTicks();
  currentState->update();
  double stateUpdateEnd = SDL_GetTicks();
  updateLength = stateUpdateEnd-stateUpdateStart;

  // the current state holds a pointer to the currrent screen
  // screen has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  // TODO: Shit is really slow because we're creating textures every frame instead of preallocating
  graphics->clear();
  double drawStart = SDL_GetTicks();
  currentState->draw();
  graphics->present();
  double drawEnd = SDL_GetTicks();
  drawLength = drawEnd-drawStart;
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
