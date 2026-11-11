#!/bin/bash
set -e -x

# ur_rtde only uses header-only parts of Boost, so installing the headers is
# sufficient. This script runs inside the manylinux container on cibuildwheel.
BOOST_VERSION=1.90.0
BOOST_UNDERSCORE=1_90_0

curl -L \
  "https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_UNDERSCORE}.tar.bz2" \
  -o "/tmp/boost_${BOOST_UNDERSCORE}.tar.bz2"
tar -xjf "/tmp/boost_${BOOST_UNDERSCORE}.tar.bz2" -C /tmp
cp -r "/tmp/boost_${BOOST_UNDERSCORE}/boost" /usr/local/include/
