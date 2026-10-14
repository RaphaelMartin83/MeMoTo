# syntax=docker/dockerfile:1

FROM python:3.10-slim-bullseye AS qt-sdk

ARG TARGETARCH
ARG QT_VERSION=6.7.3

RUN pip install --no-cache-dir aqtinstall==3.3.0 \
	&& case "$TARGETARCH" in \
		amd64) qt_host=linux; qt_arch=linux_gcc_64 ;; \
		arm64) qt_host=linux_arm64; qt_arch=linux_gcc_arm64 ;; \
		*) echo "Unsupported target architecture: $TARGETARCH" >&2; exit 1 ;; \
	esac \
	&& aqt install-qt "$qt_host" desktop "$QT_VERSION" "$qt_arch" \
		--modules qtwebsockets \
		-O /opt/qt

FROM ubuntu:focal

ARG TARGETARCH
ARG QT_VERSION=6.7.3

RUN apt-get update \
	&& DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
		build-essential \
		file \
		libbrotli-dev \
		libdbus-1-dev \
		libegl-dev \
		libfontconfig1-dev \
		libfreetype-dev \
		libgl-dev \
		libsm6 \
		libx11-dev \
		libx11-xcb-dev \
		libxcb-cursor-dev \
		libxcb-glx0-dev \
		libxcb-icccm4-dev \
		libxcb-image0-dev \
		libxcb-keysyms1-dev \
		libxcb-randr0-dev \
		libxcb-render-util0-dev \
		libxcb-shape0-dev \
		libxcb-shm0-dev \
		libxcb-sync-dev \
		libxcb-util-dev \
		libxcb-xfixes0-dev \
		libxcb-xinerama0-dev \
		libxcb-xkb-dev \
		libxcb1-dev \
		libxext-dev \
		libxfixes-dev \
		libxi-dev \
		libxkbcommon-dev \
		libxkbcommon-x11-dev \
		libxrender-dev \
		libwayland-dev \
		python3-pip \
		python3-venv \
		unzip \
		wget \
	&& rm -rf /var/lib/apt/lists/*

COPY --from=qt-sdk /opt/qt/ /opt/qt/

RUN python3 -m venv /opt/venv \
	&& /opt/venv/bin/pip install --no-cache-dir cmake==3.30.4 \
	&& case "$TARGETARCH" in \
		amd64) qt_dir=gcc_64 ;; \
		arm64) qt_dir=gcc_arm64 ;; \
		*) echo "Unsupported target architecture: $TARGETARCH" >&2; exit 1 ;; \
	esac \
	&& ln -s "/opt/qt/$QT_VERSION/$qt_dir" /usr/lib/qt-dev

ENV QT_DIR="/usr/lib/qt-dev" \
	Qt6_DIR="/usr/lib/qt-dev/lib/cmake/Qt6" \
	CMAKE_PREFIX_PATH="/usr/lib/qt-dev" \
	PATH="/usr/lib/qt-dev/bin:/opt/venv/bin:${PATH}"
