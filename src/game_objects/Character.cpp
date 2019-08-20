#include "game_objects/Character.h"

Character::Character(std::pair<int, int> _position, int _health):position(_position), health(_health) { 
  init();
}

Character::Character(std::pair<int, int> _position):position(_position){ 
  health = 100;
  init();
}

void Character::init(){
  currentSprite = new GameTexture();
  currentSprite->cartesian = true;
  currentSprite->loadTexture("../data/characters/alucard/sprites/1.png");
  currentSprite->setDimensions(position.first, position.second, 120, 120);
}

Character::~Character(){};

void Character::update(){};
void Character::draw(){
  currentSprite->render();
};

