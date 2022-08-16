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
            -0.5f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f};

        GLuint indices[] = {
            0, 1, 2,
            2, 1, 3};

        model->AddVertexData((GLvoid*)buffer, 12, GL_FLOAT);
        model->SetIndexData(indices, 6);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        model->AddBufferLayout(layoutElements);

        model->PackModel();

        return model;
    }
};