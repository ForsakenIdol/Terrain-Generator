# Random Terrain Generator

To run the program, navigate to the directory containing the source files and type "make" in the terminal / command line.
Alternatively, first type "make main" and then "./main" into the command line.

**Files included with this readme: (all files are required for proper execution, please ensure that all are present)**
1. main.cpp
2. globals.h
3. Terrain.cpp
4. Texture.cpp
5. makefile
6. carpet.ppm
7. marble.ppm
8. lenore.ppm

Currently supported features of this program include:
- Pressing 'q' exits the program.
- Pressing 'w' cycles through the 3 drawing styles: Solid, Wireframe, or both.
- Pressing 'l' activates or deactivates the terrain lights.
- Pressing 'm' toggles between flat or Gouraud shading.
- Pressing 's' toggles between terrain drawing using triangles or quads.
- Pressing 't' toggles between 1 of 3 preloaded textures for the terrain,
  including drawing the terrain with no textures.

Additional features of note:
1. Elevation shading:
   Depending on the height, the terrain will be shaded
   with a different material out of:
   Water (Blue), Sand (Yellow), Grass (Green), and Snow (White).
2. Heightmap algorithm:
   There are 2 algorithms this program uses to generate heightmap values.
   Pressing 'a' changes the particular algorithm being used,
   and regenerates the terrain, using the new algorithm.
