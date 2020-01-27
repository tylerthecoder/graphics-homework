//---------------------------------------
// Program: square.cpp
// Purpose: Demonstrate basic OpenGL commands
//          to display two squares.
// Author:  John Gauch
// Date:    September 2008
//---------------------------------------
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
using namespace std;

void drawLine(int width, int x1, int y1, int x2, int y2)
{
  cout << "Draw Line " << x1 << " " << x2 << " " << y1 << " " << y2 << endl;
  glLineWidth(width);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void drawPoint(int width, int x, int y)
{
  cout << "Draw point " << width << " " << x << " " << y << endl;
  glPointSize(2.0);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
}

void drawPoly(vector<int> points)
{
  cout << "Draw polygon of size: " << points.size() << endl;
  glBegin(GL_POLYGON);
  for (int i = 0; i < points.size() / 2; i++)
  {
    int data1 = points.at(i * 2);
    int data2 = points.at((i * 2) + 1);
    glVertex2f(data1, data2);
  }
  glEnd();
}

void setColor(float r, float g, float b)
{
  glColor3f(r, g, b);
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 500, 0, 500, 0.0, 1.0);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  // read in file
  string filename = "command.txt";
  ifstream din(filename);

  string line;
  string command;

  while (din >> command)
  {
    if (command == "draw_line")
    {
      int width, x1, y1, x2, y2;
      din >> width >> x1 >> y1 >> x2 >> y2;
      drawLine(width, x1, y1, x2, y2);
    }
    else if (command == "draw_point")
    {
      int width, x, y;
      din >> width >> x >> y;
      drawPoint(width, x, y);
    }
    else if (command == "draw_polygon")
    {
      int n;
      din >> n;
      vector<int> points;
      int numOfDataPoints = n * 2;
      for (int i = 0; i < numOfDataPoints; i++)
      {
        din >> n;
        points.push_back(n);
      }
      drawPoly(points);
    }
    else if (command == "set_color")
    {
      float r, g, b;
      din >> r >> g >> b;
      setColor(r, g, b);
    }
  }

  glFlush();
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
  glutDisplayFunc(display);
  init();

  // read in file

  glutMainLoop();
  return 0;
}
