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


// GLOBAL VARIABLES

Pixel *pixmap1d;
Shape *shapes_list;
Shape *camera_obj;
unsigned char *wastebasket;
unsigned char space = ' ';
unsigned char newline = '\n';
unsigned char hash = '#';


// FUNCTION DECLARATIONS

// Function traverses white space until a character that is not a space, newline,
// or a comment line
// Params - file_to_read: a pointer to a file handle to be read
// Returns - NOTHING
void traverse_whitespace_and_comments(FILE* file_to_read) {
	// Initialize with the first character in the file
	int current_char = fgetc(file_to_read);
	// Traverse the file, ignoring whitespace and newlines
	while (current_char == ' ' || current_char == '\n' || current_char == '#') {
		// If hash, skip the entire line
		if (current_char == '#') {
			current_char = fgetc(file_to_read); // Initialize loop with next char
			while (current_char != '\n') { // While not newline
				current_char = fgetc(file_to_read);
				// current_char now holds newline, break out of loop
			}
		}
		current_char = fgetc(file_to_read); // Get the next char
	}
	// Unget the last non-whitespace character
	ungetc(current_char, file_to_read);
}

void read_camera_data(FILE* file_to_read) {
	// data should be in this format:
	// camera, width: 0.5, height: 0.5
	wastebasket = malloc(10*sizeof(char)); // initialize a junk data variable
	(*camera_obj).type = 1;
	fscanf(file_to_read, "%s", wastebasket); // read past the width identifier
	if (strcmp(wastebasket, "width:") != 0) { // use it for error checking
		fprintf(stderr, "Error: Invalid camera width format");
	}
	fscanf(file_to_read, "%lf", &(*camera_obj).width); // capture the width value
	fscanf(file_to_read, "%s", wastebasket); // read past the trailing comma
	fscanf(file_to_read, "%s", wastebasket); //read past the height identifier
	if (strcmp(wastebasket, "height:") != 0) { // use it for error checking
		fprintf(stderr, "Error: Invalid camera height format");
	}
	fscanf(file_to_read, "%lf", &(*camera_obj).height); // capture the height value
	/*
	printf("%d\n", (*camera_obj).type);
	printf("%lf\n", (*camera_obj).width);
	printf("%lf\n", (*camera_obj).height);
	*/
	free(wastebasket); // free the junk data pointer
}

void read_sphere_data(FILE* file_to_read, int obj_index) {
	// data should be in this format:
	// sphere, color: [1.0, 0, 0], position: [0, 2, 5], radius: 2
	wastebasket = malloc(10*sizeof(char)); // initialize a junk data variable
	shapes_list[obj_index].type = 2;
	fscanf(file_to_read, "%s", wastebasket); // read past color identifier
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%lf", &shapes_list[obj_index].r_color); // read in red color value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &shapes_list[obj_index].g_color); // read in green color value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &shapes_list[obj_index].b_color); // read in blue color value
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	fscanf(file_to_read, "%s", wastebasket); // read past position identifier
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%d", &shapes_list[obj_index].x_pos); // read in x position
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%d", &shapes_list[obj_index].y_pos); // read in y position
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read);
	fscanf(file_to_read, "%d", &shapes_list[obj_index].z_pos); // read in z position
	shapes_list[obj_index].z_pos *= -1;
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%s", wastebasket); // read past radius identifier
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%d", &shapes_list[obj_index].radius);
	/*
	printf("%d\n", shapes_list[obj_index].type);
	printf("%lf\n", shapes_list[obj_index].r_color);
	printf("%lf\n", shapes_list[obj_index].g_color);
	printf("%lf\n", shapes_list[obj_index].b_color);
	printf("%d\n", shapes_list[obj_index].x_pos);
	printf("%d\n", shapes_list[obj_index].y_pos);
	printf("%d\n", shapes_list[obj_index].z_pos);
	printf("%d\n", shapes_list[obj_index].radius);
	*/
	free(wastebasket); // free the junk data pointer
}

void read_plane_data(FILE* file_to_read, int obj_index) {
	// data should be in this format:
	// plane, color: [0, 0, 1.0], position: [0, 0, 0], normal: [0, 1, 0]
	wastebasket = malloc(10*sizeof(char)); // initialize a junk data variable
	shapes_list[obj_index].type = 3;
	fscanf(file_to_read, "%s", wastebasket); // read past color identifier
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%lf", &shapes_list[obj_index].r_color); // read in red color value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &shapes_list[obj_index].g_color); // read in green color value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &shapes_list[obj_index].b_color); // read in blue color value
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	fscanf(file_to_read, "%s", wastebasket); // read past position identifier
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%d", &shapes_list[obj_index].x_pos); // read in x position
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%d", &shapes_list[obj_index].y_pos); // read in y position
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read);
	fscanf(file_to_read, "%d", &shapes_list[obj_index].z_pos); // read in z position
	shapes_list[obj_index].z_pos *= -1; // right hand coordinate system
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%s", wastebasket); // read past radius identifier
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%d", &shapes_list[obj_index].a_norm); // read in first normal value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%d", &shapes_list[obj_index].b_norm); // read in second normal value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%d", &shapes_list[obj_index].c_norm); // read in third normal value
	fgetc(file_to_read); // skip over right bracket
	/*
	printf("%d\n", shapes_list[obj_index].type);
	printf("%lf\n", shapes_list[obj_index].r_color);
	printf("%lf\n", shapes_list[obj_index].g_color);
	printf("%lf\n", shapes_list[obj_index].b_color);
	printf("%d\n", shapes_list[obj_index].x_pos);
	printf("%d\n", shapes_list[obj_index].y_pos);
	printf("%d\n", shapes_list[obj_index].z_pos);
	printf("%d\n", shapes_list[obj_index].a_norm);
	printf("%d\n", shapes_list[obj_index].b_norm);
	printf("%d\n", shapes_list[obj_index].c_norm);
	*/
	free(wastebasket); // free the junk data pointer
}

