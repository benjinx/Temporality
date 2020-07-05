#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GameObject.hpp>

class Skybox : public GameObject
{
public:
    Skybox();
    ~Skybox();

    unsigned int LoadCubemap(std::vector<std::string> faces);

    void Render() override;

private:
    unsigned int _mVAO, _mVBO, _mTexture;
};

#endif // SKYBOX_HPP