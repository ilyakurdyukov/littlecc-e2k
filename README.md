# Elbrus 2000 (e2k) backend to LCC (Little C Compiler)

### Patching the LCC sources

    $ git clone --depth=1 https://github.com/drh/lcc lcc-src
    $ patch -N -p1 -d lcc-src < lcc-e2k.patch
    $ cp e2k.c lcc-src/src/e2k.md

### Building

    $ mkdir -p lcc-build/gcc lcc-build/include
    $ ( cd lcc-src && make BUILDDIR=../lcc-build HOSTFILE=etc/linux.c all )
    $ ln -s ../cpp lcc-build/gcc/cpp
    $ cp include/* lcc-build/include/

### Example usage

    $ export LCCDIR="$(pwd)/lcc-build"
    $ lcc-build/lcc -target=e2k/linux fannkuch.c -S -o fannkuch.s
    $ cc fannkuch.s -o fannkuch && ./fannkuch 7 8 9

