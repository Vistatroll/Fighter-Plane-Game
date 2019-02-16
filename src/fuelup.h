#include "main.h"
#include "cuboid.h"
#include "cylinder.h"

#ifndef FUELUP_H
#define FUELUP_H

class Fuelup
{
public:
  Fuelup() {}
  Fuelup(float x, float y, float z);
  glm::vec3 position;
  float rotation;
  bool exist;
  void draw(glm::mat4 VP);
  Cuboid body, handle;
  Cylinder cap;

private:
  VAO *object;
};

#endif