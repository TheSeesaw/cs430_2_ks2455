// struct for vectors and points
typedef struct Vector {
  double x, y, z;
} Vector;

Vector* vector_add(Vector* output, Vector* operand_a, Vector* operand_b);

Vector* vector_sub(Vector* output, Vector* operand_a, Vector* operand_b);

Vector* vector_dot_prod(Vector* output, Vector* operand_a, Vector* operand_b);

Vector* vector_cross_prod(Vector* output, Vector* operand_a, Vector* operand_b);
