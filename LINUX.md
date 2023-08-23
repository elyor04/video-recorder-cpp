## Don't Pay Attention (not ready)

Install dependencies
```
sudo apt-get -y install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python3-dev python3-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libx264-dev && sudo apt-get -y install ffmpeg python3-pip
```

Get OpenCV from the official repository
```
git clone https://github.com/opencv/opencv.git && cd opencv
```

Prepare for the build
```
mkdir build && cd build
```

Run cmake with OpenCL enabled
```
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_OPENCL=ON -D WITH_FFMPEG=ON -D ENABLE_FAST_MATH=1 -D WITH_QT=ON -D BUILD_opencv_python3=ON ..
```

Build and install OpenCV
```
make -j$(nproc) && sudo make install && cd ~
```

Verify OpenCV Installation
```
python3 -c "import cv2; print(cv2.__version__)"
```


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
