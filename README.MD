# Aswin2D Game Engine

A modern, cross-platform 2D game engine built with C++20, featuring an integrated editor and automated dependency management.

> **⚠️ Active Development Notice:** This engine is currently under active development. Features and APIs may change as we work towards our first stable release.

## 🚀 Features

-   **Cross-Platform**: Build on Windows, Linux, and macOS with a single command
-   **Modern C++20**: Entity Component System (ECS) architecture with EnTT
-   **Integrated Editor**: Full-featured game development environment
-   **Automated Dependencies**: Zero-configuration setup with CMake FetchContent
-   **Professional Tooling**: Code formatting, documentation, and testing infrastructure
-   **High Performance**: Optimized compilation with forward declarations

## 🛠️ Quick Start

### Prerequisites

-   CMake 3.20 or higher
-   C++20 compatible compiler (MSVC 2019+, GCC 10+, Clang 12+)

#### **IDE Integration**

You can also open the project directly in your favorite IDE:

-   **🆚 Visual Studio** - Open folder or use `CMakeSettings.json` for seamless integration
-   **📝 Visual Studio Code** - Open folder with CMake Tools extension for full IntelliSense
-   **🔧 CLion** - Direct CMake project import with debugging support
-   **⚙️ Qt Creator** - Import CMake project for cross-platform development

The CMake configuration automatically handles include paths, dependencies, and build settings for any CMake-compatible

That's it! All dependencies are automatically downloaded and configured.

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/Aswin-programmer/2dEngine2.git
cd 2dEngine2

# Build (one command - all dependencies auto-downloaded)
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Run
cd ./build/bin/MSVC/Release
./Aswin2DEditor.exe

```

#### **Build Output Structure**

```
2dEngine2/build/
├── .cmake/                     # CMake configuration cache
├── bin/                        # Executable output directory
│   └── MSVC/                   # Compiler-specific folder (Windows)
│       └── Release/            # Build configuration
│           ├── Aswin2DEngine.exe    # Main engine executable
│           ├── fmod.dll             # FMOD audio libraries
│           ├── fmodstudio.dll       # FMOD studio libraries
│           └── OpenAL32.dll         # OpenAL audio library
├── lib/                        # Static library output
│   └── MSVC/Release/
│       └── Aswin2DEngine.lib   # Engine static library
├── CMakeFiles/                 # CMake build files
├── EDITOR/                     # Editor build artifacts
├── GAMEENGINE/                 # Engine build artifacts
└── _deps/                      # Auto-downloaded dependencies
    ├── box2d-*/                     # Box2D physics
    ├── entt-*/                      # EnTT ECS library
    ├── glm-*/                       # GLM math library
    ├── sol2-*/                      # sol2 Lua binding
    └── ...-*/                       # etc
