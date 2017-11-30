# Laser Squad

C++ programming project that uses SFML library for rendering and user inputs.

| Directory| Description |
| ------ | ------ |
| `plan/`| Here is the project plan as a pdf file. |
| `doc/` | Here are the documentation sources related to the project as a pdf file. |
| `src/` | Here are the C++ source files and CMake project files. |

## To compile:
This project uses CMake to generate the build files. Make sure you have set SFML_ROOT variable in CMake to point to your local installation path of the SFML library.
```
cd laser-squad-2
mkdir build
cd build
cmake ../src
```
After this you can compile and run the application depending whether you have Linux (MakeFile), Windows (Visual Studio) or Mac (Xcode).
Compile the project 'LaserSquad' after running CMake and run.

## Unit tests
This project uses Google Tests framework for unit testing. Add variable 'test' and set it ON when building with cmake `cmake -Dtest=ON` to build unit tests.
This creates unit test project called "LaserSquad_UnitTests" that can be run after compiling.