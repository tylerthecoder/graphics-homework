#include <iostream>
#include <fstream>
#include <tuple>
#include <GL/glut.h>
#include "libim/im_color.h"
#include "shading.cpp"
using namespace std;

typedef tuple<float, float, float> point;

const int dim = 100;
const int imgDim = 500;
const int mult = 500 / dim;

int drawMode = 2;
int rotateX = 0;
int rotateY = 0;
float avgR, avgB, avgG = 0.0;
point colors[dim][dim] = {};
point points[dim][dim] = {};
point normals[dim][dim] = {};

void read_images(string color_name, im_color & color_image, string depth_name, im_float & depth_image) {
  color_image.ReadJpg(color_name.c_str());
  depth_image.ReadJpg(depth_name.c_str());

  int count = 0;
  for (int y = 0; y < imgDim; y = y + mult)
    for (int x = 0; x < imgDim; x = x + mult) {
      int i = x / mult;
      int j = y / mult;
      float r = color_image.R.Data2D[y][x] / 255.0;
      float g = color_image.G.Data2D[y][x] / 255.0;
      float b = color_image.B.Data2D[y][x] / 255.0;

      if (r != 1.0 && b != 1.0 && g != 1.0) {
        avgR += r;
        avgG += g;
        avgB += b;
        count++;
      }

      // massage data
      float d = depth_image.Data2D[y][x] / (10 * 500);
      float xNorm = (i - dim / 2) / (float)dim;
      float yNorm = (j - dim / 2) / (float)dim;
      points[i][j] = make_tuple(xNorm, yNorm, d);
      colors[i][j] = make_tuple(r, g, b);
    }
  avgR /= count;
  avgG /= count;
  avgB /= count;

  // calculate normals
  for (int i = 1; i < dim-1; i++) {
    for (int j = 1; j < dim-1; j++) {
      // i+1 -> i-1 points
      point v1 = points[i+1][j];
      point v2 = points[i-1][j];
      float x1 = get<0>(v1) - get<0>(v2);
      float y1 = get<1>(v1) - get<1>(v2);
      float z1 = get<2>(v1) - get<2>(v2);

      point v3 = points[i][j+1];
      point v4 = points[i][j-1];
      float x2 = get<0>(v3) - get<0>(v4);
      float y2 = get<1>(v3) - get<1>(v4);
      float z2 = get<2>(v3) - get<2>(v4);

      float x = y1 * z2 - z1 * y2;
      float y = z1 * x2 - x1 * z2;
      float z = x1 * y2 - y1 * x2;

      float dist = sqrt(x * x + y * y + z * z);

      float normX = x / dist;
      float normY = y / dist;
      float normZ = z / dist;

      normals[i][j] = make_tuple(normX, normY, normZ);
    }
  }
}

void setColor(int i, int j) {
  point data1 = colors[i][j];
  point data2 = colors[i+1][j];
  point data3 = colors[i+1][j+1];
  point data4 = colors[i][j+1];

  float r1 = get<0>(data1);
  float g1 = get<1>(data1);
  float b1 = get<2>(data1);
  float r2 = get<0>(data2);
  float g2 = get<1>(data2);
  float b2 = get<2>(data2);
  float r3 = get<0>(data3);
  float g3 = get<1>(data3);
  float b3 = get<2>(data3);
  float r4 = get<0>(data4);
  float g4 = get<1>(data4);
  float b4 = get<2>(data4);

  glColor3f((r1+r2+r3+r4) / 4, (g1+g2+g3+g4)/4, (b1+b2+b3+b4)/4);
}

void drawPoint(int i, int j, bool drawNorm = false) {
  point c = colors[i][j];
  float r = get<0>(c);
  float g = get<1>(c);
  float b = get<2>(c);

  if (i == 0 || i == dim || j == 0 || j == dim || (r == 1.0 && g == 1.0 && b == 1.0)) {
    return;
  }

  point p = points[i][j];
  glVertex3f(get<0>(p),get<1>(p),get<2>(p));

  if (drawNorm) {
    point normal = normals[i][j];
    glNormal3f(get<0>(normal),get<1>(normal),get<2>(normal));
  }
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 'w') {
    rotateY -= 5;
  } else if (key == 's') {
    rotateY += 5;
  } else if (key == 'a') {
    rotateX += 5;
  } else if (key == 'd') {
    rotateX -= 5;
  } else if (key == '1') {
    drawMode = 0;
  } else if (key == '2') {
    drawMode = 1;
  } else if (key == '3') {
    drawMode = 2;
  }
}

void drawPenny() {
  glBegin(GL_LINE_LOOP);
  glColor3f(1,1,1);
  for (float i = 0; i < dim; i++) {
    for (float j = 0; j < dim; j++) {
      drawPoint(i, j);
    }
  }
  glEnd();
}

void drawPennyColor() {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      glBegin(GL_POLYGON);
      setColor(i, j);
      drawPoint(i, j);
      drawPoint(i+1, j);
      drawPoint(i+1, j+1);
      drawPoint(i, j+1);
      glEnd();
    }
  }
}

void drawPennyNormal() {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      glBegin(GL_POLYGON);
      drawPoint(i, j, true);
      drawPoint(i+1, j, true);
      drawPoint(i+1, j+1, true);
      drawPoint(i, j+1, true);
      glEnd();
    }
  }
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  glRotatef(rotateY, 1, 0, 0);
  glRotatef(rotateX, 0, 0, 1);
  switch(drawMode) {
    case 0:
      glDisable(GL_LIGHTING);
      drawPenny();
      break;
    case 1:
      glDisable(GL_LIGHTING);
      drawPennyColor();
      break;
    case 2:
      glEnable(GL_LIGHTING);
      drawPennyNormal();
      break;
  }
  glPopMatrix();
  glFlush();
}

void timer(int p1)
{
  glutPostRedisplay();
  glutTimerFunc(1000 / 60, timer, 0);
}


void glSetup() {
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Penny");

  glutDisplayFunc(draw);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(1000 / 60, timer, 0);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1000.0, 1000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_CULL_FACE);

  // shading
  glShadeModel(GL_SMOOTH);
  init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);
  init_material(Ka, Kd, Ks, 100 * Kp, avgR, avgG, avgB);

  glutMainLoop();
}


int main(int argc, char *argv[]) {

  im_color color;
  im_float depth;
  read_images("gauch-penny.jpg", color, "penny-depth.jpg", depth);

  // draw the window
  glutInit(&argc, argv);
  glSetup();

  return 0;
}