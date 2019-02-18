#include "main.h"

#ifndef COMPASS_H
#define COMPASS_H

class Compass
{
public:
  Compass() {}
  Compass(float x, float y);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  void tick();

private:
  VAO *object1, *object2, *object3, *object4;
};

#endif // COMPASS_H
