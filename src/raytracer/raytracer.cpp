#include "raytracer.h"
#include "../singleraytrace/tracesingleray.h"
#include "raytracescene.h"
#include <cfloat>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

/**
 * @brief RayTracer::RayTracer: constructor for the ray tracer class that sets
 * the configuration
 * @param config: the configuration to use
 */
RayTracer::RayTracer(Config config) : m_config(config) {}

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    // note that 'data' is a pointer, can access elements like 'data[i]'

    // below is my implementation of the ray tracing algorithm

    // define constants for future computation
    // note that here, k is the depth
    int k = 1;
    // relevant formula: 2 * k * tan(theta_h/2)
    float viewPlaneHeight = 2 * k * tan(scene.getCamera().getHeightAngle() / 2);
    // using aspect ratio to get the width of the view plane
    float viewPlaneWidth = scene.getCamera().getAspectRatio() * viewPlaneHeight;

    // iterate through each pixel and trace a ray
    for (int j = 0; j < scene.height(); ++j) {
        for (int i = 0; i < scene.width(); ++i) {
            // relevant algo section formula: y = viewPlaneHeight * (((H - 1 - j +
            // 0.5) / H) - 0.5)
            float y = viewPlaneHeight *
                      (((scene.height() - 1.f - j + 0.5f) / scene.height()) - 0.5f);
            // relevant algo section formula: x = viewPlaneWidth * (((i + 0.5) / W) -
            // 0.5)
            float x = viewPlaneWidth * ((i + 0.5f) / scene.width() - 0.5f);

            // get uvk, eye, and d in homogenous coordinates
            glm::vec4 uvk = glm::vec4(x, y, -k, 1);
            glm::vec4 eye = glm::vec4(0, 0, 0, 1);
            glm::vec4 direction = uvk - eye;

            // transform the ra into world space from camera space
            glm::vec4 transformedEye = scene.getCamera().getViewMatrixInverse() * eye;
            // FIX FROM INTERSECT MENTOR MEETING: not truncating view matrix inverse
            // here
            glm::vec4 transformedD =
                scene.getCamera().getViewMatrixInverse() * direction;

            // trace the ray and set the correct image value
            imageData[j * scene.width() + i] =
                traceRay(transformedEye, transformedD, scene, m_config, 0);
        }
    }
}
