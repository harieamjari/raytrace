#ifndef HARIE_MATH_H
#define HARIE_MATH_H

vec3D compute_snormal(triangle3D);
char solve_matrix(matrix3x4, vec3D*);
vec3D normalize(vec3D);
float dot_product(vec3D, vec3D);
float magnitude(vec3D);

vec3D add_vec3D(vec3D a, vec3D b);
vec3D adds_vec3D(vec3D a, float b);
vec3D sub_vec3D(vec3D a, vec3D b);
vec3D muls_vec3D(vec3D a, float b);

vec4D adds_vec4D(vec4D a, float b);
vec4D add_vec4D(vec4D a, vec4D b);
vec4D muls_vec4D(vec4D a, float b);

rgba_t muls_rgb(rgba_t, float);
rgba_t mul_rgb(rgba_t, rgba_t);
rgba_t add_rgb(rgba_t, rgba_t);

#endif
