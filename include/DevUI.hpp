#ifndef DEVUI_H
#define DEVUI_H

#include <Config.hpp>

#include <functional>

namespace DevUI
{
    void Start();
    void Render();

    void RegisterOptionsFunc(std::function<void()> func);
}

#endif // DEVUI_H
