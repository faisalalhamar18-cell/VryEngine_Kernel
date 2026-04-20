#include "VryRHI.h"

// هنا سيتم استدعاء ملفات الـ Backend بناءً على المنصة
#ifdef __APPLE__
    #include "MetalRHI.h" 
#endif

namespace VryEngine {

    // نحدد Metal كخيار افتراضي للماك
    RendererAPI RHI::s_API = RendererAPI::Metal;

    RHI* RHI::Create() {
        switch (s_API) {
            case RendererAPI::None:    return nullptr;
            case RendererAPI::Metal:   
                #ifdef __APPLE__
                    return new MetalRHI(); 
                #else
                    return nullptr;
                #endif
            case RendererAPI::DirectX11: return nullptr; // للويندوز مستقبلاً
            case RendererAPI::Vulkan:    return nullptr;
        }
        return nullptr;
    }
}