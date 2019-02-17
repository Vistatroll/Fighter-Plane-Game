#include "main.h"
#include "timer.h"
#include "cuboid.h"
#include "cone.h"
#include "cylinder.h"
#include "frustum.h"
#include "sphere.h"
#include "jet.h"
#include "score.h"
#include "checkpoint.h"
#include "volcano.h"
#include "missile.h"
#include "arrow.h"
#include "fuelup.h"
#include "ring.h"
#include "parachute.h"
#include "bomb.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Jet jet;
Cuboid sea;
vector<Volcano> volcanos;
vector<Frustum> volcano_smoke;
vector<Checkpoint> checkpoints;
Arrow arrow;
vector<Missile> enemy_missiles;
vector<Missile> jet_missiles;
vector<Bomb> bombs;
vector<Fuelup> fuelups;
vector<Ring> rings;
vector<Parachute> parachutes;
vector<Score> scores;

float screen_zoom = 0.12, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

float sea_level;
float jet_original_altitude;
float jet_speed, jet_altitude, jet_fuel;
float arrow_sf = 10;
int curr_chkp = 0;

bool left_mouse_button, right_mouse_button;
int num_missiles_jet = -1, num_bombs_jet = -1;

int plane_view, top_view, tower_view, follow_cam_view, helicopter_cam_view;

VAO *xaxis, *yaxis, *zaxis;

glm::vec3 eye, target, up;
glm::vec3 camera_pos, camera_center, camera_up;

