# Midi Light Organ

## Install Dependencies

### ALSA

```
sudo apt install libasound2-dev
```

## Enable ALSA MIDI sequencing with Modprobe

```
sudo modprobe snd_seq
```

## Disable Raspberry Pi Audio (or the matrix library will not run)

```
cat <<EOF | sudo tee /etc/modprobe.d/blacklist-rgb-matrix.conf
blacklist snd_bcm2835
EOF

sudo update-initramfs -u
```

And then reboot to make the blacklist take effect

## Build it

### Clone it

pi@raspberrypi:~/Repos/ $ git clone https://github.com/pandamusrex/midi-light-organ.git

### Install and build Zeller in a peer folder to this repo

pi@raspberrypi:~/Repos/ $ git clone https://github.com/hzeller/rpi-rgb-led-matrix.git
pi@raspberrypi:~/Repos/ $ cd rpi-rgb-led-matrix
pi@raspberrypi:~/Repos/ $ make

### Install MagicK
sudo apt-get update
sudo apt-get install libgraphicsmagick++-dev libwebp-dev -y

### And then build this

```
pi@raspberrypi:~/Repos/ $ cd midi-light-organ
pi@raspberrypi:~/Repos/midi-light-organ/ $ make
```

## Connect the Hardware

e.g. a Casio CT-X5000 connected via USB looks like this:

```
pi@raspberrypi:~/Repos/midi-light-organ $ lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 003: ID 07cf:6803 Casio Computer Co., Ltd CTK-3500 (MIDI keyboard)
Bus 001 Device 002: ID 2109:3431 VIA Labs, Inc. Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
pi@raspberrypi:~/Repos/midi-light-organ $ aplaymidi -l
 Port    Client name                      Port name
 14:0    Midi Through                     Midi Through Port-0
 16:0    CASIO USB-MIDI                   CASIO USB-MIDI MIDI 1
```

## Do not use sudo to launch the app - ALSA gets confused

It will segment fault. Still digging into this.

```
pi@raspberrypi:~/Repos/midi-light-organ $ sudo ./midi-light-organ 
midi-light-organ
Press Ctrl-C to exit
Suggestion: to slightly improve display update, add
        isolcpus=3
at the end of /boot/cmdline.txt and reboot (see README.md)
entering MidiListener doWork
Failed to create secure directory (/root/.config/pulse): Permission denied
ALSA lib seq_hw.c:466:(snd_seq_hw_open) open /dev/snd/seq failed: Permission denied
Magick: abort due to signal 11 (SIGSEGV) "Segmentation Fault"...
Aborted
```

## Instead, run it this way (with no hardware pulse)

```
pi@raspberrypi:~/Repos/midi-light-organ $ ./midi-light-organ --led-no-hardware-pulse
midi-light-organ
Press Ctrl-C to exit
Suggestion: to slightly improve display update, add
        isolcpus=3
at the end of /boot/cmdline.txt and reboot (see README.md)
FYI: not running as root which means we can't properly control timing unless this is a real-time kernel. Expect color degradation. Consider running as root with sudo.
Can't set realtime thread priority=99: Operation not permitted.
        You are probably not running as root ?
        This will seriously mess with color stability and flicker
        of the matrix. Please run as `root` (e.g. by invoking this
        program with `sudo`), or setting the capability on this
        binary by calling
        sudo setcap 'cap_sys_nice=eip' /home/pi/Repos/midi-light-organ/midi-light-organ
entering MidiListener doWork
a MIDI event is NOT available
a MIDI event is NOT available
a MIDI event is NOT available
a MIDI event is NOT available
a MIDI event is NOT available
^Cexiting MidiListener doWork
```