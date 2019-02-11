#include "cube.h"
#include "main.h"

Cube::Cube(float x, float y, float z, float w, float h, float b, color_t color)
{
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        //top
        w/2, h/2, b/2,
        -w/2, h/2, b/2,
        w/2, h/2, -b/2,
        -w/2, h/2, b/2,
        w/2, h/2, -b/2,
        -w/2, h/2, -b/2,

        //bottom
        w/2, -h/2, b/2,
        w/2, -h/2, -b/2,
        -w/2, -h/2, b/2,
        w/2, -h/2, -b/2,
        -w/2, -h/2, b/2,
        -w/2, -h/2, -b/2,

        //front
        w/2, h/2, b/2,
        w/2, -h/2, b/2,
        -w/2, h/2, b/2,
        -w/2, h/2, b/2,
        w/2, -h/2, b/2,
        -w/2, -h/2, b/2,

        //back
        w/2, -h/2, -b/2,
        -w/2, -h/2, -b/2,
        w/2, h/2, -b/2,
        -w/2, -h/2, -b/2,
        w/2, h/2, -b/2,
        -w/2, h/2, -b/2,

        //right
        w/2, h/2, b/2,
        w/2, h/2, -b/2,
        w/2, -h/2, b/2,
        w/2, h/2, -b/2,
        w/2, -h/2, b/2,
        w/2, -h/2, -b/2,

        //left
        -w/2, -h/2, -b/2,
        -w/2, -h/2, b/2,
        -w/2, h/2, -b/2,
        -w/2, h/2, -b/2,
        -w/2, -h/2, b/2,
        -w/2, h/2, b/2,
    };
    this->object = create3DObject(GL_TRIANGLES, 12 * 3, vertex_buffer_data, color, GL_FILL);
}

void Cube::draw(glm::mat4 VP) 
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}