Timer t60(1.0 / 60);

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

    up = glm::vec3(0, 1, 0);

    if (plane_view)
    {
        eye = glm::vec3(jet.position.x + 501 * sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y + 2, jet.position.z + 501 * cos(jet.rotate_angle * M_PI / 180.0f));
        target = glm::vec3(jet.position.x - 30 * sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y, jet.position.z - 30 * cos(jet.rotate_angle * M_PI / 180.0f));
    }

    if (top_view)
    {
        eye = glm::vec3(jet.position.x - sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y + 50, jet.position.z + cos(jet.rotate_angle * M_PI / 180.0f));
        target = glm::vec3(jet.position.x, jet.position.y + 2, jet.position.z);
    }

    if (tower_view)
    {
        eye = glm::vec3(100, 50, -20);
        target = glm::vec3(jet.position.x, jet.position.y - 5, jet.position.z);
    }

    if (follow_cam_view)
    {
        eye = glm::vec3(jet.position.x + 5 * sin(jet.rotate_angle * M_PI / 180.0f), jet.position.y + 0.25, jet.position.z + 5 * cos(jet.rotate_angle * M_PI / 180.0f));
        target = jet.position;
    }

    if (helicopter_cam_view)
    {
        //helicopter view
    }

    Matrices.view = glm::lookAt(eye, target, up);
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Scene render
    sea.draw(VP);

    int len = volcanos.size();
    for (int i = 0; i < len; i++)
        volcanos[i].draw(VP);

    len = volcano_smoke.size();
    for (int i = 0; i < len; i++)
        volcano_smoke[i].draw(VP);

    len = checkpoints.size();
    for (int i = 0; i < len; i++)
        checkpoints[i].draw(VP);

    arrow.draw(VP, arrow_sf);

    len = fuelups.size();
    for (int i = 0; i < len; i++)
        if (fuelups[i].exist)
            fuelups[i].draw(VP);

    len = rings.size();
    for (int i = 0; i < len; i++)
        rings[i].draw(VP);

    len = parachutes.size();
    for (int i = 0; i < len; i++)
        if (parachutes[i].exist)
            parachutes[i].draw(VP);

    len = enemy_missiles.size();
    for (int i = 0; i < len; i++)
        if (enemy_missiles[i].exist)
            enemy_missiles[i].draw(VP);

    len = jet_missiles.size();
    for (int i = 0; i < len; i++)
        if (jet_missiles[i].exist)
            jet_missiles[i].draw(VP);

    len = bombs.size();
    for (int i = 0; i < len; i++)
        if (bombs[i].exist)
            bombs[i].draw(VP);

    jet.draw(VP);

    Matrices.view = glm::lookAt(camera_pos, camera_center, camera_up);
    VP = Matrices.projection1 * Matrices.view;
    len = scores.size();
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

    if (forward && jet.move)
    {
        float x_change = -0.5 * sin(jet.rotate_angle * M_PI / 180.0f);
        float z_change = -0.5 * cos(jet.rotate_angle * M_PI / 180.0f);
        jet.tick(x_change, 0, z_change);
    }
    if (backward && jet.move)
    {
        jet.tick(0, 0, 0.5);
    }
    if (up && jet.move)
    {
        jet.tick(0, 0.1, 0);
    }
    if (down && jet.move)
    {
        jet.tick(0, -0.1, 0);
    }
    if (tilt_left && jet.move)
    {
        jet.tilt_jet = true;
        jet.tilt_angle += 1;
    }
    if (tilt_right && jet.move)
    {
        jet.tilt_jet = true;
        jet.tilt_angle -= 1;
    }
    if (rotate_ccl && jet.move)
    {
        jet.rotate_jet = true;
        jet.rotate_angle += 0.5;
    }
    if (rotate_cl && jet.move)
    {
        jet.rotate_jet = true;
        jet.rotate_angle -= 0.5;
    }

    if (left_mouse_button && jet.move)
    {
        Missile jet_m = Missile(jet.position.x, jet.position.y - 0.5, jet.position.z, 2, 0.6, COLOR_LIGHT_GREY, COLOR_DARK_GREEN);
        if (num_missiles_jet == -1)
        {
            jet_missiles.push_back(jet_m);
            num_missiles_jet++;
            jet_missiles[num_missiles_jet].x_rotation = -90;
            jet_missiles[num_missiles_jet].y_rotation = jet.rotate_angle;
        }
        else
        {
            if (sqrt((jet_missiles[num_missiles_jet].position.x - jet_m.position.x) * (jet_missiles[num_missiles_jet].position.x - jet_m.position.x) + (jet_missiles[num_missiles_jet].position.y - jet_m.position.y) * (jet_missiles[num_missiles_jet].position.y - jet_m.position.y) + (jet_missiles[num_missiles_jet].position.z - jet_m.position.z) * (jet_missiles[num_missiles_jet].position.z - jet_m.position.z)) >= 10)
            {
                jet_missiles.push_back(jet_m);
                num_missiles_jet++;
                jet_missiles[num_missiles_jet].x_rotation = -90;
                jet_missiles[num_missiles_jet].y_rotation = jet.rotate_angle;
            }
        }
    }

    if (right_mouse_button && jet.move)
    {
        Bomb jet_b = Bomb(jet.position.x, jet.position.y - 0.5, jet.position.z);
        if (num_bombs_jet == -1)
        {
            bombs.push_back(jet_b);
            num_bombs_jet++;
        }
        else
        {
            if (sqrt((bombs[num_bombs_jet].position.x - jet_b.position.x) * (bombs[num_bombs_jet].position.x - jet_b.position.x) + (bombs[num_bombs_jet].position.y - jet_b.position.y) * (bombs[num_bombs_jet].position.y - jet_b.position.y) + (bombs[num_bombs_jet].position.z - jet_b.position.z) * (bombs[num_bombs_jet].position.z - jet_b.position.z)) >= 10)
            {
                bombs.push_back(jet_b);
                num_bombs_jet++;
            }
        }
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

    jet_altitude = 10 * jet.position.y;
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

    arrow_sf = sqrt((jet.position.x - checkpoints[0].position.x) * (jet.position.x - checkpoints[0].position.x) + (jet.position.y - checkpoints[0].position.y) * (jet.position.y - checkpoints[0].position.y) + (jet.position.z - checkpoints[0].position.z) * (jet.position.z - checkpoints[0].position.z)) / (float)4;
    if (arrow_sf <= 2)
        arrow_sf = 2;
    if (arrow_sf >= 10)
        arrow_sf = 10;

    int len = jet_missiles.size();
    for (int i = 0; i < len; i++)
    {
        jet_missiles[i].tick(-0.3 * sin(jet_missiles[i].y_rotation * M_PI / 180.0f), 0, -0.3 * cos(jet_missiles[i].y_rotation * M_PI / 180.0f));
    }

    len = bombs.size();
    for (int i = 0; i < len; i++)
    {
        bombs[i].tick();
    }

    if (checkpoints[curr_chkp].exist)
    {
        if (sqrt((jet.position.x - checkpoints[curr_chkp].position.x) * (jet.position.x - checkpoints[curr_chkp].position.x) + (jet.position.y - checkpoints[curr_chkp].position.y) * (jet.position.y - checkpoints[curr_chkp].position.y) + (jet.position.z - checkpoints[curr_chkp].position.z) * (jet.position.z - checkpoints[curr_chkp].position.z)) <= 60)
        {
            len = enemy_missiles.size();
            Missile en_m = Missile(checkpoints[curr_chkp].position.x - 1.8, checkpoints[curr_chkp].position.y + 5, checkpoints[curr_chkp].position.z, 2, 0.6, COLOR_LIGHT_GREY, COLOR_RED);

            if (len == 0)
            {
                enemy_missiles.push_back(en_m);
                enemy_missiles[len].move_up = true;
            }
            else
            {
                if (sqrt((enemy_missiles[len - 1].position.x - en_m.position.x) * (enemy_missiles[len - 1].position.x - en_m.position.x) + (enemy_missiles[len - 1].position.y - en_m.position.y) * (enemy_missiles[len - 1].position.y - en_m.position.y) + (enemy_missiles[len - 1].position.z - en_m.position.z) * (enemy_missiles[len - 1].position.z - en_m.position.z)) >= 15)
                {
                    enemy_missiles.push_back(en_m);
                    enemy_missiles[len].move_up = true;
                }
            }
        }
    }

    len = enemy_missiles.size();
    for (int i = 0; i < len; i++)
    {
        if (enemy_missiles[i].position.y >= jet.position.y && enemy_missiles[i].move_up)
        {
            enemy_missiles[i].move_up = false;
            if (jet.position.x <= enemy_missiles[i].position.x)
                enemy_missiles[i].z_rotation = 90;
            else
                enemy_missiles[i].z_rotation = -90;
            if (jet.position.z <= enemy_missiles[i].position.z)
            {
                int angle = 20 + rand() % 50;
                enemy_missiles[i].x_rotation = -angle;
            }
            else
            {
                int angle = 20 + rand() % 50;
                enemy_missiles[i].x_rotation = angle;
            }
        }

        if (enemy_missiles[i].move_up)
        {
            enemy_missiles[i].tick(0, 0.1, 0);
        }
        if (enemy_missiles[i].z_rotation == 90)
        {
            enemy_missiles[i].tick(-0.1 * cos(enemy_missiles[i].x_rotation * M_PI / 180.0f), 0, 0.1 * sin(enemy_missiles[i].x_rotation * M_PI / 180.0f));
        }
        else if (enemy_missiles[i].z_rotation == -90)
        {
            enemy_missiles[i].tick(0.1 * cos(enemy_missiles[i].x_rotation * M_PI / 180.0f), 0, 0.1 * sin(enemy_missiles[i].x_rotation * M_PI / 180.0f));
        }
    }

    len = volcano_smoke.size();
    for (int i = 0; i < volcanos.size(); i++)
    {
        Frustum s = Frustum(volcanos[i].position.x, volcanos[i].position.y + 12, volcanos[i].position.z, 0.2, 1.7, 1.9, COLOR_DARK_GREY);
        if (len == 0)
        {
            volcano_smoke.push_back(s);
            volcano_smoke[len].rotation = 0;
        }
        else
        {
            if ((volcano_smoke[len - 1].position.y - s.position.y) >= 3)
            {
                volcano_smoke.push_back(s);
                volcano_smoke[len].rotation = 0;
            }
        }
    }

    len = volcano_smoke.size();
    for (int i = 0; i < len; i++)
    {
        volcano_smoke[i].tick(0.02, 0.05, -0.02);
    }
    len = parachutes.size();
    for (int i = 0; i < len; i++)
        parachutes[i].tick();
}

