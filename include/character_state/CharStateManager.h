#ifndef _CharStateManager_h
#define _CharStateManager_h 

class Character;
class CharStateManager final {
public:
  static CharStateManager* getInstance(){
    static CharStateManager instance;
    return &instance;
  };

  void registerCharacter(Character* character, int playerNum);
  bool screenFrozen = false;
  Character* getCharPointer(int playerNum);

private:
  CharStateManager() = default;
  ~CharStateManager() = default;
  CharStateManager(const CharStateManager&) = delete;
  CharStateManager& operator=(const CharStateManager&) = delete;
  CharStateManager(CharStateManager&&) = delete;
  CharStateManager& operator=(CharStateManager&&) = delete;

  Character* characterPointers[2];
};


#endif 
