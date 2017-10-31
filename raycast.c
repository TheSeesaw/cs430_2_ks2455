// program takes in a CSV file and creates an image based on the drawn objects
// By: Kristoffer Schindele
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


// STRUCT DEFINITIONS

// struct representing pixel
typedef struct Pixel {
	unsigned char r, g, b;
} Pixel;

// struct representing a shape
// NOTE: all shapes share the same definition,
// but some fields are unused
typedef struct Shape {
	int type, x_pos, y_pos, z_pos, radius, a_norm, b_norm, c_norm; // 1 = camera, 2 = sphere, 3 = plane
	double width, height, r_color, g_color, b_color;
} Shape;
