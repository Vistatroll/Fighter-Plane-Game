#include "arrow.h"
#include "main.h"

Arrow::Arrow(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data_1[] = {
        // top
        0.0f, -0.1f, 0.0f,
        0.3f, 0.0f, 0.0f,
        0.3f, -0.1f, 0.0f,
        0.0f, -0.1f, 0.0f,
        -0.3f, 0.0f, 0.0f,
        -0.3f, -0.1f, 0.0f,

        //mig right
        0.0f, -0.1f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.3f, -0.1f, 0.0f,
        0.3f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.3f, -0.1f, 0.0f,

        // mid left
        0.0f, -0.1f, 0.0f,
        0.0f, -0.5f, 0.0f,
        -0.3f, -0.1f, 0.0f,
        -0.3f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        -0.3f, -0.1f, 0.0f,

        //bottom -arrow-head
        0.0f, -0.5f, 0.0f,
        0.4f, -0.5f, 0.0f,
        0.0f, -0.9f, 0.0f,
        0.0f, -0.5f, 0.0f,
        -0.4f, -0.5f, 0.0f,
        0.0f, -0.9f, 0.0f};

    this->object = create3DObject(GL_TRIANGLES, 8 * 3, vertex_buffer_data_1, COLOR_RED, GL_FILL);
}

void Arrow::draw(glm::mat4 VP, float sf)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 scale = glm::scale(glm::vec3(sf, sf, sf));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}
