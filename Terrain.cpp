/*
 * COMPSCI 3GC3 Assignment 3
 * Author: Lachlan D Whang
 * Student Number: 400287705
 *
 * This file contains the class definitions for the Terrain class.
 */

#include "globals.h"

// Material values for different parts of the scene
float snowAmbient[4] = {1, 1, 1, 1};
float snowDiffuse[4] = {1, 1, 1, 1};
float snowSpecular[4] = {0.1, 0.1, 0.1, 0.5};
float snowShiny = 2;

float grassAmbient[4] = {0, 0.3, 0.2, 1};
float grassDiffuse[4] = {0,0.3,0.2, 1};
float grassSpecular[4] = {0.1, 0.1, 0.1, 0.5};
float grassShiny = 2;

float sandAmbient[4] = {0.52, 0.26, 0.08, 1.0};
float sandDiffuse[4] = {0.52, 0.26, 0.08, 1.0};
float sandSpecular[4] = {0.1, 0.1, 0.1, 0.5};
float sandShiny = 2;

float waterAmbient[4] = {0, 0, 0.8, 1};
float waterDiffuse[4] = {0, 0, 0.8, 1};
float waterSpecular[4] = {0.1, 0.1, 0.1, 0.5};
float waterShiny = 2;

// When dealing with terrain numbers put EVERYTHING in terms of floats
float normals[MAXIMUM_TERRAIN_SIZE][MAXIMUM_TERRAIN_SIZE][3];

Terrain::Terrain(int t_size) {
    if (MINIMUM_TERRAIN_SIZE <= t_size && t_size <= MAXIMUM_TERRAIN_SIZE) terrainSize = t_size;
    else terrainSize = DEFAULT_HEIGHT; // Default initialization
    
    // Initailize the heightmap using the assigned value for terrainSize
    // Heightmap array is ALWAYS a square
    heightMap = new float* [terrainSize];
    for (int i = 0; i < terrainSize; i++) heightMap[i] = new float[terrainSize];
    
    // Reset & fill heightmap
    if (main_algorithm) fill_heightmap();
    else fill_heightmap_2();
}

void Terrain::reset_heightmap() {
    for (int i = 0; i < terrainSize; i++) {
        for (int j = 0; j < terrainSize; j++) heightMap[i][j] = DEFAULT_HEIGHT;
    }
}

// The following is a variant on the algorithm given in
// http://www.lighthouse3d.com/opengl/terrain/index.php?circles
void Terrain::fill_heightmap() {
    // Always reset the heightmap before a refill
    reset_heightmap();

    int i_max = terrainSize * 2; float radius = terrainSize / 10.0; float disp = terrainSize / 10.0;
    
    for (int i = 0; i < i_max; i++) {
        float x_rand = randBetween(0, terrainSize); float z_rand = randBetween(0, terrainSize);

        for (int x = x_rand - radius; x <= x_rand + radius; x++) {
            for (int z = z_rand - radius; z <= z_rand + radius; z++) {
                if (0 <= x && x < terrainSize && 0 <= z && z < terrainSize) {
                    // pd = distance from circle center * 2 / terrainCircleSize
                    float pd = sqrtf(pow(x - x_rand, 2) + pow(z - z_rand, 2)) * 2.0 / radius;
                    // if fabs(pd) <= 1.0
                    if (fabs(pd) <= 1.0) {
                        // height(tx,tz) +=  disp/2 + sin(pd*3.14)*disp/2;
                        // (experimenting with different algorithms to see the result)
                        heightMap[x][z] += disp / 2.0 + sinf(pd * 3.141592) * disp / 2.0;
                    }
                    
                }
            }
        }
    }
    calculate_normals(); // Always regenerate the normals after filling the heightmap
}

