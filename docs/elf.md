# ELF

 The Executable and Linking Format (ELF) is a binary format originally developed and published by UNIX System Laboratories (USL). It is the default binary format for the executable files used by SVR4 and Solaris 2.x. ELF is more powerful and flexible than the a.out and COFF binary formats. Combined with appropriate tools, programmers can use ELF to control the flow of execution at run time.

## ELF Types

There are three main types for ELF files.

- An executable file contains code and data suitable for execution. It specifies the memory layout of the process.
- A relocatable file contains code and data suitable for linking with other relocatable and shared object files.
- A shared object file (a.k.a. shared library) contains code and data suitable for the link editor ld at link time and the dynamic linker at run time. The dynamic linker may be called ld.so.1, libc.so.1 or ld-linux.so.1, depending on the implementation. 

The most useful part of ELF lies in its section structure. With the right tools and techniques, programmers can manipulate the execution of executables with great flexibility. 


## The .init and .fini Sections

On an ELF system, a program consists of one executable file and zero or more shared object files. To execute such a program, the system uses those files to create a process image in memory. A process image has segments which contain executable instructions, data and so on. For an ELF file to be loaded into memory, it has to have a program header which is an array of structures which describe segments and other information which the system needs to prepare the program for execution.

A segment consists of sections, which is the most important aspect of ELF from the programmer's point of view.

Each executable or shared object file generally contains a section table, which is an array of structure describing the sections inside the ELF object file. There are several special sections defined by the ELF documentations which hold program and control information. The following ones are very useful to programmers.

__.fini__: _This section holds executable instructions that contribute to the process termination code. That is, when a program exits normally, the system arranges to execute the code in this section._


__.init__: _This section holds executable instructions that contribute to the process initialization code. That is, when a program starts to run the system arranges to execute the code in this section before the main program entry point (called main in C programs)._ 

The .init and .fini sections have a special purpose. If a function is placed in the .init section, the system will execute it before the main function. Also the functions placed in the .fini section will be executed by the system after the main function returns. This feature is utilized by compilers to implement global constructors and destructors in C++.

When an ELF executable is executed, the system will load in all the shared object files before transferring control to the executable. With the properly constructed .init and .fini sections, constructors and destructors will be called in the right order.

## Global Constructors and Destructors in C++

Global constructors and destructors in <tt> C++</tt> have
to be handled very carefully to meet the language specification.
Constructors have to be called before the <em> main</em>
function. Destructors have to be executed after it returns.
Under ELF, this can be treated gracefully by the compiler. For example,
the <tt> GNU C/C++</tt> compiler, <i> gcc</i>, provides two auxiliary
start up files called <em> crtbegin.o</em> and <em> crtend.o</em>, in addition
to two normal auxiliary files <em> crti.o</em> and <em> crtn.o</em>.
Together with the <b> .ctors</b> and <b> .dtors</b> sections described
below, the <tt> C++</tt>
global constructors and destructors can be executed in
the proper order with minimal run-time overhead.
<P>
<DL ><DT>.ctors
<DD>   <BR> 
   This section holds an array of the global constructor
   function pointers of a program.
<P>
 <DT>.dtors
<DD>   <BR> 
    This section holds an array of the global destructor
    function pointers of a program.
<P>
 <DT>crtbegin.o
<DD>   <BR> 
    There are four sections:
<P>
<UL><LI> The <b> .ctors</b> section. It has a local symbol,
  <i> <code>__CTOR_LIST__</code></i>, which is the head of the global
  constructor function pointer array. This array in <em> crtbegin.o</em>
  only has one dummy element.
<P>
  <LI> The <b> .dtors</b> section. It has a local symbol,
  <i> <code>__DTOR_LIST__</code></i>, which is the head of the global
  destructor function pointer array. This array in <em> crtbegin.o</em>
  only has only one dummy element.
<P>
  <LI> The <b> .text</b> section. It contains only one function,
  <i> <code>__do_global_dtors_aux</code></i>, which goes through
  <i> <code>__DTOR_LIST__</code></i>
  from the head and calls each destructor function on the list.
<P>
  <LI> The <b> .fini</b> section. It contains only a call to
  <i> <code>__do_global_dtors_aux</code></i>. Please remember it has just a
  function call without return since the <b> .fini</b> section in
  <b> crtbegin.o</b> is part of the body of a function.
</ul>
<P>
 <DT>crtend.o
<DD>   <BR> 
There are also four sections:
<P>
<UL><LI> The <b> .ctors</b> section. It has a local symbol,
  <i> <code>__CTOR_END__</code></i>, which is the label for the tail of the
  global constructor function pointer array.
<P>
  <LI> The <b> .dtors</b> section. It has a local symbol,
  <i> <code>__DTOR_END__</code></i>, which is the label for the tail of the
  global destructor function pointer array.
<P>
  <LI> The <b> .text</b> section. It contains only one function,
  <i> <code>__do_global_ctors_aux</code></i>, which goes through
  <i> <code>__CTOR_LIST__</code></i>
  from the tail and calls each constructor function on the list.
<P>
  <LI> The <b> .init</b> section. It contains only a function call to
  <i> <code>__do_global_ctors_aux</code></i>. Please remember it has just a
  function call without return since the <b> .init</b> section in
  <b> crtend.o</b> is part of the body of a function.
<P>
</ul>
<P>
 <DT>crti.o
<DD>   <BR> 
   It has only a function label <i> <code>_init</code></i> in the
   <b> .init</b> section and a function label <i> <code>_fini</code></i> in the
   <b> .fini</b> section.
<P>
 <DT>crtn.o
<DD>   <BR> 
   It has only a return instruction each in the <b> .init</b> and
   <b> .fini</b> sections.
<P>
 </dl>
<P>
At compile time while generating the relocatable files, 
<i> gcc</i> puts each global constructor
on <i> <code>__CTOR_LIST__</code></i> by putting a pointer to the constructor
function in the  <b> .ctors</b> section.
It also puts each
global destructor on <i> <code>__DTOR_LIST__</code></i> by putting a pointer
to the destructor function in the <b> .dtors</b> section.
<P>
At link time, the <i> gcc</i> driver places <b> crtbegin.o</b>
immediately before all the relocatable files and <b> crtend.o</b>
immediately after all the relocatable files. In addition,
<b> crti.o</b> was placed before <b> crtbegin.o</b> and
<b> crtn.o</b> was placed after <b> crtend.o</b>.
<P>
While generating the executable file, the link editor, <b> ld</b>,
concatenates the <b> .ctors</b> sections and the <b> .dtors</b> sections
from all the relocatable files to form <i> <code>__CTOR_LIST__</code></i>
and <i> <code>__DTOR_LIST__</code></i>, respectively. The <b> .init</b>
sections from all the  relocatable files form the
<i> <code>_init</code></i> function and the <b> .fini</b> sections
form the <em> <code>_fini</code></em> function.
<P>
At run time, the system will execute the <i> <code>_init</code></i>
function before the <i> main</i> function and execute the
<i> <code>_fini</code></i> function after the <i> main</i> function returns.
