#!/bin/sh

if [ "$1" = "r" ]; then
  premake5 gmake2 && make -j$(nproc) -C build config=release
else
  premake5 gmake2 && make -j$(nproc) -C build config=debug
fi
