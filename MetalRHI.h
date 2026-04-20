#pragma once
#include "VryRHI.h"

// ملاحظة: نستخدم ملفات تعريفية بسيطة هنا لتجنب تداخل C++ مع Objective-C
namespace VryEngine {

    class MetalRHI : public RHI {
    public:
        MetalRHI();
        virtual ~MetalRHI();

        virtual bool Initialize(void* windowHandle, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(float r, float g, float b, float a) override;
        virtual void Clear() override;
        virtual void Present() override;

    private:
        void* m_Device;        // MTLDevice
        void* m_CommandQueue;  // MTLCommandQueue
        void* m_Layer;         // CAMetalLayer
        float m_ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    };

}