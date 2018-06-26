#include "game.h"
#include "openingScene.h"
#include "openingState.h"

Game::Game(){}
Game::~Game(){}

void Game::init(){

  coreGraphics.init(640,480);
  inputManager.init();
  inputManager.addObserver(this);


  printf("Successful intialization\n");
  running = true;
  // set initial state.. idk how i feel about doing it here
  // should probably have an 'engine' class?
  currentState = new OpeningState();
}

void Game::run(){

  // get input, send to currentState
  inputManager.update();
  //Input lastInput = inputManager.getLastItemFromInputArray();

  currentState->update();
  // the current state holds a pointer to the currrent scene
  // scene has a surface pointer with all the pixels that need to be
  // written and swapped this frame
  coreGraphics.update(currentState->getCurrentScene()->getCurrentSurface());
}

void Game::onNotify(const char* messageType) {
  printf("Notification recieved: %s\n", messageType);

  // handle quit request
  // def need to make messageType an enum so I can switch on it
  // strcmp returns 0 on true, dumb
  if(std::strcmp(messageType, "QUIT_REQUEST") == 0){
    printf("Shutting down\n");
    running = false;
  }else{
    printf("Not quitting\n");
  }
  
}

bool Game::stillRunning(){
  return running;
}
