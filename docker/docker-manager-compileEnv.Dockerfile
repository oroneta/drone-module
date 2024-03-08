FROM redhat/ubi9:latest

LABEL version="0.1"
LABEL description="entorno para compilar"

RUN mkdir -p /opt/docker-manager
WORKDIR /opt/docker-manager
VOLUME /opt/docker-manager
RUN yum -y update
RUN yum -y install g++ gcc make cmake
RUN yum -y --allowerasing install wget curl
RUN yum -y install git 
