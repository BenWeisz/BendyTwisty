#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <OpenGL/gl3.h>

#include "VertexBufferLayout.h"
#include "Model.h"

namespace ModelLoader {
Model* LoadModel(const std::string& path);
Model* SimplePlane();
Model* NormalsPlane();
Model* NormalsCube();
Model* RainbowCube();
Model* SimpleCube();
};