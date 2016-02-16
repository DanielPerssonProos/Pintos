#!/bin/bash

echo Loading Script...

echo Script Loaded!

export PATH="${HOME}/TDDB68/Pintos/linuxpintos/src/utils/:${PATH}/"
pintos --qemu -- -f -q

echo Script Done!
