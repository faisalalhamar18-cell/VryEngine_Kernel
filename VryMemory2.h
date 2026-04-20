#include <cstdint>
#include <cstdlib>
#include <utility>
#include <new>

namespace VryEngine {

    class StackAllocator {
    public:
        // نستخدم typedef لتعريف "Marker" يمثل نقطة في الذاكرة للعودة إليها
        using Marker = size_t;

        explicit StackAllocator(size_t size) : m_size(size), m_offset(0) {
            m_buffer = static_cast<uint8_t*>(std::malloc(m_size));
        }

        ~StackAllocator() {
            std::free(m_buffer);
        }

        // تخصيص مساحة
        void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
            size_t current_ptr = reinterpret_cast<size_t>(m_buffer) + m_offset;
            size_t padding = (alignment - (current_ptr % alignment)) % alignment;

            if (m_offset + padding + size > m_size) return nullptr;

            void* ptr = m_buffer + m_offset + padding;
            m_offset += padding + size;
            return ptr;
        }

        // --- الميزة الإضافية في الـ Stack ---

        // الحصول على "علامة" للمكان الحالي في الذاكرة
        Marker GetMarker() const {
            return m_offset;
        }

        // العودة إلى علامة سابقة (إلغاء تخصيص كل ما بعدها)
        void FreeToMarker(Marker marker) {
            if (marker <= m_offset) {
                m_offset = marker;
            }
        }

        // مسح الـ Stack بالكامل
        void Clear() {
            m_offset = 0;
        }

    private:
        uint8_t* m_buffer;
        size_t m_size;
        size_t m_offset;

        StackAllocator(const StackAllocator&) = delete;
        StackAllocator& operator=(const StackAllocator&) = delete;
    };

}