# syntax=docker/dockerfile:1

FROM ghcr.io/raphaelmartin83/qt_compilation_base

# The linuxdeployqt appimage
RUN wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
RUN chmod u+x linuxdeployqt-continuous-x86_64.AppImage

ENV QT_DIR="/usr/lib/qt-dev/" \
	Qt6_DIR="/usr/lib/qt-dev/" \
	buildDir="/tmp/buildmmt" \
	OUTPUT_DIR="/var/mmt" \
	MMT_TAG="v0.0.0"


ENTRYPOINT ["/bin/bash", "/var/mmt/toolchains/linux/x86_64/build.sh"]
