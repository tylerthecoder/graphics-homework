make -C libim all
make -C jpeg all

g++ -Wall -O3 image.cpp libim/libim.a jpeg/libjpeg.a -o image -lGL -lGLU -lglut

./image

g++ -Wall penny.cpp -o penny -lGL -lGLU -lglut
./penny
