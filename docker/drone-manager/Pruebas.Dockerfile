FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3 as builder

WORKDIR /opt/oronetaBuilder

RUN microdnf upgrade -y && microdnf install -y g++ gcc  make cmake git pkg-config
#RUN git clone https://github.com/open-source-parsers/jsoncpp.git \
#    && cd jsoncpp && mkdir -p build \
#    && cd build \
#    && cmake \
#    -DCMAKE_BUILD_TYPE=release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF \
#    -DCMAKE_INSTALL_INCLUDEDIR=include/jsoncpp -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" .. \
#    && make && make install

RUN yum -y install https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm
RUN yum -y install jsoncpp-devel

# Esto de aqui abajo es de ejemplo -------
RUN mkdir -p ./bin
RUN echo "Holaaaa" > ./bin/test.log

RUN mkdir -p ./build
ADD ./src/drone-manager/build/Makefile ./build/
ADD ./src/drone-manager/src ./build/