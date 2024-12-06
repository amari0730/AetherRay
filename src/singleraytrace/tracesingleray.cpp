#include "tracesingleray.h"
#include "../light/lighting.h"
#include "../shapes/shapeoverall.h"
#include "../utils/scenedata.h"
#include "raytracer/raytracescene.h"
#include <cfloat>
#include <cmath>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>

/**
 * @brief SphereIntersect: finds intersection point between a ray and radius 0.5
 * sphere
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */
auto SphereIntersect(glm::vec3 point, glm::vec3 direction) {
    return ShapeIntersect{[=]() {
        // define useful constants
        float px = point[0], py = point[1], pz = point[2];
        float dx = direction[0], dy = direction[1], dz = direction[2];

        // define quadratic formula values and the discriminant
        float A = dx * dx + dy * dy + dz * dz;
        float B = 2.f * px * dx + 2.f * py * dy + 2.f * pz * dz;
        float C = px * px + py * py + pz * pz - (0.5 * 0.5);
        float D = B * B - 4.f * A * C;
        if (D > 0) { // two intersection points
            float t1 = (-B + sqrt(D)) / (2.f * A);
            float t2 = (-B - sqrt(D)) / (2.f * A);
            if (t1 < 0.f && t2 < 0.f) {
                return -1.f;
            } else if (t1 < 0.f) {
                return t2;
            } else if (t2 < 0.f) {
                return t1;
            } else { // both intersections are valid
                return std::min(t1, t2);
            }
            return (float)std::min(t1, t2);
        } else if (D < 0) { // zero intersection points
            return -1.f;
        } else { // D = 0 // one intersection point
            float potentialT = (float)(-B) / (2 * A);
            return potentialT >= 0.f ? potentialT : -1.f; // ensure nonegative t value
        }
    }};
}

/**
 * @brief SphereNormal: computes the normal vector for a sphere in object space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
auto SphereNormal(glm::vec3 intersection) {
    return ShapeNormal{[=]() {
        // compute and return the object normal
        return glm::vec4(2.f * intersection[0], 2.f * intersection[1],
                         2.f * intersection[2], 0.f);
    }};
}


/**
 * @brief SphereIntersect: finds intersection point between a ray and radius 0.5
 * sphere
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */

float getDiscriminant(float a, float b, float c) {
    return b*b - 4.f*a*c;
}

auto movingSphereIntersect(glm::vec3 point, glm::vec3 direction, double time, glm::vec3 center2) {
    return ShapeIntersect{[=]() {
        // define useful constants
        // compute the current center.
        glm::vec3 center_direc = (center2 - glm::vec3(0, 0, 0)); // assuming all are centered in the origin in object space.
        glm::vec3 new_center = glm::vec3(0, 0, 0) + center_direc * (float) time;

        glm::vec3 y = (point - new_center); // center is (0, 0, 0)
        // std::cout << radius << '\n';
        float radius = 0.5f;

        float a = glm::dot(direction, direction);
        float b = 2.f*(glm::dot(y, direction));
        float c = glm::dot(y, y) - radius * radius;

        float dis = getDiscriminant(a, b, c);
        if (dis < 0) {
            return -1.f;
        }
        float sqrt_dis = std::sqrt(dis);

        float t1 = (-b - sqrt_dis) / (2 * a);
        float t2 = (-b + sqrt_dis) / (2 * a);

        if (t1 >= 0) {
            return t1;
        }
        else if (t2 >= 0) {
            return t2;
        }
        return -1.f;
    }};
}

