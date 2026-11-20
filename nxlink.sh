#!/bin/sh
set -e

if [ -z "$1" ]; then
  echo "usage: $0 <switch-ip> [nro-path]" >&2
  echo "  sends an NRO to nxlink/netloader (port 28280). default: build_switch2/borealis_demo.nro" >&2
  exit 1
fi

IP="$1"
NRO="${2:-build_switch2/borealis_demo.nro}"
PORT="28280"

if [ ! -f "$NRO" ]; then
  echo "error: NRO not found: $NRO" >&2
  echo "build it first (e.g. ./build.sh) so it produces $NRO" >&2
  exit 1
fi

if ! command -v nxlink >/dev/null 2>&1; then
  echo "error: nxlink not found in PATH (install devkitPro tools / ensure nxlink is available)" >&2
  exit 1
fi

NRO_DIR="$(dirname "$NRO")"
NRO_FILE="$(basename "$NRO")"

echo "Sending $NRO to $IP:$PORT via nxlink..."
cd "$NRO_DIR"
exec nxlink -a "$IP" -p "$PORT" "$NRO_FILE"
