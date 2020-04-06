# ELF For Programmers

 The Executable and Linking Format (ELF) is a binary format originally developed and published by UNIX System Laboratories (USL). It is the default binary format for the executable files used by SVR4 and Solaris 2.x. ELF is more powerful and flexible than the a.out and COFF binary formats. Combined with appropriate tools, programmers can use ELF to control the flow of execution at run time.

## ELF Types

There are three main types for ELF files.

- An executable file contains code and data suitable for execution. It specifies the memory layout of the process.
- A relocatable file contains code and data suitable for linking with other relocatable and shared object files.
- A shared object file (a.k.a. shared library) contains code and data suitable for the link editor ld at link time and the dynamic linker at run time. The dynamic linker may be called ld.so.1, libc.so.1 or ld-linux.so.1, depending on the implementation. 

The most useful part of ELF lies in its section structure. With the right tools and techniques, programmers can manipulate the execution of executables with great flexibility. 


