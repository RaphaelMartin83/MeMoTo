# Read https://github.com/tpoechtrager/osxcross?tab=readme-ov-file
# Read https://github.com/crazy-max/docker-osxcross

from ubuntu:noble

COPY xcode /tmp/

ARG DEBIAN_FRONTEND=noninteractive

# Get OSXCROSS dependancies
RUN apt update \
	&& apt install -y make \
	clang \
	cmake \
	curl \
	unzip \
	gcc \
	g++ \
	git \
	cpio \
	clang \
	libxml2-dev \
	libssl-dev \
	libz-dev \
	liblzma-dev \
	patch \
	python3 \
	lzma-dev \
	libxml2-dev \
	bzip2 \
	zlib1g-dev \
	libbz2-dev \
	wget

# Build OSXCROSS
RUN git clone https://github.com/tpoechtrager/osxcross.git \
	&& cd osxcross \
	&& ./tools/gen_sdk_package_pbzx.sh $(ls /tmp/*.xip | head -n 1) \
	&& mv $(ls MacOSX*.sdk.tar* | head -n 1) tarballs \
	&& TARGET_DIR=/usr/local/osxcross UNATTENDED=1 ./build.sh \
	&& export SDK_VERSION=$(ls tarballs | sed 's/MacOSX//' | sed 's/\.sdk.*//') \
	&& export PATH=/usr/local/osxcross/bin:$PATH

# Build Qt for host (cannot use qt_base, need earlier ubuntu)
ARG QT_MAJOR=6
ARG QT_MINOR=7
ARG QT_PATCH=3

RUN cd /tmp \
	&& wget -q https://download.qt.io/official_releases/qt/$QT_MAJOR.$QT_MINOR/$QT_MAJOR.$QT_MINOR.$QT_PATCH/single/qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH.zip \
	&& unzip -q qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH.zip \
	&& rm qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH.zip \
	&& apt-get install -y make cmake build-essential libclang-dev ninja-build bison python3 gperf pkg-config \
	libfontconfig1-dev libfreetype6-dev libx11-dev libx11-xcb-dev libxext-dev libxfixes-dev libxi-dev \
	libxrender-dev libxcb1-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync-dev \
	libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev libxcb-util-dev libxcb-xinerama0-dev libxcb-xkb-dev libxkbcommon-dev \
	libxkbcommon-x11-dev libatspi2.0-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev \
	&& mkdir /tmp/qt-hostbuild && mkdir /usr/bin/qt-host && cd /tmp/qt-hostbuild \
	&& cmake /tmp/qt-everywhere-src-$QT_MAJOR.$QT_MINOR.$QT_PATCH/ -GNinja -DCMAKE_BUILD_TYPE=Release -DQT_BUILD_EXAMPLES=OFF -DQT_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/usr/bin/qt-host \
	&& cmake --build . --parallel 8 \
	&& cmake --install .
