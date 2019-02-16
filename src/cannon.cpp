#include "cannon.h"
#include "main.h"
#define pi 3.14159265358979323846

Cannon::Cannon(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;

    cannon_body = Cylinder(x, y + 2, z, 3, 0.5, 20, COLOR_LIGHT_GREY);
    cannon_base = Cuboid(x, y + 0.6, z + 0.8, 1, 1, 1, COLOR_DARK_GREY);

    int n = 20;
    float r = 1;
    float vertex1[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex1[i] = x + 0.5;
        vertex1[i + 1] = y - 1.5 + r * cos(theta);
        vertex1[i + 2] = r * sin(theta);
        vertex1[i + 3] = x + 0.5;
        vertex1[i + 4] = y - 1.5 + r * cos(theta + (2 * pi) / n);
        vertex1[i + 5] = r * sin(theta + (2 * pi) / n);
        vertex1[i + 6] = x + 0.5;
        vertex1[i + 7] = y - 1.5;
        vertex1[i + 8] = 0;
        theta += (2 * pi) / n;
    }
    this->wheel1 = create3DObject(GL_TRIANGLES, n * 3, vertex1, COLOR_BLACK, GL_LINE);

    float vertex2[9 * n];
    theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex2[i] = x - 0.5;
        vertex2[i + 1] = y - 1.5 + r * cos(theta);
        vertex2[i + 2] = r * sin(theta);
        vertex2[i + 3] = x - 0.5;
        vertex2[i + 4] = y - 1.5 + r * cos(theta + (2 * pi) / n);
        vertex2[i + 5] = r * sin(theta + (2 * pi) / n);
        vertex2[i + 6] = x - 0.5;
        vertex2[i + 7] = y - 1.5;
        vertex2[i + 8] = 0;
        theta += (2 * pi) / n;
    }
    this->wheel2 = create3DObject(GL_TRIANGLES, n * 3, vertex2, COLOR_BLACK, GL_LINE);
}

void Cannon::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    this->cannon_body.draw(MVP);
    this->cannon_base.draw(MVP);
    draw3DObject(this->wheel1);
    draw3DObject(this->wheel2);
}