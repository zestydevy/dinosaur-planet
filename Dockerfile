FROM ubuntu:24.04 AS base

# Create directory for mount
RUN mkdir /dino
WORKDIR /dino

# Install required packages
ENV DEBIAN_FRONTEND=noninteractive

COPY packages.txt ./
RUN apt-get update && apt-get install -y $(cat packages.txt) && rm -rf /var/lib/apt/lists/*

COPY requirements.txt ./
COPY tools/splat/requirements.txt tools/splat/
RUN pip3 install -r requirements.txt --break-system-packages

# Symlink dino.py
RUN ln -s /dino/dino.py /usr/local/bin/dino


FROM base AS ido

# Compile IDO recomp
ADD tools/ido_static_recomp ./tools/ido_static_recomp
RUN make -C tools/ido_static_recomp setup && \
    make -C tools/ido_static_recomp VERSION=5.3 RELEASE=1 -j && \
    make -C tools/ido_static_recomp VERSION=7.1 RELEASE=1 -j


FROM base

# Copy IDO binaries
COPY --from=ido /dino/tools/ido_static_recomp/build /dino/tools/ido_static_recomp/build

# Set up user (if they don't exist)
ARG login=sabre
ARG uid=1001
RUN id -u $uid &>/dev/null || adduser --system --uid $uid --group $login

# Set entrypoint
RUN echo "#!/bin/bash\nexec \"\$@\"" > /entrypoint.sh && chmod +x /entrypoint.sh
USER $uid
ENTRYPOINT ["/entrypoint.sh"]
