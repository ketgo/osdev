# Arch i386 [OLD]

This folder contains i386 arch specific source code. Description of each file is shown in the following table:

| File | Description |
| --- | --- |
| `boot.S` | Contains the assembly code for kernel bootstrap. The multiboot headers and memory stack are set followed by call to `kernel_main`. |
| `linker.ld` | File containing rules on how the linker should assemble different sections of the code. |
| `crti.S`, `crtn.S` | These are C runtime files used to initiate global constructors by GCC. As GCC uses ELF binary format, please refer the [`ELF`]() documentation for details. |
