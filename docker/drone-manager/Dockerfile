# Build stage
FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3 as builder

WORKDIR /opt/oronetaBuilder


RUN microdnf upgrade -y && microdnf install -y g++ gcc  make cmake git pkg-config
#RUN microdnf -y install curl
RUN microdnf -y install yum rpm wget tar dnf python3 python3-pip openssl perl
RUN git clone https://github.com/mavlink/MAVSDK.git --recursive
WORKDIR /opt/oronetaBuilder/MAVSDK

RUN cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_MAVSDK_SERVER=ON -Bbuild/default -H. 
RUN cmake --build build/default -j8
RUN cmake --build build/default --target install

# Esto de aqui abajo es de ejemplo -------
RUN mkdir -p ./bin
RUN echo "Holaaaa" > ./bin/test.log

RUN mkdir -p ./build
ADD ./src/drone-manager/build/Makefile ./build/
ADD ./src/drone-manager/src ./build/
# Copias lo que necesites
# Y haces compile
# RUN make
#
# Recuerda hacer el make que los binarios se copien a una carpeta /opt/oronetaBuilder/bin

# ---------------------------------------------
# Production stage
FROM registry.access.redhat.com/ubi9/ubi:9.3

RUN mkdir /opt/OronetaManager
WORKDIR /opt/OronetaManager

# Copy all files from the builder stage
COPY --from=builder /opt/oronetaBuilder/bin ./bin

# Copy static files
COPY ./src/drone-manager/static/ .

# Install dependencies mongocxx
RUN ./script/installMongoCxx.sh



# tail ./test.log
# CMD ["./script/showTree.sh"]

CMD ["mongocxx-config", "--version"]