/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <limits.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "skinning_technique.h"
#include "Basic/Utils.h"
#include "../Texture.h"
#include "Graphics/Lights.h"

namespace Animation
{
    SkinningTechnique::SkinningTechnique()
    {   
    }


    bool SkinningTechnique::Init()
    {
        if (!Technique::Init())
        {
            return false;
        }

        if (!AddShader(GL_VERTEX_SHADER, "src/Graphics/Shaders/skinning.vert"))
        {
            return false;
        }

        if (!AddShader(GL_FRAGMENT_SHADER, "src/Graphics/Shaders/skinning.frag"))
        {
            return false;
        }

        if (!Finalize())
        {
            return false;
        }
    
        m_WVPLocation = GetUniformLocation("gWVP");
        m_WorldMatrixLocation = GetUniformLocation("gWorld");
        m_lightSpaceMatrix = GetUniformLocation("lightSpaceMatrix");
        m_colorTextureLocation = GetUniformLocation("gColorMap");
        m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
        m_dirLightLocation.color = GetUniformLocation("gDirectionalLight.Base.Color");
        m_dirLightLocation.ambientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
        m_dirLightLocation.direction = GetUniformLocation("gDirectionalLight.Direction");
        m_dirLightLocation.diffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
        m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
        m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
        m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
        m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");

        if (m_dirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION ||
            m_WVPLocation == INVALID_UNIFORM_LOCATION ||
            m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
            m_colorTextureLocation == INVALID_UNIFORM_LOCATION ||
            m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
            m_dirLightLocation.color == INVALID_UNIFORM_LOCATION ||
            m_dirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION ||
            m_dirLightLocation.direction == INVALID_UNIFORM_LOCATION ||
            m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
            m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
            m_numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
            m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION)
        {
            return false;
        }

        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation) ; i++)
        {
            char name[128];
            memset(name, 0, sizeof(name));
            _snprintf_s(name, sizeof(name), "gPointLights[%d].Base.Color", i);
            m_pointLightsLocation[i].color = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gPointLights[%d].Base.AmbientIntensity", i);
            m_pointLightsLocation[i].ambientIntensity = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gPointLights[%d].Position", i);
            m_pointLightsLocation[i].position = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gPointLights[%d].Base.DiffuseIntensity", i);
            m_pointLightsLocation[i].diffuseIntensity = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gPointLights[%d].Atten.Constant", i);
            m_pointLightsLocation[i].atten.constant = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gPointLights[%d].Atten.Linear", i);
            m_pointLightsLocation[i].atten.linear = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gPointLights[%d].Atten.Exp", i);
            m_pointLightsLocation[i].atten.exp = GetUniformLocation(name);

            if (m_pointLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
                m_pointLightsLocation[i].ambientIntensity == INVALID_UNIFORM_LOCATION ||
                m_pointLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
                m_pointLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
                m_pointLightsLocation[i].atten.constant == INVALID_UNIFORM_LOCATION ||
                m_pointLightsLocation[i].atten.linear == INVALID_UNIFORM_LOCATION ||
                m_pointLightsLocation[i].atten.exp == INVALID_UNIFORM_LOCATION)
            {
                return false;
            }
        }

        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation) ; i++)
        {
            char name[128];
            memset(name, 0, sizeof(name));
            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Base.Color", i);
            m_spotLightsLocation[i].color = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
            m_spotLightsLocation[i].ambientIntensity = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Position", i);
            m_spotLightsLocation[i].position = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Direction", i);
            m_spotLightsLocation[i].direction = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Cutoff", i);
            m_spotLightsLocation[i].Cutoff = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
            m_spotLightsLocation[i].diffuseIntensity = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Atten.Constant", i);
            m_spotLightsLocation[i].atten.constant = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Atten.Linear", i);
            m_spotLightsLocation[i].atten.linear = GetUniformLocation(name);

            _snprintf_s(name, sizeof(name), "gSpotLights[%d].Base.Atten.Exp", i);
            m_spotLightsLocation[i].atten.exp = GetUniformLocation(name);

            if (m_spotLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].ambientIntensity == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].direction == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].atten.constant == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].atten.linear == INVALID_UNIFORM_LOCATION ||
                m_spotLightsLocation[i].atten.exp == INVALID_UNIFORM_LOCATION)
            {
                return false;
            }
        }

        for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++)
        {
            char name[128];
            memset(name, 0, sizeof(name));
            _snprintf_s(name, sizeof(name), "gBones[%d]", i);
            m_boneLocation[i] = GetUniformLocation(name);
        }

        return true;
    }


    void SkinningTechnique::SetWVP(const glm::mat4& WVP)
    {
        glUniformMatrix4fv(m_WVPLocation, 1, GL_FALSE, glm::value_ptr(WVP));
    }

    void SkinningTechnique::SetWorldMatrix(const glm::mat4& world)
    {
        glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_FALSE, glm::value_ptr(world));
    }

    void SkinningTechnique::SetLightSpaceMatrix(const glm::mat4 & lightSpaceMatrix)
    {
        glUniformMatrix4fv(m_lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    }

    void SkinningTechnique::SetRenderingDepth(bool renderingDepth)
    {
        glUniform1i(GetUniformLocation("renderingDepth"), renderingDepth);
    }

    void SkinningTechnique::SetDepthMap(std::string depthMapName, GLuint shadowMapIndex, GLuint textureID)
    {
        glUniform1i(GetUniformLocation(depthMapName.c_str()), shadowMapIndex);
        glActiveTexture(GL_TEXTURE0 + shadowMapIndex);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void SkinningTechnique::SetColorTextureUnit(unsigned int textureUnit)
    {
        glUniform1i(m_colorTextureLocation, textureUnit);
    }


    void SkinningTechnique::SetDirectionalLight(const DirectionalLight& light)
    {
        glUniform3f(m_dirLightLocation.color, light.color.x, light.color.y, light.color.z);
        glUniform1f(m_dirLightLocation.ambientIntensity, light.ambientIntensity);
        glm::vec3 Direction = glm::normalize(light.direction);
        glUniform3f(m_dirLightLocation.direction, Direction.x, Direction.y, Direction.z);
        glUniform1f(m_dirLightLocation.diffuseIntensity, light.diffuseIntensity);
    }

    void SkinningTechnique::SetDirectionalLight(const ::DirectionalLight& light)
    {
        glUniform3f(m_dirLightLocation.color, light._color.x, light._color.y, light._color.z);
        glUniform1f(m_dirLightLocation.ambientIntensity, light._ambientIntensity);
        glm::vec3 direction = glm::normalize(light._direction);
        glUniform3f(m_dirLightLocation.direction, direction.x, direction.y, direction.z);
        glUniform1f(m_dirLightLocation.diffuseIntensity, light._diffuseIntensity);
    }


    void SkinningTechnique::SetEyeWorldPos(const glm::vec3& eyeWorldPos)
    {
        glUniform3f(m_eyeWorldPosLocation, eyeWorldPos.x, eyeWorldPos.y, eyeWorldPos.z);
    }


    void SkinningTechnique::SetMatSpecularIntensity(float intensity)
    {
        glUniform1f(m_matSpecularIntensityLocation, intensity);
    }


    void SkinningTechnique::SetMatSpecularPower(float power)
    {
        glUniform1f(m_matSpecularPowerLocation, power);
    }


    void SkinningTechnique::SetPointLights(unsigned int numLights, const PointLight2* lights)
    {
        glUniform1i(m_numPointLightsLocation, numLights);
    
        for (unsigned int i = 0 ; i < numLights ; i++)
        {
            glUniform3f(m_pointLightsLocation[i].color, lights[i].color.x, lights[i].color.y, lights[i].color.z);
            glUniform1f(m_pointLightsLocation[i].ambientIntensity, lights[i].ambientIntensity);
            glUniform1f(m_pointLightsLocation[i].diffuseIntensity, lights[i].diffuseIntensity);
            glUniform3f(m_pointLightsLocation[i].position, lights[i].position.x, lights[i].position.y, lights[i].position.z);
            glUniform1f(m_pointLightsLocation[i].atten.constant, lights[i].attenuation.constant);
            glUniform1f(m_pointLightsLocation[i].atten.linear, lights[i].attenuation.linear);
            glUniform1f(m_pointLightsLocation[i].atten.exp, lights[i].attenuation.exp);
        }
    }

    void SkinningTechnique::SetSpotLights(unsigned int numLights, const SpotLight* lights)
    {
        glUniform1i(m_numSpotLightsLocation, numLights);

        for (unsigned int i = 0 ; i < numLights ; i++)
        {
            glUniform3f(m_spotLightsLocation[i].color, lights[i].color.x, lights[i].color.y, lights[i].color.z);
            glUniform1f(m_spotLightsLocation[i].ambientIntensity, lights[i].ambientIntensity);
            glUniform1f(m_spotLightsLocation[i].diffuseIntensity, lights[i].diffuseIntensity);
            glUniform3f(m_spotLightsLocation[i].position,  lights[i].position.x, lights[i].position.y, lights[i].position.z);
            glm::vec3 direction = glm::normalize(lights[i].direction);
            glUniform3fv(m_spotLightsLocation[i].direction, 1, glm::value_ptr(direction));
            glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(glm::radians(lights[i].cutoff)));
            glUniform1f(m_spotLightsLocation[i].atten.constant, lights[i].attenuation.constant);
            glUniform1f(m_spotLightsLocation[i].atten.linear,   lights[i].attenuation.linear);
            glUniform1f(m_spotLightsLocation[i].atten.exp,      lights[i].attenuation.exp);
        }
    }

    void SkinningTechnique::SetBoneTransform(unsigned int index, const glm::mat4& transform)
    {
        assert(index < MAX_BONES);
        glUniformMatrix4fv(m_boneLocation[index], 1, false, glm::value_ptr(transform));
    }

    void SkinningTechnique::SetMaterial(const Material& material)
    {
        glUniform3fv(GetUniformLocation("material._diffuse"), 1, glm::value_ptr(material._diffuse));
        glUniform3fv(GetUniformLocation("material._specular"), 1, glm::value_ptr(material._specular));
        glUniform3fv(GetUniformLocation("material._ambient"),1, glm::value_ptr(material._ambient));
        glUniform1f(GetUniformLocation("material._shininess"), material._shininess);
    }

    void SkinningTechnique::SetUseTexture(bool useTexture)
    {
        glUniform1i(GetUniformLocation("useTexture"), useTexture);
    }
}
