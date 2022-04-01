FROM ubuntu:20.04

# Prevent the interactive wizards from stopping the build
ARG DEBIAN_FRONTEND=noninteractive

# Get the basics
RUN apt update -y &&         \
    apt install -y           \
             build-essential \
             cmake           \
             git             \
             lsb-core        \
             python3         \
             python3-pip     \
             sudo            \
             wget

# Build gtest and gmock
RUN apt install -y libgtest-dev &&                                 \
    cmake /usr/src/googletest/CMakeLists.txt &&                    \
    ls /usr/src/googletest &&                                      \
    (cd /usr/src/googletest && make) &&                            \
    cp -r /usr/src/googletest/googlemock/include/* /usr/include && \
    cp -r /usr/src/googletest/googletest/include/* /usr/include && \
    cp -r /usr/src/googletest/lib/* /usr/lib


RUN apt clean
