#!/bin/bash -e

rm -rf Release
mkdir -p Release/Build
cd Release/Build

cmake ../../Sources -DCMAKE_BUILD_TYPE=Release
cmake --build . --target all deploy_deb --parallel

mv Output/* ..
cd ../..

rm -rf Release/Build
