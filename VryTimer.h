#pragma once
#include <chrono>

namespace VryEngine {

    class Timer {
    public:
        Timer() {
            Reset();
        }

        // إعادة ضبط المؤقت للبدء من الصفر
        void Reset() {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        // الحصول على الوقت المنقضي بالثواني (مفيد للـ Delta Time)
        float Elapsed() {
            return std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - m_Start).count() * 0.000001f;
        }

        // الحصول على الوقت بالملي ثانية (مفيد للـ Profiling)
        float ElapsedMillis() {
            return Elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

}