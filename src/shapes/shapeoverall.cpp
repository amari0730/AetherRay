#include "shapeoverall.h"
#include <cmath>

/**
 * @brief withinEpsilon: determines if two floats are within a small epsilon
 * value of each other
 * @param num1: a number to compare with num1
 * @param num2: a number to compare with num1
 * @return a boolean indicating true of the numbers are within the range and
 * false otherwise
 */
bool withinEpsilon(float num1, float num2) {
    return std::abs(num1 - num2) < pow(10, -4);
}
<<<<<<< HEAD
=======

<<<<<<< HEAD
float getDiscriminant(float a, float b, float c) { return b * b - 4.f * a * c; }
=======
float getDiscriminant(float a, float b, float c) {
    return b*b - 4.f*a*c;
}

>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
>>>>>>> refs/remotes/origin/main
