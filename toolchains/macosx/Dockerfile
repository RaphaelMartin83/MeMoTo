FROM stateoftheartio/qt6:6.7-macos-aqt

# Installs missing libraries and tools
RUN sudo apt update \
	&& sudo apt install -y python3-pip \
	&& sudo python3 -m pip install aqtinstall \
	&& sudo aqt install-qt -O /opt/Qt mac desktop 6.7.0 clang_64 -m qtwebsockets \
	&& curl -Lo /tmp/qtbase-src.tar.xz "https://download.qt.io/official_releases/qt/6.7/6.7.0/submodules/qtbase-everywhere-src-6.7.0.tar.xz" \
	&& tar -C /tmp -xf /tmp/qtbase-src.tar.xz \
	&& rm -f /tmp/qtbase-src.tar.xz \
	&& cmake -S "/tmp/qtbase-everywhere-src-6.7.0" -G Ninja -B /tmp/macdeployqt-build -DQT_FEATURE_macdeployqt=ON -DQT_FEATURE_network=OFF -DQT_FEATURE_gui=OFF -DQT_FEATURE_concurrent=OFF -DQT_FEATURE_sql=OFF -DQT_FEATURE_dbus=OFF -DQT_FEATURE_testlib=OFF -DQT_FEATURE_printsupport=OFF -DQT_FEATURE_androiddeployqt=OFF -DBUILD_SHARED_LIBS=OFF \
	&& cmake --build /tmp/macdeployqt-build \
	&& sudo cp /tmp/macdeployqt-build/libexec/moc /opt/Qt/6.7.0/macos/./libexec/moc

COPY <<-EOT build.sh
	set -e

	# Repo is expected to be mapped in /var/mmt

	qt-cmake -S /var/mmt/MMT/ -B /tmp/mmt_build_macos -G Ninja
	cmake --build /tmp/mmt_build_macos/

	# This commented code would be used to create a .dmg file with macdeployqt
	# It is deactivated because .dmg files cannot be used without signature and notarization on MacOSX machines anyway
	# We are still compiling to make sure MacOSX target compilation is not broken
	#sudo mkdir -p /usr/local/Qt-6.7.0/plugins
	#sudo cp -r /opt/Qt/6.7.0/macos/plugins/* /usr/local/Qt-6.7.0/plugins
	#/tmp/macdeployqt-build/bin/macdeployqt /tmp/mmt_build_macos/*.app -dmg -no-strip

	# Publishes file to MeMoTo output dir
	#cp /tmp/mmt_build_macos/MMT.dmg "$(printenv OUTPUT_DIR)/MeMoTo-MacOSX.dmg"

	#echo ""
	#echo "Output: $(printenv OUTPUT_DIR)/MeMoTo-MacOSX.dmg"
EOT

ENTRYPOINT ["/bin/bash", "build.sh"]
