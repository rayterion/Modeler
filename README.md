# Modeler

> A Windows-focused 3D CAD modeller for architects and 3D artists, with a planned AI-assisted scene builder.

[![CI](https://github.com/rayterion/Modeler/actions/workflows/ci.yml/badge.svg)](https://github.com/rayterion/Modeler/actions/workflows/ci.yml)
[![Docs](https://github.com/rayterion/Modeler/actions/workflows/docs.yml/badge.svg)](https://rayterion.github.io/Modeler)

## Features

- Custom wxWidgets-based GUI with a menu bar (FILE / MODIFY / WINDOW / HELP)
- INI-driven user preferences (`bin/preferences.ini`)
- Project file persistence (`.model` format)
- DWG / DXF import pipeline (planned)
- AI-assisted scene builder (planned)

## Documentation

Environment setup, prerequisite checks, installation steps, and build/test/run commands are documented in `SETUP.md`.

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
