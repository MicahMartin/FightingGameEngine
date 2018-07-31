#ifndef _input_h
#define _input_h
#include <SDL2/SDL.h>
#include <boost/functional/hash.hpp>


class Input{
public:
  Input();
  Input(uint32_t eventKey, uint16_t eventValue) : eventKey(eventKey), eventValue(eventValue) {};

  ~Input();
  uint32_t  getKeyCode();

  // magic code to make this type hashable.
  bool operator==(Input const& otherInput) const {

    return otherInput.eventKey == eventKey && otherInput.eventValue == eventValue;
  }

  struct InputHasher{
    std::size_t operator()(const Input& input) const {

      std::size_t seed = 0;

      boost::hash_combine(seed, boost::hash_value(input.eventKey));
      boost::hash_combine(seed, boost::hash_value(input.eventValue));

      return seed;

    }
  };


private:
  const char* keyName;
  uint32_t eventKey;
  uint16_t eventValue;

};

#endif /* ifndef _input_h */
