#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "vector.h" // include vectors for shape positions

// enum for object type
enum ObjType { Camera, Sphere, Plane };
// struct representing pixel
typedef struct Pixel {
	unsigned char r, g, b;
} Pixel;

typedef struct Shape {
	enum ObjType type;
	double width, height, radius;
	//Vector3d *position, *color, *normal;
	double pos_x, pos_y, pos_z, color_r, color_g, color_b, norm_x, norm_y, norm_z;
} Shape;

void traverse_whitespace_and_comments(FILE *file_to_read);

void read_camera_data(FILE *file_to_read, Shape *camera);

void read_sphere_data(FILE *file_to_read, Shape *output_list, int obj_index);

int read_object_file_director(char *in_file_name, Shape *camera, Shape *output_list);

int write_pixels_to_p6(Pixel *buffer, char *out_file_name, int res_width, int res_height);
