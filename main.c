#include <stdio.h>
#include <stdlib.h>

#include "math_tools.h"

void print_point3Dp(const struct point3D *_matrix){
   for (size_t i = 0; i < (size_t)(sizeof(*_matrix)/3); i++){
      printf("%10.4f", _matrix[i].x);
   }
   putchar('\n');
   for (size_t i = 0; i < (size_t)(sizeof(*_matrix)/3); i++){
      printf("%10.4f", _matrix[i].y);
   }
   putchar('\n');
   for (size_t i = 0; i < (size_t)(sizeof(*_matrix)/3); i++){
      printf("%10.4f", _matrix[i].z);
   }
   putchar('\n');
}


int main(){
   const struct point3D cube[] = {
      { 3.0,  -3.0, -3.0},
      { 3.0,  -3.0,  3.0},
      {-3.0,  -3.0,  3.0},
      {-3.0,  -3.0, -3.0},   
      { 3.0,   3.0, -3.0},
      { 3.0,   3.0,  3.0},
      { -3.0,  3.0,  3.0},
      { -3.0,  3.0, -3.0}  
   };

   struct point3D *transformed_cube = rotation_x(cube, 10.0);
   print_point3Dp(transformed_cube);

   free(transformed_cube);

   return 0;
}
