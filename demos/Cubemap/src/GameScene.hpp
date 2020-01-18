#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

class GameScene : public Scene
{
public:
    GameScene() {};

    void Start() override;
    void Update(float dt) override;

    unsigned int LoadCubemap(std::vector<std::string> faces);

private:
    unsigned int skyboxVAO, skyboxVBO, cubemapTexture;
};

#endif // GAMESCENE_H