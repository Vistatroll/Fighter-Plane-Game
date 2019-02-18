#include "compass.h"
#include "main.h"
#define pi 3.14159265358979323846

Compass::Compass(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;

    int n = 100;
    float vertex1[9 * n];
    float theta = (2 * pi) / n;
    float r = 2.1;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex1[i] = r * cos(theta);
        vertex1[i + 1] = r * sin(theta);
        vertex1[i + 2] = 0;
        vertex1[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex1[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex1[i + 5] = 0;
        vertex1[i + 6] = 0;
        vertex1[i + 7] = 0;
        vertex1[i + 8] = 0;
        theta += (2 * pi) / n;
    }
    this->object1 = create3DObject(GL_TRIANGLES, n * 3, vertex1, COLOR_BLACK, GL_FILL);

    const GLfloat vertex4[] = {
        0, 1.9, 0,
        -0.2, 1.7, 0,
        0.2, 1.7, 0,
    };
    this->object4 = create3DObject(GL_TRIANGLES, 3, vertex4, COLOR_WHITE, GL_FILL);

    const GLfloat vertex2[] = {
        -0.4, 0, 0,
        0.4, 0, 0,
        0, 1.6, 0,
    };
    this->object2 = create3DObject(GL_TRIANGLES, 3, vertex2, COLOR_RED, GL_FILL);

    const GLfloat vertex3[] = {
        -0.4, 0, 0,
        0.4, 0, 0,
        0, -1.6, 0,
    };
    this->object3 = create3DObject(GL_TRIANGLES, 3, vertex3, COLOR_LIGHT_GREY, GL_FILL);
}

void Compass::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 scale = glm::scale(glm::vec3(1.5, 1.5, 1.5));
    Matrices.model *= (translate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object4);

    Matrices.model = glm::mat4(1.0f);
    Matrices.model *= (translate * rotate * scale);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
}

void Compass::tick()
{
}
