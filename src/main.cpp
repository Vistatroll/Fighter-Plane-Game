#include "main.h"
#include "timer.h"
#include "jet.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Jet jet;

float screen_zoom = 0.5, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

int plane_view = 1, top_view, tower_view, follow_cam_view, helicopter_cam_view;

VAO *xaxis, *yaxis, *zaxis;

glm::vec3 eye, target, up;

Timer t60(1.0 / 60);

void createxaxis()
{
    GLfloat vertex_buffer_data[] = {
        1000, 0, 0,  // vertex 0
        -1000, 0, 0, // vertex 1
    };

    GLfloat color_buffer_data[] = {
        1, 0, 0, // color 0
        1, 0, 0, // color 1
    };

    xaxis = create3DObject(GL_LINES, 3, vertex_buffer_data, COLOR_RED, GL_LINE);
}
void createyaxis()
{
    GLfloat vertex_buffer_data[] = {
        0, 1000, 0,  // vertex 0
        0, -1000, 0, // vertex 1
    };

    GLfloat color_buffer_data[] = {
        0, 1, 0, // color 0
        0, 1, 0, // color 1
    };

    yaxis = create3DObject(GL_LINES, 3, vertex_buffer_data, COLOR_GREEN, GL_LINE);
}

void createzaxis()
{
    GLfloat vertex_buffer_data[] = {
        0, 0, 1000,  // vertex 0
        0, 0, -1000, // vertex 1
    };

    GLfloat color_buffer_data[] = {
        0, 0, 1, // color 0
        0, 0, 1, // color 1
    };

    zaxis = create3DObject(GL_LINES, 3, vertex_buffer_data, COLOR_BLUE, GL_LINE);
}
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // eye  = glm::vec3( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // target =  glm::vec3(0, 0, 0);
    // // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // up = glm::vec3(0, 1, 0);
    // eye = jet.position;
    // target = glm::vec3(jet.position.x, jet.position.y, jet.position.z - 5);
    up = glm::vec3(0, 1, 0);

    if (plane_view)
    {
        eye = glm::vec3(jet.position.x - sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y, jet.position.z + cos(jet.rotate_angle * M_PI / 180.0f));
        target = glm::vec3(jet.position.x - 30 * sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y, jet.position.z + 30 * cos(jet.rotate_angle * M_PI / 180.0f));
    }

    if (top_view)
    {
        eye = glm::vec3(jet.position.x - sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y + 50, jet.position.z + cos(jet.rotate_angle * M_PI / 180.0f));
        target = glm::vec3(jet.position.x, jet.position.y + 2, jet.position.z);
    }

    if (tower_view)
    {
        eye = glm::vec3(10, 5, 0);
        target = jet.position;
    }

    if (follow_cam_view)
    {
        eye = glm::vec3(jet.position.x + 40 * sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y + 20, jet.position.z + 40 * cos(jet.rotate_angle * M_PI / 180.0f));
        target = jet.position;
    }

    if (helicopter_cam_view)
    {
        //helicopter view
    }

    Matrices.view = glm::lookAt(eye, target, up);
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;

    // // Scene render
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translatexaxis = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 xaxisTransform = translatexaxis;
    Matrices.model *= xaxisTransform;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(xaxis);

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateyaxis = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 yaxisTransform = translateyaxis;
    Matrices.model *= yaxisTransform;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(yaxis);

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translatezaxis = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 zaxisTransform = translatezaxis;
    Matrices.model *= zaxisTransform;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(zaxis);

    jet.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    int forward = glfwGetKey(window, GLFW_KEY_UP);
    int backward = glfwGetKey(window, GLFW_KEY_DOWN);
    int up = glfwGetKey(window, GLFW_KEY_Z);
    int down = glfwGetKey(window, GLFW_KEY_C);
    int tilt_left = glfwGetKey(window, GLFW_KEY_Q);
    int tilt_right = glfwGetKey(window, GLFW_KEY_E);
    int rotate_ccl = glfwGetKey(window, GLFW_KEY_A);
    int rotate_cl = glfwGetKey(window, GLFW_KEY_D);

    if (glfwGetKey(window, GLFW_KEY_1))
    {
        //plane view
        plane_view = 1;
        top_view = tower_view = follow_cam_view = helicopter_cam_view = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2))
    {
        //top view
        top_view = 1;
        plane_view = tower_view = follow_cam_view = helicopter_cam_view = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_3))
    {
        //tower view
        tower_view = 1;
        plane_view = top_view = follow_cam_view = helicopter_cam_view = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_4))
    {
        //follow cam view
        follow_cam_view = 1;
        plane_view = top_view = tower_view = helicopter_cam_view = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_5))
    {
        //helicopter cam view
        helicopter_cam_view = 1;
        plane_view = top_view = tower_view = follow_cam_view = 0;
    }

    if (forward)
    {
        float x_change = -0.5 * sin(jet.rotate_angle * M_PI / 180.0f);
        float z_change = -0.5 * cos(jet.rotate_angle * M_PI / 180.0f);
        jet.tick(x_change, 0, z_change);
    }
    if (backward)
    {
        jet.tick(0, 0, 0.5);
    }
    if (up)
    {
        jet.tick(0, 0.5, 0);
    }
    if (down)
    {
        jet.tick(0, -0.5, 0);
    }
    if (tilt_left)
    {
        jet.tilt_jet = true;
        jet.tilt_angle += 1;
    }
    if (tilt_right)
    {
        jet.tilt_jet = true;
        jet.tilt_angle -= 1;
    }
    if (rotate_ccl)
    {
        jet.rotate_jet = true;
        jet.rotate_angle += 1;
    }
    if (rotate_cl)
    {
        jet.rotate_jet = true;
        jet.rotate_angle -= 1;
    }
}

void tick_elements()
{
}

void initGL(GLFWwindow *window, int width, int height)
{
    // Create the models
    jet = Jet(0, 0, -2);

    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    createxaxis();
    createyaxis();
    createzaxis();

    glClearColor(COLOR_SKY.r / 256.0, COLOR_SKY.g / 256.0, COLOR_SKY.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    srand(time(0));
    int width = 1000;
    int height = 1000;

    window = initGLFW(width, height);

    // eye = jet.position;
    // target = glm::vec3(jet.position.x, jet.position.y, jet.position.z - 5);
    // up = glm::vec3(0, 1, 0);

    initGL(window, width, height);

    while (!glfwWindowShouldClose(window))
    {
        if (t60.processTick())
        {
            draw();
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
