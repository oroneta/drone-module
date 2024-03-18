FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3 as builder

WORKDIR /opt/oronetaBuilder

RUN microdnf upgrade -y && microdnf install -y g++ gcc  make cmake git pkg-config
RUN microdnf -y install yum
#TODO: JSONCPP NO INSTALADO
#RUN microdnf download -y epel-release
#RUN microdnf install -y 
#RUN yum install -y jsoncpp-devel

# Esto de aqui abajo es de ejemplo -------
RUN mkdir -p ./bin
RUN echo "Holaaaa" > ./bin/test.log

RUN mkdir -p ./build
ADD ./src/drone-manager/build/Makefile ./build/
ADD ./src/drone-manager/src ./build/