/**
 * @brief SphereNormal: computes the normal vector for a sphere in object space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
auto movingSphereNormal(glm::vec3 intersection, double time, glm::vec3 center2) {
    return ShapeNormal{[=]() {
        // using time compute the current center.
        // compute and return the object normal
        glm::vec3 center_direc = (center2 - glm::vec3(0, 0, 0)); // assuming all are centered in the origin in object space.
        glm::vec3 new_center = glm::vec3(0, 0, 0) + center_direc * (float)time;
        return glm::vec4(glm::normalize(intersection - new_center), 0.f);
    }};
}

/**
 * @brief CubeIntersect: finds intersection point between a ray and cube with
 * side length 1
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */
auto CubeIntersect(glm::vec3 point, glm::vec3 direction) {
    return ShapeIntersect{[=]() {
        // define useful constants and vector of potential t values
        float px = point[0], py = point[1], pz = point[2];
        float dx = direction[0], dy = direction[1], dz = direction[2];
        std::vector<float> tValues = std::vector<float>();

        // computations for the six planes of the cube
        // checks: division by zero, t values greater than zero, and bounds
        // x planes computation
        if (dx != 0) {
            float t1 = (0.5 - px) / dx;
            if (((py + dy * t1) <= 0.5 && (pz + dz * t1) <= 0.5) &&
                (py + dy * t1) >= -0.5 && (pz + dz * t1) >= -0.5) {
                if (t1 >= 0) {
                    tValues.push_back(t1);
                }
            }

            float t4 = (-0.5 - px) / dx;
            if (((py + dy * t4) <= 0.5 && (pz + dz * t4) <= 0.5) &&
                (py + dy * t4) >= -0.5 && (pz + dz * t4) >= -0.5) {
                if (t4 >= 0) {
                    tValues.push_back(t4);
                }
            }
        }

        // y planes computations
        if (dy != 0) {
            float t2 = (0.5 - py) / dy;
            if (((pz + dz * t2) <= 0.5 && (px + dx * t2) <= 0.5) &&
                ((pz + dz * t2) >= -0.5 && (px + dx * t2) >= -0.5)) {
                if (t2 >= 0) {
                    tValues.push_back(t2);
                }
            }

            float t5 = (-0.5 - py) / dy;
            if (((pz + dz * t5) <= 0.5 && (px + dx * t5) <= 0.5) &&
                ((pz + dz * t5) >= -0.5 && (px + dx * t5) >= -0.5)) {
                if (t5 >= 0) {
                    tValues.push_back(t5);
                }
            }
        }

        // z planes computations
        if (dz != 0) {
            float t3 = (0.5 - pz) / dz;
            if (((px + dx * t3) <= 0.5 && (py + dy * t3) <= 0.5) &&
                ((px + dx * t3) >= -0.5 && (py + dy * t3) >= -0.5)) {
                if (t3 >= 0) {
                    tValues.push_back(t3);
                }
            }

            float t6 = (-0.5 - pz) / dz;
            if (((px + dx * t6) <= 0.5 && (py + dy * t6) <= 0.5) &&
                ((px + dx * t6) >= -0.5 && (py + dy * t6) >= -0.5)) {
                if (t6 >= 0) {
                    tValues.push_back(t6);
                }
            }
        }

        // if no t values, return -1 to incdicate no intersection, or return the min
        if (tValues.empty())
            return -1.f;
        else
            return *std::min_element(tValues.begin(), tValues.end());
    }};
}

