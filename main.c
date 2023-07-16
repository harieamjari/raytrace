#include <stdio.h>
#include "common.h"

extern rgba_t get_pixel(int, int);

int image_width, image_height;
scene3D scene = {0};

int main(int argc, char *argv[]){
  if (argc == 3){
    image_width = atoi(argv[1]); 
    image_height = atoi(argv[2]); 
  }
  else {
    image_width = 640; 
    image_height = 480; 
  }

  vec3D vertices[3] = {
    (vec3D){0.0, -0.5, -50.0},
    (vec3D){200.0, 0.0, 100.0},
    (vec3D){-200.0, 0.0, 100.0}
  };

  triangle3D triangle = {
    .vertices[0] = vertices + 0,
    .vertices[1] = vertices + 1,
    .vertices[2] = vertices + 2
  };

  object3D object[2] = {
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 3,
      .vertices = vertices,
      .nb_faces = 1,
      .faces = &triangle
    },
    {
      .geometry_type = GEOMETRY_SPHERE,
      .sphere_center = (vec3D){0.0, 3.0, 20.0},
      .sphere_r = 4.0,
      .reflection_coef = 0.5,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){190,180,240}
    }
  };

  scene = (scene3D){2, object, (rgba_t){0, 126, 127, 255}};

  for (int y = 0; y < image_height; y++)
    for (int x = 0; x < image_width; x++){
      rgba_t rgba = get_pixel(x - (image_width/2), (image_height/2) - y);
      putchar(rgba.val[0]);
      putchar(rgba.val[1]);
      putchar(rgba.val[2]);
      putchar(rgba.val[3]);
    }

  return 0;
}
