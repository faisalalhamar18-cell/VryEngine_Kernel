#pragma once
#include <string>
#include <functional>

namespace VryEngine {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    class Event {
    public:
        virtual ~Event() = default;
        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
    };

    // حدث إغلاق النافذة
    class WindowCloseEvent : public Event {
    public:
        static EventType GetStaticType() { return EventType::WindowClose; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "WindowClose"; }
    };

    // حدث تغيير حجم النافذة (مهم جداً للرندرة)
    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}
        unsigned int GetWidth() const { return m_Width; }
        unsigned int GetHeight() const { return m_Height; }

        static EventType GetStaticType() { return EventType::WindowResize; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "WindowResize"; }
    private:
        unsigned int m_Width, m_Height;
    };

    // موزع الأحداث (Dispatcher)
    class EventDispatcher {
    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        template<typename T, typename F>
        bool Dispatch(const F& func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };
}