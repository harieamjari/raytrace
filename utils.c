/* utils.c - utilities for raytracing */
/* Copyright (c) 2023 Al-buharie Amjari */
/* Release under MIT */

#include <stdlib.h>
#include <math.h>
#include "common.h"

extern vec3D compute_snormal(triangle3D);
extern char solve_matrix(matrix3x4, vec3D*);
extern vec3D normalize(vec3D);
extern float dot_product(vec3D, vec3D);
extern float magnitude(vec3D);

extern vec3D add_vec3D(vec3D a, vec3D b);
extern vec3D sub_vec3D(vec3D a, vec3D b);
extern vec3D muls_vec3D(vec3D a, float b);

extern vec4D adds_vec4D(vec4D a, float b);
extern vec4D add_vec4D(vec4D a, vec4D b);
extern vec4D muls_vec4D(vec4D a, float b);

extern rgba_t muls_rgb(rgba_t, float);
extern rgba_t mul_rgb(rgba_t, rgba_t);
extern rgba_t add_rgb(rgba_t, rgba_t);

// returns 1 if it interesects a polygon and initializes
// t, intersect, normal, and object.
extern scene3D scene;

uint8_t u8_getr(rgba_t rgba){
  return (uint8_t)(rgba.r * 255.0);
}

uint8_t u8_getg(rgba_t rgba){
  return (uint8_t)(rgba.g * 255.0);
}

uint8_t u8_getb(rgba_t rgba){
  return (uint8_t)(rgba.b * 255.0);
}

