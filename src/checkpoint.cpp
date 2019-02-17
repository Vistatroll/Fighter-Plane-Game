#include "checkpoint.h"
#include "main.h"
#define pi 3.14159265358979323846

Checkpoint::Checkpoint(float x, float y, float z, color_t color)
{
    this->w = 6;
    this->h = 4;
    this->b = 6;

    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->exist = true;

    cuboid = Cuboid(0, 0, 0, this->h, this->w, this->b, color);

    cannon_body = Cylinder(0, 4, 0, 3, 0.5, 20, COLOR_LIGHT_GREY);
    cannon_base = Cuboid(0, 2.5, 0.8, 1, 1, 1, COLOR_DARK_GREY);

    int n = 20;
    float r = 1;
    float vertex1[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex1[i] = 0.5;
        vertex1[i + 1] = 0.5 + r * cos(theta);
        vertex1[i + 2] = r * sin(theta);
        vertex1[i + 3] = 0.5;
        vertex1[i + 4] = 0.5 + r * cos(theta + (2 * pi) / n);
        vertex1[i + 5] = r * sin(theta + (2 * pi) / n);
        vertex1[i + 6] = 0.5;
        vertex1[i + 7] = 0.5;
        vertex1[i + 8] = 0;
        theta += (2 * pi) / n;
    }
    this->wheel1 = create3DObject(GL_TRIANGLES, n * 3, vertex1, COLOR_BLACK, GL_LINE);

    float vertex2[9 * n];
    theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex2[i] = -0.5;
        vertex2[i + 1] = 0.5 + r * cos(theta);
        vertex2[i + 2] = r * sin(theta);
        vertex2[i + 3] = -0.5;
        vertex2[i + 4] = 0.5 + r * cos(theta + (2 * pi) / n);
        vertex2[i + 5] = r * sin(theta + (2 * pi) / n);
        vertex2[i + 6] = -0.5;
        vertex2[i + 7] = 0.5;
        vertex2[i + 8] = 0;
        theta += (2 * pi) / n;
    }
    this->wheel2 = create3DObject(GL_TRIANGLES, n * 3, vertex2, COLOR_BLACK, GL_LINE);
}

void Checkpoint::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    this->cuboid.draw(MVP);
    if (this->exist)
    {
        this->cannon_body.draw(MVP);
        this->cannon_base.draw(MVP);
        draw3DObject(this->wheel1);
        draw3DObject(this->wheel2);
    }
}