set -e

g++ -Wall task1.cpp -o task1 -lGL -lGLU -lglut
g++ -Wall task2.cpp -o task2 -lGL -lGLU -lglut

./task1 < input.txt
./task2
