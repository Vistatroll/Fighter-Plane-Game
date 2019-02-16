#include "cylinder.h"
#include "main.h"
#define pi 3.14159265358979323846

Cylinder::Cylinder(float x, float y, float z, float h, float r, float rotation, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = rotation;

    int n = 100;
    float vertex1[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex1[i] = r * cos(theta);
        vertex1[i + 1] = r * sin(theta);
        vertex1[i + 2] = -h / 2;
        vertex1[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex1[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex1[i + 5] = -h / 2;
        vertex1[i + 6] = 0.0f;
        vertex1[i + 7] = 0.0f;
        vertex1[i + 8] = -h / 2;
        theta += (2 * pi) / n;
    }
    this->object1 = create3DObject(GL_TRIANGLES, n * 3, vertex1, color, GL_FILL);

    n = 100;
    float vertex2[9 * n];
    theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex2[i] = r * cos(theta);
        vertex2[i + 1] = r * sin(theta);
        vertex2[i + 2] = h / 2;
        vertex2[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex2[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex2[i + 5] = h / 2;
        vertex2[i + 6] = 0.0f;
        vertex2[i + 7] = 0.0f;
        vertex2[i + 8] = h / 2;
        theta += (2 * pi) / n;
    }
    this->object2 = create3DObject(GL_TRIANGLES, n * 3, vertex2, color, GL_FILL);

    n = 20;
    float vertex3[18 * n];
    theta = (2 * pi) / n;
    for (int i = 0; i < 18 * n; i += 18)
    {
        vertex3[i] = r * cos(theta);
        vertex3[i + 1] = r * sin(theta);
        vertex3[i + 2] = h / 2;
        vertex3[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex3[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex3[i + 5] = h / 2;
        vertex3[i + 6] = r * cos(theta);
        vertex3[i + 7] = r * sin(theta);
        vertex3[i + 8] = -h / 2;
        vertex3[i + 9] = r * cos(theta + (2 * pi) / n);
        vertex3[i + 10] = r * sin(theta + (2 * pi) / n);
        vertex3[i + 11] = -h / 2;
        vertex3[i + 12] = r * cos(theta + (2 * pi) / n);
        vertex3[i + 13] = r * sin(theta + (2 * pi) / n);
        vertex3[i + 14] = h / 2;
        vertex3[i + 15] = r * cos(theta);
        vertex3[i + 16] = r * sin(theta);
        vertex3[i + 17] = -h / 2;
        theta += (2 * pi) / n;
    }
    this->object3 = create3DObject(GL_TRIANGLES, n * 3 * 2, vertex3, color, GL_FILL);
}

void Cylinder::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
}

void Cylinder::tick()
{
}
