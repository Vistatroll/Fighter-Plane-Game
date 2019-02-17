#include "frustum.h"
#include "main.h"
#define pi 3.14159265358979323846

Frustum::Frustum(float x, float y, float z, float h, float r1, float r2, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->r1 = r1;
    this->r2 = r2;
    this->h = h;

    int n = 20;
    float vertex_buffer_data[18 * n];
    float theta = (2 * pi) / n;
    for (int i = 0; i < 18 * n; i += 18)
    {
        vertex_buffer_data[i] = r1 * cos(theta);
        vertex_buffer_data[i + 1] = h / 2;
        vertex_buffer_data[i + 2] = r1 * sin(theta);
        vertex_buffer_data[i + 3] = r1 * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 4] = h / 2;
        vertex_buffer_data[i + 5] = r1 * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 6] = r2 * cos(theta);
        vertex_buffer_data[i + 7] = -h / 2;
        vertex_buffer_data[i + 8] = r2 * sin(theta);
        vertex_buffer_data[i + 9] = r2 * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 10] = -h / 2;
        vertex_buffer_data[i + 11] = r2 * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 12] = r1 * cos(theta + (2 * pi) / n);
        vertex_buffer_data[i + 13] = h / 2;
        vertex_buffer_data[i + 14] = r1 * sin(theta + (2 * pi) / n);
        vertex_buffer_data[i + 15] = r2 * cos(theta);
        vertex_buffer_data[i + 16] = -h / 2;
        vertex_buffer_data[i + 17] = r2 * sin(theta);
        theta += (2 * pi) / n;
    }
    this->object = create3DObject(GL_TRIANGLES, n * 3 * 2, vertex_buffer_data, color, GL_FILL);
}

void Frustum::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Frustum::tick(float speed_x, float speed_y, float speed_z)
{
    this->position.x += speed_x;
    this->position.y += speed_y;
    this->position.z += speed_z;
}
