#ifndef TRACESINGLERAY_H
#define TRACESINGLERAY_H

#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include "utils/rgba.h"

RGBA traceRay(glm::vec4 position, glm::vec4 direction,
              const RayTraceScene &scene, const RayTracer::Config &config, int completedReflections, double time);

float traceShadowRay(glm::vec4 position, glm::vec4 direction,
                     const RayTraceScene &scene, double time);

#endif // TRACESINGLERAY_H
