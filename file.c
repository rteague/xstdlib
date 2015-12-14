/********************************************************************
 * Name: file.c
 * Author: Rashaud Teague <rashaud.teague@gmail.com>
 * Date: 07/27/2009
 * License: GNU LGPL
 * Description: Extension functions to the standard C Library
 *********************************************************************/

#include "xstdlib.h"

/**
 * Returns trailing name component of path
 * @return buffer containing the basename
 */
char *basename(const char *path, char *buffer) {
	int path_len = (int)strlen(path);
	int i;
	int slash_pos = -1;
	int substr_length = 0;
	for (i = path_len; i >= 0; i--) if (path[i] == '/') { slash_pos = i; break; }
	if (slash_pos == -1) { strcpy(buffer, path); return buffer; }
	
	substr_length = path_len - slash_pos;
	strncpy(buffer, path + slash_pos + 1, substr_length);

	return buffer;
}

/**
 * Copies a file from source to destination
 * @return 1 if the copy is a success, 0 if not
 */
int copy(const char *source, const char *dest) {
	char c;
	int in, out;
	struct stat fs;
	
	if ((in = open(source, O_RDONLY, 0)) == -1) return 0;
	stat(source, &fs);
	
	if ((out = open(dest, O_WRONLY | O_CREAT, fs.st_mode)) == -1) return 0;

	while (read(in, &c, 1) > 0) write(out, &c, 1);

	close(in); close(out);
	
	return 1;
}

long long fappend(const char *path, char *data) {
	FILE *fp;
	if ((fp = fopen(path, "a")) == NULL)
		return -1;
	fputs(data, fp);
	fclose(fp);
	return strlen(data);
}

/**
 * name: file
 * Reads entire file into an array
 * @param const char *filename
 * @param char *elements[]
 * @param unsigned short flags ( FILE_IGNORE_NEW_LINES|FILE_SKIP_EMPTY_LINES )
 * @return int line_count
 */
int file(const char *filename, char *elements[], unsigned char flags) {
	int line_count = 0;
	
	FILE *fp;
	if ((fp = fopen(filename, "r")) == NULL) return -1;

	char *s;
	size_t s_len;
	
	while (!feof(fp)) {
		if ((s = (char *) calloc(LINE_MAX, sizeof(char))) == NULL) {
			fclose(fp);
			return -1;
		}
		
		fgets(s, LINE_MAX, fp);
		s_len = strlen(s);

		if (!(flags & FILE_NO_FLAGS)) {
			if ((flags & FILE_SKIP_EMPTY_LINES) && s_len == 0) {
				free(s); s = NULL; continue;
			}
			
			if ((flags & FILE_IGNORE_NEW_LINES) && s_len > 0)
				if (s[s_len - 1] == '\n') s[s_len - 1] = '\0';
		}
		
		elements[line_count] = s;
		line_count++;
	}
	
	fclose(fp);
	
	return line_count;
}

/**
 * Checks whether a file or directory exists.
 * @return 1 if found, zero if not found
 */
int file_exists(const char *name) {
	struct stat fbuf; if (stat(name, &fbuf) == -1) return 0; return 1;
}

long long file_get_contents(const char * filename, char *buffer, int maxlen) {
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) return -1;

    int c, i = 0;

    while ((c = getc(fp)) != EOF && i < maxlen) { buffer[i] = c; i++; }
    buffer[i] = '\0';

    fclose(fp);

    return i;
}

static void _filesize_dir_walk(const char *path, long int *s) {
	DIR *dir;
	struct dirent *df;
	struct stat fs;
	char _path[PATH_MAX];
	char _dirent_path[PATH_MAX];
	realpath(path, _path);
	if ((dir = opendir(_path)) != NULL) {
		while ((df = readdir(dir)) != NULL) {
			if (strcmp(df->d_name, ".") == 0 || strcmp(df->d_name, "..") == 0) continue;
			sprintf(_dirent_path, "%s/%s", _path, df->d_name);
			if (stat(_dirent_path, &fs) != -1) {
				if (fs.st_mode & S_IFDIR)
					_filesize_dir_walk(_dirent_path, s);
				*s += fs.st_size;
			}
		}
		closedir(dir);
	}
}

/**
 * Returns the size of (in bytes) a given file.
 * @return -1 on error
 */
long long filesize(const char *path) {
	long int size = 0;
	struct stat fbuf;
	if (stat(path, &fbuf) == -1) return -1;
	if (fbuf.st_mode & S_IFDIR) {
		_filesize_dir_walk(path, &size);
		return size;
	}
	return fbuf.st_size;
}

/**
 * Copies a file from source to destination, then removing the source
 * @return 1 if the copy is a success, 0 if not
 */
int move(char *source, char *dest) {
	if (!copy(source, dest)) return 0;
	
	/* after copying the file, remove it */
	if ((unlink(source)) == -1) return 0;
	
	return 1;
}

long long readfile(const char *path, FILE *des) {
	FILE *fp;
	if ((fp = fopen(path, "r")) == NULL) return -1;
	char s[LINE_MAX];
	while (!(feof(fp))) {
		memset(s, 0, LINE_MAX);
		fgets(s, LINE_MAX, fp);/* in --to-> out */fputs(s, des);
		if (ferror(fp) != 0) return -1;
	}
	fclose(fp);
	return filesize(path);
}