// calculates the distance between to points in space
double distance_between_positions(int a_x, int a_y, int a_z, int b_x, int b_y, int b_z)
{
	// return the distance formula applied to points
  return sqrt(pow(a_x - b_x, 2) + pow(a_y - b_y, 2) + pow(a_z - b_z, 2));
}


int sphere_intersection(Shape *shape, int rd_x, int rd_y, int rd_z)
{
	double dot_prod_ray_sphere;
	// get the dot product of the raycast ray and the sphere's position, derefernce struct fields
	dot_prod_ray_sphere = (((*shape).x_pos * rd_x) + ((*shape).y_pos * rd_y) + ((*shape).z_pos * rd_z));
	// a negative product indicates a miss
	if (dot_prod_ray_sphere <= 0)
	{
		return 0; // 0 indicates a miss
	}
	// a positive product indicates a possible hit
	rd_x *= dot_prod_ray_sphere; // scale the vector to get point of closest intersection
	rd_y *= dot_prod_ray_sphere;
	rd_z *= dot_prod_ray_sphere;
	// get distance between closest point and sphere center, passing the ray as point a
	// and the sphere's center as point b, pass dereferenced struct fields
	double distance_to_sphere_center = distance_between_positions(rd_x, rd_y, rd_z, (*shape).x_pos, (*shape).y_pos, (*shape).z_pos);

}


// MAIN FUNCTION

// Main function performs error checking, reads in data from scene file, and
// passes scene object to a file writer.
int main(int argc, char *argv[]) {
	// argument checking
	if (argc != 5) {
		fprintf(stderr, "Error: Invalid number of arguments.");
		return 1;
	}
	else if (argv[3] == NULL) {
		fprintf(stderr, "Error: Problem reading input file.");
		return 1;
	}
	else if (argv[4] == NULL) {
		fprintf(stderr, "Error: Problem reading output file.");
		return 1;
	}

	// always read binary
	FILE* file_handle_in = fopen(argv[3], "rb"); // open file for reading
	// capture width and height values
	int img_width = atoi(argv[1]);
	int img_height = atoi(argv[2]);
	// allocate memory for shapes array, maximum possible shapes = 128
	shapes_list = malloc(sizeof(Shape)*128);
	// allocate memory for the camera object
	camera_obj = malloc(sizeof(Shape));
	// initialize a shape object counter
	int shape_count = 0;
	// variable to check for end of file
	unsigned char eof_check;
	// variable to capture object type and determine read function
	unsigned char *object_type = malloc(7*sizeof(char));
	// begin reading shape data
	// as long as the next read of fscanf returns 1, there is still a valid character to read
	while ((int)fscanf(file_handle_in, "%c", &eof_check) == 1)
	{
		// back up one character
		ungetc(eof_check, file_handle_in);
		// read the type of object
		fscanf(file_handle_in, "%s", object_type);
		// trim the trailing comma
		object_type[strlen(object_type) - 1] = 0;
		printf("%s\n", object_type);
		// switch on the type of object
		if (strcmp(object_type, "camera") == 0) {
			printf("read: camera\n");
			read_camera_data(file_handle_in);
		}
		else if (strcmp(object_type, "sphere") == 0) {
			printf("read: sphere\n");
			read_sphere_data(file_handle_in, shape_count);
			shape_count++;
		}
		else if (strcmp(object_type, "plane") == 0) {
			printf("read: plane\n");
			read_plane_data(file_handle_in, shape_count);
			shape_count++;
		}
		else {
			fprintf(stderr, "Error: Invalid object.");
			return 1;
		}
	}
	// allocate memory for pixel buffer
	pixmap1d = malloc(sizeof(Pixel)*img_width*img_height);
	// initialize Pij vector
	double pij_x, pij_y = 0;
	double pij_z = -1;
	Shape *current_shape;
	// dereference camera attributes for easier reading
	double c_w = (*camera_obj).width;
	double c_h = (*camera_obj).height;
	// iterate through all the pixels
	int w_index, h_index, shape_index = 0;
	for (; w_index < img_width; w_index++)
	{
		// compute x coord
		pij_x = ((c_w / -2) +
						(w_index * (c_w / img_width)) +
						(0.5 * (c_w / img_width)));
		for (; h_index < img_height; h_index++)
		{
			// compute y coord
			pij_y = ((c_h / -2) +
			        (h_index * (c_h / img_height)) +
							(0.5 * (c_h / img_height)));

			// normalize Rd
			double ray_length = sqrt((pij_x * pij_x) + (pij_y * pij_y) + (pij_z * pij_z));
			pij_x /= ray_length;
			pij_y /= ray_length;
			pij_z /= ray_length;
			// iterate through the objects list
			for (; shape_index < shape_count; shape_index++)
			{
				// get a pointer to the current shape
				current_shape = &shapes_list[shape_index];
				// check each for an intersection, switching on the type of object
				switch(shapes_list[shape_index].type)
				{
					case 1 :
						sphere_intersection(current_shape, pij_x, pij_y, pij_z);
						break;
					case 2 :
						break;
					case 3 :
						break;
					default:
						fprintf(stderr, "Error: Attempted intersection test on unknown object type.");
				}
			}
		}
	}

	// close the input file before exiting
	fclose(file_handle_in);
	return 0;
}
