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

#ifndef SKINNING_TECHNIQUE_H
#define	SKINNING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_util.h"
#include <glm/glm.hpp>

struct BaseLight
{
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;

    BaseLight()
    {
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        ambientIntensity = 0.0f;
        diffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{        
    glm::vec3 direction;

    DirectionalLight()
    {
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight2 : public BaseLight
{
    glm::vec3 position;

    struct
    {
        float constant;
        float linear;
        float exp;
    } attenuation;

    PointLight2()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        attenuation.constant = 1.0f;
        attenuation.linear = 0.0f;
        attenuation.exp = 0.0f;
    }
};

struct SpotLight : public PointLight2
{
    glm::vec3 direction;
    float cutoff;

    SpotLight()
    {
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
        cutoff = 0.0f;
    }
};

class SkinningTechnique : public Technique
{
public:

    static const unsigned int MAX_POINT_LIGHTS = 2;
    static const unsigned int MAX_SPOT_LIGHTS = 2;
    static const unsigned int MAX_BONES = 100;

    SkinningTechnique();

    virtual bool Init();

    void SetWVP(const glm::mat4& WVP);
    //void SeViewMatrix(const glm::mat4& view);
    //void SetPerspectiveMatrix(const glm::mat4& perspective);
    void SetWorldMatrix(const glm::mat4& WVP);
    void SetColorTextureUnit(unsigned int textureUnit);
    void SetDirectionalLight(const DirectionalLight& light);
    void SetPointLights(unsigned int numLights, const PointLight2* lights);
    void SetSpotLights(unsigned int numLights, const SpotLight* lights);
    void SetEyeWorldPos(const glm::vec3& eyeWorldPos);
    void SetMatSpecularIntensity(float intensity);
    void SetMatSpecularPower(float power);
    void SetBoneTransform(unsigned int index, const glm::mat4& transform);

private:
    
    GLuint m_WVPLocation;
    //GLuint m_ViewLocation;
    //GLuint m_PerspectiveLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_colorTextureLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_numPointLightsLocation;
    GLuint m_numSpotLightsLocation;

    struct {
        GLuint color;
        GLuint ambientIntensity;
        GLuint diffuseIntensity;
        GLuint direction;
    } m_dirLightLocation;

    struct {
        GLuint color;
        GLuint ambientIntensity;
        GLuint diffuseIntensity;
        GLuint position;
        struct {
            GLuint constant;
            GLuint linear;
            GLuint exp;
        } atten;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];

    struct {
        GLuint color;
        GLuint ambientIntensity;
        GLuint diffuseIntensity;
        GLuint position;
        GLuint direction;
        GLuint Cutoff;
        struct {
            GLuint constant;
            GLuint linear;
            GLuint exp;
        } atten;
    } m_spotLightsLocation[MAX_SPOT_LIGHTS];
    
    GLuint m_boneLocation[MAX_BONES];
};


#endif	/* SKINNING_TECHNIQUE_H */
