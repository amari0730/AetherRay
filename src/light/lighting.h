#ifndef LIGHTING_H
#define LIGHTING_H

<<<<<<< HEAD
#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"
#include "utils/rgba.h"
#include "utils/scenedata.h"
=======
#include "../raytracer/raytracer.h"
#include "../raytracer/raytracescene.h"
#include "../utils/rgba.h"
#include "../utils/scenedata.h"
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
#include <glm/glm.hpp>

RGBA phong(glm::vec4 position, glm::vec4 normal, glm::vec4 directionToCamera,
           SceneMaterial &material, const std::vector<SceneLightData> &lights,
           const SceneGlobalData &globalData, const RayTraceScene &scene,
           const RayTracer::Config &config, int completedReflections,
<<<<<<< HEAD
           PrimitiveType shapeType, glm::vec4 objectSpaceIntersection);
=======
           PrimitiveType shapeType, glm::vec4 objectSpaceIntersection, double time);
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e

#endif // LIGHTING_H
