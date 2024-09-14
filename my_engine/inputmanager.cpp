#include "inputmanager.h"

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::pressKey(unsigned int keyID)
{
    m_keyMap[keyID] = true;
}

void InputManager::releasKey(unsigned int keyID)
{
    m_keyMap[keyID] = false;
}

bool InputManager::isKeyPressed(unsigned int keyID)
{
    // check manually if keyID was inside of map
    // and if it's not inside of map, return false
    auto it = m_keyMap.find(keyID);

    if (it != m_keyMap.end())
        return it->second;
    else
        return false;
}
