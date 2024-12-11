#include "raytracescene.h"
<<<<<<< HEAD
#include "utils/sceneparser.h"
=======
#include "../utils/sceneparser.h"
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
#include <iostream>
#include <stdexcept>

/**
 * @brief RayTraceScene::RayTraceScene: creates a class instance, sets the class
 * fields
 * @param width: width of the scene to render
 * @param height: height of the scene to render
 * @param metaData: metadata of the scene to render
 */
RayTraceScene::RayTraceScene(int width, int height,
                             const RenderData &metaData) {
    // set initial fields
    width_ = width;
    height_ = height;
    globalData_ = metaData.globalData;

    // get camera data to pass to constructor
    float aspectRatio = (float)width_ / (float)(height);
    SceneCameraData cameraData = metaData.cameraData;
    camera_ = Camera(cameraData.pos, cameraData.look, cameraData.up, aspectRatio,
                     cameraData.heightAngle);

    // set remaning fields
    lights_ = metaData.lights;
    shapes_ = metaData.shapes;
}

/**
 * @brief RayTraceScene::width: getter for width_ field
 * @return the width_ field of the class
 */
const int &RayTraceScene::width() const { return width_; }

/**
 * @brief RayTraceScene::height: getter for the height_ field
 * @return the height_ field of the class
 */
const int &RayTraceScene::height() const { return height_; }

/**
 * @brief RayTraceScene::getGlobalData: getter for the globalData_ field
 * @return the globalData_ field of the class
 */
const SceneGlobalData &RayTraceScene::getGlobalData() const {
    return globalData_;
}

/**
 * @brief RayTraceScene::getCamera: getter for the camera_ field
 * @return the camera_ field of the class
 */
const Camera &RayTraceScene::getCamera() const { return camera_; }

/**
 * @brief RayTraceScene::getLights: getter for the lights_ field
 * @return the lights_ field of the class
 */
const std::vector<SceneLightData> &RayTraceScene::getLights() const {
    return lights_;
}

/**
 * @brief RayTraceScene::getShapes: getter for the shapes_ field
 * @return the shapes_ field of the class
 */
const std::vector<RenderShapeData> &RayTraceScene::getShapes() const {
    return shapes_;
}
