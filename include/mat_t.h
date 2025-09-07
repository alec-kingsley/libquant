#ifndef MAT_T_H
#define MAT_T_H

typedef double mat_t;

#define MAT_T_0 0.0
#define MAT_T_1 1.0

#define MAT_T_ADD(a, b) (a + b)
#define MAT_T_SUB(a, b) (a - b)
#define MAT_T_MUL(a, b) (a * b)
#define MAT_T_DIV(a, b) (a / b)

#define MAT_T_EQ(a, b) (a == b)

#define MAT_T_PRINT(m) printf("%f", m)

#endif
