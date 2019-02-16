#include "bomb.h"
#include "main.h"
#define pi 3.14159265358979323846

Bomb::Bomb(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->exist = true;

    up = Frustum(0, 0.7, 0, 1, 0.5, 1, COLOR_BOMB);
    down = Frustum(0, -0.7, 0, 1, 1, 0.5, COLOR_BOMB);
    middle = Frustum(0, 0, 0, 0.4, 1, 1, COLOR_DARK_GREY);
}

void Bomb::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    up.draw(MVP);
    down.draw(MVP);
    middle.draw(MVP);
}