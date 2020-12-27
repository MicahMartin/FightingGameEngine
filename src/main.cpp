#include <SDL.h>
#include <thread>
#include <chrono>
#include "util/Util.h"
#include "game.h"
#include "ggponet.h"

void ggpoUpdate(Game* game){
  GameState* currentState = game->stateManager->getState();
  if(std::strcmp(currentState->stateName, "FIGHT_STATE") == 0){
    FightState* fightState = (FightState*)currentState;
    if (fightState->netPlayState) {
      ggpo_idle(fightState->ggpo, 1);
    }
  }
}

int main(int argc, char* args[]) {


  // Game.init
  Game game;
  printf("checking args defaults\n");
  if (argc >= 2) {
    game.stateManager->getInstance()->setPnum(std::stoi(args[1]));
    if (argc >= 3) {
      if (std::stoi(args[2]) + std::stoi(args[3]) == 0) {
        game.graphics->resizeWindow(true);
      } else {
        game.graphics->resizeWindow(std::stoi(args[2]), std::stoi(args[3]));
      }
    }
    if (argc >= 4) {
      game.stateManager->getInstance()->setCharName(1, args[4]);
      game.stateManager->getInstance()->setCharName(2, args[5]);
    }

    if (argc >= 6) {
      game.stateManager->getInstance()->setIP(args[6]);
      game.stateManager->getInstance()->setPort(std::stoi(args[7]));
    }
  } else {
    // defaults
    printf("calling defaults\n");
    game.stateManager->getInstance()->setPnum(1);
    game.graphics->resizeWindow(true);
    game.stateManager->getInstance()->setCharName(1, "samurai");
    game.stateManager->getInstance()->setCharName(2, "samurai");
    game.stateManager->getInstance()->setIP("127.0.0.1");
    game.stateManager->getInstance()->setPort(7000);
  }

  //mainloop
  {
    using namespace std::chrono;
    auto nextFrame = system_clock::now() + FPS{1};
    auto prevSec = time_point_cast<seconds>(system_clock::now());
    int fpsCounter = 0;

    while(game.running) {
      game.update();
      ggpoUpdate(&game);
      game.draw();

      ++fpsCounter;
      auto currentSec = time_point_cast<seconds>(system_clock::now());
      if (currentSec > prevSec) {
        game.graphics->setFPS(fpsCounter);
        fpsCounter = 0;
        prevSec = currentSec;
      }
      while(nextFrame > system_clock::now())
      ;
      nextFrame = system_clock::now() + FPS{1};
    }
  }

  printf("exiting game\n");
  SDL_Quit();
  return 0;
}
