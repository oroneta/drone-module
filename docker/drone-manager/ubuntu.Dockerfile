FROM ubuntu:22.04

RUN mkdir -p /opt/oronetaBuilder
RUN mkdir -p /opt/libPreparation
WORKDIR /opt/libPreparation
RUN apt-get -y update && apt-get -y upgrade
RUN apt-get -y install build-essential cmake make libboost-all-dev gcc g++
RUN apt-get -y install wget curl python3 python3-pip perl bzip2 git pkg-config
RUN apt-get -y install protobuf-compiler libprotobuf-dev libzstd-dev 
RUN apt-get -y install openssl libssl-dev libboost-dev
RUN openssl version

#install mongo cxx driver
RUN git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
WORKDIR /opt/libPreparation/mongo-cxx-driver/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DMONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX=OFF
RUN cmake --build .
RUN cmake --build . --target install

WORKDIR /opt/libPreparation
#install mavsdk 
RUN git clone https://github.com/mavlink/MAVSDK.git --recursive
WORKDIR /opt/libPreparation/MAVSDK
RUN mkdir build
WORKDIR /opt/libPreparation/MAVSDK/build
RUN cmake ..
RUN make -j$(nproc)
RUN make install

WORKDIR /opt/oronetaBuilder
RUN mkdir -p ./bin
RUN echo "Holaaaa" > ./bin/test.log

RUN mkdir -p ./build
ADD ./src/drone-manager/build/Makefile ./build/
ADD ./src/drone-manager/src ./build/ 
