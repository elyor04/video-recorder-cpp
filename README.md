# hikvision-ip-camera-video-recorder (c++)
## for linux 64-bit

```
sudo apt-get -y install intel-opencl-icd ocl-icd-opencl-dev clinfo git autoconf automake build-essential cmake git-core libass-dev libfreetype6-dev libgnutls28-dev libmp3lame-dev libsdl2-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev libxcb-xfixes0-dev meson ninja-build pkg-config texinfo wget yasm zlib1g-dev nasm libx264-dev libx265-dev libnuma-dev libvpx-dev libfdk-aac-dev libopus-dev libdav1d-dev
```
```
clinfo -l
```
```
mkdir opencv-ffmpeg
```
```
cd opencv-ffmpeg
```
```
git clone https://github.com/opencv/opencv.git
```
```
git clone https://github.com/FFmpeg/FFmpeg.git
```
```
cd FFmpeg
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
cd ..
```
```
mkdir build
```
```
cd build
```
```
cmake -DWITH_OPENCL=ON ../opencv
```
```
make -j$(nproc)
```
```
sudo make install
```
