## Install Rust and Cargo

First of all you must install Cargo for working with different Rust source files from TVM-linker and TONOS-cli.

The easiest way to get Cargo is to install the current stable release of Rust by using rustup. Installing Rust using rustup will also install cargo. On Linux and macOS systems, this is done as follows:

```
curl https://sh.rustup.rs -sSf | sh
```

## Install or udpate gcc compiler

LLVM uses C++ 2017. You need to install gcc version 7.4 or higher (> 8.0 preferred). For Windows you can use Microsoft Visual Studio 2017 or higher.
If gcc already installed, check current gcc version by means of command

```
gcc --version
```

Or you can try to compile some simple C++ file with option -std=c++17. If you receive a warning or error, than you gcc version is incorrect for LLVM compilation.

If current version too low, you can upgrade it, for example for Ubuntu it will be a command sequence:

```
sudo apt-get update 
sudo apt-get install build-essential software-properties-common -y
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install gcc-snapshot -y
sudo apt-get update
sudo apt-get install gcc-8 g++-8 
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8
```

This sequence will update gcc to version 8.0, if you prefer a newer version, replace "8" and "80" in command lines to desirable version number.

## Install cmake and Python

Install Python version 2.7 and cmake version 3.6 or higher.

## Install zlib

You need zlib version 1.2.3.4 or higher
