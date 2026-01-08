# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a C++ learning project (`cpp-playground`) organized into:
- **cpp_notes/**: Tutorial code organized by topic (basics, algorithms)
- **qt_demo/**: Qt-based demo applications (calculator, note_book)
- **mystl/**: Custom STL implementation (vector, string, smart pointers, etc.)
- **tool/ok-cpp/**: Submodule - lightweight C++ project management tool
- **docs/**: Documentation (git, cmake workflows)

## Build System

This project uses CMake with hierarchical organization. Each sub-project has its own `CMakeLists.txt`.

### Building Individual Projects

Each tutorial subfolder is self-contained with its own CMakeLists.txt. Build from within the subdirectory:

```bash
# Example: Build the static keyword tutorial
cd cpp_notes/01_basics/02_keywords/static
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run the executable
./build/static
```

### Build Output

Projects use `set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)` to place binaries in a `build/` folder within each project directory.

### Using ok-cpp Tool

The `tool/ok-cpp/` submodule provides a project management utility. Install and use:

```bash
cd tool/ok-cpp
chmod +x ./install.sh
sudo ./install.sh

# Create a new project (generates main.cpp, CMakeLists.txt, build/)
ok-cpp mkp 03_AVL_tree

# Run any project from root
ok-cpp run cpp_notes/01_basics/02_keywords/static
ok-cpp run --debug cpp_notes/01_basics/02_keywords/static
```

**Important**: Always use `ok-cpp mkp projectname` to create new tutorial projects.

## Code Architecture

### cpp_notes/ Structure

```
cpp_notes/
├── 01_basics/           # Basic C++ concepts
│   ├── 01_ptr_ref/      # Pointers, references, move semantics
│   ├── 02_keywords/     # C++ keywords (const, static, etc.)
│   └── 03_string/       # String handling (C strings, std::string)
└── algorithm/           # Algorithm implementations
    ├── 02_tree/         # Tree structures (RB tree)
    └── 03_string_match/ # String matching algorithms (KMP)
```

Each tutorial subdirectory contains:
- `main.cpp` - Example code demonstrating the concept
- `CMakeLists.txt` - Build configuration
- `build/` - Build artifacts (not tracked in git)

### Adding New Tutorials

When adding new tutorial content:

1. Create subdirectory under appropriate category (e.g., `cpp_notes/01_basics/04_new_topic/`)
2. Add a `CMakeLists.txt` following the template pattern from existing tutorials
3. Add the subdirectory to the parent's `CMakeLists.txt` via `add_subdirectory()`
4. Write `main.cpp` with examples following the established style:
   - Use numbered test functions (`test01_*`, `test02_*`, etc.)
   - Include section separators with `//` comment blocks
   - Add summary comments at the end

### mystl/ Structure

Custom STL implementation with header-only library:
- `vector.h`, `deque.h`, `array.h` - Container implementations
- `basic_string.h` - String class
- `shared_ptr.h`, `unique_ptr.h` - Smart pointers
- `algorithm.h` - Algorithm implementations

No CMakeLists.txt - header-only library meant for inclusion.

### qt_demo/ Structure

Qt applications with their own CMake configuration in each subfolder.

## Git Workflow

### Submodules

This repository contains `tool/ok-cpp` as a git submodule. To update:

```bash
# Update submodule to latest
git submodule update --remote

# Commit the submodule reference update
git add tool/ok-cpp
git commit -m "chore: update ok-cpp submodule"
```

### Branches

- `dev` - Development branch (primary)
- `main` - Stable branch
- `doc/cmake` - Documentation branch
- `note/basic` - Notes branch

When merging or rebasing, ensure all branches stay synchronized.

## CMake Template Pattern

Each tutorial's `CMakeLists.txt` follows this pattern:

```cmake
cmake_minimum_required(VERSION 3.20)

project(project_name)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    add_compile_definitions(DEBUG_MODE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -pedantic)
    endif()
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Source files
set(SOURCE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})
```

## Include Paths

Some projects (like RB tree) include headers from the project root:
```cmake
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../)
```

This allows including headers from `cpp_notes/` level. Path is relative to the subdirectory.
