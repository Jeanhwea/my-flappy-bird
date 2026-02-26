#!/usr/bin/env bash
set -eux

GIT_TAG=$(git describe --tags --always --dirty="+dev")
OUT_DIR=output
DST_DIR=dist
OS_NAME=$(uname -s)
OS_ARCH=$(uname -m)
DMG_DIR=${OUT_DIR}/dmg

APP_PKG=my-flappy-bird
DMG_VOL=${APP_PKG}-${OS_NAME}-${OS_ARCH}-${GIT_TAG}
DMG_IMG=${OUT_DIR}/${APP_PKG}-${OS_NAME}-${OS_ARCH}-${GIT_TAG}.dmg

# Check if cmake is installed
if ! command -v cmake &> /dev/null
then
    echo "cmake could not be found"
    exit
fi

# Display cmake version
cmake --version

cmake -B output -G"Xcode" --log-level=STATUS

cmake --build output --config Release

mkdir -p ${DMG_DIR}
mv ${OUT_DIR}/bin/${APP_PKG}/Release/${APP_PKG}.app ${DMG_DIR}
ln -s /Applications ${DMG_DIR}/Applications
hdiutil create -volname ${DMG_VOL} -srcfolder ${DMG_DIR} -ov -format UDZO ${DMG_IMG}
mkdir -p ${DST_DIR}
mv ${DMG_IMG} ${DST_DIR}

rm -rf ${OUT_DIR}
