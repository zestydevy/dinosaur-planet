FROM ubuntu:20.04

RUN apt-get update \
    && apt-get install -y build-essential git binutils-mips-linux-gnu python3 python3-pip

RUN adduser sabre --system
USER sabre
COPY ./requirements.txt requirements.txt
RUN pip3 install -r requirements.txt --user

