#pragma once

#include "rgba.h"
#include <QImage>
#include <QString>

struct Image {
    RGBA *data;
    int width;
    int height;
};

Image *loadImageFromFile(std::string file);
