#!/usr/bin/env bash
set -euo pipefail

# Load .env if present and export all variables
if [ -f "${PWD}/.env" ]; then
  set -a
  # shellcheck disable=SC1090
  source "${PWD}/.env"
  set +a
else
  # Ensure tests build folder is on PYTHONPATH
  export PYTHONPATH="${PWD}/build/tests:${PYTHONPATH:-}"
fi

# Use provided python interpreter or fallback to 'python'
PYTHON="${1:-python}"
shift || true

# Run pytest (forward args)
exec "$PYTHON" -m pytest -s --verbose "$@"