#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." OFF) # Makes boolean 'test' available.


# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project(LaserSquad)

# Set version information in a config.h file
set(LaserSquad_VERSION_MAJOR 1)
set(LaserSquad_VERSION_MINOR 0)
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# Define sources and executable
set(EXECUTABLE_NAME "LaserSquad")
set(GAME_SOURCES Game.cpp Game.hpp GameCharacter.cpp GameCharacter.hpp Health.hpp Inventory.cpp Inventory.hpp Item.hpp Grid.cpp Grid.hpp Tile.cpp Tile.hpp TileMap.cpp TileMap.hpp Weapon.cpp Weapon.hpp Screen.cpp Screen.hpp GameScreen.cpp GameScreen.hpp MainMenuScreen.cpp MainMenuScreen.hpp NgMenuScreen.cpp NgMenuScreen.hpp AnimationManager.cpp AnimationManager.hpp Animation.hpp Button.cpp Button.hpp)
add_executable(${EXECUTABLE_NAME} main.cpp ${GAME_SOURCES})


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()



# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
include(CPack)

# Copy needed files to build dir
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/img $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/img
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/font $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/font)


################################
# Testing
################################
if (test)
  set(UNITTESTS_EXECUTABLE_NAME "LaserSquad_UnitTests")

  # force static runtime libraries for msvc builds
  if(MSVC)
    set(variables
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_RELEASE
	  CMAKE_CXX_FLAGS_RELWITHDEBINFO
	  CMAKE_CXX_FLAGS_MINSIZEREL
    )
    foreach(variable ${variables})
	  if(${variable} MATCHES "/MD")
	    string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	  endif()
    endforeach()
  endif()

  if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
  endif (APPLE)

  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(libs/googletest)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  add_executable(${UNITTESTS_EXECUTABLE_NAME} test/test_Grid.cpp test/test_Game.cpp ${GAME_SOURCES})

  # Standard linking to gtest stuff.
  target_link_libraries(${UNITTESTS_EXECUTABLE_NAME} gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(${UNITTESTS_EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable ${UNITTESTS_EXECUTABLE_NAME} to see those specific tests.
  add_test(NAME that-test-I-made COMMAND ${UNITTESTS_EXECUTABLE_NAME})

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  add_test(that-other-test-I-made ${UNITTESTS_EXECUTABLE_NAME})
endif()
