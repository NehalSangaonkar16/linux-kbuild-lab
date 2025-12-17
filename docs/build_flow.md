# Linux Kernel Build Flow (Kbuild Learning Subsystem)

## Entry Point — User Action

The build flow starts when the developer runs one of these commands:
```bash
make menuconfig
make M=drivers/kbuild_lab modules
```
Each triggers a different part of the kernel build system.

## Configuration Phase — Kconfig Resolution
### 1. Kconfig Discovery

When `make menuconfig` is invoked the top-level `Kconfig` file is parsed. It recursively includes:
```bash
drivers/Kconfig
└── drivers/kbuild_lab/Kconfig
```
The Kbuild Learning Subsystem menu becomes visible

### 2. Symbol Resolution

User selects:
```bash
CONFIG_KBUILD_LAB = m
CONFIG_KBUILD_LAB_DEBUG = y
```

Kconfig then resolves dependencies (`depends on`) in the Kconfig file, applies tristate logic and hides invalid options.

### 3. Generated Configuration Artifacts

Kconfig generates:

`.config`	Stores symbol values

`include/generated/autoconf.h`	Used by C compiler

`include/config/kbuild_lab.h`	Used by Makefiles

Example:
```bash
#define CONFIG_KBUILD_LAB 1
#define CONFIG_KBUILD_LAB_DEBUG 1
```


## Build Invocation Phase — `make M=`

Command:
```bash
make M=drivers/kbuild_lab modules
```

This tells the kernel:

“Build only this subtree, using the global configuration.”

### 1. Top-Level Makefile Processing

GNU Make parses the top-level Makefile and detects `M=drivers/kbuild_lab`

Sets:
```bash
KBUILD_EXTMOD := drivers/kbuild_lab
```

Skips unrelated directories

## Kbuild Expansion Phase — Makefile Logic
### 1. Entering `drivers/kbuild_lab/Makefile`

The Makefile contains:
```bash
obj-$(CONFIG_KBUILD_LAB) += kbuild_lab.o
kbuild_lab-y := core.o helper.o
```

### 2. Symbol Expansion

Since:
```bash
CONFIG_KBUILD_LAB = m
```

Make expands to:
```bash
obj-m += kbuild_lab.o
```

If CONFIG_KBUILD_LAB = y:
```bash
obj-y += kbuild_lab.o
```

If unset:
```bash
obj-n += kbuild_lab.o   # ignored
```

## Object Aggregation Phase
### 1. Multi-Object Linking

This line:
```bash
kbuild_lab-y := core.o helper.o
```

Instructs Kbuild to:

Compile `core.c → core.o`

Compile `helper.c → helper.o`

Link both into:
```bash
kbuild_lab.o
```

This single object becomes:

Built-in object OR

Module object

## Compilation Phase

Each `.c` file is compiled with:

Kernel-specific flags

Architecture-specific flags

No libc

`-ffreestanding`

Example (simplified):
```bash
gcc -D__KERNEL__ -DMODULE -c core.c
```

## Module Linking Phase `(CONFIG_KBUILD_LAB = m)`
### 1. Intermediate Objects

Build flow:
```bash
core.o + helper.o
      ↓
kbuild_lab.o
      ↓
kbuild_lab.mod.o
      ↓
kbuild_lab.ko
```

### 2. modpost Stage

`modpost:`

Validates exported symbols

Adds module metadata

Generates `Module.symvers`


## Built-in Linking Phase `(CONFIG_KBUILD_LAB = y)`

If built-in:
```bash
core.o + helper.o
      ↓
kbuild_lab.o
      ↓
built-in.a
      ↓
vmlinux
```

No `.ko` is produced.


## Runtime Phase
### 1. Module Mode
```bash
insmod kbuild_lab.ko
```

Runtime path:
```bash
sys_init_module()
└── load_module()
    └── module_init()
```
### 2. Built-in Mode

During boot:
```bash
start_kernel()
└── do_initcalls()
    └── kbuild_lab_init()
```

## Conditional Compilation Path

Inside `core.c`:
```bash
#ifdef CONFIG_KBUILD_LAB_DEBUG
    pr_info("debug enabled\n");
#endif
```

If enabled → code compiled in

If disabled → code not compiled at all

No runtime overhead.

## Final Build Flow Summary (One Slide / Whiteboard)
```bash
menuconfig
   ↓
Kconfig symbols
   ↓
.config + generated headers
   ↓
Makefile expansion (obj-$(CONFIG))
   ↓
Object compilation
   ↓
Module (.ko) OR built-in (vmlinux)
   ↓
Runtime init (insmod OR initcall)
```
