#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <tuple>
#include <vector>

// lens struct
typedef struct Lens {
    // radius of the spherical lens
    float lensRadius;
    // constant representing clarity of material
    float eta;
    // representing distance in assembly from previous element
    float thickness;
    // to this one
} lens_t;

// globally defined lenses
lens_t globalLens1 = {1.1, 1.2, 1.1};
lens_t globalLens2 = {1.5, 1.3, 4.5};
std::vector<lens_t> lenses = {globalLens1, globalLens2};

// support for separate aperture, checking if x^2 and y^2 less than or equal to
// r^2
typedef struct Aperture {
    float apertureRadius;
} aperture_t;

// globally defined aperture
Aperture aperture = {0.5};

/**
 * @brief LensSphereIntersect: finds intersection points between a ray and
 * radius 0.5 sphere
 * @param point: the starting point of the ray in object space
 * @param direction: the direction of the ray in object space
 * @return the t valuse for the parameterized ray that intersects the sphere
 */
std::vector<float> LensSphereIntersect(glm::vec3 point, glm::vec3 direction,
                                       float radius, float zOffset) {
    // define useful constants
    float px = point[0], py = point[1],
        pz = point[2] - zOffset; // offset as moved back along z axis starting
    // position essentially

    float dx = direction[0], dy = direction[1], dz = direction[2];
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
 * @brief LensSphereNormal: computes the normal vector for a sphere in object
 * space
 * @param intersection: the intersection point in object space
 * @return a normal vector to the object in object space
 */
glm::vec4 LensSphereNormal(glm::vec3 intersection) {
    // compute and return the object normal
    return glm::vec4(2.f * intersection[0], 2.f * intersection[1],
                     2.f * intersection[2], 0.f);
}

/**
 * @brief refraction: computes refracted vector out of a surface
 * @param incidentVector: negative of vector entering surface
 * @param normalVector: normal vector of surface at intersection point
 * @param etaRatio: ratio of eta_original / eta_new
 * @param cosineThetaIncident: cosine of incident angle
 * @param cosineThetaTransmit: cosine of transmittance angle
 * @return the transmitted refracted vector
 */
glm::vec3 refraction(glm::vec3 incidentVector, glm::vec3 normalVector,
                     float etaRatio, float cosineThetaIncident,
                     float cosineThetaTransmit) {
    glm::vec3 transmittedDirection =
        etaRatio * -incidentVector +
                                     (etaRatio * cosineThetaIncident - cosineThetaTransmit) * normalVector;
    return transmittedDirection;
}

/**
 * @brief computeLensAdjustedDirection: computes adjusted direction and position
 * after ray hits a lens
 * @param initialDirection: initial direction of the ray
 * @param initialPosition: initial position of the ray
 * @param lens: lens to send ray through
 * @param cumulativeThickness: cumulative distance away from origin along
 * positive z-axis
 * @return the updated position and direction as well as a boolean indicating if
 * the ray passed through the lens
 */
std::tuple<glm::vec3, glm::vec3, bool>
computeLensAdjustedDirection(glm::vec3 initialDirection,
                             glm::vec3 initialPosition, Lens &lens,
                             float cumulativeThickness) {

    std::vector<float> tValues =
        LensSphereIntersect(initialPosition, initialDirection, lens.lensRadius,
                                                     cumulativeThickness + lens.thickness);

    // if a valid t value exists, send the ray through the lens
    if (tValues.size() > 0) {
        float t = lens.lensRadius > 0
                      ? *std::min_element(tValues.begin(), tValues.end())
                      : *std::max_element(tValues.begin(), tValues.end());
        glm::vec3 intersectionPoint = initialDirection * t + initialPosition;

        // get normal, converting into lens space (essentially object space)
        glm::vec3 lensNormal = LensSphereNormal(
            intersectionPoint -
            (glm::vec3(0, 0, cumulativeThickness + lens.thickness)));

        float cosineThetaIncident =
            glm::dot(-glm::normalize(initialDirection), glm::normalize(lensNormal));
        float etaRatio =
            1.0 / lens.eta; // implicit assumption going from air to lens

        // use Snell's law to get the new direction (see citation)
        float sineSquaredThetaIncidentInitial =
            std::max(0.f, 1.f - cosineThetaIncident * cosineThetaIncident);
        float sineSquaredThetaIncidentAdjusted =
            etaRatio * etaRatio * sineSquaredThetaIncidentInitial;
        float cosineThetaTransmit = std::sqrt(1 - sineSquaredThetaIncidentAdjusted);

        // get new direction with refraction
        glm::vec3 newDirection = refraction(
            glm::normalize(-initialDirection), glm::normalize(lensNormal), etaRatio,
            cosineThetaIncident, cosineThetaTransmit);
        return {newDirection, intersectionPoint, true};
    }
    // return defaults and false if ray is outside of the lens
    return {glm::vec3(0), glm::vec3(0), false};
}

/**
 * @brief computeLensesAdjustedDirection: computed adjusted direction and
 * position after a ray passes through a lens assembly
 * @param initialDirection: initial direction of the ray
 * @return the updated position and direction as well as a boolean indicating if
 * the ray passed through the lens
 */
std::tuple<glm::vec3, glm::vec3, bool>
computeLensesAdjustedDirection(glm::vec3 initialDirection) {
    glm::vec3 currentDirection = initialDirection;
    glm::vec3 currentPosition = glm::vec3(0);
    float cumulativeThickness = 0.f;

    // iterate through global lenses and update direction, position, and thickness
    for (lens_t &lens : lenses) {
        auto [nextDirection, nextPosition, stillInLens] =
            computeLensAdjustedDirection(currentDirection, currentPosition, lens,
                                                                                       cumulativeThickness);
        if (!stillInLens)
            // return defaults and false if ray is outside of the lens
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
