FROM ubuntu:22.04

LABEL maintainer="Rafael P Monteiro <rafael.p.monteiro@gmail.com>"

#Set workdir
WORKDIR /home/easy
 
#Install packages
RUN apt update 
RUN apt install wget unzip git -y
RUN apt install build-essential autoconf automake cmake gcc g++ -y

RUN apt install net-tools openssh-server -y

EXPOSE 22

ENTRYPOINT bash

# docker build -t cpp .
# docker run -v C:/Users/pdi02/Documents/GitHub/EasyRPC/C:/home/easy --network host -it cpp
# docker run -v C:/Users/pdi02/Documents/GitHub/EasyRPC/C:/home/easy -P -it cpp
