#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "states/FightState.h"
#include "game.h"
#include <boost/serialization/list.hpp>


int volume = 100;
Game::Game(){

  // init Graphics
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_AUDIO) != 0 ){
    throw( std::runtime_error(SDL_GetError()) );
  }
  Mix_Init(MIX_INIT_MP3);
  if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0) {
    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
  }

  graphics->init();
  // register with input manager so we can catch quit messages
  inputManager->init();
  inputManager->addObserver("game", this);
  // add the initial virtual controller
  virtualControllers[0].controllerIndex = 1;
  virtualControllers[1].controllerIndex = 2;
  inputManager->addVirtualController(&virtualControllers[0]);
  inputManager->addVirtualController(&virtualControllers[1]);
  if (inputManager->p1SDLController != NULL) {
    inputManager->stickToVC[inputManager->p1SDLController] = &virtualControllers[0];
  }
  if (inputManager->p2SDLController != NULL) {
    inputManager->stickToVC[inputManager->p2SDLController] = &virtualControllers[1];
  }
  // set the state to the title screen
  OpeningState* openingState = new OpeningState();
  stateManager->pushState(openingState);
}

Game::~Game(){}

void Game::update() {
  ++gameTime;
  inputManager->update();

  GameState* currentState = stateManager->getState();
  currentState->gameTime = gameTime;

  if(std::strcmp(currentState->stateName, "FIGHT_STATE") == 0){
    FightState* fightState = (FightState*)currentState;
    if (fightState->netPlayState && fightState->doneSync) {
      fightState->netPlayHandleInput();
    }
  }
  currentState->handleInput();
  currentState = stateManager->getState();
  currentState->update();
  currentState = stateManager->getState();
  
}
void Game::draw(){
  // the current state holds a pointer to the currrent screen
  // screen has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  double clearStart = SDL_GetTicks();
  graphics->clear();
  double clearEnd = SDL_GetTicks();
  clearLength = clearEnd - clearStart;

  double stateDrawStart = SDL_GetTicks();
  stateManager->getState()->draw();
  double stateDrawEnd = SDL_GetTicks();
  stateDrawLength = stateDrawEnd - stateDrawStart;

  double drawStart = SDL_GetTicks();
  // printf("drawStart %f\n", drawStart);
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
    SDL_CloseAudio();
    exit(EXIT_SUCCESS);
    // printf("Here is the input history size %d\n", inputManager.getInputHistorySize());

  }else if (std::strcmp(eventName, "RESTART_REQUEST") == 0){
    printf("restarting\n");
    GameState* currentState = stateManager->getState();
    while(std::strcmp(currentState->stateName, "OPENING_STATE") != 0){
      stateManager->popState();
      currentState = stateManager->getState();
    }
  } else if (std::strcmp(eventName, "SAVE_STATE") == 0){
    GameState* currentState = stateManager->getState();
    if(std::strcmp(currentState->stateName, "FIGHT_STATE") == 0){
      FightState* statePointer = (FightState*)currentState;
      statePointer->saveState(&statePointer->buffer, &statePointer->bufferLen, statePointer->gameTime);
    }
  } else if (std::strcmp(eventName, "LOAD_STATE") == 0){
    GameState* currentState = stateManager->getState();
    if(std::strcmp(currentState->stateName, "FIGHT_STATE") == 0){
      FightState* statePointer = (FightState*)currentState;
      statePointer->loadState(statePointer->buffer, statePointer->bufferLen);
    }
  } else if (std::strcmp(eventName, "VOLUME_UP_REQUEST") == 0){
    Mix_Volume(-1, 64);
  } else if (std::strcmp(eventName, "VOLUME_DOWN_REQUEST") == 0){
    Mix_Volume(-1, 0);
  } else if (std::strcmp(eventName, "VIEW_CBOXES_TOGGLE") == 0){
    graphics->getFlag(GF_SHOW_CB) ? graphics->clearFlag(GF_SHOW_CB) : graphics->setFlag(GF_SHOW_CB);
  }
}
