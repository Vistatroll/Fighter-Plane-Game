#include "main.h"
#include "cone.h"
#include "cuboid.h"

#ifndef JET_H
#define JET_H

class Jet
{
  public:
    Jet() {}
    Jet(float x, float y, float z);
    glm::vec3 position;
    float tilt_angle, rotate_angle, rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float speed_x, float speed_y, float speed_z);
    bool rotate_jet, tilt_jet;
    Cone head, tail;
    Cuboid target1, target2;

  private:
    VAO *object1, *object2, *object3, *object4;
};

#endif
