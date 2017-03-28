#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void dropLineFeed(char *msg)
{
	int i = 0;

	for (i = 0; i < strlen(msg); i++) {
		if ('\n' == msg[i]) {
			msg[i] = '\0';
		}
	}
}

bool safeCat(char **dest, size_t *dest_size, char *src, size_t src_size)
{
	bool error = true;

	if (NULL != (*dest = realloc(*dest, *dest_size + src_size + 1))) {
		strcat(*dest, src);
		*dest_size = strlen(*dest);
		error = false;
	}
	return error;
}

void replaceDash(char *msg)
{
	int i = 0;

	for (i = 0; i < strlen(msg); i++) {
		if ('-' == msg[i]) {
			msg[i] = '_';
		}
	}
}

void toUpperCase(char *msg)
{
	int i = 0;

	for (i = 0; i < strlen(msg); i++) {
		msg[i] = (char)toupper(msg[i]);
	}
}
