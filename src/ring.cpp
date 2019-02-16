#include "ring.h"
#include "main.h"
#define pi 3.14159265358979323846

Ring::Ring(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 90;
    ring = Frustum(x, y, z, 1, 6, 6.2, COLOR_RING);
}

void Ring::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    this->ring.draw(MVP);
}