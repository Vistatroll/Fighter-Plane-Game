#include "main.h"

#ifndef SPHERE_H
#define SPHERE_H

class Sphere
{
public:
  Sphere() {}
  Sphere(float x, float y, float z, float r, color_t color);
  glm::vec3 position;
  float rotation;
  float radius;
  void draw(glm::mat4 VP);
  void tick();

private:
  VAO *object;
};

#endif // SPHERE_H
