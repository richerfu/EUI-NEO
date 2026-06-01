#include "core/render/render_backend.h"
#include "core/render/render_types.h"
#include "core/window/window_backend.h"

#if defined(EUI_WINDOW_BACKEND_SDL2)
#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif
#include <SDL.h>
#if defined(EUI_RENDER_BACKEND_VULKAN)
#include <SDL_vulkan.h>
#endif
#else
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <GLFW/glfw3.h>
#endif

#include <chrono>
#include <memory>
#include <thread>

namespace {

void sleepBriefly() {
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
}

void drawableSize(core::window::Handle window, int& width, int& height) {
#if defined(EUI_WINDOW_BACKEND_SDL2)
#if defined(EUI_RENDER_BACKEND_VULKAN)
    SDL_Vulkan_GetDrawableSize(static_cast<SDL_Window*>(window), &width, &height);
#else
    SDL_GL_GetDrawableSize(static_cast<SDL_Window*>(window), &width, &height);
#endif
#else
    glfwGetFramebufferSize(static_cast<GLFWwindow*>(window), &width, &height);
#endif
}

void renderClearFrame(core::window::Handle window, core::render::RenderBackend& backend) {
    int width = 0;
    int height = 0;
    drawableSize(window, width, height);
    if (width <= 0 || height <= 0) {
        return;
    }

    backend.makeCurrent();
    backend.beginFrame({
        window,
        core::window::nativeWindowInfo(window),
        width,
        height,
        1.0f
    });
    backend.clear({0.16f, 0.18f, 0.20f, 1.0f});
    backend.present();
}

} // namespace

int main() {
    core::render::initializeRenderBackendLoader();

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
    request.title = "Render Clear Probe";
    request.renderApi = core::render::windowRenderApi();

    core::window::Handle window = core::window::createWindow(request);
    if (window == nullptr) {
#if defined(EUI_WINDOW_BACKEND_SDL2)
        SDL_Quit();
#else
        glfwTerminate();
#endif
        return 1;
    }

    std::unique_ptr<core::render::RenderBackend> backend = core::render::createRenderBackend(window);
    if (!backend || !backend->initialize()) {
        core::window::destroyWindow(window);
#if defined(EUI_WINDOW_BACKEND_SDL2)
        SDL_Quit();
#else
        glfwTerminate();
#endif
        return 1;
    }

    renderClearFrame(window, *backend);

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

    backend->makeCurrent();
    backend->releaseRenderCache();
    backend.reset();
    core::window::destroyWindow(window);
#if defined(EUI_WINDOW_BACKEND_SDL2)
    SDL_Quit();
#else
    glfwTerminate();
#endif
    return 0;
}
