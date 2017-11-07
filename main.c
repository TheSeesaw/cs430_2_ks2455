// program takes in a CSV file and creates an image based on the drawn objects
// By: Kristoffer Schindele
#include <stdlib.h>
#include "file_io.h"
#include "raycast.h"

// GLOBAL VARIABLES //
int res_width, res_height;
Shape *camera = NULL;
Shape *shapes_list = NULL;
Point *view_plane = NULL;

// main function for the raycaster performs the following:
//  - reads a csv file for objects in a scene
//  - perform raycasting to build pixel buffer
//  - write pixel buffer to a P6 format .ppm file
int main(int argc, char *argv[]) {
	// argument checking
	if (argc != 5) {
		fprintf(stderr, "Error: Invalid number of arguments.\n");
		return 1;
	}
	else if (argv[3] == NULL) {
		fprintf(stderr, "Error: Problem reading input file.\n");
		return 1;
	}
	else if (argv[4] == NULL) {
		fprintf(stderr, "Error: Problem reading output file.\n");
		return 1;
	}
	shapes_list = malloc(128*sizeof(Shape)); // initialize shapes list
	camera = malloc(sizeof(Shape)); // initialize camera object
  // Read file
	read_object_file_director(argv[3], camera, shapes_list);
	// read width and height
  res_width = atoi(argv[1]);
  res_height = atoi(argv[2]);
	view_plane = malloc(res_width*res_height*sizeof(Point));
  // Perform raycasting
	construct_view_plane(view_plane, res_width, res_height, camera->width, camera->height);
	// print view plane coords
	int total_length = res_width * res_height;
	for (int ind = 0; ind < total_length; ind++)
	{
		printf("Pij #%d (%lf, %lf, %lf)\n", (ind+1), view_plane[ind].x, view_plane[ind].y, view_plane[ind].z);
	}
  // Write results
	// Free memory
	free(view_plane);
	free(camera);
	free(shapes_list);
	return 0;
}
