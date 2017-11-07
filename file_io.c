#include <stdlib.h>
#include "file_io.h"

#define MAX_OBJECTS 128

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

// read single camera object from a csv file
void read_camera_data(FILE* file_to_read, Shape *camera) {
	// data should be in this format:
	// camera, width: 0.5, height: 0.5
	char *wastebasket = malloc(10*sizeof(char)); // initialize a junk data variable
  fscanf(file_to_read, "%s", wastebasket); // read past the camera identifier
	camera->type = Camera;
	fscanf(file_to_read, "%s", wastebasket); // read past the width identifier
	if (strcmp(wastebasket, "width:") != 0) { // use it for error checking
		fprintf(stderr, "Error: Invalid camera width format.\n");
    return;
	}
	fscanf(file_to_read, "%lf", &camera->width); // capture the width value
	fscanf(file_to_read, "%s", wastebasket); // read past the trailing comma
	fscanf(file_to_read, "%s", wastebasket); //read past the height identifier
	if (strcmp(wastebasket, "height:") != 0) { // use it for error checking
		fprintf(stderr, "Error: Invalid camera height format.\n");
    return;
	}
	fscanf(file_to_read, "%lf", &camera->height); // capture the height value
	/*
	printf("%d\n", camera->type);
	printf("%lf\n", camera->width);
	printf("%lf\n", camera->height);
	*/
	free(wastebasket);
}

void read_sphere_data(FILE* file_to_read, Shape* output_list, int obj_index) {
	// data should be in this format:
	// sphere, color: [1.0, 0, 0], position: [0, 2, 5], radius: 2
	char *wastebasket = malloc(10*sizeof(char)); // initialize a junk data variable
	output_list[obj_index].type = Sphere;
	fscanf(file_to_read, "%s", wastebasket); // read past color identifier
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%lf", &output_list[obj_index].color_r); // read in red color value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &output_list[obj_index].color_g); // read in green color value
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &output_list[obj_index].color_b); // read in blue color value
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	fscanf(file_to_read, "%s", wastebasket); // read past position identifier
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%lf", &output_list[obj_index].pos_x); // read in x position
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &output_list[obj_index].pos_y); // read in y position
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read);
	fscanf(file_to_read, "%lf", &output_list[obj_index].pos_z); // read in z position
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%s", wastebasket); // read past radius identifier
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%lf", &output_list[obj_index].radius);
	/*
	printf("Type: %d\n", shapes_list[obj_index].type);
	printf("R Channel: %lf\n", shapes_list[obj_index].r_color);
	printf("G Channel: %lf\n", shapes_list[obj_index].g_color);
	printf("B Channel: %lf\n", shapes_list[obj_index].b_color);
	printf("X Pos: %d\n", shapes_list[obj_index].x_pos);
	printf("Y Pos: %d\n", shapes_list[obj_index].y_pos);
	printf("Z Pos: %d\n", shapes_list[obj_index].z_pos);
	printf("Radius: %d\n", shapes_list[obj_index].radius);
  */
	free(wastebasket); // free the junk data pointer
	printf("sphere good\n");
}


/* reads the camera object information, then calls read functions for each Shape
   in the input file
*/
int read_object_file_director(char *in_file_name, Shape *camera, Shape *output_list)
{
  // declare variables
  char type_string[15];
  int object_index = 0;
  // open the file to read binary data
  FILE* file_handle_in = fopen(in_file_name, "rb");
  // read in the camera
  read_camera_data(file_handle_in, camera);
  // read a single sphere
	fscanf(file_handle_in, "%s", type_string); // NOTE: temporary: read past sphere identifier
  read_sphere_data(file_handle_in, output_list, object_index);
  // TODO: read objects
  /*
  while (fscanf(file_handle_in, "%s", type_string) == 1) // next object type stored in type_string
  {
    traverse_whitespace_and_comments(file_handle_in); // reset to start of next line
    type_string[strlen(type_string) - 1] = 0; // trim trailing comma
    // switch on type, pass to read function
    if (strcmp(type_string, "sphere") == 0)
    {
      //read_sphere_data(file_handle_in);
    }
    else if (strcmp(type_string, "plane") == 0)
    {
      //read_plane_data(file_handle_in);
    }
    else
    {
      fprintf(stderr, "Error: invalid shape type.\n");
      return 1;
    }
  }
  */
  fclose(file_handle_in);
	printf("good\n");
  return 0;
}

// writes pixel data to a P6 .ppm file in binary
int write_pixels_to_p6(Pixel *buffer)
{
  return 1;
}
