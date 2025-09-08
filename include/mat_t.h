#ifndef MAT_T_H
#define MAT_T_H

typedef double mat_t;

#define MAT_T_0 0.0
#define MAT_T_1 1.0

/* may need to be replaced with function call for more complicated MAT_T values */
#define MAT_T(n) MAT_T_MUL((mat_t)n, MAT_T_1)

#define MAT_T_ADD(a, b) (a + b)
#define MAT_T_SUB(a, b) (a - b)
#define MAT_T_MUL(a, b) (a * b)
#define MAT_T_DIV(a, b) (a / b)

#define MAT_T_PRECISION 1e-10

#define MAT_T_EQ(a, b) (a + MAT_T_PRECISION > b && b > a - MAT_T_PRECISION)

#define MAT_T_PRINT(m) printf("%.3f", m)

#endif
