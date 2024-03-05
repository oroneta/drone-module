FROM redhat/ubi9:latest
RUN mkdir -p /app
WORKDIR /app
VOLUME /app
RUN yum install g++ gcc make cmake
RUN yum install wget curl
RUN yum install git
