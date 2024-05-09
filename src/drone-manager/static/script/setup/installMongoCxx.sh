#!/bin/bash

# i found the below steps from:
# https://developer.mongodb.com/community/forums/t/problems-trying-to-compile-mongocxx/2084
function install_mongocxx_dependencies() {
    TOP_DIR=$(pwd)

    curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.10.1/mongo-cxx-driver-r3.10.1.tar.gz
    tar -xzf mongo-cxx-driver-r3.10.1.tar.gz
    cd mongo-cxx-driver-r3.10.1/build
    mkdir cmake-build
    cd cmake-build
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
    sudo make install

    git clone https://github.com/mongodb/mongo-cxx-driver.git \
        --branch releases/stable --depth 1
    cd mongo-cxx-driver/build

    cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DBSONCXX_POLY_USE_MNMLSTC=1 \
      -DCMAKE_INSTALL_PREFIX=/usr/local

    make
    make install
    cd $TOP_DIR
    rm -rf mongo-c-driver-1.17.0*
}

function main() {
    install_mongocxx_dependencies
}

main