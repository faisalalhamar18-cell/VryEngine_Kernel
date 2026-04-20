#pragma once
#include <VryMemory1.h>
#include <string>
#include <iostream>
#include <cstdarg> // للتعامل مع الـ Formatted strings مثل printf

namespace VryEngine {

    enum class LogLevel {
        Trace, Info, Warning, Error, Fatal
    };

    class Logger {
    public:
        // تهيئة النظام وتخصيص Arena خاصة للـ Logs
        static void Init(size_t poolSize = 1024 * 1024) { // 1MB افتراضي
            GetLogArena() = new ArenaAllocator(poolSize);
        }

        static void Shutdown() {
            delete GetLogArena();
        }

        // الدالة الأساسية لمعالجة الرسائل
        static void LogMessage(LogLevel level, const char* format, ...) {
            // 1. تحديد علامة المستوى (Tag)
            const char* levelStrings[] = { "[TRACE]: ", "[INFO]:  ", "[WARN]:  ", "[ERROR]: ", "[FATAL]: " };
            
            // 2. استخدام الـ Arena لتخصيص مساحة مؤقتة للرسالة
            char* buffer = (char*)GetLogArena()->Allocate(2048); // حجز 2KB للرسالة الواحدة
            if (!buffer) return;

            // 3. معالجة النصوص المتغيرة (Variadic Arguments)
            va_list args;
            va_start(args, format);
            vsnprintf(buffer, 2048, format, args);
            va_end(args);

            // 4. الطباعة (يمكنك لاحقاً توجيهها لملف أو للـ Console)
            std::cout << levelStrings[(int)level] << buffer << std::endl;

            // 5. في الـ Log، غالباً ما نقوم بعمل Reset للـ Arena بعد كل إطار أو عند الامتلاء
            if (GetLogArena()->GetUsedMemory() > 1024 * 512) { // إذا تجاوزت نصف ميجا
                GetLogArena()->Reset();
            }
        }

    private:
        // نستخدم Static لتسهيل الوصول إليها من أي مكان في المحرك
        static ArenaAllocator*& GetLogArena() {
            static ArenaAllocator* s_LogArena = nullptr;
            return s_LogArena;
        }
    };

}

// --- ماكروز الاستخدام (Core Macros) ---
#define VRY_LOG_INFO(format, ...)  VryEngine::Logger::LogMessage(VryEngine::LogLevel::Info, format, ##__VA_ARGS__)
#define VRY_LOG_WARN(format, ...)  VryEngine::Logger::LogMessage(VryEngine::LogLevel::Warning, format, ##__VA_ARGS__)
#define VRY_LOG_ERROR(format, ...) VryEngine::Logger::LogMessage(VryEngine::LogLevel::Error, format, ##__VA_ARGS__)