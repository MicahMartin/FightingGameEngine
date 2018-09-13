#include "states/StateCollection.h"
#include "states/OpeningState.h"
#include "states/MenuState.h"

StateCollection::StateCollection(){ 
  pushState(new MenuState(this));
  pushState(new OpeningState(this));
};

StateCollection::~StateCollection() { };
