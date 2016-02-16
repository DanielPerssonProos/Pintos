#!/bin/bash

export PATH="${HOME}/TDDB68/Pintos/linuxpintos/src/utils/:${PATH}/"
pintos --qemu -- -f -q
pintos --qemu -p ../../examples/$1 -a $1 -- -q
pintos --qemu -- run $1
