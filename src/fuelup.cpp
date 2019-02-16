#include "fuelup.h"
#include "main.h"
#define pi 3.14159265358979323846

Fuelup::Fuelup(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->exist = true;
    body = Cuboid(0, 0, 0, 3, 3, 2, COLOR_DARK_GREEN);
    handle = Cuboid(1.8, 0, 0, 2, 0.6, 0.2, COLOR_GREEN);
    cap = Cylinder(0, 1.75, 0, 1, 0.5, 90, COLOR_GREEN);
}

void Fuelup::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation++ * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    this->body.draw(MVP);
    this->cap.draw(MVP);
    this->handle.draw(MVP);
}