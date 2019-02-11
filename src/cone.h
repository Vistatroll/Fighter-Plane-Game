#include "main.h"

#ifndef CONE_H
#define CONE_H

class Cone
{
  public:
    Cone() {}
    Cone(float x, float y, float z, float h, float r, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);

  private:
    VAO *object;
};

#endif