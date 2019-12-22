#include "Material.hpp"

#include <Shader.hpp>
#include <Texture.hpp>
#include <Log.hpp>

void Material::Bind(Shader* shader)
{
    shader->Use();

    // Set Materials
    shader->SetVec3("material.diffuse", _mDiffuse);
    shader->SetVec3("material.ambient", _mAmbient);
    shader->SetVec3("material.emissive", _mEmissive);
    shader->SetVec3("material.transparent", _mTransparent);
    shader->SetVec3("material.reflective", _mReflective);

    // Set Textures

    // Diffuse Map
    if (_mDiffuseMap)
    {
        shader->SetBool("material.hasDiffuseMap", true);
        shader->SetInt("material.diffuseMap", Material::TextureID::DIFFUSE);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::DIFFUSE);
        _mDiffuseMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasDiffuseMap", false);
    }

    // Specular Map
    if (_mSpecularMap)
    {
        shader->SetBool("material.hasSpecularMap", true);
        shader->SetInt("material.specularMap", Material::TextureID::SPECULAR);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::SPECULAR);
        _mSpecularMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasSpecularMap", false);
    }

    // Ambient Map
    if (_mAmbientMap)
    {
        shader->SetBool("material.hasAmbientMap", true);
        shader->SetInt("material.ambientMap", Material::TextureID::AMBIENT);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::AMBIENT);
        _mAmbientMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasAmbientMap", false);
    }

    // Emissive Map
    if (_mEmissiveMap)
    {
        shader->SetBool("material.hasEmissiveMap", true);
        shader->SetInt("material.emissiveMap", Material::TextureID::EMISSIVE);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::EMISSIVE);
        _mEmissiveMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasEmissiveMap", false);
    }

    // Height Map
    if (_mHeightMap)
    {
        shader->SetBool("material.hasHeightMap", true);
        shader->SetInt("material.heightMap", Material::TextureID::HEIGHT);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::HEIGHT);
        _mHeightMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasHeightMap", false);
    }

    // Normal Map
    if (_mNormalMap)
    {
        shader->SetBool("material.hasNormalMap", true);
        shader->SetInt("material.normalMap", Material::TextureID::NORMAL);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::NORMAL);
        _mNormalMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasNormalMap", false);
    }

    // Shininess Map
    if (_mShininessMap)
    {
        shader->SetBool("material.hasShininessMap", true);
        shader->SetInt("material.shininessMap", Material::TextureID::SHININESS);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::SHININESS);
        _mShininessMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasShininessMap", false);
    }

    // Opacity Map
    if (_mOpacityMap)
    {
        shader->SetBool("material.hasOpacityMap", true);
        shader->SetInt("material.opacityMap", Material::TextureID::OPACITY);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::OPACITY);
        _mOpacityMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasOpacityMap", false);
    }

    // Displacement Map
    if (_mDisplacementMap)
    {
        shader->SetBool("material.hasDisplacementMap", true);
        shader->SetInt("material.displacementMap", Material::TextureID::DISPLACEMENT);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::DISPLACEMENT);
        _mDisplacementMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasDisplacementMap", false);
    }

    // Light Map
    if (_mLightMap)
    {
        shader->SetBool("material.hasLightMap", true);
        shader->SetInt("material.lightMap", Material::TextureID::LIGHT_MAP);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::LIGHT_MAP);
        _mLightMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasLightMap", false);
    }

    // Reflection Map
    if (_mReflectionMap)
    {
        shader->SetBool("material.hasReflectionMap", true);
        shader->SetInt("material.reflectionMap", Material::TextureID::REFLECTION);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::REFLECTION);
        _mReflectionMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasReflectionMap", false);
    }

    // Base Color Map
    if (_mBaseColorMap)
    {
        shader->SetBool("material.hasBaseColorMap", true);
        shader->SetInt("material.baseColorMap", Material::TextureID::BASE_COLOR);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::BASE_COLOR);
        _mBaseColorMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasBaseColorMap", false);
    }

    // Normal Camera Map
    if (_mNormalCameraMap)
    {
        shader->SetBool("material.hasNormalCameraMap", true);
        shader->SetInt("material.normalCameraMap", Material::TextureID::NORMAL_CAMERA);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::NORMAL_CAMERA);
        _mNormalCameraMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasNormalCameraMap", false);
    }

    // Emission Color Map
    if (_mEmissionColorMap)
    {
        shader->SetBool("material.hasEmissionColorMap", true);
        shader->SetInt("material.emissionColorMap", Material::TextureID::EMISSION_COLOR);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::EMISSION_COLOR);
        _mEmissionColorMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasEmissionColorMap", false);
    }

    // Metalness Map
    if (_mMetalnessMap)
    {
        shader->SetBool("material.hasMetalnessMap", true);
        shader->SetInt("material.metalnessMap", Material::TextureID::METALNESS);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::METALNESS);
        _mMetalnessMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasMetalnessMap", false);
    }

    // Diffuse Roughness Map
    if (_mDiffuseRoughnessMap)
    {
        shader->SetBool("material.hasDiffuseRoughnessMap", true);
        shader->SetInt("material.diffuseRoughnessMap", Material::TextureID::DIFFUSE_ROUGHNESS);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::DIFFUSE_ROUGHNESS);
        _mDiffuseRoughnessMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasDiffuseRoughnessMap", false);
    }

    // Ambient Occlusion Map
    if (_mAmbientOcclusionMap)
    {
        shader->SetBool("material.hasAmbientOcclusionMap", true);
        shader->SetInt("material.ambientOcclusionMap", Material::TextureID::AMBIENT_OCCLUSION);
        glActiveTexture(GL_TEXTURE0 + Material::TextureID::AMBIENT_OCCLUSION);
        _mAmbientOcclusionMap->Bind();
    }
    else
    {
        shader->SetBool("material.hasAmbientOcclusionMap", false);
    }
}
