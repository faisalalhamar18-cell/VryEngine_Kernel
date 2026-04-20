#pragma once
#include <cstdint>

namespace VryEngine {

    enum class RendererAPI {
        None = 0, Metal = 1, DirectX11 = 2, Vulkan = 3
    };

    class RHI {
    public:
        virtual ~RHI() = default;

        // دوال التحكم الأساسية التي يجب على كل Backend تطبيقها
        virtual bool Initialize(void* windowHandle, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear() = 0;
        virtual void Present() = 0;

        // دالة إنشاء الـ Backend المناسب للمنصة الحالية
        static RHI* Create();
        
        static RendererAPI GetAPI() { return s_API; }

    protected:
        static RendererAPI s_API;
    };
}