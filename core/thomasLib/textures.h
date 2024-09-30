#ifndef TEXTURE_H
#define TEXTURE_H
#include <ew/external/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Texture2D {
public:
    Texture2D(const char* filePath, int filterMode, int wrapMode);
    ~Texture2D();
    void Bind(unsigned int slot = 0); //Bind to a specific texture unit
private:
    unsigned int m_id; //GL texture handle
    int m_width, m_height;
};
#endif 