#pragma once
#include <string>
#include <vector>

class IOManager
{
  public:
    static bool readFiletoBuffer(std::string filePath, std::vector<unsigned char>& buffer);
};
