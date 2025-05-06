#
# Copyright (c) 2022 Wind River Systems, Inc.
#

MACHINEOVERRIDES:prepend = "amd-x86:"

MACHINE_FEATURES += "x86 pci usbhost ext2 keyboard screen alsa acpi vfat serial"
MACHINE_FEATURES += "efi pcbios"
MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS += "linux-firmware-radeon"
MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS += "linux-firmware-amdgpu"

EFI_PROVIDER ?= "${@bb.utils.contains('INCOMPATIBLE_LICENSE', 'GPL-3.0-only', 'systemd-boot', 'grub-efi', d)}"

# grub-efi is depended in wic.bbclass, remove it to aviod depency error.
WKS_FILE_DEPENDS_BOOTLOADERS:remove = "${@bb.utils.contains('INCOMPATIBLE_LICENSE', 'GPLv3', 'grub-efi', '', d)}"

# Do not include installer routine when GPLv3 is not allowed.
INITRAMFS_SCRIPTS:remove = "${@bb.utils.contains('INCOMPATIBLE_LICENSE', 'GPL-3.0-only', 'initramfs-module-install initramfs-module-install-efi', '', d)}"
LABELS_LIVE:remove = "${@bb.utils.contains('INCOMPATIBLE_LICENSE', 'GPL-3.0-only', 'install', '', d)}"

KERNEL_IMAGETYPE = "bzImage"

XSERVER ?= "xserver-xorg \
           xserver-xorg-extension-dri \
           xserver-xorg-extension-dri2 \
           xserver-xorg-extension-glx \
           xserver-xorg-extension-extmod \
           xserver-xorg-extension-dbe \
           xserver-xorg-module-libint10 \
           xf86-input-mouse \
           xf86-input-evdev \
           xf86-input-synaptics \
           xf86-video-vesa \
           xf86-video-fbdev \
           xf86-video-intel \
           xf86-video-mga \
           xf86-video-modesetting \
           xf86-video-ati \
           mesa \
           mesa-driver-swrast \
           mesa-demos \
           matchbox-wm"

PREFERRED_PROVIDER_libglu = "libglu"
PREFERRED_PROVIDER_libdrm = "libdrm"

PREFERRED_PROVIDER_virtual/kernel ??= "linux-yocto"
PREFERRED_VERSION_linux-yocto ?= "6.12%"
PREFERRED_VERSION_linux-yocto-rt ?= "6.12%"

IMAGE_FSTYPES += "wic wic.bmap live"
WKS_FILE ?= "${@bb.utils.contains('INCOMPATIBLE_LICENSE', 'GPLv3', 'systemd-bootdisk.wks', 'mkefidisk.wks', d)}"
do_image_wic[depends] += "gptfdisk-native:do_populate_sysroot"
do_image_wic[recrdeptask] += "do_bootimg"

# Vala api requires gobject introspection which depends on qemu-usermode.
# But qemu-usermode is removed in this BSP since it is not compatible with some AMD ISAs.
PACKAGECONFIG:remove:pn-networkmanager = "vala"

# DPDK enable and configuration
COMPATIBLE_MACHINE:amd-x86:pn-dpdk = "${MACHINE}"
COMPATIBLE_MACHINE:amd-x86:pn-dpdk-module = "${MACHINE}"
