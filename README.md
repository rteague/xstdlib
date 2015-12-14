#README

##About

The xstdlib (eXtended Standard Library) is a library that ports many essential or miscellaneous high-level language functions (like in PHP, Python or Perl) to C or any functions that are of common use to C developers.

##Notice

* Must be a sudoer on the system to install this, you will have to run commands that require 'sudo'
* ALWAYS CHECK FOR UPDATES!!!!!

##Installation

1. Pull from github...
2. cd /path/to/xstdlib/
3. run: make
4. run: sudo make install
5. run: make clean

##Uninstallation

Just return to the extracted folder from the package you downloaded and run: sudo make uninstall

##Usage and compile instructions

To use this library is just like using any other library in C just include the header file in your source file, like so:
```c
#include <xstdlib.h>
```

When compiling a program using the xstdlib library add the flag -lxstdlib ...for example:
```bash
gcc -c Wall mysource.c -o mysource.o -lxstdlib
```

(or with cc)

```bash
cc -c Wall mysource.c -o mysource.o -lxstdlib
```