void Terrain::calculate_normals() {
    for (int x = 0; x < terrainSize - 1; x++) {
        for (int z = 0; z < terrainSize - 1; z++) {
            
            float t1[3];
            t1[0] = x; t1[1] = heightMap[x][z]; t1[2] = z;
            
            float t3[3];
            t3[0] = x + 1; t3[1] = heightMap[x+1][z]; t3[2] = z;
            
            float t2[3];
            t2[0] = x; t2[1] = heightMap[x][z+1]; t2[2] = z + 1;
            
            // Generate 2 vectors for the cross product
            float v1[3] = {t2[0] - t1[0], t2[1] - t1[1], t2[2] - t1[2]};
            float v2[3] = {t3[0] - t1[0], t3[1] - t1[1], t3[2] - t1[2]};
            
            float vX = v1[1] * v2[2] - v1[2] * v2[1];
            float vY = v1[2] * v2[0] - v1[0] * v2[2];
            float vZ = v1[0] * v2[1] - v1[1] * v2[0];
            
            // Normalize vector (to magnitude 1)
            float magnitude = sqrtf(vX * vX + vY * vY + vZ * vZ);
            float normalized[3] = {vX / magnitude, vY / magnitude, vZ / magnitude};
            
            // Store normal vector
            for (int i = 0; i < 3; i++) normals[x][z][i] = normalized[i];
        }
    }
}

void Terrain::print_heightmap() {
    for (int i = 0; i < terrainSize - 1; i++) {
        for (int j = 0; j < terrainSize - 1; j++) {
            printf("%.2f " , heightMap[i][j]);
        }
        printf("%.2f\n", heightMap[i][terrainSize - 1]);
    }
}

float Terrain::max_height() {
    float max = heightMap[0][0];
    for (int i = 0; i < terrainSize - 1; i++) {
        for (int j = 0; j < terrainSize - 1; j++) {
            if (heightMap[i][j] > max) max = heightMap[i][j];
        }
    }
    return max;
}

float Terrain::min_height() {
    float min = heightMap[0][0];
    for (int i = 0; i < terrainSize - 1; i++) {
        for (int j = 0; j < terrainSize - 1; j++) {
            if (heightMap[i][j] < min) min = heightMap[i][j];
        }
    }
    return min;
}

void Terrain::toggle_drawing_mode() { drawMode = (drawMode + 1) % 3; }