/**
 * @brief CubeNormal: computes the normal vector for a cube in object space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
auto CubeNormal(glm::vec3 intersection) {
    return ShapeNormal{[=]() {
        // define constants
        float x = intersection[0], y = intersection[1], z = intersection[2];

        // determine which part of cube was hit and return the corrosponding normal
        if (withinEpsilon(x, 0.5))
            return glm::vec4(1.f, 0.f, 0.f, 0.f);
        else if (withinEpsilon(x, -0.5))
            return glm::vec4(-1.f, 0.f, 0.f, 0.f);
        else if (withinEpsilon(y, 0.5))
            return glm::vec4(0.f, 1.f, 0.f, 0.f);
        else if (withinEpsilon(y, -0.5))
            return glm::vec4(0.f, -1.f, 0.f, 0.f);
        else if (withinEpsilon(z, 0.5))
            return glm::vec4(0.f, 0.f, 1.f, 0.f);
        else if (withinEpsilon(z, -0.5))
            return glm::vec4(0.f, 0.f, -1.f, 0.f);
        else // should never reach here
            return glm::vec4(0.f, 0.f, 0.f, 0.f);
    }};
}

/**
 * @brief ConeIntersect: finds intersection point between a ray and cone with
 * height 1 and base radius 0.5
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */
auto ConeIntersect(glm::vec3 point, glm::vec3 direction) {
    return ShapeIntersect{[=]() {
        // vector of t values to consider
        std::vector<float> tValues = std::vector<float>();

        // define useful constants
        float px = point[0], py = point[1], pz = point[2];
        float dx = direction[0], dy = direction[1], dz = direction[2];

        // conical top
        // relevant algo section equation: A = dx^2 + dz^2 - 0.25dy^2
        float A = dx * dx + dz * dz - (0.25 * dy * dy);
        // relevant algo section equation: B = 2pxdx + 2pzdz - 0.5pydy + 0.24dy
        float B = 2.f * px * dx + 2.f * pz * dz - (0.5 * py * dy) + (0.25 * dy);
        // relevant algo section equation: C = px^2 + py^2 - 0.25py^2 + 0.25py -
        // 1/16
        float C = px * px + pz * pz - (0.25 * py * py) + (0.25 * py) - (1.f / 16.f);
        // get the discriminant
        float D = B * B - 4.f * A * C;

        if (D > 0) { // two intersection points
            float t1 = (-B + sqrt(D)) / (2 * A);
            float t2 = (-B - sqrt(D)) / (2 * A);
            // relevant algo section bounds check: -0.5 <= y <= 0.5
            if (py + dy * t1 <= 0.5 && py + dy * t1 >= -0.5) {
                if (t1 >= 0) {
                    tValues.push_back(t1);
                }
            }
            // relevant algo section bounds check: -0.5 <= y <= 0.5
            if (py + dy * t2 <= 0.5 && py + dy * t2 >= -0.5) {
                if (t2 >= 0) {
                    tValues.push_back(t2);
                }
            }
        } else if (D == 0) { // one intersection point
            float potentialT = (float)(-B) / (2 * A);
            // relevant algo section bounds check: -0.5 <= y <= 0.5
            if (py + dy * potentialT <= 0.5 && py + dy * potentialT >= -0.5) {
                if (potentialT >= 0) {
                    tValues.push_back(potentialT);
                }
            }
        }

        // flat base
        // relevant algo section equaton: t = (-0.5 - py) / dy
        float potentialTFlatBase = (-0.5f - py) / dy;
        // relevant algo section bounds check: x^2 + y^2 < 0.5^2
        if (((px + dx * potentialTFlatBase) * (px + dx * potentialTFlatBase) +
             (pz + dz * potentialTFlatBase) * (pz + dz * potentialTFlatBase)) <=
            (0.5 * 0.5)) {
            if (potentialTFlatBase >= 0) {
                tValues.push_back(potentialTFlatBase);
            }
        }

        // if no t values, return -1 to incdicate no intersection, or return the min
        if (tValues.empty())
            return -1.f;
        else
            return *std::min_element(tValues.begin(), tValues.end());
    }};
}

