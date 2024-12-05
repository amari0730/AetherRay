#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

/**
 * @brief Camera::Camera: default camera constructor for compiling purposes
 */
Camera::Camera() {}

/**
 * @brief Camera::Camera: camera constructor that sets the fields
 * @param pos: position of the camera
 * @param look: look vector of the camera (direction it is looking)
 * @param up: up vector of the camera (direction that is considered up)
 * @param aspectRatio: ratio width:height of the camera's view
 * @param heightAngle: vertical angle of what the camera can see
 */
Camera::Camera(glm::vec4 pos, glm::vec4 look, glm::vec3 up, float aspectRatio,
               float heightAngle) {
    // to compute the view matrix, perform numerous calculations
    // citation: the code here is adapted from my Lab 4: Transforms code
    glm::vec4 negativePos = -pos;
    glm::mat4 mTranslate =
        glm::translate(glm::mat4(1.f),
                                          glm::vec3(negativePos[0], negativePos[1], negativePos[2]));
    // relevant formula: w = normalize(-look)
    glm::vec3 w = glm::normalize(-look);
    // relevant formula: v = normalize(up - (up \cdot w) * w)
    glm::vec3 v = glm::normalize(up - (glm::dot(up, w) * w));
    // u = v x w
    glm::vec3 u = glm::cross(v, w);
    glm::mat4 mRotate = glm::mat4(u[0], v[0], w[0], 0, u[1], v[1], w[1], 0, u[2],
                                  v[2], w[2], 0, 0, 0, 0, 1);

    // set the fields of the class
    // relevant formula: viewMatrix = (mRotate)(mTranslate)
    viewMatrix_ = mRotate * mTranslate;
    aspectRatio_ = aspectRatio;
    heightAngle_ = heightAngle;
    // FIX FROM INTERSECT MENTOR MEETING: store inverse
    viewMatrixInverse_ = glm::inverse(viewMatrix_);
}

/**
 * @brief Camera::getViewMatrix: getter for the viewMatrix_ field
 * @return the viewMatrix_ field of the class
 */
glm::mat4 Camera::getViewMatrix() const { return viewMatrix_; }

/**
 * @brief Camera::getViewMatrixInverse: getter for the inverseViewMatrix_ field
 * @return the viewMatrixInverse_ field of the class
 */
glm::mat4 Camera::getViewMatrixInverse() const {
    // FIX FROM INTERSECT MENTOR MEETING: store inverse
    return viewMatrixInverse_;
}

/**
 * @brief Camera::getAspectRatio: getter for the aspectRatio_ field
 * @return the aspectRatio_ field of the class
 */
float Camera::getAspectRatio() const { return aspectRatio_; }

/**
 * @brief Camera::getHeightAngle: getter for the heightAngle_ field
 * @return the heightAngle field of the class
 */
float Camera::getHeightAngle() const { return heightAngle_; }

float Camera::getFocalLength() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getAperture() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}
