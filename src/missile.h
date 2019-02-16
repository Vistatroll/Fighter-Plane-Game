#include "main.h"
#include "cylinder.h"
#include "cone.h"
#include "frustum.h"

#ifndef MISSILE_H
#define MISSILE_H

class Missile
{
public:
  Missile() {}
  Missile(float x, float y, float z, float l, float r, color_t color_body, color_t color_head);
  glm::vec3 position;
  float x_rotation, y_rotation, z_rotation;
  bool move_up;
  bool exist;
  void draw(glm::mat4 VP);
  void tick(float speed_x, float speed_y, float speed_z);
  Cylinder body;
  Cone head;
  Frustum tail;

private:
  VAO *object;
};

#endif // MISSILE_H
