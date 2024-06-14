# Maintainer: Kevin Kim <chaeya@gmail.com>

pkgname=hamonikr-albert
pkgver=0.23.0.hamonikr1.1.g805a213
pkgrel=1
pkgdesc="Program launcher for HamoniKR"
arch=('x86_64')
url="https://github.com/hamonikr/hamonikr-albert"
license=('GPL3')
depends=(
    'qt6-base'
    'qt6-declarative'
    'qt6-svg'
    'qt6-quick3d'
    'qt6-wayland'
    'python'
    'python-pip'
    'python-pyqt6'
    'libx11'
    'libxext'
    'libqalculate'
)
makedepends=(
    'cmake'
    'gcc'
    'libarchive'
    'libqalculate'
    'make'
    'muparser'
    'pybind11'
    'python'
    'pkgconf'
    'qt6-base'
    'qt6-declarative'
    'qt6-scxml'
    'qt6-svg'
    'qt6-tools'
)
source=("git+$url.git")
md5sums=('SKIP')

pkgver() {
    cd "$srcdir/$pkgname"
    git describe --tags | sed 's/^v//;s/-/./g'
}

build() {
    cd "$srcdir/$pkgname"
    cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
    cmake --build build
}

package() {
    cd "$srcdir/$pkgname"
    DESTDIR="$pkgdir" cmake --install build
}

# vim:set ts=2 sw=2 et:
