#include "App.hpp"

#include <Camera.hpp>
#include <DevUI.hpp>
#include <Log.hpp>
#include <Shader.hpp>

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

App* App::_sInst = nullptr;

const float TARGET_FPS = 60.0f;

App::~App() {
    DeleteShaders();

    //ImGui_ImplSDL2_Shutdown();

    delete _mWindow;
    _mWindow = nullptr;

    SDL_Quit();
}

void App::Run()
{
    const float targetElapsed = 1.0f / TARGET_FPS;

    float prevTime = 0.0f;
    float frameTime = 0.0f;
    SDL_Window* sdlWindow = _mWindow->GetSDLWindow();
    SDL_Event event;

    while (_mRunning)
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
            {
                _mRunning = false;
            }
            
            switch (event.type)
            {
                case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        _mWindow->OnWindowResize({ event.window.data1, event.window.data2 });
                    }
                }
                break;
            }
            
            // Input
            _mInput.ProcessEvent(&event);
        }
        //float currTime = (float)glfwGetTime();
        //float elapsed = currTime - prevTime;
        //frameTime += elapsed;
        //float dt = elapsed / targetElapsed;

        //HandleInput(dt);
        //Update(dt);

        /*if (frameTime >= targetElapsed)
        {
            frameTime = 0.0f;
            Render();
        }*/

        Render();
        //prevTime = currTime;
    }
}

bool App::Start()
{
    // Welcome
    printf("Temporality Engine v%s\n\n", VERSION);

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) > 0)
    {
        LogError("Failed to init SDL!\n");
        return false;
    }

    // Window
    _mWindow = new Window(1024, 768);

    // Display OpenGL info
    OpenGLInfo();

    // Load Engine Shaders
    printf("\nLoading Engine Shaders\n");
    AddShader("axis", new Shader({
        "shaders/axis.vert",
        "shaders/axis.frag" }));

    AddShader("defaultLighting", new Shader({
        "shaders/defaultLighting.vert",
        "shaders/defaultLighting.frag" }));

    // Clear Window
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup Scene
    _mCurrentScene->Start();

    // Register the options function into the UI
    //DevUI::RegisterOptionsFunc(&Scene::Options);

    // Controller
    _mInput.Init();
    
    return true;
}

void App::Update(float dt)
{
    _mCurrentScene->Update(dt);
}

void App::Render()
{
    _mWindow->Clear();

    _mCurrentScene->Render();

    //DevUI::Render();

    _mWindow->Present();
}

void App::OpenGLInfo()
{
    // OpenGL Basic Info
    LogInfo("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
    LogInfo("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
    LogInfo("OpenGL Version: %s\n", glGetString(GL_VERSION));
    LogInfo("GLSL Version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Anti-Aliasing
    int samples;
    glGetIntegerv(GL_SAMPLES, &samples);
    LogInfo("Anti-Aliasing: %dx\n", samples);

    // Binary Shader Formats
    GLint formats = 0;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
    LogInfo("Binary Shader Formats: %d\n", formats);

    // Max UBO Size
    int tmp;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &tmp);
    LogInfo("Max UBO Size: %d\n", tmp);

    // Max Vertex UBOs
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &tmp);
    LogInfo("Max Vertex UBOs: %d\n", tmp);

    // Max Fragment UBOs
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &tmp);
    LogInfo("Max Fragment UBOs: %d\n", tmp);

    // Max Geometry UBOs
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &tmp);
    LogInfo("Max Geometry UBOs: %d\n", tmp);

    // Max UBO Bindings
    int maxBindings;
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindings);
    LogInfo("Max UBO Bindings: %d\n", maxBindings);
}

void App::Screenshot()
{
    std::vector<unsigned int> pixels(3 * GetWindow()->GetWidth() * GetWindow()->GetHeight());

    glReadPixels(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    
    stbi_flip_vertically_on_write(true);

    time_t now = time(0);
    tm* localtm = localtime(&now);
    std::string name = "Screenshot " + std::to_string(localtm->tm_mon+1) + std::to_string(localtm->tm_mday) + std::to_string(localtm->tm_year+1900) + "_"
                                                 + std::to_string(localtm->tm_hour) + std::to_string(localtm->tm_min) + std::to_string(localtm->tm_sec) + ".png";

    LogVerbose("Date: %s, %s, %s, Time: %s:%s:%s\n", std::to_string(localtm->tm_mon+1), std::to_string(localtm->tm_mday), std::to_string(localtm->tm_year+1900)
                                                   , std::to_string(localtm->tm_hour), std::to_string(localtm->tm_min), std::to_string(localtm->tm_sec));

    int screenshot = stbi_write_png(name.c_str(), GetWindow()->GetWidth(), GetWindow()->GetHeight(), 3, pixels.data(), 3 * GetWindow()->GetWidth());

    if (screenshot)
        LogInfo("Screenshot successful!\n");
    else
        LogInfo("Screenshot unsuccessful.\n");
}

void App::AddShader(std::string name, Shader* shader)
{
    _mShaders[name] = shader;
}

Shader* App::GetShader(std::string name)
{
    if (_mShaders.find(name) == _mShaders.end())
    {
        return nullptr;
    }

    return _mShaders[name];
}

void App::DeleteShaders()
{
    // Destroy the shaders
    for (auto& shader : _mShaders)
        shader.second->Destroy();

    // Clear shader vector
    _mShaders.clear();
}

void App::ReloadShaders()
{
    for (auto s : _mShaders)
        s.second->Reload();
}

//void App::HandleInput(float dt)
//{
//    if (_mInputMap[GLFW_KEY_W])
//        _mCurrentCamera->HandleMovement(Direction::FORWARD, dt);
//    if (_mInputMap[GLFW_KEY_S])
//        _mCurrentCamera->HandleMovement(Direction::BACKWARD, dt);
//    if (_mInputMap[GLFW_KEY_A])
//        _mCurrentCamera->HandleMovement(Direction::LEFT, dt);
//    if (_mInputMap[GLFW_KEY_D])
//        _mCurrentCamera->HandleMovement(Direction::RIGHT, dt);
//    if (_mInputMap[GLFW_KEY_Q])
//        _mCurrentCamera->HandleMovement(Direction::UP, dt);
//    if (_mInputMap[GLFW_KEY_E])
//        _mCurrentCamera->HandleMovement(Direction::DOWN, dt);
//}

//switch (key) {
//case GLFW_KEY_F5: // Reloads shaders
//{
//    std::cout << "\nReloading shaders!\n";
//    ReloadShaders();
//    break;
//}
//
//case GLFW_KEY_PRINT_SCREEN:
//{
//    Screenshot();
//    break;
//}