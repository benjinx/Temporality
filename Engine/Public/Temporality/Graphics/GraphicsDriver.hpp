#ifndef TEMPORALITY_GRAPHICS_DRIVER_HPP
#define TEMPORALITY_GRAPHICS_DRIVER_HPP

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace Temporality {

class GraphicsDriver
{
public:
    explicit GraphicsDriver() = default;

    virtual ~GraphicsDriver() = default;

    virtual void SetWindowTitle(const std::string& title) = 0;

    virtual std::string GetWindowTitle() = 0;

    virtual void SetWindowSize(const glm::ivec2& size) = 0;

    virtual glm::ivec2 GetWindowSize() = 0;

    virtual void ProcessEvents() = 0;

    virtual void SwapBuffers() = 0;
};

void SetGraphicsDriver(std::unique_ptr<GraphicsDriver> && driver);

GraphicsDriver* GetGraphicsDriver();

}

#endif // TEMPORALITY_GRAPHICS_DRIVER_HPP