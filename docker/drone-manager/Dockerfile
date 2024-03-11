# Build stage
FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3 as builder

WORKDIR /opt/oronetaBuilder

RUN microdnf upgrade -y && microdnf install -y g++


# Esto de aqui abajo es de ejemplo -------
RUN mkdir ./bin
RUN echo "Holaaaa" > ./bin/test.log
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