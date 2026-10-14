# syntax=docker/dockerfile:1

FROM ghcr.io/raphaelmartin83/qt_compilation_base

RUN wget -q -O /usr/local/bin/linuxdeployqt \
	https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-aarch64.AppImage \
	&& chmod u+x /usr/local/bin/linuxdeployqt

ENV buildDir="/tmp/buildmmt" \
	OUTPUT_DIR="/var/mmt" \
	MMT_TAG="v0.0.0"

ENTRYPOINT ["/bin/bash", "/var/mmt/toolchains/linux/build.sh"]
