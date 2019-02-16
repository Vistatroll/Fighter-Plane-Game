#include "main.h"

#ifndef CYLINDER_H
#define CYLINDER_H

class Cylinder
{
public:
  Cylinder() {}
  Cylinder(float x, float y, float z, float h, float r, float rotation, color_t color);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  void tick();

private:
  VAO *object1, *object2, *object3;
};

#endif // CYLINDER_H
