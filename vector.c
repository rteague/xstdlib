/********************************************************************
 * Name: vector.c
 * Author: rashaudteague
 * Date: 08/12/2012
 * License: GNU LGPL <http://www.gnu.org/licenses/>
 * Description: Extension functions to the standard C Library
 ********************************************************************/

#include "xstdlib.h"

vector *vector_init(unsigned short type, unsigned short memory_type) {
	vector *v = NULL;
	if ((v = (vector *)malloc(sizeof(vector))) == NULL) return NULL;
	if ((v->list = list_init()) == NULL) return NULL;
	v->type = type;
	v->memory_type = memory_type;
	return v;
}
