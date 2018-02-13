FROM centos:7

# Version informations
MAINTAINER Gaelle Letort <physiboss@gmail.com>
LABEL version="1.0"

## install dependencies
RUN yum update -y && yum install -y git

RUN yum groupinstall -y "Development tools"

## download PhysiBoSS srcs from GitHub
WORKDIR /home/
RUN git clone https://github.com/gletort/PhysiBoSS.git

## compile PhysiBoSS
RUN cd PhysiBoSS; make install;
# create directory where to mount local folders to run the simulation inside
RUN mkdir running

CMD [""]
