# Modeler Setup Guide (Windows)

This guide explains how to verify every prerequisite, install missing tools correctly, and build/run the project with dependencies kept inside the repository.

## Scope

- Target OS: Windows 10/11
- Build toolchain: Visual Studio 2022 Build Tools (Desktop C++)
- Dependency manager: repo-local vcpkg (`./vcpkg`)
- Shell: PowerShell

## 1) Check Prerequisites

Run these commands from PowerShell.

```powershell
# Core tools
cmake --version
git --version

# Visual Studio C++ toolchain checks
where.exe cl
where.exe msbuild

# Optional docs tool
doxygen --version

# Python virtual environment check (for repo-isolated command sessions)
python --version
Test-Path .\.venv\Scripts\Activate.ps1
```

Expected results:
- `cmake --version` prints 3.25+.
- `git --version` prints a valid Git version.
- `where.exe cl` and `where.exe msbuild` return at least one path.
- `doxygen --version` may fail if docs are not needed.
- `Test-Path .\.venv\Scripts\Activate.ps1` returns `True` if `.venv` already exists.

## 2) Install Missing Prerequisites

Use the sections below only for missing tools.

### 2.1 Install Git

```powershell
winget install --id Git.Git -e --source winget
```

### 2.2 Install CMake

```powershell
winget install --id Kitware.CMake -e --source winget
```

### 2.3 Install Visual Studio 2022 Build Tools (Desktop C++)

```powershell
winget install --id Microsoft.VisualStudio.2022.BuildTools -e --source winget --override "--wait --quiet --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

After install, reopen PowerShell and re-run:

```powershell
where.exe cl
where.exe msbuild
```

### 2.4 Install Doxygen (optional, docs only)

```powershell
winget install --id DimitriVanHeesch.Doxygen -e --source winget
```

### 2.5 Install Python and create local virtual environment (recommended)

If Python is missing:

```powershell
winget install --id Python.Python.3.12 -e --source winget
```

From the project root:

```powershell
python -m venv .venv
.\.venv\Scripts\Activate.ps1
```

## 3) Project Setup (Repo-Local vcpkg)

Run everything from the project root.

```powershell
# activate repo-local venv session first
.\.venv\Scripts\Activate.ps1

# clone vcpkg inside this repository
if (-not (Test-Path .\vcpkg)) {
  git clone https://github.com/microsoft/vcpkg.git vcpkg
}

# bootstrap local vcpkg executable
.\vcpkg\bootstrap-vcpkg.bat
```

## 4) Build, Test, and Run

Using the helper script:

```powershell
.\.venv\Scripts\Activate.ps1

# configure only
.\scripts\modeler.ps1 setup

# build debug
.\scripts\modeler.ps1 build

# run unit tests
.\scripts\modeler.ps1 test -Configuration Debug

# run app
.\scripts\modeler.ps1 run
```

Release mode:

```powershell
.\.venv\Scripts\Activate.ps1
.\scripts\modeler.ps1 build -Configuration Release
.\scripts\modeler.ps1 test -Configuration Release
```

Without helper script (explicit CMake):

```powershell
.\.venv\Scripts\Activate.ps1

cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE="${PWD}\vcpkg\scripts\buildsystems\vcpkg.cmake" -DMODELER_BUILD_TESTS=ON
cmake --build build --config Debug --parallel
ctest --test-dir build --build-config Debug --output-on-failure
```

## 5) Validation Checklist

Before opening a PR, confirm:

```powershell
cmake --version
git --version
where.exe cl
where.exe msbuild
Test-Path .\vcpkg\scripts\buildsystems\vcpkg.cmake
.\scripts\modeler.ps1 help
```

Optional quality checks:

```powershell
clang-format --version
clang-tidy --version
cppcheck --version
```

## 6) Troubleshooting

- Error: `Local vcpkg toolchain file not found at ...\vcpkg\scripts\buildsystems\vcpkg.cmake`
  - Cause: vcpkg not cloned/bootstrapped in the repo.
  - Fix:

```powershell
git clone https://github.com/microsoft/vcpkg.git vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

- Error: `No CMAKE_CXX_COMPILER could be found` or `Visual Studio instance not found`
  - Cause: C++ build tools missing or shell not refreshed.
  - Fix: install Visual Studio Build Tools (Desktop C++), then open a new PowerShell window.

- Error during tests: `No tests were found`
  - Cause: configure/build failed before tests ran.
  - Fix: resolve the first CMake error, then rerun build and test commands.

- PowerShell policy blocks activation script
  - Fix for current session only:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy RemoteSigned
.\.venv\Scripts\Activate.ps1
```
