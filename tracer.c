#include <math.h>
#include "common.h"

extern char shoot_ray(
		vec3D O,
		vec3D P,
		rgba_t *rgba,
		/* distance from the intersection */
		float *t,
		/* intersection point */
		vec3D *intersect,
		/* surface normal (normalize) */
                vec3D *normal,
		/* triangle of intersection */
		triangle3D *face,
		/* object of intersection */
		object3D *object,
		int depth);
extern float dot_product(vec3D, vec3D);
rgba_t get_pixel(int x, int y){
  vec3D O =  (vec3D){0.0, 5.0, 0.0};
  vec3D P = (vec3D){(float)x + O.x, (float)y + O.y, 400.0 + O.z};

  vec3D light = (vec3D){0.0, 0.0, -1.0};

  float t = 0.0;
  vec3D intersect, normal;
  triangle3D face;
  object3D object;
  rgba_t rgba;

  if (shoot_ray(O, P, &rgba, &t, &intersect, &normal, &face, &object, 0)){
    uint8_t r = 0; 
    uint8_t g = 0;
    uint8_t b = 0;
    if (object.geometry_type == GEOMETRY_NPRIMITIVE){
      float lum = (dot_product(light, normal) + 1.0)/2.0;
      if ((int)fabs(intersect.x) & 1 && (int)intersect.z % 2 ||
  	(!((int)intersect.x % 2) && !((int)intersect.z % 2))){
        r = (uint8_t)(255.0);
        g = (uint8_t)(255.0);
        b = (uint8_t)(255.0);
      }
    } else if (object.geometry_type == GEOMETRY_SPHERE) {
      float lum = (dot_product(light, normal) + 1.0)/2.0;

      r = (uint8_t)(lum*255.0);
      g = (uint8_t)(lum*255.0);
      b = (uint8_t)(lum*255.0);
    }

    uint8_t a = 255;
    return rgba;
  }
  else
    return (rgba_t){0, 126, 127, 255};

}
