#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "libim/im_color.h"
using namespace std;

const int imgDim = 512;
const int numOfImages = 20;
const int numOfCubes = 30;
const int worldDim = 500;
const int sh = 1000;
const int sw = 1000;
const int gravity = -1;
typedef unsigned char tex[imgDim * imgDim * 3];
float imgData[numOfImages][imgDim][imgDim][3] = {};
tex textures[numOfImages] = {};

struct vector {
  int x;
  int y;
  int z;
};

struct cube {
  int r;
  vector pos;
  vector vel;
  vector rot;
  tex *texture;
};

cube cubes[numOfCubes];

void read_images() {
  for (int i = 0; i < 20; i++) {
    im_color color_image;
    string filename = "cats_dogs/" + ((i < 10) ? "dog" + to_string(i) : "cat" + to_string(i - 10)) + ".jpg";
    color_image.ReadJpg(filename.c_str());

    int index = 0;
    for (int y = 0; y < imgDim; y++)
      for (int x = 0; x < imgDim; x++) {
        textures[i][index++] = color_image.R.Data2D[y][x];
        textures[i][index++] = color_image.G.Data2D[y][x];
        textures[i][index++] = color_image.B.Data2D[y][x];
      }
  }
}

void init_cubes() {
  for (int i = 0; i < numOfCubes; i++) {
    cubes[i].pos.x = rand() % worldDim * 2 - worldDim;
    cubes[i].pos.y = rand() % worldDim * 2 - worldDim;
    cubes[i].pos.z = rand() % worldDim * 1.5 - worldDim;
    cubes[i].vel.x = rand() % 10 - 5;
    cubes[i].vel.y = rand() % 10 - 20;
    cubes[i].vel.z = 0;
    cubes[i].rot.x = rand() % 360;
    cubes[i].rot.y = rand() % 360;
    cubes[i].rot.z = rand() % 360;
    cubes[i].r = rand() % 10 + 100;
    cubes[i].texture = &textures[rand() % numOfImages];
  }
}

void drawCubeFace(vector a, vector b, vector c, vector d) {
  glTexCoord2f(0, 0);
  glVertex3f(a.x, a.y, a.z);
  glTexCoord2f(0, 1);
  glVertex3f(b.x, b.y, b.z);
  glTexCoord2f(1, 1);
  glVertex3f(c.x, c.y, c.z);
  glTexCoord2f(1, 0);
  glVertex3f(d.x, d.y, d.z);
}

void drawCube(cube cub) {
  int size = cub.r;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgDim, imgDim, 0, GL_RGB, GL_UNSIGNED_BYTE, cub.texture);

  vector a,b,c,d,e,f,g,h;

  a.x = -size / 2; a.y = -size / 2; a.z = size / 2;
  b.x = size / 2; b.y = -size / 2; b.z = size / 2;
  c.x = size / 2; c.y = size / 2; c.z = size / 2;
  d.x = -size / 2; d.y = size / 2; d.z = size / 2;
  e.x = -size / 2; e.y = -size / 2; e.z = -size / 2;
  f.x = size / 2; f.y = -size / 2; f.z = -size / 2;
  g.x = size / 2; g.y = size / 2; g.z = -size / 2;
  h.x = -size / 2; h.y = size / 2; h.z = -size / 2;

  glPushMatrix();
  glTranslatef(cub.pos.x, cub.pos.y, cub.pos.z);
  glRotatef(cub.rot.x, 1, 0, 0);
  glRotatef(cub.rot.y, 0, 1, 0);
  glRotatef(cub.rot.z, 0, 0, 1);

  glBegin(GL_QUADS);

  drawCubeFace(g,c,b,f);
  drawCubeFace(a,e,f,b);
  drawCubeFace(g,f,e,h);
  drawCubeFace(a,d,h,e);
  drawCubeFace(g,h,d,c);
  drawCubeFace(a,b,c,d);

  glEnd();

  glPopMatrix();
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  for (int i =0; i < numOfCubes; i++) {
    drawCube(cubes[i]);
  }

  glFlush();
}

void update(int time) {
  glutPostRedisplay();
  for (int i = 0; i < numOfCubes; i++) {
    cubes[i].pos.x += cubes[i].vel.x;
    cubes[i].pos.y += cubes[i].vel.y;
    cubes[i].vel.y += gravity;
    cubes[i].pos.z += cubes[i].vel.z;
    cubes[i].rot.x += rand() % 2;
    cubes[i].rot.y += rand() % 2;
    cubes[i].rot.z += rand() % 2;

    int edge = worldDim + cubes[i].r;

    if (cubes[i].pos.y < -edge) {
      cubes[i].pos.y = edge;
      cubes[i].vel.y = rand() % 10 - 20;
    }

    if (cubes[i].pos.x < -edge) {
      cubes[i].pos.x = edge;
    }

    if (cubes[i].pos.x > edge) {
      cubes[i].pos.x = -edge;
    }
  }
  glutTimerFunc(1000 / 30, update, 0);
}

int main(int argc, char *argv[]) {
  read_images();
  init_cubes();

  glutInit(&argc, argv);
  glutInitWindowSize(sw, sh);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("CATDOG");

  glutDisplayFunc(draw);
  glutTimerFunc(1000 / 60, update, 0);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-worldDim, worldDim, -worldDim, worldDim, -worldDim, worldDim);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_CULL_FACE);

  // textures
  glEnable(GL_TEXTURE_2D);
  glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

  glutMainLoop();

  return 0;
}
