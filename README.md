<!--
YAopenvfD/README.md: project introduction
Copyright (C) 2023 Guoxin "7Ji" Pu <pugokushin@gmail.com>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
-->

# **Y**et **A**nother **openvf**d **D**aemon

An alternative daemon, replacing `OpenVFDService` which is provided as the official service daemon in the [linux_openvfd] project.

This project is aimed at providing distro-independent single-binary feature-equivalent experience to the official `OpenVFDService` + `service.openvfd` Kodi addon combination. 

Most of the new report features are added to make it more headless-server-targeted than media-center-targeted.

## Kernel dependency
This is only a userspace daemon, it depends on `openvfd` kernel module (available [here][linux_openvfd]) to provide `/dev/openvfd` to operate on, and an existing `openvfd` node in your tree. 

Currently it seems all flippy's and ophub's kernels come with this module built in tree, and corresponding node existing, e.g. [linux-aarch64-flippy-git](https://github.com/7Ji-PKGBUILDs/linux-aarch64-flippy-git). My kernel package [linux-aarch64-7ji](https://github.com/7Ji-PKGBUILDs/linux-aarch64-7ji) only has the openvfd module built as a module, but does not have `openvfd` node implanted in tree.

To verify if `openvfd` is availabel as a module for your current kernel:
```
modinfo openvfd
```

To verify if `openvfd` node existing in your DTB, check if the following path exists:
```
ls /proc/device-tree/openvfd
```
If you're using a kernel with `openvfd` built as a module, but without the `openvfd` node, you can still use this, by applying `meson-enable-openvfd.dtbo` overlay from my [device trees collection](https://github.com/7Ji/device-trees), the following is an example modified `extlinux.conf`:
```
MENU TITLE Select the boot target
TIMEOUT 10
DEFAULT alarm-7ji
  LABEL  alarm-flippy
  LINUX  /boot/vmlinuz-linux-aarch64-flippy
  INITRD /boot/booster-linux-aarch64-flippy.img
  FDTDIR /boot/dtbs/linux-aarch64-flippy
  APPEND root=UUID=9ad19ba4-1c29-4e5c-a82c-fbce457e7442 rootflags=compress=zstd:3,subvol=@ rw ignore_loglevel

  LABEL  alarm-7ji
  LINUX  /boot/vmlinuz-linux-aarch64-7ji
  INITRD /boot/booster-linux-aarch64-7ji.img
  FDTDIR /boot/dtbs/linux-aarch64-7ji
  FDTOVERLAYS /boot/dtbs/overlay/meson-enable-openvfd.dtbo
  APPEND root=UUID=9ad19ba4-1c29-4e5c-a82c-fbce457e7442 rootflags=compress=zstd:3,subvol=@ rw console=ttyAML0,115200n8 ignore_loglevel
```
(Note both kernels come with the module built, but only `flippy` kernel's DTB comes with the node, so an overlay is needed for `7ji` kernel)

## Usage
Jump to [Example](#example) for some quick examples
### Command syntax
```
YAopenvfD [reporter] ([reporter] ([reporter] (...))) [watcher] ([watcher] ([watcher])) (--dots-order [dots-order]) (--help) (--version)
```

 - `[reporter]`
   - Define a reporter that should be displayed for a certain duration, during which optionally keep a dot on
   - Syntax: `[duration](@[dot]):[type](:[definer])`
     - `[duration]`
       - duration of this reporter, in second, non-negative (0 for forever)
     - `[dot]`
       - name of the dot that should be kept on during this reporter (e.g. usb, eth, etc)
     - `[type]`
       - one of the following (and corresponding definer):
         - `string`
           - display string in definer, scroll if too long
         - `temp`
           - display temp in thermal zone N defined in definer (0 is usually for CPU), in celcius
         - `io`
            - display a type of I/O speed of block device defined in definer
            - `[io definer]`: `[block device]:[type]`
              - `[block device]`: the name of block device, e.g. mmcblk2
              - `[type]`: r or R (for read), w or W (for write), m or M (for mixed)
         - `cpu`
           - display whole CPU load, or optionally for a specific core no. in definer (0 to 3 for a 4-core CPU) if valid
         - `net`
            - display a type of network TX/RX on a specific interface
            - `[net definer]`: `[interface]:[type]`
              - `[interface]`: the name of block device, e.g. mmcblk2
              - `[type]`: t or T (for Tx/transmit), r or R (for Rx/receive)
         - `date`
           - display date/time in one of the following formats as defined in definer:
             - `24h`: time in hh:mm style, 24h (e.g. 12:34), with blinking colon
             - `12h`: time in hh:mm style, 12h (e.g. 11:59), with blinking colon
             - `year`: year in yyyy style (e.g. 2023)
             - `year_month`: year and month in yymm style (e.g. 2303)
             - `month_year`: month and year in mmyy style (e.g. 0323)
             - `month_day`: month and day in yydd style (e.g. 0307)
             - `day_month`: day and month in ddyy style (e.g. 0703)
             - `weekday`: weekday in [text][number] style (e.g. SUN0, MON1, etc)
  - `[watcher]`:   
    - define a watcher that turn on/off a dot on hotplug
    - Syntax: `@[dot]:[type]:[definer]`
      - `[dot]`: same definition as in [reporter]
      - `[type]`: one of the following (and corresponding definer):
        - `file`: watch for the 'file' (could be non-regular file) defined in definer that could appear/disappear, definer e.g. `/tmp/myfile`, `/var/lock/mysql.lock`, etc
        - `dev`: short hand for file under `/dev`, definer e.g. `sda`, `disk/by-id/nvme-eui.e8238fa6bf530001001b448b498949f8`, etc
        - `sys`: short hand for file under `/sys`, definer e.g. `class/input/mouse0`, etc
        - `net_carrier`: watch for carrier status of a network interface, definer e.g. `eth0`, `wlan0`, etc
  - `--dots-order [dots-order]`
    - rewrite the bit order of led dots
    - `[dots-order]`: a comma (,) seperated list of numbers 0-6, same format as OpenVFD kernel module's vfd_dot_bits argument, default is 0,1,2,3,4,5,6
      - It's recommended to load OpenVFD kernel module and start YAopenvfD in one go, so you can re-use the module's `vfd_dot_bits` argument for YAopenvfD's `dots-order`
  - `--help`: print a help message and early quit
  - `--version`: print the version and early quit


## Example
 - A simple one, dislay the time in 24h format, with blinking colon
    ```
    ./YAopenvfD 0:date:24h
    ```
 - Another simple one, display the temperature of thermal zone 0 (usually CPU)
    ```
    ./YAopenvfD 0:temp:0
    ```
 - A more complext one that does not involve LED dots:
    ```
    ./YAopenvfD 5:temp:0 3:string:-a- 5:date:24h 5:date:12h 3:string:mvm
    ```
    This loops endlessly as follows:
    - Display the temperature for thermal zone 0 for 5 seconds
    - Display an unhappy face for 3 seconds
    - Display the time in 24h format with blinking colon for 5 seconds
    - Display the time in 12h format with blinking colon for 5 seconds
    - Display a smile face for 3 seconds
 - A yet more complex one that does not involve LED dots
    ```
    ./YAopenvfD 5:cpu 5:temp:0 5:date:24h 5:date:12h 5:io:mmcblk2:r 5:io:mmcblk2:w 3:eth0:t 3:eth0:r '10:string:Enjoy the fun!'
    ```
   This Loops endlessly as follows:
   - Display the cpu load for 5 seconds
   - Display the temperature for thermal zone 0 for 5 seconds
   - Display the clock in 24h format for 5 seconds
   - Display the clock in 12h format for 5 seconds
   - Display the read speed of `mmcblk2` for 5 seconds
   - Display the write speed of `mmcblk2` for 5 seconds
   - Display the transmit speed on interface `eth0` for 3 seconds
   - Display the receive speed on interface `eth0` for 3 seconds
   - Display the string `Enjoy the fun!` in scrolling style for 10 seconds
 - A more advanced one, that involved LED dots to make it easier for user to tell which one is displayed
    ```
    ./YAopenvfD 1:date:year 1:date:month_day 1:date:weekday 10@alarm:date:24h 3:date:12h 3:temp:0 3:cpu 5@usb:io:mmcblk2:m 5@eth:net:end0:m 3:string:'HK1'
    ```
   This Loops endlessly as follows:
   - Display the year for 1 second
   - Display the month and day in MMDD style for 1 second
   - Display the week date in `[TEXT][NUMBER]` style (e.g. `TUE2`, `SUN0`, `WED3`) for 1 second
   - Display the time in 24h format with blinking colon, keep the led dot for `alarm` on, for 10 seconds
   - Display the time in 12h format with blinking colon, for 3 seconds
   - Display the temperature for thermal zone 0 for 3 seconds
   - Display the CPU load for 3 seconds
   - Display the mixed I/O speed on block device `mmcblk2`, keep the led dot for `usb` on, for 5 seconds
   - Display the mixed Tx/Rx speed on network interface `end0` keep the led dot for `eth` on, for 5 seconds
   - Display the static text `HK1` for 3 seconds
 - My personal setup
    ```
    ./YAopenvfD 3:date:year 3:date:month_day 3:date:weekday 10@alarm:date:24h 10:date:12h @eth:net_carrier:end0 @wifi:net_carrier:wlan0 @usb:file:/sys/bus/usb/devices/1-1 @play:dev:sda @pause:dev:ttyUSB0 --dots-order 0,1,3,2,4,5,6
    ```
    - The dots order is overwritten to `0,1,3,2,4,5,6`, to fix the swapped order of LED dots `play` and `pause`
    - This loops endlessly as follows :
      - Display the year for 3 seconds
      - Display the month and day in MMDD style for 3 seconds
      - Display the weekday in `[TEXT][NUMBER]` style (e.g. `TUE2`) for 3 seconds
      - Display the time in 24h format with blinking colon, while keep the led dot for `alarm` on, for 10 seconds
      - Display the time in 12h format with blinking colon, for 10 seconds
    - It also watches for the following hotplug events:
      - Turn on LED dot `eth` if network interface `end0` has carrier, and turn off if not
      - Turn on LED dot `wifi` if network interface `wlan0` has carrier, and turn off if not
      - Turn on LED dot `usb` if sysfs virtule folder `/sys/bus/usb/devices/1-1` exists, and turn off if not (This is the first USB device under the USB3.0 root hub for my box, so it essentially watches for hotplug on the USB3.0 port)
      - Turn on LED dot `play` if special file `sda` exists under `/dev` and off if not (This is the first external block device since the box has no native SCSI block device, it essentialy watches for external USB drive)
      - Turn on LED dot `pause` if special file `ttyUSB0` exists under `/dev` and off if not (This is the first USB serial device)
    
## Download
You can download release build on [release page](../../releases)

You can also download staticly linked binaries automatically built on each push, just click the green `√` the top middle of this page. **These are development builds and there might be dangerous**

An official AUR package [YAopenvfD](https://aur.archlinux.org/packages/yaopenvfd) is also available to install with your favorite AUR helper on Arch Linux, prebuilt binary packages for x86_64 and aarch64 are also also available from my [archrepo](https://github.com/7Ji/archrepo)

## Build
Just clone and repo run `make`
 1. Clone the repo
    ```
    git clone https://github.com/7Ji/YAopenvfD
    ```
 2. Enter the folder
    ```
    cd YAopenvfD
    ```
 3. Build the binary
    ```
    make
    ```

## Install
To install to a psuedo root (useful for a build system):
```
make DESTDIR=some/target/root install
```
To install to your current root:
```
sudo make install
```

## Systemd integration
If you use the official AUR package, or install manually by `sudo make install`, you would install some pre-configured systemd units to your root, use them as follows:
- Configure module parameters in `/etc/conf.d/YAopenvfD`, e.g.
  ```
  vfd_gpio_clk=0,64,0
  vfd_gpio_dat=0,63,0
  vfd_gpio_stb=0,0,0xFF
  vfd_gpio0=0,0,0xFF
  vfd_gpio1=0,0,0xFF
  vfd_gpio2=0,0,0xFF
  vfd_gpio3=0,0,0xFF
  vfd_gpio_protocol=0,0
  vfd_chars=0,4,3,2,1
  vfd_dot_bits=0,1,3,2,4,5,6
  vfd_display_type=0x00,0x00,0x00,0x06
  YAopenvfD_args=--dots-order 0,1,3,2,4,5,6 10@alarm:date:24h 10:date:12h 10@usb:io:mmcblk1:m 10@wifi:net:end0:m 3:cpu 3:temp:0 @eth:net_carrier:end0
  ```
- Start `YAopenvfD-updater.service` to check if the configuration works properly:
  ```
  sudo systemctl start YAopenvfD-updater.service
  ```
- If you would like to debug the module parameters, restart the modloader service:
  ```
  sudo systemctl restart YAopenvfD-modloader.service
  ```
  _This would also restart the updater service if it's running_
- If you would like to debug the daemon arguments, restart the updater service:
  ```
  sudo systemctl restart YAopenvfD-updater.service
  ```
- Enable `YAopenvfD-updater.service` if everything works perfectly to make it auto started on boot:
  ```
  sudo systemctl enable YAopenvfD-updater.service
  ```

## License
**YAopenvfD** (**Y**et **A**nother **openvf**d **D**aemon) is licensed under [**GPL3**](https://gnu.org/licenses/gpl.html)
 * Copyright (C) 2023 Guoxin "7Ji" Pu (pugokushin@gmail.com)
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.


[linux_openvfd]: https://github.com/arthur-liberman/linux_openvfd