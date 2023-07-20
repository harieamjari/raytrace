#ifndef HARI_COMMON_H
#define HARI_COMMON_H
#include <stdint.h>

typedef struct rgba_t rgba_t;
struct rgba_t {
  // RGBA
  float r, g, b, a;
//  union {
//    uint8_t val[4];
//    struct {uint8_t r, g, b, a;};
//  };
};

typedef struct vec3D vec3D;
struct vec3D {
  float x, y, z;
};

typedef struct vec4D vec4D;
struct vec4D {
  float x, y, z, w;
};

typedef struct matrix3x4 matrix3x4;
struct matrix3x4 {
  vec4D row[3];
};

typedef struct triangle3D triangle3D;
struct triangle3D {
  vec3D *vertices[3];
};

enum material_t {
  MATERIAL_CHECKER_BOARD,
  MATERIAL_RGB,
  MATERIAL_IMG
};
typedef enum material_t material_t;

typedef struct material3D material3D;
struct material3D {
  material_t material_type;
  union {
    rgba_t color;
    char **img;
  };
};

enum geometry_t {
  GEOMETRY_NPRIMITIVE = 0,
  GEOMETRY_SPHERE,
};
typedef enum geometry_t geometry_t;

typedef struct object3D object3D;
struct object3D {
  geometry_t geometry_type;  
  union {
   // GEOMETRY_NPRIMITIVE
    struct {
      // some objects reuses same vertex for the same
      // triangle
      int nb_vertices;
      // vertex lists 
      vec3D *vertices;
    
      int nb_faces;
      triangle3D *faces;
    };
   // GEOMETRY_SPHERE
    struct {
      vec3D sphere_center;
      float sphere_r;
    };
  };
  // faces[10].vertices[2]

  material3D material;
  float reflection_coef;

  float b, s, d, f;
};

typedef struct light3D light3D;
struct light3D {
  geometry_t geometry_type;  
  union {
   // GEOMETRY_NPRIMITIVE
    struct {
      // some lights reuses same vertex
      int nb_vertices;
      // vertex lists 
      vec3D *vertices;
    
      int nb_faces;
      triangle3D *faces;
    };
   // GEOMETRY_SPHERE
    struct {
      vec3D sphere_center;
      float sphere_r;
    };
  };
  float light_intensity;
};


typedef struct scene3D scene3D;
struct scene3D {
  int nb_objects;
  object3D *objects;
  int nb_lights;
  light3D *lights;
  rgba_t background;
};

#endif
