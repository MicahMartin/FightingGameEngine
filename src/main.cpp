#include <SDL2/SDL.h>
#include <chrono>
#include "game.h"

typedef std::chrono::high_resolution_clock Clock;
int main() {
  static double FPS = 1000/60;

  // Game.init
  Game game;

  double gameStart = SDL_GetTicks();
  int frameCount = 0;
  while(game.running){
    auto frameStart = Clock::now();

    game.update();
    // we want to run the update stuff once every frame (16~ MS)
    // so we check how many MS its taken us to generate the current frame 
    // so if it took 3MS to render this frame, we have 13MS of free time before reading input and rendering again
    auto frameEnd = Clock::now();
    double delayLength = FPS - (std::chrono::duration<double, std::ratio<1000>>(frameEnd - frameStart).count());
    if(delayLength > 0){
      SDL_Delay(delayLength);
    } else  { 
      if(game.inFightState){
        game.running = false;
      }
    }
    frameCount++;
    float avgFPS = frameCount/ ( (gameStart - SDL_GetTicks()) / 1000.f );
    if( avgFPS > 2000000 )
    {
      avgFPS = 0;
    }
    printf("--- slow tick ---\n");
    printf("frameStart  %lld\n", frameStart);
    printf("frameEnd    %lld\n", frameEnd);
    printf("input       %f\n", game.inputLength);
    printf("handleInput %f\n", game.handleInputLength);
    printf("update      %f\n", game.updateLength);
    printf("clear       %f\n", game.clearLength);
    printf("draw        %f\n", game.stateDrawLength);
    printf("present     %f\n", game.drawLength);
    printf("FPS %f\n", avgFPS);
    printf("-----------------\n");
  }
  return 0;
}
