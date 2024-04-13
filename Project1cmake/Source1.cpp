#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
struct Target {
  float x, y, r, s, co[3];
};
struct Shot {
    float x1, y1,x2, y2;  //start && end point of shot
    bool flag;
};
vector<Target>v(5);Shot shot;
int mx = 400, my= 300,sc= 0;
bool go= false; //flag for game over 
void drawbackground() {
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw stars as random points
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 1000; i++) {
        float x = (float)(rand() % 640);
        float y = (float)(rand() % 480);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawtarget(const Target& target) {
    glColor3fv(target.co);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14/ 180.0;
        float cx = target.x + target.r * cos(theta);
        float cy = target.y + target.r * sin(theta);
        glVertex2f(cx, cy);
    }
    glEnd();
}
void drawrocket(int x, int y) { // draw shooter
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2i(x, y);
    glVertex2i(x - 20, y - 50);
    glVertex2i(x + 20, y - 50);
    glEnd();
}

void drawshot(const Shot& shot) { // draw the shot as a red line
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(shot.x1, shot.y1);
    glVertex2f(shot.x2, shot.y2);
    glEnd();
}
void drawscore() { // show the score
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(10, 10);// position pixel and bitmap write operations
    string txt = "Score: " + to_string(sc);
    for (char c :txt) 
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);//size=18
}
void drawGAMEOVER() {// show the message
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(200, 240);
    string txt = "GAME OVER!";
    for (char c : txt) {
       glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); 
    }
}
void draw() {
    drawbackground();
    for (const auto&xx :v)drawtarget(xx); // draw the moving targets
    drawrocket(mx, my);
    if (shot.flag) drawshot(shot);
    drawscore();
    if (go) drawGAMEOVER();// boolian to check if the game is over
    glutSwapBuffers();
}
bool check_boom(const Target& tr, const Shot& sh) {
    float d = sqrt(pow(sh.x2 - tr.x, 2) + pow(sh.y2 - tr.y, 2));
    return d< tr.r;
}
void movetargets() {
    for (auto&yy : v) {
         yy.y -=yy.s;
        // Reset the target when it goes off the top
        if (yy.y < 0.0f) {
           yy.y = 480.0f;
           yy.r = (float)(rand() % 20) + 10.0f; // random size for each target
           yy.s = (float)(rand() % 5) + 1.0f;  //  random target speed
            for (int i = 0; i < 3;i++) 
                yy.co[i] = (float)rand() / RAND_MAX; //random color
        }
        if (shot.flag && check_boom(yy, shot)) {
           yy.y = 480.0f;shot.flag = false; 
            sc++; // increase the score
        }
    }
    // check if game is over
    if (sc>10)go= true;
}
void moveshot() { 
    //move the shot upward
    if (shot.flag) {
        shot.y1 += 5.0; shot.y2 += 5.0;
    if (shot.y1 > 600.0) shot.flag = false;
    }
}
void timer(int val) {
    movetargets(); moveshot();
    if (!go) {
       glutPostRedisplay();
       glutTimerFunc(17, timer, 0);
    }
}
void moving_mouse(int x, int y) {
    mx = x; my = 600 - y;
    glutPostRedisplay();
}
void clicking_on_mouse(int button, int stat, int x, int y) {
    if (!go&& button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        // Fire a shot only if the game is not over
        shot.flag = true; shot.x1 = mx;
        shot.y1 = my + 20;shot.x2 = mx;
        shot.y2 = my+ 40; // length of the shot
    }
}
void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);
    srand((unsigned)time(nullptr)); // initialize targets with random property
    for (auto& tr : v) {
        tr.x = (float)(rand() % 640);
        tr.y = (float)(rand() % 480);
        tr.r = (float)(rand() % 20) + 10.0f;
        tr.s = (float)(rand() % 5) + 1.0f;
        for (int i = 0; i < 3;i++) 
            tr.co[i] = (float)rand() / RAND_MAX;
    }
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Moving Targets with Rocket and Shots");
    init();
    glutDisplayFunc(draw);
    glutTimerFunc(0, timer, 0);
    glutMotionFunc(moving_mouse);
    glutMouseFunc(clicking_on_mouse);
    glutMainLoop();
    return 0;
}
