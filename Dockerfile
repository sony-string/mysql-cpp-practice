FROM ubuntu:latest

# RUN apt-get update && apt-get install -y \
#     build-essential \
#     cmake \
#     gdb \
#     valgrind \
#     doxygen \
#     graphzviz \
#     libcunit1 \
#     libcunit1-doc \
#     libcunit1-dev \
#     && apt-get clean \
#     && rm -rf /var/lib/apt/lists/*

RUN apt-get update

RUN apt-get install -y build-essential

RUN apt-get install libmysqlcppconn-dev

RUN apt-get install libmysqlclient-dev

RUN apt-get clean

RUN rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/http_web_server_c

ADD . /usr/src/http_web_server_c
