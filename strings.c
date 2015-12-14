/********************************************************************
 * Name: strings.c
 * Author: Rashaud Teague <rashaud.teague@gmail.com>
 * Date: 07/27/2009
 * License: GNU LGPL
 * Description: Extension functions to the standard C Library
 *********************************************************************/

#include "xstdlib.h"

char *escape(const char *s, char *buffer) {
	size_t s_len = strlen(s);
	size_t i, j;
	for (i = 0, j = 0; j < s_len; i++, j++) {
		buffer[i] = s[j];
		if (buffer[i] == 0x0 || buffer[i] == 0x27 || buffer[i] == 0x22 || buffer[i] == 0x5C) {
			buffer[i]   = 0x5C;
			buffer[++i] = s[j];
		}
	}
	buffer[i] = '\0';
	return buffer;
}

/**
 * Checks whether a character is a valid ascii printable character codes 32-126
 * Returns true if the character is valid, false if not valid.
 */
unsigned char is_ascii_pchar(char c) {
	if (c >= 32 && c <= 126) return 1;
	return 0;
}

/**
 * Converts an integer into a string
 */
char *itoa(long i, char *buffer) {
	sprintf(buffer, "%li", i);
	return buffer;
}

/**
 * Joins a string from a split
 */
char *join(const char *glue, char *elements[], int element_count, char *buffer) {
	int i;
	for (i = 0; i < element_count; i++) {
		if (strcmp(elements[i], "") != 0) {
			
			strcat(buffer, elements[i]);
			if (i != element_count - 1) strcat(buffer, glue);
		}
	}
	
	return buffer;
}

/**
 * Trims leading white spaces from a string
 */
char * ltrim(char *str) {
	int i = 0;
	
	while (isspace(str[i++]));
	strncpy(str, str + i - 1, strlen(str));
			
	return str;
}

/**
 * Trims trailing white spaces from a string
 */
char *rtrim(char *str) {
	size_t i = strlen(str) - 1;
	
	while (isspace(str[i])) i--;
	str[i + 1] = '\0';
	
	return str;
}

/**
 * Splits a string into an array by a string delimeter, returns number of
 * elements created or -1 if failed
 */
int split(const char *delimiter, const char *s, char *elements[]) {
	size_t s_len         = strlen(s);
	size_t delimiter_len = strlen(delimiter);
	int i = 0, e = 0, d, last_d = 0, c, x;
	char *p;

	if (delimiter_len > 0) {
		while ((d = strpos(delimiter, s, i)) >= 0) {
			c = (i == 0) ? c = 0 : last_d + delimiter_len;
			if (d == 0) {
				if ((p = (char *)malloc(sizeof(char))) == NULL) return e;
				memcpy(p, "\0", 1);
			} else {
				x = d - c;
				if ((p = (char *)malloc(x)) == NULL) return e;
				memcpy(p, s + c, x);
				p[x] = '\0';
			}
			last_d = d;
			elements[e] = p;
			i++, e++;
		}
		if ((p = (char *)malloc(s_len - c)) == NULL) return e;
		memcpy(p, s + last_d + delimiter_len, s_len - c);
		elements[e] = p;
		e++;
	}
	return e;
}

void split_free(char *s[], int s_size) {
	int i;
	for (i = 0; i < s_size; i++) {
		free(s[i]); s[i] = NULL;
	}
}

char *str_concave(const char *str, char *buffer, int size_limit) {
	int str_size = (int)strlen(str);
	
	if (str_size <= size_limit) { strcpy(buffer, str); return buffer; }
	
	int half = (int)size_limit / 2;
	int i = half;
	int left_over = size_limit % 2;
	int apposition_size = 3;
	int end = half - apposition_size + left_over;
	
	memcpy(buffer, str, half);
	memcpy(buffer + half, "...", apposition_size); /* add the apposition */           i += apposition_size;
	memcpy(buffer + i, str + ((str_size - half) + apposition_size - left_over), end); i += end;
	buffer[i] = '\0';

	return buffer;
}

char *str_pad(const char *str, char *buffer, unsigned int pad_length, const char *pad_str, unsigned short pad_type) {
	int str_size = (int)strlen(str);
	int pad_str_size = (int)strlen(pad_str);
	int pads = (pad_length - str_size) / pad_str_size;
	int left_over = (pad_length - str_size) - pads * pad_str_size;
	int i;
	
	if (str_size == pad_length) { strcpy(buffer, str); return buffer; }
	if (pad_type == STR_PAD_BOTH) { pads /= 2; pad_type |= STR_PAD_LEFT | STR_PAD_RIGHT; }
	
	if (pad_type & STR_PAD_LEFT) {
		strcpy(buffer, pad_str);
		for (i = 1; i < pads; i++) strcat(buffer, pad_str);
		if (!(pad_type & STR_PAD_RIGHT)) strncat(buffer, pad_str, left_over);
		strcat(buffer, str);
	}
	
	if (pad_type & STR_PAD_RIGHT) {
		if (!(pad_type & STR_PAD_LEFT)) strcpy(buffer, str);
		for (i = 0; i <= pads; i++) strncat(buffer, pad_str, pad_str_size);
		strncat(buffer, pad_str, left_over);
	}
	
	buffer[pad_length] = '\0';
	
	return buffer;
}

