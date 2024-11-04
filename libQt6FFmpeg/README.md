 #   FFmpegPlayer 视频播放器模块 

# Native compilation on ARM/ARM64 host

# Build MPP
mkdir -p ~/dev && cd ~/dev
git clone -b jellyfin-mpp --depth=1 https://github.com/nyanmisaka/mpp.git rkmpp
pushd rkmpp
mkdir rkmpp_build
pushd rkmpp_build
cmake \
    -DCMAKE_INSTALL_PREFIX=/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_TEST=OFF \
    ..
make -j $(nproc)
make install


# Build RGA
mkdir -p ~/dev && cd ~/dev
git clone -b jellyfin-rga --depth=1 https://github.com/nyanmisaka/rk-mirrors.git rkrga
meson setup rkrga rkrga_build \
    --prefix=/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg \
    --libdir=lib \
    --buildtype=release \
    --default-library=shared \
    -Dcpp_args=-fpermissive \
    -Dlibdrm=false \
    -Dlibrga_demo=false
meson configure rkrga_build
ninja -C rkrga_build install

# Build the minimal x264 (You can customize the configure and install prefix)
git clone https://code.videolan.org/videolan/x264.git
../configure \
    --prefix=/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg \
    --disable-cli \
    --disable-asm \
    --enable-shared=no \
    --enable-static
make -j $(nproc) 
make install

# Build the minimal FFmpeg (You can customize the configure and install prefix)
mkdir -p ~/dev && cd ~/dev
git clone --depth=1 https://github.com/nyanmisaka/ffmpeg-rockchip.git ffmpeg
cd ffmpeg
./configure --prefix=/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg \
--enable-gpl \
--enable-version3 \
--enable-libdrm \
--enable-rkmpp \
--enable-rkrga \
--enable-static \
--enable-libx264 \
--enable-openssl \
--pkg-config-flags="--static" \
--extra-cflags="-std=c11 -I/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg/include" \
--extra-ldflags="-L/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg/lib" \
--extra-libs="-lpthread -lm" \
--enable-static \
--disable-shared \
..
make -j $(nproc)

# Try the compiled FFmpeg without installation
./ffmpeg -decoders | grep rkmpp
./ffmpeg -encoders | grep rkmpp
./ffmpeg -filters | grep rkrga

# Install FFmpeg to the prefix path
make install
```
F & Q
./config --prefix=/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg
ERROR: OpenSSL <3.0.0 is incompatible with the gpl

linaro@linaro-alip:~/Downloads/ffmpeg-rockchip-master$ ./build_aarch64_linux.sh 
ERROR: x264 not found using pkg-config
vim /etc/profile
export PKG_CONFIG_PATH=/home/linaro/Project/Qt6FFmpeg/libQt6FFmpeg/lib/pkgconfig
source /etc/profile
