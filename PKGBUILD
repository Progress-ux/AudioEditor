pkgname=editor
pkgver=1.0.0
pkgrel=1
pkgdesc="CLI c++ program for audio edit metadata"
arch=('x86_64')
url="https://github.com/Progress-ux/AudioEditor.git"
license=('MIT')

depends=(
    'taglib'
)

source=()
sha256sums=() 

build() {
    cmake -B "$startdir/build" -S "$startdir" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build "$startdir/build"
}

package() {
    DESTDIR="$pkgdir" cmake --install "$startdir/build"
}
