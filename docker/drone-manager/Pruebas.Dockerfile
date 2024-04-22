FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3 as builder

WORKDIR /opt/oronetaBuilder
RUN microdnf -y install yum
RUN yum upgrade -y && yum install -y g++ gcc  make cmake git pkg-config
#RUN microdnf -y install curl
RUN yum -y install rpm wget tar python3 python3-devel python3-pip openssl perl bzip2
RUN yum -y install libquadmath-devel openssl-devel

#install boost
RUN curl -OL https://boostorg.jfrog.io/artifactory/main/release/1.85.0/source/boost_1_85_0.tar.bz2
RUN tar --bzip2 -xf boost_1_85_0.tar.bz2
RUN rm -f boost_1_85_0.tar.bz2
WORKDIR /opt/oronetaBuilder/boost_1_85_0
RUN ./bootstrap.sh
RUN ./b2 install

# Set library path
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

WORKDIR /opt/oronetaBuilder
#RUN dnf -y group install "Development Tools" "Development Libraries"
RUN git clone https://github.com/mavlink/MAVSDK.git --recursive
WORKDIR /opt/oronetaBuilder/MAVSDK
RUN cmake -Bbuild/default -DCMAKE_BUILD_TYPE=Release -H.
RUN cmake --build build/default -j8
RUN cmake --build build/default --target install

# Esto de aqui abajo es de ejemplo -------
WORKDIR /opt/oronetaBuilder
RUN mkdir -p ./bin
RUN echo "Holaaaa" > ./bin/test.log

RUN mkdir -p ./build
ADD ./src/drone-manager/build/Makefile ./build/
ADD ./src/drone-manager/src ./build/