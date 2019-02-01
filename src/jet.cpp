#include "jet.h"
#include "main.h"
#define pi 3.14159265358979323846

Jet::Jet(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
    this->tilt_angle = this->rotate_angle = this->rotation = 0;
    this->rotate_jet = this->tilt_jet = false;
    // static const GLfloat vertex1[] = {
    //     0, 0, -2.5,
    //     -0.5, 0, -0.75,
    //     0.5, 0, -0.75,

    //     -0.75, 0, -0.75,
    //     -0.5, 0, -0.75,
    //     -0.4, 0, -0.9,
    //     0.75, 0, -0.75,
    //     0.5, 0, -0.75,
    //     0.4, 0, -0.9,

    //     -1, 0, 1,
    //     -0.75, 0, -0.75,
    //     1, 0, 1,
    //     -0.75, 0, -0.75,
    //     1, 0, 1,
    //     0.75, 0, -0.75,

    //     -2, 0, 2.5,
    //     -1, 0, 1,
    //     2, 0, 2.5,
    //     -1, 0, 1,
    //     2, 0, 2.5,
    //     1, 0, 1,
    // };
    // this->object1 = create3DObject(GL_TRIANGLES, 7 * 3, vertex1, COLOR_DARK_GREY, GL_FILL);
    // static const GLfloat vertex2[] = {
    //     -0.75, 0, 2.5,
    //     0.75, 0, 2.5,
    //     -1, 0, 2.75,
    //     0.75, 0, 2.5,
    //     -1, 0, 2.75,
    //     1, 0, 2.75,
    // };
    // this->object2 = create3DObject(GL_TRIANGLES, 2 * 3, vertex2, COLOR_BLACK, GL_FILL);
    int n_circles = 2000;
    int n = 20;
    float r = 0.4;
    float vertex1[9 * n * n_circles];
    float theta = (2 * pi) / n;
    for (int j = -n_circles / 2; j < n_circles / 2; j++)
    {
        for (int i = 0; i < 9 * n; i += 9)
        {
            vertex1[((j + n_circles / 2) * n * 9) + (i)] = r * cos(theta);
            vertex1[((j + n_circles / 2) * n * 9) + (i + 1)] = r * sin(theta);
            vertex1[((j + n_circles / 2) * n * 9) + (i + 2)] = -((float)(j) / 1000);
            vertex1[((j + n_circles / 2) * n * 9) + (i + 3)] = r * cos(theta + (2 * pi) / n);
            vertex1[((j + n_circles / 2) * n * 9) + (i + 4)] = r * sin(theta + (2 * pi) / n);
            vertex1[((j + n_circles / 2) * n * 9) + (i + 5)] = -((float)(j) / 1000);
            vertex1[((j + n_circles / 2) * n * 9) + (i + 6)] = 0.0f;
            vertex1[((j + n_circles / 2) * n * 9) + (i + 7)] = 0.0f;
            vertex1[((j + n_circles / 2) * n * 9) + (i + 8)] = -((float)(j) / 1000);
            theta += (2 * pi) / n;
        }
    }
    static const GLfloat vertex2[] = {
        //back
        0.4f, -0.2f, 1.0f,
        0.3f, 0.0f, 1.0f,
        1.0f, -0.2f, 1.0f,

        //front
        0.4f, -0.2f, 0.8f,
        0.3f, 0.0f, 0.8f,
        1.0f, -0.2f, 0.8f,

        // //top
        0.4f, 0.0f, 1.0f,
        1.0f, -0.2f, 1.0f,
        1.0f, -0.2f, 0.8f,

        0.4f, 0.0f, 1.0f,
        0.4f, 0.0f, 0.8f,
        1.0f, -0.2f, 0.8f,

        // // bottom
        0.3f, -0.2f, 1.0f,
        1.0f, -0.2f, 1.0f,
        1.0f, -0.2f, 0.8f,

        0.3f, -0.2f, 0.8f,
        0.3f, -0.2f, 1.0f,
        1.0f, -0.2f, 0.8f,

        // left back
        -0.4f, -0.2f, 1.0f,
        -0.3f, 0.0f, 1.0f,
        -1.0f, -0.2f, 1.0f,

        //left front
        -0.4f, -0.2f, 0.8f,
        -0.3f, 0.0f, 0.8f,
        -1.0f, -0.2f, 0.8f,

        // //top
        -0.4f, 0.0f, 1.0f,
        -1.0f, -0.2f, 1.0f,
        -1.0f, -0.2f, 0.8f,

        -0.4f, 0.0f, 1.0f,
        -0.4f, 0.0f, 0.8f,
        -1.0f, -0.2f, 0.8f,

        // // bottom
        -0.3f, -0.2f, 1.0f,
        -1.0f, -0.2f, 1.0f,
        -1.0f, -0.2f, 0.8f,

        -0.3f, -0.2f, 0.8f,
        -0.3f, -0.2f, 1.0f,
        -1.0f, -0.2f, 0.8f};

    static const GLfloat vertex3[] = {
        //back
        0.4f, 0.0f, 0.2f,
        0.3f, 0.2f, 0.2f,
        2.0f, -0.0f, 0.2f,

        //front
        0.4f, -0.0f, -0.1f,
        0.3f, 0.2f, -0.1f,
        2.0f, -0.0f, -0.1f,

        // //top
        0.4f, 0.2f, 0.2f,
        2.0f, -0.0f, 0.2f,
        2.0f, -0.0f, -0.1f,

        0.4f, 0.2f, 0.2f,
        0.4f, 0.2f, -0.1f,
        2.0f, -0.0f, -0.1f,

        // // bottom
        0.3f, -0.0f, 0.2f,
        2.0f, -0.0f, 0.2f,
        2.0f, -0.0f, -0.1f,

        0.3f, -0.0f, -0.1f,
        0.3f, -0.0f, 0.2f,
        2.0f, -0.0f, -0.1f,

        // left back
        -0.4f, -0.0f, 0.2f,
        -0.3f, 0.2f, 0.2f,
        -2.0f, -0.0f, 0.2f,

        //left front
        -0.4f, -0.0f, -0.1f,
        -0.3f, 0.2f, -0.1f,
        -2.0f, -0.0f, -0.1f,

        // //top
        -0.4f, 0.2f, 0.2f,
        -2.0f, -0.0f, 0.2f,
        -2.0f, -0.0f, -0.1f,

        -0.4f, 0.2f, 0.2f,
        -0.4f, 0.2f, -0.1f,
        -2.0f, -0.0f, -0.1f,

        // // bottom
        -0.3f, -0.0f, 0.2f,
        -2.0f, -0.0f, 0.2f,
        -2.0f, -0.0f, -0.1f,

        -0.3f, -0.0f, -0.1f,
        -0.3f, -0.0f, 0.2f,
        -2.0f, -0.0f, -0.1f};

    this->object1 = create3DObject(GL_TRIANGLES, 3 * n * n_circles, vertex1, COLOR_DARK_GREY, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 12 * 3, vertex2, COLOR_WHITE, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 12 * 3, vertex3, COLOR_WHITE, GL_FILL);
}

void Jet::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position);
    glm::mat4 rotate = glm::mat4(1.0f);
    // rotate *= glm::rotate((float)(this->rotate_angle * M_PI / 180.0f), glm::vec3(sin(this->tilt_angle * M_PI / 180.0f), -cos(this->tilt_angle * M_PI / 180.0f), 0));
    rotate *= glm::rotate((float)(this->tilt_angle * M_PI / 180.0f), glm::vec3(sin(this->rotate_angle * M_PI / 180.0f), 0, cos(this->rotate_angle * M_PI / 180.0f)));
    rotate *= glm::rotate((float)(this->rotate_angle * M_PI / 180.0f), glm::vec3(0, 1, 0));

    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object3);
    draw3DObject(this->object2);
    draw3DObject(this->object1);
}

void Jet::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Jet::tick(float speed_x, float speed_y, float speed_z)
{
    this->position.x += speed_x;
    this->position.y += speed_y;
    this->position.z += speed_z;
}