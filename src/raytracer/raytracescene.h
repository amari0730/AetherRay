#pragma once

#include "../camera/camera.h"
#include "../utils/scenedata.h"
#include "../utils/sceneparser.h"

/**
 * @brief The RayTraceScene class: class for with scene information for raytracing purposes
 */
class RayTraceScene {
public:
    // constructor with information to set fields
    RayTraceScene(int width, int height, const RenderData &metaData);

    // The getter of the width of the scene
    const int &width() const;

    // The getter of the height of the scene
    const int &height() const;

    // The getter of the global data of the scene
    const SceneGlobalData &getGlobalData() const;

    // The getter of the shared pointer to the camera instance of the scene
    const Camera &getCamera() const;

    // getter for lights of the scene
    const std::vector<SceneLightData> &getLights() const;

    // getter for shapes of the scene
    const std::vector<RenderShapeData> &getShapes() const;

private:
    // width_: width of the scene
    int width_;
    // height_: height of the scene
    int height_;
    // globalData_: global data for the scene
    SceneGlobalData globalData_;
    // camera_: camera in the scene
    Camera camera_;
    // lights_: lights in the scene
    std::vector<SceneLightData> lights_;
    // shapes_: shapes in the scene
    std::vector<RenderShapeData> shapes_;
};
