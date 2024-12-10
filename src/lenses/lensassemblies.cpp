#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <tuple>
#include <vector>

// Lens constants
typedef struct Lens {
    float lensRadius;
    float eta;
    float thickness; // representing distance in assembly from previous element
        // to this one
} lens_t;

lens_t globalLens1 = {1, 1.2, 3};
lens_t globalLens2 = {5, 5, 5};
// std::vector<lens_t> lenses = {globalLens1};
std::vector<lens_t> lenses = {globalLens1, globalLens2};

typedef struct Aperture {
    float apertureRadius;
} aperture_t;

Aperture aperture = {0.5};

/**
 * @brief SphereIntersect: finds intersection points between a ray and radius
 * 0.5 sphere
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t value for the parameterized ray that intersects the sphere
 */
std::vector<float> LensSphereIntersect(glm::vec3 point, glm::vec3 direction,
                                       float radius, float zOffset) {
    // define useful constants
    float px = point[0], py = point[1],
        pz = point[2] - zOffset; // offset as moved back along z axis starting
        // position essentially

    float dx = direction[0], dy = direction[1], dz = direction[2];
    // std::cout << px << std::endl;
    // std::cout << py << std::endl;
    // std::cout << pz << std::endl;
    // std::cout << "just did p" << std::endl;
    // std::cout << dx << std::endl;
    // std::cout << dy << std::endl;
    // std::cout << dz << std::endl;
    // std::cout << "just did d" << std::endl;
    // collect all t values
    std::vector<float> tValues = {};

    // define quadratic formula values and the discriminant
    float A = dx * dx + dy * dy + dz * dz;
    float B = 2.f * px * dx + 2.f * py * dy + 2.f * pz * dz;
    float C = px * px + py * py + pz * pz - (radius * radius);
    float D = B * B - 4.f * A * C;
    if (D > 0) { // two intersection points
        float t1 = (-B + sqrt(D)) / (2.f * A);
        float t2 = (-B - sqrt(D)) / (2.f * A);
        // first three cases should never happen here
        if (t1 < 0.f && t2 < 0.f) {
        } else if (t1 < 0.f) {
            tValues.push_back(t2);
        } else if (t2 < 0.f) {
            tValues.push_back(t1);
        } else { // both intersections are valid
            tValues.push_back(t1);
            tValues.push_back(t2);
        }
    } else if (D < 0) { // zero intersection points (should not happen here)
    } else {            // D = 0 // one intersection point
        float potentialT = (float)(-B) / (2 * A);
        if (potentialT >= 0.f) // ensure nonegative t value
            tValues.push_back(potentialT);
    }
    return tValues;
}

/**
 * @brief SphereNormal: computes the normal vector for a sphere in object space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
glm::vec4 LensSphereNormal(glm::vec3 intersection) {
    // compute and return the object normal
    return glm::vec4(2.f * intersection[0], 2.f * intersection[1],
                     2.f * intersection[2], 0.f);
}

// refraction and direction transmittance
glm::vec3 refraction(glm::vec3 incidentVector, glm::vec3 normalVector,
                     float etaRatio, float cosineThetaIncident,
                     float cosineThetaTransmit) {
    glm::vec3 transmittedDirection =
        etaRatio * -incidentVector +
                                     (etaRatio * cosineThetaIncident - cosineThetaTransmit) * normalVector;
    return transmittedDirection;
}

std::tuple<glm::vec3, glm::vec3, bool>
computeLensAdjustedDirection(glm::vec3 initialDirection,
                             glm::vec3 initialPosition, Lens &lens,
                             float cumulativeThickness) {
    // get values for refraction purposes
    // std::cout << initialDirection.x << std::endl;
    // std::cout << initialDirection.y << std::endl;
    // std::cout << initialDirection.z << std::endl;
    // std::cout << "" << std::endl;
    // std::cout << initialPosition.x << std::endl;
    // std::cout << initialPosition.y << std::endl;
    // std::cout << initialPosition.z << std::endl;
    // std::cout << "" << std::endl;

    std::vector<float> tValues =
        LensSphereIntersect(initialPosition, initialDirection, lens.lensRadius,
                                                     cumulativeThickness + lens.thickness);
    // std::cout << tValues.size() << std::endl;
    // std::cout << tValues[0] << std::endl;
    // std::cout << tValues[1] << std::endl;

    if (tValues.size() > 0) {
        float t = lens.lensRadius > 0
                      ? *std::min_element(tValues.begin(), tValues.end())
                      : *std::max_element(tValues.begin(), tValues.end());
        glm::vec3 intersectionPoint = initialDirection * t + initialPosition;
        // std::cout << "t:" << t << std::endl;
        // std::cout << "intersection" << std::endl;
        // std::cout << intersectionPoint.x << std::endl;
        // std::cout << intersectionPoint.y << std::endl;
        // std::cout << intersectionPoint.z << std::endl;

        glm::vec3 lensNormal = LensSphereNormal(
            intersectionPoint -
            (glm::vec3(0, 0, cumulativeThickness + lens.thickness)));
        // std::cout << "lensnormal" << std::endl;
        // std::cout << lensNormal.x << std::endl;
        // std::cout << lensNormal.y << std::endl;
        // std::cout << lensNormal.z << std::endl;

        float cosineThetaIncident = glm::dot(-initialDirection, lensNormal);
        float etaRatio =
            1.0 / lens.eta; // implicit assumption going from air to lens

        // use Snell's law to get the new direction (see citation)
        float sineSquaredThetaIncidentInitial =
            std::max(0.f, 1.f - cosineThetaIncident * cosineThetaIncident);
        float sineSquaredThetaIncidentAdjusted =
            etaRatio * etaRatio * sineSquaredThetaIncidentInitial;
        float cosineThetaTransmit = std::sqrt(1 - sineSquaredThetaIncidentAdjusted);
        // std::cout << "\nangles" << std::endl;
        // std::cout << cosineThetaIncident << std::endl;
        // std::cout << sineSquaredThetaIncidentInitial << std::endl;
        // std::cout << sineSquaredThetaIncidentAdjusted << std::endl;
        // std::cout << cosineThetaTransmit << std::endl;
        // std::cout << "angles\n" << std::endl;

        glm::vec3 newDirection = refraction(
            glm::normalize(-initialDirection), glm::normalize(lensNormal), etaRatio,
            cosineThetaIncident, cosineThetaTransmit);
        return {newDirection, intersectionPoint, true};
    }
    return {glm::vec3(0), glm::vec3(0), false};
}

std::tuple<glm::vec3, glm::vec3, bool>
computeLensesAdjustedDirection(glm::vec3 initialDirection) {
    glm::vec3 currentDirection = initialDirection;
    glm::vec3 currentPosition = glm::vec3(0);
    float cumulativeThickness = 0.f;
    for (lens_t &lens : lenses) {
        auto [nextDirection, nextPosition, stillInLens] =
            computeLensAdjustedDirection(currentDirection, currentPosition, lens,
                                                                                       cumulativeThickness);
        if (!stillInLens)
            return {glm::vec3(0), glm::vec3(0), false};
        currentDirection = nextDirection;
        currentPosition = nextPosition;
        cumulativeThickness += lens.thickness;
    }
    return {currentDirection, currentPosition, true};
}

// Citations

// refraction:
// https://www.pbr-book.org/3ed-2018/Reflection_Models/Specular_Reflection_and_Transmission#Refract

// lenses: http://mentallandscape.com/Papers_siggraph95.pdf and
// https://www.pbr-book.org/3ed-2018/Camera_Models/Realistic_Cameras#fragment-RealisticCameraPrivateMethods-2
