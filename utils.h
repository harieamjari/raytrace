#ifndef HARIE_UTILS_H
#define HARIE_UTILS_H
char shoot_ray(
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
	uint64_t *rng,
	int depth);
#endif
