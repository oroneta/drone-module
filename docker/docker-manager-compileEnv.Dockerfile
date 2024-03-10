FROM redhat/ubi9:latest

LABEL version="0.1"
LABEL description="entorno para compilar"

RUN mkdir -p /opt/drone-manager
WORKDIR /opt/drone-manager
VOLUME /opt/drone-manager
RUN yum -y update
RUN yum -y install g++ gcc make cmake
RUN yum -y --allowerasing install wget curl
RUN yum -y install git libzstd openssl mongodb-org
