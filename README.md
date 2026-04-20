# VryEngine 🚀

**VryEngine** is a high-performance, cross-platform game engine built from scratch. It emphasizes low-level hardware control, custom abstraction layers, and a unique architectural approach inspired by the human brain.

## 🧠 Brain-Based Architecture
The engine is logically partitioned into "Lobes" to maintain a clean separation of concerns:
- **VryTemporal_Lobe:** Core systems, memory management, and logging.
- **VryParietal_Lobe:** Sensory processing (Event System, Input handling).
- **VryOccipital_Lobe:** Vision and Graphics (RHI, Metal/DirectX Backends).
- **VrySkin:** The interface layer (Windowing system via GLFW).

## 🛠 Current Features
- **Custom RHI (Render Hardware Interface):** An abstraction layer designed to support multiple graphics APIs.
- **Native Metal Backend:** Optimized specifically for Apple Silicon (M4) using Objective-C++ and Metal framework.
- **Robust Event System:** A decoupled, dispatcher-based system for handling window and input events.
- **Native Performance:** Zero-overhead design with minimal external dependencies.

## 🚀 Getting Started

### Prerequisites
- **OS:** macOS (Apple Silicon M-Series recommended).
- **Compiler:** Clang with Objective-C++ support.
- **Build System:** CMake 3.10+.
- **Dependencies:** GLFW3 (Installable via `brew install glfw`).

### Build and Run
```bash
# Clone the repository
git clone [https://github.com/yourusername/VryEngine.git](https://github.com/yourusername/VryEngine.git)
cd VryEngine

# Build the project
mkdir build && cd build
cmake ..
make

# Run the application
./VryApp
