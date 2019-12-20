#include <bitset>
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
  printf("opening state constructor\n");
  //Load music
  openingSong = Mix_LoadMUS("../data/audio/openingTheme.mp3");
  if(openingSong == NULL) {
    printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
  }
}

OpeningState::~OpeningState() { 
  printf("openingState destructor called\n");
}

void OpeningState::enter() {
  // initialize the menu state & opening screen
  printf("Entered the opening state\n");
  Mix_PlayMusic(openingSong, -1);
}

void OpeningState::exit() { 
}

void OpeningState::pause() {
  printf("pausing the opening state\n");
}

void OpeningState::resume() { 
  printf("resuming the opening state! \n");
  if( Mix_PausedMusic() == 1 ) {
    //Resume the music
    Mix_ResumeMusic();
  }
}

void OpeningState::handleInput() {
  VirtualController* input = inputManager->getVirtualController(0);
  if(input->wasPressed(DOWN)){
    printf("down was pressed\n");
    stateManager->pushState(new MenuState());
  }
}

void OpeningState::update() {
  openingScreen.update();
}

void OpeningState::draw() {
  openingScreen.draw();
}
