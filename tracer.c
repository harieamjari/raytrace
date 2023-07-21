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
                light3D *light,
		char occlusion_test,
		int depth);
extern float dot_product(vec3D, vec3D);
extern scene3D scene;

rgba_t get_pixel(int x, int y){
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

  if (shoot_ray(O, P, &rgba, &t, &intersect, &normal, &face, &object, &light, 0, 0)){
    float r = 0; 
    float g = 0;
    float b = 0;
//    if (object.geometry_type == GEOMETRY_NPRIMITIVE){
//      float lum = (dot_product(light, normal) + 1.0)/2.0;
//      if ((int)fabs(intersect.x) & 1 && (int)intersect.z % 2 ||
//  	(!((int)intersect.x % 2) && !((int)intersect.z % 2))){
//        r = (float)(255.0);
//        g = (float)(255.0);
//        b = (float)(255.0);
//      }
//    } else if (object.geometry_type == GEOMETRY_SPHERE) {
//      float lum = (dot_product(light, normal) + 1.0)/2.0;
//
//      r = (float)(lum*255.0);
//      g = (float)(lum*255.0);
//      b = (float)(lum*255.0);
//    }
//
//    uint8_t a = 1.0;
    return rgba;
  }
  else
    return scene.background;

}
