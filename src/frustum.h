#include "main.h"

#ifndef FRUSTUM_H
#define FRUSTUM_H

class Frustum
{
public:
  Frustum() {}
  Frustum(float x, float y, float z, float h, float r1, float r2, color_t color);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  void tick();

private:
  VAO *object;
};

#endif // FRUSTUM_H
