#include "matrix.h"
#include "reporter.h"
#include <stdio.h>
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
 * Maintain the same determinant.
 */
static void matrix_triangularize(Matrix *matrix);

/**
 * Subtract multiple * src row from dest row in matrix.
 */
static void matrix_subtract_row(Matrix *matrix, size_t dest_idx, size_t src_idx,
                                mat_t multiple);

/**
 * Swap two rows in matrix.
 */
static void matrix_swap_rows(Matrix *matrix, size_t idx_a, size_t idx_b);

/**
 * Multiply a row by a scalar value.
 */
static void matrix_multiply_row(Matrix *matrix, size_t idx, mat_t scalar);

size_t matrix_height(Matrix *matrix) { return matrix->height; }

size_t matrix_width(Matrix *matrix) { return matrix->width; }

void matrix_set(Matrix *matrix, size_t i, size_t j, mat_t value) {
    if (!i || !j || i > matrix->height || j > matrix->width) {
        report_logic_error("index out of bounds");
    }
    matrix->values[(i - 1) * matrix->height + j - 1] = value;
}

mat_t matrix_get(Matrix *matrix, size_t i, size_t j) {
    if (!i || !j || i > matrix->height || j > matrix->width) {
        report_logic_error("index out of bounds");
    }
    return matrix->values[(i - 1) * matrix->height + j - 1];
}

static void matrix_subtract_row(Matrix *matrix, size_t dest_idx, size_t src_idx,
                                mat_t multiple) {
    const size_t width = matrix_width(matrix);
    mat_t *dest = matrix->values + width * (dest_idx - 1);
    const mat_t *src = matrix->values + width * (src_idx - 1);
    size_t i;

    for (i = 0; i < width; i++) {
        dest[i] = MAT_T_SUB(dest[i], MAT_T_MUL(multiple, src[i]));
    }
}

static void matrix_swap_rows(Matrix *matrix, size_t idx_a, size_t idx_b) {
    const size_t width = matrix_width(matrix);
    mat_t temp;
    mat_t *a = matrix->values + width * (idx_a - 1);
    mat_t *b = matrix->values + width * (idx_b - 1);
    size_t i;

    if (idx_a == idx_b) {
        return;
    }

    for (i = 0; i < width; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

static void matrix_multiply_row(Matrix *matrix, size_t idx, mat_t scalar) {
    const size_t width = matrix_width(matrix);
    mat_t *a = matrix->values + width * (idx - 1);
    size_t i;
    for (i = 0; i < width; i++) {
        a[i] = MAT_T_MUL(a[i], scalar);
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

    for (src_idx = height; src_idx > 1; src_idx--) {
        /* ensure that the src idx row has a non-zero element at its end */
        for (dest_idx = src_idx; dest_idx >= 1; dest_idx--) {
            if (!MAT_T_EQ(MAT_T_0, matrix_get(matrix, dest_idx, src_idx))) {
                if (dest_idx != src_idx) {
                    matrix_swap_rows(matrix, src_idx, dest_idx);
                    /* swapping rows multiplies determinant by -1, so this needs
                     * to be undone */
                    matrix_multiply_row(matrix, src_idx, MAT_T(-1.0));
                }
                break;
            }
        }

        for (dest_idx = 1; dest_idx < src_idx; dest_idx++) {
            /* set desired element of target row to 0 */
            if (!MAT_T_EQ(MAT_T_0, matrix_get(matrix, dest_idx, src_idx))) {
                multiple = MAT_T_DIV(matrix_get(matrix, dest_idx, src_idx),
                                     matrix_get(matrix, src_idx, src_idx));
                matrix_subtract_row(matrix, dest_idx, src_idx, multiple);
            }
        }
    }
}

bool matrix_is_diagonal(Matrix *matrix) {
    const size_t width = matrix_width(matrix);
    const size_t height = matrix_height(matrix);
    bool is_diagonal = true;
    size_t i, j;

    if (height != width) {
        report_logic_error(
            "diagonalization undefined for matrix height != width");
    }

    for (i = 1; i <= width && is_diagonal; i++) {
        for (j = 1; j <= width && is_diagonal; j++) {
            if (i != j && !MAT_T_EQ(MAT_T_0, matrix_get(matrix, i, j))) {
                is_diagonal = false;
            }
        }
    }
    return is_diagonal;
}

void matrix_diagonalize(Matrix *matrix) {
    const size_t height = matrix_height(matrix);
    const size_t width = matrix_width(matrix);
    mat_t multiple;
    size_t dest_idx, src_idx;

    if (width != height) {
        report_logic_error("non-square matrix cannot be diagonalized");
    }

    matrix_triangularize(matrix);

    for (src_idx = height - 1; src_idx > 0; src_idx--) {
        for (dest_idx = src_idx + 1; dest_idx <= height; dest_idx++) {
            /* set desired element of target row to 0 */
            if (!MAT_T_EQ(MAT_T_0, matrix_get(matrix, dest_idx, src_idx))) {
                multiple = MAT_T_DIV(matrix_get(matrix, dest_idx, src_idx),
                                     matrix_get(matrix, src_idx, src_idx));
                matrix_subtract_row(matrix, dest_idx, src_idx, multiple);
            }
        }
    }
}

static Matrix *matrix_clone(Matrix *matrix) {
    const size_t height = matrix_height(matrix);
    const size_t width = matrix_width(matrix);
    Matrix *clone = matrix_create(height, width);
    if (clone == NULL)
        goto matrix_clone_fail;
    memcpy(clone->values, matrix->values, height * width * sizeof(mat_t));
    return clone;
matrix_clone_fail:
    return NULL;
}

mat_t matrix_determinant(Matrix *matrix) {
    mat_t result = MAT_T_1;
    size_t i;
    Matrix *triangle = matrix_clone(matrix);
    if (triangle == NULL)
        goto matrix_determinant_fail;

    if (matrix_width(matrix) != matrix_height(matrix)) {
        report_logic_error("determinant undefined for non-square matrix");
    }

    matrix_triangularize(triangle);

    for (i = 1; i <= matrix_width(triangle); i++) {
        result = MAT_T_MUL(result, matrix_get(triangle, i, i));
    }

    matrix_destroy(triangle);

    return result;
matrix_determinant_fail:
    /* TODO - return some sort of error code */
    return 0;
}

Matrix *matrix_create_identity(size_t width) {
    size_t i;
    Matrix *matrix = matrix_create(width, width);
    if (matrix == NULL)
        goto matrix_create_identity_fail;

    for (i = 1; i <= width; i++) {
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

void matrix_print(Matrix *matrix) {
    const size_t width = matrix_width(matrix);
    const size_t height = matrix_height(matrix);
    size_t i, j;

    printf("┌ ");
    for (i = 0; i < width; i++) {
        printf("      ");
    }
    puts("┐");
    for (i = 1; i <= height; i++) {
        printf("| ");
        for (j = 1; j <= width; j++) {
            /* TODO - ensure equal spacing for these */
            MAT_T_PRINT(matrix_get(matrix, i, j));
            printf(" ");
        }
        puts("|");
    }
    printf("└ ");
    for (i = 0; i < width; i++) {
        printf("      ");
    }
    puts("┘");
}
