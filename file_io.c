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
	output_list[obj_index].pos_z *= -1;
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%s", wastebasket); // read past radius identifier
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%lf", &output_list[obj_index].radius);
	/*
	printf("Type: %d\n", output_list[obj_index].type);
	printf("R Channel: %lf\n", output_list[obj_index].color_r);
	printf("G Channel: %lf\n", output_list[obj_index].color_g);
	printf("B Channel: %lf\n", output_list[obj_index].color_b);
	printf("X Pos: %lf\n", output_list[obj_index].pos_x);
	printf("Y Pos: %lf\n", output_list[obj_index].pos_y);
	printf("Z Pos: %lf\n", output_list[obj_index].pos_z);
	printf("Radius: %lf\n", output_list[obj_index].radius);
  */
	free(wastebasket); // free the junk data pointer
}

void read_plane_data(FILE* file_to_read, Shape* output_list, int obj_index)
{
	// data should be in this format:
	// sphere, color: [1.0, 0, 0], position: [0, 2, 5], normal: [0, 1, 0]
	char *wastebasket = malloc(10*sizeof(char)); // initialize a junk data variable
	output_list[obj_index].type = Plane;
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
	output_list[obj_index].pos_z *= -1;
	fgetc(file_to_read); // skip over right bracket
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fscanf(file_to_read, "%s", wastebasket); // read past normal identifier
	traverse_whitespace_and_comments(file_to_read); // skip spaces
	fgetc(file_to_read); // skip over left bracket
	fscanf(file_to_read, "%lf", &output_list[obj_index].norm_x); // read in x normal
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read); // skip over spaces
	fscanf(file_to_read, "%lf", &output_list[obj_index].norm_y); // read in y normal
	fgetc(file_to_read); // skip over comma
	traverse_whitespace_and_comments(file_to_read);
	fscanf(file_to_read, "%lf", &output_list[obj_index].norm_z); // read in z normal
	fgetc(file_to_read); // skip over right bracket
	/*
	printf("Type: %d\n", output_list[obj_index].type);
	printf("R Channel: %lf\n", output_list[obj_index].color_r);
	printf("G Channel: %lf\n", output_list[obj_index].color_g);
	printf("B Channel: %lf\n", output_list[obj_index].color_b);
	printf("X Pos: %lf\n", output_list[obj_index].pos_x);
	printf("Y Pos: %lf\n", output_list[obj_index].pos_y);
	printf("Z Pos: %lf\n", output_list[obj_index].pos_z);
	printf("X Norm: %lf\n", output_list[obj_index].norm_x);
	printf("Y Norm: %lf\n", output_list[obj_index].norm_y);
	printf("Z Norm: %lf\n", output_list[obj_index].norm_z);
	*/
	free(wastebasket); // free the junk data pointer
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
  // read objects
  while (fscanf(file_handle_in, "%s", type_string) == 1) // next object type stored in type_string
  {
    traverse_whitespace_and_comments(file_handle_in); // reset to start of next line
    type_string[strlen(type_string) - 1] = 0; // trim trailing comma
    // switch on type, pass to read function
    if (strcmp(type_string, "sphere") == 0)
    {
      read_sphere_data(file_handle_in, output_list, object_index);
    }
    else if (strcmp(type_string, "plane") == 0)
    {
      read_plane_data(file_handle_in, output_list, object_index);
    }
    else
    {
      fprintf(stderr, "Error: invalid shape type.\n");
      return -1;
    }
		object_index++;
  }
  fclose(file_handle_in);
  return object_index;
}

// writes pixel data to a P6 .ppm file in binary
int write_pixels_to_p6(Pixel *buffer, char *out_file_name, int res_width, int res_height)
{
	// open a new file for writing binary
	FILE* file_handle_out = fopen(out_file_name, "wb");
	// initialize writing variables
	unsigned char space = ' ';
	unsigned char newline = '\n';
	unsigned char hash = '#';
	unsigned char capital_p = 'P';
	// write header data
	fwrite(&capital_p, sizeof(char), 1, file_handle_out); // write P
	fprintf(file_handle_out, "%d", 6); // write magic number
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fwrite(&hash, sizeof(char), 1, file_handle_out); // write a hash
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", res_width); // write width
	fwrite(&space, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", res_height); // write height
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	fprintf(file_handle_out, "%d", 255); // write maxval
	fwrite(&newline, sizeof(char), 1, file_handle_out);
	// write pixel data from buffer
	for (int i = 0; i < res_width*res_height; i += 1) {
		fwrite(&buffer[i].r, sizeof(char), 1, file_handle_out); // write red value
		fwrite(&buffer[i].g, sizeof(char), 1, file_handle_out); // write green value
		fwrite(&buffer[i].b, sizeof(char), 1, file_handle_out); // write blue value
	}
	// close file
	fclose(file_handle_out);
  return 0;
}
