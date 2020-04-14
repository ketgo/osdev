#ifndef CPU_H
#define CPU_H

/*
    cpu.h
        CPU Interface

    This file provides an intefrace for basic control of CPU devices. 
    This is accomplished by abstracting hardware dependencies behind 
    this interface.

	All routines and types are declared extern and must be defined within
	external libraries to define specific hal implimentations. As the
    implementation is architecture specific, they are located in the 
    corresponding sub-folder of the arch/ folder.
*/

//! initialize cpu
extern int init_cpu();

//! shutdown hardware abstraction layer
extern int shutdown_cpu();

//! returns cpu vender
extern const char *get_cpu_vender();

#endif /* CPU_H */
