#include "ModelImporter.h"

Model* ModelImporter::LoadModel(const std::string& path) {
    // Open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not load shader file: " << path << std::endl;
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    unsigned int vertCount = 0;
    unsigned int faceCount = 0;
    while (!buffer.eof()) {
        std::string tag;
        std::string v[3];
        buffer >> tag >> v[0] >> v[1] >> v[2];
        if (tag == "v")
            vertCount++;
        else if (tag == "f")
            faceCount++;
    }

    buffer.clear();
    buffer.seekg(0);

    GLfloat* bufferData = new GLfloat[vertCount * 3];
    GLuint* indices = new GLuint[faceCount * 3];

    unsigned int vertIndex = 0;
    unsigned int faceIndex = 0;
    while (!buffer.eof()) {
        std::string tag;
        buffer >> tag;
        if (tag == "v") {
            GLfloat v[3];
            buffer >> v[0] >> v[1] >> v[2];
            bufferData[vertIndex * 3] = v[0];
            bufferData[(vertIndex * 3) + 1] = v[1];
            bufferData[(vertIndex * 3) + 2] = v[2];
            vertIndex++;
        } else if (tag == "f") {
            GLuint v[3];
            buffer >> v[0] >> v[1] >> v[2];
            indices[faceIndex * 3] = v[0] - 1;
            indices[(faceIndex * 3) + 1] = v[1] - 1;
            indices[(faceIndex * 3) + 2] = v[2] - 1;
            faceIndex++;
        }
    }

    Model* model = new Model();
    model->AddVertexData((GLvoid*)bufferData, vertCount * 3, GL_FLOAT);
    model->SetIndexData(indices, faceCount * 3);

    std::vector<LayoutElement> layoutElements;
    layoutElements.push_back((LayoutElement){3, GL_FLOAT});
    model->AddBufferLayout(layoutElements);

    model->PackModel();

    delete[] bufferData;
    delete[] indices;

    return model;
}