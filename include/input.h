#ifndef _input_h
#define _input_h

#include <SDL2/SDL.h>
#include <boost/functional/hash.hpp>


class Input{
public:
  Input();
  Input(uint32_t eventKey, uint32_t eventValue) : eventKey(eventKey), eventValue(eventValue) {};
  ~Input();

  uint32_t getKey() const{
    return eventKey;
  };
  uint32_t getValue() const{
    return eventValue;
  };

  // magic code to make this type hashable.
  bool operator==(Input const& otherInput) const {

    return otherInput.eventKey == eventKey && otherInput.eventValue == eventValue;
  }

private:
  const char* keyName;
  uint32_t eventKey;
  uint32_t eventValue;

};
namespace std {
  template<>
  struct hash<Input>{
    std::size_t operator()(const Input& input) const {
      // left shift eventKey onto 64 bit val ( eventKey << 32 ), then left shift eventValue to the start ( eventValue << 0 )
      uint64_t inputHash = 0;
      inputHash |= input.getKey() << 31;
      inputHash |= input.getValue() << 0;

      return boost::hash_value(inputHash);
    }
  };



}

#endif /* ifndef _input_h */
