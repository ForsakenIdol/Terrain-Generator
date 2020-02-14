/*
 * COMPSCI 3GC3 Assignment 3
 * Author: Lachlan D Whang
 * Student Number: 400287705
 *
 * This file contains all the top level declarations for this project.
 */

#ifndef globals_h
#define globals_h

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <string.h>
#include <time.h>
#include <cstdio>

#define DEFAULT_HEIGHT 100
// Terrain size ranges from 50x50 to 300x300
#define MINIMUM_TERRAIN_SIZE 50
#define MAXIMUM_TERRAIN_SIZE 300

using namespace std;

// Global variables
extern int terrain_size; // Size of the terrain
extern bool lighting; // Controls whether lighting is activated or deactivated in the scene
extern bool using_quads; // Is the terrain drawn using GL_QUADS (true) or using GL_TRIANGLES (false)?
extern int drawMode; // Defines the type of terrain to be drawn, default is 0 (SOLID),
                    // but the other values possible are 1 (WIREFRAME) and 2 (BOTH)
extern int textureMode; // Defines the texture to draw the terrain with. Default is 0 (NO TEXTURE),
                       // while values 1, 2, and 3 define the terrain to be drawn with one of 3
                      // pre-loaded textures.
extern bool main_algorithm; // Defines the algorithm to be used when generating the heightmap.

// Global functions
extern int randBetween(int , int ); // Bounds-inclusive random int function

// Texture class
class Texture {
public:
    // Constructor
    Texture();
    
    // Class Variables
    int numberOfTextures; // Number of textures loaded, initially 0, maximum of 3 textures
    int width; int height; int max; // Class parameters for each texture
    
    GLubyte* texture1;
    int width1; int height1; int max1;
    GLubyte* texture2;
    int width2; int height2; int max2;
    GLubyte* texture3;
    int width3; int height3; int max3;
    
    GLuint textures[3];
    
    // Methods
    
    /* The actual texture loading function to call */
    void load_texture(char* file);
    /* Prepares the 'textures' array */
    void initialize_textures();
    
private:
    /* Loads a PPM texture file; prints a message if 3 textures are already loaded */
    GLubyte* LoadPPM(char* file, int* width, int* height, int* max);
};
extern Texture textures; // The 'textures' object, containing all the textures for the terrain

// Terrain class
class Terrain {
    public:
        // Constructor
        Terrain(int t_size); // Check if 50 >= t_size >= 300 and assign to terrainSize
    
        // Class Variables
        float** heightMap; // A 2D array of height values
        int terrainSize; // This must be between MIN and MAX terrain sizes
    
        // Methods
        void reset_heightmap(); // Resets all heights in the map to DEFAULT_HEIGHT
        void fill_heightmap(); // Fills the heightmap with values based on the circles algorithm
        void calculate_normals(); // Generate & store vertex normals for the heightmap
        void print_heightmap(); // Prints the heightmap to the console
        float max_height(); // Returns the maximum height value in the heightmap
        float min_height(); // Returns the minimum height value in the heightmap
        void toggle_drawing_mode(); // Toggles the drawMode value between 0, 1, or 2
        void draw_terrain(); // Draws the terrrain
        void toggle_texture_mode(); // Toggles the textureMode value between 0, 1, 2, or 3
        void fill_heightmap_2(); // Terrain generation algorithm 2 for heightmap filling
    
    private:
        void drawing_routine(); // The drawing code; modularized and separated
};

#endif /* globals_h */
