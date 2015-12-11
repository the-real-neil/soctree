# github.com/rubicks/soctree/Dockerfile

from alpine
maintainer rubicks

run \
  apk update  && \
  apk upgrade && \
  apk add     \
    autoconf  \
    automake  \
    coreutils \
    file      \
    g++       \
    git       \
    libtool   \
    make      \
    man       \
    tar       \
    xz        \
  && >/stamp.txt date -uIs

env WORKDIR=/soctree

workdir ${WORKDIR}

add . .

run \
  cd $(mktemp -d) && \
  autoreconf -ivf ${WORKDIR} && \
  ${WORKDIR}/configure && \
  make && \
  make check && \
  make distcheck && \
  make install

cmd /bin/sh
