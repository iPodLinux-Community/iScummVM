#!/bin/sh
#set -vx
#exec >> /opt/Emulators/iPodMAME/Misc/Launch.log 2>&1

# Format: $binary $romset_name
killall -15 ZeroLauncher >> /dev/null 2>&1
cpu_speed 78
cd /opt/Emulators/ScummVM
exec /opt/Emulators/ScummVM/scummvm
