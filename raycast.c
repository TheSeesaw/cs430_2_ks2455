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

Pixel *pixel_buffer;
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
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%s", wastebasket); // read past radius identifier
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%d", &shapes_list[obj_index].radius);
	printf("Type: %d\n", shapes_list[obj_index].type);
	printf("R Channel: %lf\n", shapes_list[obj_index].r_color);
	printf("G Channel: %lf\n", shapes_list[obj_index].g_color);
	printf("B Channel: %lf\n", shapes_list[obj_index].b_color);
	printf("X Pos: %d\n", shapes_list[obj_index].x_pos);
	printf("Y Pos: %d\n", shapes_list[obj_index].y_pos);
	printf("Z Pos: %d\n", shapes_list[obj_index].z_pos);
	printf("Radius: %d\n", shapes_list[obj_index].radius);
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


double sphere_intersection(Shape *sphere, int rd_x, int rd_y, int rd_z)
{
	double dot_prod_ray_sphere = 0.0;
	// get the dot product of the raycast ray and the sphere's position, derefernce struct fields
	dot_prod_ray_sphere = (((*sphere).x_pos * rd_x) + ((*sphere).y_pos * rd_y) + ((*sphere).z_pos * rd_z));
	printf("dot product: %lf\n", dot_prod_ray_sphere);
	// a negative product indicates a miss
	if (dot_prod_ray_sphere <= 0)
	{
		return -1.0; // NULL indicates a miss
	}
	// a positive product indicates a possible hit
	rd_x *= dot_prod_ray_sphere; // scale the vector to get point of closest intersection
	rd_y *= dot_prod_ray_sphere;
	rd_z *= dot_prod_ray_sphere;
	// get distance between closest point and sphere center, passing the ray as point a
	// and the sphere's center as point b, pass dereferenced struct fields
	double distance_to_sphere_center = distance_between_positions(rd_x, rd_y, rd_z, (*sphere).x_pos, (*sphere).y_pos, (*sphere).z_pos);
	// if distance to center is less than radius, closest point is on sphere, HIT
	if (distance_to_sphere_center <=  (*sphere).radius)
	{
		// a hit, calculate distance to closest point
		return 1.0;
	}
	else // else distance to center is greater than radius, MISS
	{
		return -1.0; // 0.0 indicates a miss
	}
}

