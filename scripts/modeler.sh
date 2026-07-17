#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

print_help() {
  cat <<'EOF'
Modeler developer helper

Usage:
  ./scripts/modeler.sh <command> [--release] [--vcpkg]

Commands:
  setup    Configure CMake
  build    Build the project
  test     Run unit tests
  run      Run the app executable
  docs     Generate Doxygen docs
  clean    Remove generated build directories
  help     Show this help

Options:
  --release   Use release preset (default: debug)
  --vcpkg     Use vcpkg-based presets (requires VCPKG_ROOT)

Examples:
  ./scripts/modeler.sh setup
  ./scripts/modeler.sh build
  ./scripts/modeler.sh test
  ./scripts/modeler.sh run
  ./scripts/modeler.sh setup --vcpkg --release
EOF
}

require_cmd() {
  if ! command -v "$1" >/dev/null 2>&1; then
    echo "Missing required command: $1" >&2
    exit 1
  fi
}

ensure_vcpkg_env() {
  if [[ -z "${VCPKG_ROOT:-}" ]]; then
    echo "VCPKG_ROOT is not set. Export it first, for example:" >&2
    echo "  export VCPKG_ROOT=\$HOME/vcpkg" >&2
    exit 1
  fi
  if [[ ! -f "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" ]]; then
    echo "vcpkg toolchain not found at ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" >&2
    exit 1
  fi
}

ensure_build_tools() {
  require_cmd cmake
  require_cmd ninja

  if ! command -v c++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1 && ! command -v g++ >/dev/null 2>&1; then
    echo "No C++ compiler found (expected one of: c++, g++, clang++)." >&2
    echo "Install build tools first (Ubuntu example):" >&2
    echo "  sudo apt-get update && sudo apt-get install -y build-essential ninja-build" >&2
    exit 1
  fi
}

build_preset_name() {
  local mode="$1"
  local use_vcpkg="$2"
  local preset

  if [[ "$mode" == "release" ]]; then
    preset="release"
  else
    preset="dev"
  fi

  if [[ "$use_vcpkg" == "true" ]]; then
    preset="${preset}-vcpkg"
  fi

  echo "$preset"
}

COMMAND="${1:-help}"
shift || true

MODE="debug"
USE_VCPKG="false"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --release)
      MODE="release"
      ;;
    --vcpkg)
      USE_VCPKG="true"
      ;;
    -h|--help|help)
      print_help
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      print_help
      exit 1
      ;;
  esac
  shift
done

PRESET="$(build_preset_name "$MODE" "$USE_VCPKG")"

if [[ "$USE_VCPKG" == "true" ]]; then
  ensure_vcpkg_env
fi

case "$COMMAND" in
  setup)
    ensure_build_tools
    cmake --preset "$PRESET"
    ;;
  build)
    ensure_build_tools
    cmake --preset "$PRESET"
    cmake --build --preset "$PRESET"
    ;;
  test)
    ensure_build_tools
    cmake --preset "$PRESET"
    cmake --build --preset "$PRESET"
    ctest --preset "$PRESET"
    ;;
  run)
    ensure_build_tools
    cmake --preset "$PRESET"
    cmake --build --preset "$PRESET"

    BIN_PATH="${ROOT_DIR}/build/${PRESET}/Modeler"
    if [[ -f "${BIN_PATH}.exe" ]]; then
      BIN_PATH="${BIN_PATH}.exe"
    fi

    if [[ ! -f "$BIN_PATH" ]]; then
      echo "Executable not found at $BIN_PATH" >&2
      exit 1
    fi

    "$BIN_PATH"
    ;;
  docs)
    require_cmd doxygen
    (
      cd "$ROOT_DIR"
      doxygen docs/Doxyfile
    )
    echo "Documentation generated at docs/html/index.html"
    ;;
  clean)
    rm -rf "${ROOT_DIR}/build/dev" \
           "${ROOT_DIR}/build/release" \
           "${ROOT_DIR}/build/dev-vcpkg" \
           "${ROOT_DIR}/build/release-vcpkg"
    echo "Removed generated build directories."
    ;;
  help)
    print_help
    ;;
  *)
    echo "Unknown command: $COMMAND" >&2
    print_help
    exit 1
    ;;
esac
