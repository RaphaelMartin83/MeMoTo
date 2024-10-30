# syntax=docker/dockerfile:1

FROM ghcr.io/raphaelmartin83/qt_compilation_base

# The linuxdeployqt appimage
RUN wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
RUN chmod u+x linuxdeployqt-continuous-x86_64.AppImage

ENV QT_DIR="/usr/lib/qt-dev/" \
	Qt6_DIR="/usr/lib/qt-dev/" \
	buildDir="/tmp/buildmmt" \
	OUTPUT_DIR="/var/mmt" \
	MMT_VERSION_MAJOR="0" \
	MMT_VERSION_MINOR="0" \
	MMT_VERSION_PATCH="0"


COPY <<-EOT build.sh
	set -e

	# Repo is expected to be mapped in /var/mmt

	export PATH=$PATH:$QT_DIR/bin

	cmake -DMEMOTO_VERSION_MAJOR="$(printenv MMT_VERSION_MAJOR)" -DMEMOTO_VERSION_MINOR="$(printenv MMT_VERSION_MINOR)" -DMEMOTO_VERSION_PATCH="$(printenv MMT_VERSION_PATCH)" -DMEMOTO_VERSION_FULL="$(printenv MMT_VERSION_MAJOR).$(printenv MMT_VERSION_MINOR).$(printenv MMT_VERSION_PATCH)" -DCMAKE_BUILD_TYPE=Release -S /var/mmt/MMT/ -B $buildDir
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

	# Publishes file to MeMoTo build dir
	cp MeMoTo-x86_64.AppImage "$(printenv OUTPUT_DIR)"

	echo ""
	echo "Output: $(printenv OUTPUT_DIR)/MeMoTo-x86_64.AppImage"
EOT

ENTRYPOINT ["/bin/bash", "build.sh"]
