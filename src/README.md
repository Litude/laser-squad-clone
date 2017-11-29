# LaserSquad

Source code, makefiles, etc. here.

## To compile:

In Visual Studio:

1. File -> Open -> Folder... (navigate to the /src folder)
2. Choose either x64-Release of x64-Debug as configuration
3. Compile from the CMake menu

## Tests
This project uses Google Tests framework for unit testing.
Add variable 'test' and set it ON when building cmake `cmake -Dtest=ON` to build unit tests.
This creates unit test executable called "LaserSquad_UnitTests" that can be run.