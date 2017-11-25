#!/bin/bash
mkdir bin 2> /dev/null
find . -name "*.o" -print0 | xargs -0 rm 2> /dev/null

g++ -c main.cpp Game.cpp Screen.cpp MainMenuScreen.cpp GameCharacter.cpp Map.cpp Tile.cpp -std=c++0x -g -Wall -Wextra -pthread -Wno-missing-field-initializers -I"$SFML_ROOT/include"
g++ -o sfml-app main.o Game.o Screen.o MainMenuScreen.o GameCharacter.o Map.o Tile.o -L"$SFML_ROOT/lib" -lsfml-graphics -lsfml-window -lsfml-system

find . -name "*.o" -print0 | xargs -0 mv -t bin/
mv sfml-app.exe bin/
