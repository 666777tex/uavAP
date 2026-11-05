FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
build-essential \
cmake \
g++ \
wget \
libboost-system-dev \
libqt5svg5-dev \
qtbase5-dev \
redis-server \
git \
nlohmann-json3-dev \
libcurl4-openssl-dev \
&& rm -rf /var/lib/apt/lists/*

RUN wget https://gitlab.com/libeigen/eigen/-/archive/3.3.4/eigen-3.3.4.tar.bz2

RUN tar -xjf eigen-3.3.4.tar.bz2

RUN cmake -S eigen-3.3.4 -B eigen-build \
    -DCMAKE_INSTALL_PREFIX=/usr/local

RUN cmake --install eigen-build

WORKDIR /uavAP

COPY . .

RUN cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=/usr/local \
    -DGS=1

RUN cmake --build build -j$(nproc)

RUN cmake --install build

RUN ldconfig


