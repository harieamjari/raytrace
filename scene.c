  vec3D lightv[3] = {
    (vec3D){1.1, 10.7, 25.1},
    (vec3D){-5.1, 10.7, 25.1},
    (vec3D){-2.1, 10.5, 20.1},
  };

  triangle3D lightf[2] = {
    {
      .vertices[0] = lightv + 0,
      .vertices[1] = lightv + 1,
      .vertices[2] = lightv + 2
    },
  };

  vec3D roomv[6] = {
    (vec3D){200.0, -1.0, 100.0},
    (vec3D){0.0, -0.9, -50.0},
    (vec3D){-200.0, -1.0, 100.0},

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
    (vec3D){-10.0, -1.0, 100.0},
    (vec3D){-10.0, -1.0, -100.0},
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
      .material.color = (rgba_t){1.0, 0.5, 0.5, 1.0}
    },
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 3,
      .vertices = rightv,
      .nb_faces = 1,
      .faces = rightf,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){0.5, 0.5, 1.0, 1.0}
    },
    {
      .geometry_type = GEOMETRY_SPHERE,
      .sphere_center = (vec3D){0.0, 2.0, 25.0},
      .sphere_r = 3.0,
      .reflection_coef = 0.2,
      .material.material_type = MATERIAL_RGB,
      .material.color = (rgba_t){1.0,1.0,1.0, 1.0}
    }
  };

  light3D lights[1] = {
    {
      .geometry_type = GEOMETRY_NPRIMITIVE,
      .nb_vertices = 3,
      .vertices = lightv,
      .nb_faces = 1,
      .faces = lightf,
      .light_intensity = 6.0,
      .light_color = (rgba_t){
        (float)0xfd/255.0,
        (float)0xfb/255.0,
        (float)0xd3/255.0,
	1.0
      },
    },
//    {
//      .geometry_type = GEOMETRY_SPHERE,
//      .sphere_center = (vec3D){-2.0, 10.0, 23.0},
//      .sphere_r = 0.5,
//      .light_intensity = 4.5,
//      .light_color = (rgba_t){
//        (float)0xfd/255.0,
//        (float)0xfb/255.0,
//        (float)0xd3/255.0,
//	1.0
//      },
//    }
  };

  scene = (scene3D){5, objects, 1, lights, (rgba_t){0, 0.51, 0.45, 1.0}};
