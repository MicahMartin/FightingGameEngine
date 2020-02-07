#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "states/FightState.h"
#include "game.h"

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
 //  For more precise ticks use SDL_GetPerformanceFrequency & SDL_GetPerformanceCounter so you can get a milisecond value, 
 //  but as a more precise double
  ++gameTime;
  // read input event stack for this frame and send to virtual controller(s)
  double inputManagerStart = SDL_GetTicks();
  inputManager->update();
  double inputManagerEnd = SDL_GetTicks();
  inputLength = inputManagerEnd-inputManagerStart;
  // printf("inputLength %f\n", inputLength);

  // printf("made it back to update\n");
  GameState* currentState = stateManager->getState();
  // if(strcmp(currentState->stateName, "FightState") == 0){
  //   FightState* fightState = (FightState*)currentState;
  //   printf("in fight state %s, %d\n", fightState->stateName, fightState->everythingCompiled);
  //   if(fightState->everythingCompiled){
  //     printf("everything compiled\n");
  //     inFightState = true;
  //   }
  // }
  currentState->gameTime = gameTime;

  double handleInputFrameStart = SDL_GetTicks();

  currentState->handleInput();
  currentState = stateManager->getState();

  double handleInputFrameEnd = SDL_GetTicks();
  handleInputLength = handleInputFrameEnd-handleInputFrameStart;

  double stateUpdateStart = SDL_GetTicks();
  currentState->update();
  currentState = stateManager->getState();
  double stateUpdateEnd = SDL_GetTicks();
  updateLength = stateUpdateEnd-stateUpdateStart;
  // printf("updateLength %f\n", updateLength);
  if (currentState->slowMode && currentState->slowModeCounter++ == 3) {
    currentState->slowModeCounter = 0;
  }

  // the current state holds a pointer to the currrent screen
  // screen has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  double clearStart = SDL_GetTicks();
  graphics->clear();
  double clearEnd = SDL_GetTicks();
  clearLength = clearEnd - clearStart;

  double stateDrawStart = SDL_GetTicks();
  currentState->draw();
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
    exit(EXIT_SUCCESS);
    // printf("Here is the input history size %d\n", inputManager.getInputHistorySize());

  }else{
    printf("Not quitting\n");
  }
}
