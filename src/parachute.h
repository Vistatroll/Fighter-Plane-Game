#include "main.h"
#include "cuboid.h"
#include "frustum.h"

#ifndef PARACHUTE_H
#define PARACHUTE_H

class Parachute
{
public:
  Parachute() {}
  Parachute(float x, float y, float z);
  glm::vec3 position;
  float rotation;
  bool exist;
  void draw(glm::mat4 VP);
  Cuboid seat;
  Frustum balloon1, balloon2;
  Cuboid holders[4]; 

private:
  VAO *object;
};

#endif