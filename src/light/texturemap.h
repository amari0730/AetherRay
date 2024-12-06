#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H
#include "utils/scenedata.h"

std::tuple<float, float> getShapeUV(PrimitiveType shapeType,
                                glm::vec3 intersection);

#endif // TEXTUREMAP_H
