# hikvision-recorder-qt

## don't pay attention down below (not enough instructions yet)
https://github.com/intel/compute-runtime/releases
```
clinfo -l
```
https://medium.com/@vladakuc/compile-opencv-4-7-0-with-ffmpeg-5-compiled-from-the-source-in-ubuntu-434a0bde0ab6
```
sudo apt-get -y install autoconf automake build-essential cmake git-core libass-dev libfreetype6-dev libgnutls28-dev libmp3lame-dev libsdl2-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev libxcb-xfixes0-dev meson ninja-build pkg-config texinfo wget yasm zlib1g-dev nasm libx264-dev libx265-dev libnuma-dev libvpx-dev libfdk-aac-dev libopus-dev libdav1d-dev
```
```
./configure --enable-shared --enable-gpl --enable-libx264 --enable-libx265 --enable-libvpx --enable-zlib --enable-opencl
```
```
make -j$(nproc)
```
```
sudo make install
```
```
cmake -DWITH_OPENCL=ON ../opencv
```