// generate monte carlo ray 
vec3D montc_ray(vec3D norm){
  vec3D randv = normalize((vec3D){
    (float)(500 - rand()%1000),
    (float)(500 - rand()%1000),
    (float)(500 - rand()%1000)
  });

  if (dot_product(randv, norm) <= 0.0)
    return muls_vec3D(randv, -1.0);

  return randv;
}

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
		/* object of intersection */
		light3D *light,
		int depth){

  if (depth > 5) return 0;
  *t = +INFINITY;
  vec3D D = sub_vec3D(P, O);
  char has_intersect = 0;
  for (int object_i = 0; object_i < scene.nb_objects; object_i++){
    object3D objecti = scene.objects[object_i];
    if (objecti.geometry_type == GEOMETRY_NPRIMITIVE){
      for (int face_i = 0; face_i < objecti.nb_faces; face_i++){
        triangle3D facei = objecti.faces[face_i];
        vec3D A = sub_vec3D(*(facei.vertices[0]), O);
        vec3D B = sub_vec3D(*(facei.vertices[1]), O);
        vec3D C = sub_vec3D(*(facei.vertices[2]), O);
//        vec3D B = *(facei.vertices[1]);
//        vec3D C = *(facei.vertices[2]);
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
  
      	vec3D temp_intersect = add_vec3D(add_vec3D(add_vec3D(muls_vec3D(A, v.x), muls_vec3D(B, v.y)), muls_vec3D(C, 1.0-(v.x + v.y))), O /*(vec3D){0.0,0.0,0.0}*/);
	float d = magnitude(temp_intersect);
	//fprintf(stderr, "tr %f\n", d);
        if (*t > d){
      	has_intersect = 1;
        *t = d;
      	//*intersect = add_vec3D(add_vec3D(muls_vec3D(A, v.x), muls_vec3D(B, v.y)), muls_vec3D(C, 1.0-(v.x + v.y)));
      	*intersect = temp_intersect; 
      	*face = facei;
        *object = objecti;
        *normal = compute_snormal(facei);
        }
      }
    } else if (objecti.geometry_type == GEOMETRY_SPHERE){
        vec3D L = sub_vec3D(objecti.sphere_center, O);
        //vec3D L = objecti.sphere_center;
//        float m = dot_product(normalize(D), L);
//        if (m  <= 0.0)
//          continue;
//	float dc = magnitude(L);
//	float d;
//	if ((d = dc*dc - m*m) <= 0.0) {
//          continue;
//        }
//	float p0 = 0.0;
//	if ((p0 = objecti.sphere_r * objecti.sphere_r - d) <= d)
//	  continue;
//
//        p0 = -sqrtf(p0) + m;
//
	float tc = dot_product(L, normalize(D));
	
	if ( tc < 0.0 ) continue;
	float d2 = magnitude(L)*magnitude(L) - tc*tc;
	
	float radius2 = objecti.sphere_r * objecti.sphere_r;
	if ( d2 > radius2) continue;

	//solve for t1c
	float t1c = sqrtf( radius2 - d2 );

	//solve for intersection points
	float t1 = tc - t1c;
	if ( *t > t1 ) {
      	has_intersect = 1;
        *t = t1;
      	*intersect = add_vec3D(muls_vec3D(normalize(D), t1), O);
      	*object = objecti;
//      	*normal = normalize(sub_vec3D(*intersect, objecti.sphere_center));
      	*normal = normalize(sub_vec3D(sub_vec3D(*intersect, O), sub_vec3D(objecti.sphere_center, O)));
	//fprintf(stderr, "norm %f\n", *normal);
	}

    }
  }
  // check lights
  char light_intersect = 0;
  for (int light_i = 0; light_i < scene.nb_lights; light_i++){
    light3D lighti = scene.lights[light_i];
    if (lighti.geometry_type == GEOMETRY_NPRIMITIVE){
//      for (int face_i = 0; face_i < lighti.nb_faces; face_i++){
//        triangle3D facei = lighti.faces[face_i];
//        vec3D A = *(facei.vertices[0]);
//        vec3D B = *(facei.vertices[1]);
//        vec3D C = *(facei.vertices[2]);
//        matrix3x4 matrix = (matrix3x4){
//          (vec4D){A.x - C.x, B.x - C.x, -(P.x - O.x), O.x - C.x},
//          (vec4D){A.y - C.y, B.y - C.y, -(P.y - O.y), O.y - C.y},
//          (vec4D){A.z - C.z, B.z - C.z, -(P.z - O.z), O.z - C.z},
//        };
//  
//        vec3D v = {0.0, 0.0, 0.0};
//        char no_solution = solve_matrix(matrix, &v);
//        if (no_solution)
//          continue;
//        /* not in the triangle */
//        if (v.x < 0.0 || v.y < 0.0 || v.z < 0.0 || v.x + v.y > 1.0)
//          continue;
//  
//      	*intersect = add_vec3D(add_vec3D(add_vec3D(muls_vec3D(A, v.x), muls_vec3D(B, v.y)), muls_vec3D(C, 1.0-(v.x + v.y))), O /*(vec3D){0.0,0.0,0.0}*/);
//	float d = magnitude(*intersect);
//	//fprintf(stderr, "tr %f\n", d);
//        if (*t > d){
//      	has_intersect = 1;
//        *t = d;
//      	//*intersect = add_vec3D(add_vec3D(muls_vec3D(A, v.x), muls_vec3D(B, v.y)), muls_vec3D(C, 1.0-(v.x + v.y)));
//      	*face = facei;
//        *light = lighti;
//        *normal = compute_snormal(facei);
//        }
//      }
    } else if (lighti.geometry_type == GEOMETRY_SPHERE){
        vec3D L = sub_vec3D(lighti.sphere_center, O);
	float tc = dot_product(L, normalize(D));
	
	if ( tc < 0.0 ) continue;
	float d2 = magnitude(L)*magnitude(L) - tc*tc;
	
	float radius2 = lighti.sphere_r * lighti.sphere_r;
	if ( d2 > radius2) continue;

	//solve for t1c
	float t1c = sqrtf( radius2 - d2 );

	//solve for intersection points
	float t1 = tc - t1c;
	if ( *t > t1 ) {
      	has_intersect = 2;
        *t = t1;
      	*intersect = add_vec3D(muls_vec3D(normalize(D), t1), O);
      	*light = lighti;
      	*normal = normalize(sub_vec3D(*intersect, lighti.sphere_center));
	//fprintf(stderr, "norm %f\n", *normal);
	}
//        float m = dot_product(normalize(D), L);
//        if (m  <= 0.0)
//          continue;
//	float dc = magnitude(L);
//	float d;
//	if ((d = dc*dc - m*m) <= 0.0) {
//          continue;
//        }
//	float p0 = 0.0;
//	if ((p0 = lighti.sphere_r * lighti.sphere_r - d) <= 0.0)
//	  continue;
//
//        p0 = -sqrtf(p0) + m;
//	if ( *t > p0 ) {
//          has_intersect = 2;
//          *t = p0;
//          *intersect = add_vec3D(muls_vec3D(normalize(D), p0), O);
//          if (light != NULL){
//            *light = lighti;
//          }
//          if (normal != NULL)
//            *normal = normalize(add_vec3D(*intersect, muls_vec3D(lighti.sphere_center, -1.0)));
          //fprintf(stderr, "norm %f\n", *normal);
	}

  }
  // we have intersected a light
  if (has_intersect == 2){
	  rgba->r = 1.0;
	  rgba->g = 1.0;
	  rgba->b = 1.0;
	  rgba->a = 1.0;

    return 2;
  }
  // no intersection
  if (has_intersect == 0) return 0;
  const float lum = (dot_product(normalize(sub_vec3D(scene.lights[0].sphere_center, *intersect)), *normal) + 1.0)/2.0;

  /* apply default color material */
  if (object->material.material_type == MATERIAL_CHECKER_BOARD){
    if (((int)fabs(intersect->x) & 1 && (int)intersect->z % 2) ||
      (!((int)intersect->x % 2) && !((int)intersect->z % 2))){
      rgba->r = 0.9;
      rgba->g = 0.9;
      rgba->b = 0.9;
      rgba->a = 1.0;
    } else {
      rgba->r = 0.2;
      rgba->g = 0.2;
      rgba->b = 0.2;
      rgba->a = 1.0;
    }
  } else if (object->material.material_type == MATERIAL_RGB) {
    *rgba = object->material.color;
  }


  if (object->geometry_type == GEOMETRY_NPRIMITIVE){
    //vec3d rl = sub_vec3d(sub_vec3d(scene.lights[0].sphere_center, o), sub_vec3d(*intersect, o));
    vec3D rl = sub_vec3D(scene.lights[0].sphere_center, *intersect);
 //   vec3D rlo = sub_vec3D(*intersect, scene.lights[0].sphere_center);
    float dl = magnitude(rl);
    vec3D nrl = normalize(rl);
    vec3D PO = add_vec3D(*intersect, nrl);
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
    light3D dlight;
    rgba_t total_illum;
    char sret;
    //*rgba = object->material.color;
    sret = shoot_ray(*intersect, /*add_vec3D(*intersect, nrl)*/scene.lights[0].sphere_center, &drgba, &dt, &dintersect, &dnormal, &dface, &dobject, &dlight, depth + 1);
    if (sret == 0 /*sret == 1 */) {
      total_illum = (rgba_t){0.0, 0.0, 0.0, 1.0};
    //  fprintf(stderr, "no such light\n");
    } else if (sret == 1){
      total_illum = (rgba_t){0.0, 0.0, 0.0, 1.0};
    //  float val = powf(M_E, -dl/scene.lights[0].light_intensity);
    } else if (sret == 2){
      float val = powf(M_E, -dl/scene.lights[0].light_intensity);
      total_illum = (rgba_t){val, val, val, 1.0};
    }
    for (int i = 0; i < 50; i++){
      vec3D randp = montc_ray(dnormal);
      if (shoot_ray(*intersect, add_vec3D(*intersect, randp), &drgba, &dt, &dintersect, &dnormal, &dface, &dobject, &dlight, depth + 4) == 1){
        float val = powf(M_E, -dt/scene.lights[0].light_intensity);
        total_illum = add_rgb(total_illum, muls_rgb(drgba, val));
      }

    }
    *rgba = mul_rgb(*rgba, total_illum);
    rgba->a = 1.0;

    // is occluded by lights
//      float r = 0.0, g = 0.0, b = 0.0;
//      if ((int)fabs(intersect->x) & 1 && (int)intersect->z % 2 ||
//  	(!((int)intersect->x % 2) && !((int)intersect->z % 2))){
//        r = 1.0;
//        g = 1.0;
//        b = 1.0;
//      }
//      rgba->r = r;
//      rgba->g = g;
//      rgba->b = b;
//      rgba->a = 1.0;

  } else if (object->geometry_type == GEOMETRY_SPHERE){
    //vec3D I = muls_vec3D(sub_vec3D(*intersect, O), -1.0);
      vec3D I = *intersect;
    //vec3D R = add_vec3D(I, muls_vec3D(*normal, -2.0*dot_product(*normal, I)));
    vec3D R = sub_vec3D(muls_vec3D(*normal, 2.0*dot_product(*normal, I)), I);
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
    light3D dlight;
    rgba_t total_illum;
    if (shoot_ray(*intersect, scene.lights[0].sphere_center, &drgba, &dt, &dintersect, &dnormal, &dface, &dobject, &dlight, depth + 4) != 2) {
      total_illum.r = 0.0;
      total_illum.g = 0.0;
      total_illum.b = 0.0;
      total_illum.a = 1.0;
    } else {
   //   vec3D dl = sub_vec3D(scene.lights[0].sphere_center, *intersect);
      float val = powf(M_E, -dt/scene.lights[0].light_intensity);
      total_illum = (rgba_t){val, val, val, 1.0};
      rgba->a = 1.0;
    }
    for (int i = 0; i < 50; i++){
      vec3D randp = montc_ray(dnormal);
      if (shoot_ray(*intersect, add_vec3D(*intersect, randp), &drgba, &dt, &dintersect, &dnormal, &dface, &dobject, &dlight, depth + 4) == 1){
        float val = powf(M_E, -dt/scene.lights[0].light_intensity);
        total_illum = add_rgb(total_illum, muls_rgb(drgba, val));
      }

    }
    *rgba = mul_rgb(*rgba, total_illum);
    rgba->a = 1.0;
   
//      if (shoot_ray(*intersect, PO, &drgba, &dt, &dintersect, &dnormal, &dface, &dobject, &dlight, depth + 1) == 1)
//        *rgba = add_rgb(*rgba, muls_rgb(drgba, object->reflection_coef));
//      else {
//        *rgba = add_rgb(*rgba, muls_rgb(scene.background, object->reflection_coef));
//        //*rgba = scene.background;
//      }
//      rgba->r *= lum;
//      rgba->g *= lum;
//      rgba->b *= lum;
//      rgba->a = 1.0;
  }

  return has_intersect;
}
