#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <new>

namespace VryEngine {

    class PoolAllocator {
    private:
        // هيكل الفتحة الفارغة - نستخدم الذاكرة نفسها لتخزين المؤشر للفتحة التالية
        struct Node {
            Node* next;
        };

    public:
        // sizePerBlock: حجم الكائن الواحد، numBlocks: عدد الكائنات
        PoolAllocator(size_t blockScalarSize, size_t numBlocks, size_t alignment = alignof(std::max_align_t))
            : m_numBlocks(numBlocks), m_usedMemory(0) {
            
            // التأكد من أن حجم البلوك يكفي لتخزين عنوان المؤشر (Node) على الأقل
            m_blockSize = (blockScalarSize > sizeof(Node)) ? blockScalarSize : sizeof(Node);
            
            // حجز الذاكرة الكلية
            m_totalSize = m_blockSize * m_numBlocks;
            m_buffer = static_cast<uint8_t*>(std::malloc(m_totalSize));

            // إعداد القائمة المتصلة للفتحات الفارغة (Free List)
            Reset();
        }

        ~PoolAllocator() {
            std::free(m_buffer);
        }

        // تخصيص فتحة واحدة: O(1)
        void* Allocate() {
            if (m_freeList == nullptr) return nullptr; // الـ Pool ممتلئ

            Node* slot = m_freeList;
            m_freeList = m_freeList->next;
            m_usedMemory += m_blockSize;
            return reinterpret_cast<void*>(slot);
        }

        // تحرير فتحة معينة: O(1)
        void Free(void* ptr) {
            if (!ptr) return;

            // إعادة الفتحة إلى بداية القائمة المتصلة
            Node* node = static_cast<Node*>(ptr);
            node->next = m_freeList;
            m_freeList = node;
            m_usedMemory -= m_blockSize;
        }

        // إعادة ضبط الـ Pool بالكامل
        void Reset() {
            m_usedMemory = 0;
            m_freeList = reinterpret_cast<Node*>(m_buffer);
            
            // ربط كل الفتحات ببعضها في البداية
            Node* current = m_freeList;
            for (size_t i = 0; i < m_numBlocks - 1; ++i) {
                uint8_t* nextAddress = reinterpret_cast<uint8_t*>(current) + m_blockSize;
                current->next = reinterpret_cast<Node*>(nextAddress);
                current = current->next;
            }
            current->next = nullptr; // آخر فتحة تشير لـ null
        }

        // استعلامات
        size_t GetUsedMemory() const { return m_usedMemory; }
        size_t GetBlockSize() const { return m_blockSize; }

    private:
        size_t m_blockSize;
        size_t m_numBlocks;
        size_t m_totalSize;
        size_t m_usedMemory;
        
        uint8_t* m_buffer;
        Node* m_freeList; // رأس القائمة التي تحتوي الفتحات الفارغة

        // منع النسخ
        PoolAllocator(const PoolAllocator&) = delete;
        PoolAllocator& operator=(const PoolAllocator&) = delete;
    };

}