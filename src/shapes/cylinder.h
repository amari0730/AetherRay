#include "shapeoverall.h"
#include <cmath>
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
