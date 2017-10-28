# CS-430: Project 2
The program raycast Takes in a CSV file containing object data and builds a scene
 based on the shape data within. Outputs a P6 image representing a view of the scene.

Compiles with 'make' while inside the project_2 directory

Usage:$ raycast width height [input file] output.ppm

Known Issues: intersection tests always fail, resulting in a plain black image,
              crashes if width and height are set too high (~400)
