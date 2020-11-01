#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "math_tools.h"

struct point3D *rotation_x(const struct point3D *_matrix, const double d_angle){
   struct point3D *transformed = malloc(sizeof(struct point3D)*sizeof(*_matrix));
   assert(transformed != NULL);

   for (size_t i = 0; i < sizeof(*_matrix)/3; i++){
      transformed[i].x =  _matrix[i].x;
      transformed[i].y = (_matrix[i].y*cos(d_angle*M_PI/180.0))+(-_matrix[i].z*sin(d_angle*M_PI/180.0));
      transformed[i].z = (_matrix[i].y*sin(d_angle*M_PI/180.0))+(_matrix[i].z*cos(d_angle*M_PI/180.0));
   }

   return transformed;
}
