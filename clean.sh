#!/bin/sh

make clean
rm -f config.status config.cache config.log configure Makefile.in Makefile aclocal.m4 confdefs.h
rm -f gtk-synce.spec
rm -f build_rpm.sh
rm -f build_tgz.sh
rm -f src/Makefile.in
rm -f src/Makefile
rm -fr src/.deps
rm -fr autom4te*

