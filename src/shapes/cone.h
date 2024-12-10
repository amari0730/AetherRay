#include "shapeoverall.h"
#include <cmath>
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
