# 🍿 Popcorn

A toy kernel for i386 with ambitions for semi-POSIX compatibility

<br/>

## Check it out

Through the magic of [v86](https://github.com/copy/v86) you can check the OS out in your browser [here](https://valtyr.github.io/popcorn)!

## Getting started

I don't know why you want to do this, but hey try it at your own risk!

### Prerequisites

Before you get started you need QEMU and Docker installed on your computer. Procurement is left as an exercise to the reader.

If you use fish (like any sane person would) run `source commands.fish` to get access to the project's convenience functions.
If you use some other shell feel free to refer to `commands.fish`, and copy-paste the commands into your shell.

### Build your build environment

To get reproducible builds and minimal PATH-crud all the required build-chain tools are available in a docker image. To start
compiling the OS itself we must therefore start by building the docker image. To do this run the following command from the
project's root directory:

```
build-buildenv
```

### Build and run Popcorn

Now you can actually compile and run Popcorn itself. Run the following command, once again from the project's root directory:

```
build-and-run-kernel
```

This compiles the kernel, creates a GRUB rescue .iso file and boots QEMU.


### Run on real hardware

Since we've let `grub-mkrescue` do the heavy lifting in creating a bootable iso we can simply flash it to a USB-drive
and use it as a boot drive.

Here's an example using `dd`:

```
sudo dd if=dist/x86_64/kernel.iso of=[YOUR_DISK] bs=1m
```

> **Be careful!**
>
> Make sure that the device you write to doesn't contain anything important.
> <br/> The dd command can irreversibly destroy your data if you're not careful :trollface:


## Stuff to do

- [x] Multiboot2 and multiboot2 response parsing
- [x] Basic input/output
- [x] Serial output for debugging
- [x] Basic framebuffer
- [x] Set up a rudimentary GDT
- [x] Get interrupts working
- [x] Interface with the RTC
- [x] PS2 keyboard support
- [x] Set up CI and auto deploy a v86 website
- [ ] Implement a `kmalloc`
- [ ] Create an ELF parser
- [ ] Soundblaster 16 driver
- [ ] Get some timers running
- [ ] Start implementing syscalls!
- [ ] Write a libc and start working on userland
- [ ] Virtual terminal
- [ ] Window server

