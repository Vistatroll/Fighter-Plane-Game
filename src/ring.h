#include "main.h"
#include "frustum.h"

#ifndef RING_H
#define RING_H

class Ring
{
public:
  Ring() {}
  Ring(float x, float y, float z);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  Frustum ring;

private:
  VAO *object;
};

#endif