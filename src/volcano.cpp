#include "volcano.h"
#include "main.h"
#define pi 3.14159265358979323846

Volcano::Volcano(float x, float y, float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;

    this->bottom = Frustum(0, 0, 0, 6, 6, 9, COLOR_VOLCANO);
    this->top = Frustum(0, 7.5, 0, 9, 3, 6, COLOR_VOLCANO);

    int n = 50;
    float vertex1[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex1[i] = 7 * cos(theta);
        vertex1[i + 1] = 0;
        vertex1[i + 2] = 7 * sin(theta);
        vertex1[i + 3] = 7 * cos(theta + (2 * pi) / n);
        vertex1[i + 4] = 0;
        vertex1[i + 5] = 7 * sin(theta + (2 * pi) / n);
        vertex1[i + 6] = 0;
        vertex1[i + 7] = 0;
        vertex1[i + 8] = 0;
        theta += (2 * pi) / n;
    }
    this->object = create3DObject(GL_TRIANGLES, n * 3, vertex1, COLOR_FIRE, GL_FILL);
}

void Volcano::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    this->top.draw(MVP);
    this->bottom.draw(MVP);
}