FROM gcc:12 as builder

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libssl-dev \
    libmariadb-dev \
    libhiredis-dev \
    libyaml-cpp-dev \
    libre2-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    libgrpc-dev \
    git \
    && rm -rf /var/lib/apt/lists/*

RUN \
    echo '#!/bin/sh' > /usr/bin/grpc_cpp_plugin && \
    echo '#!/bin/sh' > /usr/bin/grpc_php_plugin && \
    echo '#!/bin/sh' > /usr/bin/grpc_python_plugin && \
    echo '#!/bin/sh' > /usr/bin/grpc_ruby_plugin && \
    echo '#!/bin/sh' > /usr/bin/grpc_node_plugin && \
    echo '#!/bin/sh' > /usr/bin/grpc_go_plugin && \
    echo '#!/bin/sh' > /usr/bin/grpc_java_plugin && \
    echo '#!/bin/sh' > /usr/bin/protoc-gen-grpc && \
    chmod +x /usr/bin/grpc_*_plugin /usr/bin/protoc-gen-grpc

WORKDIR /app

COPY . .

RUN rm -rf build && mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DBUILD_TESTS=OFF \
          .. && \
    make -j$(nproc) && \
    mkdir -p /app/bin && \
    cp src/server/ourchat_server /app/bin/

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    libssl-dev \
    libmariadb-dev \
    libhiredis-dev \
    libyaml-cpp-dev \
    libprotobuf-dev \
    libgrpc++-dev \
    libgrpc-dev \
    libabsl-dev \
    && rm -rf /var/lib/apt/lists/* \
    && ldconfig

WORKDIR /app

COPY --from=builder /app/bin/ourchat_server /usr/local/bin/
COPY config/ /app/config/

RUN mkdir -p /app/logs && \
    chmod +x /usr/local/bin/ourchat_server

EXPOSE 50051 8080

ENV CONFIG_PATH=/app/config/server.yaml

CMD ["/usr/local/bin/ourchat_server", "/app/config/server.yaml"]
