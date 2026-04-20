#include "MetalRHI.h"
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Cocoa/Cocoa.h>

namespace VryEngine {

    MetalRHI::MetalRHI() : m_Device(nullptr), m_CommandQueue(nullptr), m_Layer(nullptr) {}

    MetalRHI::~MetalRHI() {
        // التنظيف (سيتم لاحقاً)
    }

    bool MetalRHI::Initialize(void* windowHandle, uint32_t width, uint32_t height) {
        // 1. الحصول على كرت الشاشة (GPU)
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) return false;
        m_Device = (__bridge_retained void*)device;

        // 2. إنشاء طابور الأوامر
        m_CommandQueue = (__bridge_retained void*)[device newCommandQueue];

        // 3. تجهيز الطبقة الرسومية (Metal Layer) وربطها بالنافذة
        NSWindow* nswin = (__bridge NSWindow*)windowHandle;
        CAMetalLayer* layer = [CAMetalLayer layer];
        layer.device = device;
        layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        
        nswin.contentView.layer = layer;
        nswin.contentView.wantsLayer = YES;
        m_Layer = (__bridge_retained void*)layer;

        return true;
    }

    void MetalRHI::SetClearColor(float r, float g, float b, float a) {
        m_ClearColor[0] = r; m_ClearColor[1] = g; m_ClearColor[2] = b; m_ClearColor[3] = a;
    }

    void MetalRHI::Clear() {
        id<MTLCommandQueue> queue = (__bridge id<MTLCommandQueue>)m_CommandQueue;
        CAMetalLayer* layer = (__bridge CAMetalLayer*)m_Layer;
        
        id<CAMetalDrawable> drawable = [layer nextDrawable];
        if (!drawable) return;

        // تجهيز عملية المسح (تلوين الخلفية)
        MTLRenderPassDescriptor* pass = [MTLRenderPassDescriptor renderPassDescriptor];
        pass.colorAttachments[0].texture = drawable.texture;
        pass.colorAttachments[0].loadAction = MTLLoadActionClear;
        pass.colorAttachments[0].clearColor = MTLClearColorMake(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
        pass.colorAttachments[0].storeAction = MTLStoreActionStore;

        id<MTLCommandBuffer> buffer = [queue commandBuffer];
        id<MTLRenderCommandEncoder> encoder = [buffer renderCommandEncoderWithDescriptor:pass];
        [encoder endEncoding];
        
        [buffer presentDrawable:drawable];
        [buffer commit];
    }

    void MetalRHI::Present() {
        // في Metal، الـ Present يتم استدعاؤه داخل Clear/Render حالياً للتبسيط
    }
}