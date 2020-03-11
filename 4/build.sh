set -e

make -C libim all
make -C jpeg all


g++ -Wall penny.cpp libim/libim.a jpeg/libjpeg.a -o penny -lGL -lGLU -lglut
./penny
