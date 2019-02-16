#include "sphere.h"
#include "main.h"

Sphere::Sphere(float x, float y, float z, float r, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->radius = radius;
    // GLUquadric *quad;
    // gluSphere(quad, 10, 20, 20);
}

void Sphere::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    GLUquadric *quad;
    gluSphere(quad, this->radius, 20, 20);
}

void Sphere::tick()
{
}
