/*
 * COMPSCI 3GC3 Assignment 3
 * Author: Lachlan D Whang
 * Student Number: 400287705
 */

#include "globals.h"

GLdouble eye[] = { 0, 0, 0 };
GLdouble lookAt[] = { 0, 100, 0 };
GLdouble up[] = { 0, 1, 0 };

int terrain_size;
Terrain t1 = NULL;
bool using_quads = true;
int drawMode = 0;
bool flat_shading = false;
bool main_algorithm = true;

Texture textures;
int textureMode = 0;

bool lighting = true;
GLfloat ambient[2][4] = {
    { 1, 1, 1, 1 },
    { 1, 1, 1, 1 }
};
GLfloat diffuse[2][4] = {
    { 1, 1, 1, 1 },
    { 1, 1, 1, 1 }
};
GLfloat specular[2][4] = {
    { 1, 1, 1, 1 },
    { 1, 1, 1, 1 }
};
GLfloat lightPos[2][4] = {
    {0, 0, 0, 1},
    {0, 0, 0, 1}
};

void init(int argc, char** argv) {
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Assignment 4");
    
    // Seed RNG with the seconds part of the current time
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    srand(timeinfo->tm_sec);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
}

int randBetween(int low, int high) { return (rand() % (high - low + 1) + low); }

void display() {

    // Lighting functions here
    if (lighting) {
        for (unsigned int i = 0; i < 2; i++) {
            glLightfv(GL_LIGHT0 + i, GL_POSITION, lightPos[i]);
            glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambient[i]);
            glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diffuse[i]);
            glLightfv(GL_LIGHT0 + i, GL_SPECULAR, specular[i]);
            
        }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], up[0], up[1], up[2]);
    
    t1.draw_terrain();
    
    // Draw solid spheres at the positions of the lights
    glPushMatrix();
        glTranslatef(lightPos[0][0], lightPos[0][1], lightPos[0][2]);
        glutSolidSphere(5, 10, 10);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(lightPos[1][0], lightPos[1][1], lightPos[1][2]);
        glutSolidSphere(5, 10, 10);
    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    // Window size cannot be less than 200x200
    int minWindowSize = 200;
    if (w < minWindowSize || h < minWindowSize) {
        glutReshapeWindow((w < minWindowSize) ? minWindowSize : w, (h < minWindowSize) ? minWindowSize : h);
    }
    // Window size cannot be greater than 1200x1200
    int maxWindowSize = 1200;
    if (w > maxWindowSize || h > maxWindowSize) {
        glutReshapeWindow((w > maxWindowSize) ? maxWindowSize : w, (h > maxWindowSize) ? maxWindowSize : h);
    }
    else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, w, h);
        gluPerspective(45, w / h, 1, terrain_size * 10);
    }
    glutPostRedisplay();
}

void kbd(unsigned char key, int x, int y) {
    switch(key) {
        case 'q': // 'q' pressed, exit program
        case 'Q':
            exit(0);
            break;
        case 'r': // 'r' pressed, wipe current scene, generate a new terrain, and redraw
        case 'R':
            cout << "Redrawing...\n";
            t1 = Terrain(terrain_size);
            //printf("Max height: %.2f , Min height: %.2f\n" , t1.max_height(), t1.min_height());
            glutPostRedisplay();
            break;
        case 'w': // 'w' pressed, toggle terrain drawing mode
        case 'W':
            cout << "Toggling drawing mode...\n";
            t1.toggle_drawing_mode();
            glutPostRedisplay();
            break;
        case 'l': // 'l' pressed, toggle scene lighting
        case 'L':
            if (!lighting) {
                cout << "Lighting on\n";
                glEnable(GL_LIGHTING);
            }
            else {
                cout << "Lighting off\n";
                glDisable(GL_LIGHTING);
            }
            lighting = !lighting;
            glutPostRedisplay();
            break;
        case 's': // 's' pressed, switch between drawing terrain using triangles or quads
        case 'S':
            if (using_quads) {
                cout << "Drawing terrain using triangles\n";
            }
            else {
                cout << "Drawing terrain using quads\n";
            }
            using_quads = !using_quads;
            glutPostRedisplay();
            break;
        case 'm': // 'm' pressed, toggle between flat and Gouraud shading
        case 'M':
            if (flat_shading) {
                cout << "Using Gouraud shading\n";
                glShadeModel(GL_SMOOTH);
            }
            else {
                cout << "Using flat shading\n";
                glShadeModel(GL_FLAT);
            }
            flat_shading = !flat_shading;
            glutPostRedisplay();
            break;
        case 't': // 't' pressed, toggle between textures (and no texture '0')
        case 'T':
            t1.toggle_texture_mode();
            if (textureMode == 0) {
                cout << "Drawing terrain without textures...\n";
                glDisable(GL_TEXTURE_2D);
            }
            else {
                glEnable(GL_TEXTURE_2D);
                //printf("Drawing terrain using texture %d...\n", textureMode);
            }
            glutPostRedisplay();
            break;
        case 'a': // 'a' pressed, change the map filling algorithm
        case 'A':
            cout << "Changing the algorithm...\n";
            main_algorithm = !main_algorithm;
            t1 = Terrain(terrain_size);
            //printf("Max height: %.2f , Min height: %.2f\n" , t1.max_height(), t1.min_height());
            glutPostRedisplay();
            break;
    }
}

