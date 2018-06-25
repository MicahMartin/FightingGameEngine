#include "game.h"

Game::Game(){}
Game::~Game(){}

void Game::init(){

  coreGraphics.init(640,480);
  inputManager.init();

  printf("Successful intialization\n");
}
void Game::run(){

}
