#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

/**
 * @brief dfsBuild: builds out the scene to render
 * @param node: node to build out from
 * @param CTM: the cumulative transform matrix
 * @param renderData: the data that is being arranged
 *
 * citation: this function is copied from my code for Lab 5: Scene Parsing
 */
void dfsBuild(SceneNode *node, glm::mat4 CTM, RenderData &renderData) {
    glm::mat4 newCTM = CTM;

    // iterate through the transformations and add them to the CTM
    for (SceneTransformation *transformation : node->transformations) {
        switch (transformation->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            newCTM *= glm::translate(transformation->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            newCTM *= glm::scale(transformation->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            newCTM *= glm::rotate(transformation->angle, transformation->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            newCTM *= transformation->matrix;
            break;
        }
    }

    // iterate through scene primitives and record their CTM values
    for (ScenePrimitive *primitive : node->primitives) {
<<<<<<< HEAD
=======

        // primitive->type = PrimitiveType::PRIMITIVE_SPHERE_MOVING;
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
        renderData.shapes.push_back(
            RenderShapeData{*primitive, newCTM, glm::inverse(newCTM)});
    }

    // iterate through the lights and reocrd their direction and position in world
    // space
    for (SceneLight *light : node->lights) {
        renderData.lights.push_back(SceneLightData{
                                                   light->id, light->type, light->color, light->function,
            newCTM * glm::vec4(0, 0, 0, 1), newCTM * light->dir, light->penumbra,
            light->angle, light->width, light->height});
    }

    // reccur in a depth first manner on all children of the current node
    for (SceneNode *child : node->children) {
        dfsBuild(child, newCTM, renderData);
    }
}

/**
 * @brief SceneParser::parse: parses a scene
 * @param filepath: filepath of the scene to parse
 * @param renderData: location to place the parsed data
 * @return a boolean indicating if the parsing was successful
 *
 * citation: this function is copied from Lab 5: Scene Parsing
 */
bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // get global and camera data
    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();

    // populate renderData's list of primitives and their transforms
    SceneNode *rootNode = fileReader.getRootNode();
    renderData.shapes.clear();
    dfsBuild(rootNode, glm::mat4(1.f), renderData);
    return true;
}
