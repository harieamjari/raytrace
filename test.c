#include <stdio.h>
#include <stdlib.h>
#include "common.h"

extern char solve_matrix(matrix3x4, vec3D *);

int main(){
  matrix3x4 matrix = (matrix3x4){
(vec4D)    {-3.0, 4.0, 8.0, 7.0},
(vec4D)    {2.0, -3.0, 5.0, -4.0},
(vec4D)    {1.0, 8.0, -3.0, 3.0 }
  };
  vec3D v = {0};
  char m = solve_matrix(matrix, &v);
  printf("%f %f %f\n", v.x, v.y, v.z);
  if (m) puts("unsolvable");
  return 0;
}
