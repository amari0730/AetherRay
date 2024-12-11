#include "shapeoverall.h"
#include <cmath>

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
