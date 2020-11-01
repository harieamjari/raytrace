#ifndef MATH_TOOLS
#define MATH_TOOLS

struct point3D {
   double x, y, z;

};

/* The convention is to use the right hand rule */
struct point3D *rotation_x  (const struct point3D *_matrix, const double d_angle);
struct point3S *rotation_y  (const struct point3D *_matrix, const double d_angle);
struct point3D *rotation_z  (const struct point3D *_matrix, const double d_angle);

struct point3D *rotation_xyz(const struct point3D *_matrix,
                             const double d_angle_x,
                             const double d_angle_y,
                             const double d_angle_z);


#endif /* MATH_TOOLS */
