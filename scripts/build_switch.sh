#!/bin/bash
set -e

BUILD_DIR=cmake-build-switch

cmake -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Release -DBUILTIN_NSP=ON -DPLATFORM_SWITCH=ON
make -C ${BUILD_DIR} UpcomingSwitchGames.nro -j$(nproc)