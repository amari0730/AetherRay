#include "../singleraytrace/tracesingleray.h"
#include "../utils/scenedata.h"
#include "light/texturemap.h"
#include "utils/imagereader.h"
#include "utils/rgba.h"
#include <cfloat>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <map>

/**
 * @brief loadedImages: map to store pointers to already loaded textures to
 * avoid having to reload the files
 */
std::map<std::string, Image *> loadedImages;

/**
 * @brief toRGBA: Helper function to convert illumination to RGBA, applying some
 * form of tone-mapping (e.g. clamping) in the process
 * @param illumination: the illumination value to convert to RGBA
 * @return an RGBA equivalent to an illumination in floats
 *
 * citation: this function is directly copied from Lab 6: Light
 */
RGBA toRGBA(const glm::vec4 &illumination) {
    uint8_t redColor = 255 * std::min(std::max(illumination[0], 0.f), 1.f);
    uint8_t greenColor = 255 * std::min(std::max(illumination[1], 0.f), 1.f);
    uint8_t blueColor = 255 * std::min(std::max(illumination[2], 0.f), 1.f);
    return RGBA{redColor, greenColor, blueColor};
}

/**
 * @brief toIllumination: Helper function to convert RGBA to illumination values
 * @param RGBA: the illumination value to convert to RGBA
 * @return an RGBA equivalent to an illumination in floats
 *
 * citation: this function is havily inspired by the toRGBA function from Lab 6:
 * Light
 */
glm::vec4 toIllumination(RGBA color) {
    float redColor = color.r / 255.f;
    float greenColor = color.g / 255.f;
    float blueColor = color.b / 255.f;
    return glm::vec4(redColor, greenColor, blueColor, 0);
}

/**
 * @brief getTextureInterpolation: helper function to computer linear
 * interpolation between texture and object diffuse colors
 * @param material: material of the object for which to computer the
 * interpolation
 * @param globalData: global data of the scene
 * @param shapeType: type of shape for which the interpolation is being computed
 * @param objectSpaceIntersection: intersection point of the object in object
 * space
 * @return the scene color to use for the diffuse calculation involving linear
 * interpolation between the texture and object diffuse color
 */
SceneColor getTextureInterpolation(SceneMaterial &material,
                                   const SceneGlobalData &globalData,
                                   PrimitiveType shapeType,
                                   glm::vec4 objectSpaceIntersection) {
    // calculate u and v
    auto [u, v] = getShapeUV(shapeType, objectSpaceIntersection);

    // see if image is already loaded, if not then load it
    if (!loadedImages.contains(material.textureMap.filename)) {
        Image *newLoadedImage = loadImageFromFile(material.textureMap.filename);
        loadedImages[material.textureMap.filename] = newLoadedImage;
    }
    Image *imageToUse = loadedImages[material.textureMap.filename];

    // calculate c and r
    // relevant algo section formula: c = floor(u * m * w) % w
    int c = (int)(u * material.textureMap.repeatU * imageToUse->width) %
            imageToUse->width;
    // relevant algo section formula: check if c = m * w
    if (c == material.textureMap.repeatU * imageToUse->width)
        --c;
    // relevant algo section formula: floor((1-v) * n * h) % h
    int r = (int)((1 - v) * material.textureMap.repeatV * imageToUse->height) %
            imageToUse->height;
    // relevant algo section formula: check if r = n * h
    if (r == material.textureMap.repeatV * imageToUse->height)
        --r;

    // calculate diffuse and linearly interpolate
    SceneColor diffuse = globalData.kd * material.cDiffuse;
    SceneColor linearInterpolation =
        material.blend *
                                         toIllumination(imageToUse->data[r * imageToUse->width + c]) +
                                     (1.f - material.blend) * diffuse;

    // return the linear interpolation value
    return linearInterpolation;
}

/**
 * @brief phong: computes the pixel color to render based on the Phone Lighting
 * Model
 * @param position: the position of the object and ray intersection
 * @param normal: the normal vector of the object in world space
 * @param directionToCamera: direction from the point of intersection towards
 * the camera
 * @param material: the material of the object that was intersected
 * @param lights: the lights to use for illumination consideration
 * @param globalData: global data about the scene
 * @param scene: entire scene to raytrace (for recursion purposes)
 * @param config: configuration of the raytracer
 * @param completedReflections: how many reflections have been computed for the
 * given pathway already
 * @param shapeType: type of shape the light is being computed for
 * @param objectSpaceIntersection: intersection in object space
 * @return an RGBA value corrosponding to the color of the object to render
 *
 * citation: some of this code is copied from and/or directly inspired by my
 * code from Lab 6: Light
 */
