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
        SPECULAR = 1,
        AMBIENT = 2,
        EMISSIVE = 3,
        HEIGHT = 4,
        NORMAL = 5,
        SHININESS = 6,
        OPACITY = 7,
        DISPLACEMENT = 8,
        LIGHT_MAP = 9,
        REFLECTION = 10,
        BASE_COLOR = 11,
        NORMAL_CAMERA = 12,
        EMISSION_COLOR = 13,
        METALNESS = 14,
        DIFFUSE_ROUGHNESS = 15,
        AMBIENT_OCCLUSION = 16,
    };

    Material() = default;
    virtual ~Material() = default;

    void Bind(Shader* shader);

    void SetDiffuse(glm::vec3 value) { _mDiffuse = value; }
    void SetSpecular(glm::vec3 value) { _mSpecular = value; }
    void SetAmbient(glm::vec3 value) { _mAmbient = value; }
    void SetEmissive(glm::vec3 value) { _mEmissive = value; }
    void SetTransparent(glm::vec3 value) { _mTransparent = value; }
    void SetReflective(glm::vec3 value) { _mReflective = value; }

    void SetDiffuseMap(std::unique_ptr<Texture>&& texture) { _mDiffuseMap = std::move(texture); }
    void SetSpecularMap(std::unique_ptr<Texture>&& texture) { _mSpecularMap = std::move(texture); }
    void SetAmbientMap(std::unique_ptr<Texture>&& texture) { _mAmbientMap = std::move(texture); }
    void SetEmissiveMap(std::unique_ptr<Texture>&& texture) { _mEmissiveMap = std::move(texture); }
    void SetHeightMap(std::unique_ptr<Texture>&& texture) { _mHeightMap = std::move(texture); }
    void SetNormalMap(std::unique_ptr<Texture>&& texture) { _mNormalMap = std::move(texture); }
    void SetShininessMap(std::unique_ptr<Texture>&& texture) { _mShininessMap = std::move(texture); }
    void SetOpacityMap(std::unique_ptr<Texture>&& texture) { _mOpacityMap = std::move(texture); }
    void SetDisplacementMap(std::unique_ptr<Texture>&& texture) { _mDisplacementMap = std::move(texture); }
    void SetLightMap(std::unique_ptr<Texture>&& texture) { _mLightMap = std::move(texture); }
    void SetReflectionMap(std::unique_ptr<Texture>&& texture) { _mReflectionMap = std::move(texture); }
    void SetBaseColorMap(std::unique_ptr<Texture>&& texture) { _mBaseColorMap = std::move(texture); }
    void SetNormalCameraMap(std::unique_ptr<Texture>&& texture) { _mNormalCameraMap = std::move(texture); }
    void SetEmissionColorMap(std::unique_ptr<Texture>&& texture) { _mEmissionColorMap = std::move(texture); }
    void SetMetalnessMap(std::unique_ptr<Texture>&& texture) { _mMetalnessMap = std::move(texture); }
    void SetDiffuseRoughnessMap(std::unique_ptr<Texture>&& texture) { _mDiffuseRoughnessMap = std::move(texture); }
    void SetAmbientOcclusionMap(std::unique_ptr<Texture>&& texture) { _mAmbientOcclusionMap = std::move(texture); }

    bool DiffuseMapExists() { return (_mDiffuseMap != nullptr); }
    bool SpecularMapExists() { return (_mSpecularMap != nullptr); }
    bool AmbientMapExists() { return (_mAmbientMap != nullptr); }
    bool EmissiveMapExists() { return (_mEmissiveMap != nullptr); }
    bool HeightMapExists() { return (_mHeightMap != nullptr); }
    bool NormalMapExists() { return (_mNormalMap != nullptr); }
    bool ShininessMapExists() { return (_mShininessMap != nullptr); }
    bool OpacityMapExists() { return (_mOpacityMap != nullptr); }
    bool DisplacementMapExists() { return (_mDisplacementMap != nullptr); }
    bool LightMapExists() { return (_mLightMap != nullptr); }
    bool ReflectionMapExists() { return (_mReflectionMap != nullptr); }
    bool BaseColorMapExists() { return (_mBaseColorMap != nullptr); }
    bool NormalCameraMapExists() { return (_mNormalCameraMap != nullptr); }
    bool EmissionColorMapExists() { return (_mEmissionColorMap != nullptr); }
    bool MetalnessMapExists() { return (_mMetalnessMap != nullptr); }
    bool DiffuseRoughnessMapExists() { return (_mDiffuseRoughnessMap != nullptr); }
    bool AmbientOcclusionMapExists() { return (_mAmbientOcclusionMap != nullptr); }

private:
    glm::vec3   _mDiffuse = glm::vec3(1.0f, 1.0f, 1.0f),
                _mSpecular = glm::vec3(0),
                _mAmbient = glm::vec3(0),
                _mEmissive = glm::vec3(0),
                _mTransparent = glm::vec3(0),
                _mReflective = glm::vec3(0);

    //std::unordered_map<std::string, std::unique_ptr<Texture>> Textures;
    std::unique_ptr<Texture>    _mDiffuseMap,
                                _mSpecularMap,
                                _mAmbientMap,
                                _mEmissiveMap,
                                _mHeightMap,
                                _mNormalMap,
                                _mShininessMap,
                                _mOpacityMap,
                                _mDisplacementMap,
                                _mLightMap,
                                _mReflectionMap,
                                _mBaseColorMap,
                                _mNormalCameraMap,
                                _mEmissionColorMap,
                                _mMetalnessMap,
                                _mDiffuseRoughnessMap,
                                _mAmbientOcclusionMap;
};

#endif // MATERIAL_H
