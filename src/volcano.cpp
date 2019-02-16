#include "volcano.h"
#include "main.h"
#define pi 3.14159265358979323846

Volcano::Volcano(float x, float y, float z, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;

    this->bottom = Frustum(x, y, z, 2, 2, 3, COLOR_VOLCANO);
    this->top = Frustum(x, y + 2.5, z, 3, 1, 2, COLOR_VOLCANO);

    // int n = 100;
    // float vertex1[9 * n];
    // float theta = (2 * pi) / n;
    // for (int i = 0; i < 9 * n; i += 9)
    // {
    //     vertex1[i] = 3 * cos(theta);
    //     vertex1[i + 1] = 0;
    //     vertex1[i + 2] = 3 * sin(theta);
    //     vertex1[i + 3] = 3 * cos(theta + (2 * pi) / n);
    //     vertex1[i + 4] = 0;
    //     vertex1[i + 5] = 3 * sin(theta + (2 * pi) / n);
    //     vertex1[i + 6] = 0;
    //     vertex1[i + 7] = 0;
    //     vertex1[i + 8] = 0;
    //     theta += (2 * pi) / n;
    // }
    // this->object = create3DObject(GL_TRIANGLES, n * 3, vertex1, COLOR_RED, GL_FILL);
}

void Volcano::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 scale = glm::scale(glm::vec3(3, 3, 3));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // draw3DObject(this->object);
    this->top.draw(MVP);
    this->bottom.draw(MVP);
}