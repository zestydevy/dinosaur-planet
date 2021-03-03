FROM ubuntu:20.04

RUN apt-get -qq update \
    && apt-get -y install \
    binutils-mips-linux-gnu \
    build-essential \
    git \
    locales \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/* \
    # Set timezone to UTC by default
    && ln -sf /usr/share/zoneinfo/Etc/UTC /etc/localtime \
    # Use unicode
    && locale-gen C.UTF-8 || true

COPY ./requirements.txt requirements.txt
COPY ./entrypoint.sh entrypoint.sh
RUN adduser sabre --system \
    && chmod +x entrypoint.sh
USER sabre
RUN pip3 install -r requirements.txt --user --no-warn-script-location

ENV PATH="/home/sabre/.local/bin:${PATH}"
ENTRYPOINT ["/entrypoint.sh"]
