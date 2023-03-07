# snimg - create an sn.img file for Lenovo device
---
## Description
Some OEMs (like Lenovo) limit a user's ability to enable bootloader unlocking.  Sometimes a small file called *sn.img* is required to enable bootloader unlocking.  Lenovo does not always make this file available for all their devices.

To enable unlocking, the command is usually:  `fastboot flash unlock sn.img`

The bootloader (*LinuxLoader* aka *abl.elf*) takes the sn.img file and passes it to *VBVerifyImage( )* for authentication.
It fibs to *VBVerifyImage( )* and tells it that this is a 0x1000-byte boot image.

If successful, this authorises the command `fastboot oem unlock-go` to unlock the bootloader.

**NOTE**: not all Android bootloaders issued by these OEMs contain the "fastboot flash unlock" or "fastboot oem unlock-go" commands.  So this sn.img file will be useless in that case.

See *snimg.h* for information on the format of the *sn.img* file.

---
## How to use sn.img
```
usage:     snimg <serialno>

               serialno - serial number of device (mandatory), eg H0H0HEHE
```
---
## How to build snimg
This program uses [CMake](https://cmake.org/) and [OpenSSL](https://www.openssl.org/).

- [Prereqs for Windows](#prereqs-for-windows)
- [Prereqs for Linux](#prereqs-for-linux)
- [Download source](#download-source)
- [Compile the source](#compile-the-source)

### Prereqs for Windows
1. [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) or later.  Community Edition is ok.
1. in Visual Studio, install **Visual Studio 2022 (v143)** or later as the *Platform Toolset*
1. download and install latest [CMake for Windows](https://cmake.org/download/).  It's easiest to use the *.msi installer.
1. download and install OpenSSL in *c:\Program Files\OpenSSL*.  It's easiest to install the [precompiled headers](https://github.com/CristiFati/Prebuilt-Binaries/tree/master/OpenSSL).

### Prereqs for Linux
1. GCC 7.5.0 or later
1. OpenSSL 1.1.1.  Usually pre-installed on Linux (type: ```openssl version```).  If not installed, visit: [OpenSSL's github repo](https://github.com/openssl/openssl) for instructions.
1. CMake 3.25.1 or later
```
    wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
    tar -zxvf cmake-3.25.1.tar.gz
    cd cmake-3.25.1
    ./bootstrap
    make
    sudo make install
```
### Download source
```
    git clone https://github.com/Yahoo-Mike/snimg.git
```
### Build the source
```
    cd snimg                                    # change to base directory
    cmake -S . -B build                         # configure the project
    cmake --build build                         # build the project
```    
