#ifndef SHAPEOVERALL_H
#define SHAPEOVERALL_H

#include <functional>
#include <glm/glm.hpp>

bool withinEpsilon(float num1, float num2);

// use namespaces for generic functions
using GetIntersection = auto() -> float;
using GetObjectNormal = auto() -> glm::vec4;
using GetUV = auto() -> std::tuple<float, float>;

/**
 * @brief The ShapeIntersect class: interface for getting intersection with a
 * shape
 */
struct ShapeIntersect {
    std::function<GetIntersection> getIntersection;
};

/**
 * @brief The ShapeNormal class: interface for getting normal vector at point on
 * a shape
 */
struct ShapeNormal {
    std::function<GetObjectNormal> getObjectNormal;
};

/**
 * @brief The ShapeUV class: interface for getting UV coordinates at a point on
 * the given shape
 */
struct ShapeUV {
    std::function<GetUV> getUV;
};

#endif // SHAPEOVERALL_H
