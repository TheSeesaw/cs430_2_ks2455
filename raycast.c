// program takes in a CSV file and creates an image based on the drawn objects
// By: Kristoffer Schindele
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// STRUCT DEFINITIONS

// struct representing pixel
typedef struct Pixel {
	unsigned char r, g, b;
} Pixel;

// struct representing a shape
// NOTE: all shapes share the same definition,
// but some fields are unused
typedef struct Shape {
	int type; // 1 = camera, 2 = sphere, 3 = plane
  int x_pos, y_pos, z_pos;
	double color_arr[3]; // 0 = r, 1 = g, 2 = b
  int radius;
  // TODO: implement plane primitives
} Shape;


// GLOBAL VARIABLES

Pixel *pixmap1d;
Shape *shapes_list;
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

}

void read_sphere_data(FILE* file_to_read) {

}

void read_plane_data(FILE* file_to_read) {
	
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
	else if (argv[3] == NULL || argv[4] == NULL) {
		fprintf(stderr, "Error reading or writing file.");
		return 1;
	}

	// always read binary
	FILE* file_handle_in = fopen(argv[3], "rb"); // open file for reading
	// capture width and height values
	int img_width = (int)argv[1];
	int img_height = (int)argv[2];
	// allocate memory for pixel buffer
	pixmap1d = malloc(sizeof(Pixel)*img_width*img_height);
	// allocate memory for shapes array, maximum possible shapes = 128
	shapes_list = malloc(sizeof(Shape)*128);
	// variable to check for end of file
	unsigned char eof_check;
	// working variables for each read pass
	unsigned char *object_type = malloc(7*sizeof(char));
	double width, height;
	double colors_arr[3];
	int position_arr[3];
	int radius;
	int normal_arr[3];
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
		// switch on the type of object
		switch (object_type) {
			case 'camera' :
				break;
			case 'sphere' :
				break;
			case 'plane' :
				break;
			default:
				fprintf(stderr, "Error: Invalid object.");
				return 1;
		}
	}

	// skip any whitespace before the start of the line
	traverse_whitespace_and_comments(file_handle_in);

	// close the input file before exiting
	fclose(file_handle_in);
	return 0;
}
