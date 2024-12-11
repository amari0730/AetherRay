#include "tracesingleray.h"
#include "../light/lighting.h"
#include "../raytracer/raytracescene.h"
#include "../shapes/cone.h"
#include "../shapes/cube.h"
#include "../shapes/cylinder.h"
#include "../shapes/shapeoverall.h"
#include "../shapes/sphere.h"
#include "../utils/scenedata.h"
#include <cfloat>
#include <cmath>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>

/**
 * @brief traceRay: traces a single ray and tracks intersections with implicitly
 * defined objects in a scene
 * @param position: starting position of the ray
 * @param direction: direction of the ray
 * @param scene: information about the scene
 * @param config: configuration of the raytracer
 * @param completedReflections: how many reflections the current ray has already
 * undergone
 * @param time: with potential object movement
 * @return the color in the scene that the ray hits
 */
RGBA traceRay(glm::vec4 position, glm::vec4 direction,
              const RayTraceScene &scene, const RayTracer::Config &config,
              int completedReflections, double time) {
    // default return color is black
    RGBA toReturnColor = RGBA{0, 0, 0};

    // include variables to store information about minimum
    bool hitObject = false;
    float minT = FLT_MAX;
    glm::mat4 minTCTM;
    PrimitiveType minTType;
    SceneMaterial minTMaterial;
    glm::mat4 inverseMinTCTM;
    glm::vec3 min_center2;
    PrimitiveType shapeType;

    // go through each shape and get minimum intersection
    for (const RenderShapeData &primitiveShape : scene.getShapes()) {
        // transform ray into object space
        // FIX FROM INTERSECT MENTOR MEETING: I am no longer truncating the ctm
        // inverse here for direction
        glm::vec4 objectPosition = primitiveShape.inverseCTM * position;
        glm::vec4 objectDirection = primitiveShape.inverseCTM * direction;

        // define starting minimum and go through each shape
        float potentialMinT = -1;
        switch (primitiveShape.primitive.type) {
        case PrimitiveType::PRIMITIVE_CUBE: {
            potentialMinT =
                CubeIntersect(objectPosition, objectDirection).getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_CONE:
            potentialMinT =
                ConeIntersect(objectPosition, objectDirection).getIntersection();
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            potentialMinT =
                CylinderIntersect(objectPosition, objectDirection).getIntersection();
            break;
        case PrimitiveType::PRIMITIVE_SPHERE: {
            potentialMinT =
                SphereIntersect(objectPosition, objectDirection).getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_SPHERE_MOVING: {
            potentialMinT =
                movingSphereIntersect(objectPosition, objectDirection, time,
                                                  primitiveShape.primitive.center2)
                                .getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_CUBE_MOVING: {
            potentialMinT = movingCubeIntersect(objectPosition, objectDirection, time,
                                                primitiveShape.primitive.center2)
                                .getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_MESH:
            // unimplemented
            break;
        }

        // if a new minimum was found, update the stored information
        if (potentialMinT != -1.f && potentialMinT < minT) {
            hitObject = true;
            minT = potentialMinT;
            minTCTM = primitiveShape.ctm;
            minTType = primitiveShape.primitive.type;
            minTMaterial = primitiveShape.primitive.material;
            inverseMinTCTM = primitiveShape.inverseCTM;
            if (PrimitiveType::PRIMITIVE_SPHERE_MOVING ==
                    primitiveShape.primitive.type ||
                PrimitiveType::PRIMITIVE_CUBE_MOVING ==
                                                                                               primitiveShape.primitive.type ||
                PrimitiveType::PRIMITIVE_CONE_MOVING ==
                                                                                                                                                                        primitiveShape.primitive.type ||
                PrimitiveType::PRIMITIVE_CYLINDER_MOVING ==
                                                                                                                                                                                                                                                 primitiveShape.primitive.type) {
                min_center2 = primitiveShape.primitive.center2;
            }
        }
    }

    // if an object was hit, do the lighting computation
    if (hitObject) {
        // get object position and direction
        glm::vec4 objectNormal;
        glm::vec4 objectPosition = inverseMinTCTM * position;
        glm::vec4 objectDirection = inverseMinTCTM * direction;

        // compute normal based on type of shape
        switch (minTType) {
        case PrimitiveType::PRIMITIVE_CUBE: {
            objectNormal =
                CubeNormal(objectPosition + minT * objectDirection).getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_CONE: {
            objectNormal =
                ConeNormal(objectPosition + minT * objectDirection).getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_CYLINDER: {
            objectNormal = CylinderNormal(objectPosition + minT * objectDirection)
                               .getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_SPHERE: {
            objectNormal = SphereNormal(objectPosition + minT * objectDirection)
                               .getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_SPHERE_MOVING: {
            objectNormal =
                movingSphereNormal((objectPosition + minT * objectDirection), time,
                                              min_center2)
                               .getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_CUBE_MOVING: {
            objectNormal = movingCubeNormal((objectPosition + minT * objectDirection),
                                            time, min_center2)
                               .getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_MESH:
            // unimplemented
            break;
        case PrimitiveType::PRIMITIVE_CONE_MOVING:
            // unimplemented
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER_MOVING:
            // unimplemented
            break;
        }

        // transform the normal into world space, ensure proper direction
        glm::vec3 normal = glm::inverse(glm::transpose(glm::mat3(minTCTM))) *
                           glm::vec3(objectNormal);
        if (glm::dot(normal, glm::vec3(-direction)) < 0) {
            normal = -normal;
        }

        // compute the lighting
        toReturnColor =
            phong(position + minT * direction, glm::vec4(normal, 0), -direction,
                              minTMaterial, scene.getLights(), scene.getGlobalData(), scene,
                              config, completedReflections, minTType,
                              objectPosition + minT * objectDirection, time);
    }

    // return the color hit by the ray
    return toReturnColor;
}

/**
 * @brief traceShadowRay: traces a single ray and tracks intersections with
 * implicitly defined objects in a scene
 * @param position: starting position of the ray
 * @param direction: direction of the ray
 * @param scene: information about the scene
 * @param time: with potential object movement
 * @return the distance of the closest object from the current one
 */
float traceShadowRay(glm::vec4 position, glm::vec4 direction,
                     const RayTraceScene &scene, double time) {
    // default return value if no objects are hit
    float toReturnDistance = -1.f;

    // include variables to store information about minimum
    bool hitObject = false;
    float minT = FLT_MAX;

    // go through each shape and get minimum intersection
    for (const RenderShapeData &primitiveShape : scene.getShapes()) {
        // transform ray into object space
        glm::vec4 objectPosition = primitiveShape.inverseCTM * position;
        glm::vec4 objectDirection = primitiveShape.inverseCTM * direction;

        // define starting minimum and go through each shape
        float potentialMinT = -1;
        switch (primitiveShape.primitive.type) {
        case PrimitiveType::PRIMITIVE_CUBE: {
            potentialMinT =
                CubeIntersect(objectPosition, objectDirection).getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_CONE:
            potentialMinT =
                ConeIntersect(objectPosition, objectDirection).getIntersection();
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            potentialMinT =
                CylinderIntersect(objectPosition, objectDirection).getIntersection();
            break;
        case PrimitiveType::PRIMITIVE_SPHERE: {
            potentialMinT =
                SphereIntersect(objectPosition, objectDirection).getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_SPHERE_MOVING: {
            potentialMinT =
                movingSphereIntersect(objectPosition, objectDirection, time,
                                                  primitiveShape.primitive.center2)
                                .getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_CUBE_MOVING: {
            potentialMinT = movingCubeIntersect(objectPosition, objectDirection, time,
                                                primitiveShape.primitive.center2)
                                .getIntersection();
            break;
        }
        case PrimitiveType::PRIMITIVE_MESH:
            break;
        }

        // if a new minimum was found, update the stored information
        if (potentialMinT != -1.f && potentialMinT < minT) {
            hitObject = true;
            minT = potentialMinT;
        }
    }

    // if an object was hit, do the lighting computation
    if (hitObject) {
        toReturnDistance = glm::length(minT * direction);
    }

    // return the distance to nearest object
    return toReturnDistance;
}
