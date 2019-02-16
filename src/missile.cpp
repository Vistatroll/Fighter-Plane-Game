#include "missile.h"
#include "main.h"
#define pi 3.14159265358979323846

Missile::Missile(float x, float y, float z, float h, float r, color_t color_body, color_t color_head)
{
    this->position = glm::vec3(x, y, z);
    this->x_rotation = 0;
    this->y_rotation = 0;
    this->z_rotation = 0;
    this->exist = true;
    this->move_up = false;
    this->body = Cylinder(0, 0, 0, h, r - 0.1, 90, color_body);
    this->head = Cone(0, h / 2, 0, 2 * r, r, color_head);
    this->head.rotation = 90;
    this->tail = Frustum(0, -h / 2 - r, 0, 1.2 * r, r - 0.1, 1.2 * r, COLOR_BLACK);
}

void Missile::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate1 = glm::rotate((float)(this->x_rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 rotate2 = glm::rotate((float)(this->y_rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotate3 = glm::rotate((float)(this->z_rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 scale = glm::scale(glm::vec3(0.5, 0.5, 0.5));
    Matrices.model *= (translate * rotate3 * rotate2 * rotate1 * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    this->body.draw(MVP);
    this->head.draw(MVP);
    this->tail.draw(MVP);
}

void Missile::tick(float speed_x, float speed_y, float speed_z)
{
    this->position.x += speed_x;
    this->position.y += speed_y;
    this->position.z += speed_z;
}
