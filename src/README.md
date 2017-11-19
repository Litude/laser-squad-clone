Source code, makefiles, etc. here.

To compile:

In Visual Studio:

1. File -> Open -> Folder... (navigate to the /src folder)
2. Choose either x64-Release of x64-Debug as configuration
3. Compile from the CMake menu

If Visual Studio complains about not finding SFML, open CMakeLists.txt and change set(CMAKE_PREFIX_PATH xxx) to SFML folder.
