#!/bin/bash

set -e

source /var/mmt/toolchains/common.sh

# Extract version from tag
MMT_VERSION_FULL=$(getVersionFromTag $MMT_TAG)
MMT_VERSION_MAJOR=$(getVersionMajorFromTag $MMT_TAG)
MMT_VERSION_MINOR=$(getVersionMinorFromTag $MMT_TAG)
MMT_VERSION_PATCH=$(getVersionPatchFromTag $MMT_TAG)

# Repo is expected to be mapped in /var/mmt

export PATH=$PATH:$QT_DIR/bin

cmake \
  -DMEMOTO_VERSION_MAJOR="${MMT_VERSION_MAJOR}" \
  -DMEMOTO_VERSION_MINOR="${MMT_VERSION_MINOR}" \
  -DMEMOTO_VERSION_PATCH="${MMT_VERSION_PATCH}" \
  -DMEMOTO_VERSION_FULL="${MMT_VERSION_FULL}" \
  -DCMAKE_BUILD_TYPE=Release \
  -S /var/mmt/MMT/ \
  -B $buildDir
cmake --build $buildDir --parallel

# Extracts the appimage to avoid using fuse (and priviledges associated to it)
./linuxdeployqt-continuous-x86_64.AppImage --appimage-extract

# Creates application tree following linuxdeployqt documentation
rm -rf mmt-deploy-root
mkdir mmt-deploy-root
mkdir mmt-deploy-root/usr
mkdir mmt-deploy-root/usr/bin
mkdir mmt-deploy-root/usr/lib
mkdir mmt-deploy-root/usr/share
mkdir mmt-deploy-root/usr/share/applications
mkdir -p mmt-deploy-root/usr/share/icons/hicolor/256x256/apps/

# Copies files
cp ${buildDir}/MMT mmt-deploy-root/usr/bin
cp /var/mmt/toolchains/linux/MeMoTo.desktop mmt-deploy-root/usr/share/applications/
cp /var/mmt/MMT/logo/logo.png mmt-deploy-root/usr/share/icons/hicolor/256x256/apps/MeMoTo.png

squashfs-root/AppRun mmt-deploy-root/usr/share/applications/MeMoTo.desktop -appimage

# Publishes file to MeMoTo output dir
cp MeMoTo-x86_64.AppImage "$(printenv OUTPUT_DIR)"

echo ""
echo "Output: $(printenv OUTPUT_DIR)/MeMoTo-x86_64.AppImage"

