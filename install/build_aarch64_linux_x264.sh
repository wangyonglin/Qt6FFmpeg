#/bin/sh
git clone --depth=1 https://code.videolan.org/videolan/x264.git x264
mkdir ./x264_build
pushd ./x264_build
../x264/configure --prefix=/usr --enable-shared
make -j $(nproc)
sudo make install
