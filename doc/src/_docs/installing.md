---
layout: docs
title: Installing Albert
permalink: /docs/installing/
---

There are two ways to get Albert: Using a package manager or building Albert from the sources. Using a package manager is highly recommended, since it is less error prone and the necessary dependencies are pulled automatically.

## Using package managers

### Arch Linux (and derivatives) - [AUR](https://aur.archlinux.org/packages/albert/)
There's a PKGBUILD for Arch Linux in the AUR. Explaing how to build and install packages from AUR goes beyond scope here. Refer to the related Arch Wiki sections. Note that this is an automated way to build the package from sources.

### Prebuilt binary packages - [OBS](https://build.opensuse.org/package/show/home:manuelschneid3r/albert)
These packages are built and hosted using the openSUSE Build Service. Using these is the most convenient way to get albert and stay up to date. You have to add the repo to your package manager and import the keyfile which is used to verify the packages signatures. This has to be done only once. From then on you can install and update albert like any other package on you system.

#### Importing the keyfile

For RPM based package managers:
```bash
sudo rpm --import \
  https://build.opensuse.org/projects/home:manuelschneid3r/public_key
```

For DEB based package managers:
```bash
curl https://build.opensuse.org/projects/home:manuelschneid3r/public_key | sudo apt-key add -
```

#### Adding the repo

Adding the repo to the package managers of basic distributions (Arch, Debian, Ubuntu, Fedora and openSuse) is straight forward. Just visit [the OBS software repo](https://software.opensuse.org/download.html?project=home:manuelschneid3r&package=albert) and follow the instructions there. If you are using derivatives of these distributions you have to find out which repository they are based on and proceed using this repo. E.g. [Linux Mint 19 is based on Ubuntu 18.04 repositories](https://en.wikipedia.org/wiki/Linux_Mint_version_history#Release_history).

## Building from sources

Building from sources is the least convenient, but most flexible way. The build process is trivial, but you have to manage the dependencies on your own. Before you can start building Albert you need some libraries.

### Prerequisites

The goal is to be always compatible with the latest Ubuntu LTS release. To build Albert from sources you will need CMake, a C++ compiler supporting at least the C++14 standard, and the Qt toolkit.

Further the plugins may introduce optional dependencies, e.g the calculator plugin needs the [muparser](http://beltoforion.de/article.php?a=muparser) library and the QMLBoxModel frontend needs the QtDeclarative library. Check the [docker file](https://raw.githubusercontent.com/albertlauncher/albert/dev/Dockerfile.ubuntu1804) for an up to date list of dependencies.

Problems may arise with distributions that split submodules into optional dependencies. Ubuntu is known to split the SQL driver submodules into separate packages. Additionally, Elementary OS - which builds on Ubuntu - does not install optional dependencies. Users may therefore encounter linkage errors and have to explicitly install the missing dependencies.

Another concern is the difference between compile time and runtime dependencies. Some distributions ship libraries as single packages while others ship a normal package and a *\*-dev* package. Dev packages usually contain the header files and static libraries in addition to the shared libraries. Normal packages are stripped down to the shared libraries. On distributions that do not differentiate between these kinds of packages, basically every package is a dev package. For the compilation on e.g. Ubuntu, dev packages are needed at compile time but at runtime normal packages are sufficient. If the optional dependency of a plugin is not available at runtime it will refuse to load; the core application will run fine though.

### Compilation

To configure, build and install run the following commands:
```bash
git clone --recursive https://github.com/albertlauncher/albert.git
mkdir albert-build
cd albert-build
cmake ../albert -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug
make
sudo make install
```

Lets go through them and clarify what they do. The first command clones the Albert git repository to the local file system. Since no destination directory is specified a directory with the name of the repository is created. The next step is to create the out-of-source-build directory. A self-explanatory name is always a good one.

After changing the working directory to the just created build directory the `cmake` command configures the build environment and creates the makefiles. The first positional parameter is the path to the source directory which contains a file called `CMakeLists.txt`. The `-D` parameter sets CMake variables.

The CMake variable `CMAKE_BUILD_TYPE` specifies the build type to use. If you want to report bugs it makes sense to build a `Debug` build, since the build then produces debugging information with which GDB can work. Core dumps of this build can be used to track down issues. Futher the make output is more verbose. If you dont want any of those that use the build type `Release`.

`CMAKE_INSTALL_PREFIX` defines the prefix for the installation directory. This value usually defaults to `/usr/local`. Since albert looks up libraries, plugins and stylefiles etc only `/usr`, `/usr/local` are supported. If you still want to make it work somewhere else, you have to use environment variables to manipulate XDG base dir specs and the lookup paths of the dynamic linker. Do absolutely not do this unless you exactly know what you are doing.

Finally `make` builds the application and `sudo make install` installs the application on your system. Albert is not a portable application so the install step is mandatory.
