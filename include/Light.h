#pragma once

#include <string>

#include <glm/ext.hpp>

#include "ModelLoader.h"
#include "Transform.h"
#include "Model.h"
#include "ShaderProgram.h"

#define LIGHT_POINT 0x00
#define LIGHT_DIRECTIONAL 0x01

class Light {
   public:
    Light(glm::vec3 color, unsigned int type, const std::string& name);
    glm::vec3 GetColor() const;
    void SetColor(const float* rgb);
    const unsigned int GetType() const;
    std::string& GetName();

   private:
    std::string m_Name;
    static unsigned int m_LightNum;

   protected:
    glm::vec3 m_Color;
    unsigned int m_Type;
};