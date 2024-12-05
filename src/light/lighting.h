#ifndef LIGHTING_H
#define LIGHTING_H

#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include "utils/rgba.h"
#include "utils/scenedata.h"
#include <glm/glm.hpp>

RGBA phong(glm::vec4 position, glm::vec4 normal, glm::vec4 directionToCamera,
           SceneMaterial &material, const std::vector<SceneLightData> &lights,
           const SceneGlobalData &globalData, const RayTraceScene &scene,
           const RayTracer::Config &config, int completedReflections,
           PrimitiveType shapeType, glm::vec4 objectSpaceIntersection);

#endif // LIGHTING_H
