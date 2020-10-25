#include <SDL2/SDL.h>
#include <chrono>
#include "game.h"
#include "ggponet.h"

typedef std::chrono::high_resolution_clock Clock;
int main(int argc, char* args[]) {
  static double FPS = 1000/60;


  // Game.init
  Game game;
  if (argc >= 1) {
    game.stateManager->getInstance()->setPnum(std::stoi(args[1]));
    std::string realWindowName = game.graphics->windowName + std::to_string(std::stoi(args[1]));
    SDL_SetWindowTitle(game.graphics->getWindow(), realWindowName.c_str());
    if (argc >= 3) {
      if (std::stoi(args[2]) + std::stoi(args[3]) == 0) {
        game.graphics->resizeWindow(true);
      } else {
        game.graphics->resizeWindow(std::stoi(args[2]), std::stoi(args[3]));
      }
    }
    // port = std::stoi(args[2]);
    // enemyPort = std::stoi(args[3]);
  }

  double gameStart = SDL_GetTicks();
  int frameCount = 0;
  while(game.running){
    auto frameStart = Clock::now();

    game.update();
    game.draw();
    // we want to run the update stuff once every frame (16~ MS)
    // so we check how many MS its taken us to generate the current frame 
    // so if it took 3MS to render this frame, we have 13MS of free time before reading input and rendering again
    auto frameEnd = Clock::now();
    double delayLength = FPS - (std::chrono::duration<double, std::ratio<1000>>(frameEnd - frameStart).count());
    if(delayLength > 0){
      GameState* currentState = game.stateManager->getState();
      if(std::strcmp(currentState->stateName, "FIGHT_STATE") == 0){
        FightState* fightState = (FightState*)currentState;
        if (fightState->netPlayState) {
          // printf("calling ggpo idle\n");
          auto idleStart = Clock::now();
          ggpo_idle(fightState->ggpo, delayLength);
          auto idleEnd = Clock::now();
          double newDelay = (std::chrono::duration<double, std::ratio<1000>>(idleEnd - idleStart).count());
          SDL_Delay(delayLength - newDelay);
        } else {
          // printf("calling normal delay\n");
          SDL_Delay(delayLength);
        }
      }else {
        SDL_Delay(delayLength);
      }
    } else  { 
      printf("long delay %d: \n", static_cast<int>(delayLength));
    }
    frameCount++;
    float avgFPS = frameCount/ ( (gameStart - SDL_GetTicks()) / 1000.f );
    if( avgFPS > 2000000 ) {
      avgFPS = 0;
    }
    // printf("--- slow tick ---\n");
    // printf("input       %f\n", game.inputLength);
    // printf("handleInput %f\n", game.handleInputLength);
    // printf("update      %f\n", game.updateLength);
    // printf("clear       %f\n", game.clearLength);
    // printf("draw        %f\n", game.stateDrawLength);
    // printf("present     %f\n", game.drawLength);
    // printf("FPS %f\n", avgFPS);
    // printf("-----------------\n");
  }
  SDL_Quit();
  printf("exiting game\n");
  return 0;
}
