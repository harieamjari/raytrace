#include <stdlib.h>
#include <math.h>
#include "common.h"

extern vec3D compute_snormal(triangle3D);
extern char solve_matrix(matrix3x4, vec3D*);
extern vec3D normalize(vec3D);
extern float dot_product(vec3D, vec3D);
extern float magnitude(vec3D);
extern vec3D add_vec3D(vec3D a, vec3D b);
extern vec3D muls_vec3D(vec3D a, float b);
extern vec4D adds_vec4D(vec4D a, float b);
extern vec4D add_vec4D(vec4D a, vec4D b);
extern vec4D muls_vec4D(vec4D a, float b);

// returns 1 if it interesects a polygon and initializes
// t, intersect, normal, and object.
extern scene3D scene;
char shoot_ray( vec3D O,
		vec3D P,
		/* rgba values */
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
		int depth){

  if (depth > 5) return 0;
  *t = +INFINITY;
  vec3D D = add_vec3D(P, muls_vec3D(O, -1.0));
  char has_intersect = 0;
  for (int object_i = 0; object_i < scene.nb_objects; object_i++){
    object3D objecti = scene.objects[object_i];
    if (objecti.geometry_type == GEOMETRY_NPRIMITIVE){
      for (int face_i = 0; face_i < objecti.nb_faces; face_i++){
        triangle3D facei = objecti.faces[face_i];
        vec3D A = *(facei.vertices[0]);
        vec3D B = *(facei.vertices[1]);
        vec3D C = *(facei.vertices[2]);
        matrix3x4 matrix = (matrix3x4){
          (vec4D){A.x - C.x, B.x - C.x, -(P.x - O.x), O.x - C.x},
          (vec4D){A.y - C.y, B.y - C.y, -(P.y - O.y), O.y - C.y},
          (vec4D){A.z - C.z, B.z - C.z, -(P.z - O.z), O.z - C.z},
        };
  
        vec3D v = {0.0, 0.0, 0.0};
        char no_solution = solve_matrix(matrix, &v);
        if (no_solution)
          continue;
        /* not in the triangle */
        if (v.x < 0.0 || v.y < 0.0 || v.z < 0.0 || v.x + v.y > 1.0)
          continue;
  
      	*intersect = add_vec3D(add_vec3D(add_vec3D(muls_vec3D(A, v.x), muls_vec3D(B, v.y)), muls_vec3D(C, 1.0-(v.x + v.y))), O /*(vec3D){0.0,0.0,0.0}*/);
	float d = magnitude(*intersect);
	//fprintf(stderr, "tr %f\n", d);
        if (*t > d){
      	has_intersect = 1;
        *t = d;
      	//*intersect = add_vec3D(add_vec3D(muls_vec3D(A, v.x), muls_vec3D(B, v.y)), muls_vec3D(C, 1.0-(v.x + v.y)));
      	*face = facei;
        *object = objecti;
        *normal = compute_snormal(facei);
        }
      }
    } else if (objecti.geometry_type == GEOMETRY_SPHERE){
        vec3D L = add_vec3D(objecti.sphere_center, muls_vec3D(O, -1.0));
        float m = dot_product(normalize(D), L);
        if (m  <= 0.0)
          continue;
	float dc = magnitude(L);
	float d;
	if ((d = dc*dc - m*m) <= 0.0) {
          continue;
        }
	float p0 = 0.0;
	if ((p0 = objecti.sphere_r * objecti.sphere_r - d) <= 0.0)
	  continue;

        p0 = -sqrtf(p0) + m;
	if ( *t > p0 ) {
      	has_intersect = 1;
        *t = p0;
      	*intersect = add_vec3D(muls_vec3D(normalize(D), p0), O);
	if (object != NULL)
      	  *object = objecti;
	if (normal != NULL)
      	  *normal = normalize(add_vec3D(*intersect, muls_vec3D(objecti.sphere_center, -1.0)));
	//fprintf(stderr, "norm %f\n", *normal);
	}

    }
  }
  if (!has_intersect) return has_intersect;
  if (object->geometry_type == GEOMETRY_NPRIMITIVE){
      //float lum = (dot_product(light, normal) + 1.0)/2.0;
      uint8_t r = 0, g = 0, b = 0;
      if ((int)fabs(intersect->x) & 1 && (int)intersect->z % 2 ||
  	(!((int)intersect->x % 2) && !((int)intersect->z % 2))){
        r = (uint8_t)(255.0);
        g = (uint8_t)(255.0);
        b = (uint8_t)(255.0);
      }
      rgba->val[0] = r;
      rgba->val[1] = g;
      rgba->val[2] = b;
      rgba->val[3] = 255;

  } else if (object->geometry_type == GEOMETRY_SPHERE){
      //vec3D I = normalize(muls_vec3D(add_vec3D(*intersect, muls_vec3D(O, -1.0)), -1.0));
      vec3D I = *intersect;
      vec3D B = muls_vec3D(*normal, dot_product(*normal, I));
      vec3D A = add_vec3D(I, muls_vec3D(*normal, -dot_product(*normal, I)));
      vec3D R = add_vec3D(I, muls_vec3D(*normal, -2.0*dot_product(*normal, I)));
      vec3D PO = add_vec3D(add_vec3D(R, O), *intersect);
      /* rgba values */
      rgba_t drgba;
      /* distance from the intersection */
      float dt;
      /* intersection point */
      vec3D dintersect;
      vec3D dnormal;
      /* surface normal (normalize) */
      triangle3D dface;
      object3D dobject;
      if (shoot_ray(*intersect, PO, &drgba, &dt, &dintersect, &dnormal, &dface, &dobject, depth + 1))
        *rgba = drgba;
      else {
        *rgba = scene.background;
      }
      
     
  }

  return has_intersect;
}
