/********************************************************************
 * Name: lists.c
 * Author: rashaudteague
 * Date: 06/26/2012
 * License: GNU LGPL <http://www.gnu.org/licenses/>
 * Description: Extension functions to the standard C Library
 ********************************************************************/

#include "xstdlib.h"
 
list *list_init(void) {
	list *new_list = NULL;
	if ((new_list = (list *)malloc(sizeof(list))) == NULL) return NULL;
	new_list->size = 0;
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->current = NULL;
	new_list->next = NULL;
	return new_list;
}

list_element *list_element_init(list *l, void *data, unsigned short type, unsigned short memory_type) {
	list_element *el = NULL;
	if ((el = (list_element *)malloc(sizeof(list_element))) == NULL) return NULL;
	
	el->data = data;
	el->type = type;
	el->memory_type = memory_type;
	el->list = l;
	el->destroy = NULL;
	el->print = NULL;
	
	el->prev = NULL;
	el->next = NULL;
	
	return el;
}

list_element *list_insert_before(list *l, list_element *before, void *data, unsigned int type, unsigned short memory_type) {
	list_element *new_element = NULL;
	if ((new_element = list_element_init(l, data, type, memory_type)) == NULL) return NULL;
	
	if (l->size == 0) {
		l->head = new_element;
		l->tail = new_element;
	} else if (before == l->head) {
		before->prev = new_element;
		new_element->next = before;
		l->head = new_element;
	} else {
		new_element->prev = before->prev;
		new_element->next = before;
		before->prev = new_element;
	}
	
	l->size++;
	
	return new_element;
}

list_element *list_insert_after(list *l, list_element *after, void *data, unsigned int type, unsigned short memory_type) {
	list_element *new_element = NULL;
	if ((new_element = list_element_init(l, data, type, memory_type)) == NULL) return NULL;
	
	if (l->size == 0) {
		l->head = new_element;
		l->tail = new_element;
	} else if (after == l->tail) {
		after->next = new_element;
		new_element->prev = after;
		l->tail = new_element;
	} else {
		new_element->prev = after;
		new_element->next = after->next;
		after->next = new_element;
	}
	
	l->size++;
	
	return new_element;
}

void list_pop_back(list *l) {
	if (l != NULL) if (l->tail != NULL) list_remove_element(l, l->tail);
}

list_element *list_current(list *l) {
	if (l->current == NULL && l->next == NULL) {
		l->current = l->head;
	} else {
		l->current = l->next;
	}
	l->next = l->current != NULL ? l->current->next : NULL;
	return l->current;
}

void list_clear(list *l) {
	if (l != NULL) {
		list_element *e = NULL;
		while ((e = list_current(l)) != NULL)
			list_remove_element(l, e);
		list_rewind(l);
	}
}

void list_remove_element(list *l, list_element *e) {
	if (e->prev != NULL) e->prev->next = e->next;
	if (e->next != NULL) e->next->prev = e->prev;
	if (e == l->tail) l->tail = e->prev;
	if (e == l->head) l->head = e->next;
	list_free_element_data(e);
	e->prev = NULL; e->next = NULL;
	e->list = NULL;
	e->type = 0;
	e->memory_type = 0;
	free(e); e = NULL;
	l->size--;
}

void list_destroy(list *l) {
	list_clear(l);
	if (l != NULL) { free(l); l = NULL; }
}

void list_print(list *l) {
	list_element *e;
	printf("[ ");
	while ((e = list_current(l)) != NULL) {
		if (e->print == NULL) {
			switch (e->type) {
				case CHAR: printf("'%c',", *((char *)e->data)); break;
				case STRING: printf("\"%s\",", (char *)e->data); break; // this would be a string of sorts
				case SHORT: printf("%d,", *((short *)e->data)); break;
				case USHORT: printf("%u,", *((unsigned short *)e->data)); break;
				case INT: printf("%d,", *((int *)e->data)); break;
				case UINT: printf("%u,", *((unsigned int *)e->data)); break;
				case LONG: printf("%ld,", *((long *)e->data)); break;
				case ULONG: printf("%lu,", *((unsigned long *)e->data)); break;
				case LONGLONG: printf("%lld,", *((long long *)e->data)); break;
				case ULONGLONG: printf("%llu,", *((unsigned long long *)e->data)); break;
				case FLOAT: printf("%.8f,", *((float *)e->data)); break;
				case DOUBLE: printf("%.16f,", *((double *)e->data)); break;
				case LONGDOUBLE: printf("%.16Lf,", *((long double *)e->data)); break;
			}
		} else {
			e->print(e);
		}
	}
	printf(" ]\n");
}

void list_free_element_data(list_element *e) {
	if (e->destroy != NULL) {
		e->destroy(e->data);
	} else {
		if (e->memory_type == DYNAMIC) free(e->data);
		e->data = NULL;
	}
}

void list_rewind(list *l) {
	l->current = NULL;
	l->next = NULL;
}

void list_fill(list *l, unsigned int qty, long double value) {
	static long double v; v = value;
	while (qty-- > 0) list_push_back(l, &v, LONGDOUBLE, NONDYNAMIC);
}

void list_random_fill(list *l, unsigned int qty) {
	srand(time(NULL));
	while (qty-- > 0) {
		long double *n;
		if ((n = (long double *)malloc(sizeof(long double))) == NULL) {
			fprintf(stderr, "Ran out of memory in list_random_fill...\n");
			exit(EXIT_FAILURE);
		}
		*n = random(0, 100000);
		list_push_back(l, n, LONGDOUBLE, DYNAMIC);
	}
}

void list_swap(list *l, list_element *a, list_element *b) {
	list_element *temp_prev;
	list_element *temp_next;
	temp_prev = a->prev;
	temp_next = a->next;
	
	if (a->prev != b && a->prev != NULL) a->prev->next = b;
	if (a->next != b && a->next != NULL) a->next->prev = b;
	
	if (b->prev != a && b->prev != NULL) b->prev->next = a;
	if (b->next != a && b->next != NULL) b->next->prev = a;
	
	if (a != b->prev) { a->prev = b->prev; } else a->prev = b;
	if (a != b->next) { a->next = b->next; } else a->next = b;
	
	if (b != temp_prev) { b->prev = temp_prev; } else b->prev = a;
	if (b != temp_next) { b->next = temp_next; } else b->next = a;
	
	if (a == l->head) { l->head = b; } else if (b == l->head) l->head = a;
	if (a == l->tail) { l->tail = b; } else if (b == l->tail) l->tail = a;
}
