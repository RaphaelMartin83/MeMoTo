#!/bin/bash

set -e

source /var/mmt/toolchains/common.sh

case "$(uname -m)" in
  x86_64)
    APPIMAGE_ARCH="x86_64"
    APPIMAGE_DEPLOY_ARCH="x86_64"
    ;;
  aarch64)
    APPIMAGE_ARCH="arm64"
    APPIMAGE_DEPLOY_ARCH="aarch64"
    ;;
  *)
    echo "Unsupported build architecture: $(uname -m)" >&2
    exit 1
    ;;
esac

# Extract version from tag
MMT_VERSION_FULL=$(getVersionFromTag "$MMT_TAG")
MMT_VERSION_MAJOR=$(getVersionMajorFromTag "$MMT_TAG")
MMT_VERSION_MINOR=$(getVersionMinorFromTag "$MMT_TAG")
MMT_VERSION_PATCH=$(getVersionPatchFromTag "$MMT_TAG")

# Repo is expected to be mapped in /var/mmt

export PATH="$PATH:$QT_DIR/bin"

cmake \
  -DMEMOTO_VERSION_MAJOR="${MMT_VERSION_MAJOR}" \
  -DMEMOTO_VERSION_MINOR="${MMT_VERSION_MINOR}" \
  -DMEMOTO_VERSION_PATCH="${MMT_VERSION_PATCH}" \
  -DMEMOTO_VERSION_FULL="${MMT_VERSION_FULL}" \
  -DCMAKE_BUILD_TYPE=Release \
  -S /var/mmt/MMT/ \
  -B "$buildDir"
cmake --build "$buildDir" --parallel

# Runs tests (disable Qt display to run the tests)
QT_QPA_PLATFORM="vnc" make -C "$buildDir" test

# Extracts the appimage to avoid using fuse (and priviledges associated to it)
/usr/local/bin/linuxdeployqt --appimage-extract

# Creates application tree following linuxdeployqt documentation
rm -rf mmt-deploy-root
mkdir -p mmt-deploy-root/usr/bin \
  mmt-deploy-root/usr/lib \
  mmt-deploy-root/usr/share/applications \
  mmt-deploy-root/usr/share/icons/hicolor/256x256/apps/

# Copies files
cp "$buildDir/MMT" mmt-deploy-root/usr/bin
cp /var/mmt/toolchains/linux/MeMoTo.desktop mmt-deploy-root/usr/share/applications/
cp /var/mmt/MMT/logo/logo.png mmt-deploy-root/usr/share/icons/hicolor/256x256/apps/MeMoTo.png

squashfs-root/AppRun mmt-deploy-root/usr/share/applications/MeMoTo.desktop -appimage

# Publishes file to MeMoTo output dir
if [[ "$APPIMAGE_ARCH" != "$APPIMAGE_DEPLOY_ARCH" ]]; then
  mv "MeMoTo-${APPIMAGE_DEPLOY_ARCH}.AppImage" "MeMoTo-${APPIMAGE_ARCH}.AppImage"
fi
cp "MeMoTo-${APPIMAGE_ARCH}.AppImage" "$(printenv OUTPUT_DIR)"

echo ""
echo "Output: $(printenv OUTPUT_DIR)/MeMoTo-${APPIMAGE_ARCH}.AppImage"
