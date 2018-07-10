#!/bin/bash

PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
cd "${PROJECT_DIR}"

CMAKE_CACHE_DIR="${PROJECT_DIR}/cmake-build-debug"
mkdir -p "${CMAKE_CACHE_DIR}"
pushd "${CMAKE_CACHE_DIR}"
cmake ..
make
popd
"${CMAKE_CACHE_DIR}/WILLBO"
