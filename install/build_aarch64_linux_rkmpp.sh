#/bin/sh
# Build MPP
git clone -b jellyfin-mpp --depth=1 https://github.com/nyanmisaka/mpp.git rkmpp
pushd rkmpp
mkdir rkmpp_build
pushd rkmpp_build
cmake \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_TEST=OFF \
    ..
make -j $(nproc)
sudo make install
