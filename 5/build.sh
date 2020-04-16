set -e

make -C libim all
make -C jpeg all


g++ -Wall cats-and-dogs.cpp libim/libim.a jpeg/libjpeg.a -o catdog -lGL -lGLU -lglut
./catdog
