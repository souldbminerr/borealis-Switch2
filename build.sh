#!/bin/sh
set -e

if [ -z "$DEVKITPRO" ]; then
  echo "error: DEVKITPRO not set (install devkitPro + libounce/ounce-devkit and set DEVKITPRO)" >&2
  exit 1
fi

# Sets up the Switch 2/libounce cross-toolchain environment
if [ -f "$DEVKITPRO/switch2vars.sh" ]; then
  # shellcheck disable=SC1091
  . "$DEVKITPRO/switch2vars.sh"
else
  echo "warning: $DEVKITPRO/switch2vars.sh not found; continuing with current env" >&2
fi

aarch64-none-elf-cmake -S . -B build_switch2 \
  -DPLATFORM_SWITCH2=ON \
  -DUSE_DRAKO3D=ON \
  -DCMAKE_BUILD_TYPE=Release "$@"

cmake --build build_switch2 -j8