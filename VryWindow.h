#pragma once
#include "VryConfig.h"
#include "VryEvent1.h"
#include <string>
#include <functional>

namespace VryEngine {

    struct WindowProps {
        std::string Title;
        uint32_t Width, Height;
        WindowProps(std::string title = "VryEngine", uint32_t w = 1280, uint32_t h = 720)
            : Title(title), Width(w), Height(h) {}
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}
        virtual void OnUpdate() = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        
        // --- هذه هي الدالة المفقودة التي تسبب الخطأ ---
        virtual void* GetNativePointer() = 0; 

        static Window* Create(const WindowProps& props = WindowProps());
    };
}