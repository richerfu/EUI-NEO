#include "core/window/window_backend.h"

#if defined(EUI_WINDOW_BACKEND_SDL2)
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif
#include <SDL.h>
#else
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#endif

#include <chrono>
#include <thread>

namespace {

core::window::RenderApi configuredRenderApi() {
#if defined(EUI_RENDER_BACKEND_VULKAN)
    return core::window::RenderApi::Vulkan;
#else
    return core::window::RenderApi::OpenGL;
#endif
}

void sleepBriefly() {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

} // namespace

int main() {
#if defined(EUI_WINDOW_BACKEND_SDL2)
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        return 1;
    }
#else
    if (!glfwInit()) {
        return 1;
    }
#endif

    core::window::WindowCreateRequest request;
    request.width = 800;
    request.height = 600;
    request.title = "Window Only Probe";
    request.renderApi = configuredRenderApi();

    core::window::Handle window = core::window::createWindow(request);
    if (window == nullptr) {
#if defined(EUI_WINDOW_BACKEND_SDL2)
        SDL_Quit();
#else
        glfwTerminate();
#endif
        return 1;
    }

#if defined(EUI_WINDOW_BACKEND_SDL2)
    bool running = true;
    while (running) {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)) {
                running = false;
            }
        }
        sleepBriefly();
    }
#else
    while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(window))) {
        glfwPollEvents();
        sleepBriefly();
    }
#endif

    core::window::destroyWindow(window);
#if defined(EUI_WINDOW_BACKEND_SDL2)
    SDL_Quit();
#else
    glfwTerminate();
#endif
    return 0;
}
