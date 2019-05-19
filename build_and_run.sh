#!/bin/bash

PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd  )"
cd "${PROJECT_DIR}"
./scripts/build.sh
./cmake-build-debug/WILLBO