/**
 * @brief ConeNormal: computes the normal vector for a cone in object space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
auto ConeNormal(glm::vec3 intersection) {
    return ShapeNormal{[=]() {
        // define constants
        float x = intersection[0], y = intersection[1], z = intersection[2];

        // check which part of cone is hit and return the normal
        if (withinEpsilon(y, -0.5))
            return glm::vec4(0.f, -1.f, 0.f, 0.f);
        else if (withinEpsilon(x * x + z * z,
                               (((0.5 - y) / 2.f) * (0.5 - y) / 2.f)))
            return glm::vec4(2.f * x, 0.25 - 0.5 * y, 2.f * z, 0.f);
        else // should never reach here
            return glm::vec4(0.f, 0.f, 0.f, 0.f);
    }};
}

/**
 * @brief CylinderIntersect: finds intersection point between a ray and a
 * cylinder with top and bottom radius 0.5 and height 1
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */
auto CylinderIntersect(glm::vec3 point, glm::vec3 direction) {
    return ShapeIntersect{[=]() {
        // vector of t values to consider
        std::vector<float> tValues = std::vector<float>();

        // define useful constants
        float px = point[0], py = point[1], pz = point[2];
        float dx = direction[0], dy = direction[1], dz = direction[2];

        // infinite cylinder part top
        // overall equation from lecture: x^2 + z^2 = 0.5^2
        float A = dx * dx + dz * dz;
        float B = 2.f * px * dx + 2.f * pz * dz;
        float C = px * px + pz * pz - (0.5 * 0.5);
        float D = B * B - 4.f * A * C;

        if (D > 0) { // two intersection points
            float t1 = (-B + sqrt(D)) / (2 * A);
            float t2 = (-B - sqrt(D)) / (2 * A);
            // relevant lecture bounds check: -0.5 <= y <= 0.5
            if (py + dy * t1 <= 0.5 && py + dy * t1 >= -0.5) {
                if (t1 >= 0) {
                    tValues.push_back(t1);
                }
            }
            // relevant lecture  bounds check:
            if (py + dy * t2 <= 0.5 && py + dy * t2 >= -0.5) {
                if (t2 >= 0) {
                    tValues.push_back(t2);
                }
            }
        } else if (D == 0) { // one intersection point
            float potentialT = (float)(-B) / (2 * A);
            // relevant lecture bounds check:
            if (py + dy * potentialT <= 0.5 && py + dy * potentialT >= -0.5) {
                if (potentialT >= 0) {
                    tValues.push_back(potentialT);
                }
            }
        }

        // bottom cap base
        // relevant lecture equaton:
        float potentialBottomBase = (-0.5f - py) / dy;
        if ((px + dx * potentialBottomBase) * (px + dx * potentialBottomBase) +
                (pz + dz * potentialBottomBase) * (pz + dz * potentialBottomBase) <=
            (0.5 * 0.5)) {
            if (potentialBottomBase >= 0) {
                tValues.push_back(potentialBottomBase);
            }
        }

        // top cap base
        // relevant lecture equaton:
        float potentialTopBase = (0.5f - py) / dy;
        if ((px + dx * potentialTopBase) * (px + dx * potentialTopBase) +
                (pz + dz * potentialTopBase) * (pz + dz * potentialTopBase) <=
            (0.5 * 0.5)) {
            if (potentialTopBase >= 0) {
                tValues.push_back(potentialTopBase);
            }
        }

        // if no t values, return -1 to incdicate no intersection, or return the min
        if (tValues.empty())
            return -1.f;
        else
            return *std::min_element(tValues.begin(), tValues.end());
    }};
}

/**
 * @brief CylinderNormal: computes the normal vector for a cylinder in object
 * space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
auto CylinderNormal(glm::vec3 intersection) {
    return ShapeNormal{[=]() {
        // define constants
        float x = intersection[0], y = intersection[1], z = intersection[2];

        // check which part of the cylinder is hit and return the normal vector
        if (withinEpsilon(y, -0.5f))
            return glm::vec4(0.f, -1.f, 0.f, 0.f);
        else if (withinEpsilon(y, 0.5f))
            return glm::vec4(0.f, 1.f, 0.f, 0.f);
        else if (withinEpsilon(x * x + z * z, 0.25f))
            return glm::vec4(2.f * x, 0, 2.f * z, 0.f);
        else // should never reach here
            return glm::vec4(0.f, 0.f, 0.f, 0.f);
    }};
}

/**
 * @brief traceRay: traces a single ray and tracks intersections with implicitly
 * defined objects in a scene
 * @param position: starting position of the ray
 * @param direction: direction of the ray
 * @param scene: information about the scene
 * @param config: configuration of the raytracer
 * @param completedReflections: how many reflections the current ray has already
 * undergone
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
            potentialMinT = movingSphereIntersect(objectPosition, objectDirection, time, primitiveShape.primitive.center2).getIntersection();
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
            if (PrimitiveType::PRIMITIVE_SPHERE_MOVING == primitiveShape.primitive.type) {
                min_center2 = primitiveShape.primitive.center2;
            }
        }
    }

    // if an object was hit, do the lighting computation
    if (hitObject) {
        // get object position and direction
        // TODO: store inverse of the CTM
        // TODO: store inverse of the view matrix
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
            objectNormal = movingSphereNormal((objectPosition + minT * objectDirection), time, min_center2).getObjectNormal();
            break;
        }
        case PrimitiveType::PRIMITIVE_MESH:
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
 * @return the color in the scene that the ray hits
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
            potentialMinT = movingSphereIntersect(objectPosition, objectDirection, time, primitiveShape.primitive.center2).getIntersection();
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
