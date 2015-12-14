/********************************************************************
 * Name: xstdlib.h
 * Author: Rashaud Teague <rashaud.teague@gmail.com>
 * Date: 07/27/2009
 * License: GNU LGPL
 * Description: Extension functions to the standard C Library
 *********************************************************************/

#ifndef XSTDLIB_H_
#define XSTDLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>
#include <dirent.h>

#define XSTDLIB_VERSION 190

#define FILE_NO_FLAGS         00000000
#define FILE_IGNORE_NEW_LINES 00000001
#define FILE_SKIP_EMPTY_LINES 00000010

#define KB 0x400
#define MB 0x100000
#define GB 0x40000000

#if defined(__LP64__)
        #define BIT_SIZE 64
#else
        #define BIT_SIZE 32
#endif

#define random(low, high) (low + rand() % ((high + 1) - low))
#define foreach(element, as, count) \
			as = element; unsigned int _fori = 0; \
			for (_fori = 0; _fori < count; _fori++, as++)

int bin2dec(const char *bin);
int ctoi(unsigned char c);
char *dec2bin(int dec, char *buffer);
void dec2frac(float decimal, float *le_fraction);
char *dec2hex(int dec, char *buffer);
int fact(int n);
int hex2dec(char *hex);
void reduce_frac(float *le_fraction, float *reduce);
float xround(const float n, int precision);

/* file operation prototypes */
char *basename(const char *path, char *buffer);
int copy(const char *source, const char *dest);
long long fappend(const char *path, char *data);
int file(const char *filename, char *elements[], unsigned char flags);
int file_exists(const char *name);
long long file_get_contents(const char *filename, char *buffer, int maxlen);
long long filesize(const char *path);
int move(char *source, char *dest);
long long readfile(const char *path, FILE *des);
/* end */


/* string operation prototypes */
enum str_pad_types {
	STR_PAD_LEFT = 1,
	STR_PAD_RIGHT,
	STR_PAD_BOTH,
};
char *escape(const char *s, char *buffer);
unsigned char is_ascii_pchar(char c);
char *itoa(long i, char *buffer);
char *join(const char *glue, char *elements[], int element_count, char *buffer);
char *ltrim(char *str);
char *rtrim(char *str);
int split(const char *delimiter, const char *str, char *elements[]);
void split_free(char *s[], int s_size);
char *str_concave(const char *str, char *buffer, int size_limit);
char *str_pad(const char *str, char *buffer, unsigned int pad_length, const char *pad_str, unsigned short pad_type);
int strpos(const char *needle, const char *haystack, int offset);
char *str_repeat(const char *str, int multiplier, char *buffer);
char *strrev(char *str);
char *str_replace(const char *find, const char *replace, const char *source, char *buffer);
char *strtoupper(char *str);
char *strtolower(char *str);
char *substr(const char *s, char *buffer, int start, int length);
char *swapcase(char *str);
char *trim(char *str);
char *ucfirst(char *str);
char *ucwords(char *str);
char *unescape(char *s);
char *wordwrap(const char *s, char *buffer, unsigned int line_limit, unsigned short cut);
/* end */

#define file_free split_free

/* os */
char * shell_exec(const char *command, char *out);

/* io */
void die(const char *format, ...);

/* lists */
enum __types__ {
	/* standard */
	VOID = 1, /* this assumes pointer-to-void void* */
	CHAR,
	STRING, /* pointer-to-char, char *, basically */
	SHORT,
	USHORT,
	INT,
	UINT,
	LONG,
	ULONG,
	LONGLONG,
	ULONGLONG,
	FLOAT,
	DOUBLE,
	LONGDOUBLE,
};

enum __list_memory_types__ {
	NONDYNAMIC,
	DYNAMIC,
};

typedef struct __list_element__ {
	void *data;
	struct __list_element__ *prev;
	struct __list_element__ *next;
	void *list;
	unsigned short type;
	unsigned memory_type;
	void (*destroy)(void *d);
	void (*print)(void *v);
} list_element;

typedef struct __list__ {
	unsigned long size;
	list_element *head;
	list_element *tail;
	list_element *current; /* the current iteration position of this vector */
	list_element *next; /* next element from current */
} list;

list *list_init(void);
list_element *list_element_init(list *l, void *data, unsigned short type, unsigned short memory_type);
list_element *list_insert_before(list *l, list_element *before, void *data, unsigned int type, unsigned short memory_type);
list_element *list_insert_after(list *l, list_element *after, void *data, unsigned int type, unsigned short memory_type);
#define list_push_front(l, data, type, memory_type) list_insert_before(l, l->head, data, type, memory_type)
#define list_push_back(l, data, type, memory_type) list_insert_after(l, l->tail, data, type, memory_type)
void list_pop_back(list *l);
list_element *list_current(list *l);
void list_clear(list *l);
void list_remove_element(list *l, list_element *e);
void list_destroy(list *l);
void list_print(list *l);
void list_free_element_data(list_element *e);
void list_rewind(list *l);
void list_fill(list *l, unsigned int qty, long double value);
void list_random_fill(list *l, unsigned int qty);
void list_swap(list *l, list_element *a, list_element *b);

/* vector */
typedef list_element vector_element;
typedef struct __vector__ {
	unsigned short type;
	unsigned short memory_type;
	list *list;
} vector;

vector *vector_init(unsigned short type, unsigned short memory_type);
#define vector_size(v) (v->list->size)
#define vector_insert_before(v, before, data) list_insert_before(v->list, before, data, v->type, v->memory_type)
#define vector_insert_after(v, after, data) list_insert_after(v->list, after, data, v->type, v->memory_type)
#define vector_push_front(v, data) list_push_front(v->list, data, v->type, v->memory_type)
#define vector_push_back(v, data) list_push_back(v->list, data, v->type, v->memory_type)
#define vector_pop_back(v) list_pop_back(v->list)
#define vector_current(v) list_current(v->list)
#define vector_clear(v) list_clear(v->list)
#define vector_remove_element(v, e) list_remove_element(v->list, e)
#define vector_destroy(v) if (v != NULL) { list_destroy(v->list); free(v); v = NULL; }
#define vector_print(v) list_print(v->list)
#define vector_free_element_data(e) list_free_element_data(e)
#define vector_rewind(v) list_rewind(v->list)
#define vector_fill(v, qty, val) list_fill(v->list, qty, val)
#define vector_random_fill(v, qty) list_random_fill(v->list, qty)
#define vector_swap(v, a, b) list_swap(v->list, a, b)
#define vector_move_element(va, vb, e)  \
	if (va != NULL && vb != NULL) {     \
		vector_push_back(vb, e->data);  \
		vector_remove_element(va, e);   \
	}

#define HASHSIZE 256

typedef struct __hashtab__ {
	struct __hashtab__ *next;
	char key[NAME_MAX];
	void *val;
	unsigned short type;
	// a pointer to function to free memory of val
	// it could be the standard free() function or any
	// function to handle the destruction of memory
	void (*destroy)(void *v);
	void (*print)(void *v);
} hashtab;

unsigned int hash(const char *s);
void hash_init(hashtab *h[]);
hashtab *hash_get(hashtab *h[], const char *key);
hashtab *hash_set(hashtab *h[], const char *key, void *val, unsigned short type, void (*destroy)(void *v), void (*print)(void *v));
void hash_unset(hashtab *h[], const char *key);
void hash_destroy(hashtab *h[], unsigned int size);
void hash_print(hashtab *h[], unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
