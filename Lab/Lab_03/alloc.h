#ifndef ALLOC_H_   /* Include guard */
#define ALLOC_H_
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int mm_alloc(matrix* mat);

#endif