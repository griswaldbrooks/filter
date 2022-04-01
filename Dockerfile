# syntax=docker/dockerfile:1
FROM ubuntu:20.04
ARG UIDGID
ARG USER

# fail build if args are missing
RUN if [ -z "$UIDGID" ]; then echo '\nERROR: UIDGID not set. Run \n\n \texport UIDGID=$(id -u):$(id -g) \n\n on host before building Dockerfile.\n'; exit 1; fi
RUN if [ -z "$USER" ]; then echo '\nERROR: USER not set. Run \n\n \texport USER=$(whoami) \n\n on host before building Dockerfile.\n'; exit 1; fi

# Prevent the interactive wizards from stopping the build
ARG DEBIAN_FRONTEND=noninteractive

# Get the basics
RUN apt update -y && apt install -y \
    build-essential        \
    cmake                  \
    git                    \
    lsb-core               \
    python3                \
    python3-pip            \
    sudo                   \
    wget

# Build gtest and gmock
RUN apt install -y libgtest-dev &&                                 \
    cmake /usr/src/googletest/CMakeLists.txt &&                    \
    ls /usr/src/googletest &&                                      \
    (cd /usr/src/googletest && make) &&                            \
    cp -r /usr/src/googletest/googlemock/include/* /usr/include && \
    cp -r /usr/src/googletest/googletest/include/* /usr/include && \
    cp -r /usr/src/googletest/lib/* /usr/lib

# chown working directory to user
RUN mkdir -p /home/${USER}/ws && chown -R ${UIDGID} /home/${USER}
