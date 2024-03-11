FROM mongodb/mongo-cxx-driver:latest

LABEL version="0.1"
LABEL description="entorno para compilar"

RUN mkdir -p /opt/drone-manager
WORKDIR /opt/drone-manager
VOLUME /opt/drone-manager
RUN yum -y update
RUN yum -y install g++ gcc make cmake tar
RUN yum -y --allowerasing install wget 
RUN yum -y install git 