void Terrain::drawing_routine() {
    float color;
    float terrainOffset = terrainSize / 2.0;
    
    for (int x = 0; x < terrainSize - 1; x++) {
        for (int z = 0; z < terrainSize - 1; z++) {
            // Setting materials here if the lighting is activated
            if (lighting && drawMode != 1) {
                if (100 <= heightMap[x][z] && heightMap[x][z] < 105) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, waterAmbient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, waterDiffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, waterSpecular);
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, waterShiny);
                }
                if (105 <= heightMap[x][z] && heightMap[x][z] < 110) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sandAmbient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sandDiffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sandSpecular);
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, sandShiny);
                }
                if (110 <= heightMap[x][z] && heightMap[x][z] < 115) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grassAmbient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grassDiffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grassSpecular);
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, grassShiny);
                }
                if (115 <= heightMap[x][z]) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, snowAmbient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, snowDiffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, snowSpecular);
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, snowShiny);
                }
            }
            
            // Set the current texture here if texturing is activated
            if (textureMode != 0) glBindTexture(GL_TEXTURE_2D, textures.textures[textureMode - 1]);
            
            if (using_quads) glBegin(GL_QUADS);
            else glBegin(GL_TRIANGLES);
                // Vertex 1
                if (drawMode != 1) {
                    color = (float) heightMap[x][z] / max_height();
                    glColor3f(color, color, color);
                }
                glNormal3fv(normals[x][z]);
                glTexCoord2f(0, 0);
                glVertex3f(x - terrainOffset, heightMap[x][z], z - terrainOffset);
                // Vertex 2
                if (drawMode != 1) {
                    color = (float) heightMap[x][z + 1] / max_height();
                    glColor3f(color, color, color);
                }
                glNormal3fv(normals[x][z + 1]);
                glTexCoord2f(0, 1);
                glVertex3f(x - terrainOffset, heightMap[x][z + 1], (z + 1) - terrainOffset);
                // Vertex 3 (not drawn when triangles are being used)
                if (using_quads) {
                    if (drawMode != 1) {
                        color = (float) heightMap[x + 1][z + 1] / max_height();
                        glColor3f(color, color, color);
                    }
                    glNormal3fv(normals[x + 1][z + 1]);
                    glTexCoord2f(1, 1);
                    glVertex3f((x + 1) - terrainOffset, heightMap[x + 1][z + 1], (z + 1) - terrainOffset);
                }
                // Vertex 4
                if (drawMode != 1) {
                    color = (float) heightMap[x + 1][z] / max_height();
                    glColor3f(color, color, color);
                }
                glNormal3fv(normals[x + 1][z]);
                glTexCoord2f(1, 0);
                glVertex3f((x + 1) - terrainOffset, heightMap[x + 1][z], z - terrainOffset);
            glEnd();
            
            // If we are using triangles, we need to draw the second triangle part of the "quad"
            if (!using_quads) {
                glBegin(GL_TRIANGLES);
                    if (drawMode != 1) {
                        color = (float) heightMap[x + 1][z + 1] / max_height();
                        glColor3f(color, color, color);
                    }
                    glNormal3fv(normals[x + 1][z + 1]);
                    glVertex3f((x + 1) - terrainOffset, heightMap[x + 1][z + 1], (z + 1) - terrainOffset);
                
                    if (drawMode != 1) {
                        color = (float) heightMap[x + 1][z] / max_height();
                        glColor3f(color, color, color);
                    }
                    glNormal3fv(normals[x + 1][z]);
                    glVertex3f((x + 1) - terrainOffset, heightMap[x + 1][z], z - terrainOffset);
                
                    if (drawMode != 1) {
                        color = (float) heightMap[x][z + 1] / max_height();
                        glColor3f(color, color, color);
                    }
                    glNormal3fv(normals[x][z + 1]);
                    glVertex3f(x - terrainOffset, heightMap[x][z + 1], (z + 1) - terrainOffset);
                glEnd();
            }
        }
    }
}

void Terrain::draw_terrain() {
    if (drawMode == 0) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawing_routine();
    }
    else if (drawMode == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0, 1, 0);
        drawing_routine();
    }
    else if (drawMode == 2) {
        // Solid part
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawing_routine();
        // Wireframe part
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0, 1, 0);
        drawMode = 1; // We temporarily switch the drawing mode to pain the wireframe in a single color
        drawing_routine();
        drawMode = 2;
    }
}

void Terrain::toggle_texture_mode() { textureMode = (textureMode + 1) % 4; }

void Terrain::fill_heightmap_2() {
    // Always reset the heightmap before a refill
    reset_heightmap();
    
    int i_max = pow(terrainSize, 3) / 1000; float disp = 1.2;
    
    for (int i = 0; i < i_max; i++) {
        int n1 = rand();
        float n2 = sqrtf(2.0 * (float) pow(terrain_size, 2));
        float c = ((double) rand() / RAND_MAX) * n2 - n2 / 2.0;
        
        for (int x = 0; x < terrainSize; x++) {
            for (int z = 0; z < terrainSize; z++) {
                if (sinf(n1) * x + cosf(n1) * z - c < 0) {
                    if (heightMap[x][z] < MAXIMUM_TERRAIN_SIZE) heightMap[x][z] += disp;
                    else heightMap[x][z] = MAXIMUM_TERRAIN_SIZE;
                }
                else {
                    heightMap[x][z] = heightMap[x][z] - disp > 100 ? \
                    heightMap[x][z] -= disp : 100;
                }
            }
        }
    }
    calculate_normals(); // Always regenerate the normals after filling the heightmap
}
