#include "main.h"
#include "parachute.h"

Parachute::Parachute(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 90;
    // speed = 0.01;
    exist = 1;

    seat = Cuboid(0, 0, 0, 1, 1, 1, COLOR_CORAL);
    balloon1 = Frustum(0, 3, 0, 2, 3, 2, COLOR_BLUEVIOLET);
    balloon2 = Frustum(0, 3, 0, 2, 2, 3, COLOR_BLUEVIOLET);
    holders[0] = Cuboid(0.5, 2, 0.5, 3.1, 0.1, 0.1, COLOR_LIGHT_GREY);
    holders[0].rotation = 10;
    holders[1] = Cuboid(-0.5, 2, -0.5, 3.1, 0.1, 0.1, COLOR_LIGHT_GREY);
    holders[1].rotation = -10;
    holders[2] = Cuboid(0.5, 2, -0.5, 3.1, 0.1, 0.1, COLOR_LIGHT_GREY);
    holders[2].rotation = -10;
    holders[3] = Cuboid(-0.5, 2, 0.5, 3.1, 0.1, 0.1, COLOR_LIGHT_GREY);
    holders[3].rotation = 10;
}

void Parachute::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    seat.draw(MVP);
    balloon1.draw(MVP);
    balloon2.draw(MVP);
    holders[0].draw(MVP);
    holders[1].draw(MVP);
    holders[2].draw(MVP);
    holders[3].draw(MVP);
}

void Parachute::tick()
{
    this->position.y -= 0.01;
}