void write_pixels_to_p6(char* outfile, Pixel *pixmap1d, int width,int height,int max_color_value) {
	printf("Converting to P6 . . .\n");
	// initialize file for writing binary data
	FILE* file_handle_out = fopen(outfile, "wb");
	// var to hold magic number for writing purposes
	int t = 6;
	int p = (int)'P';
	// copy header data
	fwrite(&p, sizeof(char), 1, file_handle_out); // write P
	fprintf(file_handle_out, "%d", 6); // write magic number
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fwrite(&hash, sizeof(char), 1, file_handle_out); // write a hash
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", width); // write width
	fwrite(&space, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", height); // write height
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", max_color_value); // write maxval
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	// write each pixel from the raster
	for (int i = 0; i < width*height; i += 1) {
		fwrite(&pixmap1d[i].r, sizeof(char), 1, file_handle_out); // write red value
		//fwrite(&newline, sizeof(char), 1, file_handle_out);
		fwrite(&pixmap1d[i].g, sizeof(char), 1, file_handle_out); // write green value
		//fwrite(&newline, sizeof(char), 1, file_handle_out);
		fwrite(&pixmap1d[i].b, sizeof(char), 1, file_handle_out); // write blue value
		//fwrite(&newline, sizeof(char), 1, file_handle_out);
	}
	// close file
	fclose(file_handle_out);
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
	// variable to capture object type and determine read function
	char object_type[7];
	// begin reading shape data
	// as long as the next read of fscanf returns 1, there is still a valid character to read
	int pass_count = 1;
	while (fscanf(file_handle_in, "%s", object_type) == 1)
	{
		// trim the trailing comma
		object_type[strlen(object_type) - 1] = 0;
		// switch on the type of object
		if (strcmp(object_type, "camera") == 0) {
			printf("Read: Camera\n");
			read_camera_data(file_handle_in);
		}
		else if (strcmp(object_type, "sphere") == 0) {
			printf("Read: Sphere\n");
			read_sphere_data(file_handle_in, shape_count);
			shape_count++;
		}
		else if (strcmp(object_type, "plane") == 0) {
			printf("Read: Plane\n");
			read_plane_data(file_handle_in, shape_count);
			shape_count++;
		}
		else {
			fprintf(stderr, "Error: Invalid object.");
			return 1;
		}
		printf("loop count: %d\n", pass_count);
		pass_count++;
	}
	if (feof(file_handle_in))
	{
		printf("Finished reading file\n");
	}
	else
	{
		fprintf(stderr, "Error: Read interrupted.");
	}
	// done reading data, close in file
	fclose(file_handle_in);
	// allocate memory for pixel buffer
	pixel_buffer = malloc(sizeof(Pixel)*img_width*img_height);
	// initialize Pij vector
	double pij_x, pij_y = 0;
	double pij_z = 1;
	Shape *current_shape;
	// dereference camera attributes for easier reading
	double c_w = (*camera_obj).width;
	double c_h = (*camera_obj).height;
	double ray_length = 0.0;
	// initialize a variable for the shape with the closest intersection point
	int closest_shape_index = -1; // defaults to out of bounds
	// initialize a variable for distance to closest intersection
	double closest_intersect_dis = INFINITY;
	printf("%d width\n%d height\n", img_width, img_height);
	// iterate through all the pixels
	int w_index, h_index, shape_index;
	w_index = 0; // reset width index
	for (; w_index < img_width; w_index++)
	{
		// compute x coord
		pij_x = ((c_w / -2) +
						(w_index * (c_w / img_width)) +
						(0.5 * (c_w / img_width)));
		h_index = 0; // reset height index
		printf("x val: %d, pixel x: %lf\n", w_index, pij_x);
		for (; h_index < img_height; h_index++)
		{
			// compute y coord
			pij_y = ((c_h / -2) +
			        (h_index * (c_h / img_height)) +
							(0.5 * (c_h / img_height)));
			printf("y val: %d, pixel y: %lf\n", h_index, pij_y);
			// normalize Rd
			ray_length = sqrt((pij_x * pij_x) + (pij_y * pij_y) + (pij_z * pij_z));
			pij_x /= ray_length;
			pij_y /= ray_length;
			pij_z /= ray_length;
			closest_shape_index = -1; // reset closest shape index
			closest_intersect_dis = INFINITY; // reset closest intersection distance
			// iterate through the objects list for each pixel
			shape_index = 0; // reset shape index
			for (; shape_index < shape_count; shape_index++)
			{
				printf("checking shape: %d", shape_index);
				// get a pointer to the current shape
				current_shape = &shapes_list[shape_index];
				printf(" type: %d\n", shapes_list[shape_index].type);
				// initialize variable to hold intersection test result, null indicates no intersection
				double intersection_test_result = -1.0; // defaults to a miss
				// initialize variable to hold the distance between closest point and camera
				// check each for an intersection, switching on the type of object
				switch(shapes_list[shape_index].type)
				{
					case 2 :
						intersection_test_result = sphere_intersection(current_shape, pij_x, pij_y, pij_z);
						printf("Intersection result was a: %lf\n", intersection_test_result);
						break;
					case 3 :
						//intersection_test_result = plane_intersection(current_shape, pij_x, pij_y, pij_z);
						break;
					default:
						fprintf(stderr, "Error: Attempted intersection test on unknown object type.");
				}
				// update closest shape
				if (intersection_test_result > 0) // intersection was a hit
				{
					// test most recent shape against current closest shape
					if (intersection_test_result < closest_intersect_dis)
					{
						// update closest shape index and distance
						closest_shape_index = shape_index;
						closest_intersect_dis = intersection_test_result;
					}
				}
			}
			// done with shapes, color pixel based on the results of cloest_shape_index
			if (closest_shape_index == -1) // no intersections, color pixel black
			{
				pixel_buffer[(w_index * img_width) + (h_index * img_width)].r = 0;
				pixel_buffer[(w_index * img_width) + (h_index * img_width)].g = 0;
				pixel_buffer[(w_index * img_width) + (h_index * img_width)].b = 0;
			}
			else // else, color of closest intersected object
			{
				pixel_buffer[(w_index * img_width) + (h_index * img_width)].r = shapes_list[closest_shape_index].r_color;
				pixel_buffer[(w_index * img_width) + (h_index * img_width)].g = shapes_list[closest_shape_index].g_color;
				pixel_buffer[(w_index * img_width) + (h_index * img_width)].b = shapes_list[closest_shape_index].b_color;
			}
		}
	}
	// done coloring pixels, write pixels to a file
	write_pixels_to_p6(argv[4], pixel_buffer, img_width, img_height, 255);
	// close the input file before exiting
	return 0;
}
