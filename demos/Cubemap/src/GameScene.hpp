#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <Temporality.hpp>

class GameScene : public Scene
{
public:
    GameScene() {};

    void Start() override;
    void Update(float dt) override;
    void Render() override;

private:
    bool _mReflection = false,
         _mRefraction = false;
};

#endif // GAMESCENE_H