void initGL(GLFWwindow *window, int width, int height)
{
    sea = Cuboid(0, sea_level, 0, abs(2 * sea_level), 2000, 2000, COLOR_SEA);

    jet = Jet(0, jet_original_altitude, 40);

    for (int i = 0; i < 10; i++)
    {
        int x = rand() % 200 - 100;
        int z = -(40 * i + 20);
        Checkpoint chkp = Checkpoint((float)x, 1, (float)z, COLOR_CHECKPOINT);
        checkpoints.push_back(chkp);
    }

    Volcano volcano = Volcano(-30, 0, -60, COLOR_VOLCANO);
    volcanos.push_back(volcano);
    volcano = Volcano(20, 0, -200, COLOR_VOLCANO);
    volcanos.push_back(volcano);
    volcano = Volcano(-15, 0, -320, COLOR_VOLCANO);
    volcanos.push_back(volcano);
    volcano = Volcano(5, 0, -440, COLOR_VOLCANO);
    volcanos.push_back(volcano);
    for (int i = 0; i < 10; i++)
    {
        int x = -(rand() % 300 + 40);
        int z = -(rand() % 500 + 50);
        volcano = Volcano((float)x, 0, (float)z, COLOR_VOLCANO);
        volcanos.push_back(volcano);
    }
    for (int i = 0; i < 10; i++)
    {
        int x = (rand() % 300 + 30);
        int z = -(rand() % 500 + 50);
        volcano = Volcano((float)x, 0, (float)z, COLOR_VOLCANO);
        volcanos.push_back(volcano);
    }

    arrow = Arrow(checkpoints[curr_chkp].position.x, 30, checkpoints[curr_chkp].position.z);

    for (int i = 0; i < 5; i++)
    {
        int x = rand() % 80 - 40;
        int y = rand() % 20 + 5;
        int z = -(rand() % 50 + 25);
        Fuelup fp = Fuelup((float)x, (float)y, (float)z);
        fuelups.push_back(fp);
    }

    for (int i = 0; i < 5; i++)
    {
        int n = rand() % 2 + 3;
        int x = rand() % 100 - 50;
        int y = rand() % 15 + 10;
        int z = -(30 * i + 40);
        for (int j = 0; j < n; j++)
        {
            z -= 3;
            Ring r = Ring((float)x, (float)y, (float)z);
            rings.push_back(r);
        }
    }

    for (int i = 0; i < 20; i++)
    {
        int x = rand() % 60 - 30;
        int y = 20 * (i + 1);
        int z;
        if (i == 0)
            z = -10;
        else
            z = parachutes[i - 1].position.z - 20;
        Parachute para = Parachute((float)x, (float)y, (float)z);
        parachutes.push_back(para);
    }

    float pos_org_x = camera_pos.x - 15;
    float pos_org_y = camera_pos.y - 30;
    Score s;
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
    follow_cam_view = 1;

    jet_original_altitude = 10;
    sea_level = -400;
    jet_altitude = 10 * jet_original_altitude;
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
            detect_collisions();
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
    Matrices.projection = glm::ortho(left, right, bottom, top, -500.0f, 500.0f);
    Matrices.projection1 = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

void detect_collisions()
{
    int len1, len2;

    //plane-missile
    len1 = enemy_missiles.size();
    for (int i = 0; i < len1; i++)
    {
        if (enemy_missiles[i].exist)
        {
            if (enemy_missiles[i].position.y <= jet.position.y + 0.5 && enemy_missiles[i].position.y - 0.1 >= jet.position.y - 0.5)
            {
                if (enemy_missiles[i].position.x <= jet.position.x + 3 && enemy_missiles[i].position.x >= jet.position.x - 3)
                {
                    if (enemy_missiles[i].position.z <= jet.position.z + 1.5 && enemy_missiles[i].position.z >= jet.position.z - 1.5)
                    {
                        enemy_missiles[i].exist = false;
                    }
                }
            }
        }
    }

    //missile-cannon
    len1 = jet_missiles.size();
    len2 = checkpoints.size();
    for (int i = 0; i < len1; i++)
    {
        for (int j = 0; j < len2; j++)
        {
            if (j == curr_chkp)
            {
                if (jet_missiles[i].exist && checkpoints[j].exist)
                {
                    if (jet_missiles[i].position.y <= checkpoints[j].position.y + 4.5 && jet_missiles[i].position.y >= checkpoints[j].position.y + 1.5)
                    {
                        if (jet_missiles[i].position.x <= checkpoints[j].position.x + 2 && jet_missiles[i].position.x >= checkpoints[j].position.x - 2)
                        {
                            if (jet_missiles[i].position.z <= checkpoints[j].position.z + 1.5 && jet_missiles[i].position.z >= checkpoints[j].position.z - 1.5)
                            {
                                jet_missiles[i].exist = false;
                                checkpoints[j].exist = false;
                                curr_chkp++;
                                arrow.position.x = checkpoints[curr_chkp].position.x;
                                arrow.position.y = 30;
                                arrow.position.z = checkpoints[curr_chkp].position.z;
                            }
                        }
                    }
                }
            }
        }
    }

    //bomb-cannon
    len1 = bombs.size();
    len2 = checkpoints.size();
    for (int i = 0; i < len1; i++)
    {
        for (int j = 0; j < len2; j++)
        {
            if (j == curr_chkp)
            {
                if (bombs[i].exist && checkpoints[j].exist)
                {
                    if (bombs[i].position.y - 0.6 <= checkpoints[j].position.y + 4)
                    {
                        if (bombs[i].position.z >= checkpoints[j].position.z - 1.5 && bombs[i].position.z <= checkpoints[j].position.z + 1.5)
                        {
                            if (bombs[i].position.x <= checkpoints[j].position.x + 2 && bombs[i].position.x - 0.6 >= checkpoints[j].position.x - 2)
                            {
                                bombs[i].exist = false;
                                checkpoints[j].exist = false;
                                curr_chkp++;
                                arrow.position.x = checkpoints[curr_chkp].position.x;
                                arrow.position.y = 30;
                                arrow.position.z = checkpoints[curr_chkp].position.z;
                            }
                        }
                    }
                }
            }
        }
    }

    //missile-parachute
    len1 = jet_missiles.size();
    len2 = parachutes.size();
    for (int i = 0; i < len1; i++)
    {
        for (int j = 0; j < len2; j++)
        {
            if (jet_missiles[i].exist && parachutes[j].exist)
            {
                if (jet_missiles[i].position.y + 0.2 <= parachutes[j].position.y + 6 && jet_missiles[i].position.y - 0.2 >= parachutes[j].position.y - 1)
                {
                    if (jet_missiles[i].position.x + 0.2 <= parachutes[j].position.x + 2 && jet_missiles[i].position.x - 0.2 >= parachutes[j].position.x - 2)
                    {
                        if (jet_missiles[i].position.z + 1 <= parachutes[j].position.z + 2 && jet_missiles[i].position.z - 1 >= parachutes[j].position.z - 2)
                        {
                            parachutes[j].exist = false;
                        }
                    }
                }
            }
        }
    }

    //plane-fuel
    len1 = fuelups.size();
    for (int i = 0; i < len1; i++)
    {
        if (fuelups[i].exist)
        {
            if (jet.position.y + 0.4 <= fuelups[i].position.y + 1.7 && jet.position.y - 0.4 >= fuelups[i].position.y - 1.7)
            {
                if (jet.position.x + 2 >= fuelups[i].position.x + 1.5 && jet.position.x - 2 <= fuelups[i].position.x - 1.5)
                {
                    if (jet.position.z + 2 >= fuelups[i].position.z + 1.5 && jet.position.z - 2 <= fuelups[i].position.z - 1.5)
                    {
                        fuelups[i].exist = false;
                    }
                }
            }
        }
    }

    //no flying zone
    len1 = volcanos.size();
    for (int i = 0; i < len1; i++)
    {
        if (jet.position.x <= volcanos[i].position.x + 9 && jet.position.x >= volcanos[i].position.x - 9)
        {
            if (jet.position.z <= volcanos[i].position.z + 9 && jet.position.z >= volcanos[i].position.z - 9)
            {
                quit(window);
            }
        }
    }

    //going through ring
    len1 = rings.size();
    for (int i = 0; i < len1; i++)
    {
        if (jet.position.y >= rings[i].position.y - 4 && jet.position.y <= rings[i].position.y + 4)
        {
            if (jet.position.x >= rings[i].position.x - 0.8 && jet.position.x <= rings[i].position.x + 0.8)
            {
                if (jet.position.z >= rings[i].position.z - 0.5 && jet.position.z <= rings[i].position.z + 0.5)
                {
                    cout << "Into the ring\n";
                }
            }
        }
    }
}
