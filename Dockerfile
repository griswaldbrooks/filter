# syntax=docker/dockerfile:1
FROM ubuntu:22.04 as upstream

# Prevent the interactive wizards from stopping the build
ARG DEBIAN_FRONTEND=noninteractive

# Get the basics
# hadolint ignore=DL3008
RUN --mount=type=cache,target=/var/cache/apt,id=apt \
  apt-get update -y && apt-get install -q -y --no-install-recommends \
  build-essential        \
  cmake                  \
  lsb-core               \
  wget                   \
  && rm -rf /var/lib/apt/lists/*

FROM upstream AS development

ARG UID
ARG GID
ARG USER

# fail build if args are missing
# hadolint ignore=SC2028
RUN if [ -z "$UID" ]; then echo '\nERROR: UID not set. Run \n\n \texport UID=$(id -u) \n\n on host before building Dockerfile.\n'; exit 1; fi
# hadolint ignore=SC2028
RUN if [ -z "$GID" ]; then echo '\nERROR: GID not set. Run \n\n \texport GID=$(id -g) \n\n on host before building Dockerfile.\n'; exit 1; fi
# hadolint ignore=SC2028
RUN if [ -z "$USER" ]; then echo '\nERROR: USER not set. Run \n\n \texport USER=$(whoami) \n\n on host before building Dockerfile.\n'; exit 1; fi
# hadolint ignore=DL3008
RUN --mount=type=cache,target=/var/cache/apt,id=apt \
  apt-get update && apt-get upgrade -y \
  && apt-get install -q -y --no-install-recommends \
  git \
  python3 \
  python3-pip \
  sudo \
  ssh \
  vim \
  wget \
  && rm -rf /var/lib/apt/lists/*

# install developer tools
RUN python3 -m pip install --no-cache-dir \
  pre-commit==3.0.4

# install hadolint
RUN wget -q -O /bin/hadolint https://github.com/hadolint/hadolint/releases/download/v2.12.0/hadolint-Linux-x86_64 \
  && chmod +x /bin/hadolint

# Setup user home directory
# --no-log-init helps with excessively long UIDs
RUN groupadd --gid $GID $USER \
  && useradd --no-log-init --uid $GID --gid $UID -m $USER --groups sudo \
  && echo $USER ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USER \
  && chmod 0440 /etc/sudoers.d/$USER \
  && echo "source /opt/ros/${ROS_DISTRO}/setup.bash" >> /home/${USER}/.profile \
  && touch /home/${USER}/.bashrc \
  && chown -R ${GID}:${UID} /home/${USER}

USER $USER
ENV SHELL /bin/bash
ENTRYPOINT []

# Setup mixin
WORKDIR /home/${USER}/ws
