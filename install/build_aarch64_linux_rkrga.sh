#/bin/sh
# Build RGA
git clone -b jellyfin-rga --depth=1 https://github.com/nyanmisaka/rk-mirrors.git rkrga
meson setup rkrga rkrga_build \
    --prefix=/usr \
    --libdir=lib \
    --buildtype=release \
    --default-library=shared \
    -Dcpp_args=-fpermissive \
    -Dlibdrm=false \
    -Dlibrga_demo=false
meson configure rkrga_build
ninja -C rkrga_build install
