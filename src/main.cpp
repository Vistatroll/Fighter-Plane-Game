#include "main.h"
#include "timer.h"
#include "ball.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

int first_person = 1, tower;

glm::vec3 eye, target, up;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
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

    Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;

    // Scene render
    ball1.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    if (left)
    {
        // Do something
    }
    first_person = glfwGetKey(window, GLFW_KEY_F);
    tower = glfwGetKey(window, GLFW_KEY_T);
    if (first_person)
    {
        eye = ball1.position;
        target = glm::vec3(ball1.position.x, ball1.position.y, ball1.position.z - 5);
    }
    if (tower)
    {
        eye = glm::vec3(10, 5, 0);
        target = ball1.position;
    }
}

void tick_elements()
{
    ball1.tick();
    // camera_rotation_angle += 1;
}

void initGL(GLFWwindow *window, int width, int height)
{
    // Create the models
    ball1 = Ball(0, 0, 0, COLOR_RED);

    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
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

    eye = ball1.position;
    target = glm::vec3(ball1.position.x, ball1.position.y, ball1.position.z - 5);
    up = glm::vec3(0, 1, 0);

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
