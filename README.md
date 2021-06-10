# PixelCore

*Yet Another Crappy Image Editor...*

## Requirements

 * **Qt** *(5.12, 5.15 will probably be a requirement at some point)*
   * *Core, Gui, Widgets, Concurrent, Test*
 * **Little CMS** *(2+)*
 * **OpenImageIO** *(2.1+)*
 * **OpenMP**
 * **CMake**

 ### macOS

Make sure you have the latest version of [MacPorts](https://macports.org) installed and are running macOS 10.12 (Sierra) or higher.

```
sudo port install qt5 cmake lcms2 openimageio clang-11
```

```
git clone https://github.com/rodlie/pixelcore
cd pixelcore
mkdir build && cd build
CC=/opt/local/bin/clang-mp-11 CXX=/opt/local/bin/clang++-mp-11 cmake .. && make
./pixelcore
```

 ### Ubuntu

 Make sure you have Ubuntu 20.04 or higher installed.

 ```
 sudo apt-get install cmake qt5-default qtbase5-dev qt5-image-formats-plugins libopenimageio-dev liblcms2-dev
 ```

```
git clone https://github.com/rodlie/pixelcore
cd pixelcore
mkdir build && cd build
cmake .. && make
./pixelcore
```
