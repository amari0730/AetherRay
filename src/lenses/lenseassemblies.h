#ifndef LENSEASSEMBLIES_H
#define LENSEASSEMBLIES_H

#include <glm/glm.hpp>
#include <tuple>

std::tuple<glm::vec3, glm::vec3, bool>
computeLensesAdjustedDirection(glm::vec3 initialDirection);

#endif // LENSEASSEMBLIES_H
