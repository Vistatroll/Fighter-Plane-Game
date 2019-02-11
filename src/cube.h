#include "main.h"

#ifndef CUBE_H
#define CUBE_H

class Cube
{
  public:
    Cube() {}
    Cube(float x, float y, float z, float w, float h, float b, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);

  private:
    VAO *object;
};

#endif