# Kbuild Learning Subsystem – Experiments & Observations

This document records the hands-on experiments performed to understand the
Linux kernel build system (Kconfig + Kbuild) in depth.  
Each experiment focuses on a specific build-system concept and captures
observations relevant to real-world kernel development.

---

## Experiment 1: Module vs Built-in Build

### 1. Objective
Understand how a single Kconfig symbol controls whether code is built
as a loadable module or built directly into the kernel.

### 2. Configuration
```text
CONFIG_KBUILD_LAB=m
```

### 3. Build Command
```bash
make M=drivers/kbuild_lab modules
```
### 4. Observation

A loadable module `kbuild_lab.ko` is generated.

The module can be inserted and removed dynamically using `insmod` / `rmmod`.

Initialization happens via `module_init()`.

### 5. Runtime Verification
``` bash
sudo insmod kbuild_lab.ko
dmesg | tail
sudo rmmod kbuild_lab
```

### 6. Key Learning

`CONFIG_KBUILD_LAB=m` expands `obj-$(CONFIG_KBUILD_LAB)` to `obj-m`

Module code is linked separately and loaded at runtime

## Experiment 2: Built-in Kernel Object

### 1. Objective

Understand how the same code behaves when built into the kernel image.

Configuration
```bash
CONFIG_KBUILD_LAB=y
```

### 2. Build & Boot

Kernel rebuilt and booted with the updated configuration

### 3. Observation

No `.ko` file is generated

The driver initializes automatically during kernel boot

Logs appear without running `insmod`

### 4. Key Learning

Built-in objects are linked into `vmlinux`

Initialization happens via kernel initcall mechanism

Built-in code cannot be unloaded

## Experiment 3: Conditional Compilation using Kconfig
### 1. Objective

Verify how `CONFIG_*` symbols control compilation, not runtime behavior.

### 2. Configuration
```bash
CONFIG_KBUILD_LAB_DEBUG=y
```
### 2. Code Path
```bash
#ifdef CONFIG_KBUILD_LAB_DEBUG
    pr_info("debug enabled\n");
#endif
```
### 3. Observation

Debug messages appear only when the config is enabled

When disabled, debug code is completely removed from the binary

### 4. Key Learning

Kconfig enables compile-time feature selection

No runtime overhead when a feature is disabled

## Experiment 4: Multi-Object Module Linking
### 1. Objective

Understand how multiple `.c` files are combined into a single kernel object.

### 2. Makefile Logic
```bash
kbuild_lab-y := core.o helper.o
```
### 3. Observation

`core.c` and `helper.c` compile into separate `.o` files

They are linked into a single `kbuild_lab.o`

Final output is either `kbuild_lab.ko` or `built-in` object

### 4. Key Learning

Kbuild supports logical grouping of multiple objects

This mirrors real kernel subsystems and complex drivers

