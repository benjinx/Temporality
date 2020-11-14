#include <Temporality/Module.hpp>
#include <Temporality/OpenGL/GraphicsDriver.hpp>

namespace Temporality::OpenGL {

void ModuleInit() {
    SetGraphicsDriver(std::unique_ptr<Temporality::GraphicsDriver>(new GraphicsDriver()));
}

void ModuleTerm() {
    SetGraphicsDriver(nullptr);
}

extern "C" struct TemporalityModule _TemporalityModule = {
    "OpenGL",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::OpenGL