#!/bin/bash
mkdir bin 2> /dev/null
find . -name "*.o" -print0 | xargs -0 rm 2> /dev/null

g++ -c main.cpp Game.cpp Screen.cpp GameScreen.cpp MainMenuScreen.cpp TileMap.cpp GameCharacter.cpp Grid.cpp Tile.cpp -std=c++0x -g -Wall -Wextra -pthread -Wno-missing-field-initializers -I"$SFML_ROOT/include"
g++ -o sfml-app main.o Game.o Screen.o GameScreen.o MainMenuScreen.o TileMap.o GameCharacter.o Grid.o Tile.o -L"$SFML_ROOT/lib" -lsfml-graphics -lsfml-window -lsfml-system

find . -name "*.o" -print0 | xargs -0 mv -t bin/
mv sfml-app.exe bin/
