#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "common.h"

typedef struct rgbai_t rgbai_t;	
struct rgbai_t {
  uint8_t r, g, b, a;
};

extern rgba_t get_pixel(int, int, uint64_t *);

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
  assert(img_buf != NULL);
  for (int y = 0; y < image_height; y++)
      img_buf[y] = malloc(sizeof(rgbai_t)*image_width);
  //srand(time(NULL));

  #include "scene.c"

#pragma omp parallel for
  for (int y = 0; y < image_height; y++){
    uint64_t rng = y;
    for (int x = 0; x < image_width; x++){
      rgba_t rgba = get_pixel(x - (image_width/2), (image_height/2) - y, &rng);
      img_buf[y][x].r = (uint8_t)(rgba.r*255.0);
      img_buf[y][x].g = (uint8_t)(rgba.g*255.0);
      img_buf[y][x].b = (uint8_t)(rgba.b*255.0);
      img_buf[y][x].a = (uint8_t)(rgba.a*255.0);
    }
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
