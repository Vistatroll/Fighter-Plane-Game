#include "main.h"
#include "frustum.h"

#ifndef VOLCANO_H
#define VOLCANO_H

class Volcano
{
public:
  Volcano() {}
  Volcano(float x, float y, float z, color_t color);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP);
  void tick();
  Frustum top, bottom;

private:
  VAO *object;
};

#endif // VOLCANO_H
