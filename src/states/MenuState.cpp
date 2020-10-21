#include <bitset>
#include "states/MenuState.h"
#include "states/FightState.h"
#include "states/NetConnectState.h"
#include "input/InputManager.h"

MenuState::MenuState() {
  printf("menuState constructor\n");
  // TODO: let the menu add its own texture
  mainMenu.addMenuItem("versus", "versus", 240, 50, [this]{
    printf("pushing the fightState \n");
    stateManager->pushState(new FightState());
    printf("changed the state to the fightState \n");
  }, [this]{});

  mainMenu.addMenuItem("netplay", "netplay", 240, 50, [this]{
    stateManager->pushState(new NetConnectState());
  }, [this]{});

  mainMenu.addMenuItem("config", "config", 240, 50, [this]{
    activeMenu = &configMenu;
  }, [this]{});

  configMenu.addDisplayElem("cursor", "../data/images/cursor.png", 25, 25, 900, 100, false);
  configMenu.addDisplayElem("button_select", "../data/images/buttonSelect.png", 200, 400, 900, 100, false);

  configMenu.addMenuItem("player1_button_config", "config", 240, 50, [this]{
    // refactor u vagrant
    inputManager->userBeingConfig = 1;
    inputManager->keySelectionMode = true;
    DisplayElem* buttonSelect = configMenu.getDisplayElem("button_select");
    DisplayElem* cursor = configMenu.getDisplayElem("cursor");
    buttonSelect->display = true;
    cursor->display = true;
    }, [this]{
    DisplayElem* buttonSelect = configMenu.getDisplayElem("button_select");
    DisplayElem* cursor = configMenu.getDisplayElem("cursor");
    if (!inputManager->keySelectionMode) {
      buttonSelect->display = false;
      cursor->display = false;
      } else {
        switch (inputManager->configCounter) {
          case 0:
          printf("case 0\n");
          cursor->elemTexture.setCords(900, 100);
            break;
          case 1:
          printf("case 1\n");
          cursor->elemTexture.setCords(900, 200);
            break;
          case 2:
          printf("case 2\n");
          cursor->elemTexture.setCords(900, 300);
            break;
          case 3:
          printf("case 3\n");
          cursor->elemTexture.setCords(900, 400);
            break;
          default:
            break;
        }

      }
    });

  configMenu.addMenuItem("player2_button_config", "config", 240, 50, [this]{
    inputManager->userBeingConfig = 2;
    inputManager->keySelectionMode = true;
    DisplayElem* buttonSelect = configMenu.getDisplayElem("button_select");
    DisplayElem* cursor = configMenu.getDisplayElem("cursor");
    buttonSelect->display = true;
    cursor->display = true;
    }, [this]{
    DisplayElem* buttonSelect = configMenu.getDisplayElem("button_select");
    DisplayElem* cursor = configMenu.getDisplayElem("cursor");
    if (!inputManager->keySelectionMode) {
      buttonSelect->display = false;
      cursor->display = false;
      } else {
        switch (inputManager->configCounter) {
          case 0:
          printf("case 0\n");
          cursor->elemTexture.setCords(900, 100);
            break;
          case 1:
          printf("case 1\n");
          cursor->elemTexture.setCords(900, 200);
            break;
          case 2:
          printf("case 2\n");
          cursor->elemTexture.setCords(900, 300);
            break;
          case 3:
          printf("case 3\n");
          cursor->elemTexture.setCords(900, 400);
            break;
          default:
            break;
        }

      }
    });

  mainMenu.vc = inputManager->getVirtualController(0);
  configMenu.vc = inputManager->getVirtualController(0);

  activeMenu = &mainMenu;
}

MenuState::~MenuState() {
  printf("menuState destructor\n");
}

void MenuState::enter() {
  printf("entered the menu state\n");
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
  Mix_PauseMusic();
  delete this;
}

void MenuState::pause() {
  printf("pausing the menu state\n");
}

void MenuState::resume() { 
  printf("resuming the menu state! \n");
}

void MenuState::handleInput() {
  VirtualController* vc = inputManager->getVirtualController(0);
  if (vc->wasPressed(MK)) {
    stateManager->popState();
    return;
  }
  activeMenu->handleInput();
}

void MenuState::update() {
  menuScreen.update();
}

void MenuState::draw() {
  menuScreen.draw();
  activeMenu->draw();
}
