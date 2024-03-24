FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3 as builder

WORKDIR /opt/oronetaBuilder
RUN microdnf upgrade -y && microdnf install -y g++ gcc  make cmake git pkg-config
#RUN microdnf -y install curl
RUN microdnf -y install yum rpm wget tar dnf python3 python3-pip openssl perl

#RUN dnf -y group install "Development Tools" "Development Libraries"
RUN git clone https://github.com/mavlink/MAVSDK.git --recursive
WORKDIR /opt/oronetaBuilder/MAVSDK
#RUN wget https://github.com/mavlink/MAVSDK/archive/refs/tags/v2.5.1.tar.gz
#RUN tar -xvf v2.5.1.tar.gz
#WORKDIR /opt/oronetaBuilder/MAVSDK-v2.5.1
#RUN git submodule update --init --recursive
RUN cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_MAVSDK_SERVER=ON -Bbuild/default -H. 
RUN cmake --build build/default -j8
RUN cmake --build build/default --target install

#TODO: JSONCPP NO INSTALADO
#RUN microdnf download -y epel-release
#RUN microdnf install -y 
#RUN yum install -y jsoncpp-devel

# Esto de aqui abajo es de ejemplo -------
WORKDIR /opt/oronetaBuilder
RUN mkdir -p ./bin
RUN echo "Holaaaa" > ./bin/test.log

RUN mkdir -p ./build
ADD ./src/drone-manager/build/Makefile ./build/
ADD ./src/drone-manager/src ./build/