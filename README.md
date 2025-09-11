# suckless dwm (elijw's build)

Hi. Original dwm readme [here](README.original).

See [modifications](#modifications) before building.

## Table of Contents

* [Applied patches](#applied-patches)
* [Modifications](#modifications)
* [Compiling / building](#compiling--building)

## Applied patches

* alpha
* attachaside
* cfacts
* customrefreshrate
* defaulttransparency
* dwmc
* movestack
* pertag\_with-sel
* vanitygaps ([hardcoded](https://github.com/elijw/dwm/blob/3dec9494ef641e72405a5a60ca2d62bdc54ecd87/config.def.h#L23-L30) gaps. [commented out all but one keybind](https://github.com/elijw/dwm/blob/3dec9494ef641e72405a5a60ca2d62bdc54ecd87/config.def.h#L176-L191))
* xrdb  

## Modifications  

* [MACHINE flag](#machine-flag)
  * [Example usage of MACHINE](#example-usage-of-machine)
* [Layouts enum](#layouts-enum)
* [Modifier aliases](#modifier-aliases)

### MACHINE flag

I introduced a `MACHINE` flag so that this build works across both my desktop and laptop without needing to change code between machines.

There are likely much better solutions to this problem... this was just the first one that came to mind, and it may change in the future.

The [`HOSTNAME`](https://github.com/elijw/dwm/blob/3dec9494ef641e72405a5a60ca2d62bdc54ecd87/config.mk#L11) variable is set using the `hostname` command and [compared against a hard-coded value](https://github.com/elijw/dwm/blob/3dec9494ef641e72405a5a60ca2d62bdc54ecd87/config.mk#L13-L19) (your desktop’s hostname). Based on this check, `MACHINE` is assigned either `0` or `1` and passed to the compiler via `CPPFLAGS += -DMACHINE=$(MACHINE)`.

This makes it possible to keep a single repository and build configuration that automatically adapts to the machine it’s being built on. I’ve applied this modification to all my suckless repos.

```mk
# config.mk

# cross-machine solution
# ----------------------
# set DESKTOP_HOSTNAME to your desktop's hostname
DESKTOP_HOSTNAME ?= arch-desktop

# automatically determine MACHINE based on HOSTNAME
# MACHINE = 0 -> desktop, MACHINE = 1 -> laptop
HOSTNAME := $(shell hostname)

ifeq ($(origin MACHINE), undefined)
ifeq ($(HOSTNAME),$(DESKTOP_HOSTNAME))
 MACHINE := 0
else
 MACHINE := 1
endif
endif
```

#### Example usage of `MACHINE`

You can wrap machine-specific code in `config.h` (or elsewhere) using conditional compilation.
This makes it easy to set different defaults for desktop vs. laptop builds.

```c
#define DESKTOP 0
#define LAPTOP  1

/*
 * example usage:
 */
#if MACHINE == LAPTOP
/* laptop-specific settings */
static const unsigned int borderpx  = 2;   // thinner borders
static const unsigned int gappx     = 5;   // smaller gaps
static const float mfact            = 0.55; // master area size
static const unsigned int refreshhz = 60;   // laptop refresh rate
#else
/* desktop-specific settings */
static const unsigned int borderpx  = 4;   // thicker borders
static const unsigned int gappx     = 15;  // larger gaps
static const float mfact            = 0.50; // master area size
static const unsigned int refreshhz = 144;  // high refresh monitor
#endif
```
---  
### `Layouts` enum  

Made an [enum for the layouts array] to replace magic numbers with readable names. It might be a bit heretical by suckless standards, but after years of dealing with mysterious array indices in college, I can’t help myself.  

---  

### Modifier aliases  

Another slightly controversial tweak: I defined my own aliases for modifier keys instead of using the ModMask directly.
```c
#define ALT   Mod1Mask
#define SUPER Mod4Mask
```
**Why?**: Makes keybindings  easier to read at a glance.  
I apologize if this makes you cringe.  

## Compiling / building

If you haven't, modify line 7 in [config.mk](https://github.com/elijw/dwm/blob/3dec9494ef641e72405a5a60ca2d62bdc54ecd87/config.mk#L7) to reflect your desktop machine's `$HOSTNAME`.

* [Requirements](#requirements)
* [Building](#building)
* [config.mk variables](#configmk)
* [Makefile targets](#makefile-targets)

### Requirements

* Xlib development headers (e.g. `libX11-devel` or `libx11-dev` depending on distro)

### Building

* [Customize config.mk](#configmk) to fit your system.
* Run:

```sh
sudo make clean install
```

### config.mk

Below is a summary of all the variables.

| Variable        | Purpose / Notes                                                                         |
| --------------- | --------------------------------------------------------------------------------------- |
| `VERSION`       | The version of dwm you are building.                                                    |
| `HOSTNAME`      | Pulled from `hostname` command to determine which machine you are on.                   |
| `MACHINE`       | Automatically set to `0` for desktop, `1` for laptop. Used for conditional compilation. |
| `PREFIX`        | Installation base directory. Default: `/usr/local`.                                     |
| `MANPREFIX`     | Where the manpage is installed. Default: `${PREFIX}/share/man`.                         |
| `X11INC`        | Path to X11 header files. Adjust if your distro uses a different location.              |
| `X11LIB`        | Path to X11 libraries.                                                                  |
| `XINERAMALIBS`  | Linker flags for Xinerama support. Comment out to disable.                              |
| `XINERAMAFLAGS` | Preprocessor flag for Xinerama support (`-DXINERAMA`).                                  |
| `FREETYPEINC`   | Include path for Freetype headers.                                                      |
| `FREETYPELIBS`  | Linker flags for Freetype (`-lXft -lfontconfig`).                                       |
| `INCS`          | Combined include flags for the compiler (`-I` paths).                                   |
| `LIBS`          | Combined linker flags including X11, Xinerama, Freetype, and optional Imlib2.           |
| `CPPFLAGS`      | Preprocessor flags passed to the compiler (`-D` flags, version, Xinerama, MACHINE).     |
| `CFLAGS`        | Compiler flags (e.g., standard, optimization, warnings, includes).                      |
| `LDFLAGS`       | Linker flags passed when building the `dwm` binary.                                     |
| `CC`            | Compiler to use (usually `cc` or `gcc`).                                                |

### Makefile targets

| Target / Rule | Purpose                                                                                  |
| ------------- | ---------------------------------------------------------------------------------------- |
| **all**       | Default target. Builds `dwm`.                                                            |
| **.c.o**      | Compiles each `.c` source file into an object (`.o`) file using `${CC}` and `${CFLAGS}`. |
| **\${OBJ}**   | Ensures all object files depend on `config.h` and `config.mk` (rebuild if they change).  |
| **config.h**  | Creates `config.h` by copying `config.def.h` if it doesn’t exist.                        |
| **dwm**       | Links all object files (`${OBJ}`) into the final `dwm` binary.                           |
| **clean**     | Removes the `dwm` binary, object files, and tarballs.                                    |
| **dist**      | Builds a source distribution tarball `dwm-${VERSION}.tar.gz` with all required files.    |
| **install**   | Installs `dwm` and `dwmc` into `${PREFIX}/bin`, installs the manpage to `${MANPREFIX}`.  |
| **uninstall** | Removes installed `dwm` and its manpage.                                                 |
