#!/bin/bash -ex
NCORES=$(($(grep -c ^processor /proc/cpuinfo) + 1))
cd / 
mkdir boost
cd boost
wget --no-check-certificate 'https://sourceforge.net/projects/boost/files/boost/1.76.0/boost_1_76_0.tar.bz2' 
tar xf boost_1_76_0.tar.bz2 
./bootstrap.sh --with-libraries=system,thread,program_options
./b2 -j${NCORES} cxxflags="-fPIC" runtime-link=static variant=release link=static install
rm -rf boost_1_76_0.tar.bz2