/**
 * Repeat a string x number of times by the multiplier
 */
char *str_repeat(const char *str, int multiplier, char *buffer) {
	int i;
	for (i = 0; i < multiplier; i++) strcat(buffer, str);
	return buffer;
}

/**
 * Find position of first occurrence of a string
 * @return pos, return -1 if the needle is not found
 */
int strpos(const char *needle, const char *haystack, int offset) {
	int pos = -1;
	int haystack_size = strlen(haystack);
	int needle_size = strlen(needle);
	int i;
	char *search;
	if ((search = (char *) malloc(sizeof(char) * needle_size + 1)) == NULL) {
		fprintf(stderr, "error in malloc in strpos\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < haystack_size; i++) {
		substr(haystack, search, i, needle_size);
		
		if (strcmp(search, needle) == 0) {
			if (offset > 0) {
				i = i + (needle_size - 1);
				offset--;
			} else {
				free(search);
				return i;
			}
		}
	}
	free(search);
	return pos;
}

/**
 * Replaces a sub string with string
 */
char *str_replace(const char *find, const char *replace, const char *source, char *buffer) {
	int i = 0, p = 0, c = 0;
	size_t find_len    = strlen(find);
	size_t replace_len = strlen(replace);
	size_t source_len  = strlen(source);
	while ((c = strpos(find, source, i++)) >= 0) {
		strncpy(buffer+strlen(buffer), source+p, c-p);
		strncpy(buffer+strlen(buffer), replace, replace_len);
		p = c + find_len;
	}
	strncpy(buffer+strlen(buffer), source+p, source_len - p);
	buffer[strlen(buffer)] = '\0';
	return buffer;
}

/**
 * Reverses a string
 */
char *strrev(char *str) {
	size_t str_size = strlen(str);
	
	char temp;
	size_t middle = ceil(str_size / 2);
	size_t i;
	for (i = 0; i < middle; i++) {
		temp = str[i];
		str[i] = str[str_size - i - 1];
		str[str_size - i - 1] = temp;
	}
	
	return str;
}

/**
 * Makes a string uppercase
 */
char *strtoupper(char *str) {
	size_t str_size = strlen(str);
	size_t i = 0;
	for ( ; i < str_size; i++) str[i] = toupper(str[i]);
	return str;
}

/**
 * Makes a string lowercase
 */
char *strtolower(char *str) {
	size_t str_size = strlen(str);
	size_t i = 0;
	for ( ; i < str_size; i++) str[i] = tolower(str[i]);
	return str;
}

/**
 * Return a substring of a string
 */
char *substr(const char *s, char *buffer, int start, int length) {
	size_t s_len = strlen(s);

	if (!(length > s_len || start > s_len - 1)) {
		if (start < 0) start = s_len + start;
		if (length < 0) {
			start = start + length;
			length *= -1;
		}
		if (!(start == 0 && length == 0)) {
			if (start + length > s_len) length = s_len - start;
			memcpy(buffer, s + start, length);
		}
	} else {
		length = 0;
	}
	
	buffer[length] = '\0';
	return buffer;
}

/**
 * Converts all lowercase into uppercase and vice versa
 */
char *swapcase(char *str) {
	size_t str_size = strlen(str);
	size_t i;
	for (i = 0; i < str_size; i++) {
		if (islower(str[i])) {
			str[i] = toupper(str[i]);
		} else if (isupper(str[i])) {
			str[i] = tolower(str[i]);
		}
	}
	
	return str;
}

/**
 * name: trim
 * Trims leading and trailing white spaces from a string
 * @param str
 * @return rstr
 */
char *trim(char *str) {
	return rtrim(ltrim(str));
}

/**
 * Turns the first letter in the string to uppercase
 */
char *ucfirst(char *str) {
	str[0] = toupper(str[0]);
	return str;
}

/**
 * Turns the first letter in every word to uppercase
 */
char *ucwords(char *str) {
	size_t str_size = strlen(str);
	size_t i;
	for (i = 0; i < str_size; i++) {
		if (i == 0 || isspace(str[i-1])) {
			str[i] = toupper(str[i]);
		}
	}
	
	return str;
}

char *unescape(char *s) {
	size_t s_len = strlen(s);
	size_t i;
	for (i = 0; i < s_len; i++) {
		if (s[i] == 0x5C && (s[i+1] == 0x0 || s[i+1] == 0x27 || s[i+1] == 0x22 || s[i+1] == 0x5C))
			strncpy(s + i, s + i + 1, s_len);
	}
	return s;
}

char *wordwrap(const char *s, char *buffer, unsigned int line_limit, unsigned short cut) {
	size_t s_len = strlen(s);
	int i = 0, j = 0, c = 0;
	for ( ; i < s_len; i++, j++) {
		if (c == line_limit) {
			if (s[i] != ' ') {
				if (!cut) while (s[i] != ' ') buffer[j++] = s[i++];
				else { buffer[j++] = s[i]; if (s[i + 1] == ' ') i++; }
				buffer[j] = '\n';
			} else buffer[j] = '\n';
			c = 0;
		} else buffer[j] = s[i];
		c++;
	}
	buffer[j] = '\0';
	return buffer;
}

