#ifndef TRACESINGLERAY_H
#define TRACESINGLERAY_H

<<<<<<< HEAD
#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include "utils/rgba.h"

RGBA traceRay(glm::vec4 position, glm::vec4 direction,
              const RayTraceScene &scene, const RayTracer::Config &config, int completedReflections);

float traceShadowRay(glm::vec4 position, glm::vec4 direction,
                     const RayTraceScene &scene);
=======
#include "../raytracer/raytracer.h"
#include "../raytracer/raytracescene.h"
#include "../utils/rgba.h"

RGBA traceRay(glm::vec4 position, glm::vec4 direction,
              const RayTraceScene &scene, const RayTracer::Config &config, int completedReflections, double time);

float traceShadowRay(glm::vec4 position, glm::vec4 direction,
                     const RayTraceScene &scene, double time);
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e

#endif // TRACESINGLERAY_H
