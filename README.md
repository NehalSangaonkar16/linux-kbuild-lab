# Linux Kernel Build System – Kbuild Learning Subsystem

This repository contains a **hands-on Linux kernel mini-project** created to deeply understand the **Linux kernel build system**, specifically **Kconfig** and **Kbuild**.

The goal of this project is not to build a feature-rich driver, but to understand **how the Linux kernel decides what to build, how it builds it, and how code transitions between built-in and loadable module forms**—a core skill required for kernel and device driver development in large codebases.

---

##  Objectives

- Understand **Kconfig symbol resolution and dependencies**
- Learn how **Kbuild Makefiles** control compilation
- Explore **built-in vs module** build paths
- Practice **partial kernel builds** using `make M=`
- Understand **multi-object module linking**
- Learn how to debug **kernel build failures**

---

##  Project Structure
```bash
drivers/kbuild_lab/
├── Kconfig # Kernel configuration options
├── Makefile # Kbuild logic (obj-y / obj-m)
├── core.c # Module init/exit and core logic
├── helper.c # Secondary object file
└── helper.h
```


This layout mirrors the real Linux kernel source tree and follows standard kernel development practices.

---

##  Kconfig Options

The subsystem introduces the following configuration symbols:

### `CONFIG_KBUILD_LAB`
- Type: `tristate`
- Controls whether the subsystem is:
  - Built into the kernel (`y`)
  - Built as a module (`m`)
  - Not built (`n`)

### `CONFIG_KBUILD_LAB_DEBUG`
- Type: `bool`
- Depends on `CONFIG_KBUILD_LAB`
- Enables additional debug logging
- Demonstrates conditional compilation using Kconfig symbols

These options are accessible via:

```bash
make menuconfig
→ Device Drivers
→ Kbuild Learning Subsystem
```
##  Build Instructions
Build only this subsystem as a module
```bash
make M=drivers/kbuild_lab modules
```

The make M= mechanism allows building a specific subdirectory without rebuilding the entire kernel.

##  Load and Test (Module Mode)
```bash
sudo insmod kbuild_lab.ko
dmesg | tail
sudo rmmod kbuild_lab
```

## Expected kernel log output:
```bash
kbuild_lab: init called
kbuild_lab: debug enabled
kbuild_lab: helper function executed
```
