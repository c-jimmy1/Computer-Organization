#include "free.h"

int mm_free(matrix* mat)
{
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
  for (int i = 0; i < mat->rows; ++i) {
    free(mat->data[i]);
  }
  free(mat->data);
  /* note: free does not return a value */
  
  return 0;
}


