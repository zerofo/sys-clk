#!/bin/bash
set -e

ROOT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIST_DIR="$ROOT_DIR/dist"
AMS_DIR="$DIST_DIR/atmosphere/contents"
SXOS_DIR="$DIST_DIR/sxos/titles"
CORES="$(nproc --all)"

if [[ -n "$1" ]]; then
    DIST_DIR="$1"
fi

echo "DIST_DIR: $DIST_DIR"
echo "CORES: $CORES"

echo "*** sysmodule ***"
TITLE_ID="$(grep -oP '"title_id":\s*"0x\K(\w+)' "$ROOT_DIR/sysmodule/perms.json")"

pushd "$ROOT_DIR/sysmodule"
make -j$CORES
popd > /dev/null

for p in $AMS_DIR $SXOS_DIR; do
    mkdir -p "$p/$TITLE_ID/flags"
    cp -vf "$ROOT_DIR/sysmodule/out/sys-clk.nsp" "$p/$TITLE_ID/exefs.nsp"
    >"$p/$TITLE_ID/flags/boot2.flag"
done

echo "*** manager ***"
pushd "$ROOT_DIR/manager"
make -j$CORES
popd > /dev/null

mkdir -p "$DIST_DIR/switch"
cp -vf "$ROOT_DIR/manager/sys-clk-manager.nro" "$DIST_DIR/switch/sys-clk-manager.nro"

echo "*** overlay ***"
pushd "$ROOT_DIR/overlay"
make -j$CORES
popd > /dev/null

mkdir -p "$DIST_DIR/switch/.overlays"
cp -vf "$ROOT_DIR/overlay/out/sys-clk-overlay.ovl" "$DIST_DIR/switch/.overlays/sys-clk-overlay.ovl"

echo "*** assets ***"
mkdir -p "$DIST_DIR/config/sys-clk"
cp -vf "$ROOT_DIR/config.ini.template" "$DIST_DIR/config/sys-clk/config.ini.template"
>"$DIST_DIR/config/sys-clk/log.flag"
cp -vf "$ROOT_DIR/README.md" "$DIST_DIR/README.md"
