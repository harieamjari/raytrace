#include <math.h>
#include <stdio.h>
#include "common.h"
#include "utils.h"
extern scene3D scene;

rgba_t get_pixel(int x, int y, uint64_t *rng){
  vec3D O =  (vec3D){0.0, 3.0, 5.0};
  vec3D P = (vec3D){(float)x, (float)y + O.y, 400.0};

//  vec3D light = (vec3D){0.0, 0.0, -1.0};
//

  float t = 0.0;
  vec3D intersect, normal;
  triangle3D face;
  object3D object;
  light3D light;
  rgba_t rgba;
  float light_atten_coef = 1.0;
  float val = powf(M_E, -t*light_atten_coef);

  char sret = shoot_ray(O, P, &rgba, &t, &intersect, &normal, &face, &object, &light, 0, rng, 0);
  rgba_t color = rgba;
  if (sret == 1 || sret == 0){
    return (rgba_t) {
      color.r,
      color.g,
      color.b,
      1.0,
    };

  }
  else if (sret == 2)
    return light.light_color;

  else
    return  scene.background_color;

}
