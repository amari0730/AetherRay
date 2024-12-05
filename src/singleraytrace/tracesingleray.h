#ifndef TRACESINGLERAY_H
#define TRACESINGLERAY_H

#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include "utils/rgba.h"

RGBA traceRay(glm::vec4 position, glm::vec4 direction,
              const RayTraceScene &scene, const RayTracer::Config &config, int completedReflections);

float traceShadowRay(glm::vec4 position, glm::vec4 direction,
                     const RayTraceScene &scene);

#endif // TRACESINGLERAY_H
