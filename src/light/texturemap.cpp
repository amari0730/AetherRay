#include "texturemap.h"
#include "../shapes/shapeoverall.h"
#include "utils/scenedata.h"
#include <cmath>
#include <iostream>
#include <numbers>
#include <ostream>

/**
 * @brief SphereUV: gets UV coordinates based on intersection with a sphere
 * @param intersection: intersection point on the sphere in object space
 * @return a tuple with the u and v coordiantes to use for texture mapping
 */
auto SphereUV(glm::vec3 intersection) {
    return ShapeUV{[=]() {
        // compute the UV coordinates
        float phi = asin(intersection.y / 0.5f);
        float v = phi / std::numbers::pi + 0.5f;
        float u;
        if (withinEpsilon(v, 1.f) ||
            withinEpsilon(v, 0.f)) // collapse into singularity, just use 0.5f
            u = 0.5f;
        else { // standard case
            float theta = atan2(intersection.z, intersection.x);
            if (theta < 0)
                u = -theta / (2.f * std::numbers::pi);
            else // theta >= 0
                u = 1 - (theta / (2.f * std::numbers::pi));
        }
        return std::tuple{u, v};
    }};
}

/**
 * @brief CubeUV: gets UV coordinates based on intersection with a cube
 * @param intersection: intersection point on the cube in object space
 * @return a tuple with the u and v coordiantes to use for texture mapping
 */
auto CubeUV(glm::vec3 intersection) {
    return ShapeUV{[=]() {
        // define constants
        float x = intersection[0], y = intersection[1], z = intersection[2];

        // compute the UV coordinates
        if (withinEpsilon(x, 0.5f))
            return std::tuple(-z + 0.5f, y + 0.5f);
        else if (withinEpsilon(x, -0.5f))
            return std::tuple(z + 0.5f, y + 0.5f);
        else if (withinEpsilon(y, 0.5))
            return std::tuple(x + 0.5f, -z + 0.5f);
        else if (withinEpsilon(y, -0.5))
            return std::tuple(x + 0.5f, z + 0.5f);
        else if (withinEpsilon(z, 0.5))
            return std::tuple(x + 0.5f, y + 0.5f);
        else if (withinEpsilon(z, -0.5))
            return std::tuple(-x + 0.5f, y + 0.5f);
        else // should never reach here
            return std::tuple(-1.f, -1.f);
    }};
}

/**
 * @brief ConeUV: gets UV coordinates based on intersection with a cone
 * @param intersection: intersection point on the cone in object space
 * @return a tuple with the u and v coordiantes to use for texture mapping
 */
auto ConeUV(glm::vec3 intersection) {
    return ShapeUV{[=]() {
        // define constants
        float x = intersection[0], y = intersection[1], z = intersection[2];

        // compute the UV coordinates
        if (withinEpsilon(y, -0.5))
            return std::tuple(x + 0.5f, z + 0.5f);
        else if (withinEpsilon(x * x + z * z,
                               (((0.5 - y) / 2.f) * (0.5 - y) / 2.f))) {
            float v = y + 0.5f;
            float u;
            if (withinEpsilon(v, 1.f)) // collapse into singularity, just use 0.5f
                u = 0.5f;
            else { // standard case
                float theta = atan2(intersection.z, intersection.x);
                if (theta < 0)
                    u = -theta / (2.f * std::numbers::pi);
                else // theta >= 0
                    u = 1 - (theta / (2.f * std::numbers::pi));
            }
            return std::tuple{u, v};
        } else // should never reach here
            return std::tuple(-1.f, -1.f);
    }};
}

/**
 * @brief CylinderUV: gets UV coordinates based on intersection with a cylinder
 * @param intersection: intersection point on the cylinder in object space
 * @return a tuple with the u and v coordiantes to use for texture mapping
 */
auto CylinderUV(glm::vec3 intersection) {
    return ShapeUV{[=]() {
        // define constants
        float x = intersection[0], y = intersection[1], z = intersection[2];

        // compute the UV coordinates
        if (withinEpsilon(y, -0.5f))
            return std::tuple(x + 0.5f, z + 0.5f);
        else if (withinEpsilon(y, 0.5f))
            return std::tuple(x + 0.5f, -z + 0.5f);
        else if (withinEpsilon(x * x + z * z, 0.25f)) {
            float phi = asin(intersection.y / 0.5f);
            float v = y + 0.5f;
            float u;
            float theta = atan2(intersection.z, intersection.x);
            if (theta < 0)
                u = -theta / (2.f * std::numbers::pi);
            else // theta >= 0
                u = 1 - (theta / (2.f * std::numbers::pi));
            return std::tuple{u, v};
        } else // should never reach here
            return std::tuple(-1.f, -1.f);
    }};
}

/**
 * @brief getShapeUV: gets the u and v coordinates for texture mapping based on
 * the shape type
 * @param shapeType: type of the shape for which to compute the u and v
 * coordinates
 * @param intersection: point of intersection with the shape in object space
 * @return the u and v coordinates of the intersection with the shape
 */
std::tuple<float, float> getShapeUV(PrimitiveType shapeType,
                                    glm::vec3 intersection) {
    switch (shapeType) {
    case PrimitiveType::PRIMITIVE_CUBE: {
        return CubeUV(intersection).getUV();
    }
    case PrimitiveType::PRIMITIVE_CONE: {
        return ConeUV(intersection).getUV();
    }
    case PrimitiveType::PRIMITIVE_CYLINDER: {
        return CylinderUV(intersection).getUV();
    }
    case PrimitiveType::PRIMITIVE_SPHERE: {
        return SphereUV(intersection).getUV();
    }
    case PrimitiveType::PRIMITIVE_MESH: {
        // unimplemented
        break;
    }
    }
    // default return in error
    return std::tuple(-1.f, -1.f);
}
