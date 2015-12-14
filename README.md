#README

##About

The xstdlib (eXtended Standard Library) is a library that ports many essential or miscellaneous high-level language functions (like in PHP, Python or Perl) to C or any functions that are of common use to C developers.

##Notice

* Must be a sudoer on the system to install this, you will have to run commands that require 'sudo'
* ALWAYS CHECK FOR UPDATES!!!!!

##Installation

###THE NEW WAY - RECOMMENDED

(NOTE: Make sure you replace <version> with the correct value, for example xstdlib-1.7.2.tar.gz)

1. Unpack xstdlib-<version>.tar.gz tar ball by running this in command line: tar -zxvf xstdlib-<version>.tar.gz
2. cd xstdlib/
3. make
4. sudo make install
5. make clean

##Uninstallation

Just return to the extracted folder from the package you downloaded and run: sudo make uninstall

##Usage and compile instructions

To use this library is just like using any other library in C just include the header file in your source file, like so:
#include <xstdlib.h>

When compiling a program using the xstdlib library add the flag -lxstdlib ...for example:
gcc -c Wall mysource.c -o mysource.o -lxstdlib

(or with cc)

cc -c Wall mysource.c -o mysource.o -lxstdlib

