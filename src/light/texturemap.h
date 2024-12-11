#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include "../utils/scenedata.h"

std::tuple<float, float> getShapeUV(PrimitiveType shapeType,
                                    glm::vec3 intersection, double time, glm::vec3 center2);

#endif // TEXTUREMAP_H
