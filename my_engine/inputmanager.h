#pragma once
#include <unordered_map>

class InputManager
{
    std::unordered_map<unsigned int, bool> m_keyMap;

  public:
    InputManager();
    ~InputManager();

    void pressKey(unsigned int keyID);
    void releasKey(unsigned int keyID);

    bool isKeyPressed(unsigned int keyID);
};
