/********************************************************************
 * Name: os.c
 * Author: rashaudteague
 * Date: 11/22/2011
 * License: GNU LGPL <http://www.gnu.org/licenses/>
 * Description: Extension functions to the standard C Library
 ********************************************************************/

#include "xstdlib.h"

char * shell_exec(const char *command, char *out) {
	FILE *in;
	if ((in = popen(command, "r")) == NULL) return NULL;

	char _out[LINE_MAX];

	while (fgets(_out, LINE_MAX, in) != NULL) strcat(out, _out);
	
	pclose(in);

	return out;
}

