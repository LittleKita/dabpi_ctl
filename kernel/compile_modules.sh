#!/bin/bash

cd rpi-dabpi
make -C /lib/modules/$(uname -r)/build M=$(pwd) modules || exit 1
cd ..

cd si468x
make -C /lib/modules/$(uname -r)/build M=$(pwd) modules || exit 1
cd ..

