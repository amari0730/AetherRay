#pragma once

<<<<<<< HEAD
#include "utils/rgba.h"
=======
#include "rgba.h"
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
#include <QString>
#include <QImage>
#include <iostream>

struct Image {
    RGBA* data;
    int width;
    int height;
};

Image* loadImageFromFile(std::string file);
