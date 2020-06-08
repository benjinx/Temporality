#ifndef APPLICATION_H
#define APPLICATION_H

#include <Config.hpp>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Window.hpp>

class Shader;

class App
{
public:

    static App* Inst() { 
        return _sInst; 
    }

    App() { _sInst = this; }
    ~App();

    bool Start();
    void Run();
    void Quit() { _mRunning = false; }

    Window * GetWindow() { return _mWindow; }

    Scene * GetCurrentScene() { return _mCurrentScene; }
    void SetCurrentScene(Scene* scene) { _mCurrentScene = scene; }

    void Screenshot();

    void AddShader(std::string name, Shader* shader);
    Shader* GetShader(std::string name);
    void DeleteShaders();
    void ReloadShaders();

    void SetCurrentCamera(Camera* camera) { _mCurrentCamera = camera; }
    
    Camera* GetCurrentCamera() { return _mCurrentCamera; }

private:

    static App* _sInst;

    void Update(float dt);
    void Render();
    void HandleInput(float dt);
    void OpenGLInfo();

    Window* _mWindow = nullptr;
    Scene* _mCurrentScene = nullptr;
    
    std::unordered_map<int, bool> _mInputMap;

    // List of Shaders
    std::unordered_map<std::string, Shader*> _mShaders;
    
    float _mLastMX = -1, 
          _mLastMY = -1;

    Camera* _mCurrentCamera = nullptr;

    bool _mRunning = true;
};

#endif // APPLICATION_H