```

## 📦 Automated Dependencies

The engine automatically manages these libraries via CMake FetchContent:

-   **Box2D** - Physics simulation
-   **EnTT** - Entity Component System
-   **GLEW** - OpenGL extension loading
-   **GLFW** - Window and input management
-   **GLM** - Mathematics library
-   **ImGui** - Immediate mode GUI
-   **Lua** - Scripting engine
-   **MiniAudio** - Audio processing
-   **OpenAL** - 3D audio API
-   **RapidJSON** - JSON parsing
-   **libsndfile** - Audio file I/O
-   **Sol2** - Lua C++ binding
-   **STB** - Image loading utilities
-   **TinyFileDialogs** - Native file dialogs

## 🏗️ Project Structure

```
2dEngine2/
├── .github/                        # CI/CD Pipeline
│   └── workflows/                  # GitHub Actions workflows
├── build/                          # CMake build directory (auto-generated)
│   ├── _deps/                      # Auto-downloaded dependencies
│   └── GAMEENGINE/                 # Engine build artifacts
├── cmake/                          # CMake configuration files
├── EDITOR/                         # Game Editor Application
│   ├── cmake/                      # Editor CMake configuration
│   ├── assets/                     # Editor-specific assets
│   │   ├── FONTS/                  # Font resources
│   │   ├── SCRIPTS/                # Lua scripts and game logic
│   │   ├── SPACESHOOTER/           # Demo game assets
│   │   ├── TEXTURES/               # Image and sprite assets
│   │   └── TILEDMAP/               # Tile-based level assets
│   ├── include/                    # Editor header files
│   │   ├── DISPLAYS/               # UI display components
│   │   ├── SCENE/                  # Scene management
│   │   ├── SYSTEMS/                # Editor-specific systems
│   │   └── UTILITIES/              # Editor utility functions
│   └── src/                        # Editor source implementation
├── GAMEENGINE/                     # Core Game Engine Library
│   ├── cmake/                      # Engine CMake configuration
│   ├── libs/                       # External libraries
│   │   └── FMOD/                   # 🎵 FMOD Audio Engine (cross-platform)
│   │       ├── bin/                # 🪟 Windows DLLs (fmod.dll, fmodstudio.dll)
│   │       ├── include/            # 📄 FMOD header files (.h, .hpp)
│   │       └── lib/                # 📚 Static libraries and Linux .so files
│   │           └── linux_x86_64/   # 🐧 Linux shared libraries
│   ├── resources/                  # Engine default resources
│   ├── include/                    # Public engine headers
│   │   ├── ECS/                    # Entity Component System
│   │   │   └── COMPONENTS/         # Game components (Transform, Sprite, etc.)
│   │   ├── RENDERER/               # Graphics rendering system
│   │   │   ├── BUFFERS/            # OpenGL buffer management
│   │   │   ├── SHADER/             # Shader compilation and management
│   │   │   └── TEXTURE/            # Texture loading and binding
│   │   ├── PHYSICS/                # Box2D physics integration
│   │   ├── SCRIPTING/              # Lua scripting support
│   │   ├── SOUNDS/                 # Audio system (OpenAL)
│   │   ├── INPUT/                  # Input handling (keyboard, mouse)
│   │   └── UTILITIES/              # Core utility functions
│   ├── src/                        # Engine source implementation
│   ├── benchmark/                  # Benchmark (Not yet implemented)
│   └── test/                       # Unit tests (Not yet implemented)
└── licenses/                       # Third-party license files
```

## 🎮 Engine Architecture

### Entity Component System (ECS)

Built on EnTT for high-performance entity management:

-   **Entities**: Game objects as lightweight IDs
-   **Components**: Data containers (Transform, Sprite, Physics, etc.)
-   **Systems**: Logic processors (Rendering, Physics, Input, etc.)

### Modular Design

-   **GAMEENGINE**: Core functionality (rendering, physics, audio, scripting)
-   **EDITOR**: Development tools and asset management
-   **Clean separation**: Engine can be used standalone or with editor

### Cross-Platform Rendering

-   OpenGL-based renderer with modern pipeline
-   Batch rendering for optimal performance
-   Shader management system
-   Texture atlas support

## 🔧 Development

### Code Formatting

The project uses `.clang-format` for consistent code style.

### Testing

-   **Not yet implemented**

### Documentation

-   **Not yet implemented**

### TODO List

-   **🏗️ Namespace Implementation** - Add comprehensive `Aswin2D::*` namespace structure for the Game Engine library
-   **📖 Automated API Documentation** - Implement Doxygen generation with GitHub Pages deployment
-   **🧪 Unit Testing Framework** - Integrate automated testing with CTest and CI pipeline
-   **📦 Standalone Engine Package** - Create distributable library with clean public API
-   **⚙️ CI/CD Pipeline** - Set up GitHub Actions for automated builds, tests, and releases
-   **🔀 Multithreading Architecture** - Implement dedicated rendering thread and thread pools for game logic
-   **🚀 Vulkan API Support** - Consider implementing Vulkan renderer alongside OpenGL for modern graphics performance

## 🎯 Supported Platforms

| Platform | Compiler   | Status             |
| -------- | ---------- | ------------------ |
| Windows  | MSVC 2019+ | ✅ Fully Supported |
| Windows  | MinGW/GCC  | 🔧 To be tested    |
| Linux    | GCC 10+    | 🔧 To be tested    |
| Linux    | Clang 12+  | 🔧 To be tested    |

## 📝 License

This project is licensed under the [MIT License](LICENSE) - see the LICENSE file for details.

## 🙏 Acknowledgments

Special thanks to the **Scion2D project** and its creator **dwjclark11** for inspiration and architectural insights that helped shape Aswin2D's development.

**🔗 [Scion2D on GitHub](https://github.com/dwjclark11/Scion2D)**

_Scion2D's elegant approach to 2D game engine architecture and ECS implementation provided valuable guidance during Aswin2D's design phase._

### Open Source Dependencies

This project builds upon the incredible work of these open-source libraries:

-   **[EnTT](https://github.com/skypjack/entt)** - Fast and reliable Entity Component System
-   **[GLFW](https://github.com/glfw/glfw)** - Multi-platform library for OpenGL contexts
-   **[Dear ImGui](https://github.com/ocornut/imgui)** - Immediate mode GUI for C++
-   **[Box2D](https://github.com/erincatto/box2d)** - 2D physics engine for games
-   **[OpenAL Soft](https://github.com/kcat/openal-soft)** - Cross-platform 3D audio API
-   **[GLM](https://github.com/g-truc/glm)** - Mathematics library for graphics software
-   **[RapidJSON](https://github.com/Tencent/rapidjson)** - Fast JSON parser and generator
-   **[sol2](https://github.com/ThePhD/sol2)** - C++ Lua binding library
-   **[stb](https://github.com/nothings/stb)** - Single-file public domain libraries
-   **[libsndfile](https://github.com/libsndfile/libsndfile)** - Audio file reading/writing
-   **[miniaudio](https://github.com/mackron/miniaudio)** - Single file audio playback library
-   **[Lua](https://www.lua.org/)** - Powerful, efficient scripting language

_Without these amazing projects, Aswin2D would not be possible. Thank you to all contributors and maintainers!_ ❤️
