FROM ubuntu:20.04

# Create directory for mount
RUN mkdir /dino
WORKDIR /dino

# Install required packages
ENV DEBIAN_FRONTEND=noninteractive

COPY packages.txt ./
RUN apt-get update && apt-get install -y $(cat packages.txt) && rm -rf /var/lib/apt/lists/*

COPY requirements.txt ./
RUN pip3 install -r requirements.txt

# Symlink dino.py
RUN ln -s /dino/dino.py /usr/local/bin/dino

# Set up user (if they don't exist)
ARG login=sabre
ARG uid=1000
RUN id -u $uid &>/dev/null || adduser --system --uid $uid --group $login

# Set entrypoint
RUN echo "#!/bin/bash\nexec \"\$@\"" > /entrypoint.sh && chmod +x /entrypoint.sh
USER $uid
ENTRYPOINT ["/entrypoint.sh"]
