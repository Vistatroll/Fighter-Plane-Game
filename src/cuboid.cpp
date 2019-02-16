#include "cuboid.h"
#include "main.h"

Cuboid::Cuboid(float x, float y, float z, float height, float width, float breadth, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;

    float h = height / 2;
    float w = width / 2;
    float b = breadth /2 ;

    const GLfloat vertex_buffer_data[] = {
        //top
        w, h, b,
        w, h, -b,
        -w, h, -b,
        w, h, b,
        -w, h, b,
        -w, h, -b,
        
        //bottom
        w, -h, b,
        w, -h, -b,
        -w, -h, -b,
        w, -h, b,
        -w, -h, b,
        -w, -h, -b,
        
        // front
        w, h, -b,
        w, -h, -b,
        -w, -h, -b,
        w, h, -b,
        -w, h, -b,
        -w, -h, -b,
        
        //back
        w, h, b,
        w,-h, b,
        -w, -h, b,
        w, h, b,
        -w, h, b,
        -w,-h, b,

        // lefy side
         -w, h, b,
        -w, -h, b,
        -w, h, -b,
        -w, h, -b,
        -w, -h, -b,
        -w, -h, b,

        // fight side
         w, h, b,
        w, -h, b,
        w, h, -b,
        w, h, -b,
        w, -h, -b,
        w, -h, b,

    };
    this->object = create3DObject(GL_TRIANGLES, 12 * 3, vertex_buffer_data, color, GL_FILL);
}

void Cuboid::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Cuboid::tick() {
}
