#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#else
#include <GL/glut.h>
#endif
#include <vector>
using namespace std;

int SW = 500;
int SH = 500;

float sx = 100;
float sy = 100;
float sz = 0;
float rot = 0;

float len = 100;

float vx = 3;
float vy = 2;
float vr = 2;
float gravity = .1;
float elst = .8;

int startMouseClickX = 0;
int startMouseClickY = 0;
bool isMouseDown = false;

void drawRect(int sx, int sy, int w, int h)
{
  glBegin(GL_POLYGON);
  glVertex2f(sx, sy);
  glVertex2f(sx + w, sy);
  glVertex2f(sx + w, sy + h);
  glVertex2f(sx, sy + h);
  glEnd();
}

void cube(float midx, float midy, float midz, float size)
{

  // Define 8 vertices
  float ax = midx - size / 2;
  float ay = midy - size / 2;
  float az = midz + size / 2;
  float bx = midx + size / 2;
  float by = midy - size / 2;
  float bz = midz + size / 2;
  float cx = midx + size / 2;
  float cy = midy + size / 2;
  float cz = midz + size / 2;
  float dx = midx - size / 2;
  float dy = midy + size / 2;
  float dz = midz + size / 2;
  float ex = midx - size / 2;
  float ey = midy - size / 2;
  float ez = midz - size / 2;
  float fx = midx + size / 2;
  float fy = midy - size / 2;
  float fz = midz - size / 2;
  float gx = midx + size / 2;
  float gy = midy + size / 2;
  float gz = midz - size / 2;
  float hx = midx - size / 2;
  float hy = midy + size / 2;
  float hz = midz - size / 2;

  glPushMatrix();
  glRotatef(rot, sx + len / 2, sy + len / 2, sz);

  // Draw 6 faces
  glBegin(GL_QUADS);

  // Purple
  glColor3f(1.0, 0.0, 1.0);
  glVertex3f(gx, gy, gz);
  glVertex3f(cx, cy, cz);
  glVertex3f(bx, by, bz);
  glVertex3f(fx, fy, fz);

  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(ax, ay, az);
  glVertex3f(ex, ey, ez);
  glVertex3f(fx, fy, fz);
  glVertex3f(bx, by, bz);

  glColor3f(0.0, 1.0, 1.0);
  glVertex3f(gx, gy, gz);
  glVertex3f(fx, fy, fz);
  glVertex3f(ex, ey, ez);
  glVertex3f(hx, hy, hz);

  // Green
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(ax, ay, az);
  glVertex3f(dx, dy, dz);
  glVertex3f(hx, hy, hz);
  glVertex3f(ex, ey, ez);

  // Yellow
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(gx, gy, gz);
  glVertex3f(hx, hy, hz);
  glVertex3f(dx, dy, dz);
  glVertex3f(cx, cy, cz);

  // Red
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(ax, ay, az);
  glVertex3f(bx, by, bz);
  glVertex3f(cx, cy, cz);
  glVertex3f(dx, dy, dz);
  glEnd();

  glPopMatrix();
}

void setColor(float r, float g, float b)
{
  glColor3f(r, g, b);
}

void logicUpdate()
{
  if (!isMouseDown)
  {
    vy -= gravity;
    sx = sx + vx;
    sy = sy + vy;
    rot += vr;
  }

  if (sx > SW - len)
  {
    vx *= -elst;
    sx = (SW - len) - 10;
  }
  if (sx < 0)
  {
    vx *= -elst;
    sx = 10;
  }
  if (sy < 0)
  {
    vy *= -elst;
    sy = 0;
  }
  if (sy > SH - len)
  {
    vy *= -elst;
    sy = SH - len;
  }
}

void mouseClick(int p1, int up, int x, int y)
{
  if (up == 0)
  {
    sx = x - len / 2;
    sy = SH - (y + len / 2);
    vx = 0;
    vy = 0;
    startMouseClickX = x;
    startMouseClickY = y;
    isMouseDown = true;
  }
  else
  {
    isMouseDown = false;

    int dx = x - startMouseClickX;
    int dy = y - startMouseClickY;

    vx = dx / -10;
    vy = dy / 10;
  }
}

void mouseMove(int x, int y)
{
  if (isMouseDown)
  {
    sx = x - len / 2;
    sy = SH - (y + len / 2);
  }
}

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);

  logicUpdate();
  cube(sx + len / 2, sy + len / 2, sz, len);
  glFlush();
}

void timer(int p1)
{
  glutPostRedisplay();
  glutTimerFunc(1000 / 60, timer, 0);
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, SW, 0, SH, -1000.0, 1000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_CULL_FACE);
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Square");
  glutDisplayFunc(draw);
  glutTimerFunc(1000 / 60, timer, 0);
  glutMouseFunc(mouseClick);
  glutMotionFunc(mouseMove);
  init();

  // read in file

  glutMainLoop();
  return 0;
}
