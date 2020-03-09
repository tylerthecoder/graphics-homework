#include <iostream>
#include <fstream>
#include <tuple>
#include <GL/glut.h>
using namespace std;

const int xDim = 50;
const int yDim = 50;

tuple<int, int, int> colors[xDim][yDim] = {};
float depth[xDim][yDim] = {};

void readAndPopulate() {
  string colorFilename = "color-data.txt";
  ifstream colorIn(colorFilename);
  int x, y, r, g, b, d;

  while (colorIn >> x >> y >> r >> g >> b) {
    int i = x / 50;
    int j = y / 50;
    colors[i][j] = make_tuple(r, g, b);
  }

  colorIn.close();

  string depthFilename = "depth-data.txt";
  ifstream depthIn(depthFilename);

  int maxDepth = 0;

  while (depthIn >> x >> y >> d) {
    int i = x / 10;
    int j = y / 10;
    cout << "Reading Penny data" << endl;
    cout << x << y << " " << d << endl;
    if (d > maxDepth) {
      maxDepth = d;
    }
    depth[i][j] = d;
  }

  // normalize depth data

  for (int i = 0; i < xDim; i++) {
    for (int j = 0; j < yDim; j++) {
      depth[i][j] = depth[i][j] / maxDepth;
      cout << depth[i][j] << endl;
    }
  }


}



void drawPenny() {
  glPushMatrix();
  glRotatef(50, -1, 0, 1);
   glBegin(GL_LINE_LOOP);//start drawing a line loop
    for (float i = 0; i < xDim; i++) {
      for (float j = 0; j < yDim; j++) {
        float d = depth[(int)i][(int)j];
        float x = (i - xDim / 2) / (float)xDim;
        float y = (j - yDim / 2) / (float)yDim;
        cout << x << " " << y << d <<  " " << endl;
        glVertex3f(x,y,d);//left of window
      }
    }
  glEnd();//end drawing of line loop
  glPopMatrix();
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawPenny();

  glFlush();
}

void glSetup() {
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("Penny");
  glutDisplayFunc(draw);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1000.0, 1000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_CULL_FACE);

  glutMainLoop();
}


int main(int argc, char *argv[]) {
  // read in data form files
  readAndPopulate();

  // build the model

  // draw the window
  glutInit(&argc, argv);
  glSetup();


  return 0;
}