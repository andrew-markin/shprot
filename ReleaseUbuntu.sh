#!/bin/bash -e
rm -rf Release && mkdir -p Release/Build && cd $_
cmake ../../Sources -DCMAKE_BUILD_TYPE=Release && make -j$(nproc) && make deploy_deb
mv Output/* .. && cd ../.. && rm -rf Build && cd ..
