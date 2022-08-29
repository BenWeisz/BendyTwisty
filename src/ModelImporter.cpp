#include "ModelImporter.h"

Model* ModelImporter::LoadModel(const std::string& path) {
    // Open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not load shader file: " << path << std::endl;
        return nullptr;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    unsigned int vertCount = 0;
    unsigned int faceCount = 0;
    while (!ss.eof()) {
        std::string tag;
        std::string v[3];
        ss >> tag >> v[0] >> v[1] >> v[2];
        if (tag == "v")
            vertCount++;
        else if (tag == "f")
            faceCount++;
    }

    ss.clear();
    ss.seekg(0);

    GLfloat* buffer = new GLfloat[vertCount * 6];
    GLuint* indices = new GLuint[faceCount * 3];

    unsigned int vertIndex = 0;
    unsigned int vertNormalIndex = 0;
    unsigned int faceIndex = 0;
    while (!ss.eof()) {
        std::string tag;
        ss >> tag;
        if (tag == "v") {
            GLfloat v[3];
            ss >> v[0] >> v[1] >> v[2];
            buffer[vertIndex * 6] = v[0];
            buffer[(vertIndex * 6) + 1] = v[1];
            buffer[(vertIndex * 6) + 2] = v[2];
            vertIndex++;
        } else if (tag == "vn") {
            GLfloat n[3];
            ss >> n[0] >> n[1] >> n[2];
            buffer[3 + (vertNormalIndex * 6)] = n[0];
            buffer[3 + (vertNormalIndex * 6) + 1] = n[1];
            buffer[3 + (vertNormalIndex * 6) + 2] = n[2];
            vertNormalIndex++;
        } else if (tag == "f") {
            // We will only load models who's vertex normal indices are the same as the vertex data
            GLuint v[3];

            std::string faceStr;
            ss >> faceStr;
            v[0] = std::stoul(faceStr.substr(0, faceStr.find("//")));

            ss >> faceStr;
            v[1] = std::stoul(faceStr.substr(0, faceStr.find("//")));

            ss >> faceStr;
            v[2] = std::stoul(faceStr.substr(0, faceStr.find("//")));

            indices[faceIndex * 3] = v[0] - 1;
            indices[(faceIndex * 3) + 1] = v[1] - 1;
            indices[(faceIndex * 3) + 2] = v[2] - 1;
            faceIndex++;
        }
    }

    Model* model = new Model();
    model->AddVertexData((GLvoid*)buffer, vertCount * 6, GL_FLOAT);
    model->SetIndexData(indices, faceCount * 3);

    std::vector<LayoutElement> layoutElements;
    layoutElements.push_back((LayoutElement){3, GL_FLOAT});
    layoutElements.push_back((LayoutElement){3, GL_FLOAT});
    model->AddBufferLayout(layoutElements);

    model->PackModel();

    delete[] buffer;
    delete[] indices;

    return model;
}