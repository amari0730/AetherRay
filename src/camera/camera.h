#pragma once

#include <glm/glm.hpp>

/**
 * @brief The Camera class: class representing a virtual camera
 */
class Camera {
public:
    // default constructor
    Camera();

    // constructor with fields
    Camera(glm::vec4 pos, glm::vec4 look, glm::vec3 up, float aspectRatio,
           float heightAngle);

    // returns the view matrix for the current camera settings
    glm::mat4 getViewMatrix() const;

    // gets the inverse of the view matrix for the current camera settings
    glm::mat4 getViewMatrixInverse() const;

    // returns the aspect ratio of the camera
    float getAspectRatio() const;

    // returns the height angle of the camera in RADIANS
    float getHeightAngle() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getFocalLength() const;

    // Returns the focal length of this camera.
    // This is for the depth of field extra-credit feature only;
    // You can ignore if you are not attempting to implement depth of field.
    float getAperture() const;

private:
    // viewMatrix_: matrix transforming to camera space
    glm::mat4 viewMatrix_;
    // aspectRatio_: ratio of width:height that the camera can see
    float aspectRatio_;
    // heightAngle_: vertical angle that the camera can see
    float heightAngle_;
    // viewMatrixInverse_: inverse of the view matrix
    glm::mat4 viewMatrixInverse_;
};
