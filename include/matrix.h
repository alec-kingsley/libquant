#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include "mat_t.h"

typedef struct Matrix Matrix;

/**
 * Get the number of columns of the matrix.
 */
size_t matrix_width(Matrix *matrix);

/**
 * Get the number of rows of the matrix.
 */
size_t matrix_height(Matrix *matrix);

/**
 * Set the value in the matrix at row `i`, column `j` to `value`.
 */
void matrix_set(Matrix *matrix, size_t i, size_t j, mat_t value);

/**
 * Get row `i`, column `j` from the matrix.
 */
mat_t matrix_get(Matrix *matrix, size_t i, size_t j);

/**
 * Calculate the determinant of a matrix.
 */
mat_t matrix_determinant(Matrix *matrix);

/**
 * Create an identity matrix.
 * Return NULL on failure.
 */
Matrix *matrix_create_identity(size_t width);

/**
 * Create a Matrix filled with all zeroes.
 * Return NULL on failure.
 */
Matrix *matrix_create(size_t height, size_t width);

/**
 * Destroy the Matrix.
 */
void matrix_destroy(Matrix *matrix);

#endif
