FROM nvcr.io/nvidia/l4t-base:r32.6.1

RUN apt update && apt -y install build-essential \
                                    pkg-config \
                                    cmake \
                                    ffmpeg \
                                    openssh-client \
                                    openmpi-common openmpi-bin libopenmpi-dev \
                                    libboost-program-options1.65-dev \
                                    gstreamer1.0-tools gstreamer1.0-alsa \
                                    gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
                                    gstreamer1.0-plugins-ugly gstreamer1.0-plugins-bad gstreamer1.0-libav \
                                    libglib2.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev 

WORKDIR /usr/src/app
COPY ./ ./

WORKDIR /usr/src/app/build

RUN cmake .. && make -j3
