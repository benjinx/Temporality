#ifndef MATERIAL_H
#define MATERIAL_H

#include <Config.hpp>
#include <Math.hpp>
#include <Texture.hpp>

class Shader;

#include <memory>

class Material
{
public:

    enum TextureID {
        DIFFUSE = 0,
        METALLIC_ROUGHNESS = 1,
        NORMAL = 2,
        EMISSIVE = 3,
        OCCLUSION = 4,
    };

    Material() = default;
    virtual ~Material() = default;

    void Bind(Shader* shader);

    void SetDiffuse(glm::vec4 value) { _mDiffuse = glm::vec3(value); }
    void SetEmissive(glm::vec3 value) { _mEmissive = value; }

    void SetRoughness(float value) { _mRoughness = value; }
    void SetMetallic(float value) { _mMetallic = value; }
    void SetNormalScale(float value) { _mNormalScale = value; }
    void SetOcclusionStrength(float value) { _mOcclusionStrength = value; }

    void SetDiffuseMap(std::unique_ptr<Texture>&& texture) { _mDiffuseMap = std::move(texture); }
    void SetMetallicRoughnessMap(std::unique_ptr<Texture>&& texture) { _mMetallicRoughnessMap = std::move(texture); }
    void SetNormalMap(std::unique_ptr<Texture>&& texture) { _mNormalMap = std::move(texture); }
    void SetEmissiveMap(std::unique_ptr<Texture>&& texture) { _mEmissiveMap = std::move(texture); }
    void SetOcclusionMap(std::unique_ptr<Texture>&& texture) { _mOcclusionMap = std::move(texture); }

    bool DiffuseMapExists() { return (_mDiffuseMap != nullptr); }
    bool NormalMapExists() { return (_mNormalMap != nullptr); }
    bool MetallicRoughnessMapExists() { return (_mMetallicRoughnessMap != nullptr); }
    bool EmissiveMapExists() { return (_mEmissiveMap != nullptr); }
    bool OcclusionMapExists() { return (_mOcclusionMap != nullptr); }

private:
    glm::vec3   _mDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
                _mEmissive = glm::vec3(0);

    float       _mMetallic = 0.0f, 
                _mRoughness = 0.0f,
                _mNormalScale = 1.0f,
                _mOcclusionStrength = 0.0f;

    std::unique_ptr<Texture>    _mDiffuseMap,
                                _mMetallicRoughnessMap,
                                _mNormalMap,
                                _mEmissiveMap,
                                _mOcclusionMap;
};

#endif // MATERIAL_H
