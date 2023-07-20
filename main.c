#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

typedef struct rgbai_t rgbai_t;	
struct rgbai_t {
  uint8_t r, g, b, a;
};

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
  rgbai_t **img_buf = malloc(sizeof(rgbai_t *)*image_height);
  for (int y = 0; y < image_height; y++)
      img_buf[y] = malloc(sizeof(rgbai_t)*image_width);
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

  vec3D topv[3] = {
    (vec3D){0.0, 10.9, -50.0},
    (vec3D){200.0, 11.0, 100.0},
    (vec3D){-200.0, 11.0, 100.0},
  };

  triangle3D topf[1] = {
    {
      .vertices[0] = topv + 0,
      .vertices[1] = topv + 1,
      .vertices[2] = topv + 2
    }
  };


  vec3D leftv[3] = {
    (vec3D){-9.9, 50, 25.0},
    (vec3D){-10.0, -1.0, -100.0},
    (vec3D){-10.0, -1.0, 100.0},
  };

  triangle3D leftf[1] = {
    {
      .vertices[0] = leftv + 0,
      .vertices[1] = leftv + 1,
      .vertices[2] = leftv + 2
    }
  };
  vec3D rightv[3] = {
    (vec3D){9.9, 50, 25.0},
    (vec3D){10.0, -1.0, -100.0},
    (vec3D){10.0, -1.0, 100.0},
  };

  triangle3D rightf[1] = {
    {
      .vertices[0] = rightv + 0,
      .vertices[1] = rightv + 1,
      .vertices[2] = rightv + 2
    }
  };

  object3D objects[5] = {
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 6,
      .vertices = roomv,
      .nb_faces = 2,
      .faces = roomf,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){1.0, 1.0, 1.0, 1.0}
    },
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 3,
      .vertices = topv,
      .nb_faces = 1,
      .faces = topf,
      .material.material_type = MATERIAL_CHECKER_BOARD,
      .material.color = (rgba_t){1.0, 1.0, 1.0, 1.0}
    },
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 3,
      .vertices = leftv,
      .nb_faces = 1,
      .faces = leftf,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){1.0, 0.0, 0.0, 1.0}
    },
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 3,
      .vertices = rightv,
      .nb_faces = 1,
      .faces = rightf,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){0.0, 0.0, 1.0, 1.0}
    },
    {
      .geometry_type = GEOMETRY_SPHERE,
      .sphere_center = (vec3D){0.0, 2.0, 25.0},
      .sphere_r = 3.0,
      .reflection_coef = 0.0,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){1.0,1.0,1.0, 1.0}
    }
  };

  light3D lights[1] = {
    {
      .geometry_type = GEOMETRY_SPHERE,
      .sphere_center = (vec3D){-2.0, 9.0, 19.0},
      .sphere_r = 0.5,
      .light_intensity = 14.0,
    }
  };

  scene = (scene3D){5, objects, 1, lights, (rgba_t){0, 0.51, 0.45, 1.0}};

//#pragma omp parallel for
  for (int y = 0; y < image_height; y++)
    for (int x = 0; x < image_width; x++){
      rgba_t rgba = get_pixel(x - (image_width/2), (image_height/2) - y);
      img_buf[y][x].r = (uint8_t)(rgba.r*255.0);
      img_buf[y][x].g = (uint8_t)(rgba.g*255.0);
      img_buf[y][x].b = (uint8_t)(rgba.b*255.0);
      img_buf[y][x].a = (uint8_t)(rgba.a*255.0);
    }


  for (int y = 0; y < image_height; y++)
    for (int x = 0; x < image_width; x++){
      putchar(img_buf[y][x].r);
      putchar(img_buf[y][x].g);
      putchar(img_buf[y][x].b);
      putchar(img_buf[y][x].a);

    }
  for (int y = 0; y < image_height; y++)
      free(img_buf[y]);

  free(img_buf);

  return 0;
}
