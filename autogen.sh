#!/usr/bin/env bash

# github.com/rubicks/soctree/autogen.sh

set -e

_here=$(dirname $(readlink -f ${BASH_SOURCE}))

autoreconf -ivf ${_here}
