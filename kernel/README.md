# Kernel modules for RPi fm/fmhd/dab receiver

## compile for current RPi-Kernel

Update to current Kernel:

```
rpi-update
```

Get current source https://github.com/notro/rpi-source/wiki:

```
sudo wget https://raw.githubusercontent.com/notro/rpi-source/master/rpi-source -O /usr/bin/rpi-source && sudo chmod +x /usr/bin/rpi-source && /usr/bin/rpi-source -q --tag-update
rpi-source
```

```
./compile_modules.sh
```

## Using the modules

```
modprobe snd-bcm2835
```

### Using with device tree:

```
insmod si468x/snd-soc-si468x.ko
insmod rpi-dabpi/snd-soc-rpi-dabpi.ko cpu_dai_name=`basename $(find /sys -name "*.i2s" -type d)`
```
