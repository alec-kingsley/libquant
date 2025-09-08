#include "matrix.h"
#include "reporter.h"
#include <stdlib.h>
#include <string.h>

struct Matrix {
    size_t height;
    size_t width;

    mat_t *values;
};

/**
 * Triangularize a matrix, such that no values are above or to the right of
 * the top-left <-> bottom-right diagonal.
 */
static void matrix_triangularize(Matrix *matrix);

/**
 * Subtract multiple * src row from dest row in matrix.
 */
static void matrix_subtract_row(Matrix *matrix, size_t dest_idx, size_t src_idx,
                                mat_t multiple);

size_t matrix_height(Matrix *matrix) { return matrix->height; }

size_t matrix_width(Matrix *matrix) { return matrix->width; }

void matrix_set(Matrix *matrix, size_t i, size_t j, mat_t value) {
    matrix->values[i * matrix->height + j] = value;
}

mat_t matrix_get(Matrix *matrix, size_t i, size_t j) {
    return matrix->values[i * matrix->height + j];
}

static void matrix_subtract_row(Matrix *matrix, size_t dest_idx, size_t src_idx,
                                mat_t multiple) {
    const size_t width = matrix_width(matrix);
    mat_t *dest = matrix->values + width * dest_idx;
    const mat_t *src = matrix->values + width * src_idx;
    size_t i;

    for (i = 0; i < width; i++) {
        dest[i] -= MAT_T_MUL(multiple, src[i]);
    }
}

static void matrix_triangularize(Matrix *matrix) {
    const size_t height = matrix_height(matrix);
    const size_t width = matrix_width(matrix);
    mat_t multiple;
    size_t dest_idx, src_idx;

    if (width != height) {
        report_logic_error("non-square matrix cannot be triangularized");
    }

    for (src_idx = height - 1; src_idx > 0; src_idx--) {
        for (dest_idx = 0; dest_idx < src_idx; dest_idx++) {
            multiple = MAT_T_DIV(matrix_get(matrix, dest_idx, src_idx),
                                 matrix_get(matrix, src_idx, src_idx));
            matrix_subtract_row(matrix, dest_idx, src_idx, multiple);
        }
    }
}

static Matrix *matrix_clone(Matrix *matrix) {
    const size_t height = matrix_height(matrix);
    const size_t width = matrix_width(matrix);
    Matrix *clone = matrix_create(height, width);
    if (clone == NULL)
        goto matrix_clone_fail;
    memcpy(clone->values, matrix->values, height * width);
    return clone;
matrix_clone_fail:
    return NULL;
}

mat_t matrix_determinant(Matrix *matrix) {
    mat_t result = MAT_T_1;
    size_t i = 0;
    Matrix *triangle = matrix_clone(matrix);
    if (triangle == NULL)
        goto matrix_determinant_fail;

    if (matrix_width(matrix) != matrix_height(matrix)) {
        report_logic_error("determinant undefined for non-square matrix");
    }

    matrix_triangularize(triangle);

    while (i < matrix_width(triangle)) {
        result = MAT_T_MUL(result, matrix_get(triangle, i, i));
    }

    matrix_destroy(triangle);

matrix_determinant_fail:
    /* TODO - return some sort of error code */
    return 0;
}

Matrix *matrix_create_identity(size_t width) {
    size_t i;
    Matrix *matrix = matrix_create(width, width);
    if (matrix == NULL)
        goto matrix_create_identity_fail;

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
    if (matrix == NULL)
        goto matrix_create_fail;

    matrix->values = calloc(height * width, sizeof(mat_t));
    if (matrix->values == NULL)
        goto matrix_create_fail;

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
