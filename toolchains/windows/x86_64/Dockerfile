# syntax=docker/dockerfile:1

# See https://medium.com/@vladadgad/cross-compile-qt-for-windows-on-linux-platform-57e4b71ed1aa for documentation
# See https://rizkiepratama.net/blog/2023/10/15/cross-compile-qt6-project-using-mxe
# See https://github.com/mxe/mxe

FROM ubuntu:jammy

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
	autoconf \
	automake \
	autopoint \
	bash \
	bison \
	bzip2 \
	flex \
	g++ \
	g++-multilib \
	gettext \
	git \
	gperf \
	intltool \
	libc6-dev-i386 \
	libgdk-pixbuf2.0-dev \
	libltdl-dev \
	libgl-dev \
	libpcre3-dev \
	libssl-dev \
	libtool-bin \
	libxml-parser-perl \
	lzip \
	make \
	openssl \
	p7zip-full \
	patch \
	perl \
	python3 \
	python3-distutils \
	python3-mako \
	python3-packaging \
	python3-pkg-resources \
	python3-setuptools \
	python-is-python3 \
	ruby \
	sed \
	sqlite3 \
	unzip \
	wget \
	xz-utils \
	git \
	cmake

ENV MXE_USR_DIR="/var/mxe/usr/" \
	OUTPUT_DIR="/var/mmt/"

# Compiles MXE Qt6 Static environment for w64 cross compilation
RUN cd /var \
	&& git clone https://github.com/mxe/mxe.git \
	&& cd mxe \
	&& make qt6 MXE_TARGETS="x86_64-w64-mingw32.static"

ENTRYPOINT ["/bin/bash", "/var/mmt/toolchains/windows/x86_64/build.sh"]
