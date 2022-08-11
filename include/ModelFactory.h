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

    static const Model* CheckeredPlane(
        const GLfloat width, const GLfloat height,
        const unsigned int hCount, const unsigned int vCount) {
        Model* model = new Model();

        // Set up the buffer data
        GLfloat* buffer = new GLfloat[(hCount + 1) * (vCount + 1) * 3];

        GLfloat hOffset = -width / 2.0f;
        GLfloat vOffset = height / 2.0f;
        GLfloat hStep = width / hCount;
        GLfloat vStep = -height / vCount;
        unsigned int i = 0;
        for (unsigned int y = 0; y < vCount + 1; y++) {
            for (unsigned int x = 0; x < hCount + 1; x++) {
                buffer[i++] = hOffset + (hStep * x);
                buffer[i++] = vOffset + (vStep * y);
                buffer[i++] = 1.0f;
            }
        }

        // Set up the index data
        GLuint* indices = new GLuint[hCount * vCount * 6];

        i = 0;
        for (unsigned int y = 0; y < vCount; y++) {
            for (unsigned int x = 0; x < hCount; x++) {
                // Top Triangle
                indices[i++] = (y * (hCount + 1)) + x;
                indices[i++] = ((y + 1) * (hCount + 1)) + x;
                indices[i++] = (y * (hCount + 1)) + x + 1;

                // Bottom Triangle
                indices[i++] = (y * (hCount + 1)) + x + 1;
                indices[i++] = ((y + 1) * (hCount + 1)) + x;
                indices[i++] = ((y + 1) * (hCount + 1)) + x + 1;
            }
        }

        model->SetVertexData(buffer, (hCount + 1) * (vCount + 1) * 3);
        model->SetIndexData(indices, hCount * vCount * 6);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){.count = 3, .type = GL_FLOAT});

        model->PackModel(layoutElements);

        return model;
    }
};