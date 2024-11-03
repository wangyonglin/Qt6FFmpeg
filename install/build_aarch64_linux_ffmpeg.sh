#/bin/sh
# Build the minimal FFmpeg (You can customize the configure and install prefix)
git clone --depth=1 https://github.com/nyanmisaka/ffmpeg-rockchip.git ffmpeg
mkdir ./ffmpeg_build
pushd ./ffmpeg_build
../ffmpeg/configure --prefix=/usr --enable-gpl --enable-version3 --enable-libdrm --enable-rkmpp --enable-rkrga --enable-libx264 --enable-libx265 --enable-openssl 
make -j $(nproc)

# Try the compiled FFmpeg without installation
#./ffmpeg -decoders | grep rkmpp
#./ffmpeg -encoders | grep rkmpp
#./ffmpeg -filters | grep rkrga

# Install FFmpeg to the prefix path
sudo make install
