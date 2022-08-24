#pragma once

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>
#include <vector>

#include "Model.h"
#include "VertexBufferLayout.h"

class ModelFactory {
   public:
    static Model* SimplePlane() {
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
    static Model* NormalsPlane() {
        Model* model = new Model();

        GLfloat buffer[] = {
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f};

        GLuint indices[] = {
            1, 2, 0,
            1, 2, 3};

        model->AddVertexData((GLvoid*)buffer, 24, GL_FLOAT);
        model->SetIndexData(indices, 6);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        model->AddBufferLayout(layoutElements);

        model->PackModel();

        return model;
    }
    static Model* RainbowCube() {
        Model* model = new Model();

        GLfloat buffer[] = {
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5, 0.0f, 0.0f, 0.0f};

        GLuint indices[] = {
            0, 2, 1,
            3, 2, 0,
            4, 6, 5,
            7, 6, 4,
            4, 0, 1,
            4, 1, 5,
            7, 3, 2,
            7, 2, 6,
            4, 7, 0,
            0, 7, 3,
            1, 2, 5,
            5, 2, 6};

        model->AddVertexData((GLvoid*)buffer, 48, GL_FLOAT);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        model->AddBufferLayout(layoutElements);

        model->SetIndexData(indices, 36);
        model->PackModel();

        return model;
    }
    static Model* SimpleCube() {
        Model* model = new Model();

        GLfloat buffer[] = {
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5};

        GLuint indices[] = {
            0, 2, 1,
            3, 2, 0,
            4, 6, 5,
            7, 6, 4,
            4, 0, 1,
            4, 1, 5,
            7, 3, 2,
            7, 2, 6,
            4, 7, 0,
            0, 7, 3,
            1, 2, 5,
            5, 2, 6};

        model->AddVertexData((GLvoid*)buffer, 24, GL_FLOAT);

        std::vector<LayoutElement> layoutElements;
        layoutElements.push_back((LayoutElement){3, GL_FLOAT});
        model->AddBufferLayout(layoutElements);

        model->SetIndexData(indices, 36);
        model->PackModel();

        return model;
    }
};