#include <cstdint>
#include <cstdlib>
#include <utility>
#include <new>

namespace VryEngine {

    class ArenaAllocator {
    public:
        // حجز بلوك الذاكرة مسبقاً
        explicit ArenaAllocator(size_t size) : m_size(size), m_offset(0) {
            m_buffer = static_cast<uint8_t*>(std::malloc(m_size));
        }

        ~ArenaAllocator() {
            std::free(m_buffer);
        }

        // 1. تخصيص مساحة خام (Raw Memory)
        void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
            size_t current_ptr = reinterpret_cast<size_t>(m_buffer) + m_offset;
            size_t padding = (alignment - (current_ptr % alignment)) % alignment;

            if (m_offset + padding + size > m_size) {
                return nullptr; // فشل التخصيص: المساحة ممتلئة
            }

            void* ptr = m_buffer + m_offset + padding;
            m_offset += padding + size;
            return ptr;
        }

        // 2. تخصيص كائن وبناؤه (Object Allocation)
        template<typename T, typename... Args>
        T* Create(Args&&... args) {
            void* mem = Allocate(sizeof(T), alignof(T));
            if (!mem) return nullptr;
            
            // استخدام Placement New لاستدعاء Constructor الكائن في تلك المساحة
            return new (mem) T(std::forward<Args>(args)...);
        }

        // إعادة ضبط الـ Arena
        void Reset() {
            m_offset = 0;
            // ملاحظة: هنا لا يتم استدعاء الـ Destructors للكائنات
            // الـ Arena مصممة للبيانات البسيطة (POD) أو التي لا تملك موارد خارجية
        }

        // استعلامات مفيدة للمحرك
        size_t GetUsedMemory() const { return m_offset; }
        size_t GetTotalSize() const { return m_size; }
        size_t GetFreeSpace() const { return m_size - m_offset; }

    private:
        uint8_t* m_buffer;
        size_t m_size;
        size_t m_offset;

        // منع النسخ لضمان سلامة الذاكرة
        ArenaAllocator(const ArenaAllocator&) = delete;
        ArenaAllocator& operator=(const ArenaAllocator&) = delete;
    };

}