#pragma once

#include <glm/glm.hpp>
<<<<<<< HEAD
#include "utils/rgba.h"
=======
#include "../utils/rgba.h"
#include <random>
>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e

// A forward declaration for the RaytraceScene class

class RayTraceScene;

// A class representing a ray-tracer

class RayTracer
{
public:
<<<<<<< HEAD
=======

>>>>>>> 2b4a126666c55702a96a7ee627c657304f86348e
    struct Config {
        bool enableShadow        = false;
        bool enableReflection    = false;
        bool enableRefraction    = false;
        bool enableTextureMap    = false;
        bool enableTextureFilter = false;
        bool enableParallelism   = false;
        bool enableSuperSample   = false;
        bool enableAcceleration  = false;
        bool enableDepthOfField  = false;
        int maxRecursiveDepth    = 4;
        bool onlyRenderNormals   = false;
    };

public:
    RayTracer(Config config);

    // Renders the scene synchronously.
    // The ray-tracer will render the scene and fill imageData in-place.
    // @param imageData The pointer to the imageData to be filled.
    // @param scene The scene to be rendered.
    void render(RGBA *imageData, const RayTraceScene &scene);

private:
    const Config m_config;
};

