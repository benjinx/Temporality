#include <Temporality.hpp>

#include "GameScene.hpp"

int main(int argc, char * argv[])
{
    Utils::SetAssetPaths(RESOURCE_PATH);
    const auto& paths = Utils::GetResourcePaths();
    for (auto& p : paths) {
        //printf("%s\n", p.c_str());
        LogInfo("%s\n", p);
    }

    App app;
    Scene* gameScene = new GameScene();

    app.SetCurrentScene(gameScene);

    if (!app.Start())
    {
        return EXIT_FAILURE;
    }

    app.Run();

    delete gameScene;

    return EXIT_SUCCESS;
}
