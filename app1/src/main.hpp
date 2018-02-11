#ifndef APP1_MAIN_HPP
#define APP1_MAIN_HPP
#pragma once

//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
//#include <btBulletDynamicsCommon.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/DefaultLogger.hpp>
#include <PerlinNoise.hpp>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

const unsigned DEFAULT_WIDTH = 1600;
const unsigned DEFAULT_HEIGHT = 900;

#include "types.hpp"
#include "mesh.hpp"

#endif