void special(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP: // Up arrow pressed, move camera up 'y' axis
            if (eye[1] < terrain_size + 200) eye[1] += 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // Down arrow pressed, move camera down 'y' axis
            if (eye[1] > 100) eye[1] -= 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // Left arrow pressed, rotate camera anticlockwise
            eye[0] += 5; eye[2] -= 5;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // Right arrow pressed, rotate camera clockwise
            eye[0] -= 5; eye[2] += 5;
            glutPostRedisplay();
            break;
    }
}

void callBackInit() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(kbd);
    glutSpecialFunc(special);
    glutMainLoop();
}

void print_start() { // Prints all the starting information to the console
    cout << "\n\n\
--------------- COMPSCI 3GC3 Assignment 4 ---------------\n\
Author: Lachlan Whang\n\
Mac ID: whangl     Student Number: 400287705\n\
---------------------------------------------------------\n\
Currently supported features of this program include:\n\
~ Pressing 'q' exits the program.\n\
~ Pressing 'w' cycles through the 3 drawing styles: Solid, Wireframe, or both.\n\
~ Pressing 'l' activates or deactivates the terrain lights.\n\
~ Pressing 'm' toggles between flat or Gouraud shading.\n\
~ Pressing 's' toggles between terrain drawing using triangles or quads.\n\
~ Pressing 't' toggles between 1 of 3 preloaded textures for the terrain,\n\
  including drawing the terrain with no textures.\n\
\n\
Program points:\n\
~ The default heightmap filling algorithm is a variant of the circles algorithm given\n\
  on the following page: http://www.lighthouse3d.com/opengl/terrain/index.php?circles\n\
~ With lighting turned off, the terrain will be shaded in greyscale\n\
  with grey shades proportional to terrain height.\n\
~ While this program supports terrain sizes from 50x50 to 300x300,\n\
  larger terrains are more memory intensive to render,\n\
  and terrains larger than 150x150 may be too computationally expensive\n\
  on lower end machines.\n\
~ The 3 preloaded textures were taken from the 'Textures' lab\n\
  for COMPSCI 3GC3. I do not own any of the texture '.ppm' files\n\
  used in this project.\n\
\n\
Additional features of note include:\n\
~ Elevation shading:\n\
  Depending on the height, the terrain will be shaded\n\
  with a different material out of:\n\
  Water (Blue), Sand (Yellow), Grass (Green), and Snow (White).\n\
~ Heightmap algorithm:\n\
  There are 2 algorithms this program uses to generate heightmap values.\n\
  Pressing 'a' changes the particular algorithm being used,\n\
  and regenerates the terrain, using the new algorithm.\n\
\n\
This program assumes a certain degree of user common sense.\n\
Please use reasonable numbers in your specifiers!\n\
\n\n";
}

int main(int argc, char** argv) {
    print_start();
    
    cout << "Enter the size of the terrain, between 50 and 300: ";
    scanf("%d", &terrain_size);
    if (!(MINIMUM_TERRAIN_SIZE <= terrain_size && terrain_size <= MAXIMUM_TERRAIN_SIZE)) {
        cout << "Terrain height not between 50 and 300! Initializing to default size...\n";
        terrain_size = DEFAULT_HEIGHT;
    }
    // Shift camera position so terrain always appears the same size
    eye[0] = eye[2] = terrain_size;
    eye[1] = terrain_size + 100;
    
    init(argc, argv);
    
    textures = Texture();
    glGenTextures(3, textures.textures);
    textures.load_texture("carpet.ppm");
    textures.load_texture("marble.ppm");
    textures.load_texture("lenore.ppm");
    textures.initialize_textures();
    
    t1 = Terrain(terrain_size);
    //printf("Max height: %.2f , Min height: %.2f\n" , t1.max_height(), t1.min_height());
    // Set light positions relative to terrain
    lightPos[0][1] = lightPos[1][1] = t1.max_height() + 50;
    lightPos[0][0] = lightPos[0][2] = terrain_size + 50;
    lightPos[1][0] = lightPos[1][2] = -(terrain_size + 50);
    
    callBackInit();
    
    return 0;
}
