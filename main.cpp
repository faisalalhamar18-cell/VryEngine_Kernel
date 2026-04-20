#include "VryWindow.h"
#include "VryLog.h"
#include "VryRHI.h"
#include "VryEvent1.h"

bool g_ApplicationRunning = true;

// دالة معالجة الأحداث (العقل المدبر)
void OnEvent(VryEngine::Event& e) {
    VryEngine::EventDispatcher dispatcher(e);

    // إذا استلمنا حدث إغلاق، نوقف الحلقة الرئيسية
    dispatcher.Dispatch<VryEngine::WindowCloseEvent>([](VryEngine::WindowCloseEvent& event) {
        VRY_LOG_INFO("Closing Window...");
        g_ApplicationRunning = false;
        return true;
    });

    // إذا تغير حجم النافذة
    dispatcher.Dispatch<VryEngine::WindowResizeEvent>([](VryEngine::WindowResizeEvent& event) {
        VRY_LOG_INFO("Window Resized to: {0}x{1}", event.GetWidth(), event.GetHeight());
        return true;
    });
}

int main() {
    VryEngine::Logger::Init();
    VRY_LOG_INFO("VryEngine Event System Online.");

    auto window = VryEngine::Window::Create(VryEngine::WindowProps("VryEngine - Events System", 1280, 720));
    
    // ربط النافذة بدالة المعالجة
    window->SetEventCallback(OnEvent);

    VryEngine::RHI* rhi = VryEngine::RHI::Create();
    rhi->Initialize(window->GetNativePointer(), 1280, 720);

    while (g_ApplicationRunning) {
        window->OnUpdate();

        rhi->SetClearColor(0.1f, 0.4f, 0.9f, 1.0f);
        rhi->Clear();
    }

    delete rhi;
    VRY_LOG_INFO("VryEngine Shutdown Cleanly.");
    return 0;
}