#include "VryWindow.h"
#include "VryLog.h"      // سيجده الآن تلقائياً بفضل CMake
#include "VryEvent1.h"    // سيجده الآن تلقائياً بفضل CMake

#include <string>
#include <functional>

struct GLFWwindow;

namespace VryEngine {
    class WindowImpl : public Window {
    public:
        WindowImpl(const WindowProps& props);
        virtual ~WindowImpl();

        void OnUpdate() override;
        unsigned int GetWidth() const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        void SetEventCallback(const std::function<void(Event&)>& callback) override;
        void* GetNativePointer() const override;

    private:
        GLFWwindow* m_Window;
        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            std::function<void(Event&)> EventCallback;
        } m_Data;
    };
}

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace VryEngine {

    WindowImpl::WindowImpl(const WindowProps& props) {
        if (!glfwInit()) return;

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width, height);
            if (data.EventCallback) data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            if (data.EventCallback) data.EventCallback(event);
        });
    }

    WindowImpl::~WindowImpl() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void WindowImpl::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowImpl::SetEventCallback(const std::function<void(Event&)>& callback) {
        m_Data.EventCallback = callback;
    }

    void* WindowImpl::GetNativePointer() const {
        return (void*)glfwGetCocoaWindow(m_Window);
    }
}