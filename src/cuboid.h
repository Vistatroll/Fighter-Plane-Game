#include "main.h"

#ifndef CUBOID_H
#define CUBOID_H

class Cuboid
{
public:
  Cuboid() {}
  Cuboid(float x, float y, float z, float h, float w, float b, color_t color);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  void tick();

private:
  VAO *object;
};

#endif // CUBOID_H
