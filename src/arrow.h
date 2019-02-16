#include "main.h"

#ifndef ARROW_H
#define ARROW_H

class Arrow
{
public:
  Arrow() {}
  Arrow(float x, float y, float z);
  glm::vec3 position;
  float rotation;
  void draw(glm::mat4 VP, float scale_factor);

private:
  VAO *object;
};

#endif