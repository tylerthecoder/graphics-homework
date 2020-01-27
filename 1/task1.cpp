#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
  string filename;
  string outFilename;

  int gridWidth = 500;
  int gridHeight = 500;
  int gridType = 0;

  cout << "Please enter an input file name: " << endl;
  getline(cin, filename);
  cout << "Please enter an output file name: " << endl;
  getline(cin, outFilename);
  cout << "Enter grid type:" << endl
       << "1: bar" << endl
       << "2: dot" << endl
       << "3: line" << endl
       << "4: area" << endl;
  cin >> gridType;

  ifstream infile(filename);
  ofstream outfile(outFilename);

  outfile << "set_color 1.0 1.0 1.0";

  float paddingLeft = 10;
  float paddingBottom = 10;
  // cout the grid lines
  for (float i = 0; i < 10; i++)
  {
    float startX = paddingLeft;
    float endX = gridWidth - paddingLeft;
    float startY = (i / 10.0) * (gridHeight - paddingBottom) + paddingBottom;
    float endY = startY;
    outfile << "draw_line 1 " << startX << " " << startY << " " << endX << " " << endY << endl;
  }
  outfile << "draw_line 1 " << paddingLeft << " " << paddingBottom << " " << paddingLeft << " " << gridHeight - paddingBottom << endl;

  int dataPoint;
  int maxDataPoint = -1;
  vector<int> data;
  while (infile >> dataPoint)
  {
    data.push_back(dataPoint);
    if (dataPoint > maxDataPoint)
    {
      maxDataPoint = dataPoint;
    }
  }

  cout << "Max Data point " << maxDataPoint << endl;

  // scale the data
  for (int i = 0; i < data.size(); i++)
  {
    data[i] = (data[i] * (gridHeight - paddingBottom * 2)) / maxDataPoint;
    cout << data[i] << endl;
  }

  // scale all the data
  int colSpacing = (gridWidth - paddingLeft * 2) / data.size();
  int colWidth = colSpacing / 2;

  cout << "Col Spacing" << colSpacing << endl;
  cout << "Col Width" << colWidth << endl;

  outfile << "set_color 1.0 0.0 0.0";

  switch (gridType)
  {
  case 1:
    for (int i = 0; i < data.size(); i++)
    {
      int startX = i * colSpacing + paddingLeft + colSpacing / 2;
      int startY = paddingBottom;
      int width = colWidth;
      int height = data.at(i);
      outfile << "draw_polygon 4 " << startX << " " << startY << " "
              << startX + width << " " << startY << " "
              << startX + width << " " << startY + height << " "
              << startX << " " << startY + height << endl;
    }
    break;
  case 2:
    for (int i = 0; i < data.size(); i++)
    {
      int x = i * colSpacing + paddingLeft + colSpacing / 2;
      int y = data.at(i);
      outfile << "draw_point 1 " << x << " " << y << endl;
    }
    break;
  case 3:
    for (int i = 1; i < data.size(); i++)
    {
      int startX = (i - 1) * colSpacing + paddingLeft + colSpacing / 2;
      int startY = data.at(i - 1);
      int endX = i * colSpacing + paddingLeft + colSpacing / 2;
      int endY = data.at(i);
      outfile << "draw_line 1 " << startX << " " << startY << " " << endX << " " << endY << endl;
    }
    break;
  case 4: /* Area Graph */
    for (int i = 1; i < data.size(); i++)
    {
      int startX = (i - 1) * colSpacing + paddingLeft + colSpacing / 2;
      int endX = i * colSpacing + paddingLeft + colSpacing / 2;
      int bottomY = paddingBottom;
      int startY = data.at(i - 1);
      int endY = data.at(i);
      outfile << "draw_polygon 4 " << startX << " " << bottomY << " "
              << endX << " " << bottomY << " "
              << endX << " " << endY << " "
              << startX << " " << startY << endl;
    }
    break;
  }
  return 0;
}