RGBA phong(glm::vec4 position, glm::vec4 normal, glm::vec4 directionToCamera,
           SceneMaterial &material, const std::vector<SceneLightData> &lights,
           const SceneGlobalData &globalData, const RayTraceScene &scene,
           const RayTracer::Config &config, int completedReflections,
           PrimitiveType shapeType, glm::vec4 objectSpaceIntersection) {
    // normalizing directions
    normal = glm::normalize(normal);
    directionToCamera = glm::normalize(directionToCamera);

    // output illumination (ignoring opacity here)
    glm::vec4 illumination(0, 0, 0, 1);

    // add the ambient term
    // equation: ka * Oa
    illumination += globalData.ka * material.cAmbient;

    // iterate through each of the lights
    for (const SceneLightData &light : lights) {
        switch (light.type) {
        case LightType::LIGHT_POINT: {
            // get distance
            float distanceToLight = glm::length(light.pos - position);

            // compute attenuation factor using formuala from Lab 6
            float fAtt = std::min(
                1.f, 1.f / (light.function[0] + distanceToLight * light.function[1] +
                            distanceToLight * distanceToLight * light.function[2]));

            // check for shadows
            float minDistance;
            glm::vec4 directionToLight = glm::normalize(light.pos - position);
            if (config.enableShadow) {
                // trace a shadow ray to determine possible intersection
                float epsilon = pow(10, -4);
                minDistance = traceShadowRay(position + epsilon * directionToLight,
                                             directionToLight, scene);
            } else
                minDistance = -1.f;

            // add the diffuse term
            float dotProductLambert = glm::dot(directionToLight, normal);
            if (dotProductLambert > 0 &&
                (minDistance > distanceToLight || minDistance == -1.f)) {
                if (config.enableTextureMap &&
                    material.blend > 0) { // complete texture mapping
                    // add interpolated color to the output
                    SceneColor linearInterpolation = getTextureInterpolation(
                        material, globalData, shapeType, objectSpaceIntersection);
                    illumination +=
                        light.color * fAtt * linearInterpolation * dotProductLambert;
                } else
                    illumination += fAtt * light.color * globalData.kd *
                                    material.cDiffuse * dotProductLambert;
            }

            // add the specular term
            glm::vec4 R = glm::reflect(-directionToLight, normal);
            float dotProductSpecular = glm::dot(R, directionToCamera);
            if (dotProductLambert > 0 &&
                (minDistance > distanceToLight || minDistance == -1.f)) {
                illumination += fAtt * light.color * globalData.ks *
                                material.cSpecular *
                                (float)pow(dotProductSpecular, material.shininess);
            }
            break;
        }
        case LightType::LIGHT_DIRECTIONAL: {
            // compute the attenuation factor (just 1 for directional lights)
            float fAtt = 1.f;

            // check for shadows
            glm::vec4 directionToLight = glm::normalize(-light.dir);
            float minDistance;
            if (config.enableShadow) {
                float epsilon = pow(10, -4);
                // trace a shadow ray to determine possible intersection
                minDistance = traceShadowRay(position + epsilon * directionToLight,
                                             directionToLight, scene);
            } else
                minDistance = -1;

            // add the diffuse term
            float dotProductLambert = glm::dot(directionToLight, normal);
            // equation: kd * Od * (N \cdot L)
            if (dotProductLambert > 0 && minDistance == -1.f) {
                if (config.enableTextureMap &&
                    material.blend > 0) { // complete texture mapping
                    // add interpolated color to the output
                    SceneColor linearInterpolation = getTextureInterpolation(
                        material, globalData, shapeType, objectSpaceIntersection);
                    illumination +=
                        light.color * fAtt * linearInterpolation * dotProductLambert;
                } else
                    illumination += fAtt * light.color * globalData.kd *
                                    material.cDiffuse * dotProductLambert;
            }

            // add the specular term
            glm::vec4 R = glm::reflect(-directionToLight, normal);
            float dotProductSpecular = glm::dot(R, directionToCamera);
            // equation: ks * Os * (R \cdot V)^n
            if (dotProductLambert > 0 && minDistance == -1.f) {
                illumination += fAtt * light.color * globalData.ks *
                                material.cSpecular *
                                (float)pow(dotProductSpecular, material.shininess);
            }
            break;
        }
        case LightType::LIGHT_SPOT:
            // get distance
            float distanceToLight = glm::length(light.pos - position);

            // compute attenuation factor using formuala from Lab 6: Light
            float fAtt = std::min(
                1.f, 1.f / (light.function[0] + distanceToLight * light.function[1] +
                            distanceToLight * distanceToLight * light.function[2]));

            // special for spot lights: calculate light intensity based on angle
            glm::vec4 directionToObject = glm::normalize(position - light.pos);
            float thetaOuter = light.angle;
            float thetaInner = (thetaOuter - light.penumbra);
            // cos(x) = light_direction \cdot direction_to_object
            // note that the light direction is not by default normalized
            float x = acos(glm::dot(glm::normalize(light.dir),
                                    directionToObject)); // angle between center of
                // spotlight and the object

            // compute light intensity based on how x compares to light direction
            SceneColor lightIntensity;
            if (x <= thetaInner)
                lightIntensity = light.color;
            else if (x <= thetaOuter) {
                // find falloff using formula from the assignment handout
                float falloff =
                    -2 * pow((x - thetaInner) / (thetaOuter - thetaInner), 3) +
                                3 * pow((x - thetaInner) / (thetaOuter - thetaInner), 2);
                lightIntensity = light.color * (1 - falloff);
            } else // thetaOuter < x
                lightIntensity = glm::vec4(0);

            // check for shadows
            glm::vec4 directionToLight = glm::normalize(light.pos - position);
            float minDistance;
            if (config.enableShadow) {
                // trace a shadow ray to determine possible intersection
                float epsilon = pow(10, -4);
                minDistance = traceShadowRay(position + epsilon * directionToLight,
                                             directionToLight, scene);
            } else
                minDistance = -1;

            // add the diffuse term
            float dotProductLambert = glm::dot(directionToLight, normal);
            if (dotProductLambert > 0 &&
                (minDistance > distanceToLight || minDistance == -1.f)) {
                if (config.enableTextureMap &&
                    material.blend > 0) { // complete texture mapping
                    // add interpolated color to the output
                    SceneColor linearInterpolation = getTextureInterpolation(
                        material, globalData, shapeType, objectSpaceIntersection);
                    illumination +=
                        light.color * fAtt * linearInterpolation * dotProductLambert;
                } else
                    illumination += fAtt * lightIntensity * globalData.kd *
                                    material.cDiffuse * dotProductLambert;
            }

            // add the specular term
            glm::vec4 R = glm::reflect(-directionToLight, normal);
            float dotProductSpecular = glm::dot(R, directionToCamera);
            if (dotProductLambert > 0 &&
                (minDistance > distanceToLight || minDistance == -1.f)) {
                illumination += fAtt * lightIntensity * globalData.ks *
                                material.cSpecular *
                                (float)pow(dotProductSpecular, material.shininess);
            }
            break;
        }
    }

    // add potential reflective component
    if (config.enableReflection &&
        completedReflections < config.maxRecursiveDepth &&
        (material.cReflective[0] != 0 || material.cReflective[1] != 0 ||
                                                                                       material.cReflective[2] != 0)) {
        // calculate reflected ray (line copied from Lab 6: Light)
        glm::vec4 reflectedRay =
            2.f * glm::dot(directionToCamera, normal) * normal - directionToCamera;

        // calculate reflection through recursive raytracing, adding epsilon to
        // avoid self reflection
        float epsilon = pow(10, -1);
        illumination +=
            toIllumination(traceRay(position + reflectedRay * epsilon, reflectedRay,
                                                scene, config, completedReflections + 1)) *
                        globalData.ks * material.cReflective;
    }

    // convert the illumination value to an RGBA and return it
    RGBA returnValue = toRGBA(illumination);
    return returnValue;
}
