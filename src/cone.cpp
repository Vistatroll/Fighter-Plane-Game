#include "cone.h"
#include "main.h"
#define pi 3.14159265358979323846

Cone::Cone(float x, float y, float z, float h, float r, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;

    int n = 100;
    float vertex_buffer_data[9 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 9 * n; i += 9)
    {
        vertex_buffer_data[i] = r * cos(theta);
        vertex_buffer_data[i + 1] = r * sin(theta);
        vertex_buffer_data[i + 2] = 0.0f;
        vertex_buffer_data[i + 3] = r * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 4] = r * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 5] = 0.0f;
        vertex_buffer_data[i + 6] = 0.0f;
        vertex_buffer_data[i + 7] = 0.0f;
        vertex_buffer_data[i + 8] = -h;

        theta += (2 * pi) / n;
    }

    this->object = create3DObject(GL_TRIANGLES, n * 3, vertex_buffer_data, color, GL_FILL);
}

void Cone::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); 
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}