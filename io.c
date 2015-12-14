/********************************************************************
 * Name: io.c
 * Author: rashaudteague
 * Date: 11/22/2011
 * License: GNU LGPL <http://www.gnu.org/licenses/>
 * Description: Extension functions to the standard C Library
 ********************************************************************/

#include "xstdlib.h"

void die(const char *format, ...) {
	va_list arg;
	int done;
	
	va_start (arg, format);
	done = vfprintf (stdout, format, arg);
	va_end (arg);
	
	exit(EXIT_SUCCESS);
}
