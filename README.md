# **Y**et **A**nother **openvf**d **D**aemon

An alternative to `OpenVFDService`, which is provided as the official service daemon in the [linux_openvfd] project.

This project is aimed at providing distro-independent single-binary feature-equivalent experience to the official `OpenVFDService` + CoreELEC-only Kodi addon combination. 

Most of the new report features are added to make it more headless-server-targeted than media-center-targeted.

## Example
 - Dislay the clock in 24h format forever
    ```
    ./YAopenvfD 0:clock:24
    ```
 - Display the temperature of thermal zone 0 (usually CPU) forever
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
    ./YAopenvfD 5:cpu 5:temp:0 5:clock:24 5:clock:12 5:io:mmcblk2:r 5:io:mmcblk2:w 3:eth0:t 3:eth0:r '10:string:Enjoy the fun!'
    ```





[linux_openvfd]: https://github.com/arthur-liberman/linux_openvfd