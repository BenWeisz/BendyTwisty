#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <vector>

#include "Model.h"
#include "VertexBufferLayout.h"

class ModelFactory {
   public:
    static const Model* SimplePlane() {
        Model* model = new Model();

        GLfloat buffer[] = {
            -0.5f, 0.5f, 1.0f,
            -0.5f, -0.5f, 1.0f,
            0.5f, 0.5f, 1.0f,
            0.5f, -0.5f, 1.0f};

        GLuint indices[] = {
            0, 1, 2,
            2, 1, 3};

        model->SetVertexData(buffer, 12);
        model->SetIndexData(indices, 6);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){.count = 3, .type = GL_FLOAT});

        model->PackModel(layoutElements);

        return model;
    }
};