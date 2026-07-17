# Modeler

> A cross-platform 3D CAD modeller for architects and 3D artists, with a planned AI-assisted scene builder.

[![CI](https://github.com/rayterion/Modeler/actions/workflows/ci.yml/badge.svg)](https://github.com/rayterion/Modeler/actions/workflows/ci.yml)
[![Docs](https://github.com/rayterion/Modeler/actions/workflows/docs.yml/badge.svg)](https://rayterion.github.io/Modeler)

## Features

- Custom wxWidgets-based GUI with a menu bar (FILE / MODIFY / WINDOW / HELP)
- INI-driven user preferences (`bin/preferences.ini`)
- Project file persistence (`.model` format)
- DWG / DXF import pipeline (planned)
- AI-assisted scene builder (planned)

## Building

### Prerequisites

| Tool | Minimum version |
|------|----------------|
| CMake | 3.25 |
| Ninja | latest |
| A C++17 compiler | GCC 11 / Clang 14 / MSVC 2022 |
| wxWidgets | 3.2 |
| vcpkg *(optional)* | latest |

Ubuntu quick install for build tools:

```bash
sudo apt-get update
sudo apt-get install -y build-essential ninja-build
```

### Quick start

```bash
# 1. Clone and enter the repo
git clone https://github.com/rayterion/Modeler.git && cd Modeler

# 2. Configure + build (Debug, system dependencies)
./scripts/modeler.sh build

# 3. Run
./scripts/modeler.sh run
```

### Common developer commands

```bash
./scripts/modeler.sh setup                # configure debug build
./scripts/modeler.sh build                # configure + build debug
./scripts/modeler.sh test                 # configure + build + run tests
./scripts/modeler.sh run                  # build + run app
./scripts/modeler.sh docs                 # generate docs/html/index.html
./scripts/modeler.sh clean                # remove generated build dirs
```

Release builds:

```bash
./scripts/modeler.sh build --release
./scripts/modeler.sh test --release
```

Using vcpkg dependency resolution:

```bash
export VCPKG_ROOT=$HOME/vcpkg
./scripts/modeler.sh build --vcpkg
./scripts/modeler.sh test --vcpkg
```

### CMake presets (without helper script)

```bash
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

Available configure presets: `dev`, `release`, `dev-vcpkg`, `release-vcpkg`.

### Setup error troubleshooting

- `Could not find toolchain file: /scripts/buildsystems/vcpkg.cmake`
  - `VCPKG_ROOT` was not set before running CMake.
  - Fix: `export VCPKG_ROOT=/absolute/path/to/vcpkg` and use `--vcpkg` commands.
- `CMAKE_MAKE_PROGRAM is not set` / `CMAKE_CXX_COMPILER not set`
  - Required build tools are missing.
  - Fix (Ubuntu): `sudo apt-get install -y build-essential ninja-build`

## Project structure

```
Modeler/
├── include/modeler/        # Public C++ interfaces (no wxWidgets dependency)
│   ├── config/             #   AppConfig — INI reader/writer
│   ├── core/               #   IRenderer, ICommand, CommandStack
│   └── ui/                 #   MenuPanel enum
├── src/                    # Implementations
│   ├── config/
│   ├── core/
│   └── ui/
│       └── menu_buttons/
├── tests/                  # GoogleTest unit + integration tests
│   ├── mocks/
│   └── unit/
├── docs/                   # Doxygen configuration
├── cmake/                  # CMake helper modules
└── bin/                    # Runtime assets (preferences.ini, project files)
```

## Architecture overview

| Layer | Location | wx dependency |
|-------|----------|--------------|
| Domain interfaces | `include/modeler/core/` | **None** — fully unit-testable |
| Configuration | `include/modeler/config/` | **None** |
| UI presentation | `src/ui/` | Yes — wxWidgets |

Key interfaces:
- **`IRenderer`** — decouples the menu/dialog layer from the concrete renderer
- **`AppConfig`** — pure `std::filesystem` INI I/O, no GUI dependency
- **`ICommand` / `CommandStack`** — Command pattern for undo/redo

## Contributing

1. Run `clang-format -i` on all changed files before committing.
2. All new logic in `include/modeler/` must have a corresponding unit test.
3. CI enforces `clang-format --dry-run --Werror` and `clang-tidy`.

## License

[TBD]
