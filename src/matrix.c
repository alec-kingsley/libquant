#include "matrix.h"

struct Matrix {
  size_t height;
  size_t width;

  mat_t *values;
};

/**
 * Calculate the determinant of a matrix recursively.
 */
static mat_t matrix_determinant_recurse(Matrix *matrix, size_t depth);

size_t matrix_height(Matrix *matrix) {
  return matrix->height;
}

size_t matrix_width(Matrix *matrix) {
  return matrix->width;
}

void matrix_set(Matrix *matrix, size_t i, size_t j, mat_t value) {
  matrix->values[i * matrix->height + j] = value;
}

mat_t matrix_get(Matrix *matrix, size_t i, size_t j) {
  return matrix->values[i * matrix->height+ j];
}

static matrix_determinant_recurse(Matrix *matrix, size_t depth);

mat_t matrix_determinant(Matrix *matrix) {
  return matrix_determinant_recurse(matrix, 0, 0);
}

Matrix *matrix_create_identity(size_t width) {
  size_t i;
  Matrix *matrix = matrix_create(width, width);
  if (matrix == NULL) goto matrix_create_identity_fail;

  for (i = 0; i < width; i++) {
    matrix_set(matrix, i, i, MAT_T_1);
  }

  return matrix;
  matrix_create_identity_fail:
  matrix_destroy(matrix);
  return NULL;
}

Matrix *matrix_create(size_t height, size_t width) {
  Matrix *matrix = calloc(1, sizeof(Matrix));
  if (matrix == NULL) goto matrix_create_fail;

  matrix->values = calloc(height * width, sizeof(mat_t));
  if (matrix->values == NULL) goto matrix_create_fail;

  matrix->height = height;
  matrix->width = width;

  return matrix;
matrix_create_fail:
  matrix_destroy(matrix);
  return NULL;
}

void matrix_destroy(Matrix *matrix) {
  if (matrix != NULL) {
    free(matrix->values);
    free(matrix);
  }
}

