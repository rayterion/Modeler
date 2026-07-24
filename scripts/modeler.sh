#!/usr/bin/env bash
set -euo pipefail

cat <<'EOF'
Modeler is Windows-focused.

Use the PowerShell helper script:
  .\scripts\modeler.ps1 build
  .\scripts\modeler.ps1 test
  .\scripts\modeler.ps1 run

See README.md for full Windows setup instructions.
EOF

exit 1
