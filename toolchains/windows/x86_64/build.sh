#!/bin/bash

set -e

source /var/mmt/toolchains/common.sh

# Extract version from tag
MMT_VERSION_FULL=$(getVersionFromTag $MMT_TAG)
MMT_VERSION_MAJOR=$(getVersionMajorFromTag $MMT_TAG)
MMT_VERSION_MINOR=$(getVersionMinorFromTag $MMT_TAG)
MMT_VERSION_PATCH=$(getVersionPatchFromTag $MMT_TAG)

# Repo is expected to be mapped in /var/mmt
export PATH=${MXE_USR_DIR}/bin:$PATH

mkdir -p $(printenv OUTPUT_DIR)/build_winx86_64
${MXE_USR_DIR}/bin/x86_64-w64-mingw32.static-cmake \
  -DMEMOTO_VERSION_MAJOR="${MMT_VERSION_MAJOR}" \
  -DMEMOTO_VERSION_MINOR="${MMT_VERSION_MINOR}" \
  -DMEMOTO_VERSION_PATCH="${MMT_VERSION_PATCH}" \
  -DMEMOTO_VERSION_FULL="${MMT_VERSION_FULL}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH=${MXE_USR_DIR}/x86_64-w64-mingw32.static \
  -S /var/mmt/MMT/ \
  -B $(printenv OUTPUT_DIR)/build_winx86_64

${MXE_USR_DIR}/bin/x86_64-w64-mingw32.static-cmake --build $(printenv OUTPUT_DIR)/build_winx86_64 --parallel
