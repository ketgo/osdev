# OSDev

My OS development 

## Development Environment

The following dependencies need to be installed for building and development:

- x86 Assembler: [NASM](https://www.nasm.us/)
- Disk writer: [`dd`](https://en.wikipedia.org/wiki/Dd_(Unix))
- C cross-compiler: i386-elf-binutils, i386-elf-gcc, i386-elf-gdb
- x86 PC enumerator: [QEMU](https://www.qemu.org/docs/master/qemu-doc.html#QEMU-PC-System-emulator), [bochs](http://bochs.sourceforge.net/)

# NewLib

The directory `newlib` contains the [Newlib](https://sourceware.org/newlib/) source code which 
is used to port a standard C lib for building OS applications. It is also used to generate 
no-host version of the lib which is used by the kernel for some OS independent standard string 
and memory manipulation functions.

### Building and Running

Once all prerequisites have been installed, the `make` utility can be used to build and run project.

- To run OS on Emulator. This will auto-builds the kernel image and load it on Emulator:
  ```bash
  $ make run
  ```
- To build kernel image only:
  ```bash
  $ make build
  ```
- To cleanup project builds:
  ```bash
  $ make clean
  ```

The build process creates a `build\<target>` directory where all header files, libraries and kernel image are placed appropriately. Thus this directory acts as our OS root using which an OS image is created. The `<target>` here refers to the CPU arch for which the kernel is being built. 

### Debugging

There are couple of ways to debug the kernel. One is using the cross-compiled `gdb` utility (e.g. i386-elf-gdb), the other is using the internal debugger of Bochs emulator.

## Development Tasks

- [ ] Phase I - Basics
  - [x] [Setting up a Cross-Toolchain](https://wiki.osdev.org/GCC_Cross_Compiler)
  - [x] [Creating a Hello World kernel](https://wiki.osdev.org/Bare_Bones)
  - [x] [Setting up a Project](https://wiki.osdev.org/Meaty_Skeleton)
    - [x] i386
    - [ ] [x86_64 - Long Mode](https://wiki.osdev.org/Long_Mode)
  - [x] [Calling Global Constructors](https://wiki.osdev.org/Calling_Global_Constructors)
  - [x] [Terminal Support](https://wiki.osdev.org/index.php?title=Printf&action=edit&redlink=1)
  - [x] [Stack Smash Protector](https://wiki.osdev.org/Stack_Smashing_Protector)
  - [x] [Multiboot](https://wiki.osdev.org/Multiboot)
  - [x] [Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table)
  - [ ] [Interrupts](https://wiki.osdev.org/Interrupts)
  - [ ] [Memory Management](https://wiki.osdev.org/Memory_Management)
    - [ ] [Page Tables](https://wiki.osdev.org/Setting_Up_Paging)
    - [ ] [Higher Half](https://wiki.osdev.org/Higher_Half_x86_Bare_Bones)
    - [ ] [Page Frame Allocation](https://wiki.osdev.org/Page_Frame_Allocation)
  - [ ] [Multithreaded Kernel](https://wiki.osdev.org/index.php?title=Multithreaded_Kernel&action=edit&redlink=1)
  - [ ] [Keyboard](https://wiki.osdev.org/Keyboard)
  - [ ] [Internal Kernel Debugger](https://wiki.osdev.org/index.php?title=Internal_Kernel_Debugger&action=edit&redlink=1)
  - [ ] [Filesystem Support](https://wiki.osdev.org/Filesystem)
    - [ ] [Initialization Ramdisk](https://wiki.osdev.org/Initrd)
- [ ] Phase II - User-Space
  - [ ] [User-Space](https://wiki.osdev.org/index.php?title=User-Space&action=edit&redlink=1)
  - [ ] [Program Loading](https://wiki.osdev.org/index.php?title=Program_Loading&action=edit&redlink=1)
  - [ ] [System Calls](https://wiki.osdev.org/System_Calls)
  - [ ] [OS Specific Toolchain](https://wiki.osdev.org/OS_Specific_Toolchain)
  - [ ] [Creating a C Library](https://wiki.osdev.org/Creating_a_C_Library)
  - [ ] [Fork and Execute](https://wiki.osdev.org/index.php?title=Fork&action=edit&redlink=1)
  - [ ] [Shell](https://wiki.osdev.org/Shell)
- [ ] Phase III - Extending your Operating System
  - [ ] [Time](https://wiki.osdev.org/Time)
  - [ ] [Threads](https://wiki.osdev.org/Thread)
  - [ ] [Thread Local Storage](https://wiki.osdev.org/Thread_Local_Storage)
  - [ ] [Symmetric Multiprocessing](https://wiki.osdev.org/SMP)
  - [ ] [Secondary Storage](https://wiki.osdev.org/index.php?title=Secondary&action=edit&redlink=1)
  - [ ] [Real Filesystems](https://wiki.osdev.org/File_Systems)
  - [ ] [Graphics](https://wiki.osdev.org/How_do_I_set_a_graphics_mode)
  - [ ] [User Interface](https://wiki.osdev.org/User_Interface)
    - [ ] [Compositing](https://wiki.osdev.org/Compositing)
  - [ ] [Networking](https://wiki.osdev.org/Networking)
  - [ ] [Sound](https://wiki.osdev.org/Sound)
  - [ ] [Universal Serial Bus](https://wiki.osdev.org/USB)
- [ ] Phase IV - Bootstrapping
  - [ ] [Porting Software](https://wiki.osdev.org/Cross-Porting_Software)
  - [ ] [Porting GCC](https://wiki.osdev.org/Porting_GCC_to_your_OS)
  - [ ] Compiling your OS under your OS
  - [ ] Fully Self-hosting
  
  ## References

- [Baremetal x86_64](https://github.com/winksaville/baremetal-x86_64)
- [Klosy Legacy](https://github.com/kodo-pp/klosy-legacy)
- [BeeOS](https://github.com/davxy/beeos)
- [Beelzebub](https://github.com/vercas/Beelzebub)
- [Qword](https://github.com/qword-os/qword)
- [Linux](https://github.com/torvalds/linux)
