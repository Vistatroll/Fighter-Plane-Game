#include "main.h"
#include "cuboid.h"
#include "cylinder.h"

#ifndef CHECKPOINT_H
#define CHECKPOINT_H

class Checkpoint
{
public:
  Checkpoint() {}
  Checkpoint(float x, float y, float z, color_t color);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  float w, h, b, y;
  Cuboid cuboid;
  Cylinder cannon_body;
  Cuboid cannon_base;

private:
  VAO *wheel1, *wheel2;
};

#endif