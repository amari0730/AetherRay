#include "shapeoverall.h"
#include <cmath>
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
 * @brief CubeIntersect: finds intersection point between a ray and cube with
 * side length 1
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */
auto movingCubeIntersect(glm::vec3 point, glm::vec3 direction, double time, glm::vec3 center2) {
    return ShapeIntersect{[=]() {
        // define useful constants and vector of potential t values


        float dx = direction[0], dy = direction[1], dz = direction[2];
        glm::vec3 center_direc = (center2 - glm::vec3(0, 0, 0)); // assuming all are centered in the origin in object space.
        glm::vec3 new_center = glm::vec3(0, 0, 0) + center_direc * (float) time;
        float px = point[0] - new_center[0], py = point[1] - new_center[1], pz = point[2] - new_center[2];
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
auto movingCubeNormal(glm::vec3 intersection, double time, glm::vec3 center2) {
    return ShapeNormal{[=]() {
        // define constants
        glm::vec3 center_direc = (center2 - glm::vec3(0, 0, 0)); // assuming all are centered in the origin in object space.
        glm::vec3 new_center = glm::vec3(0, 0, 0) + center_direc * (float)time;
        float x = intersection[0] - new_center[0], y = intersection[1] - new_center[1], z = intersection[2] - new_center[2];
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





