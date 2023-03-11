# Package
These are files needed if you want to pack YAopenvfD for your distro which a convenient systemd service unit (`YAopenvfD.service`) to manage it and a single configuration file to provide versatile customization.
## Manual Installation
 - `YAopenvfD.conf` should be installed to `/etc/conf.d/YAopenvfD`
 - `YAopenvfD.service` should be installed to `/usr/lib/systemd/system/YAopenvfD.service`
 - `YAopenvfD` binary compiled with outer `Makefile` should be installed to `/usr/bin/YAopenvfD`
 - Kernel should has `openvfd` module built as loadable module, either in tree or with DKMS

## Arch Packaging
`PKGBUILD` here could be used to pack an ArchLinux package, it's also available on [AUR](https://aur.archlinux.org/packages/yaopenvfd) maintained by myself so don't re-upload it to AUR.
