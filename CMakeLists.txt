cmake_minimum_required(VERSION 3.10)
project(VryEngine VERSION 1.0 LANGUAGES CXX OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OBJCXX_STANDARD 17)

# تفعيل الـ ARC للماك
add_compile_options("$<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>")
add_definitions(-DGLFW_EXPOSE_NATIVE_COCOA)

# تجميع الملفات
file(GLOB_RECURSE ENGINE_SOURCES 
    "${CMAKE_SOURCE_DIR}/VrySkin/*.cpp" 
    "${CMAKE_SOURCE_DIR}/VryBrain/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/VryBrain/**/*.mm" 
)

add_library(VryEngine_Lib STATIC ${ENGINE_SOURCES})

# --- الحل السحري: تسجيل كل المسارات في الـ Global Include ---
target_include_directories(VryEngine_Lib PUBLIC 
    "${CMAKE_SOURCE_DIR}/VrySkin"
    "${CMAKE_SOURCE_DIR}/VryBrain/VryTemporal_Lobe"
    "${CMAKE_SOURCE_DIR}/VryBrain/VryParietal_Lobe"
    "${CMAKE_SOURCE_DIR}/VryBrain/VryOccipital_Lobe"
    "${CMAKE_SOURCE_DIR}"
    /opt/homebrew/include
)

add_executable(VryApp main.cpp)
target_link_libraries(VryApp VryEngine_Lib "/opt/homebrew/lib/libglfw.3.dylib" "-framework Cocoa" "-framework Metal" "-framework QuartzCore" "-framework IOKit" "-framework CoreVideo" "-framework AppKit")

set_target_properties(VryApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")