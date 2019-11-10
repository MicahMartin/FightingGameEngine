#include <SDL2/SDL.h>
#include "Game.h"

int main() {
  static double FPS = 1000/60;

  // Game.init
  Game game;

  while(game.stillRunning()){
    double frameStart = SDL_GetTicks();
    game.update();
    // we want to run the update stuff once every frame (16~ MS)
    // so we check how many MS its taken us to generate the current frame 
    // so if it took 3MS to render this frame, we have 13MS of free time before reading input and rendering again
    double frameEnd = SDL_GetTicks();
    double delayLength = FPS - (frameEnd - frameStart);
    if(int(delayLength) > 0){
      SDL_Delay(int(delayLength));
    } else  {
      printf("--- slow tick ---\n");
      printf("frameStart  %f\n", frameStart);
      printf("frameEnd %f\n", frameEnd);
      printf("input       %f\n", game.inputLength);
      printf("handleInput %f\n", game.handleInputLength);
      printf("update      %f\n", game.updateLength);
      printf("draw        %f\n", game.drawLength);
      printf("-----------------\n");
    }
  }
  return 0;
}
