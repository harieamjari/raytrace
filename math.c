#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include "common.h"


vec3D add_vec3D(vec3D a, vec3D b){
  return (vec3D){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3D adds_vec3D(vec3D a, float b){
  return (vec3D){a.x + b, a.y + b, a.z + b};
}

vec3D sub_vec3D(vec3D a, vec3D b){
  return (vec3D){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3D muls_vec3D(vec3D a, float b){
  return (vec3D){a.x * b, a.y * b, a.z * b};
}

vec4D adds_vec4D(vec4D a, float b){
  return (vec4D){a.x + b, a.y + b, a.z + b, a.w + b};
}

vec4D add_vec4D(vec4D a, vec4D b){
  return (vec4D){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

vec4D muls_vec4D(vec4D a, float b){
  return (vec4D){a.x * b, a.y * b, a.z * b, a.w * b};
}

vec3D normalize(vec3D v){
  float mag = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
  return (vec3D){v.x/mag, v.y/mag, v.z/mag};
}

rgba_t add_rgb(rgba_t a, rgba_t v){
  float r = a.r + v.r;
  float g = a.g + v.g;
  float b = a.b + v.b;
  return (rgba_t){
    .r = (r > 1.0 ? 1.0 : r ),
    .g = (g > 1.0 ? 1.0 : g ),
    .b = (b > 1.0 ? 1.0 : b ),
    .a = a.a
  };
}

rgba_t muls_rgb(rgba_t a, float v){
  float r = a.r * v;
  float g = a.g * v;
  float b = a.b * v;
  return (rgba_t){
//    .r = (r > 1.0 ? 1.0 : r ),
//    .g = (g > 1.0 ? 1.0 : g ),
//    .b = (b > 1.0 ? 1.0 : b ),
    .r = r,
    .g = g,
    .b = b,
    .a = a.a,
  };
}

rgba_t mul_rgb(rgba_t a, rgba_t v){
  float r = a.r * v.r;
  float g = a.g * v.g;
  float b = a.b * v.b;
  return (rgba_t){
//    .r = (r > 1.0 ? 1.0 : r ),
//    .g = (g > 1.0 ? 1.0 : g ),
//    .b = (b > 1.0 ? 1.0 : b ),
    .r = (r),
    .g = (g),
    .b = (b),
    .a = a.a,
  };
}

rgba_t muls_rgba(rgba_t a, float b){
  return (rgba_t){
    .r = (a.r * b),
    .g = (a.g * b),
    .b = (a.b * b),
    .a = (a.a * b),
  };
}
float dot_product(vec3D a, vec3D b){
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

float magnitude(vec3D v){
  return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3D compute_snormal(triangle3D triangle){
  vec3D A = *triangle.vertices[0];
  vec3D B = *triangle.vertices[1];
  vec3D C = *triangle.vertices[2];

  vec3D v1 = add_vec3D(A, muls_vec3D(C, -1.0));
  vec3D v2 = add_vec3D(B, muls_vec3D(C, -1.0));
  return normalize((vec3D){
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x
  });

}


// returns 1 if unsolvable.
#pragma STDC FENV_ACCESS ON
char solve_matrix(matrix3x4 matrix, vec3D *v){
  /* to put it simply, it turns a matrix:
   *
   *   matrix3x4 matrix = (matrix3x4){
   *     (vec4D){-3.0, 4.0, 8.0, 7.0},
   *     (vec4D){2.0, -3.0, 5.0, -4.0},
   *     (vec4D){1.0, 8.0, -3.0, 3.0 }
   *   };
   *
   * into its row echelon form:
   *
   *   matrix3x4 matrix = (matrix3x4){
   *     (vec4D){1.0, 0.0, 0.0, -1.346020},
   *     (vec4D){0.0, 1.0, 0.0, 0.574395},
   *     (vec4D){0.0, 0.0, 1.0, 0.083045}
   *   };
   *
   * and then assigns:
   *
   *   v->x = matrix.row[0].w;
   *   v->y = matrix.row[1].w;
   *   v->z = matrix.row[2].w;
   */




  feclearexcept(FE_ALL_EXCEPT);
#define FAST_GAUSSIAN
#ifndef FAST_GAUSSIAN
  if (matrix.row[0].x == 0.0 &&
      matrix.row[1].x == 0.0 &&
      matrix.row[2].x == 0.0)
    return 1;
  if (matrix.row[0].y == 0.0 &&
      matrix.row[1].y == 0.0 &&
      matrix.row[2].y == 0.0)
    return 1;
  if (matrix.row[0].z == 0.0 &&
      matrix.row[1].z == 0.0 &&
      matrix.row[2].z == 0.0)
    return 1;

  /* Make sure 0th row 0th column is non-zero*/
  if (matrix.row[0].x == 0.0){
    if (matrix.row[1].x != 0.0)
      matrix.row[0] = add_vec4D(matrix.row[0], matrix.row[1]);
    else 
      matrix.row[0] = add_vec4D(matrix.row[0], matrix.row[2]);
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[1].x != 0.0){
#endif
    matrix.row[1] = add_vec4D(matrix.row[1], muls_vec4D(matrix.row[0], -matrix.row[1].x / matrix.row[0].x));
#ifndef FAST_GAUSSIAN
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[2].x != 0.0){
#endif
    matrix.row[2] = add_vec4D(matrix.row[2], muls_vec4D(matrix.row[0], -matrix.row[2].x / matrix.row[0].x));
#ifndef FAST_GAUSSIAN
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[0].x != 1.0) {
#endif
    matrix.row[0] = muls_vec4D(matrix.row[0], 1.0/matrix.row[0].x);
#ifndef FAST_GAUSSIAN
  }
#endif
  matrix.row[0].x = 1.0;

  if (fetestexcept(FE_DIVBYZERO | FE_INVALID))
    return 1;

  /* Make sure 1st row 1st column is non-zero*/
  if (matrix.row[1].y == 0.0){
    if (matrix.row[0].y != 0.0)
      matrix.row[1] = add_vec4D(matrix.row[1], matrix.row[0]);
    else if (matrix.row[2].y != 0.0)
      matrix.row[1] = add_vec4D(matrix.row[1], matrix.row[2]);
    else return 1;
  }

#ifndef FAST_GAUSSIAN
  if (matrix.row[0].y != 0.0){
#endif
    matrix.row[0] = add_vec4D(matrix.row[0], muls_vec4D(matrix.row[1], -matrix.row[0].y / matrix.row[1].y));
#ifndef FAST_GAUSSIAN
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[2].y != 0.0){
#endif
    matrix.row[2] = add_vec4D(matrix.row[2], muls_vec4D(matrix.row[1], -matrix.row[2].y / matrix.row[1].y));
#ifndef FAST_GAUSSIAN
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[1].y != 1.0) {
#endif
    matrix.row[1] = muls_vec4D(matrix.row[1], 1.0/matrix.row[1].y);
#ifndef FAST_GAUSSIAN
  }
#endif
  matrix.row[1].y = 1.0;
  if (fetestexcept(FE_DIVBYZERO | FE_INVALID))
    return 1;

  /* Make sure 2nd row 2nd column is non-zero*/
  if (matrix.row[2].z == 0.0){
    if (matrix.row[0].z != 0.0)
      matrix.row[2] = add_vec4D(matrix.row[2], matrix.row[0]);
    else if (matrix.row[1].z != 0.0)
      matrix.row[2] = add_vec4D(matrix.row[2], matrix.row[1]);
    else return 1;
  }

#ifndef FAST_GAUSSIAN
  if (matrix.row[0].z != 0.0){
#endif
    matrix.row[0] = add_vec4D(matrix.row[0], muls_vec4D(matrix.row[2], -matrix.row[0].z / matrix.row[2].z));
#ifndef FAST_GAUSSIAN
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[1].z != 0.0){
#endif
    matrix.row[1] = add_vec4D(matrix.row[1], muls_vec4D(matrix.row[2], -matrix.row[1].z / matrix.row[2].z));
#ifndef FAST_GAUSSIAN
  }
#endif

#ifndef FAST_GAUSSIAN
  if (matrix.row[2].z != 1.0) {
#endif
    matrix.row[2] = muls_vec4D(matrix.row[2], 1.0/matrix.row[2].z);
#ifndef FAST_GAUSSIAN
  }
#endif


  if (matrix.row[0].x != 1.0 || matrix.row[1].y != 1.0)
    return 1;

  v->x = matrix.row[0].w;
  v->y = matrix.row[1].w;
  v->z = matrix.row[2].w;

  return 0;
}
#pragma STDC FENV_ACCESS DEFAULT
