#include "raytracer.h"
#include "../lenses/lenseassemblies.h"
#include "../singleraytrace/tracesingleray.h"
#include "raytracescene.h"
#include <cfloat>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
<<<<<<< HEAD
=======
<<<<<<< HEAD

=======
#include "../lenses/lenseassemblies.h"
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
>>>>>>> refs/remotes/origin/main
/**
 * @brief RayTracer::RayTracer: constructor for the ray tracer class that sets
 * the configuration
 * @param config: the configuration to use
 */
RayTracer::RayTracer(Config config) : m_config(config) {}

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    // note that 'data' is a pointer, can access elements like 'data[i]'

    // define constants for future computation
    // note that here, k is the depth
    int k = 1;
    // relevant formula: 2 * k * tan(theta_h/2)
    float viewPlaneHeight = 2 * k * tan(scene.getCamera().getHeightAngle() / 2);
    // using aspect ratio to get the width of the view plane
    float viewPlaneWidth = scene.getCamera().getAspectRatio() * viewPlaneHeight;
<<<<<<< HEAD

    // iterate through each pixel and trace a ray
=======
    const int samplesPerPixel = 100;
    // iterate through each pixel and trace a ray

>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
    for (int j = 0; j < scene.height(); ++j) {
        for (int i = 0; i < scene.width(); ++i) {
            // relevant formula: y = viewPlaneHeight * (((H - 1 - j +
            // 0.5) / H) - 0.5)
            float y = viewPlaneHeight *
                      (((scene.height() - 1.f - j + 0.5f) / scene.height()) - 0.5f);
            // relevant formula: x = viewPlaneWidth * (((i + 0.5) / W) -
            // 0.5)
            float x = viewPlaneWidth * ((i + 0.5f) / scene.width() - 0.5f);

            // get uvk, eye, and d in homogenous coordinates
            glm::vec4 uvk = glm::vec4(x, y, -k, 1);
            glm::vec4 eye = glm::vec4(0, 0, 0, 1);
            glm::vec4 direction = uvk - eye;
<<<<<<< HEAD

            // transform the ra into world space from camera space
            glm::vec4 transformedEye = scene.getCamera().getViewMatrixInverse() * eye;
            // FIX FROM INTERSECT MENTOR MEETING: not truncating view matrix inverse
            // here
            glm::vec4 transformedD =
                scene.getCamera().getViewMatrixInverse() * direction;

            // trace the ray and set the correct image value
            imageData[j * scene.width() + i] =
                traceRay(transformedEye, transformedD, scene, m_config, 0);
=======
            glm::vec4 accumulatedColor = glm::vec4(0, 0, 0, 255);

            // move lens through the lens assembly, switching z direction to move to
            // camera lens space
            direction.z *= -1.f;
            auto [newDirection, newPosition, inLens] =
                computeLensesAdjustedDirection(direction);
            if (inLens) { // if ray within the lens, trace it
                direction = glm::vec4(newDirection, 0);
                // convert back to regular camera space
                direction.z *= -1.f;
                eye = glm::vec4(newPosition, 1);
                // convert back to regular camera space
                eye.z *= -1.f;

                // transform the ray into world space from camera space
                glm::vec4 transformedEye =
                    scene.getCamera().getViewMatrixInverse() * eye;
                glm::vec4 transformedD =
                    scene.getCamera().getViewMatrixInverse() * direction;

                // trace the ray and set the correct image value
                for (int k = 0; k < samplesPerPixel; k++) {
                    float open = (float)(k) / (float)samplesPerPixel;
                    float close = (float)(k + 1) / (float)samplesPerPixel;
                    double rayTime =
                        open +
                                     (static_cast<double>(arc4random()) / RAND_MAX) * (close - open);
                    RGBA color = traceRay(transformedEye, transformedD, scene, m_config,
                                          0, rayTime);
                    accumulatedColor.r += color.r;
                    accumulatedColor.b += color.b;
                    accumulatedColor.g += color.g;
                }

                accumulatedColor /= (float)(samplesPerPixel);
                RGBA finalColor;
                for (int i = 0; i < 3; i++) {
                    accumulatedColor[i] =
                        (int)std::min(255.f, std::max(0.f, accumulatedColor[i]));
                }
                finalColor.r = accumulatedColor.r;
                finalColor.g = accumulatedColor.g;
                finalColor.b = accumulatedColor.b;
                finalColor.a = 255;

                // trace the ray and set the correct image value
                imageData[j * scene.width() + i] = finalColor;

            } else {
                // set the color to white if ray is outside of the camera
                imageData[j * scene.width() + i] = RGBA{255, 255, 255};
            }
<<<<<<< HEAD
=======
            // transform the ra into world space from camera space
            // glm::vec4 transformedEye = scene.getCamera().getViewMatrixInverse() * eye;
            // // FIX FROM INTERSECT MENTOR MEETING: not truncating view matrix inverse
            // // here
            // glm::vec4 transformedD =
            //     scene.getCamera().getViewMatrixInverse() * direction;

            // // You could have the shutter open for 1/1000th of a second every frame, or 1/60th of a second.
            // for (int k = 0; k < samplesPerPixel; k++) {
            //     float open = (float)(k) / (float)samplesPerPixel;
            //     float close = (float)(k + 1) / (float)samplesPerPixel;
            //     double rayTime = open + (static_cast<double>(arc4random()) / RAND_MAX) * (close - open);
            //     RGBA color = traceRay(transformedEye, transformedD, scene, m_config, 0, rayTime);
            //     accumulatedColor.r += color.r;
            //     accumulatedColor.b += color.b;
            //     accumulatedColor.g += color.g;
            // }

            // accumulatedColor /= (float)(samplesPerPixel);
            // RGBA finalColor;
            // for (int i = 0; i < 3; i++) {
            //     accumulatedColor[i] = (int)std::min(255.f, std::max(0.f, accumulatedColor[i]));
            // }
            // finalColor.r = accumulatedColor.r;
            // finalColor.g = accumulatedColor.g;
            // finalColor.b = accumulatedColor.b;
            // finalColor.a = 255;
            // // trace the ray and set the correct image value
            // imageData[j * scene.width() + i] = finalColor;
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
>>>>>>> refs/remotes/origin/main
        }
    }
}
