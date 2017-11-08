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
Pixel *pixel_plane = NULL;

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
	pixel_plane = malloc(res_width * res_height * sizeof(Pixel));
  // Construct the view plane coordinates based on image resolution and camera dimensions
	construct_view_plane(view_plane, res_width, res_height, camera->width, camera->height);
	/* print view plane coords
	int total_length = res_width * res_height;
	for (int ind = 0; ind < total_length; ind++)
	{
		printf("Pij #%d (%lf, %lf, %lf)\n", (ind+1), view_plane[ind].x, view_plane[ind].y, view_plane[ind].z);
	}
	*/
	// Perform raycasting
	int view_plane1d_length = res_width * res_height;
	Point *origin = malloc(sizeof(Point));
	origin->x = 0;
	origin->y = 0;
	origin->z = 0;
	double ray_length;
	Vector3d *normalized_ray = malloc(sizeof(Vector3d));
	// Loop through all the points in the view plane
	for (int view_plane_index = 0; view_plane_index < view_plane1d_length; view_plane_index++)
	{
		// normalize the current Pij
		ray_length = distance_between_points(origin, &view_plane[view_plane_index]);
		normalized_ray->x = view_plane[view_plane_index].x / ray_length;
		normalized_ray->y = view_plane[view_plane_index].y / ray_length;
		normalized_ray->z = view_plane[view_plane_index].z / ray_length;
		// TODO: loop through all shapes
		// raytrace for target point in view plane
		intersection_test_director(&shapes_list[0], normalized_ray);
		// Save color result in the pixel_plane
	}
  // Write results
	// Free memory
	free(pixel_plane);
	free(view_plane);
	free(camera);
	free(shapes_list);
	return 0;
}
