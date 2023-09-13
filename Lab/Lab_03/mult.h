#ifndef MULT_H_   /* Include guard */
#define MULT_H_
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "alloc.h"

matrix* mm_matrix_mult(matrix* mat1, matrix* mat2);

#endif
