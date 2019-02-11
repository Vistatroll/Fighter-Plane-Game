#include "main.h"
#include "timer.h"
#include "cube.h"
#include "jet.h"
#include "score.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Jet jet;
Cube sea;
vector<Score> scores;

float screen_zoom = 0.25, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

float sea_level = -40;
float jet_original_altitude;

float jet_speed, jet_altitude, jet_fuel;

int plane_view = 1, top_view, tower_view, follow_cam_view, helicopter_cam_view;

VAO *xaxis, *yaxis, *zaxis;

glm::vec3 eye, target, up;
glm::vec3 camera_pos, camera_center, camera_up;

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

void score_decide(int digit, int i)
{
    if (digit == 0)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = true;
        scores[i].exist7 = false;
    }
    if (digit == 1)
    {
        scores[i].exist2 = scores[i].exist3 = true;
        scores[i].exist1 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = false;
    }
    if (digit == 2)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist4 = scores[i].exist5 = scores[i].exist7 = true;
        scores[i].exist3 = scores[i].exist6 = false;
    }
    if (digit == 3)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist7 = true;
        scores[i].exist5 = scores[i].exist6 = false;
    }
    if (digit == 4)
    {
        scores[i].exist2 = scores[i].exist3 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist1 = scores[i].exist4 = scores[i].exist5 = false;
    }
    if (digit == 5)
    {
        scores[i].exist1 = scores[i].exist3 = scores[i].exist4 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist2 = scores[i].exist5 = false;
    }
    if (digit == 6)
    {
        scores[i].exist1 = scores[i].exist3 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist2 = false;
    }
    if (digit == 7)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = true;
        scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = true;
    }
    if (digit == 8)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = true;
    }
    if (digit == 9)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist5 = false;
    }
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
        eye = glm::vec3(100, 5, -20);
        target = jet.position;
    }

    if (follow_cam_view)
    {
        eye = glm::vec3(jet.position.x + 25 * sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y - 5, jet.position.z + 25 * cos(jet.rotate_angle * M_PI / 180.0f));
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

    sea.draw(VP);
    jet.draw(VP);

    Matrices.view = glm::lookAt(camera_pos, camera_center, camera_up);
    VP = Matrices.projection * Matrices.view;
    float len = scores.size();
    for (int i = 0; i < len; i++)
    {
        scores[i].draw(VP);
    }
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
    int buf = jet_speed;
    int h = buf / 100;
    buf %= 100;
    int t = buf / 10;
    int u = buf % 10;
    score_decide(h, 2);
    score_decide(t, 3);
    score_decide(u, 4);

    jet_altitude = 10 * (jet.position.y - sea_level);
    buf = jet_altitude;
    h = buf / 100;
    buf %= 100;
    t = buf / 10;
    u = buf % 10;
    score_decide(h, 7);
    score_decide(t, 8);
    score_decide(u, 9);

    buf = jet_fuel;
    h = buf / 100;
    buf %= 100;
    t = buf / 10;
    u = buf % 10;
    score_decide(h, 12);
    score_decide(t, 13);
    score_decide(u, 14);
}

void initGL(GLFWwindow *window, int width, int height)
{
    // Create the models
    sea = Cube(0, sea_level, 0, 4000, 80, 4000, COLOR_BLUE);
    jet = Jet(0, 10, jet_original_altitude);

    float pos_org_x = camera_pos.x - 15;
    float pos_org_y = camera_pos.y - 12;
    Score s;
    Score p;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist3 = s.exist4 = s.exist6 = s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 6.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist5 = s.exist6 = s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 6.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist5 = s.exist6 = s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

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
    int width = 1200;
    int height = 1200;

    jet_original_altitude = -10;
    jet_altitude = jet_original_altitude - sea_level;
    jet_speed = 40;
    jet_fuel = 50;

    camera_pos = glm::vec3(0, 0, 1);
    camera_center = glm::vec3(0, 0, 0);
    camera_up = glm::vec3(0, 1, 0);

    window = initGLFW(width, height);

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
