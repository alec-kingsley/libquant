#include <stdio.h>
#include <stdbool.h>
#include "matrix.h"
#include "colors.h"

/**
 * Assert two `mat_t` values are equal, and print a relevant status message.
 */
int mat_t_assert_equal(mat_t a, mat_t b);

/**
 * Assert two `size_t` values are equal, and print a relevant status message.
 */
int size_t_assert_equal(size_t a, size_t b);

/**
 * Assert two matrices are equal, and print a relevant status message.
 */
int matrix_assert_equal(Matrix *a, Matrix *b);

int mat_t_assert_equal(mat_t a, mat_t b) {
    bool success = MAT_T_EQ(a, b);
    if (success) {
        printf(GREEN "Success" RESET "\n");
    } else {
        printf(RED "Failure: ");
        MAT_T_PRINT(a);
        printf(" != ");
        MAT_T_PRINT(b);
        printf(RESET "\n");
    }
    return success ? 0 : -1;
}

int size_t_assert_equal(size_t a, size_t b) {
    bool success = a == b;
    if (success) {
        printf(GREEN "Success" RESET "\n");
    } else {
        printf(RED "Failure: %li != %li" RESET "\n", a, b);
    }
    return success ? 0 : -1;
}

int matrix_assert_equal(Matrix *a, Matrix *b) {
    bool success = true;
    size_t i, j;

    if (matrix_width(a) != matrix_width(b)) {
        success = false;
        printf(RED "Failure: widths differ" RESET "\n");
    } else if (matrix_height(a) != matrix_height(b)) {
        success = false;
        printf(RED "Failure: heights differ" RESET "\n");
    } else {
        for (i = 0; i < matrix_height(a) && success; i++) {
            for (j = 0; j < matrix_width(a) && success; j++) {
                if (!MAT_T_EQ(matrix_get(a, i, j), matrix_get(b, i, j))) {
                    success = false;
                    printf(RED "Failure: matrices differ at (%li, %li)" RESET "\n", i, j);
                }
            }
        }
    }
    if (success) {
        printf(GREEN "Success" RESET "\n");
    }
    return success ? 0 : -1;
}

/**
 * Test `matrix_width`.
 * Return # of failed test cases.
 */
int test_matrix_width(void);

/**
 * Test `matrix_height`.
 * Return # of failed test cases.
 */
int test_matrix_height(void);

/**
 * Test `matrix_create_identity`.
 * Return # of failed test cases.
 */
int test_matrix_create_identity(void);

/**
 * Test `matrix_create`.
 * Return # of failed test cases.
 */
int test_matrix_create(void);

int test_matrix_width(void) {
    const int test_ct = 2;
    int tests_left = test_ct;
    int tests_failed = 0;
    Matrix *matrix;

    printf("Testing: matrix_width\n");

    printf("  1x1 matrix_width test: ");
    matrix = matrix_create(1, 1);
    if (matrix == NULL) goto test_matrix_width_skip_remaining_tests;
    tests_failed += size_t_assert_equal(1, matrix_width(matrix)) != 0 ? 1 : 0;
    tests_left--;
    matrix_destroy(matrix);

    printf("  1x2 matrix_width test: ");
    matrix = matrix_create(1, 2);
    if (matrix == NULL) goto test_matrix_width_skip_remaining_tests;
    tests_failed += size_t_assert_equal(2, matrix_width(matrix)) != 0 ? 1 : 0;
    tests_left--;
    matrix_destroy(matrix);

test_matrix_width_skip_remaining_tests:
    printf("Failed: %i\n", tests_failed);
    printf("Succeeded: %i\n", test_ct - tests_left - tests_failed);
    printf("Skipped: %i\n", tests_left);

    return tests_failed;
}

