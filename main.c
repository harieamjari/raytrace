#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
  srand(time(NULL));
  vec3D roomv[6] = {
    (vec3D){0.0, 0.9, -50.0},
    (vec3D){200.0, 1.0, 100.0},
    (vec3D){-200.0, 1.0, 100.0},

    (vec3D){0.0, 25.0, 30.0},
    (vec3D){25.0, -1.0, 35.0},
    (vec3D){-25.0, -1.0, 35.0},
  };

  triangle3D roomf[2] = {
    {
      .vertices[0] = roomv + 0,
      .vertices[1] = roomv + 1,
      .vertices[2] = roomv + 2
    },
    {
      .vertices[0] = roomv + 3,
      .vertices[1] = roomv + 4,
      .vertices[2] = roomv + 5
    }
  };

  object3D objects[2] = {
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 6,
      .vertices = roomv,
      .nb_faces = 2,
      .faces = roomf,
      .material.material_type = MATERIAL_CHECKER_BOARD,
      .material.color = (rgba_t){1.0, 1.0, 1.0, 1.0}
    },
    {
      .geometry_type = GEOMETRY_SPHERE,
      .sphere_center = (vec3D){0.0, 3.0, 25.0},
      .sphere_r = 3.0,
      .reflection_coef = 0.5,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){0.82,0.72,0.86, 1.0}
    }
  };

  light3D lights[1] = {
    {
      .geometry_type = GEOMETRY_SPHERE,
      .sphere_center = (vec3D){0.0, 10.0, 30.0},
      .sphere_r = 0.5,
    }
  };

  scene = (scene3D){2, objects, 1, lights, (rgba_t){0, 0.51, 0.45, 1.0}};

  for (int y = 0; y < image_height; y++)
    for (int x = 0; x < image_width; x++){
      rgba_t rgba = get_pixel(x - (image_width/2), (image_height/2) - y);
      putchar((uint8_t)(rgba.r * 255.0));
      putchar((uint8_t)(rgba.g * 255.0));
      putchar((uint8_t)(rgba.b * 255.0));
      putchar((uint8_t)(rgba.a * 255.0));
    }

  return 0;
}
