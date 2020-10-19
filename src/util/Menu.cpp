#include "util/Menu.h"

Menu::Menu() {
  menuXPos = 300;
  menuYPos = 100;
  menuWidth = 600;
  menuHeight = 600;
  
  menuRect.x = menuXPos;
  menuRect.y = menuYPos;
  menuRect.w = menuWidth;
  menuRect.h = menuHeight;

  borderRect.x = menuXPos - 10;
  borderRect.y = menuYPos - 10;
  borderRect.w = menuWidth + 20;
  borderRect.h = menuHeight + 20;

  borderColor = {255, 0, 0, 0};
  bodyColor = {0, 0, 0, 0};
  menuCursor.cursorTexture.loadTexture("../data/images/cursor.png");
  menuCursor.cursorTexture.setDimensions(0, 0, 25, 25);
  menuItemArray.reserve(8);

  menuMove = Mix_LoadWAV("../data/audio/menumove.wav");
  menuSelect = Mix_LoadWAV("../data/audio/menuselect.wav");
}

Menu::~Menu() {}

Menu::Menu(int width, int height, SDL_Color borderColor, SDL_Color bodyColor, const char* cursorTexturePath) : 
menuWidth(width), menuHeight(height), borderColor(borderColor), bodyColor(bodyColor) {
  menuCursor.cursorTexture.loadTexture(cursorTexturePath);
  menuCursor.cursorTexture.setDimensions(0, 0, 25, 25);
}

void Menu::handleInput() {
  if(vc->wasPressed(UP)){
    Mix_PlayChannel(0, menuMove, 0);
    moveCursorUp();
  }
  if(vc->wasPressed(DOWN)){
    Mix_PlayChannel(0, menuMove, 0);
    moveCursorDown();
  }
  if(vc->wasReleased(LP)){
    Mix_PlayChannel(0, menuSelect, 0);
    activateMenuItem();
  }
  for (auto i : menuItemArray) {
    i.update();
  }
}

void Menu::addMenuItem(const char* title, const char* message, int textWidth, int textHeight, std::function<void()> callBack, std::function<void()> update) {
  menuItemArray.emplace_back(title, message, textWidth, textHeight, callBack, update);

  int newMenuItemSize = menuItemArray.size();
  int currentCellStart = 0;
  int cellHeight = menuHeight / newMenuItemSize;
  int halfCellHeight = cellHeight / 2;

  for (auto &i : menuItemArray) {
    // TODO: Margin
    i.itemTexture.setCords(menuXPos + 100, menuYPos + currentCellStart + 50);
    currentCellStart = currentCellStart + cellHeight;
  }

  MenuItem* firstItem = &menuItemArray[0];
  std::pair menuItemCoords = firstItem->itemTexture.getCords();
  menuCursor.cursorTexture.setCords(menuItemCoords.first - (menuCursor.cursorTexture.getDimensions().first + 50), menuItemCoords.second);
}

void Menu::activateMenuItem() {
  menuItemArray.at(menuCursor.position).onActivate();
}

void Menu::draw() {
  SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
  SDL_RenderFillRect(renderer, &borderRect);
  SDL_SetRenderDrawColor(renderer, bodyColor.r, bodyColor.g, bodyColor.b, bodyColor.a);
  SDL_RenderFillRect(renderer, &menuRect);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  for (auto i : menuItemArray) {
    i.draw();
  }
  for (auto& i : displayElems) {
    DisplayElem& elem = i.second;
    if (elem.display) {
      elem.draw();
    }
  }

  menuCursor.cursorTexture.render();
}

void Menu::moveCursorDown() {
  if(menuItemArray.size() > 0){
    if(menuCursor.position == menuItemArray.size() - 1) {
      menuCursor.position = 0;
    } else {
      menuCursor.position++;
    }

    MenuItem* menuItemPtr = &menuItemArray[menuCursor.position];
    std::pair menuItemCoords = menuItemPtr->itemTexture.getCords();
    menuCursor.cursorTexture.setCords(menuItemCoords.first - (menuCursor.cursorTexture.getDimensions().first + 50), menuItemCoords.second);
  }
}

void Menu::moveCursorUp() {
  if(menuItemArray.size() > 0){
    if(menuCursor.position == 0) {
      menuCursor.position = menuItemArray.size() - 1;
    } else {
      menuCursor.position--;
    }

    MenuItem* menuItemPtr = &menuItemArray[menuCursor.position];
    std::pair menuItemCoords = menuItemPtr->itemTexture.getCords();
    menuCursor.cursorTexture.setCords(menuItemCoords.first - (menuCursor.cursorTexture.getDimensions().first + 50), menuItemCoords.second );
  }
}

void Menu::addDisplayElem(const char* title, const char* texturePath, int textWidth, int textHeight, int positionX, int positionY, bool display){
  displayElems.emplace(title, DisplayElem{title, texturePath, textWidth, textHeight, positionX, positionY, display});
}

DisplayElem* Menu::getDisplayElem(const char* title){
  return &displayElems[std::string(title)];
}
