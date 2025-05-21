FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    git wget curl ca-certificates \
    python3.8 python3.8-venv python3-pip \
    gcc g++ make cmake ninja-build ccache libffi-dev libssl-dev \
    libncurses-dev flex bison libusb-1.0-0-dev libreadline-dev \
    && rm -rf /var/lib/apt/lists/*


RUN update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1

WORKDIR /opt
RUN git clone --branch v4.3 --recursive https://github.com/espressif/esp-idf.git

WORKDIR /opt/esp-idf
RUN python3 -m venv ./venv && ./venv/bin/pip install --upgrade pip setuptools wheel \
    && ./install.sh

ENV PATH="/opt/esp-idf/venv/bin:/opt/esp-idf/tools:$PATH"
ENV IDF_PATH=/opt/esp-idf

WORKDIR /project
CMD ["/bin/bash"]
