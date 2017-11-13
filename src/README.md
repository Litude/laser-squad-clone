Source code, makefiles, etc. here.

To compile:

g++ -c main.cpp -I "sfml-install-path"/include

g++ main.o -o sfml-app -L "sfml-install-path"/lib -lsfml-graphics -lsfml-window -lsfml-system
