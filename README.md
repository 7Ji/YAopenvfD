# **Y**et **A**nother **openvf**d **D**aemon

An alternative to `OpenVFDService`, which is provided as the official service daemon in the [linux_openvfd] project.

This project is aimed at providing distro-independent single-binary feature-equivalent experience to the official `OpenVFDService` + CoreELEC-only Kodi addon combination. 

Most of the new report features are added to make it more headless-server-targeted than media-center-targeted.

## Example
 - Dislay the time in 24h format
    ```
    ./YAopenvfD 0:date:24h
    ```
 - Display the temperature of thermal zone 0 (usually CPU)
    ```
    ./YAopenvfD 0:temp:0
    ```
 - Loop through the following types of displays:
   - Display the cpu load for 5 seconds
   - Display the temperature for thermal zone 0 for 5 seconds
   - Display the clock in 24h format for 5 seconds
   - Display the clock in 12h format for 5 seconds
   - Display the read speed of `mmcblk2` for 5 seconds
   - Display the write speed of `mmcblk2` for 5 seconds
   - Display the transmit speed on interface `eth0` for 3 seconds
   - Display the receive speed on interface `eth0` for 3 seconds
   - Display the string `Enjoy the fun!` in scrolling style for 10 seconds
    ```
    ./YAopenvfD 5:cpu 5:temp:0 5:date:24h 5:date:12h 5:io:mmcblk2:r 5:io:mmcblk2:w 3:eth0:t 3:eth0:r '10:string:Enjoy the fun!'
    ```
 - Loop through the following types of displays:
    - Display the temperature for thermal zone 0 for 5 seconds
    - Display a unhappy face for 3 seconds
    - Display the time in 24h format for 5 seconds
    - Display the time in 12h format for 5 seconds
    - Display a smile face for 3 seconds
    ```
    ./YAopenvfD 5:temp:0 3:string:-a- 5:time:24 5:time:12 3:string:mvm
    ```
 - Loop through the following types of displays:
   - Display the year for 1 second
   - Display the month and day in MMDD style for 1 second
   - Display the week date in `[TEXT][NUMBER]` style (e.g. `TUE2`, `SUN0`, `WED3`) for 1 second
   - Display the time in 24h format, keep the led dot for `alarm` on (for easier identification between the 24h and 12h), for 10 seconds
   - Display the time in 12h format, for 3 seconds
   - Display the temperature for thermal zone 0 for 3 seconds
   - Display the CPU load for 3 seconds
   - Display the mixed I/O speed on block device `mmcblk2`, keep the led dot for `usb` on (for easier identification between I/O and net), for 5 seconds
   - Display the mixed Tx/Rx speed on network interface `end0` keep the led dot for `eth` on (for easier identification between I/O and net), for 5 seconds
   - Display the static text `HK1` for 3 seconds
    ```
    ./YAopenvfD 1:date:year 1:date:month_day 1:date:weekday 10@alarm:date:24h 3:date:12h 3:temp:0 3:cpu 5@usb:io:mmcblk2:m 5@eth:net:end0:m 3:string:'HK1'
    ```





[linux_openvfd]: https://github.com/arthur-liberman/linux_openvfd