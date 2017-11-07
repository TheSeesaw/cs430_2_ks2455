#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "raycast.h"

// TODO
void construct_view_plane(Point *view_plane1d, double res_width, double res_height, double cam_width, double cam_height)
{
  int index1d = 0;
  double pij_x = 0;
  double pij_y = 0;
  double pij_z = -1;
  // derive the Pij
  for (int i = 0 ; i < res_height; i++)
  {
    // calculate y coord
    pij_y = ((-0.5 * cam_height) + (i * (cam_height / res_height)) + (0.5 * (cam_height / res_height)));
    for (int j = 0; j < res_width; j++)
    {
      index1d = ((i * res_width) + j);
      // calculate x coord
      pij_x = ((-0.5 * cam_width) + (j * (cam_width / res_width)) + (0.5 * (cam_width / res_width)));
      // assign coordinates to view plane
      view_plane1d[index1d].x = pij_x;
      view_plane1d[index1d].y = pij_y;
      view_plane1d[index1d].z = pij_z;
    }
  }
}