int test_matrix_height(void) {
    const int test_ct = 2;
    int tests_left = test_ct;
    int tests_failed = 0;
    Matrix *matrix;

    printf("Testing: matrix_height\n");

    printf("  1x1 matrix_height test: ");
    matrix = matrix_create(1, 1);
    if (matrix == NULL) goto test_matrix_height_skip_remaining_tests;
    tests_failed += size_t_assert_equal(1, matrix_height(matrix)) != 0 ? 1 : 0;
    tests_left--;
    matrix_destroy(matrix);

    printf("  2x1 matrix_height test: ");
    matrix = matrix_create(2, 1);
    if (matrix == NULL) goto test_matrix_height_skip_remaining_tests;
    tests_failed += size_t_assert_equal(2, matrix_height(matrix)) != 0 ? 1 : 0;
    tests_left--;
    matrix_destroy(matrix);

test_matrix_height_skip_remaining_tests:
    printf("Failed: %i\n", tests_failed);
    printf("Succeeded: %i\n", test_ct - tests_left - tests_failed);
    printf("Skipped: %i\n", tests_left);

    return tests_failed;
}

int test_matrix_create_identity(void) {
    const int test_ct = 2;
    int tests_left = test_ct;
    int tests_failed = 0;

    Matrix *matrix1;
    Matrix *matrix2;
    printf("Testing: matrix_create_identity\n");
    printf("  1x1 matrix_create_identity test: ");
    matrix1 = matrix_create_identity(1);
    if (matrix1 == NULL) {
        printf(RED "Failure: Could not create" RESET "\n");
        tests_failed++;
    } else {
        tests_failed += mat_t_assert_equal(matrix_get(matrix1, 0, 0), MAT_T_1) == 0 ? 1 : 0;
        matrix_destroy(matrix1);
    }
    tests_left--;

    printf("  3x3 matrix_create_identity test: ");
    matrix1 = matrix_create_identity(3);
    matrix2 = matrix_create(3, 3);
    matrix_set(matrix2, 0, 0, MAT_T_1);
    matrix_set(matrix2, 1, 1, MAT_T_1);
    matrix_set(matrix2, 2, 2, MAT_T_1);
    if (matrix1 == NULL || matrix2 == NULL) {
        printf(RED "Failure: Could not create" RESET "\n");
        tests_failed++;
    } else {
        tests_failed += matrix_assert_equal(matrix1, matrix2) == 0 ? 1 : 0;
    }
    matrix_destroy(matrix1);
    matrix_destroy(matrix2);
    tests_left--;

    printf("Failed: %i\n", tests_failed);
    printf("Succeeded: %i\n", test_ct - tests_left - tests_failed);
    printf("Skipped: %i\n", tests_left);

    return tests_failed;
    
}

int test_matrix_create(void) {
    const int test_ct = 2;
    int tests_left = test_ct;
    int tests_failed = 0;

    Matrix *matrix;
    printf("Testing: matrix_create\n");
    printf("  1x1 matrix_create test: ");
    matrix = matrix_create(1, 1);
    if (matrix == NULL) {
        printf(RED "Failure: Could not create" RESET "\n");
        tests_failed++;
    } else {
        tests_failed += mat_t_assert_equal(matrix_get(matrix, 0, 0), MAT_T_0) == 0 ? 1 : 0;
        matrix_destroy(matrix);
    }
    tests_left--;

    printf("  2x2 matrix_create test: ");
    matrix = matrix_create(2, 2);
    if (matrix == NULL) {
        printf(RED "Failure: Could not create" RESET "\n");
        tests_failed++;
    } else {
        printf(GREEN "Success" RESET "\n");
        matrix_destroy(matrix);
    }
    tests_left--;

    printf("Failed: %i\n", tests_failed);
    printf("Succeeded: %i\n", test_ct - tests_left - tests_failed);
    printf("Skipped: %i\n", tests_left);

    return tests_failed;
}

int main(void) {
    int total_failures = 0;
    total_failures += test_matrix_width();
    total_failures += test_matrix_height();
    total_failures += test_matrix_create_identity();
    total_failures += test_matrix_create();

    return total_failures;
    /* det[A - lI] = 0 */
}
