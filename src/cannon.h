#include "main.h"
#include "cuboid.h"
#include "cylinder.h"

#ifndef CANNON_H
#define CANNON_H

class Cannon
{
public:
  Cannon() {}
  Cannon(float x, float y, float z);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  Cylinder cannon_body;
  Cuboid cannon_base;

private:
  VAO *wheel1, *wheel2;
};

#endif