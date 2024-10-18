# syntax=docker/dockerfile:1

FROM ubuntu:focal

RUN apt update \
	&& apt install -y libfreetype-dev \
	&& apt install -y libx11-dev \
	&& apt install -y libx11-xcb-dev \
	&& apt install -y libxcb-cursor-dev \
	&& apt install -y libxcb-glx0-dev \
	&& apt install -y libxcb-icccm4-dev \
	&& apt install -y libxcb-image0-dev \
	&& apt install -y libxcb-keysyms1-dev \
	&& apt install -y libxcb-randr0-dev \
	&& apt install -y libxcb-render-util0-dev \
	&& apt install -y libxcb-shape0-dev \
	&& apt install -y libxcb-shm0-dev \
	&& apt install -y libxcb-sync-dev \
	&& apt install -y libxcb-util-dev \
	&& apt install -y libxcb-xfixes0-dev \
	&& apt install -y libxcb-xinerama0-dev \
	&& apt install -y libxcb-xkb-dev \
	&& apt install -y libxcb1-dev \
	&& apt install -y libxext-dev \
	&& apt install -y libxfixes-dev \
	&& apt install -y libxi-dev \
	&& apt install -y libxkbcommon-dev \
	&& apt install -y libxkbcommon-x11-dev \
	&& apt install -y libxrender-dev \
	&& apt install -y --no-install-recommends libfontconfig1-dev \
	&& apt install -y libdbus-1-dev \
	&& apt install -y libgl-dev \
	&& apt install -y wget \
	&& apt install -y gcc g++ \
	&& apt install -y unzip \
	&& apt install -y python3 \
	&& apt install -y python3-pip

RUN python3 -m pip install cmake==3.30.4

RUN useradd -m -u 1000 --shell /bin/bash memoto-builder

USER memoto-builder
WORKDIR /home/memoto-builder

# Qt Version
ARG QT_MAJOR=6
ARG QT_MINOR=7
ARG QT_PATCH=3

# Compile and deploy Qt from source
RUN wget -q https://download.qt.io/official_releases/qt/$QT_MAJOR.$QT_MINOR/$QT_MAJOR.$QT_MINOR.$QT_PATCH/single/qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH.zip \
	&& unzip -q qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH.zip \
	&& rm qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH.zip \
	&& sed -i -e 's/\r$//' qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH/configure \
	&& sed -i -e 's/\r$//' qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH/qtbase/configure \
	&& mkdir -p build \
	&& cd build \
	&& ../qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH/configure -release -opensource -confirm-license -nomake examples -nomake tests -skip qt3d -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtconnectivity -skip qtdatavis3d \
	&& cmake --build . -j6 \
	&& cmake --install . --prefix $HOME/qt-dev \
	&& rm -rf ../qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH

