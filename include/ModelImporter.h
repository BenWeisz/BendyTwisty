#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Model.h"

namespace ModelImporter {
Model* LoadModel(const std::string& path);
};