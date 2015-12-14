/*
 * hash.c
 * hash tables--basically the same as how "K&R" does it...
 * rashaudteague
 * Wed Apr 24 21:49:03 2013
 */

#include "xstdlib.h"

unsigned int hash(const char *s) {
	unsigned int hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

void hash_init(hashtab *h[]) {
	int i; for (i = 0; i < HASHSIZE; i++) h[i] = NULL;
}

hashtab *hash_get(hashtab *h[], const char *key) {
	unsigned int hashval = hash(key);
	hashtab *entry;
	for (entry = h[hashval]; entry != NULL; entry = entry->next) {
		if (strcmp(key, entry->key) == 0)
			return entry;
	}
	return NULL;
}

hashtab *hash_set(hashtab *h[], const char *key, void *val, unsigned short type, void (*destroy)(void *v), void (*print)(void *v)) {
	hashtab *n;
	unsigned int hashval;
	if ((n = hash_get(h, key)) == NULL) {
		if ((n = (hashtab *)malloc(sizeof(hashtab))) == NULL)
			return NULL;
		hashval = hash(key);
		n->next = h[hashval];
		strcpy(n->key, key);
		n->val  = val;
		n->type = type;
		n->destroy = destroy;
		h[hashval] = n;
	} else {
		if (n->destroy != NULL) n->destroy(n->val);
		n->val = val;
		n->destroy = destroy;
	}
	return n;
}

void hash_unset(hashtab *h[], const char *key) {
	hashtab *n, *next;
	if ((n = hash_get(h, key)) != NULL) {
		if (n->destroy != NULL) n->destroy(n->val);
		else n->val = NULL;
	}
}

void hash_destroy(hashtab *h[], unsigned int size) {
	int i;
	hashtab *entry, *next;
	for (i = 0; i < size; i++) {
		for (entry = h[i]; entry != NULL; entry = next) {
			if (entry->destroy != NULL) entry->destroy(entry->val);
			next = entry->next;
			free(entry); entry = NULL;
		}
	}
}

void hash_print(hashtab *h[], unsigned int size) {
	int i;
	hashtab *entry, *next;
	printf("{ ");
	for (i = 0; i < size; i++) {
		for (entry = h[i]; entry != NULL; entry = next) {
			if (entry->print == NULL) {
				switch (entry->type) {
					case CHAR: printf("\"%s\" : '%c',", entry->key, *((char *)entry->val)); break;
					case STRING: printf("\"%s\" : \"%s\",", entry->key, (char *)entry->val); break;
					case SHORT: printf("\"%s\" : %d,", entry->key, *((short *)entry->val)); break;
					case USHORT: printf("\"%s\" : %u,", entry->key, *((unsigned short *)entry->val)); break;
					case INT: printf("\"%s\" : %d,", entry->key, *((int *)entry->val)); break;
					case UINT: printf("\"%s\" : %u,", entry->key, *((unsigned int *)entry->val)); break;
					case LONG: printf("\"%s\" : %ld,", entry->key, *((long *)entry->val)); break;
					case ULONG: printf("\"%s\" : %lu,", entry->key, *((unsigned long *)entry->val)); break;
					case LONGLONG: printf("\"%s\" : %lld,", entry->key, *((long long *)entry->val)); break;
					case ULONGLONG: printf("\"%s\" : %llu,", entry->key, *((unsigned long long *)entry->val)); break;
					case FLOAT: printf("\"%s\" : %.8f,", entry->key, *((float *)entry->val)); break;
					case DOUBLE: printf("\"%s\" : %.16f,", entry->key, *((double *)entry->val)); break;
					case LONGDOUBLE: printf("\"%s\" : %.16Lf,", entry->key, *((long double *)entry->val)); break;
				}
			} else {
				entry->print(entry);
			}
			next = entry->next;
		}
	}
	printf(" }\n");
}


